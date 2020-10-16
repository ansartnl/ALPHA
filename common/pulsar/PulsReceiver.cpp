#include "PulsReceiver.h"
#include "PulsReceiverPrivate.h"

#include <QtCore/QDataStream>
#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QUdpSocket>

PulsReceiver::PulsReceiver(QObject *parent)
    :QThread(0), mPort(0), p(0), removeOnQuit(false)
{
    setObjectName("PulsReceiver");
    moveToThread(this);
    if (parent)
        connect(parent, SIGNAL(destroyed()), SLOT(onDelete()));
}

PulsReceiver::PulsReceiver(quint16 port, QObject *parent)
    :QThread(0), mPort(port), p(0), removeOnQuit(false)
{
    setObjectName("PulsReceiver");
    moveToThread(this);
    if (parent)
        connect(parent, SIGNAL(destroyed()), SLOT(onDelete()));
}

PulsReceiver::~PulsReceiver()
{
}

void PulsReceiver::run()
{
    PulsReceiverPrivate d(mPort);

    p = &d;
    connect(p, SIGNAL(pulsarChanged(QHostAddress,QString,QHostAddress,QVariantHash)), this, SIGNAL(pulsarChanged(QHostAddress,QString,QHostAddress,QVariantHash)));
    connect(p, SIGNAL(status(QHostAddress,QString,QVariantHash)), this, SIGNAL(status(QHostAddress,QString,QVariantHash)));
    connect(p, SIGNAL(masterInfoChanged(QHostAddress,QVariantHash)), this, SIGNAL(masterInfoChanged(QHostAddress,QVariantHash)));
    exec();
    p = 0;
}

void PulsReceiver::onActivatePulsar(const QHostAddress &newpulsar)
{
    QScopedPointer<QUdpSocket> client(new QUdpSocket());
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    foreach (const QNetworkInterface &interface, interfaces)
    {
        if (interface.CanBroadcast)
        {
            foreach (const QNetworkAddressEntry &entry, interface.addressEntries())
            {
                if (entry.ip().protocol() == QUdpSocket::IPv4Protocol)
                {
                    QByteArray data;
                    QString strdata = "2";
                    strdata.append(newpulsar.toString());
                    QDataStream stream(&data, QIODevice::WriteOnly);
                    stream << strdata;
                    stream << QVariantHash();
                    stream << QDateTime(QDate(0, 0, 0), QTime(0, 0));
                    client->writeDatagram(data, entry.ip(), mPort);
                    client->writeDatagram(data, entry.broadcast(), mPort);
                }
            }
        }
    }
}

void PulsReceiver::activatePulsar(const QHostAddress &newpulsar)
{
    staticMetaObject.invokeMethod(this, "onActivatePulsar", Qt::QueuedConnection
                                  , Q_ARG(QHostAddress, newpulsar));
}

void PulsReceiver::stop()
{
    if (isRunning())
    {
        onStop();
        if (!isFinished())
        {
            wait(5000);
        }
        if (!isFinished())
        {
            qDebug() << tr("Thread terminated.");
            terminate();
        }
    }
}

void PulsReceiver::onStop()
{
    if(currentThread() != this)
    {
        // The caller is running in a
        // different thread, so use Qt to
        // call onStop() later, on our own thread:
        QMetaObject::invokeMethod(this, "onStop", Qt::QueuedConnection);
    }
    else
    {
        // Now the call has arrived from our
        // own thread, yay! We can safely
        // shut down our event loop.
        quit();
        if (removeOnQuit)
            deleteLater();
    }
}

void PulsReceiver::onDelete()
{
    removeOnQuit = true;
    QMetaObject::invokeMethod(this, "onStop", Qt::QueuedConnection);
}
