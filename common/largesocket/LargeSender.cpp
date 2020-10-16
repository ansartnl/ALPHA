#include "LargeSender.h"

#include "LargeData.h"

#include <QtNetwork/QTcpSocket>

LargeSender::LargeSender(QObject *parent) :
    QObject(parent)
{
    setObjectName("LargeSender");

    mSocket = 0;
    mIdentifier = LargeData::BAD_IDENTIFIER;
}

QTcpSocket * LargeSender::setSocket(QTcpSocket *socket)
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
    }
    return result;
}

qint64 LargeSender::writeData(const QByteArray &data)
{
    if (!mSocket)
        return -1;

    ++mIdentifier;
    if (mIdentifier == LargeData::BAD_IDENTIFIER)
        ++mIdentifier;

    parts.clear();

    for (size_t i = 0; i < data.size()/LargeData::MAX_DATA_SIZE; ++i)
        parts.append(data.mid(i * LargeData::MAX_DATA_SIZE, LargeData::MAX_DATA_SIZE));
    QByteArray tail = data.mid(parts.count() * LargeData::MAX_DATA_SIZE);
    if (!tail.isEmpty())
        parts.append(tail);

    int s = 0;
    emit started();
    for (num = 0; num < parts.count(); ++num)
        s = sendNextPart();
    return s;
}

qint64 LargeSender::sendNextPart()
{
    if (num >= parts.count())
    {
        parts.clear();
        emit finished();
        return num * qint64(LargeData::MAX_DATA_SIZE);
    }

    LargeData::DataPacket p;
    strcpy(p.header.signature, LargeData::SIG);
    p.header.identifier = mIdentifier;
    p.header.number = num;
    p.header.count = parts.count();
    memcpy(p.data, parts.at(num).constData(), parts.at(num).size());
    p.header.size = parts.at(num).size();

    if (mSocket->write(LargeData::toByteArray(p)) == -1)
    {
        emit error(tr("Error writing to socket. Disconnecting..."));
        mSocket->disconnectFromHost();
        return num * qint64(LargeData::MAX_DATA_SIZE);
    }

    return num * qint64(LargeData::MAX_DATA_SIZE);
}

void LargeSender::onDisconnected()
{
    num = 0;
}

void LargeSender::onConnected()
{
    if (!parts.isEmpty())
        sendNextPart();
}

void LargeSender::onError()
{
    emit error(mSocket->errorString());
}
