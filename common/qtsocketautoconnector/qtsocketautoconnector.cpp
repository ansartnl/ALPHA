#include "qtsocketautoconnector.h"
#include <QtCore/QTimer>

QtSocketAutoConnector::QtSocketAutoConnector(QObject *parent)
    : QObject(parent), connectionListPos(0), mInterval(1000)
{
    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(connectSocketToHost()));
}

QtSocketAutoConnector::~QtSocketAutoConnector()
{}

void QtSocketAutoConnector::onSocketStateChanged(QAbstractSocket::SocketState socketState)
{
    if (QAbstractSocket::UnconnectedState == socketState)
        if (!timer->isActive())
            timer->start(0);
}

void QtSocketAutoConnector::connectSocketToHost()
{
    if (connectionList.isEmpty() || !socket || socket->state() != QAbstractSocket::UnconnectedState)
        return;

    timer->start(mInterval);

    if (connectionListPos >= connectionList.size())
        connectionListPos = 0;

    QPair<QHostAddress, quint16> &address = connectionList[connectionListPos++];
    socket->connectToHost(address.first, address.second);
    emit connectingAttempt(address.first, address.second);
}

QAbstractSocket *QtSocketAutoConnector::setSocket(QAbstractSocket *newSocket)
{
    if (socket == newSocket)
        return newSocket;

    QAbstractSocket *oldSocket = socket;
    socket = newSocket;
    if (oldSocket)
    {
        disconnect(oldSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onSocketStateChanged(QAbstractSocket::SocketState)));
        timer->stop();
    }
    if (newSocket)
    {
        connect(newSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onSocketStateChanged(QAbstractSocket::SocketState)));
        if (QAbstractSocket::UnconnectedState == newSocket->state() && !connectionList.isEmpty())
        {
            newSocket->abort();
            timer->start(0);
        }
    }
    return oldSocket;
}

void QtSocketAutoConnector::addAddress(const QHostAddress &address, quint16 port)
{
    connectionList.push_back(QPair<QHostAddress, quint16>(address, port));
    if (socket)
        onSocketStateChanged(socket->state());
}

void QtSocketAutoConnector::removeAddress(const QHostAddress &address, quint16 port)
{
    connectionList.removeAll(QPair<QHostAddress, quint16>(address, port));
}

void QtSocketAutoConnector::removeAddress(int n)
{
    if (connectionList.size() > n)
        connectionList.removeAt(n);
}

void QtSocketAutoConnector::removeAllAddresses()
{
    connectionList.clear();
}

void QtSocketAutoConnector::setInterval(int msInteval)
{
    mInterval = msInteval;
}

int QtSocketAutoConnector::interval() const
{
    return mInterval;
}

void QtSocketAutoConnector::reconnect()
{
    if (socket)
        socket->abort();
}

bool QtSocketAutoConnector::containsAddress(const QHostAddress &address, quint16 port)
{
    return connectionList.contains(QPair<QHostAddress, quint16>(address, port));
}
