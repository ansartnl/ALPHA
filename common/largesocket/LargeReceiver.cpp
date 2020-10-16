#include "LargeReceiver.h"

#include "LargeData.h"

#include <QtCore/QTimerEvent>

#include <QtNetwork/QTcpSocket>

LargeReceiver::LargeReceiver(QObject *parent) :
    QObject(parent)
{
    setObjectName("LargeReceiver");

    mSocket = 0;
    mIdentifier = LargeData::BAD_IDENTIFIER;

    timerId = -1;
}

QTcpSocket * LargeReceiver::setSocket(QTcpSocket *socket)
{
    if (mSocket)
        disconnect(mSocket, 0, this, 0);
    QTcpSocket *result = mSocket;
    mSocket = socket;

    if (mSocket)
    {
        connect(mSocket, SIGNAL(disconnected()), SLOT(onDisconnected()));
        connect(mSocket, SIGNAL(connected()), SLOT(onConnected()));
        connect(mSocket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(onError()));
        connect(mSocket, SIGNAL(readyRead()), SLOT(onReadyRead()));
    }
    return result;
}

void LargeReceiver::onDisconnected()
{
    parts.clear();
    mIdentifier = LargeData::BAD_IDENTIFIER;
}

void LargeReceiver::onConnected()
{
}

void LargeReceiver::onError()
{
    emit error(mSocket->errorString());
}

void LargeReceiver::onReadyRead()
{
    buffer.append(mSocket->readAll());
    if (buffer.size() >= int(sizeof(LargeData::DataPacket::Header)))
    {
        if (timerId == -1)
            timerId = startTimer(1);
    }
    emit packageReceived();
}

void LargeReceiver::timerEvent(QTimerEvent *e)
{
    if (timerId == -1)
        return;
    if (e->timerId() != timerId)
        return;
    proceed();
}

void LargeReceiver::proceed()
{
    int index = buffer.indexOf(LargeData::SIG);
    if (index)
    {
        emit error(tr("Removing %1 bytes of trash").arg(index));
        buffer.remove(0, index);
    }

    int packetSize = LargeData::packetSize(buffer);

    if (buffer.size() < packetSize)
        return;

    QByteArray data = buffer.mid(0, packetSize);
    buffer.remove(0, data.size());
    proceed(LargeData::fromByteArray(data));

    if (buffer.size() < int(sizeof(LargeData::DataPacket::Header)))
        stopTimer();
}

void LargeReceiver::proceed(const LargeData::DataPacket &packet)
{
    if (mIdentifier != LargeData::BAD_IDENTIFIER)
    {
        if (mIdentifier != packet.header.identifier)
        {
            emit error(tr("New identifier came before last was closed. Some data was lost"));
            parts.clear();
        }
    }

    mIdentifier = packet.header.identifier;

    if (parts.count() != packet.header.number)
    {
        emit error(tr("Wrong packet number. Some data was lost"));
        parts.clear();
        return;
    }

    parts.append(QByteArray(packet.data, packet.header.size));

    emit packageReceived(packet.header.number, packet.header.count);

    if (packet.header.count == parts.count())
        postData();
}

void LargeReceiver::postData()
{
    QByteArray result;
    foreach (QByteArray part, parts)
        result.append(part);
    parts.clear();
    mIdentifier = LargeData::BAD_IDENTIFIER;

    emit dataCame(result);
}

void LargeReceiver::stopTimer()
{
    if (timerId == -1)
        return;
    killTimer(timerId);
    timerId = -1;
}
