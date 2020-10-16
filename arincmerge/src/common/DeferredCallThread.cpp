#include "DeferredCallThread.h"

DeferredCallThread::DeferredCallThread(QObject *parent) : QThread(parent)
{}

DeferredCallThread::~DeferredCallThread()
{
    if (isRunning())
        wait();
}

bool DeferredCallThread::call(DeferredProcedure *procedure)
{
    if (isRunning())
        return false;

    p = procedure;

    start();
    return true;
}

void DeferredCallThread::run()
{
    if (!p)
        return;
    p->call();
    p = 0;
}
