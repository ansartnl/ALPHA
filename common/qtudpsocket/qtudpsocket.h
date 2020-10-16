#ifndef _UDP_SOCKET_H__
#define _UDP_SOCKET_H__

#include <QtNetwork/QUdpSocket>
class Broadcast;

/*!
  * \class QtUdpSocket
  * \brief Отправка пакетов данных по протоколу UDP
  *
  * Данный класс предоставляет интерфейс для отправки пакетов данных
  * через стек UDP в том числе широковещательным способом по всем доступным
  * сетевым интерфейсам.
  */

// Provides datagram send via UDP protocol using broadcast to all available networks if
// network address is broadcast address
class QtUdpSocket : public QUdpSocket
{
    Q_OBJECT
public:
    explicit QtUdpSocket(QObject *parent = 0);
    explicit QtUdpSocket(const QHostAddress &a, int p, QObject *parent = 0);

    /** Sets new network address and port */
    void setParameters(const QHostAddress &a, int p);

    /** Retrieves current port */
    int port() const { return _port; }
    /** Retrieves current address */
    const QHostAddress &address() const { return _address; }

public slots:
    qint64 sendDatagram(const QByteArray &datagram);

private:
    /** Broadcaster */
    Broadcast       *_broadcaster;

    /** Current host address */
    QHostAddress    _address;
    /** Current port number */
    int             _port;
};

#endif // _UDP_SOCKET_H__
