#include "qttcpdatagramprotocol.h"

#include <QtCore/QDataStream>
#include <QtCore/QTimer>
#include <QtNetwork/QHostAddress>

//! Protocol identifier - same for each datagram
static const QByteArray g_protoID = "6B3F2685-7E1B-4fb4-B954-E89B049BA65D";


QtTcpDatagramProtocol::QtTcpDatagramProtocol(QTcpSocket *socket, QObject *parent)
    : QObject(parent), mSocket(0), needReadBlockSize(true), needEmit(false), limit(1*1024*1024) //1MB
    , version(0x00010000)
{
    setSocket(socket);
    socket->setParent(this);
}

QtTcpDatagramProtocol::QtTcpDatagramProtocol(QObject *parent)
    : QObject(parent), mSocket(0), needReadBlockSize(true), needEmit(false), limit(1*1024*1024) //1MB
    , version(0x00010000)
{}

QtTcpDatagramProtocol::~QtTcpDatagramProtocol()
{}

QTcpSocket *QtTcpDatagramProtocol::setSocket(QTcpSocket *socket)
{
    if (mSocket)
        disconnect(mSocket, 0, this, 0);
    QTcpSocket *result = mSocket;
    mSocket = socket;
    datargams.clear();
    needReadBlockSize = true;
    needEmit = false;
    if (mSocket)
    {
        connect(mSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        connect(mSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onSocketStateChanged(QAbstractSocket::SocketState)));
        connect(mSocket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
        connect(mSocket, SIGNAL(connected()), this, SIGNAL(connected()));
        QTimer::singleShot(0, this, SLOT(onReadyRead()));
    }
    return result;
}

void QtTcpDatagramProtocol::onSocketStateChanged(QAbstractSocket::SocketState socketState)
{
    if (QAbstractSocket::UnconnectedState == socketState)
    {
        needEmit = false;
        needReadBlockSize = true;
    }
}

qint64 QtTcpDatagramProtocol::writeDatagram(const QByteArray &datagram)
{
    if (!mSocket)
        return -1;

    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    int sizeOfDatagram = datagram.size();
    //if (sizeOfDatagram <= 0 || sizeOfDatagram > 0xFFFF)
    if (sizeOfDatagram <= 0)
        return -1;
    out.writeRawData(g_protoID.data(), g_protoID.size());
    out << version << quint32(sizeOfDatagram);
    out.writeRawData(datagram.data(), datagram.size());
    return mSocket->write(data);
}

void QtTcpDatagramProtocol::onReadyRead()
{
    if (!mSocket)
        return;

    try
    {
        QDataStream in(mSocket);
        forever
        {
            if (needReadBlockSize)
            {
                if (mSocket->bytesAvailable() < (int)sizeof(blockSize)
                        || mSocket->bytesAvailable() < g_protoID.size() )
                    break;

                QByteArray protoID(g_protoID.size(), Qt::Uninitialized);
                if ( !(g_protoID.size() == in.readRawData(protoID.data(), g_protoID.size())
                     && g_protoID == protoID) )
                    throw QString("Protocol is not valid");

                quint32 protoVersion;
                in >> protoVersion >> blockSize;
                if ( protoVersion != version )
                    throw QString("Protocol version is not supported. Current version '%1', received version '%2' from '%3:%4'")
                        .arg(version).arg(protoVersion).arg(mSocket->peerAddress().toString()).arg(mSocket->localPort());

                needReadBlockSize = false;
            }

            if (mSocket->bytesAvailable() < blockSize)
                return;

            datargams.push_back(mSocket->read(blockSize));
            needEmit = true;
            needReadBlockSize = true;
        }

        if (needEmit)
        {
            emit incomingDatagram();
            needEmit = false;
            trunc();
        }
    }
    catch ( const QString &ex )
    {
        // clear the socket.
        mSocket->readAll();

        if ( QAbstractSocket::ConnectedState == mSocket->state() )
        {
            qDebug() << tr("Tcp datagram protocol. Error: %1 {%2:%3}")
                        .arg(ex).arg(mSocket->peerAddress().toString()).arg(mSocket->peerPort());
        }
    }
}

bool QtTcpDatagramProtocol::hasPendingDatagrams() const
{
    return !datargams.isEmpty();
}

QByteArray QtTcpDatagramProtocol::nextDatagram()
{
    if (datargams.isEmpty())
        return QByteArray();

    QByteArray result = datargams.first();
    datargams.pop_front();
    return result;
}

void QtTcpDatagramProtocol::trunc()
{
    quint64 curSize = 0;
    foreach (const QByteArray &arr, datargams)
        curSize += arr.size();

    while (curSize > limit && datargams.size() > 1)
    {
        curSize -= datargams.first().size();
        datargams.pop_front();
    }
}

int QtTcpDatagramProtocol::datagramAmount() const
{
    return datargams.size();
}

QList<QByteArray> QtTcpDatagramProtocol::readAllDatagrams()
{
    QList<QByteArray> result(datargams);
    datargams.clear();
    return result;
}

void QtTcpDatagramProtocol::clear()
{
    datargams.clear();
}
