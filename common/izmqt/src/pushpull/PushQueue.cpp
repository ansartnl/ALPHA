#include "PushQueue.h"

#include "nzmqt/nzmqt.hpp"

#include <QDebug>
#include <QList>
#include <QByteArray>
#include <QDataStream>

namespace izmqt
{

PushQueue::PushQueue(const QString& address, int mode, QObject* parent)
    : QObject(parent), ModeProperty(mode), mAddress(address)
{
    nzmqt::ZMQContext* context = nzmqt::createDefaultContext(this);
    context->start();

    mSocket = context->createSocket(nzmqt::ZMQSocket::TYP_PUSH);
}

void PushQueue::setQueueSize(qint32 hwm)
{
    mSocket->setOption(nzmqt::ZMQSocket::OPT_SNDHWM, hwm);
}

int PushQueue::start()
{
    if ( isStarted() )
        return true;

    setStarted(false);

    try
    {
        if ( mode() == izmqt::CLIENT )
            mSocket->connectTo(mAddress);

        else if ( mode() == izmqt::SERVER )
            mSocket->bindTo(mAddress);

        else
            Q_ASSERT(false);
    }
    catch ( const zmq::error_t &e )
    {
        qDebug() << tr("PushQueue '%1'> Error: ").arg(mAddress) << e.what();
        return e.num();
    }
    catch ( ... )
    {
        qDebug() << tr("PushQueue '%1'> Unknown error (start)").arg(mAddress);
        return -1;
    }

    setStarted(true);

    return 0;
}

int PushQueue::start(const QString& address)
{
    mAddress = address;

    return start();
}

void PushQueue::stop()
{
    if ( !isStarted() )
        return;

    try
    {
        if ( mode() == izmqt::CLIENT )
            mSocket->disconnectFromHost(mAddress);

        else if ( mode() == izmqt::SERVER )
            mSocket->unbind(mAddress);

        else
            Q_ASSERT(false);
    }
    catch ( const zmq::error_t &e )
    {
        qDebug() << tr("PushQueue '%1'> Error: ").arg(mAddress) << e.what();
    }
    catch ( ... )
    {
        qDebug() << tr("PushQueue '%1'> Unknown error (stop)").arg(mAddress);
    }

    setStarted(false);
}

void PushQueue::push(QVariant v)
{
    if ( !isStarted() )
        return;

    // Variant -> byte array
    QByteArray message;
    QDataStream stream(&message, QIODevice::WriteOnly);
    stream << v;

    try
    {
        mSocket->sendMessage( message );
    }
    catch ( const zmq::error_t &e )
    {
        qDebug() << tr("PushQueue '%1'> Push error: ").arg(mAddress) << e.what();
    }
    catch ( ... )
    {
        qDebug() << tr("PushQueue '%1'> Unknown error (push)").arg(mAddress);
    }
}

} // namespace izmqt
