/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtNetwork>
#include <stdlib.h>

#include "main.h"
#include "server.h"

#include "qttcpdatagramprotocol.h"
#include "qttcpdatagramprotocolvariant.h"

Server::Server(QObject *parent)
    :   QObject(0), mTcpServer(0), mReturnCode(0)
{
}

Server::~Server()
{
    mMutex.tryLock();
    mMutex.unlock();

    if(mTcpServer)
    {
        mTcpServer->close();
        mTcpServer->deleteLater();
    }
}

void Server::applyUpdate(const Server::TData &update)
{
    foreach (const QString& airport, update.keys())
    {
        foreach(const QString& runway, update[airport].keys())
        {
            mData[airport][runway] = update[airport][runway];

            if(mData[airport][runway].numPeople < 0)
                mData[airport][runway].numPeople = 0;

            if(mData[airport][runway].numAuto < 0)
                mData[airport][runway].numAuto = 0;

            if(mData[airport][runway].numOther < 0)
                mData[airport][runway].numOther = 0;
        }
    }
}

void Server::startServer()
{
    mTcpServer = new QTcpServer();
    connect(mTcpServer, SIGNAL(newConnection()), this, SLOT(handleClient()));

    quint16 port = xmlSettings().value(QString("port"), QString("34165")).toUInt();

    if (!mTcpServer->listen(QHostAddress::Any, port))
    {
        exit(-1);
    }
}

void Server::handleClient()
{
    QTcpSocket *clientConnection = mTcpServer->nextPendingConnection();
    connect(clientConnection, SIGNAL(disconnected()),
            clientConnection, SLOT(deleteLater()));
    connect(clientConnection, SIGNAL(disconnected()),
            this, SLOT(onDisconnected()));

    QtTcpDatagramProtocol proto(clientConnection);

    QByteArray block;
    QDataStream stream(&block, QIODevice::ReadWrite);

    mMutex.lock();
    stream << mData;
    proto.writeDatagram(block);

    if(clientConnection->waitForReadyRead(1000))
    {
        QByteArray inputblock;
        QDataStream inputstream(&inputblock, QIODevice::ReadWrite);
        inputblock = proto.nextDatagram();
        if(inputblock.at(0) == '1')
        {
            reset();
        }
        else
        {
            TData update;
            inputstream >> update;
            applyUpdate(update);
        }
    }
    else
    {
        clientConnection->disconnectFromHost();
    }
}

void Server::onDisconnected()
{
    mMutex.unlock();
}

void Server::reset()
{
    mData.clear();
}
