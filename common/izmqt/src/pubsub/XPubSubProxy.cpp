#include "XPubSubProxy.h"

#include "nzmqt/nzmqt.hpp"

namespace izmqt
{

XPubSubProxy::XPubSubProxy(const QString& frontAddress, const QString& backAddress, QObject *parent) :
    QObject(parent), mFrontAddress(frontAddress), mBackAddress(backAddress)
{
    nzmqt::ZMQContext* context = nzmqt::createDefaultContext(this);
    context->start();

    mFrontSocket = context->createSocket(nzmqt::ZMQSocket::TYP_XPUB);
    connect(mFrontSocket, SIGNAL(messageReceived(const QList<QByteArray>&)),
            SLOT(onPubMessageReceived(const QList<QByteArray>&)));

    mBackSocket = context->createSocket(nzmqt::ZMQSocket::TYP_XSUB);
    connect(mBackSocket, SIGNAL(messageReceived(const QList<QByteArray>&)),
            SLOT(onSubMessageReceived(const QList<QByteArray>&)));
}

int XPubSubProxy::start(const QString &frontAddress, const QString &backAddress)
{
    mFrontAddress = frontAddress;
    mBackAddress = backAddress;

    return start();
}

int XPubSubProxy::start()
{
    if ( isStarted() )
        return true;

    setStarted(false);

    try
    {
        mFrontSocket->bindTo(mFrontAddress);
        mBackSocket->bindTo(mBackAddress);
    }
    catch ( const zmq::error_t &e )
    {
        qDebug() << tr("Proxy '%1' to '%2'> Bind error: ").arg(mFrontAddress).arg(mBackAddress) << e.what();
        return e.num();
    }
    catch ( ... )
    {
        qDebug() << tr("Proxy '%1' to '%2'> Unknown error (bind)").arg(mFrontAddress).arg(mBackAddress);
        return -1;
    }

    setStarted(true);

    return 0;
}

void XPubSubProxy::stop()
{
    if ( !isStarted() )
        return;

    try
    {
        mFrontSocket->unbind(mFrontAddress);
        mBackSocket->unbind(mBackAddress);
    }
    catch ( const zmq::error_t &e )
    {
        qDebug() << tr("Proxy '%1' to '%2'> Unbind error: ").arg(mFrontAddress).arg(mBackAddress) << e.what();
    }
    catch ( ... )
    {
        qDebug() << tr("Proxy '%1' to '%2'> Unknown error (unbind)").arg(mFrontAddress).arg(mBackAddress);
    }

    setStarted(false);
}

void XPubSubProxy::restart()
{
    stop();
    start();
}

void XPubSubProxy::onSubMessageReceived(const QList<QByteArray> &message)
{
    mFrontSocket->sendMessage(message);
}

void XPubSubProxy::onPubMessageReceived(const QList<QByteArray> &message)
{
    mBackSocket->sendMessage(message);
}

}
