#include "qttcpdatagramprotocolvariant.h"

QtTcpDatagramProtocolVariant::QtTcpDatagramProtocolVariant(QTcpSocket *socket, QObject *parent)
    : QtTcpDatagramProtocol(socket, parent)
{
    connect(this, SIGNAL(incomingDatagram()), this, SLOT(onProtocolMessage()));
}

QtTcpDatagramProtocolVariant::QtTcpDatagramProtocolVariant(QObject *parent)
    : QtTcpDatagramProtocol(parent)
{
    connect(this, SIGNAL(incomingDatagram()), this, SLOT(onProtocolMessage()));
}

QtTcpDatagramProtocolVariant::~QtTcpDatagramProtocolVariant()
{}

void QtTcpDatagramProtocolVariant::onProtocolMessage()
{
    QVariantList list;
    this >> list;
    if (!list.isEmpty())
        emit newMessages(list);
}

QtTcpDatagramProtocolVariantRequest::~QtTcpDatagramProtocolVariantRequest()
{
    if (data.isEmpty())
        return;

    QByteArray arr;
    QDataStream stream(&arr, QIODevice::WriteOnly);
    foreach (const QVariant &v, data)
        stream << v;
    s->writeDatagram(arr);
}

QtTcpDatagramProtocol &operator>> (QtTcpDatagramProtocol &protocol, QVariantList &result)
{
    result.clear();
    while (protocol.hasPendingDatagrams())
    {
        QByteArray arr = protocol.nextDatagram();
        QDataStream stream(&arr, QIODevice::ReadOnly);
        QVariant v;
        stream >> v;
        while (stream.status() == QDataStream::Ok)
        {
            result.append(v);
            stream >> v;
        }
    }
    return protocol;
}

QtTcpDatagramProtocol &operator>> (QtTcpDatagramProtocol *protocol, QVariantList &result)
{
    Q_ASSERT(protocol);
    return operator >>(*protocol, result);
}

