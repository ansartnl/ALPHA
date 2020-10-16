#ifndef FPLREMOVER_H
#define FPLREMOVER_H

#include "aftnworkerbase.h"
#include "aftnservice.h"

/**
 * @brief Remove FPL when eet time is less than (now - processing_time/fpl_remover) and exfpl have arrived or cancelled status
 *
 * @class FplRemover fplremover.h
 */
class TableCleaner : public AFTNWorkerBase
{
public:
    TableCleaner(QObject *parent);

    virtual void run();
};

class TableCleanerBase : public WorkerBase
{
    Q_OBJECT
public:
    TableCleanerBase(QObject* parent = 0);

    bool start();
    void stop();

    bool isDisabled() const { return disable; }

protected:
    void loadSettings();
    void initDefaultSettings();

    void timerEvent(QTimerEvent *event);

private:
    int timerId;
    int interval;
    int fplProcessingHour;
    int arcAftnProcessingDay;
    int manAftnProcessingDay;
    int histFplProcessingDay;
    int histManAftnProcessingDay;
    int histRplProcessingDay;
    int notamProcessingDay;
    int rplProcessingDay;
    int usersProcessingDay;
    int archiveinputProcessingDay;
    int archiveProcessingDay;

    bool disable;

    QStringList queries;
};

#endif
