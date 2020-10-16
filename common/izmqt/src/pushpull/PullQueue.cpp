#include "PullQueue.h"

#include "nzmqt/nzmqt.hpp"

#include <QDebug>
#include <QDataStream>

namespace izmqt
{

PullQueue::PullQueue(const QString& address, int mode, QObject *parent)
    : QObject(parent), ModeProperty(mode), mAddress(address)
{
    nzmqt::ZMQContext* context = nzmqt::createDefaultContext(this);
    context->start();

    mSocket = context->createSocket(nzmqt::ZMQSocket::TYP_PULL);
    connect(mSocket, SIGNAL(messageReceived(const QList<QByteArray>&)),
            SLOT(onMessageReceived(const QList<QByteArray>&)));
}

void PullQueue::setQueueSize(qint32 hwm)
{
    mSocket->setOption(nzmqt::ZMQSocket::OPT_RCVHWM, hwm);
}

int PullQueue::start()
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
        qDebug() << tr("PullQueue '%1'> Error: ").arg(mAddress) << e.what();
        return e.num();
    }
    catch ( ... )
    {
        qDebug() << tr("PullQueue '%1'> Unknown error (start)").arg(mAddress);
        return -1;
    }

    setStarted(true);

    return 0;
}

int PullQueue::start(const QString& address)
{
    mAddress = address;

    return start();
}

void PullQueue::stop()
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
        qDebug() << tr("PullQueue '%1'> Error: ").arg(mAddress) << e.what();
    }
    catch ( ... )
    {
        qDebug() << tr("PullQueue '%1'> Unknown error (stop)").arg(mAddress);
    }

    setStarted(false);
}

void PullQueue::onMessageReceived(const QList<QByteArray>& message)
{
    // We do not wait multi-part messages when pulling.
    if ( message.empty() || message.size() > 1 )
        return;

    QByteArray m = message[0];

    // Byte array -> variant
    QVariant v;
    QDataStream stream(&m, QIODevice::ReadOnly);
    stream >> v;

    if ( !v.isNull() && v.isValid() )
        emit readyRead(v);
}

} // namespace izmqt
