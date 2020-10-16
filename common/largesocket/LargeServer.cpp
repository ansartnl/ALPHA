#include "LargeServer.h"

#include "LargeSocket.h"

LargeServer::LargeServer(QObject *parent) :
    QTcpServer(parent)
{
}

QTcpSocket * LargeServer::nextPendingConnection()
{
    return new LargeSocket(QTcpServer::nextPendingConnection(), this);
}

LargeSocket * LargeServer::nextLargeSocket()
{
    return new LargeSocket(QTcpServer::nextPendingConnection(), this);
}
