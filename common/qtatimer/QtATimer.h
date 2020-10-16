#ifndef QTXTIMER_H
#define QTXTIMER_H

#include <QtCore/QObject>
#include <QtCore/QTime>

class QTimer;

/*!
  * \class QtATimer
  * \brief Таймер абсолютного вермени
  *
  * Данный класс представляет собой таймер, срабатывающий в определенное время
  * (в отличие от QTimer, срабатывающего через промежуток времени).
  * В остальном предоставляет возможности, аналогичные QTimer.
  */

class QtATimer : public QObject
{
    Q_OBJECT
public:
    explicit QtATimer(QObject *parent = 0);
    ~QtATimer();

    inline void setTime(const QTime &time, bool utc = false) { mTime = time; mIsUTC = utc; }
    inline QTime time() const { return mTime; }

    // step of internal timer, default is 5 sec
    inline void setAccuracy(int msec) { mAccuracy = msec; }
    inline int accuracy() { return mAccuracy; }

    inline void setSingleShot(bool singleShot) { mIsSingleShot = singleShot; }
    inline bool isSingleShot() { return mIsSingleShot; }

    inline bool isActive() { return mIsActive; }

    inline bool isUTC() { return mIsUTC; }

    static void singleShot(const QTime &time, QObject *receiver, const char *member);

public slots:
    void start(const QTime &time);
    void start();
    void stop();

    // emits timeout and then starts timer with time == QTime::currentTime()
    void startNow();

signals:
    void timeout();

protected:
    void init();
    QTime currentTime() const;

protected slots:
    void onTimeout();
    void onFinalTimeout();

protected:
    QTimer *mTimer;
    QTime mTime;
    int mAccuracy;
    bool mIsSingleShot;
    bool mIsActive;
    bool mIsUTC;
};

#endif // QTXTIMER_H
