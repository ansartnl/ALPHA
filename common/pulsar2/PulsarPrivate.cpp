#include "PulsarPrivate.h"

#include "PulsData.h"
#include "PulsarShareFunctions.h"

#include "Compare.h"

#include <QtCore/QStringList>
#include <QtCore/QThread>
#include <QtCore/QTimer>

#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QUdpSocket>

namespace {
    struct Sleep : public QThread { using QThread::msleep;
                                    using QThread::sleep;};
}

PulsarPrivate::PulsarPrivate(bool passive, quint16 port, const QVariantHash &additionalInfo, QObject *parent)
    :QObject(parent), server(new QUdpSocket(this))
    , waitTimeout(new QTimer(this)), sendStatusTimeout(new QTimer(this))
    , dateTime(QDateTime::currentDateTimeUtc()), iam(false)
    , mPort(port), passive(passive)
{
    myAddInfo = additionalInfo;
    stringToConsole(tr("Waiting for master..."));

    timer.start();

    connect(server, SIGNAL(readyRead()), this, SLOT(readyRead()));

    server->bind(port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

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

void PulsarPrivate::becomeMaster()
{
    stringToConsole(tr("Becoming a master..."));

    iam = true;
    emit started();
}

void PulsarPrivate::sendStatus()
{
    PulsData pd;

    if (passive) {
        pd.status = PulsData::PASSIVE;
    } else {
        if (iam)
        {
            pd.status = PulsData::ACTIVE;
            pd.information.append(tr("Status: active"));
        }
        else
        {
            pd.status = PulsData::INACTIVE;
            pd.information.append(tr("Status: inactive"));
        }
    }

    pd.information.append(tr("Start time: %1").arg(dateTime.toString("dd.MM.yyyy hh:mm")));
    pd.information.append(tr("Uptime: %1").arg(msecToString(timer.elapsed())));

    pd.additionalInfo = myAddInfo;
    pd.dateTime = dateTime;

    ::broadcast(pd, mPort, QHostAddress(mMask));
}

void PulsarPrivate::sendActivate()
{
    ::sendActivate(QHostAddress(), mPort, QHostAddress(mMask), myAddInfo);
}

void PulsarPrivate::readyRead()
{
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    while(server->hasPendingDatagrams())
    {
        QByteArray datagramm;
        datagramm.resize(server->pendingDatagramSize());
        QHostAddress peerHost;
        quint16 port;
        server->readDatagram(datagramm.data(), datagramm.size(), &peerHost, &port);

        const PulsData pd = PulsData::fromByteArray(datagramm);
        if (pd.hasError())
        {
            stringToConsole(pd.errorString());
            return;
        }

        if (iam)
        {
            if (pd.status == PulsData::ACTIVE)
            {
                bool itIsMe = false;
                bool needBecomeSlave = (pd.dateTime <= dateTime);
                foreach (const QNetworkInterface interface, interfaces)
                {
                    foreach(const QNetworkAddressEntry entry, interface.addressEntries())
                    {
                        if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol)
                        {
                            if (QHostAddress(pd.pulsSourceIp) == entry.ip())
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
                    otherAddInfo = pd.additionalInfo;
                    emit stopped(QHostAddress(pd.pulsSourceIp), pd.information.join("\n"), otherAddInfo);
                }
            } else if (pd.status == PulsData::ACTIVATE)
            {
                bool itIsMe = false;
                QHostAddress host(pd.pulsSourceIp);
                foreach (const QNetworkInterface interface, interfaces)
                {
                    foreach(const QNetworkAddressEntry entry, interface.addressEntries())
                    {
                        if (host == entry.ip())
                        {
                            itIsMe = true;
                        }
                    }
                }
                if (!itIsMe) {
                    stringToConsole(tr("Becoming a slave..."));
                    iam = false;
                    otherAddInfo = pd.additionalInfo;
                    emit stopped(QHostAddress(pd.pulsSourceIp), pd.information.join("\n"), otherAddInfo);
                    Sleep::msleep(5000);
                    waitTimeout->start();
                }
            }

        } else // if i am not master
        {
            if (pd.status == PulsData::ACTIVE)
            {
                //if (lastpeer != peerHost)
                if (masterIdentificator != pd.dateTime)
                {
                    stringToConsole(tr("Master found"));
                    otherAddInfo = pd.additionalInfo;
                    masterIdentificator = pd.dateTime;
                    emit masterChanged(QHostAddress(pd.pulsSourceIp), pd.information.join("\n")
                                       , lastpeer, pd.additionalInfo);
                    lastpeer = QHostAddress(pd.pulsSourceIp);
                } else if (Compare::notEq(otherAddInfo, pd.additionalInfo))
                {
                    otherAddInfo = pd.additionalInfo;
                    emit otherMasterInfoChanged(lastpeer, otherAddInfo);
                }
                waitTimeout->start();
            } else if (pd.status == PulsData::ACTIVATE)
            {
                QHostAddress host(pd.pulsSourceIp);
                foreach (const QNetworkInterface interface, interfaces)
                {
                    foreach(const QNetworkAddressEntry entry, interface.addressEntries())
                    {
                        if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol)
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
