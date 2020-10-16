/*
 * MysqlConverter.cpp
 *
 *  Created on: Oct 27, 2011
 *      Author: filonenko
 */

#include "mysqlconverter.h"

#include "dbconnectionkeeper.h"

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


MysqlConverter::MysqlConverter(QObject* parent) :
AFTNWorkerBase(parent)
{
    if (objectName().isEmpty())
        setObjectName("mysql_converter");
}

void MysqlConverter::run()
{
    MysqlConverterBase ptr;
    if (ptr.isDisabled())
        return;

    setWorker(&ptr);

    qloginfo(tr("Mysql converter processing"));

    ptr.start();

    // main loop
    exec();

    ptr.stop();
    setWorker(0);

    qloginfo(tr("Stop Mysql converter processing"));
}


// ------------------------------------------------
MysqlConverterBase::MysqlConverterBase(QObject *parent)
    :WorkerBase(parent), interval(5), disable(true)
{
    if (objectName().isEmpty())
        setObjectName("MysqlConverterBase");

    disable = settings()->value("mysqlconverter/disable", false).toBool();
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

void MysqlConverterBase::onMysqlReconnected()
{
    qloginfo(tr("Mysql database connection restored"));
}

void MysqlConverterBase::onMysqlDisconnected()
{
    qlogwarn(tr("Mysql database connection lost"));
}

bool MysqlConverterBase::start()
{
    if (!settings()->contains("mysqlconverter/timer_interval"))
        initDefaultSettings();

    loadSettings();

    oradb = QSqlDatabase::database(connectionName());

    if (!mysqldb.open())
        qlogwarn(mysqldb.lastError().text());
    else
        mMysqlConnKeeper->setActive(true);

    mMysqlConnKeeper->start();

    // Interval seconds timer.
    timerId = startTimer(interval * 1000);

    return oradb.isOpen() && mysqldb.isOpen();
}


void MysqlConverterBase::stop()
{
    mMysqlConnKeeper->stop();
    killTimer(timerId);

    if (mysqldb.isOpen())
    {
        QString connname = mysqldb.connectionName();
        mysqldb = QSqlDatabase();
        QSqlDatabase::removeDatabase(connname);
    }
}


void MysqlConverterBase::loadSettings()
{
    qloginfo("Load settings : MySQL");
    settings()->beginGroup("mysqlconverter");
    interval = settings()->value("timer_interval").toInt();

    settings()->beginGroup("mysql");
    mysqldb = QSqlDatabase::addDatabase("QMYSQL", "mysql_conenction");
    mysqldb.setHostName(settings()->value("host").toString());
    mysqldb.setPort(settings()->value("port").toInt());
    mysqldb.setUserName(settings()->value("user").toString());
    mysqldb.setPassword(settings()->value("password").toString());
    mysqldb.setDatabaseName(settings()->value("dbname").toString());
    settings()->endGroup();

    mMysqlConnKeeper->setDatabase(mysqldb);

    settings()->endGroup();
}


void MysqlConverterBase::initDefaultSettings()
{
    qloginfo("Init default settings");
    settings()->beginGroup("mysqlconverter");
    settings()->setValue("comment",
        "Настройки ковертации из mysql в oracle.");
    settings()->setValue("timer_interval", 30);
    settings()->setValue("timer_interval/comment", "Таймер проверки таблиц в секундах.");

    settings()->setValue("disable", false);

    settings()->beginGroup("mysql");
    settings()->setValue("host", "127.0.0.1");
    settings()->setValue("port", 3306);
    settings()->setValue("user", "root");
    settings()->setValue("password", "root");
    settings()->setValue("dbname", "flpln");
    settings()->endGroup();

    settings()->endGroup();
}

QStringList MysqlConverterBase::isValidRecord(const QSqlRecord &mysqlrec)
{
    QStringList warnList;

    QStringList nullColumns = QStringList() << "FFROM" << "FTO";
    foreach ( const QString &c, nullColumns )
    {
        if ( mysqlrec.value(c).isNull() )
        {
            warnList << tr("%1 is 'null'").arg(c);
            return warnList;
        }
    }

    if (QString(mysqlrec.value("ACOMP").toString() + mysqlrec.value("NFLIGHT").toString()).size() > 7)
    {
        warnList << tr("ACOMP = '%1', NFLIGHT = '%2'")
                    .arg(mysqlrec.value("ACOMP").toString()
                         , mysqlrec.value("NFLIGHT").toString());
        warnList << tr("AIRCRAFT_ID = '%1' is larger then 7 symbols")
                    .arg(mysqlrec.value("ACOMP").toString()
                         + mysqlrec.value("NFLIGHT").toString());
        return warnList;
    }
    else if ( mysqlrec.value("ACOMP").isNull() && mysqlrec.value("NFLIGHT").isNull() )
    {
        warnList << tr("ACOMP is 'null' and NFLIGHT is 'null'");
        return warnList;
    }
    if (mysqlrec.value("VSTYPE").toString().size() > 4)
    {
        warnList << tr("VSTYPE = '%1' is larger than 4 symbols")
                    .arg(mysqlrec.value("VSTYPE").toString());
        return warnList;
    }
    if (mysqlrec.value("FRULES").toString().size() > 1)
    {
        warnList << tr("FRULES = '%1' is larger than 1 symbol")
                    .arg(mysqlrec.value("FRULES").toString());
        return warnList;
    }
    if (mysqlrec.value("FLTYPE").toString().size() > 1)
    {
        warnList << tr("FLTYPE = '%1' is larger than 1 symbol")
                    .arg(mysqlrec.value("FLTYPE").toString());
        return warnList;
    }
    if (mysqlrec.value("WTC").toString().size() > 1)
    {
        warnList << tr("WTC = '%1' is larger than 1 symbol")
                    .arg(mysqlrec.value("WTC").toString());
        return warnList;
    }
    if (mysqlrec.value("RVSM").toString().size() > 222)
    {
        warnList << tr("RVSM = '%1' is larger than 222 symbol")
                    .arg(mysqlrec.value("RVSM").toString());
        return warnList;
    }
    if (mysqlrec.value("FFROM").toString().size() > 4)
    {
        warnList << tr("FFROM = '%1' is larger than 4 symbol")
                    .arg(mysqlrec.value("FFROM").toString());
        return warnList;
    }
    if (mysqlrec.value("SPEED").toString().size() > 5)
    {
        tr("SPEED = '%1' is larger than 5 symbol")
                .arg(mysqlrec.value("SPEED").toString());
        return warnList;
    }
    if (mysqlrec.value("FSE").toString().size() > 5)
    {
        warnList << tr("FSE = '%1' is larger than 5 symbol")
                    .arg(mysqlrec.value("FSE").toString());
        return warnList;
    }
    if (mysqlrec.value("FTO").toString().size() > 4)
    {
        warnList << tr("FTO = '%1' is larger than 4 symbol")
                    .arg(mysqlrec.value("FTO").toString());
        return warnList;
    }
    QStringList altAerodroms = mysqlrec.value("RES").toString().split(QRegExp("[\\s/]"), QString::SkipEmptyParts);
    if (!altAerodroms.empty() && altAerodroms.at(0).size() > 4)
    {
        warnList << tr("RES = '%1'").arg(mysqlrec.value("RES").toString());
        warnList << tr("ALT1 = '%1' is larger than 4 symbols").arg(altAerodroms.at(0));
        return warnList;
    }
    if (altAerodroms.size() > 1 && altAerodroms.at(1).size() > 4)
    {
        warnList << tr("RES = '%1'").arg(mysqlrec.value("RES").toString());
        warnList << tr("ALT2 = '%1' is larger than 4 symbols").arg(altAerodroms.at(1));
        return warnList;
    }
    return warnList;
}

void MysqlConverterBase::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timerId && isDBConnectionValid() && mMysqlConnKeeper->isActive())
    {
        // Copy flight plans that were not deleted or canceled: MySql -> Oracle.

        QString sqlQuery = "SELECT * FROM flights "
                "WHERE DEL != 1 and CNCL != 1 and FDATE = date(utc_timestamp()) and RVVZ >= date_add(utc_timestamp(), interval -4 HOUR) and RVVZ <= date_add(utc_timestamp(), interval 4 HOUR)";

        QSqlQuery mysqlquery(mysqldb);
        if (!mysqlquery.exec(sqlQuery))
        {
            qlogwarn(mysqlquery.lastError().text());
            setActive(true);
        }
        else
        {
            QStringList externalIdList;
            // Prepare list of MySql id within 8 hour interval request.
            while (mysqlquery.next())
            {
                QSqlRecord mysqlrec = mysqlquery.record();
                externalIdList << mysqlrec.value("id").toString();
            }

            // Remove all records from Qracle which do not have external id from the MySql query list.
            if ( !externalIdList.empty() )
            {
                QString oraSqlQuery = QString("DELETE FROM FPL "
                                              "WHERE extern_id NOT IN (%1)").arg(externalIdList.join(", "));
                QSqlQuery oraquery(oradb);
                if (!oraquery.exec(oraSqlQuery))
                    qlogwarn(oraquery.lastError().text());
            }

            setActive(true);

            // Put cursor before the first record of query.
            mysqlquery.seek(-1);

            // Process each MySql record.
            while (mysqlquery.next())
            {
                // Get their record
                QSqlRecord orarec = oradb.record("FPL");
                QSqlRecord mysqlrec = mysqlquery.record();

                // Validation of MySql record ...
                {
                    int id = mysqlrec.value("IND").toUInt();
                    QStringList warningsList = isValidRecord(mysqlrec);
                    if ( !warningsList.empty() )
                    {
                        if ( !listOfErrorId.contains(id) )
                        {
                            qlogwarn(tr("Invalid mysql record with ind = %1. Error: %2").arg(id)
                                    .arg(warningsList.join("; ")));

                            listOfErrorId.append(id);
                        }
                        continue;
                    }
                    else
                    {
                        listOfErrorId.removeAll(id);
                    }
                }

                QDateTime eet = QDateTime::fromMSecsSinceEpoch(mysqlrec.value("RVP").toDateTime().toMSecsSinceEpoch()
                    - mysqlrec.value("RVV").toDateTime().toMSecsSinceEpoch());
                orarec.remove(orarec.indexOf("ID"));

                orarec.setValue("FIR_NAME", "UTDD");
                orarec.setValue("AIRCRAFT_ID", mysqlrec.value("ACOMP").toString() + mysqlrec.value("NFLIGHT").toString());
                orarec.setValue("TAIRCRAFT", mysqlrec.value("VSTYPE").toString());
                QString frules = mysqlrec.value("FRULES").toString();
                if (frules.isEmpty())
                    orarec.setValue("FRULES", "I");
                else
                    orarec.setValue("FRULES", frules);
                orarec.setValue("TFLIGHT", mysqlrec.value("FLTYPE"));
                orarec.setValue("NUM", 1);
                QString wtc = mysqlrec.value("WTC").toString();
                if (wtc.isEmpty())
                    orarec.setValue("WTC", "H");
                else
                    orarec.setValue("WTC", wtc);
                orarec.setValue("ENTRY_FIR_EET", mysqlrec.value("RVVZ").toDateTime().toString("hh:mm"));
                orarec.setValue("EQUIPMENT", mysqlrec.value("RVSM"));
                orarec.setValue("ADEP", mysqlrec.value("FFROM"));
                orarec.setValue("TIME", QDateTime(QDateTime::currentDateTimeUtc().date(), mysqlrec.value("RVV").toTime(), Qt::UTC));
                QString speed = mysqlrec.value("SPEED").toString();
                if (speed.isEmpty())
                    orarec.setValue("SPEED", "K0800");
                else
                    orarec.setValue("SPEED", speed);
                orarec.setValue("FLEVEL", mysqlrec.value("FSE"));
                if (orarec.value("FLEVEL").isNull() || orarec.value("FLEVEL").toString().isEmpty())
                    orarec.setValue("FLEVEL", "F300");
                orarec.setValue("ADES", mysqlrec.value("FTO"));
                orarec.setValue("EET", eet.toString("hh:mm"));
                QStringList altAerodroms = mysqlrec.value("RES").toString().split(QRegExp("[\\s/]"), QString::SkipEmptyParts);
                if (altAerodroms.size() > 1)
                {
                    orarec.setValue("ALT1", altAerodroms[0]);
                    orarec.setValue("ALT2", altAerodroms[1]);
                } else if (altAerodroms.size() == 1)
                {
                    orarec.setValue("ALT1", altAerodroms[0]);
                    orarec.setValue("ALT2", QVariant(QVariant::String));
                } else
                {
                    orarec.setValue("ALT1", QVariant(QVariant::String));
                    orarec.setValue("ALT2", QVariant(QVariant::String));
                }
                orarec.setValue("OTHER", QVariant(QVariant::String));
                orarec.remove(orarec.indexOf("TRTIME"));
                //orarec.setValue("TRTIME", "");
                orarec.setValue("EXFPL", QVariant(QVariant::String));
                if (mysqlrec.value("cncl").toInt() == 1)
                    orarec.setValue("EXFPL", "c");
                orarec.setValue("VISIBILITY", 0);
                orarec.setValue("MESSAGE_ID", QVariant(QVariant::Int));
                orarec.setValue("ROUTE", mysqlrec.value("TVZRC").toString() + " " + mysqlrec.value("TEZRC").toString());

                orarec.setValue("CITY", mysqlrec.value("CITY"));
                orarec.setValue("TVZRC", mysqlrec.value("TVZRC"));
                orarec.setValue("TEZRC", mysqlrec.value("TEZRC"));
                orarec.setValue("extern_id", mysqlrec.value("id"));

//                qDebug() << mysqlrec;
//                qDebug() << orarec;

                // Remove all rows that have the same ACID ADES ADEP in Oracle with the ones that
                // came from MySql
//                {
//                    QString oraSqlQuery = "DELETE FROM FPL " +
//                            QString("WHERE AIRCRAFT_ID = '%1' and ADES = '%2' and ADEP = '%3' and extern_id != %4")
//                            .arg(orarec.value("AIRCRAFT_ID").toString())
//                            .arg(orarec.value("ADES").toString())
//                            .arg(orarec.value("ADEP").toString())
//                            .arg(mysqlrec.value("id").toInt());
//                    QSqlQuery oraquery(oradb);
//                    if (!oraquery.exec(oraSqlQuery))
//                        qlogwarn(oraquery.lastError().text());
//                }

                setActive(true);

                QString whereClause;
                QSqlDriver::StatementType statementType = QSqlDriver::InsertStatement;
                {
                    QSqlQuery oraquery(oradb);
                    oraquery.prepare("SELECT ID from FPL WHERE extern_id = ?");
                    oraquery.addBindValue( mysqlrec.value("id") );
                    if ( oraquery.exec() && oraquery.next() )
                    {
                        int id = oraquery.value(0).toInt();
                        whereClause = QString(" WHERE ID = %1").arg(id);

                        statementType = QSqlDriver::UpdateStatement;
                    }
                }

                setActive(true);

                // Insert/Update into oracle table
                QString orastring = oradb.driver()->sqlStatement(statementType, "fpl", orarec, false);
                orastring += whereClause;

//                qDebug() << orastring;

                QSqlQuery oraquery(oradb);
                if (!oraquery.exec(orastring))
                {
                    qlogwarn(oraquery.lastError().text());
                }

                setActive(true);
            }
        }

        // Remove Oracle flight plans that were deleted or canceled in the MySql 'flights' table.

        sqlQuery = QString("SELECT id FROM flights "
                           "WHERE (DEL = 1 or CNCL = 1) and RVVZ >= date_add(utc_timestamp(), interval '-4:0:%1' HOUR_SECOND) and RVVZ <= date_add(utc_timestamp(), interval 4 HOUR)")
                .arg(interval);

        if (!mysqlquery.exec(sqlQuery))
            qlogwarn(mysqlquery.lastError().text());
        else
        {
            QString idList;
            while (mysqlquery.next())
                idList += QString("%1, ").arg(mysqlquery.value(0).toInt());

            if ( !idList.isEmpty() )
            {
                QString oraSqlQuery = "DELETE FROM FPL " +
                        QString("WHERE extern_id IN (%1)").arg(idList.left(idList.length()-2));
                QSqlQuery oraquery(oradb);
                if (!oraquery.exec(oraSqlQuery))
                    qlogwarn(oraquery.lastError().text());
            }
        }

    }

    WorkerBase::timerEvent(event);
}

