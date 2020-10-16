#include "LargeSocket.h"

#include "LargeReceiver.h"
#include "LargeSender.h"

#include <QtNetwork/QHostAddress>

LargeSocket::LargeSocket(QObject *parent) :
    QTcpSocket(parent)
{
    setObjectName("LargeSocket");

    mSocket = QSharedPointer< QTcpSocket >(new QTcpSocket);

    create();
}

LargeSocket::LargeSocket(QTcpSocket *s, QObject *parent) :
    QTcpSocket(parent)
{
    setObjectName("LargeSocket");

    s->setParent(0);
    mSocket = QSharedPointer< QTcpSocket >(s);

    create();
}

LargeSocket::~LargeSocket()
{
}

bool LargeSocket::atEnd() const
{
    return largeReceiver->socket()->atEnd();
}

bool LargeSocket::isSequential() const
{
    return largeReceiver->socket()->isSequential();
}

qint64 LargeSocket::bytesAvailable() const
{
    return buffer.size() + QTcpSocket::bytesAvailable();
}

qint64 LargeSocket::bytesToWrite() const
{
    return socket()->bytesToWrite();
}

bool LargeSocket::canReadLine() const
{
    return socket()->canReadLine();
}

qint64 LargeSocket::pos() const
{
    return socket()->pos();
}

bool LargeSocket::reset()
{
    return socket()->reset();
}

bool LargeSocket::seek(qint64 pos)
{
    return socket()->seek(pos);
}

qint64 LargeSocket::size() const
{
    return socket()->size();
}

bool LargeSocket::waitForBytesWritten(int msecs)
{
    return socket()->waitForBytesWritten(msecs);
}

bool LargeSocket::waitForReadyRead(int msecs)
{
    return socket()->waitForReadyRead(msecs);
}

void LargeSocket::abort()
{
    socket()->abort();
}

void LargeSocket::connectToHost(const QHostAddress &address, quint16 port
                                , QIODevice::OpenMode openMode)
{
    socket()->connectToHost(address, port, openMode);
}

void LargeSocket::connectToHost(const QString &hostName, quint16 port
                                , QIODevice::OpenMode openMode)
{
    socket()->connectToHost(hostName, port, openMode);
}

void LargeSocket::disconnectFromHost()
{
    socket()->disconnectFromHost();
}

bool LargeSocket::flush()
{
    return socket()->flush();
}

QHostAddress LargeSocket::localAddress() const
{
    return socket()->localAddress();
}

quint16 LargeSocket::localPort() const
{
    return socket()->localPort();
}

QHostAddress LargeSocket::peerAddress() const
{
    return socket()->peerAddress();
}

QString LargeSocket::peerName() const
{
    return socket()->peerName();
}

quint16 LargeSocket::peerPort() const
{
    return socket()->peerPort();
}

QAbstractSocket::SocketState LargeSocket::state() const
{
    return socket()->state();
}

bool LargeSocket::setSocketDescriptor(int socketDescriptor, QAbstractSocket::SocketState state
                                      , QIODevice::OpenMode openMode)
{
    return socket()->setSocketDescriptor(socketDescriptor, state, openMode);
}

void LargeSocket::create()
{
    open(QTcpSocket::ReadWrite);
//    if (socket()->state() == QAbstractSocket::ConnectedState)
//        open(socket()->openMode());

    connect(socket(), SIGNAL(aboutToClose()), SIGNAL(aboutToClose()));
    connect(socket(), SIGNAL(bytesWritten(qint64)), SIGNAL(bytesWritten(qint64)));
    connect(socket(), SIGNAL(connected()), SLOT(onConnected()));
    connect(socket(), SIGNAL(disconnected()), SLOT(onDisconnected()));
    connect(socket(), SIGNAL(error(QAbstractSocket::SocketError))
            , SIGNAL(error(QAbstractSocket::SocketError)));
    connect(socket(), SIGNAL(hostFound()), SIGNAL(hostFound()));
    connect(socket(), SIGNAL(proxyAuthenticationRequired(QNetworkProxy,QAuthenticator*))
            , SIGNAL(proxyAuthenticationRequired(QNetworkProxy,QAuthenticator*)));
    connect(socket(), SIGNAL(readChannelFinished()), SIGNAL(readChannelFinished()));
    connect(socket(), SIGNAL(stateChanged(QAbstractSocket::SocketState))
            , SIGNAL(stateChanged(QAbstractSocket::SocketState)));

    largeReceiver = new LargeReceiver(this);
    largeReceiver->setSocket(socket());
    connect(largeReceiver, SIGNAL(error(QString)), SLOT(onError(QString)));
    connect(largeReceiver, SIGNAL(dataCame(QByteArray)), SLOT(onDataReceived(QByteArray)));
    connect(largeReceiver, SIGNAL(packageReceived()), SIGNAL(packageReceived()));
    connect(largeReceiver, SIGNAL(packageReceived(int,int)), SIGNAL(packageReceived(int,int)));

    largeSender = new LargeSender(this);
    largeSender->setSocket(socket());
    connect(largeSender, SIGNAL(error(QString)), SLOT(onError(QString)));
}

qint64 LargeSocket::readData(char *data, qint64 maxlen)
{
    qint64 ret = qMin(maxlen, qint64(buffer.size()));
    memcpy(data, buffer.constData(), ret);
    buffer.remove(0, ret);
    return ret;
}

qint64 LargeSocket::writeData(const char *data, qint64 len)
{
    return largeSender->writeData(QByteArray(data, len));
}

void LargeSocket::onError(const QString &str)
{
    setErrorString(str);
    emit error(str, sender()->objectName());
}

void LargeSocket::onDataReceived(const QByteArray &data)
{
    buffer.append(data);
    emit readyRead();
}

void LargeSocket::onConnected()
{
    open(socket()->openMode());
    emit connected();
}

void LargeSocket::onDisconnected()
{
    close();
    emit disconnected();
}
