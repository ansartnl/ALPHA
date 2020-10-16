#ifndef AFTNWORKER_H
#define AFTNWORKER_H

#include "aftnworkerbase.h"

#include <QtCore/QDir>

class AFTNMessageProcessor;
/**
 * @brief Read messages from file, parse it, and insert into database. Invalid messages inserts into man_aftn table
 *
 * @class AFTNWorker aftnworker.h
 */
class FileWorker: public AFTNWorkerBase
{
public:
  FileWorker(QObject *parent = 0);
  virtual void run();
};

class FileWorkerBase: public WorkerBase
{
Q_OBJECT
  QScopedPointer<AFTNMessageProcessor> mProcessor;
  QDir messagesDir;
  QDir messagesDoneDir;
  QString fileExt;
public:
  FileWorkerBase(QObject* parent = 0);
  virtual ~FileWorkerBase();
public:

  bool start();
  void stop();

public slots:

protected:
  void loadSettings();
  void initDefaultSettings();

  void timerEvent(QTimerEvent *event);
private:
  int timerId;
  int interval;
};

#endif // AFTNWORKER_H
