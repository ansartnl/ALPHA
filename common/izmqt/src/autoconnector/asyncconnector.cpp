#include "asyncconnector.h"
#include "pingpong.h"

#include "asyncreqrep/RouterServer.h"
#include "asyncreqrep/DealerClient.h"

#include <QtCore/QTimer>
#include <QtCore/QDateTime>
#include <QtCore/QStringList>
#include <QtNetwork/QTcpSocket>


namespace izmqt
{

//////////////////////////////////////////////////////////////////////////////////
// RouterPonger implementation.

RouterPonger::RouterPonger(RouterServer *router, QObject *parent)
    : QObject(parent), mRouterServer(router), mPonger(0)
{
    Q_ASSERT(mRouterServer);

    mDisconnectTimer = new QTimer(this);
    mDisconnectTimer->setInterval(5000);
    connect(mDisconnectTimer, SIGNAL(timeout()), SLOT(onTimeout()));

    mPonger = new Ponger(this);
    connect(mRouterServer, SIGNAL(requestReceived(QVariant, QString)),
            SLOT(onRequestReceived(QVariant, QString)));
    connect(mPonger, SIGNAL(pong(QVariant, QVariant)),
            SLOT(onPongReceived(QVariant, QVariant)));
}

void RouterPonger::setInterval(int i)
{
    mDisconnectTimer->setInterval(i);
}

void RouterPonger::setPingAccessor(IProtoPingAccessor *acc)
{
    mPonger->setAccessor( TProtoPingAccessorPointer(acc) );
}

void RouterPonger::onPongReceived(QVariant v, QVariant identity)
{
    QString id = identity.toString();

    mRouterServer->reply(v, id);

    // Send connected signal for the newly connected queue.
    if ( !mConnectedQueues.contains(id) )
        emit connected(id);
    mConnectedQueues[id] = QDateTime::currentDateTime();

    // Run timer only when it is stopped.
    if ( !mDisconnectTimer->isActive() )
        mDisconnectTimer->start();
}

void RouterPonger::onRequestReceived(QVariant v, QString identity)
{
    mPonger->onMessageReceived(v, identity);
}

void RouterPonger::onTimeout()
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    int disconnectInterval = mDisconnectTimer->interval() / 1000;

    QStringList disconnectList;
    // Find list of connections that do not seem to be alive.
    for ( TConnectedMap::const_iterator itc = mConnectedQueues.constBegin(); itc != mConnectedQueues.constEnd(); ++itc )
    {
        const QDateTime &dt = *itc;
        if ( dt.secsTo(currentDateTime) >= disconnectInterval )
            disconnectList << itc.key();
    }

    // Send disconnected signal with the queue identifier which connection is not alive any more.
    foreach ( const QString &identity, disconnectList )
    {
        mConnectedQueues.remove(identity);
        emit disconnected(identity);
    }

    // When nothing to monitor - stop the timer.
    if ( mConnectedQueues.empty() )
        mDisconnectTimer->stop();
}


//////////////////////////////////////////////////////////////////////////////////
// DealerPinger implementation.

DealerPinger::DealerPinger(DealerClient *client, QObject *parent)
    : QObject(parent), mDealerClient(client), mPinger(0)
{
    Q_ASSERT(mDealerClient);

    mPinger = new Pinger(this);
    connect(mPinger, SIGNAL(ping(QVariant)), mDealerClient, SLOT(request(QVariant)));
    connect(mDealerClient, SIGNAL(replyReceived(QVariant)), mPinger, SLOT(onPongReceived(QVariant)));

    connect(mPinger, SIGNAL(connected()), SIGNAL(connected()));
    connect(mPinger, SIGNAL(disconnected()), SLOT(onDisconnected()));
}

void DealerPinger::setPingAccessor(IProtoPingAccessor *acc)
{
    mPinger->setAccessor( TProtoPingAccessorPointer(acc) );
}

void DealerPinger::setInterval(int i)
{
    mPinger->setInterval(i);
}

bool DealerPinger::isConnected() const
{
    return mPinger->isConnected();
}

void DealerPinger::start()
{
    mPinger->start();
}

void DealerPinger::stop()
{
    mPinger->stop();
}

void DealerPinger::onDisconnected()
{
    mDealerClient->restart();

    // We assume direct connection here.
    // On the 'connector' side dealerClient must be updated.
    emit disconnected();
}

}   // namespace izmqt
