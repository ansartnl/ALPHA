#include "DealerClient.h"

#include "nzmqt/nzmqt.hpp"

#include <QDebug>
#include <QDataStream>

namespace izmqt
{

DealerClient::DealerClient(const QString& address, const QString &identity, QObject *parent)
    : QObject(parent), mAddress(address)
{
    nzmqt::ZMQContext* context = nzmqt::createDefaultContext(this);
    context->start();

    mSocket = context->createSocket(nzmqt::ZMQSocket::TYP_DEALER);
    connect(mSocket, SIGNAL(messageReceived(const QList<QByteArray>&)),
            SLOT(onReplyReceived(const QList<QByteArray>&)));
    connect(mSocket, SIGNAL(receivedSize(quint32)), SIGNAL(receivedSize(quint32)));
    connect(mSocket, SIGNAL(sentSize(quint32)), SLOT(onSentSize(quint32)));

    try
    {
        mSocket->setIdentity(identity);
    }
    catch ( const zmq::error_t &e )
    {
        qDebug() << tr("Dealer '%1'> Identity error: ").arg(mAddress) << e.what();
    }
}

QString DealerClient::identity() const
{
    return mSocket->identity();
}

int DealerClient::start()
{
    if ( isStarted() )
        return true;

    setStarted(false);

    try
    {
        mSocket->connectTo(mAddress);
    }
    catch ( const zmq::error_t &e )
    {
        qDebug() << tr("Dealer '%1'> Bind error: ").arg(mAddress) << e.what();
        return e.num();
    }
    catch ( ... )
    {
        qDebug() << tr("Dealer '%1'> Unknown error (connect)").arg(mAddress);
        return -1;
    }

    setStarted(true);

    return 0;
}

int DealerClient::start(const QString& address)
{
    mAddress = address;

    return start();
}

void DealerClient::stop()
{
    if ( !isStarted() )
        return;

    try
    {
        mSocket->disconnectFromHost(mAddress);
    }
    catch ( const zmq::error_t &e )
    {
        qDebug() << tr("Dealer '%1'> Unbind error: ").arg(mAddress) << e.what();
    }
    catch ( ... )
    {
        qDebug() << tr("Dealer '%1'> Unknown error (disconnect)").arg(mAddress);
    }

    setStarted(false);
}

void DealerClient::restart()
{
    stop();
    start();
}

void DealerClient::onReplyReceived(const QList<QByteArray>& request)
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

void DealerClient::onSentSize(quint32 size)
{
    quint32 totalSentSize = size + mSocket->identity().size();
    emit sentSize(totalSentSize);
}

bool DealerClient::request(QVariant v)
{
    if ( !isStarted() )
        return false;

    // Variant -> byte array
    QByteArray message;
    QDataStream stream(&message, QIODevice::WriteOnly);
    stream << v;

    try
    {
        return mSocket->sendMessage( message );
    }
    catch ( const zmq::error_t &e )
    {
        qDebug() << tr("Dealer '%1'> Request error: ").arg(mAddress) << e.what();
    }
    catch ( ... )
    {
        qDebug() << tr("Dealer '%1'> Unknown error (request)").arg(mAddress);
    }

    return false;
}

} // namespace izmqt
