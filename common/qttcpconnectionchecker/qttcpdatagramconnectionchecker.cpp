#include "qttcpdatagramconnectionchecker.h"
#include "LargeSocket.h"

#include <QtCore/QTimer>
#include <QtNetwork/QTcpSocket>

namespace CheckerProto
{
    struct Ping {};

    inline QDataStream &operator<<(QDataStream &stream, const Ping &v)
    {
        Q_UNUSED(v);
        return stream;
    }
    inline QDataStream &operator>>(QDataStream &stream, Ping &v)
    {
        Q_UNUSED(v);
        return stream;
    }

    static int initPing = qRegisterMetaTypeStreamOperators<Ping>();
}

Q_DECLARE_METATYPE(CheckerProto::Ping)


//////////////////////////////////////////////////////////////////////////////////
// QtDatagramConnectionPing implementation.

QtDatagramConnectionPing::QtDatagramConnectionPing(QObject *parent)
    : QObject(parent), mConnection(0), mPingTimer(0)
{
}

QtDatagramConnectionPing::QtDatagramConnectionPing(QtTcpDatagramProtocolVariant *conn, QObject *parent)
    : QObject(parent), mConnection(0), mPingTimer(0)
{
    setDatagramProtocol(conn);
}

void QtDatagramConnectionPing::setDatagramProtocol(QtTcpDatagramProtocolVariant *conn)
{
    if ( mPingTimer )
        mPingTimer->stop();

    mConnection = conn;

    if ( !mPingTimer )
    {
        mPingTimer = new QTimer(this);
        mPingTimer->setInterval(3000);
        connect(mPingTimer, SIGNAL(timeout()), SLOT(onTimeout()));
    }

    if ( mConnection )
    {
        QTcpSocket *socket = const_cast<QTcpSocket *>( mConnection->socket() );
        if ( qobject_cast<LargeSocket *>(socket) != 0 )
            socket = qobject_cast<LargeSocket *>(socket)->socket();

        connect(socket, SIGNAL(connected()), SLOT(onConnected()));
        connect(socket, SIGNAL(disconnected()), SLOT(onDisconnected()));

        if ( socket->state() == QAbstractSocket::ConnectedState )
            onConnected();
    }
}

void QtDatagramConnectionPing::onTimeout()
{
    if ( !mConnection )
        return;

    QTcpSocket *socket = mConnection->socket();
    if ( qobject_cast<LargeSocket *>(socket) != 0 )
        socket = qobject_cast<LargeSocket *>(socket)->socket();

    // Just do ping
    if ( socket->state() == QAbstractSocket::ConnectedState )
        *mConnection << QVariant::fromValue(CheckerProto::Ping());
}

void QtDatagramConnectionPing::onConnected()
{
    if ( mPingTimer )
        mPingTimer->start();
}

void QtDatagramConnectionPing::onDisconnected()
{
    if ( mPingTimer )
        mPingTimer->stop();
}

void QtDatagramConnectionPing::setInterval(int i)
{
    if ( mPingTimer )
        mPingTimer->setInterval(i);
}


//////////////////////////////////////////////////////////////////////////////////
// QtDatagramConnectionControler implementation.

QtDatagramConnectionControler::QtDatagramConnectionControler(QObject *parent)
    : QObject(parent), mConnection(0), mTimer(0), mIsConnected(false)
{
}

QtDatagramConnectionControler::QtDatagramConnectionControler(QtTcpDatagramProtocolVariant *conn, QObject *parent)
    : QObject(parent), mConnection(0), mTimer(0), mIsConnected(false)
{
    setDatagramProtocol(conn);
}

void QtDatagramConnectionControler::setDatagramProtocol(QtTcpDatagramProtocolVariant *conn)
{
    if ( mTimer )
        mTimer->stop();

    mConnection = conn;

    if ( !mTimer )
    {
        mTimer = new QTimer(this);
        mTimer->setInterval(6000);
        mTimer->setSingleShot(true);
        connect(mTimer, SIGNAL(timeout()), SLOT(onTimeout()));
    }

    if ( mConnection )
    {
        QTcpSocket *socket = const_cast<QTcpSocket *>( mConnection->socket() );
        if ( qobject_cast<LargeSocket *>(socket) != 0 )
            socket = qobject_cast<LargeSocket *>(socket)->socket();

        connect(socket, SIGNAL(connected()), SLOT(onConnected()));
        connect(mConnection, SIGNAL(newMessages(QVariantList)), SLOT(onReceivedVariantList(QVariantList)));
    }
}

void QtDatagramConnectionControler::onTimeout()
{
    onDisconnected();
}

void QtDatagramConnectionControler::onConnected()
{
    if ( mTimer )
        mTimer->stop();

    if ( !mIsConnected )
    {
        QTcpSocket *socket = const_cast<QTcpSocket *>( mConnection->socket() );
        if ( qobject_cast<LargeSocket *>(socket) != 0 )
            socket = qobject_cast<LargeSocket *>(socket)->socket();

        connect(socket, SIGNAL(disconnected()), SLOT(onDisconnected()));

        mIsConnected = true;

        emit connected();
    }

    if ( mTimer )
        mTimer->start();
}

void QtDatagramConnectionControler::onDisconnected()
{
    if ( mTimer )
        mTimer->stop();

    if ( mIsConnected )
    {
        mIsConnected = false;
        if ( mConnection )
        {
            QTcpSocket *socket = const_cast<QTcpSocket *>( mConnection->socket() );
            if ( qobject_cast<LargeSocket *>(socket) != 0 )
                socket = qobject_cast<LargeSocket *>(socket)->socket();

            if ( socket->state() != QAbstractSocket::UnconnectedState )
            {
                disconnect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
                socket->disconnectFromHost();
                socket->waitForDisconnected(2000);
            }
        }
        emit disconnected();
    }
}

void QtDatagramConnectionControler::setInterval(int i)
{
    if ( mTimer )
        mTimer->setInterval(i);
}

void QtDatagramConnectionControler::onReceivedVariantList(QVariantList msgs)
{
    foreach ( const QVariant &v, msgs )
    {
        if ( v.canConvert<CheckerProto::Ping>() )
        {
            onConnected();
            break;
        }
    }
}
