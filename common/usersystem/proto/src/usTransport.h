#ifndef USTRANSPORT_H
#define USTRANSPORT_H

#include <QtCore/QObject>

namespace UserSystem
{

/*
  Use:
  connect your Receive protocol's read(const QVariant &) signal to Transport::onDataReceived slot
  and Transport::send signal to your Send protocol's write(const QVariant &data) slot.
  use this transport in UserClient/UserServer constructor.

  Example:
    {
        Transport *transport = new Transport(this);
        QtVariantConnection *protocol = new QtVariantConnection(tcpSocket, this);
        connect(protocol, SIGNAL(gotVariant(QVariant)), transport, SLOT(onDataReceived(QVariant)));
        connect(transport, SIGNAL(send(QVariant)), protocol, SLOT(sendVariant(QVariant)));
        UserClient *uc = new UserClient(transport, this);
    }

  When using both UserClient and UserServer withing one application, you chould do:
    {
        UserSystem::Transport *clientTransport = new UserSystem::Transport(parent);
        UserSystem::Transport *serverTransport = new UserSystem::Transport(parent);
        QObject::connect(serverTransport, SIGNAL(send(QVariant)), clientTransport, SLOT(onDataReceived(QVariant)));
        QObject::connect(clientTransport, SIGNAL(send(QVariant)), serverTransport, SLOT(onDataReceived(QVariant)));
        UserSystem::userClient(clientTransport, parent);
        UserSystem::userServer(serverTransport, parent);
    }
  */

class Transport : public QObject
{
    Q_OBJECT
public:
    explicit Transport(QObject *parent = 0);

    void sendData(const QVariant &data);

    QString userName() const;
    void setUserName(const QString &u);

signals:
    void send(const QVariant &);
    void receive(const QVariant &);

public slots:
    void onDataReceived(const QVariant &data);

protected:
    QString mUserName;
};

} // namespace

#endif // USTRANSPORT_H
