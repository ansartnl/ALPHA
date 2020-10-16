#include "PulsarPrivate.h"
#include "Pulsar.h"
#include "PulsarShareFunctions.h"

#include "Compare.h"

#include <QtCore/QDataStream>
#include <QtCore/QStringList>
#include <QtCore/QTimer>

#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QUdpSocket>

namespace {
    struct Sleep : public QThread { using QThread::msleep;
                                    using QThread::sleep;};
}

PulsarPrivate::PulsarPrivate(bool passive, quint16 port, const QString &additionalStatus, const QVariantHash &additionalInfo, QObject *parent, const QString& serverAddress)
    :QObject(parent), server(new QUdpSocket(this))
    , waitTimeout(new QTimer(this)), sendStatusTimeout(new QTimer(this))
    , dateTime(QDateTime::currentDateTimeUtc()), iam(false)
    , additionalStatus(additionalStatus), mPort(port), passive(passive)
{
    myAddInfo = additionalInfo;
    stringToConsole(tr("Waiting for master..."));

    timer.start();

    connect(server, SIGNAL(readyRead()), this, SLOT(readyRead()));

    if(serverAddress.isEmpty())
        server->bind(port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    else
        server->bind(QHostAddress(serverAddress), port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

    connect(sendStatusTimeout, SIGNAL(timeout()), this, SLOT(sendStatus()));
    sendStatusTimeout->setInterval(mSendTime);
    sendStatusTimeout->start();

    connect(waitTimeout, SIGNAL(timeout()), this, SLOT(becomeMaster()));
    waitTimeout->setInterval(mReceiveTime);
    waitTimeout->setSingleShot(true);
    waitTimeout->start();
}

void PulsarPrivate::setMask(const QString &m)
{
    mMask = m;
}

void PulsarPrivate::setBroadcast(const QStringList &bl)
{
    mBroadcastList = bl;
}

void PulsarPrivate::becomeMaster()
{
    stringToConsole(tr("Becoming a master..."));

    iam = true;
    emit started();
}

void PulsarPrivate::sendStatus()
{
    QString status;
    QStringList information;

    if (passive) {
        status = "3";
    } else {
        if (iam)
        {
            status = "1";
            information.append(tr("Status: active\n"));
        }
        else
        {
            status = "0";
            information.append(tr("Status: inactive\n"));
        }
    }

    information.append(tr("Start time: %1").arg(dateTime.toString("dd.MM.yyyy hh:mm")));
    information.append(tr("Uptime: %1").arg(msecToString(timer.elapsed())));

    status.append(information.join("\n"));
    status.append("\n");
    status.append(additionalStatus);
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << status;
    stream << myAddInfo;
    stream << dateTime;
    broadcast(data, mPort);
}

void PulsarPrivate::sendActivate()
{
    QScopedPointer<QUdpSocket> client(new QUdpSocket());
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    foreach (const QNetworkInterface &interface, interfaces)
    {
        if (interface.CanBroadcast)
        {
            foreach (const QNetworkAddressEntry &entry, interface.addressEntries())
            {
                if ((entry.ip().protocol() == QUdpSocket::IPv4Protocol)
                        && isOurMask(entry.netmask()) && isOurBroadcast(entry.broadcast()))
                {
                    QByteArray data;
                    QString strdata = "2";
                    strdata.append(entry.ip().toString());
                    QDataStream stream(&data, QIODevice::WriteOnly);
                    stream << strdata;
                    stream << QVariantHash();
                    stream << dateTime;
                    client->writeDatagram(data, entry.ip(), mPort);
                    client->writeDatagram(data, entry.broadcast(), mPort);
                }
            }
        }
    }
}

void PulsarPrivate::broadcast(const QByteArray &data, quint16 port)
{
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    foreach (const QNetworkInterface &interface, interfaces)
    {
        if (interface.CanBroadcast)
        {
            foreach (const QNetworkAddressEntry &entry, interface.addressEntries())
            {
                if ((entry.ip().protocol() == QUdpSocket::IPv4Protocol)
                        && isOurMask(entry.netmask()) && isOurBroadcast(entry.broadcast()))
                {
                    QScopedPointer<QUdpSocket> client(new QUdpSocket());
                    client->writeDatagram(data, entry.broadcast(), port);
                }
            }
        }
    }
}

bool PulsarPrivate::isOurMask(const QHostAddress &mask)
{
    if (mMask.isEmpty())
        return true;
    return (mask == QHostAddress(mMask));
}

bool PulsarPrivate::isOurBroadcast(const QHostAddress &broadcast)
{
    if (mBroadcastList.isEmpty())
        return true;
    return mBroadcastList.contains(broadcast.toString());
}

void PulsarPrivate::readyRead()
{
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    while(server->hasPendingDatagrams())
    {
        QByteArray datagramm;
        QDateTime otherDateTime;
        datagramm.resize(server->pendingDatagramSize());
        QHostAddress peerHost;
        quint16 port;
        server->readDatagram(datagramm.data(), datagramm.size(), &peerHost, &port);

        QString strdata;
        QVariantHash addInfo;
        QDataStream stream(datagramm);
        stream >> strdata;
        stream >> addInfo;
        stream >> otherDateTime;

        if (stream.status() != QDataStream::Ok)
            continue;

        if (iam)// if i am master
        {
            if (strdata.startsWith("1"))
            {
                bool itIsMe = false;
                bool needBecomeSlave = (otherDateTime <= dateTime);
                foreach (const QNetworkInterface interface, interfaces)
                {
                    foreach(const QNetworkAddressEntry entry, interface.addressEntries())
                    {
                        if (entry.ip().protocol() == peerHost.protocol())
                        {
                            if (peerHost == entry.ip())
                            {
                                itIsMe = true;
                            }
                        }
                    }
                }
                if (!itIsMe && needBecomeSlave)
                {
                    stringToConsole(tr("Becoming a slave..."));
                    iam = false;
                    waitTimeout->start();
                    otherAddInfo = addInfo;
                    emit stopped(peerHost, strdata, addInfo);
                }
                // Stop packet
            } else if (strdata.startsWith("2"))
            {
                bool itsme = false;
                QHostAddress host(strdata.mid(1));
                foreach (const QNetworkInterface interface, interfaces)
                {
                    foreach(const QNetworkAddressEntry entry, interface.addressEntries())
                    {
                        if (host == entry.ip())
                        {
                            itsme = true;
                        }
                    }
                }
                if (!itsme) {
                    stringToConsole(tr("Becoming a slave..."));
                    iam = false;
                    otherAddInfo = addInfo;
                    emit stopped(peerHost, strdata, addInfo);
                    Pulsar::msleep(5000);
                    waitTimeout->start();
                }
            }

        } else // if i am not master
        {
            if (strdata.startsWith("1"))
            {
                //if (lastpeer != peerHost)
                if (masterIdentificator != otherDateTime)
                {
                    stringToConsole(tr("Master found"));
                    otherAddInfo = addInfo;
                    masterIdentificator = otherDateTime;
                    emit masterChanged(peerHost, strdata.mid(1), lastpeer, addInfo);
                    lastpeer = peerHost;
                } else if (Compare::notEq(otherAddInfo, addInfo))
                {
                    otherAddInfo = addInfo;
                    emit otherMasterInfoChanged(lastpeer, otherAddInfo);
                }
                waitTimeout->start();
            } else if (strdata.startsWith("2"))
            {
                QHostAddress host(strdata.mid(1));
                foreach (const QNetworkInterface interface, interfaces)
                {
                    foreach(const QNetworkAddressEntry entry, interface.addressEntries())
                    {
                        if (entry.ip().protocol() == peerHost.protocol())
                        {
                            if (peerHost.isInSubnet(entry.ip(), entry.prefixLength()))
                            {
                                if (host == entry.ip())
                                {
                                    stringToConsole(tr("Becoming a master..."));
                                    iam = true;
                                    waitTimeout->stop();
                                    otherAddInfo.clear();
                                    emit started();
                                }
                            }
                        }
                    }
                }
                if (!iam)
                    waitTimeout->start();
            }
        }
    }
}

void PulsarPrivate::setAdditionalStatus(const QString &status)
{
    additionalStatus = status;
}

void PulsarPrivate::setAdditionalInfo(const QVariantHash &info)
{
    myAddInfo = info;
}

void PulsarPrivate::addAdditionalInfo(const QString &key, const QVariant &value)
{
    myAddInfo.insert(key, value);
}

void PulsarPrivate::removeAdditionalInfo(const QString &key)
{
    myAddInfo.remove(key);
}

void PulsarPrivate::deactivate()
{
    stringToConsole(tr("Becoming a slave..."));
    iam = false;
    waitTimeout->start();
    emit stopped(QHostAddress(), "", otherAddInfo);
    Sleep::msleep(20000);
}
