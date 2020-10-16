#include "autoconnector.h"

#include "reqrep/ReplyQueue.h"
#include "reqrep/RequestQueue.h"

#include <QtCore/QTimer>
#include <QtNetwork/QTcpSocket>
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
// ConnectionPoint implementation.

ConnectionPoint::ConnectionPoint(QObject* parent)
    : QObject(parent), mReplyServer(0) {
    //    Q_UNUSED(CheckerProto::pingPong);

    mReplyServer = new ReplyQueue(QString(), SERVER, this);
    connect(mReplyServer, SIGNAL(requestReceived(QVariant)), SLOT(onRequestReceived(QVariant)));
}

bool ConnectionPoint::start(const QString& address) {
    return mReplyServer->start(address);
}

void ConnectionPoint::stop() {
    mReplyServer->stop();
}

void ConnectionPoint::onRequestReceived(QVariant v) {
    if (v.canConvert<CheckerProto::Ping>()) {
        mReplyServer->reply(QVariant::fromValue(CheckerProto::Pong()));
    }
}


//////////////////////////////////////////////////////////////////////////////////
// ConnectionControler implementation.

AutoConnector::AutoConnector(QObject* parent)
    : QObject(parent), mPingTimer(0), mIsConnected(false), mIsPongReceived(true), mRequestClient(0) {
    mRequestClient = new RequestQueue(QString(), CLIENT, this);
    connect(mRequestClient, SIGNAL(replyReceived(QVariant)), SLOT(onReplyReceived(QVariant)));

    mPingTimer = new QTimer(this);
    mPingTimer->setInterval(3000);
    connect(mPingTimer, SIGNAL(timeout()), SLOT(onTimeout()));
}

void AutoConnector::setInterval(int i) {
    if (mPingTimer)
        mPingTimer->setInterval(i);
}

bool AutoConnector::start(const QString& address) {
    mAddress = address;
    mIsPongReceived = true;
    mIsConnected = false;

    bool started =  mRequestClient->start(address);

    // Run immediate pinger.
    QTimer::singleShot(0, this, SLOT(onTimeout()));
    // Run constant pinger.
    mPingTimer->start();

    return started;
}

void AutoConnector::stop() {
    mPingTimer->stop();
    mRequestClient->stop();
}

void AutoConnector::onTimeout() {
    if (!mIsPongReceived) {
        bool isConnectedOld = mIsConnected;
        mIsConnected = false;
        mPingTimer->stop();

        // Lazy pirate method for reseting the queue.
        mRequestClient->stop();
        mRequestClient->disconnect(this);
        mRequestClient->deleteLater();
        mRequestClient = new RequestQueue(QString(), CLIENT, this);
        connect(mRequestClient, SIGNAL(replyReceived(QVariant)), SLOT(onReplyReceived(QVariant)));

        if (isConnectedOld)
            emit disconnected();

        mRequestClient->start(mAddress);

        // Run constant pinger.
        mPingTimer->start();
    }

    mIsPongReceived = false;
    mRequestClient->request(QVariant::fromValue(CheckerProto::Ping()));
}

void AutoConnector::onReplyReceived(QVariant v) {
    if (v.canConvert<CheckerProto::Pong>()) {
        if (!mIsConnected)
            emit connected();

        mIsPongReceived = true;
        mIsConnected = true;
    }
}

}   // namespace izmqt
