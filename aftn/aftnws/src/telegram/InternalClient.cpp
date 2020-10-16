#include "InternalClient.h"

#include "requests.h"
#include "qtautotransaction.h"
#include <QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>

//#include "LargeSocket.h"
//#include "qtsocketautoconnector.h"
//#include "qttcpdatagramprotocol.h"
//#include "UserClient.h"
//#include "usTransport.h"

InternalClient::InternalClient(QObject *parent) :
    QObject(parent)
{
//    mSocket = new LargeSocket(this);

//    mConnection = new QtTcpDatagramProtocol(mSocket, this);
//    connect(mConnection, SIGNAL(incomingDatagram()), SLOT(onReadyRead()));
//    connect(mConnection, SIGNAL(disconnected()), SLOT(onDisconnected()));
//    connect(mConnection, SIGNAL(connected()), SLOT(onConnected()));

//    mConnector = new QtSocketAutoConnector(this);
//    mConnector->setSocket(mSocket->socket());
//    mConnector->setInterval(1000);

//    mTransport = new UserSystem::Transport(this);
//    connect(mTransport, SIGNAL(send(QVariant)), SLOT(send(QVariant)));
//    UserSystem::userClient(mTransport, this);
}

void InternalClient::connectToHost(const QHostAddress &address, quint16 port)
{
//    mConnector->removeAllAddresses();
//    mConnector->addAddress(address, port);
}

void InternalClient::send(const QByteArray &arr)
{
//    mConnection->writeDatagram(arr);
}

void InternalClient::send(const QVariant &data)
{
//    sendVariant(data);
    if(data.canConvert < SendTelegram >())
    {
        QSqlDatabase db = QSqlDatabase::database();
        if(db.isOpen())
        {
            QtAutoTransaction transaction(db);
            QSqlQuery query(db);
            query.prepare("INSERT INTO outputtelegrams(text, addresses) VALUES(:1, :2)");
            AftnStruct as = data.value < SendTelegram >();
            query.bindValue(":1", as.text);
            query.bindValue(":2", as.receiverAddresses.join(" "));
            if(query.exec())
            {
                transaction.commit();
            }
            else
            {
                qDebug() << tr("Execution error. %1").arg(query.lastError().text());
            }
        }
    }
}

void InternalClient::onConnected()
{
//    emit connected();
}

void InternalClient::onDisconnected()
{
//    emit disconnected();
}

void InternalClient::onReadyRead()
{
//    while (mConnection->hasPendingDatagrams())
//    {
//        QByteArray datagram = mConnection->nextDatagram();
//        QDataStream stream(datagram);
//        while (!stream.atEnd())
//        {
//            QVariant d;
//            stream >> d;
//            emit readyRead(d);
//            mTransport->onDataReceived(d);
//        }
//    }
}
