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
#define MESSAGE_CONCAT_SYMBOL     ('&')
#define MESSAGE_CONCAT_REPLACE    (' ')

/**
 * @brief Base class for threads
 *
 * @class AFTNWorkerBase aftnworkerbase.h
 */
class AFTNWorkerBase: public QThread
{
public:
  explicit AFTNWorkerBase(QObject * parent = 0);
  virtual void run() = 0;
};

class WorkerBase : public QObject
{
  Q_OBJECT
public:
  explicit WorkerBase(QObject* parent = 0);
  virtual ~WorkerBase();

protected:
  void init();
  void connectToDatabase();
  QString connectionName() {return mConnectionName; }

  QSettings* settings() {return mSettings;}
protected:
  void timerEvent(QTimerEvent* event);

private:
  int dbTimerId;
  QString mConnectionName;
  QSettings *mSettings;
};
#endif // AFTNWORKERBASE_H
