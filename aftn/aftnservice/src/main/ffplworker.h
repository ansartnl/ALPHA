#ifndef FFPLWORKER_H
#define FFPLWORKER_H

#include "aftnworkerbase.h"

/**
* @brief Generate FPL from FFPL
*
* @class FFPLWorker ffplworker.h
*/
class FFPLWorker : public AFTNWorkerBase
{
  public:
    FFPLWorker(QObject* parent = 0);

    virtual void run();
    void process();
};

class FFPLWorkerBase : public WorkerBase
{
  Q_OBJECT
public:
  FFPLWorkerBase(QObject* parent = 0);

  bool start();
  void stop();
protected:
  void timerEvent(QTimerEvent* event);
  void initDefaultSettings();
  void loadSettings();

private:
  int timerId;
  int interval;

  int processingTime;
};

#endif // FFPLWORKER_H
