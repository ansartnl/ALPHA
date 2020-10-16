#include "MessageQueueBroker.h"

#include "nzmqt/nzmqt.hpp"

namespace izmqt
{

MessageQueueBroker::MessageQueueBroker(const QString& frontAddress, const QString& backAddress, QObject *parent) :
    QObject(parent), mFrontAddress(frontAddress), mBackAddress(backAddress)
{
    nzmqt::ZMQContext* context = nzmqt::createDefaultContext(this);
    context->start();

    mFrontSocket = context->createSocket(nzmqt::ZMQSocket::TYP_ROUTER);
    connect(mFrontSocket, SIGNAL(messageReceived(const QList<QByteArray>&)),
            SLOT(onRequestReceived(const QList<QByteArray>&)));

    mBackSocket = context->createSocket(nzmqt::ZMQSocket::TYP_DEALER);
    connect(mBackSocket, SIGNAL(messageReceived(const QList<QByteArray>&)),
            SLOT(onReplyReceived(const QList<QByteArray>&)));
}

int MessageQueueBroker::start(const QString& frontAddress, const QString& backAddress)
{
    mFrontAddress = frontAddress;
    mBackAddress = backAddress;

    return start();
}

int MessageQueueBroker::start()
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
        qDebug() << tr("Broker '%1' to '%2'> Bind error: ").arg(mFrontAddress).arg(mBackAddress) << e.what();
        return e.num();
    }
    catch ( ... )
    {
        qDebug() << tr("Broker '%1' to '%2'> Unknown error (bind)").arg(mFrontAddress).arg(mBackAddress);
        return -1;
    }

    setStarted(true);

    return 0;
}

void MessageQueueBroker::stop()
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
        qDebug() << tr("Broker '%1' to '%2'> Unbind error: ").arg(mFrontAddress).arg(mBackAddress) << e.what();
    }
    catch ( ... )
    {
        qDebug() << tr("Broker '%1' to '%2'> Unknown error (unbind)").arg(mFrontAddress).arg(mBackAddress);
    }

    setStarted(false);
}

void MessageQueueBroker::restart()
{
    stop();
    start();
}

void MessageQueueBroker::onRequestReceived(const QList<QByteArray> &request)
{
    mBackSocket->sendMessage(request);
}

void MessageQueueBroker::onReplyReceived(const QList<QByteArray> &reply)
{
    mFrontSocket->sendMessage(reply);
}

}
