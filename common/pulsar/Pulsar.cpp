#include "Pulsar.h"
#include "PulsarPrivate.h"

Pulsar::Pulsar(bool passive, quint16 port, const QString &additionalStatus, QObject *parent)
    //:QThread(parent), mAdditionalStatus(additionalStatus), mPort(port), passive(passive), p(0)
    :QThread(0), mAdditionalStatus(additionalStatus), mPort(port), passive(passive), p(0) //!!! without parent
{
    setObjectName("Pulsar");
    mCheckSubnet = true;
    moveToThread(this);

    removeOnQuit = false;
    if (parent)
        connect(parent, SIGNAL(destroyed()), SLOT(onDelete()));
}

Pulsar::Pulsar(QObject *parent)
    //: QThread(parent), mPort(0), passive(false)
    : QThread(0), mPort(0), passive(false) //!!! without parent
{
    setObjectName("Pulsar");
    mCheckSubnet = true;
    moveToThread(this);

    removeOnQuit = false;
    if (parent)
        connect(parent, SIGNAL(destroyed()), SLOT(onDelete()));
}

Pulsar::~Pulsar()
{
}

void Pulsar::run()
{
    PulsarPrivate d(passive, mPort, mAdditionalStatus, mAdditionalInfo, 0, mServerAddress);
    d.setMask(mMask);
    d.setBroadcast(mBroadcastList);
    p = &d;
    connect(p, SIGNAL(started()), this, SIGNAL(broadcastStarted()));
    connect(p, SIGNAL(stopped(QHostAddress,QString,QVariantHash))
            , SIGNAL(broadcastStopped(QHostAddress,QString,QVariantHash)));
    connect(this, SIGNAL(additionalStatusChanged(QString)), p, SLOT(setAdditionalStatus(QString)));

    connect(this, SIGNAL(p_deactivate()), p, SLOT(deactivate()));

    connect(p, SIGNAL(masterChanged(QHostAddress,QString,QHostAddress,QVariantHash))
            , SIGNAL(masterChanged(QHostAddress,QString,QHostAddress,QVariantHash)));
    connect(p, SIGNAL(otherMasterInfoChanged(QHostAddress,QVariantHash))
            , SIGNAL(otherMasterInfoChanged(QHostAddress,QVariantHash)));

    exec();
    p = 0;
}

void Pulsar::setAdditionalStatus(const QString &status)
{
    mAdditionalStatus = status;
    emit additionalStatusChanged(status);
}

void Pulsar::setAdditionalInfo(const QVariantHash &info)
{
    mAdditionalInfo = info;
    if (p)
        PulsarPrivate::staticMetaObject.invokeMethod(p, "setAdditionalInfo"
                                                     , Q_ARG(QVariantHash, info));
}

void Pulsar::addAdditionalInfo(const QString &key, const QVariant &value)
{
    mAdditionalInfo.insert(key, value);
    if (p)
        PulsarPrivate::staticMetaObject.invokeMethod(p, "addAdditionalInfo"
                                                     , Q_ARG(QString, key)
                                                     , Q_ARG(QVariant, value));
}

void Pulsar::removeAdditionalInfo(const QString &key)
{
    mAdditionalInfo.remove(key);
    if (p)
        PulsarPrivate::staticMetaObject.invokeMethod(p, "removeAdditionalInfo"
                                                     , Q_ARG(QString, key));
}

void Pulsar::activatePulsar()
{
    PulsarPrivate::staticMetaObject.invokeMethod(p, "sendActivate");
}

void Pulsar::deactivate()
{
    emit p_deactivate();
}

void Pulsar::stop()
{
    if(isRunning())
    {
        onStop();
        if(!isFinished())
        {
            wait(5000);
        }
        if(!isFinished())
        {
            qDebug() << tr("Thread terminated.");
            terminate();
        }
    }
}

void Pulsar::onStop()
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

void Pulsar::onDelete()
{
    removeOnQuit = true;
    QMetaObject::invokeMethod(this, "onStop", Qt::QueuedConnection);
}
