#include "tanmysql.h"

#include "dbconnectionkeeper.h"
#include "qtautotransaction.h"
#include "logmanager.h"

#include <QtDebug>

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QDateTime>
#include <QtCore/QDate>
#include <QtCore/QTime>

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>

#include "DBOCI.h"
#include "DBMySQL.h"

const QString TANMySqlBase::TBL_MSG_FROM_TAN = "msg_from_tan";
const QString TANMySqlBase::TBL_MSG_FROM_FDP = "msg_from_fdp";

TANMySql::TANMySql(QObject* parent) :
AFTNWorkerBase(parent)
{
    if (objectName().isEmpty())
        setObjectName("TANMySql");
}

void TANMySql::run()
{
    TANMySqlBase ptr;
    if (ptr.isDisabled())
        return;

    setWorker(&ptr);

    qloginfo(tr("TAN MySql processing"));

    ptr.start();

    // main loop
    exec();

    ptr.stop();
    setWorker(0);

    qloginfo(tr("Stop TAN MySql processing"));
}


// ------------------------------------------------
TANMySqlBase::TANMySqlBase(QObject *parent)
    : WorkerBase(parent), timerId(0), interval(5), mMysqlConnKeeper(0), disable(true)
{
    if (objectName().isEmpty())
        setObjectName("TANMySqlBase");

    disable = settings()->value("tanmysql/disable", false).toBool();
    if ( disable )
    {
        qlogwarn("Disabled");
        return;
    }

    mMysqlConnKeeper = new DbConnectionKeeper(QSqlDatabase(), 30000, this);
    connect(mMysqlConnKeeper, SIGNAL(connected()), SLOT(onMysqlReconnected()));
    connect(mMysqlConnKeeper, SIGNAL(disconnected()), SLOT(onMysqlDisconnected()));

    init();
}

void TANMySqlBase::onMysqlReconnected()
{
    qloginfo(tr("Mysql database connection restored"));
}

void TANMySqlBase::onMysqlDisconnected()
{
    qlogwarn(tr("Mysql database connection lost"));
}

bool TANMySqlBase::start()
{
    if (!settings()->contains("tanmysql/timer_interval"))
        initDefaultSettings();

    loadSettings();

    if (!mMysqlConnKeeper->database().open())
        qlogwarn(mMysqlConnKeeper->database().lastError().text());
    else
        mMysqlConnKeeper->setActive(true);

    mMysqlConnKeeper->start();

    // Interval seconds timer.
    timerId = startTimer(interval * 1000);
    if(!timerId)
    {
        qloginfo(tr("TANMySqlBase couldn't create timer"));
        return false;
    }

    setServerState(true);

    return connectionKeeper()->database().isOpen() && mMysqlConnKeeper->database().isOpen();
}


void TANMySqlBase::stop()
{
    setServerState(false);

    mMysqlConnKeeper->stop();
    killTimer(timerId);

    if (mMysqlConnKeeper->database().isOpen())
    {
        QString connname = mMysqlConnKeeper->database().connectionName();
        mMysqlConnKeeper->setDatabase(QSqlDatabase());
        QSqlDatabase::removeDatabase(connname);
    }
}


void TANMySqlBase::loadSettings()
{
    qloginfo("Load settings : MySQL");
    settings()->beginGroup("tanmysql");
    interval = settings()->value("timer_interval").toInt();

    settings()->beginGroup("mysql");
    QSqlDatabase mysqldb = QSqlDatabase::addDatabase("QMYSQL", "tan_mysql_connection");
    mysqldb.setHostName(settings()->value("host").toString());
    mysqldb.setPort(settings()->value("port").toInt());
    mysqldb.setUserName(settings()->value("user").toString());
    mysqldb.setPassword(settings()->value("password").toString());
    mysqldb.setDatabaseName(settings()->value("dbname").toString());
    settings()->endGroup();

    mMysqlConnKeeper->setDatabase(mysqldb);

    settings()->endGroup();
}


void TANMySqlBase::initDefaultSettings()
{
    qloginfo("Init default settings");
    settings()->beginGroup("tanmysql");
    settings()->setValue("comment",
        "Настройка параметров mysql ТАН.");
    settings()->setValue("timer_interval", 30);
    settings()->setValue("timer_interval/comment", "Таймер проверки таблиц в секундах.");

    settings()->setValue("disable", false);

    settings()->beginGroup("mysql");
    settings()->setValue("host", "127.0.0.1");
    settings()->setValue("port", 3306);
    settings()->setValue("user", "simulator");
    settings()->setValue("password", "password");
    settings()->setValue("dbname", "simulator");
    settings()->endGroup();

    settings()->endGroup();
}


void TANMySqlBase::timerEvent(QTimerEvent *event)
{
    qlogdebug(QString("TANMySqlBase::timerEvent %1 %2 %3").arg(event->timerId() == timerId ? "true":"false").arg(isDBConnectionValid() ? "true":"false").arg(mMysqlConnKeeper->isActive() ? "true":"false"));
    if(event->timerId() == timerId && isDBConnectionValid() && mMysqlConnKeeper->isActive())
    {
        setActive(true);
        readTAN();
        setActive(true);
        qlogdebug("TANMySqlBase readTAN");
        readFDP();
        setActive(true);
        qlogdebug("TANMySqlBase readFDP");
        writeFDP();
        setActive(true);
        qlogdebug("TANMySqlBase writeFDP");
        writeTAN();
        setActive(true);
        qlogdebug("TANMySqlBase writeTAN, last");
    }
    WorkerBase::timerEvent(event);
}

void TANMySqlBase::readTAN()
{
    db::CDBMYSQLQuery mysqlquery(mMysqlConnKeeper->database());
    FPLfromTAN::prepareSelectAll(mysqlquery, TBL_MSG_FROM_TAN, " ORDER BY "  TBL_FPL_TRTIME " LIMIT 100");
    if(!mysqlquery.exec())
    {
        qlogwarn(tr("Query execution error. Executed query: %1\nError: %2")
                 .arg(mysqlquery.lastQuery(), mysqlquery.lastError().text()));
    }
    else
    {
        while(mysqlquery.next())
        {
            FPLfromTAN Fpl(TBL_MSG_FROM_TAN);
            Fpl.updateInfo(mysqlquery);
            mListFromTAN.append(Fpl);
            setActive(true);
        }
    }
}

void TANMySqlBase::readFDP()
{
    {
        QtAutoTransaction trFDP(connectionKeeper()->database());
        db::CDBOCIQuery sqlquery(connectionKeeper()->database());
        sqlquery.Prepare("DELETE FROM " + FPLfromFDP::TBL_FPL_CHANGES + " WHERE " + FPLfromFDP::TBL_FPL_CHANGES_OPER_TIME + " <= systimestamp - interval '2' day");
        if(!sqlquery.exec())
        {
            qlogwarn(tr("Query execution error. Executed query: %1\nError: %2")
                     .arg(sqlquery.lastQuery(), sqlquery.lastError().text()));
        }
        else
        {
            trFDP.commit();
        }
        setActive(true);
        FPLfromFDP::prepareSelectAll(sqlquery, FPLfromFDP::TBL_FPL_CHANGES, " ORDER BY " + FPLfromFDP::TBL_FPL_CHANGES_OPER_TIME);
        if(!sqlquery.exec())
        {
            qlogwarn(tr("Query execution error. Executed query: %1\nError: %2")
                     .arg(sqlquery.lastQuery(), sqlquery.lastError().text()));
        }
        else
        {
            while(sqlquery.next())
            {
                FPLfromFDP Fpl(FPLfromFDP::TBL_FPL_CHANGES);
                Fpl.updateInfo(sqlquery);
                mListFromFDP.append(Fpl);
                setActive(true);
            }
        }
    }
    {
        QtAutoTransaction trFDP(connectionKeeper()->database());
        db::CDBOCIQuery sqlquery(connectionKeeper()->database());
        sqlquery.Prepare("DELETE FROM " + FPLfromFDP::TBL_MSG_FROM_FDP + " WHERE " + FPLfromFDP::TBL_FPL_CHANGES_OPER_TIME + " <= systimestamp - interval '2' day");
        if(!sqlquery.exec())
        {
            qlogwarn(tr("Query execution error. Executed query: %1\nError: %2")
                     .arg(sqlquery.lastQuery(), sqlquery.lastError().text()));
        }
        else
        {
            trFDP.commit();
        }
        setActive(true);
        FPLfromFDP::prepareSelectAll(sqlquery, FPLfromFDP::TBL_MSG_FROM_FDP, " ORDER BY " + FPLfromFDP::TBL_FPL_CHANGES_OPER_TIME);
        if(!sqlquery.exec())
        {
            qlogwarn(tr("Query execution error. Executed query: %1\nError: %2")
                     .arg(sqlquery.lastQuery(), sqlquery.lastError().text()));
        }
        else
        {
            while(sqlquery.next())
            {
                FPLfromFDP Fpl(FPLfromFDP::TBL_MSG_FROM_FDP);
                Fpl.updateInfo(sqlquery);
                mListFromFDP.append(Fpl);
                setActive(true);
            }
        }
    }
}

void TANMySqlBase::writeTAN()
{
    qlogdebug(tr("TANMySqlBase writeTAN %1 records").arg(mListFromFDP.size()));
    TFDPList::iterator itList = mListFromFDP.begin();
    while(itList != mListFromFDP.end())
    {
        QtAutoTransaction trTAN(mMysqlConnKeeper->database());
        db::CDBMYSQLQuery mysqlquery(mMysqlConnKeeper->database());
        FPLfromFDP FplTAN = itList->clone(TBL_MSG_FROM_FDP);
        FplTAN.setId(itList->id());
//        FplTAN.prepareDelete(mysqlquery);
//        if(!mysqlquery.exec())
//        {
//            qlogwarn(mysqlquery.lastError().text());
//        }
//        else
        {
            FplTAN.prepareInsert(mysqlquery);
            if(!mysqlquery.exec())
            {
                qlogwarn(tr("Query execution error. Executed query: %1\nError: %2")
                         .arg(mysqlquery.lastQuery(), mysqlquery.lastError().text()));
            }
            else
            {
                qlogdebug(QString("MySQL insert %1 %2").arg(FplTAN.aircarftId()).arg(FplTAN.messageText()));
                trTAN.commit();
                deleteFromFDP(*itList);
            }
        }
        itList = mListFromFDP.erase(itList);
        setActive(true);
    }
}

void TANMySqlBase::writeFDP()
{
    TTANList::iterator itList = mListFromTAN.begin();
    while(itList != mListFromTAN.end())
    {
        qlogdebug("TANMySqlBase writeFDP 0");
        QtAutoTransaction trFDP(connectionKeeper()->database());
        FPLfromTAN Fpl = itList->clone(TBL_FPL);
        checkIDinFDP(Fpl);
        setActive(true);
        qlogdebug("TANMySqlBase writeFDP 1");
        db::CDBOCIQuery sqlquery(connectionKeeper()->database());
        if(itList->oper() == "I" || itList->oper() == "U")
        {
            if(Fpl.isNew())
              Fpl.FPL::prepareInsert(sqlquery);
            else
              Fpl.FPL::prepareUpdate(sqlquery);
        }
        else if(itList->oper() == "D")
        {
            Fpl.FPL::prepareDelete(sqlquery);
        }
        else
        {
            qlogwarn(QString("Undefined oper_type='%1' for %2").arg(itList->oper()).arg(itList->aircarftId()));
        }
        qlogdebug("TANMySqlBase writeFDP 2");
        beginQuery(connectionKeeper()->database());
        setActive(true);

        qlogdebug("TANMySqlBase writeFDP 2.0");
        // temporary write in oracle.msg_from_tan (desire Krikunov)
        db::CDBOCIQuery sqlquery2(connectionKeeper()->database());
        itList->prepareInsert(sqlquery2);
        qlogdebug("TANMySqlBase writeFDP 2.1");
        if(!sqlquery2.exec())
        {
            qlogwarn(tr("Query execution error. Executed query: %1\nError: %2")
                     .arg(sqlquery2.lastQuery(), sqlquery2.lastError().text()));
        }
        else
        {
            qlogdebug(QString("ORACLE.MSG_FROM_TAN insert %1").arg(itList->aircarftId()));
        }
        qlogdebug("TANMySqlBase writeFDP 2.2");
        setActive(true);
        qlogdebug("TANMySqlBase writeFDP 3");
        //////////////////////////////////////////////////////////

        // test, desire Krikunov, don't write in table FPL
//        if(!sqlquery.exec())
//        {
//            qlogwarn(tr("Query execution error. Executed query: %1\nError: %2")
//                     .arg(sqlquery.lastQuery(), sqlquery.lastError().text()));
//        }
//        else
        {
//            if(itList->oper() == "I")
//                qlogdebug(QString("ORACLE insert %1").arg(Fpl.aircarftId()));
//            else if(itList->oper() == "U")
//                qlogdebug(QString("ORACLE update %1").arg(Fpl.aircarftId()));
//            else if(itList->oper() == "D")
//                qlogdebug(QString("ORACLE delete %1").arg(Fpl.aircarftId()));

            endQuery(connectionKeeper()->database());
            trFDP.commit();
            setActive(true);
            qlogdebug("TANMySqlBase writeFDP 4");
            QtAutoTransaction trTAN(mMysqlConnKeeper->database());
            db::CDBMYSQLQuery mysqlquery(mMysqlConnKeeper->database());
            itList->prepareDelete(mysqlquery);
            qlogdebug("TANMySqlBase writeFDP 5");
            if(!mysqlquery.exec())
            {
                qlogwarn(tr("Query execution error. Executed query: %1\nError: %2")
                         .arg(mysqlquery.lastQuery(), mysqlquery.lastError().text()));
            }
            else
            {
                trTAN.commit();
            }
        }
        itList = mListFromTAN.erase(itList);
        setActive(true);
    }
}

void TANMySqlBase::checkIDinFDP(FPL& Fpl)
{
    db::CDBOCIQuery sqlquery(connectionKeeper()->database());
    FPL::prepareSelectAll(sqlquery, TBL_FPL, QString(" WHERE "  TBL_FPL_AIRCRAFT_ID " = '%1' AND " TBL_FPL_ADEP " = '%2' AND " TBL_FPL_ADES " = '%3'")
                          .arg(Fpl.aircarftId()).arg(Fpl.departureAerodrome()).arg(Fpl.destinationAerodrome()));
    if (!sqlquery.exec())
    {
        qlogwarn(tr("Query execution error. Executed query: %1\nError: %2")
                 .arg(sqlquery.lastQuery(), sqlquery.lastError().text()));
    }
    else
    {
        while(sqlquery.next())
        {
            const QSqlRecord& record = sqlquery.record();
            QDateTime dtDepartureTime = record.value(TBL_FPL_TIME).toDateTime();
            if(dtDepartureTime.toString("hh:mm") == Fpl.departureTime().toString("hh:mm"))
            {
                //if(Fpl.id() != -1)
                //{
                    if(dtDepartureTime.toString("dd.MM") == Fpl.departureTime().toString("dd.MM"))
                    {
                        Fpl.setId(record.value(TBL_FPL_ID).toInt());
                    }
                //}
                //else
                //{
                //    Fpl.setId(record.value(TBL_FPL_ID).toInt());
                //}
            }
        }
    }
}

void TANMySqlBase::deleteFromFDP(FPL& Fpl)
{
    //delete from TBL_FPL_CHANGES
//    QtAutoTransaction trFDP(connectionKeeper()->database());
//    db::CDBOCIQuery sqlquery(connectionKeeper()->database());
//    sqlquery.Prepare("DELETE FROM " + FPLTAN::TBL_FPL_CHANGES + " WHERE " TBL_FPL_ID " = :" TBL_FPL_ID
//                     " AND " + FPLTAN::TBL_FPL_CHANGES_OPER + " = :" + FPLTAN::TBL_FPL_CHANGES_OPER +
//                     " AND " + FPLTAN::TBL_FPL_CHANGES_OPER_TIME + " = :" + FPLTAN::TBL_FPL_CHANGES_OPER_TIME);
//    sqlquery.BindValue(TBL_FPL_ID, Fpl.id());
//    sqlquery.BindValue(FPLTAN::TBL_FPL_CHANGES_OPER, Fpl.oper());
//    sqlquery.BindValue(FPLTAN::TBL_FPL_CHANGES_OPER_TIME, Fpl.operTime());
//    if(!sqlquery.exec())
//    {
//        qlogwarn(sqlquery.lastError().text());
//    }
//    else
//    {
//        trFDP.commit();
//    }
    QtAutoTransaction trFDP(connectionKeeper()->database());
    db::CDBOCIQuery sqlquery(connectionKeeper()->database());
//    FPL FplFDP = Fpl.clone(FPLfromFDP::TBL_FPL_CHANGES);
//    FplFDP.setId(Fpl.id());
//    FplFDP.prepareDelete(sqlquery);
    Fpl.prepareDelete(sqlquery);
    if(!sqlquery.exec())
    {
        qlogwarn(tr("Query execution error. Executed query: %1\nError: %2")
                 .arg(sqlquery.lastQuery(), sqlquery.lastError().text()));
    }
    else
    {
        trFDP.commit();
    }
}

void TANMySqlBase::setServerState(bool bMain)
{
    return;
    qloginfo(tr("TANMySqlBase::setServerState %1 %2").arg(bMain ? 1 : 0).arg(isDBConnectionValid() ? "true" : "false"));

    if(isDBConnectionValid())
    {
        QtAutoTransaction trFDP(connectionKeeper()->database());
        db::CDBOCIQuery sqlquery(connectionKeeper()->database());
        sqlquery.prepare("UPDATE fpl_cfg SET pps = :1");
        sqlquery.bindValue(":1", bMain ? 1 : 0);
        if(!sqlquery.exec())
        {
            qlogwarn(tr("Query execution error. Executed query: %1\nError: %2")
                     .arg(sqlquery.lastQuery(), sqlquery.lastError().text()));
        }
        else
        {
            trFDP.commit();
        }
    }
}

bool TANMySqlBase::beginQuery(const QSqlDatabase &base)
{
    QSqlQuery beginQuery(base);
    beginQuery.prepare("BEGIN DBMS_SESSION.SET_CONTEXT ( 'CLIENTCONTEXT', 'FDP_TAN_FLAG', 777 ); END;");
    if (!beginQuery.exec())
    {
        qlogwarn(tr("Query execution error. Executed query: %1\nError: %2")
                 .arg(beginQuery.lastQuery(), beginQuery.lastError().text()));
        return false;
    }
    return true;
}

bool TANMySqlBase::endQuery(const QSqlDatabase &base)
{
    QSqlQuery endQuery(base);
    endQuery.prepare("BEGIN DBMS_SESSION.SET_CONTEXT ( 'CLIENTCONTEXT', 'FDP_TAN_FLAG', 0 ); END;");
    if (!endQuery.exec())
    {
        qlogwarn(tr("Query execution error. Executed query: %1\nError: %2")
                 .arg(endQuery.lastQuery(), endQuery.lastError().text()));
        return false;
    }
    return true;
}
