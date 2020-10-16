#include "PublishQueue.h"

#include "nzmqt/nzmqt.hpp"

#include <QDebug>
#include <QList>
#include <QByteArray>
#include <QDataStream>

namespace izmqt
{

PublishQueue::PublishQueue(const QString& address, const QString &topic, int mode, QObject* parent)
    : QObject(parent), ModeProperty(mode), mAddress(address), mTopic(topic)
{
    nzmqt::ZMQContext* context = nzmqt::createDefaultContext(this);
    context->start();

    mSocket = context->createSocket(nzmqt::ZMQSocket::TYP_PUB);
}

int PublishQueue::start()
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
        qDebug() << tr("Publisher '%1'> Error: ").arg(mAddress) << e.what();
        return e.num();
    }
    catch ( ... )
    {
        qDebug() << tr("Publisher '%1'> Unknown error (start)").arg(mAddress);
        return -1;
    }

    setStarted(true);

    return 0;
}

int PublishQueue::start(const QString& address, const QString &topic)
{
    mAddress = address;
    mTopic = topic;

    return start();
}

void PublishQueue::stop()
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
        qDebug() << tr("Publisher '%1'> Error: ").arg(mAddress) << e.what();
    }
    catch ( ... )
    {
        qDebug() << tr("Publisher '%1'> Unknown error (stop)").arg(mAddress);
    }

    setStarted(false);
}

void PublishQueue::publish(QVariant v)
{
    publish(mTopic, v);
}

void PublishQueue::publish(const QString &topic, QVariant v)
{
    if ( !isStarted() )
        return;

    // Variant -> byte array
    QByteArray message;
    QDataStream stream(&message, QIODevice::WriteOnly);
    stream << v;

    try
    {
        if ( topic.isEmpty() )
        {
            mSocket->sendMessage( message );
        }
        else
        {
            QList< QByteArray > msg;
            msg += topic.toLocal8Bit();
            msg += message;

            mSocket->sendMessage(msg);
        }
    }
    catch ( const zmq::error_t &e )
    {
        qDebug() << tr("Publisher '%1'> Publish error: ").arg(mAddress) << e.what();
    }
    catch ( ... )
    {
        qDebug() << tr("Publisher '%1'> Unknown error (publish)").arg(mAddress);
    }
}

} // namespace izmqt
