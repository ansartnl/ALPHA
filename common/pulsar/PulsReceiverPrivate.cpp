#include "PulsReceiverPrivate.h"
#include "PulsarShareFunctions.h"

#include "Compare.h"

#include <QtCore/QDataStream>
#include <QtCore/QTimer>

#include <QtNetwork/QUdpSocket>

PulsReceiverPrivate::PulsReceiverPrivate(quint16 port, QObject *parent)
    :QObject(parent), client(new QUdpSocket(this)), masterLost(new QTimer(this)), port(port)
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

void PulsReceiverPrivate::readyRead()
{
    while(client->hasPendingDatagrams())
    {
        QByteArray datagramm;
        datagramm.resize(client->pendingDatagramSize());
        QHostAddress peerHost;
        quint16 port;
        client->readDatagram(datagramm.data(), datagramm.size(), &peerHost, &port);

        QString strdata;
        QVariantHash addInfo;
        QDateTime dateTime;
        QDataStream stream(datagramm);
        stream >> strdata;
        stream >> addInfo;
        stream >> dateTime;
        if (stream.status() != QDataStream::Ok)
            continue;

        if (!strdata.startsWith("2"))
        {
            emit status(peerHost, strdata.mid(1), addInfo);
        }

        if (strdata.startsWith("1"))
        {
            masterLost->stop();
            if (masterIdentificator != dateTime)
            {
                stringToConsole(tr("Master found: ")) << peerHost;
                emit pulsarChanged(peerHost, strdata.mid(1), lastpeer, addInfo);
                lastpeer = peerHost;
                masterIdentificator = dateTime;
                masterInfo = addInfo;
                masterPing = 0;
                masterPingTimer->start();
            } else if (Compare::notEq(masterInfo, addInfo))
            {
                masterInfo = addInfo;
                emit masterInfoChanged(peerHost, addInfo);
            }

            if (masterIdentificator == dateTime)
            {
                if (lastpeer == peerHost)
                {
                    masterPing = 0;
                } else if (masterPing > 3)
                {
                    stringToConsole(tr("Master address changed: ")) << peerHost;
                    emit pulsarChanged(peerHost, strdata.mid(1), lastpeer, addInfo);
                    lastpeer = peerHost;
                    masterIdentificator = dateTime;
                    masterInfo = addInfo;
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
