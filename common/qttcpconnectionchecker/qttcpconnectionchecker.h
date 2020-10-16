#ifndef _QT_TCP_CONNECTION_CHECKER_H__
#define _QT_TCP_CONNECTION_CHECKER_H__

#include "qtvariantconnection.h"

#include <QtCore/QObject>
#include <QtCore/QVariant>
class QTimer;


//! Used to send pings.
class QtConnectionPing : public QObject
{
    Q_OBJECT

public:
    QtConnectionPing(QObject *parent = 0);
    QtConnectionPing(QtVariantConnection *conn, QObject *parent = 0);

    //! Set reference to variant connection.
    void setVariantConnection(QtVariantConnection *conn);

    //! Set timeout interval for ping.
    /*!
        \param i Interval for sending pings. Default = 3 sec.
    */
    void setInterval(int i);

private slots:
    void onTimeout();
    void onConnected();
    void onDisconnected();

private:
    /** Reference to variant connection */
    QtVariantConnection *mConnection;
    /** Ping timer */
    QTimer              *mPingTimer;
};


//! Controls connection state (connected/disconnected).
class QtConnectionControler : public QObject
{
    Q_OBJECT

public:
    QtConnectionControler(QObject *parent = 0);
    QtConnectionControler(QtVariantConnection *conn, QObject *parent = 0);

    //! Set reference to variant connection.
    void setVariantConnection(QtVariantConnection *conn);

    //! Returns true if connection is valid.
    bool isConnected() const { return mIsConnected; }

    //! Set timeout interval for disconnect procedure.
    /*!
        \param i Interval for sending pings. Default = 6 sec.
    */
    void setInterval(int i);

signals:
    void connected();
    void disconnected();

private slots:
    void onTimeout();
    void onConnected();
    void onDisconnected();
    void onReceivedVariant(const QVariant &v);

private:
    /** Reference to variant connection */
    QtVariantConnection *mConnection;
    /** Disconnect connection timer */
    QTimer              *mTimer;
    /** True when connection is valid */
    bool                mIsConnected;
};

#endif // _QT_TCP_CONNECTION_CHECKER_H__
