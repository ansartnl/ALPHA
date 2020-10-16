#include "ReplyQueue.h"

#include "nzmqt/nzmqt.hpp"

#include <QDebug>
#include <QDataStream>

namespace izmqt
{

ReplyQueue::ReplyQueue(const QString& address, int mode, QObject* parent)
    : QObject(parent), ModeProperty(mode), mAddress(address)
{
    nzmqt::ZMQContext* context = nzmqt::createDefaultContext(this);
    context->start();

    mSocket = context->createSocket(nzmqt::ZMQSocket::TYP_REP);
    connect(mSocket, SIGNAL(messageReceived(const QList<QByteArray>&)),
            SLOT(onRequestReceived(const QList<QByteArray>&)));
}

int ReplyQueue::start()
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
        qDebug() << tr("Replyer '%1'> Error: ").arg(mAddress) << e.what();
        return e.num();
    }
    catch ( ... )
    {
        qDebug() << tr("Replyer '%1'> Unknown error (start)").arg(mAddress);
        return -1;
    }

    setStarted(true);

    return 0;
}

int ReplyQueue::start(const QString& address)
{
    mAddress = address;

    return start();
}

void ReplyQueue::stop()
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
        qDebug() << tr("Replyer '%1'> Error: ").arg(mAddress) << e.what();
    }
    catch ( ... )
    {
        qDebug() << tr("Replyer '%1'> Unknown error (stop)").arg(mAddress);
    }

    setStarted(false);
}

void ReplyQueue::restart()
{
    stop();
    start();
}

void ReplyQueue::onRequestReceived(const QList<QByteArray>& request)
{
    // We assume that our message is not a multi-part message.
    // We wait only one byte array in the list.
    foreach ( QByteArray m, request )
    {
        // Byte array -> variant
        QVariant v;
        QDataStream stream(&m, QIODevice::ReadOnly);
        stream >> v;

        if ( !v.isNull() && v.isValid() )
            emit requestReceived(v);
    }
}

void ReplyQueue::reply(QVariant v)
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
        qDebug() << tr("Replyer '%1'> Reply error: ").arg(mAddress) << e.what();
    }
    catch ( ... )
    {
        qDebug() << tr("Replyer '%1'> Unknown error (reply)").arg(mAddress);
    }
}

} // namespace izmqt
