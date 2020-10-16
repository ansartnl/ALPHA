#ifndef RPLWORKER_H
#define RPLWORKER_H

#include "aftnworkerbase.h"

/**
 * @brief Thread class for generating FPL from RPL
 *
 * @class RPLWorker rplworker.h
 */
class RPLWorker: public AFTNWorkerBase //Qthread
{
public:
  RPLWorker(QObject* parent = 0);

  virtual void run();
  void process();

};

class RPLWorkerBase: public WorkerBase // QObject
{
Q_OBJECT
public:
  RPLWorkerBase(QObject *parent = 0);

  bool start();
  void stop();
protected:
  void initDefaultSettings();
  void loadSettings();
  void timerEvent(QTimerEvent *event);

private:
  int timerId;
  int interval;

  int processingTime;
};

#endif // RPLWORKER_H
