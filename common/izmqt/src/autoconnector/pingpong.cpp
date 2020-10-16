#include "pingpong.h"

#include <QtCore/QTimer>
#include <QDataStream>


namespace CheckerProto {
struct Ping {};
struct Pong {};

inline QDataStream& operator<<(QDataStream& stream, const Ping& v) {
    Q_UNUSED(v);
    return stream;
}
inline QDataStream& operator>>(QDataStream& stream, Ping& v) {
    Q_UNUSED(v);
    return stream;
}

inline QDataStream& operator<<(QDataStream& stream, const Pong& v) {
    Q_UNUSED(v);
    return stream;
}
inline QDataStream& operator>>(QDataStream& stream, Pong& v) {
    Q_UNUSED(v);
    return stream;
}

static int pingPong[] = {
    qRegisterMetaTypeStreamOperators<Ping>(),
    qRegisterMetaTypeStreamOperators<Pong>()
};
}

Q_DECLARE_METATYPE(CheckerProto::Ping)
Q_DECLARE_METATYPE(CheckerProto::Pong)


namespace izmqt {

//////////////////////////////////////////////////////////////////////////////////
// VariantPingAccessor implementation.

class VariantPingAccessor : public IProtoPingAccessor {
public:
    virtual QVariant ping() const {
        return QVariant::fromValue(CheckerProto::Ping());
    }
    virtual QVariant pong() const {
        return QVariant::fromValue(CheckerProto::Pong());
    }

    virtual bool isPing(const QVariant& v) const {
        return v.canConvert<CheckerProto::Ping>();
    }
    virtual bool isPong(const QVariant& v) const {
        return v.canConvert<CheckerProto::Pong>();
    }
};


//////////////////////////////////////////////////////////////////////////////////
// Ponger implementation.

Ponger::Ponger(QObject* parent)
    : QObject(parent) {
    mProtoPingAccessor = TProtoPingAccessorPointer(new VariantPingAccessor);
}

void Ponger::setAccessor(TProtoPingAccessorPointer acc) {
    if (!acc || acc == mProtoPingAccessor)
        return;
    mProtoPingAccessor = acc;
}

void Ponger::onMessageReceived(QVariant msg, QVariant userData) {
    if (mProtoPingAccessor->isPing(msg)) {
        emit pong(mProtoPingAccessor->pong(), userData);
    }
}


//////////////////////////////////////////////////////////////////////////////////
// ConnectionControler implementation.

Pinger::Pinger(QObject* parent)
    : QObject(parent), mPingTimer(0), mIsConnected(false), mIsPongReceived(true) {
    mProtoPingAccessor = TProtoPingAccessorPointer(new VariantPingAccessor);

    mPingTimer = new QTimer(this);
    mPingTimer->setInterval(3000);
    connect(mPingTimer, SIGNAL(timeout()), SLOT(onTimeout()));
}

void Pinger::setAccessor(TProtoPingAccessorPointer acc) {
    if (!acc || acc == mProtoPingAccessor)
        return;
    mProtoPingAccessor = acc;
}

void Pinger::setInterval(int i) {
    if (mPingTimer)
        mPingTimer->setInterval(i);
}

void Pinger::start() {
    mIsPongReceived = true;
    mIsConnected = false;

    // Run immediate pinger.
    QTimer::singleShot(0, this, SLOT(onTimeout()));
    // Run constant pinger.
    mPingTimer->start();
}

void Pinger::stop() {
    mPingTimer->stop();
}

void Pinger::onTimeout() {
    if (!mIsPongReceived) {
        bool isConnectedOld = mIsConnected;
        mIsConnected = false;
        mPingTimer->stop();

        // We assume here that connect will be created with Qt::DirectConnection type.
        if (isConnectedOld)
            emit disconnected();

        // Run constant pinger.
        mPingTimer->start();
    }

    mIsPongReceived = false;
    emit ping(mProtoPingAccessor->ping());
}

void Pinger::onPongReceived(QVariant v) {
    if (mProtoPingAccessor->isPong(v)) {
        if (!mIsConnected)
            emit connected();

        mIsPongReceived = true;
        mIsConnected = true;
    }
}

}   // namespace izmqt
