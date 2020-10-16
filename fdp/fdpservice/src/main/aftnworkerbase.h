#ifndef AFTNWORKERBASE_H
#define AFTNWORKERBASE_H

#include <QtCore/QObject>
#include <QtCore/QThread>

#include <QtCore/QSettings>

#include "logger.h"

#define qlogger() Log4Qt::Logger::logger(objectName())
#define qlogdebug(x) Log4Qt::Logger::logger(objectName())->debug((x))
#define qloginfo(x) Log4Qt::Logger::logger(objectName())->info((x))
#define qlogwarn(x) Log4Qt::Logger::logger(objectName())->warn((x))
#define qlogerror(x) Log4Qt::Logger::logger(objectName())->error((x))

#define SERVICE_WORK_INTERVAL_SEC (5)

struct Sleep : public QThread { using QThread::msleep;
using QThread::sleep;};
class DbConnectionKeeper;
class WorkerBase;

/**
 * @brief Base class for threads
 *
 * @class AFTNWorkerBase aftnworkerbase.h
 */
class AFTNWorkerBase: public QThread
{
    Q_OBJECT
    Q_DISABLE_COPY(AFTNWorkerBase)
public:
    explicit AFTNWorkerBase(QObject * parent = 0);
    virtual void run() = 0;
    void stop();

    bool isActive() const;
    void setActive(bool active);

    void setWorker(WorkerBase *workerBase) { mWorkerBase = workerBase; }

private slots:
    void onStop();

private:
    /** Reference to worker */
    WorkerBase  *mWorkerBase;
};

class WorkerBase : public QObject
{
    Q_OBJECT
public:
    explicit WorkerBase(QObject* parent = 0);
    virtual ~WorkerBase();

    virtual bool start() = 0;
    virtual void stop() = 0;

    bool isDBConnectionValid() const;

    bool isActive() const { return mIsActive; }
    void setActive(bool active) { mIsActive = active; }

protected:
    void timerEvent(QTimerEvent *event);

    void init();
    void connectToDatabase();
    QString connectionName() {return mConnectionName; }
    DbConnectionKeeper* connectionKeeper() { return mConnKeeper; }

    QSettings* settings() {return mSettings;}
    const QSettings* settings() const {return mSettings;}

private slots:
    void onReconnected();
    void onDisconnected();

private:
    /** Does reconnection to database until connection is restored (if lost) */
    DbConnectionKeeper *mConnKeeper;
    /** Database connection name */
    QString mConnectionName;
    /** Access to settings */
    QSettings *mSettings;
    /** True when worker is active. */
    volatile bool mIsActive;
    /** Active state timer */
    int mActiveTimerId;
};

#endif // AFTNWORKERBASE_H
