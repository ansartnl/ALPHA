#ifndef DEFERREDCALLTHREAD_H
#define DEFERREDCALLTHREAD_H

#include "DeferredProcedure.h"
#include <QtCore/QThread>

class DeferredCallThread : public QThread
{
public:
    DeferredCallThread(QObject *parent = 0);
    virtual ~DeferredCallThread();

    bool call(DeferredProcedure *procedure);

protected:
    virtual void run();

private:
    DeferredProcedure *p;
};

#endif // DEFERREDCALLTHREAD_H
