#include "qttcpconnectionchecker.h"

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

    static int iPing = qRegisterMetaTypeStreamOperators<Ping>();
}

Q_DECLARE_METATYPE(CheckerProto::Ping)


//////////////////////////////////////////////////////////////////////////////////
// QtConnectionPing implementation.

QtConnectionPing::QtConnectionPing(QObject *parent)
    : QObject(parent), mConnection(0), mPingTimer(0)
{
}

QtConnectionPing::QtConnectionPing(QtVariantConnection *conn, QObject *parent)
    : QObject(parent), mConnection(0), mPingTimer(0)
{
    setVariantConnection(conn);
}

void QtConnectionPing::setVariantConnection(QtVariantConnection *conn)
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
        connect(socket, SIGNAL(connected()), SLOT(onConnected()));
        connect(socket, SIGNAL(disconnected()), SLOT(onDisconnected()));

        if ( socket->state() == QAbstractSocket::ConnectedState )
            onConnected();
    }
}

void QtConnectionPing::onTimeout()
{
    // Just do ping
    if ( mConnection && mConnection->socket()->state() == QAbstractSocket::ConnectedState )
    {
        mConnection->sendVariant( QVariant::fromValue(CheckerProto::Ping()) );
    }
}

void QtConnectionPing::onConnected()
{
    if ( mPingTimer )
        mPingTimer->start();
}

void QtConnectionPing::onDisconnected()
{
    if ( mPingTimer )
        mPingTimer->stop();
}

void QtConnectionPing::setInterval(int i)
{
    if ( mPingTimer )
        mPingTimer->setInterval(i);
}


//////////////////////////////////////////////////////////////////////////////////
// QtConnectionControler implementation.

QtConnectionControler::QtConnectionControler(QObject *parent)
    : QObject(parent), mConnection(0), mTimer(0), mIsConnected(false)
{
}

QtConnectionControler::QtConnectionControler(QtVariantConnection *conn, QObject *parent)
    : QObject(parent), mConnection(0), mTimer(0), mIsConnected(false)
{
    setVariantConnection(conn);
}

void QtConnectionControler::setVariantConnection(QtVariantConnection *conn)
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
        connect(socket, SIGNAL(connected()), SLOT(onConnected()));
        connect(mConnection, SIGNAL(gotVariant(const QVariant &)), SLOT(onReceivedVariant(const QVariant &)));
    }
}

void QtConnectionControler::onTimeout()
{
    onDisconnected();
}

void QtConnectionControler::onConnected()
{
    if ( mTimer )
        mTimer->stop();

    if ( !mIsConnected )
    {
        QTcpSocket *socket = const_cast<QTcpSocket *>( mConnection->socket() );
        connect(socket, SIGNAL(disconnected()), SLOT(onDisconnected()));

        emit connected();
    }

    if ( mTimer )
        mTimer->start();

    mIsConnected = true;
}

void QtConnectionControler::onDisconnected()
{
    if ( mTimer )
        mTimer->stop();

    if ( mIsConnected )
    {
        mIsConnected = false;
        if ( mConnection )
        {
            QTcpSocket *socket = const_cast<QTcpSocket *>( mConnection->socket() );
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

void QtConnectionControler::setInterval(int i)
{
    if ( mTimer )
        mTimer->setInterval(i);
}

void QtConnectionControler::onReceivedVariant(const QVariant &v)
{
    if ( v.canConvert<CheckerProto::Ping>() )
    {
        onConnected();
    }
}
