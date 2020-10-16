#include <QtDebug>

#include <QtCore/QDateTime>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>

#include "aftnservice.h"
#include "aftnmessageprocessor.h"
#include "ffplworker.h"

FFPLWorker::FFPLWorker(QObject* parent) :
  AFTNWorkerBase(parent)
{
  if (objectName().isEmpty())
    setObjectName("ffpl_worker");
}

void FFPLWorker::run()
{
  FFPLWorkerBase ptr;

  qloginfo("Processing ffpl...");
  if (!ptr.start())
    return;
  bool ok = exec();
  ptr.stop();
  qloginfo("Stop processing ffpl");
}

void FFPLWorker::process()
{
}

// ------------------------------------------------
FFPLWorkerBase::FFPLWorkerBase(QObject *parent) :
  WorkerBase(parent)
{
  init();
}

void FFPLWorkerBase::initDefaultSettings()
{
  qloginfo("Init default settings");
  settings()->beginGroup("ffpl_worker");
  settings()->setValue("timer_interval", 5);
  settings()->setValue("timer_interval/comment",
    "Таймер в СЕКУНДАХ срабатывания потока .");
  settings()->setValue("processing_time", 12);
  settings()->setValue("processing_time/comment",
    "На сколько ЧАСОВ вперед переносить Future FPL в FPL.");
  settings()->endGroup();
}

void FFPLWorkerBase::loadSettings()
{
  qloginfo("Load settings");
  settings()->beginGroup("ffpl_worker");
  interval = settings()->value("timer_interval").toInt();
  processingTime = settings()->value("processing_time").toInt();
  settings()->endGroup();
  if (processingTime > 24)
    processingTime = 24;
  else if (processingTime < 1)
    processingTime = 1;
}

bool FFPLWorkerBase::start()
{
  if (!settings()->contains("ffpl_worker/processing_time"))
    initDefaultSettings();

  loadSettings();

  timerId = startTimer(interval * 1000);
  return QSqlDatabase::database(connectionName()).isOpen();
}

void FFPLWorkerBase::stop()
{
  killTimer(timerId);
}

void FFPLWorkerBase::timerEvent(QTimerEvent* event)
{
  if (timerId == event->timerId()) {
    QString
        queryString =
            QString(
              " SELECT "
              TBL_FPL_ID ", "
              TBL_FPL_FIR_NAME ", "
              TBL_FPL_AIRCRAFT_ID ", "
              TBL_FPL_FRULES ", "
              TBL_FPL_TFLIGHT ", "
              TBL_FPL_NUM ", "
              TBL_FPL_TAIRCRAFT ", "
              TBL_FPL_TURBULENCE_CATEGORY ", "
              TBL_FPL_EQUIPMENT ", "
              TBL_FPL_ADEP ", "
              TBL_FPL_TIME ", "
              TBL_FPL_SPEED ", "
              TBL_FPL_FLEVEL ", "
              TBL_FPL_ROUTE ", "
              TBL_FPL_ADES ", "
              TBL_FPL_EET ", "
              TBL_FPL_ALT1 ", "
              TBL_FPL_ALT2 ", "
              TBL_FPL_OTHER ", "
              TBL_FPL_TRTIME ", "
              TBL_FPL_MESSAGE_ID ", "
              TBL_FPL_ENTRY_FIR_EET " "
              " FROM "
              TBL_FFPL " "
              " WHERE "
              TBL_FPL_TIME" <= SYS_EXTRACT_UTC(SYSTIMESTAMP) + interval '%1' hour "
              " AND "
              " NOT EXISTS ( "
              " SELECT "
              " * "
              " FROM " TBL_FPL " "
              " WHERE " TBL_FPL "." TBL_FPL_AIRCRAFT_ID "=" TBL_FFPL "." TBL_FPL_AIRCRAFT_ID
              " AND " TBL_FPL "." TBL_FPL_ADEP "=" TBL_FFPL "." TBL_FPL_ADEP
              " AND " TBL_FPL "." TBL_FPL_ADES "=" TBL_FFPL "." TBL_FPL_ADES
              " )").arg(processingTime);

    QSqlQuery selectQuery(QSqlDatabase::database(connectionName()));

    if (!selectQuery.exec(queryString)) {
      qlogwarn(tr("Query execution error. Executed query: %1\nError: %2")
          .arg(selectQuery.lastQuery(),
              selectQuery.lastError().text()));
      return;
    }

    while (selectQuery.next()) {
      // move from FFPL to FPL
      QSqlQuery query(QSqlDatabase::database(connectionName()));

      query.prepare(QString(TBL_FPL_INSERT_FORMAT).arg(TBL_FPL));

      query.addBindValue(selectQuery.record().value(TBL_FPL_FIR_NAME));
      query.addBindValue(selectQuery.record().value(TBL_FPL_AIRCRAFT_ID));
      query.addBindValue(selectQuery.record().value(TBL_FPL_FRULES));
      query.addBindValue(selectQuery.record().value(TBL_FPL_TFLIGHT));
      query.addBindValue(selectQuery.record().value(TBL_FPL_NUM));
      query.addBindValue(selectQuery.record().value(TBL_FPL_TAIRCRAFT));
      query.addBindValue(selectQuery.record().value(TBL_FPL_TURBULENCE_CATEGORY));
      query.addBindValue(selectQuery.record().value(TBL_FPL_EQUIPMENT));
      query.addBindValue(selectQuery.record().value(TBL_FPL_ADEP));
      query.addBindValue(selectQuery.record().value(TBL_FPL_TIME));
      query.addBindValue(selectQuery.record().value(TBL_FPL_SPEED));
      query.addBindValue(selectQuery.record().value(TBL_FPL_FLEVEL));
      query.addBindValue(selectQuery.record().value(TBL_FPL_ROUTE));
      query.addBindValue(selectQuery.record().value(TBL_FPL_ADES));
      query.addBindValue(selectQuery.record().value(TBL_FPL_EET));
      query.addBindValue(selectQuery.record().value(TBL_FPL_ALT1));
      query.addBindValue(selectQuery.record().value(TBL_FPL_ALT2));
      query.addBindValue(selectQuery.record().value(TBL_FPL_OTHER));
      query.addBindValue(selectQuery.record().value(TBL_FPL_MESSAGE_ID));
      query.addBindValue(selectQuery.record().value(TBL_FPL_ENTRY_FIR_EET));

      if (!query.exec()) {
        qlogwarn(tr("Query execution error. Executed query: %1\nError: %2").arg(query.lastQuery(),
                query.lastError().text()));
        break;
      }

      query.prepare("DELETE FROM " TBL_FFPL " WHERE " TBL_FPL_ID " = ?");
      query.addBindValue(selectQuery.record().value(TBL_FPL_ID));
      if (!query.exec()) {
        qlogwarn(tr("Query execution error. Executed query: %1\nError: %2").arg(query.lastQuery(),
                query.lastError().text()));
        break;
      }
    }
    return;
  }
  WorkerBase::timerEvent(event);
}
