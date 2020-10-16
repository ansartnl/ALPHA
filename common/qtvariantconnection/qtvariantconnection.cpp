#include "qtvariantconnection.h"
#include "qttcpdatagramprotocol.h"

QtVariantConnection::QtVariantConnection(QTcpSocket *socket, QObject *parent) :
    QObject(parent), protocol(new QtTcpDatagramProtocol(socket, this))
{
    connect(protocol, SIGNAL(incomingDatagram()), this, SLOT(onIncomingDatagram()));
    connect(socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
}

QtVariantConnection::~QtVariantConnection()
{
    protocol->socket()->flush();
}

void QtVariantConnection::setProtoVersion(quint32 v)
{
    protocol->setVersion(v);
}

void QtVariantConnection::onIncomingDatagram()
{
    while (protocol->hasPendingDatagrams())
    {
        QByteArray data = protocol->nextDatagram();
        QDataStream stream(&data, QIODevice::ReadOnly);
        QVariant v;
        stream >> v;
        if (QDataStream::Ok == stream.status())
            emit gotVariant(v);
    }
}

qint64 QtVariantConnection::sendVariant(const QVariant &v)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << v;
    return protocol->writeDatagram(data);
}

const QTcpSocket *QtVariantConnection::socket() const
{
    return protocol->socket();
}
