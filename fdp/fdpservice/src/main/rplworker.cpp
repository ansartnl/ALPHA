#include <QtDebug>

#include <QtCore/QTimerEvent>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

#include "logger.h"

#include "aftnmessageprocessor.h"
#include "rplworker.h"

RPLWorker::RPLWorker(QObject *parent) :
    AFTNWorkerBase(parent)
{
    if (objectName().isEmpty())
        setObjectName("rpl_worker");
}

void RPLWorker::run()
{
    RPLWorkerBase ptr;
    setWorker(&ptr);

    qloginfo("Processing rpl...");

    ptr.start();

    exec();

    ptr.stop();
    setWorker(0);

    qloginfo("Stop processing rpl");
}

// ------------------------------------------------
RPLWorkerBase::RPLWorkerBase(QObject* parent) :
    WorkerBase(parent)
{
    if (objectName().isEmpty())
        setObjectName("rpl_worker");
    init();
}

void RPLWorkerBase::initDefaultSettings()
{
    qloginfo("Init default setting values");
    settings()->beginGroup("rpl_worker");
    settings()->setValue("comment", "Настройки модуля для обработки RPL.");
    settings()->setValue("timer_interval", 5);
    settings()->setValue("timer_interval/comment",
                         "Таймер срабатывания в СЕКУНДАХ");
    settings()->setValue("processing_time", 12);
    settings()->setValue("processing_time/comment",
                         "На какое количество часов вперед генерировать FPL");
    settings()->endGroup();
}

void RPLWorkerBase::loadSettings()
{
    qloginfo("Load settings : RPL");
    settings()->beginGroup("rpl_worker");
    interval = settings()->value("timer_interval").toInt();
    processingTime = settings()->value("processing_time").toInt();
    settings()->endGroup();

    if (processingTime > 24)
        processingTime = 24;
    else if (processingTime < 1)
        processingTime = 1;
}

bool RPLWorkerBase::start()
{
    if (!settings()->contains("rpl_worker/processing_time"))
        initDefaultSettings();

    loadSettings();

    QSqlQuery query(QSqlDatabase::database(connectionName()));
    query.exec("ALTER SESSION SET NLS_TERRITORY = 'UNITED KINGDOM';");

    timerId = startTimer(interval * 1000);
    return QSqlDatabase::database(connectionName()).isOpen();
}

void RPLWorkerBase::stop()
{
    killTimer(timerId);
}

void RPLWorkerBase::timerEvent(QTimerEvent * event)
{
    if (event->timerId() == timerId && isDBConnectionValid()) {
        QString
                sqlRequest=QString(
                    "INSERT INTO " TBL_FPL
                    "("
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
                    TBL_FPL_ENTRY_FIR_EET " "
                    " ) "
                    " SELECT "
                    TBL_RPL_FIR_NAME ", "
                    TBL_RPL_AIRCRAFT_ID ", "
                    TBL_RPL_FRULES ", "
                    TBL_RPL_TFLIGHT ", "
                    TBL_RPL_NUM ", "
                    TBL_RPL_TAIRCRAFT ", "
                    TBL_RPL_TURBULENCE_CATEGORY ", "
                    TBL_RPL_EQUIPMENT ", "
                    TBL_RPL_ADEP ", "
                    "TO_TIMESTAMP(TO_CHAR(SYS_EXTRACT_UTC(SYSTIMESTAMP), 'YYYY-MM-DD ') || TO_CHAR("TBL_RPL_TIME", 'HH24:MI:SS'), 'YYYY-MM-DD HH24:MI:SS'), "
                    TBL_RPL_SPEED ", "
                    TBL_RPL_FLEVEL ", "
                    TBL_RPL_ROUTE ", "
                    TBL_RPL_ADES ", "
                    TBL_RPL_EET", "
                    TBL_RPL_ALT1 ", "
                    TBL_RPL_ALT2 ", "
                    TBL_RPL_OTHER ", "
                    TBL_RPL_ENTRY_FIR_EET " "
                    " FROM " TBL_RPL
                    " WHERE "
                    // Если RPL валиден
                    " SYS_EXTRACT_UTC(SYSTIMESTAMP) BETWEEN "TBL_RPL_VALID_FROM" AND "TBL_RPL_VALID_TO
                    " AND "
                    // И еще не существует такого же FPL
                    " NOT EXISTS (SELECT * FROM fpl WHERE fpl.aircraft_id = rpl.aircraft_id AND fpl.adep = rpl.adep AND fpl.ades = rpl.ades ) "
                    " AND "
                    // Если время отправления попадает в интервал
                    "TO_TIMESTAMP(TO_CHAR(SYS_EXTRACT_UTC(SYSTIMESTAMP), 'YYYY-MM-DD ') || TO_CHAR("TBL_RPL_TIME", 'HH24:MI:SS'), 'YYYY-MM-DD HH24:MI:SS') "
                    " BETWEEN SYS_EXTRACT_UTC(SYSTIMESTAMP) AND SYS_EXTRACT_UTC(SYSTIMESTAMP) + interval '%1' hour"
                    " AND "
                    // И сегодня день отправления
                    TBL_RPL_DAYS" like ('%' || (TO_CHAR(SYSTIMESTAMP, 'D')) || '%')"

                    " UNION "

                    " SELECT "
                    TBL_RPL_FIR_NAME ", "
                    TBL_RPL_AIRCRAFT_ID ", "
                    TBL_RPL_FRULES ", "
                    TBL_RPL_TFLIGHT ", "
                    TBL_RPL_NUM ", "
                    TBL_RPL_TAIRCRAFT ", "
                    TBL_RPL_TURBULENCE_CATEGORY ", "
                    TBL_RPL_EQUIPMENT ", "
                    TBL_RPL_ADEP ", "
                    "TO_TIMESTAMP(TO_CHAR(SYS_EXTRACT_UTC(SYSTIMESTAMP) + interval '1' day, 'YYYY-MM-DD ') || TO_CHAR("TBL_RPL_TIME", 'HH24:MI:SS'), 'YYYY-MM-DD HH24:MI:SS'), "
                    TBL_RPL_SPEED ", "
                    TBL_RPL_FLEVEL ", "
                    TBL_RPL_ROUTE ", "
                    TBL_RPL_ADES ", "
                    TBL_RPL_EET ", "
                    TBL_RPL_ALT1 ", "
                    TBL_RPL_ALT2 ", "
                    TBL_RPL_OTHER ", "
                    TBL_FPL_ENTRY_FIR_EET" "
                    " FROM " TBL_RPL
                    " WHERE "
                    // Если RPL валиден
                    " SYS_EXTRACT_UTC(SYSTIMESTAMP) BETWEEN "TBL_RPL_VALID_FROM" AND "TBL_RPL_VALID_TO
                    " AND "
                    // И еще не существует такого же FPL
                    " NOT EXISTS ( SELECT * FROM fpl WHERE fpl.aircraft_id = rpl.aircraft_id AND fpl.adep = rpl.adep AND fpl.ades = rpl.ades ) "
                    " AND "
                    // Если время отправления завтра попадает в интервал
                    "TO_TIMESTAMP(TO_CHAR(SYS_EXTRACT_UTC(SYSTIMESTAMP) + interval '1' day, 'YYYY-MM-DD ') || TO_CHAR("TBL_RPL_TIME", 'HH24:MI:SS'), 'YYYY-MM-DD HH24:MI:SS') between SYS_EXTRACT_UTC(SYSTIMESTAMP) AND SYS_EXTRACT_UTC(SYSTIMESTAMP) + interval '%1' hour"
                    " AND "
                    // И сегодня день отправления
                    TBL_RPL_DAYS" like ('%' || (TO_CHAR(SYSTIMESTAMP + interval '1' day, 'D')) || '%')").arg(
                    processingTime);
        QSqlQuery query(QSqlDatabase::database(connectionName()));

        query.prepare(sqlRequest);
        if (!query.exec()) {
            qlogwarn(tr("Query execution error. Executed query: %1\nError: %2")
                     .arg(query.lastQuery(), query.lastError().text()));
        }

        setActive(true);
    }
    WorkerBase::timerEvent(event);
}
