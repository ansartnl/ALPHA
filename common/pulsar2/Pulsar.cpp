#include "Pulsar.h"
#include "PulsarPrivate.h"

Pulsar::Pulsar(QObject *parent)
    : QThread(0), mPort(0), passive(false)
{
    setObjectName("Pulsar");
    moveToThread(this);

    removeOnQuit = false;
    if (parent)
        connect(parent, SIGNAL(destroyed()), SLOT(onDelete()));
}

Pulsar::Pulsar(bool passive, quint16 port, const QString &, QObject *parent)
    :QThread(0), mPort(port), passive(passive), p(0)
{
    setObjectName("Pulsar");
    moveToThread(this);

    removeOnQuit = false;
    if (parent)
        connect(parent, SIGNAL(destroyed()), SLOT(onDelete()));
}

Pulsar::Pulsar(bool passive, quint16 port, QObject *parent)
    :QThread(0), mPort(port), passive(passive), p(0)
{
    setObjectName("Pulsar");
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
    PulsarPrivate d(passive, mPort, mAdditionalInfo);
    d.setMask(mMask);
    p = &d;
    connect(p, SIGNAL(started()), SIGNAL(broadcastStarted()));
    connect(p, SIGNAL(stopped(QHostAddress,QString,QVariantHash))
            , SIGNAL(broadcastStopped(QHostAddress,QString,QVariantHash)));

    connect(this, SIGNAL(p_deactivate()), p, SLOT(deactivate()));

    connect(p, SIGNAL(masterChanged(QHostAddress,QString,QHostAddress,QVariantHash))
            , SIGNAL(masterChanged(QHostAddress,QString,QHostAddress,QVariantHash)));
    connect(p, SIGNAL(otherMasterInfoChanged(QHostAddress,QVariantHash))
            , SIGNAL(otherMasterInfoChanged(QHostAddress,QVariantHash)));

    exec();
    p = 0;
}

void Pulsar::setAdditionalInfo(const QVariantHash &info)
{
    mAdditionalInfo = info;
    if (p)
        staticMetaObject.invokeMethod(p, "setAdditionalInfo", Qt::QueuedConnection
                                      , Q_ARG(QVariantHash, info));
}

void Pulsar::addAdditionalInfo(const QString &key, const QVariant &value)
{
    mAdditionalInfo.insert(key, value);
    if (p)
        staticMetaObject.invokeMethod(p, "addAdditionalInfo", Qt::QueuedConnection
                                      , Q_ARG(QString, key)
                                      , Q_ARG(QVariant, value));
}

void Pulsar::removeAdditionalInfo(const QString &key)
{
    mAdditionalInfo.remove(key);
    if (p)
        staticMetaObject.invokeMethod(p, "removeAdditionalInfo", Qt::QueuedConnection
                                      , Q_ARG(QString, key));
}

void Pulsar::activatePulsar()
{
    staticMetaObject.invokeMethod(p, "sendActivate", Qt::QueuedConnection);
}

void Pulsar::deactivate()
{
    emit p_deactivate();
}

void Pulsar::stop()
{
    if (isRunning())
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
    if (currentThread() != this)
    {
        // The caller is running in a
        // different thread, so use Qt to
        // call onStop() later, on our own thread:
        QMetaObject::invokeMethod(this, "onStop", Qt::QueuedConnection);
    } else
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
