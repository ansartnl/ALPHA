#include "qtudpsocket.h"
#include "broadcast.h"


QtUdpSocket::QtUdpSocket(QObject *parent)
    : QUdpSocket(parent), _broadcaster(0), _address(QHostAddress::LocalHost), _port(0)
{
}

QtUdpSocket::QtUdpSocket(const QHostAddress &a, int p, QObject *parent)
    : QUdpSocket(parent), _broadcaster(0), _address(QHostAddress::LocalHost), _port(0)
{
    setParameters(a, p);
}

void QtUdpSocket::setParameters(const QHostAddress &a, int p)
{
    if ( _address == a && _port == p )
        return;

    _address = a; _port = p;

    if ( _broadcaster )
    {
        _broadcaster->deleteLater();
        _broadcaster = 0;
    }

    if ( Broadcast::isBroadcastAddress(a) )
    {
        _broadcaster = new Broadcast(this);
        _broadcaster->setPort(p);
    }
}

qint64 QtUdpSocket::sendDatagram(const QByteArray &datagram)
{
    return _broadcaster ? _broadcaster->sendDatagram(datagram) :
                          writeDatagram(datagram, _address, _port);
}
