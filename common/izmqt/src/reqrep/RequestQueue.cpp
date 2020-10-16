#include "RequestQueue.h"

#include "nzmqt/nzmqt.hpp"

#include <QDebug>
#include <QDataStream>

namespace izmqt
{

RequestQueue::RequestQueue(const QString& address, int mode, QObject *parent)
    : QObject(parent), ModeProperty(mode), mAddress(address)
{
    nzmqt::ZMQContext* context = nzmqt::createDefaultContext(this);
    context->start();

    mSocket = context->createSocket(nzmqt::ZMQSocket::TYP_REQ);
    connect(mSocket, SIGNAL(messageReceived(const QList<QByteArray>&)),
            SLOT(onReplyReceived(const QList<QByteArray>&)));
}

int RequestQueue::start()
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
        qDebug() << tr("Requester '%1'> Error: ").arg(mAddress) << e.what();
        return e.num();
    }
    catch ( ... )
    {
        qDebug() << tr("Requester '%1'> Unknown error (start)").arg(mAddress);
        return -1;
    }

    setStarted(true);

    return 0;
}

int RequestQueue::start(const QString& address)
{
    mAddress = address;

    return start();
}

void RequestQueue::stop()
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
        qDebug() << tr("Requester '%1'> Error: ").arg(mAddress) << e.what();
    }
    catch ( ... )
    {
        qDebug() << tr("Requester '%1'> Unknown error (stop)").arg(mAddress);
    }

    setStarted(false);
}

void RequestQueue::restart()
{
    stop();
    start();
}

void RequestQueue::onReplyReceived(const QList<QByteArray>& request)
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
            emit replyReceived(v);
    }
}

void RequestQueue::request(QVariant v)
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
        qDebug() << tr("Requester '%1'> Request error: ").arg(mAddress) << e.what();
    }
    catch ( ... )
    {
        qDebug() << tr("Requester '%1'> Unknown error (request)").arg(mAddress);
    }
}

} // namespace izmqt
