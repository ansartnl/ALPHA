#include "PulsReceiverPrivate.h"

#include "PulsData.h"
#include "PulsarShareFunctions.h"

#include "Compare.h"

#include <QtCore/QTimer>

#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QUdpSocket>

PulsReceiverPrivate::PulsReceiverPrivate(bool repiter, quint16 port, QObject *parent)
    :QObject(parent), client(new QUdpSocket(this)), masterLost(new QTimer(this)), mPort(port), mRepiter(repiter)
{
    masterPing = 0;
    masterPingTimer = new QTimer(this);
    connect(masterPingTimer, SIGNAL(timeout()), SLOT(onMasterPingTimer()));
    masterPingTimer->setInterval(mSendTime);
    masterPingTimer->setSingleShot(false);

    connect(client, SIGNAL(readyRead()), SLOT(readyRead()));
    client->bind(port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

    connect(masterLost, SIGNAL(timeout()), SLOT(masterLosted()));
    masterLost->setInterval(mReceiveTime);
    masterLost->setSingleShot(true);
    masterLost->start();
}

void PulsReceiverPrivate::activatePulsar(const QHostAddress &newpulsar)
{
    ::sendActivate(newpulsar, mPort, QHostAddress(mMask));
}

void PulsReceiverPrivate::readyRead()
{
    while(client->hasPendingDatagrams())
    {
        QByteArray datagramm;
        datagramm.resize(client->pendingDatagramSize());
        QHostAddress peerHost;
        quint16 port;
        client->readDatagram(datagramm.data(), datagramm.size(), &peerHost, &port);

        const PulsData pd = PulsData::fromByteArray(datagramm);
        if (pd.hasError())
        {
            stringToConsole(pd.errorString());
            return;
        }
        if (mRepiter)
        {
            bool itIsMe = false;
            QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
            foreach (const QNetworkInterface interface, interfaces)
            {
                foreach(const QNetworkAddressEntry entry, interface.addressEntries())
                {
                    if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol)
                    {
                        if (peerHost == entry.ip())
                        {
                            itIsMe = true;
                        }
                    }
                }
            }
            if (!itIsMe)
                ::broadcast(datagramm, mPort, QHostAddress(mMask));
        }

        if (pd.status != PulsData::ACTIVATE)
        {
            emit status(QHostAddress(pd.pulsSourceIp), pd.information.join("\n"), pd.additionalInfo);
        }

        if (pd.status == PulsData::ACTIVE)
        {
            masterLost->stop();
            if (masterIdentificator != pd.dateTime)
            {
                stringToConsole(tr("Master found: ")) << peerHost;
                emit pulsarChanged(QHostAddress(pd.pulsSourceIp), pd.information.join("\n")
                                   , lastpeer, pd.additionalInfo);
                lastpeer = QHostAddress(pd.pulsSourceIp);
                masterIdentificator = pd.dateTime;
                masterInfo = pd.additionalInfo;
                masterPing = 0;
                masterPingTimer->start();
            } else if (Compare::notEq(masterInfo, pd.additionalInfo))
            {
                masterInfo = pd.additionalInfo;
                emit masterInfoChanged(QHostAddress(pd.pulsSourceIp), pd.additionalInfo);
            }

            if (masterIdentificator == pd.dateTime)
            {
                if (lastpeer == peerHost)
                {
                    masterPing = 0;
                } else if (masterPing > 3)
                {
                    stringToConsole(tr("Master address changed: ")) << peerHost;
                    emit pulsarChanged(QHostAddress(pd.pulsSourceIp), pd.information.join("\n")
                                       , lastpeer, pd.additionalInfo);
                    lastpeer = QHostAddress(pd.pulsSourceIp);
                    masterIdentificator = pd.dateTime;
                    masterInfo = pd.additionalInfo;
                    masterPing = 0;
                }
            }
        }
        masterLost->start();
    }
}

void PulsReceiverPrivate::masterLosted()
{
    stringToConsole(tr("Master lost"));
    emit pulsarChanged(QHostAddress(), QString(), lastpeer, QVariantHash());
    lastpeer = QHostAddress();
    masterIdentificator = QDateTime();
    masterInfo = QVariantHash();
    masterPing = 0;
    masterPingTimer->stop();
}

void PulsReceiverPrivate::onMasterPingTimer()
{
    ++masterPing;
}
