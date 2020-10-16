#include "PulsarShareFunctions.h"

#include "PulsData.h"

#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QUdpSocket>

int id = qRegisterMetaType<QHostAddress>();

QString msecToString(quint64 msec)
{
    quint64 newmsec = msec % 1000;
    quint64 sec = (msec / 1000) % 60;
    quint64 minutes = (msec / 60000) % 60;
    quint64 hours = (msec / 3600000) % 60;

    return QString("%1:%2:%3.%4")
            .arg(hours, 2, 10, QLatin1Char('0'))
            .arg(minutes, 2, 10, QLatin1Char('0'))
            .arg(sec, 2, 10, QLatin1Char('0'))
            .arg(newmsec, 3, 10, QLatin1Char('0'));
}

bool operator <(const QHostAddress &x, const QHostAddress &y)
{
    if (x.protocol() == QAbstractSocket::IPv4Protocol)
    {
        return (x.toIPv4Address() < y.toIPv4Address());
    } else if (x.protocol() == QAbstractSocket::IPv6Protocol)
    {
        quint64 local;
        quint64 peer;
        QIPv6Address laddr = x.toIPv6Address();
        QIPv6Address paddr = y.toIPv6Address();

        qMemCopy(&local, &laddr.c, 8);
        qMemCopy(&peer, &paddr.c, 8);
        return local < peer;
    }
    return false;
}

QDebug stringToConsole(const QString &str)
{
    return (qDebug() << str);
}

void broadcast(const QByteArray &data, quint16 port, const QHostAddress &mask)
{
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    foreach (const QNetworkInterface &interface, interfaces)
    {
        if (interface.CanBroadcast)
        {
            foreach (const QNetworkAddressEntry &entry, interface.addressEntries())
            {
                if ((entry.ip().protocol() == QUdpSocket::IPv4Protocol)
                        && (mask.isNull() || (entry.netmask() == mask)))
                {
                    QScopedPointer<QUdpSocket> client(new QUdpSocket());
                    client->writeDatagram(data, entry.broadcast(), port);
                }
            }
        }
    }
}

void broadcast(const PulsData &data, quint16 port, const QHostAddress &mask)
{
    PulsData pd = data;
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    foreach (const QNetworkInterface &interface, interfaces)
    {
        if (interface.CanBroadcast)
        {
            foreach (const QNetworkAddressEntry &entry, interface.addressEntries())
            {
                if ((entry.ip().protocol() == QUdpSocket::IPv4Protocol)
                        && (mask.isNull() || (entry.netmask() == mask)))
                {
                    QScopedPointer<QUdpSocket> client(new QUdpSocket());
                    pd.pulsSourceIp = entry.ip().toString();
                    client->writeDatagram(pd.toByteArray(), entry.broadcast(), port);
                }
            }
        }
    }
}

void sendActivate(const QHostAddress &host, quint16 port, const QHostAddress &mask, const QVariantHash &addInfo)
{
    PulsData pd;
    pd.status = PulsData::ACTIVATE;
    pd.information.append(QObject::tr("Status: activate"));
    pd.additionalInfo = addInfo;
    pd.pulsSourceIp = host.toString();
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    foreach (const QNetworkInterface &interface, interfaces)
    {
        if (interface.CanBroadcast)
        {
            foreach (const QNetworkAddressEntry &entry, interface.addressEntries())
            {
                if ((entry.ip().protocol() == QUdpSocket::IPv4Protocol)
                        && (mask.isNull() || (entry.netmask() == mask)))
                {
                    QScopedPointer<QUdpSocket> client(new QUdpSocket());
                    if (host.isNull())
                        pd.pulsSourceIp = entry.ip().toString();
//                    client->writeDatagram(pd.toByteArray(), entry.ip(), port);
                    client->writeDatagram(pd.toByteArray(), entry.broadcast(), port);
                }
            }
        }
    }
}
