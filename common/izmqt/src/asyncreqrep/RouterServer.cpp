#include "RouterServer.h"

#include "nzmqt/nzmqt.hpp"

#include <QDebug>
#include <QDataStream>

namespace izmqt
{

RouterServer::RouterServer(const QString& address, QObject* parent)
    : QObject(parent), mAddress(address)
{
    nzmqt::ZMQContext* context = nzmqt::createDefaultContext(this);
    context->start();

    mSocket = context->createSocket(nzmqt::ZMQSocket::TYP_ROUTER);
    connect(mSocket, SIGNAL(messageReceived(const QList<QByteArray>&)),
            SLOT(onRequestReceived(const QList<QByteArray>&)));
    connect(mSocket, SIGNAL(receivedSize(quint32)), SIGNAL(receivedSize(quint32)));
}

int RouterServer::start()
{
    if ( isStarted() )
        return true;

    setStarted(false);

    try
    {
        mSocket->bindTo(mAddress);
    }
    catch ( const zmq::error_t &e )
    {
        qDebug() << tr("Router '%1'> Bind error: ").arg(mAddress) << e.what();
        return e.num();
    }
    catch ( ... )
    {
        qDebug() << tr("Router '%1'> Unknown error (bind)").arg(mAddress);
        return -1;
    }

    setStarted(true);

    return 0;
}

int RouterServer::start(const QString& address)
{
    mAddress = address;

    return start();
}

void RouterServer::stop()
{
    if ( !isStarted() )
        return;

    try
    {
        mSocket->unbind(mAddress);
    }
    catch ( const zmq::error_t &e )
    {
        qDebug() << tr("Router '%1'> Unbind error: ").arg(mAddress) << e.what();
    }
    catch ( ... )
    {
        qDebug() << tr("Router '%1'> Unknown error (unbind)").arg(mAddress);
    }

    setStarted(false);
}

void RouterServer::restart()
{
    stop();
    start();
}

void RouterServer::onRequestReceived(const QList<QByteArray>& request)
{
    // We wait multi-part message with 2 elements:
    // 1 - client's identity
    // 2 - message data
    if ( request.size() < 2 )
        return;

    QString identity = request[0];
    QByteArray m = request[1];

    // Byte array -> variant
    QVariant v;
    QDataStream stream(&m, QIODevice::ReadOnly);
    stream >> v;

    if ( !v.isNull() && v.isValid() )
        emit requestReceived(v, identity);
}

bool RouterServer::reply(QVariant v, QString identity)
{
    if ( !isStarted() )
        return false;

    // Variant -> byte array
    QByteArray message;
    QDataStream stream(&message, QIODevice::WriteOnly);
    stream << v;

    try
    {
        QList< QByteArray > msg;
        msg += identity.toLocal8Bit();  // Identifies client receiver.
        msg += message;

        return mSocket->sendMessage( msg );
    }
    catch ( const zmq::error_t &e )
    {
        qDebug() << tr("Router '%1'> Reply error: ").arg(mAddress) << e.what();
    }
    catch ( ... )
    {
        qDebug() << tr("Router '%1'> Unknown error (reply)").arg(mAddress);
    }

    return false;
}

} // namespace izmqt
