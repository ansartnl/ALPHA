#ifndef RESTARTER_H
#define RESTARTER_H

#include <QtCore/QThread>

class AFTNWorkerBase;
class Restarter;


//! Helper to run timer for checking workers state.
class RestarterObject : public QObject
{
    Q_OBJECT
public:
    RestarterObject(Restarter *restarter, QObject* parent = 0);

    void start(int interval);
    void stop();

protected:
    void timerEvent(QTimerEvent *event);

private:
    /** Reference to owner thread */
    Restarter *mRestarter;
    /** Worker threads checker timer */
    int mTimerId;
};


//! Restarts application when at least one of workers hangs (becomes inactive)
class Restarter : public QThread
{
    Q_OBJECT
    friend class RestarterObject;

public:
    Restarter(QObject *parent);

    void setWorkers(const QList<AFTNWorkerBase*> &workers) { mWorkers = workers; }
    void setScriptPath(const QString &path) { mScriptPath = path; }
    void setInterval(int interval);

private:
    virtual void run();

    /** Available list of workers */
    QList<AFTNWorkerBase*> mWorkers;
    /** Path to script for restart the app */
    QString mScriptPath;
    /** Interval for checking thread states */
    int mTimerInterval;
};

#endif  // RESTARTER_H
