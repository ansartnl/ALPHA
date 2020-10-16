#ifndef _QT_TCP_DATAGRAM_CONNECTION_CHECKER_H__
#define _QT_TCP_DATAGRAM_CONNECTION_CHECKER_H__

#include "qttcpdatagramprotocolvariant.h"

#include <QtCore/QObject>
#include <QtCore/QVariant>
class QTimer;


//! Used to send pings.
class QtDatagramConnectionPing : public QObject
{
    Q_OBJECT

public:
    QtDatagramConnectionPing(QObject *parent = 0);
    QtDatagramConnectionPing(QtTcpDatagramProtocolVariant *conn, QObject *parent = 0);

    //! Set reference to variant connection.
    void setDatagramProtocol(QtTcpDatagramProtocolVariant *conn);

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
    QtTcpDatagramProtocolVariant    *mConnection;
    /** Ping timer */
    QTimer  *mPingTimer;
};


//! Controls connection state (connected/disconnected).
class QtDatagramConnectionControler : public QObject
{
    Q_OBJECT

public:
    QtDatagramConnectionControler(QObject *parent = 0);
    QtDatagramConnectionControler(QtTcpDatagramProtocolVariant *conn, QObject *parent = 0);

    //! Set reference to variant connection.
    void setDatagramProtocol(QtTcpDatagramProtocolVariant *conn);

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
    void onReceivedVariantList(QVariantList);

private:
    /** Reference to variant connection */
    QtTcpDatagramProtocolVariant *mConnection;
    /** Disconnect connection timer */
    QTimer              *mTimer;
    /** True when connection is valid */
    bool                mIsConnected;
};

#endif // _QT_TCP_DATAGRAM_CONNECTION_CHECKER_H__
