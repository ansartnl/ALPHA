#include "PulsReceiver.h"
#include "PulsReceiverPrivate.h"

PulsReceiver::PulsReceiver(QObject *parent)
    :QThread(0), mPort(0), mRepiter(false), p(0), removeOnQuit(false)
{
    setObjectName("PulsReceiver");
    moveToThread(this);
    if(parent)
        connect(parent, SIGNAL(destroyed()), SLOT(onDelete()));
}

PulsReceiver::PulsReceiver(quint16 port, QObject *parent)
    :QThread(0), mPort(port), p(0), removeOnQuit(false)
{
    setObjectName("PulsReceiver");
    moveToThread(this);
    if(parent)
        connect(parent, SIGNAL(destroyed()), SLOT(onDelete()));
}

PulsReceiver::~PulsReceiver()
{
}

void PulsReceiver::run()
{
    PulsReceiverPrivate d(mRepiter, mPort);
    d.setMask(mMask);

    p = &d;
    connect(p, SIGNAL(pulsarChanged(QHostAddress,QString,QHostAddress,QVariantHash))
            , SIGNAL(pulsarChanged(QHostAddress,QString,QHostAddress,QVariantHash)));
    connect(p, SIGNAL(status(QHostAddress,QString,QVariantHash))
            , SIGNAL(status(QHostAddress,QString,QVariantHash)));
    connect(p, SIGNAL(masterInfoChanged(QHostAddress,QVariantHash))
            , SIGNAL(masterInfoChanged(QHostAddress,QVariantHash)));
    exec();
    p = 0;
}

void PulsReceiver::activatePulsar(const QHostAddress &newpulsar)
{
    if (p)
        staticMetaObject.invokeMethod(p, "activatePulsar", Qt::QueuedConnection
                                      , Q_ARG(QHostAddress, newpulsar));
}

void PulsReceiver::stop()
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
