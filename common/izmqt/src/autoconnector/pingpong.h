#ifndef IZMQT_PING_PONG_H
#define IZMQT_PING_PONG_H

#include <QObject>
#include <QVariant>
#include <QSharedPointer>
#include <QString>

#include "../izmq_global.h"

class QTimer;


namespace izmqt {

//! Specific to protocol ping/ping interface - accessor.
class IProtoPingAccessor {
public:
    virtual ~IProtoPingAccessor() {}

    virtual QVariant ping() const = 0;
    virtual QVariant pong() const = 0;

    virtual bool isPing(const QVariant&) const = 0;
    virtual bool isPong(const QVariant&) const = 0;
};
typedef QSharedPointer<IProtoPingAccessor>  TProtoPingAccessorPointer;


//! Used to receive pings and send pongs (replies).
class IZMQT_EXPORT Ponger : public QObject {
    Q_OBJECT

public:
    Ponger(QObject* parent = 0);

    void setAccessor(TProtoPingAccessorPointer acc);

signals:
    /*! Sends pong with user data.
     * \param v Pong proto value
     *  \param userData Specific user data
    */
    void pong(QVariant v, QVariant userData = QVariant());

public slots:
    /*! Receives message.
     * \param msg Message data
     *  \param userData Specific user data
    */
    void onMessageReceived(QVariant msg, QVariant userData = QVariant());

private:
    /** Ping/pong parser/generator */
    TProtoPingAccessorPointer   mProtoPingAccessor;
};


//! Controls connection state (connected/disconnected).
class IZMQT_EXPORT Pinger : public QObject {
    Q_OBJECT

public:
    Pinger(QObject* parent = 0);

    void setAccessor(TProtoPingAccessorPointer acc);

    //! Set timeout interval for ping.
    /*!
        \param i Interval for sending pings (msecs). Default = 3 sec.
    */
    void setInterval(int i);

    //! Start monitoring.
    void start();

    //! Stop ping sender.
    void stop();

    //! Returns true if connection is valid.
    bool isConnected() const { return mIsConnected; }

signals:
    void ping(QVariant);

    void connected();
    void disconnected();

public slots:
    void onPongReceived(QVariant);

private slots:
    void onTimeout();

private:
    /** Ping timer */
    QTimer*  mPingTimer;
    /** True when connection is valid */
    bool    mIsConnected;
    /** True when pong message was received (means connection is valid) */
    bool    mIsPongReceived;
    /** Ping/pong parser/generator */
    TProtoPingAccessorPointer   mProtoPingAccessor;
};

}   // namespace izmqt

#endif // IZMQT_PING_PONG_H
