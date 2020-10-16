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
    TableCleanerBase ptr;
    if (ptr.isDisabled())
        return;

    setWorker(&ptr);

    qloginfo("Process cleaning tables...");

    ptr.start();

    exec();

    ptr.stop();
    setWorker(0);

    qloginfo("Stop process cleaning tables");
}

// --------------------------------------------------------------------------
TableCleanerBase::TableCleanerBase(QObject*parent) :
    WorkerBase(parent), disable(true)
{
    if (objectName().isEmpty())
        setObjectName("table_cleaner");

    disable = settings()->value("table_cleaner/disable", false).toBool();
    if ( disable )
    {
        qlogwarn("Disabled");
        return;
    }

    init();
}

void TableCleanerBase::loadSettings()
{
    qloginfo("Load settings : Table cleaner");
    settings()->beginGroup("table_cleaner");
    interval = settings()->value("timer_interval").toInt();
    fplProcessingHour = settings()->value("fpl_processing_hours").toInt();

    if (fplProcessingHour > 24)
        fplProcessingHour = 24;
    else if (fplProcessingHour < 1)
        fplProcessingHour = 1;

    arcAftnProcessingDay = settings()->value("arc_aftn_processing_day", 1).toInt();
    manAftnProcessingDay = settings()->value("man_aftn_processing_day", 1).toInt();
    histFplProcessingDay = settings()->value("hist_fpl_processing_day", 1).toInt();
    histManAftnProcessingDay = settings()->value("hist_man_aftn_processing_day", 1).toInt();
    histRplProcessingDay = settings()->value("hist_rpl_processing_day", 1).toInt();
    notamProcessingDay = settings()->value("notam_processing_day", 1).toInt();
    rplProcessingDay = settings()->value("rpl_processing_day", 1).toInt();
    usersProcessingDay = settings()->value("users_processing_day", 1).toInt();
    archiveinputProcessingDay = settings()->value("archiveinput_processing_day", 10).toInt();
    archiveProcessingDay = settings()->value("archive_processing_day", 10).toInt();

    settings()->endGroup();
}

void TableCleanerBase::initDefaultSettings()
{
    qloginfo("Init default settings");
    settings()->beginGroup("table_cleaner");
    settings()->setValue("comment", "Table cleaner parameters.");
    settings()->setValue("disable", true);
    settings()->setValue("disable/comment", "Отключить.");
    settings()->setValue("timer_interval", 5);
    settings()->setValue("timer_interval/comment",
        "С какой периодичностью запускать данный поток в СЕКУНДАХ.");
    settings()->setValue("fpl_processing_hours", 12);
    settings()->setValue("fpl_processing_hours/comment",
        "Через какое количество ЧАСОВ удалять FPL.");
    settings()->setValue("arc_aftn_processing_day", 2);
    settings()->setValue("arc_aftn_processing_day/comment",
        "Через какое количество дней очищать архив AFTN сообщений.");
    settings()->setValue("man_aftn_processing_day", 1);
    settings()->setValue("man_aftn_processing_day/comment",
        "Через какое количество дней очищать таблицу manual AFTN.");
    settings()->setValue("hist_fpl_processing_day", 2);
    settings()->setValue("hist_fpl_processing_day/comment",
        "Через какое количество дней очищать историю FPL сообщений.");
    settings()->setValue("hist_man_aftn_processing_day", 2);
    settings()->setValue(
        "hist_man_aftn_processing_day/comment",
        "Через какое количество дней очищать архив отредактированных AFTN сообщений.");
    settings()->setValue("hist_rpl_processing_day", 2);
    settings()->setValue("hist_rpl_processing_day/comment",
        "Через какое количество дней очищать архив RPL.");
    settings()->setValue("notam_processing_day", 2);
    settings()->setValue("notam_processing_day/comment",
        "Через какое количество дней очищать архив NOTAM сообщений.");
    settings()->setValue("rpl_processing_day", 2);
    settings()->setValue("rpl_processing_day/comment",
        "Через какое количество дней очищать RPL.");
    settings()->setValue("users_processing_day", 2);
    settings()->setValue("users_processing_day/comment",
        "Через какое количество дней очищать таблицу пользователей.");
    settings()->setValue("archiveinput_processing_day", 2);
    settings()->setValue("archiveinput_processing_day/comment",
        "Через какое количество дней очищать таблицу входного архивирования.");
    settings()->setValue("archive_processing_day", 2);
    settings()->setValue("archive_processing_day/comment",
        "Через какое количество дней очищать таблицу архивирования.");
    settings()->endGroup();
}

bool TableCleanerBase::start()
{
    if (!settings()->contains("table_cleaner/timer_interval"))
        initDefaultSettings();

    loadSettings();

    queries.clear();

    queries.append(QString(
        "delete from fpl"
        "   where (exfpl = 'C' or exfpl = 'A')"
        "     and time + TO_DSINTERVAL('0 '||nvl(eet, '00:00')||':00') < sys_extract_utc(systimestamp) - interval '%1' hour").arg(fplProcessingHour));
    queries.append(QString(
        "delete from arc_aftn"
        "   where created <= sys_extract_utc(systimestamp) - interval '%1' day").arg(arcAftnProcessingDay));
    queries.append(QString(
        "delete from man_aftn"
        "   where created <= sys_extract_utc(systimestamp) - interval '%1' day").arg(manAftnProcessingDay));
    queries.append(QString(
        "delete from hist_fpl"
        "   where modified_at <= sys_extract_utc(systimestamp) - interval '%1' day").arg(histFplProcessingDay));
    queries.append(QString(
        "delete from hist_man_aftn"
        "     where modified_at <= sys_extract_utc(systimestamp) - interval '%1' day").arg(histManAftnProcessingDay));
    queries.append(QString(
        "delete from hist_rpl"
        "   where modified_at <= sys_extract_utc(systimestamp) - interval '%1' day").arg(histRplProcessingDay));
    queries.append(QString(
        "delete from notam"
        "   where valid_until <= sys_extract_utc(systimestamp) - interval '%1' day").arg(notamProcessingDay));
    queries.append(QString(
        "delete from rpl where"
        "   valid_to <= sys_extract_utc(systimestamp) - interval '%1' day").arg(rplProcessingDay));

    //system users
    queries.append(QString(
        "delete from users"
        "   where isactive = 0 " // nonactive
        "     and lasttouch <= sys_extract_utc(systimestamp) - interval '%1' day"
        "     and id != 0 and id != 1").arg(usersProcessingDay));


    queries.append(QString(
        "delete from archiveinputmessages where"
        "   msgtime <= sys_extract_utc(systimestamp) - interval '%1' day").arg(archiveinputProcessingDay));
    queries.append(QString(
        "delete from archivemessages where"
        "   msgtime <= sys_extract_utc(systimestamp) - interval '%1' day").arg(archiveProcessingDay));

    queries.append(QString(
        "delete from ffpl where"
        "   time <= sys_extract_utc(systimestamp) - interval '%1' day").arg(1)); // 1 day = 24 hours

    queries.append(QString(
        "delete from sent_aftn"
        "   where created <= sys_extract_utc(systimestamp) - interval '%1' day").arg(archiveProcessingDay));
    queries.append(QString(
        "delete from received_aftn"
        "   where created <= sys_extract_utc(systimestamp) - interval '%1' day").arg(archiveProcessingDay));

#if defined(DUSHANBE_BUILD) || defined(COMPLETE_BUILD)
    queries.append(QString(
        "delete from msg_from_tan where"
        "   oper_time <= sys_extract_utc(systimestamp) - interval '%1' hour").arg(1)); // 1 hour
#endif

#if defined(BAN_BUILD)
    queries.append(QString(
        "delete from trashtelegrams"
        "   where created <= sys_extract_utc(systimestamp) - interval '%1' day").arg(archiveProcessingDay));
#endif


    timerId = startTimer(interval * 1000);
    return QSqlDatabase::database(connectionName()).isOpen();
}

void TableCleanerBase::stop()
{
    killTimer(timerId);
}

void TableCleanerBase::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == timerId && isDBConnectionValid())
    {
        //    qloginfo(tr("Process cleaning tables"));
        QSqlQuery query(QSqlDatabase::database(connectionName()));
        Q_FOREACH(const QString& q, queries)
        {
            if (!query.exec(q)) {
                qlogwarn(q);
                qlogwarn(tr("Query excution error. Executed query: %1\nError: %2").arg(query.lastQuery(), query.lastError().text()));
            }
            setActive(true);
        }
        setActive(true);
    }
    WorkerBase::timerEvent(event);
}
