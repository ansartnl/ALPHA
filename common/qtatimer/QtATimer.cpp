#include "QtATimer.h"

#include <QtCore/QTimer>

QtATimer::QtATimer(QObject *parent) :
    QObject(parent)
{
    init();
}

QtATimer::~QtATimer()
{
    stop();
}

void QtATimer::singleShot(const QTime &time, QObject *receiver, const char *member)
{
    if (receiver && member)
    {
        QtATimer *t = new QtATimer();
        connect(t, SIGNAL(timeout()), receiver, member);
        connect(t, SIGNAL(timeout()), t, SLOT(deleteLater()));
        t->setSingleShot(true);
        t->start(time);
    }
}

void QtATimer::start(const QTime &time)
{
    setTime(time);
    start();
}

void QtATimer::start()
{
    if (isActive())
        stop();
    mIsActive = true;
    mTimer->setInterval(mAccuracy);
    mTimer->start();
}

void QtATimer::stop()
{
    mIsActive = false;
    mTimer->stop();
}

void QtATimer::startNow()
{
    start(QTime::currentTime());
    onFinalTimeout();
}

void QtATimer::init()
{
    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), SLOT(onTimeout()));

    mTime = QTime(0, 0);
    mAccuracy = 5000;
    mIsSingleShot = false;
    mIsActive = false;
    mIsUTC = false;
}

QTime QtATimer::currentTime() const
{
    if (mIsUTC)
        return QDateTime::currentDateTimeUtc().time();
    return QDateTime::currentDateTime().time();
}

void QtATimer::onTimeout()
{
    int t = currentTime().msecsTo(mTime);
    if (t < -12*60*60*1000)
        t += 24*60*60*1000;
    if (qAbs(t) < mAccuracy)
    {
        if (t > 0)
            QTimer::singleShot(t, this, SLOT(onFinalTimeout()));
    }
}

void QtATimer::onFinalTimeout()
{
    emit timeout();
    if (isSingleShot())
        stop();
}
