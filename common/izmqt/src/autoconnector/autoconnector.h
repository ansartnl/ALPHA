#ifndef IZMQT_AUTO_CONNECTOR_H
#define IZMQT_AUTO_CONNECTOR_H

#include <QObject>
#include <QVariant>
#include "../izmq_global.h"

class QTimer;


namespace izmqt {

class ReplyQueue;

//! Used to receive pings and send pongs (replies).
class IZMQT_EXPORT ConnectionPoint : public QObject {
    Q_OBJECT

public:
    ConnectionPoint(QObject* parent = 0);

    //! Binds connection point to specified address.
    bool start(const QString& address);

    //! Unbinds connection point.
    void stop();

private slots:
    void onRequestReceived(QVariant);

private:
    /** Reply server queue */
    ReplyQueue* mReplyServer;
};


class RequestQueue;

//! Controls connection state (disconnected).
class IZMQT_EXPORT AutoConnector : public QObject {
    Q_OBJECT

public:
    AutoConnector(QObject* parent = 0);

    //! Set timeout interval for ping.
    /*!
        \param i Interval for sending pings. Default = 3 sec.
    */
    void setInterval(int i);

    //! Start connection controller for specified address.
    bool start(const QString& address);

    //! Stop watching and restoring the connection.
    void stop();

    //! Returns true if connection is valid.
    bool isConnected() const { return mIsConnected; }

signals:
    void connected();
    void disconnected();

private slots:
    void onReplyReceived(QVariant);
    void onTimeout();

private:
    /** Ping timer */
    QTimer*  mPingTimer;
    /** True when connection is valid */
    bool    mIsConnected;
    /** True when pong message was received (means connection is valid) */
    bool    mIsPongReceived;
    /** Address of connection point */
    QString mAddress;
    /** Request client queue */
    RequestQueue*   mRequestClient;
};

}   // namespace izmqt

#endif // IZMQT_AUTO_CONNECTOR_H
