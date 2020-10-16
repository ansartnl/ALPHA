#include "tablecleaner.h"

#include "aftnmessageprocessor.h"

#include <QtDebug>

#include <QtCore/QDateTime>
#include <QtCore/QSettings>
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

TableCleaner::TableCleaner(QObject* parent) :
  AFTNWorkerBase(parent)
{
  if (objectName().isEmpty())
    setObjectName("table_cleaner");
}

void TableCleaner::run()
{
  qloginfo("Process cleaning tables...");
  TableCleanerBase ptr;
  if (!ptr.start())
    return;
  bool ok;
  ok = exec();
  ptr.stop();
  qloginfo("Stop process cleaning tables");
}

// --------------------------------------------------------------------------
TableCleanerBase::TableCleanerBase(QObject*parent) :
  WorkerBase(parent)
{
  init();

  queries.append(
    "delete from fpl"
      "   where (exfpl = 'C' or exfpl = 'A')"
      "     and time + TO_DSINTERVAL('0 '||eet||':00') < sys_extract_utc(systimestamp) - interval'%1' hour");
  queries.append("delete from arc_aftn"
    "   where created <= sys_extract_utc(systimestamp) - interval '%1' month");
  queries.append("delete from hist_fpl"
    "   where modified_at <= sys_extract_utc(systimestamp) - interval '%1' month");
  queries.append("delete from hist_man_aftn"
    "     where modified_at <= sys_extract_utc(systimestamp) - interval '%1' month");
  queries.append("delete from hist_rpl"
    "   where modified_at <= sys_extract_utc(systimestamp) - interval '%1' month");
  queries.append("delete from notam"
    "   where valid_until <= sys_extract_utc(systimestamp) - interval '%1' month");
  queries.append("delete from rpl where"
    "   valid_to <= sys_extract_utc(systimestamp) - interval '%1' month");
  queries.append("delete from users"
    "   where isactive = 0 " // nonactive
    "     and lasttouch <= sys_extract_utc(systimestamp) - interval '%1' month"
    "     and id != 0 and id != 1"); //system users
}

void TableCleanerBase::loadSettings()
{
  qloginfo("Load settings");
  settings()->beginGroup("table_cleaner");
  interval = settings()->value("timer_interval").toInt();
  fplProcessingHour = settings()->value("fpl_processing_hours").toInt();

  if (fplProcessingHour > 24)
    fplProcessingHour = 24;
  else if (fplProcessingHour < 1)
    fplProcessingHour = 1;

  arcAftnProcessingMonth
      = settings()->value("arc_aftn_processing_month").toInt();
  arcAftnProcessingMonth = arcAftnProcessingMonth > 1?1:arcAftnProcessingMonth;
  histFplProcessingMonth
      = settings()->value("hist_fpl_processing_month").toInt();
  histFplProcessingMonth = histFplProcessingMonth > 1?1:histFplProcessingMonth;
  histManAftnProcessingMonth = settings()->value(
    "hist_man_aftn_processing_month").toInt();
  histManAftnProcessingMonth = histManAftnProcessingMonth > 1?1:histManAftnProcessingMonth;
  histRplProcessingMonth
      = settings()->value("hist_rpl_processing_month").toInt();
  histRplProcessingMonth = histRplProcessingMonth > 1?1:histRplProcessingMonth;
  notamProcessingMonth = settings()->value("notam_processing_month").toInt();
  notamProcessingMonth = notamProcessingMonth > 1?1:notamProcessingMonth;
  rplProcessingMonth = settings()->value("rpl_processing_month").toInt();
  rplProcessingMonth = rplProcessingMonth > 1?1:rplProcessingMonth;
  usersProcessingMonth = settings()->value("users_processing_month").toInt();
  usersProcessingMonth = usersProcessingMonth > 1?1:usersProcessingMonth;

  settings()->endGroup();
}

void TableCleanerBase::initDefaultSettings()
{
  qloginfo("Init default settings");
  settings()->beginGroup("table_cleaner");
  settings()->setValue("comment", "Table cleaner parameters.");
  settings()->setValue("timer_interval", 5);
  settings()->setValue("timer_interval/comment",
    "С какой периодичностью запускать данный поток в СЕКУНДАХ.");
  settings()->setValue("fpl_processing_hours", 12);
  settings()->setValue("fpl_processing_hours/comment",
    "Через какое количество ЧАСОВ удалять FPL.");
  settings()->setValue("arc_aftn_processing_month", 2);
  settings()->setValue("arc_aftn_processing_month/comment",
    "Через какое количество месяцев очищать архив AFTN сообщений.");
  settings()->setValue("hist_fpl_processing_month", 2);
  settings()->setValue("hist_fpl_processing_month/comment",
    "Через какое количество месяцев очищать историю FPL сообщений.");
  settings()->setValue("hist_man_aftn_processing_month", 2);
  settings()->setValue(
    "hist_man_aftn_processing_month/comment",
    "Через какое количество месяцев очищать архив отредактированных AFTN сообщений.");
  settings()->setValue("hist_rpl_processing_month", 2);
  settings()->setValue("hist_rpl_processing_month/comment",
    "Через какое количество месяцев очищать архив RPL.");
  settings()->setValue("notam_processing_month", 2);
  settings()->setValue("notam_processing_month/comment",
    "Через какое количество месяцев очищать архив NOTAM сообщений.");
  settings()->setValue("rpl_processing_month", 2);
  settings()->setValue("rpl_processing_month/comment",
    "Через какое количество месяцев очищать RPL.");
  settings()->setValue("users_processing_month", 2);
  settings()->setValue("users_processing_month/comment",
    "Через какое количество месяцев очищать таблицу пользователей.");
  settings()->endGroup();
}

bool TableCleanerBase::start()
{
  if (!settings()->contains("table_cleaner/processing_time"))
    initDefaultSettings();

  loadSettings();

  queries[0] = queries.at(0).arg(fplProcessingHour);
  queries[1] = queries.at(1).arg(arcAftnProcessingMonth);
  queries[2] = queries.at(2).arg(histFplProcessingMonth);
  queries[3] = queries.at(3).arg(histManAftnProcessingMonth);
  queries[4] = queries.at(4).arg(histRplProcessingMonth);
  queries[5] = queries.at(5).arg(notamProcessingMonth);
  queries[6] = queries.at(6).arg(rplProcessingMonth);
  queries[7] = queries.at(7).arg(usersProcessingMonth);

  timerId = startTimer(interval * 1000);
  return QSqlDatabase::database(connectionName()).isOpen();
}

void TableCleanerBase::stop()
{
  killTimer(timerId);
}

void TableCleanerBase::timerEvent(QTimerEvent* event)
{
  if (event->timerId() == timerId) {
    //    qloginfo(tr("Process cleaning tables"));
    QSqlQuery query(QSqlDatabase::database(connectionName()));
    Q_FOREACH(const QString& q, queries)
      {
        if (!query.exec(q)) {
          qlogwarn(q);
          qlogwarn(tr("Query excution error. Executed query: %1\nError: %2").arg(query.lastQuery(), query.lastError().text()));
        }
      }
    return;
  }
  WorkerBase::timerEvent(event);
}
