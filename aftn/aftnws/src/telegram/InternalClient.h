#ifndef INTERNALCLIENT_H
#define INTERNALCLIENT_H

#include "SmartSender.h"

//class LargeSocket;
//class QtSocketAutoConnector;
//class QtTcpDatagramProtocol;

//namespace UserSystem
//{
//class Transport;
//} // namespace UserSystem

class QHostAddress;

class InternalClient : public QObject, public SmartSender
{
    Q_OBJECT
public:
    explicit InternalClient(QObject *parent = 0);

    void connectToHost(const QHostAddress &address, quint16 port);

    virtual void send(const QByteArray &arr);

public slots:
    void send(const QVariant &data);

protected slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();

signals:
    void connected();
    void disconnected();
    void readyRead(const QVariant &data);

protected:
//    LargeSocket *mSocket;
//    QtSocketAutoConnector *mConnector;
//    QtTcpDatagramProtocol *mConnection;
//    UserSystem::Transport *mTransport;
};

#endif // INTERNALCLIENT_H
