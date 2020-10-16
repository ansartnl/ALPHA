#ifndef SLEEP_H
#define SLEEP_H

#include <QtCore/QThread>

struct Sleep : public QThread
{
    using QThread::sleep;
    using QThread::msleep;
    using QThread::usleep;
};

#endif // SLEEP_H
