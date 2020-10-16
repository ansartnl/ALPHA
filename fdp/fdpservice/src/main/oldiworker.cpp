#include "oldiworker.h"
#include "qtautotransaction.h"

#include "aftnmessageprocessor.h"

#include <QtDebug>

#include <QtCore/QDateTime>
#include <QtCore/QTimer>

#include <QtSql/QSqlDriver>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>

#include "OLDIMesFPL.h"
#include "OLDIMesSTS.h"
#include "OLDIMessages.h"
#include "OLDIProc.h"

#include "Restriction.h"
#include "Dao.h"

const QString OldiWorkerBase::sSIMLName = "SIML";

OldiWorker::OldiWorker(QObject* parent) :
        AFTNWorkerBase(parent)
{
    if (objectName().isEmpty())
        setObjectName("oldi_worker");
}

void OldiWorker::run()
{
    OldiWorkerBase ptr;
    if (ptr.isDisabled())
        return;

    setWorker(&ptr);

    // connect(&ptr, SIGNAL(aftnStatusChanged(bool)), this, SIGNAL(aftnStatusChanged(bool)));
    qloginfo(tr("Oldi processing"));

    ptr.start();

    // main loop
    exec();

    ptr.stop();
    setWorker(0);

    qloginfo(tr("Stop Oldi processing"));
}


// ------------------------------------------------
OldiWorkerBase::OldiWorkerBase(QObject *parent) :
    WorkerBase(parent), pOLDIProc(0), bAppend_ENTRY_FIR_EET(true), disable(false)
{
    if (objectName().isEmpty())
        setObjectName("oldi_worker");
    disable = settings()->value("oldi/disable", false).toBool();
    if ( disable )
    {
        qlogwarn("Disabled");
        return;
    }
    init();
}

void OldiWorkerBase::defaultSettings()
{
    QString sTemp;
    settings()->beginGroup("oldi");
    settings()->setValue("comment",
                         "Настройки для обмена OLDI сообщениями.");

    sTemp = settings()->value("owner_name", "FDP").toString();
    settings()->setValue("owner_name", sTemp);
    settings()->setValue("owner_name/comment",
                         "Логическое имя сервера FDP.");

    sTemp = settings()->value("cwp_name", "CWPA,CWPB,CWPC,CWPD").toString();
    settings()->setValue("cwp_name", sTemp);
    settings()->setValue("cwp_name/comment",
                         "Логические имена рабочих станций CWP.");

    sTemp = settings()->value("lam_timeout", 5).toString();
    settings()->setValue("lam_timeout", sTemp);
    settings()->setValue("lam_timeout/comment",
                         "Таймаут сообщения LAM в СЕКУНДАХ.");

    sTemp = settings()->value("append_entry_fir_eet", "true").toString();
    settings()->setValue("append_entry_fir_eet", sTemp);
    settings()->setValue("append_entry_fir_eet/comment",
                         "Учитывать ENTRY_FIR_EET.");

    sTemp = settings()->value("disable", "false").toString();
    settings()->setValue("disable", sTemp);
    settings()->setValue("disable/comment",
                         "Отключить.");

    settings()->beginGroup("clear_data");
    settings()->setValue("comment",
                         "Настройки автоматического удаления SFPL и FPL.");

    sTemp = settings()->value("auto_clear_data", "no").toString();
    settings()->setValue("auto_clear_data", sTemp);
    settings()->setValue("auto_clear_data/comment",
                         "Запуск процесса автоматического удаления данных.");

    sTemp = settings()->value("delete_fpl_after", "48").toString();
    settings()->setValue("delete_fpl_after", sTemp);
    settings()->setValue("delete_fpl_after/comment",
                         "Удалять активный FPL через N ЧАСОВ.");

    sTemp = settings()->value("delete_sfpl_after", "6").toString();
    settings()->setValue("delete_sfpl_after", sTemp);
    settings()->setValue("delete_sfpl_after/comment",
                         "Удалять активный SFPL через N ЧАСОВ.");

    settings()->endGroup();
    settings()->endGroup();
}

bool OldiWorkerBase::start()
{
    defaultSettings();

    bAppend_ENTRY_FIR_EET = settings()->value("oldi/append_entry_fir_eet", true).toBool();

    pOLDIProc = new OLDIProc(QSqlDatabase::database(connectionName()),
                             settings()->value("oldi/owner_name", "FDP").toString(),
                             settings()->value("oldi/lam_timeout", 10).toUInt(), this);

    pOLDIProc->SetIsFDP(true);


    connect(pOLDIProc, SIGNAL(recieveMessage(QSharedPointer<OLDIMessage>)), this, SLOT(oldiMessageReceived(QSharedPointer<OLDIMessage>)));

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(checkFplToSend()));
    timer->start(5000);

    if(settings()->value("oldi/clear_data/auto_clear_data", "yes").toString() == "yes")
    {
        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(onTimerClearData()));
        timer->start(settings()->value("oldi/clear_data/delete_sfpl_after", 6).toUInt() * 60 * 60 * 60 * 1000);
        onTimerClearData();
    }

    return QSqlDatabase::database(connectionName()).isOpen();
}

void OldiWorkerBase::stop()
{
    delete pOLDIProc;
    pOLDIProc = 0;
}

void OldiWorkerBase::checkFplToSend()
{
    if ( !isDBConnectionValid() )
        return;

    QSqlDatabase db = QSqlDatabase::database(connectionName());
    { // FPL
        QtAutoTransaction transaction(db);

        QSqlQuery query(db);
        QString sQuery = QString("select fpl.*, reciever "
                                 "from fpl join oldi_fpl on fpl.aircraft_id = oldi_fpl.aircraft_id AND fpl.adep = oldi_fpl.adep AND fpl.ades = oldi_fpl.ades "
                                 "and fpl.time %1 >= SYS_EXTRACT_UTC(SYSTIMESTAMP) - interval '6' hour "
                                 "and fpl.time %2 <= SYS_EXTRACT_UTC(SYSTIMESTAMP) + interval '6' hour")
                .arg(bAppend_ENTRY_FIR_EET ? "+ to_dsinterval('0 '||nvl(fpl.entry_fir_eet,'00:00')||':00')" : "")
                .arg(bAppend_ENTRY_FIR_EET ? "+ to_dsinterval('0 '||nvl(fpl.entry_fir_eet,'00:00')||':00')" : "");
        if (query.exec(sQuery))
        {
            while (query.next())
            {
                QStringList receivers = settings()->value("oldi/cwp_name", "CWPA,CWPB,CWPC").toString().split(',');
                if (!query.record().value("reciever").isNull())
                {
                    receivers = QStringList(query.record().value("reciever").toString().split(","));
                }
                foreach(const QString& receiver, receivers)
                {
                    OLDIMesDataABI dataABI;
                    dataABI.iMesNum = pOLDIProc->GetNextMesNum();
                    dataABI.sFrom = settings()->value("oldi/owner_name", "FDP").toString();
                    dataABI.sTo = receiver;

                    dataABI.sACID = query.record().value("AIRCRAFT_ID").toString();
                    dataABI.sFRules = query.record().value("FRULES").toString();
                    dataABI.sTFlight = query.record().value("TFLIGHT").toString();
                    dataABI.iNum = query.record().value("NUM").toInt();
                    dataABI.sTPlane = query.record().value("TAIRCRAFT").toString();
                    dataABI.sTurbulence = query.record().value("WTC").toString();
                    dataABI.sEquipment = query.record().value("EQUIPMENT").toString();
                    dataABI.sADEP = query.record().value("ADEP").toString();

                    dataABI.dtADEP = query.record().value("TIME").toDateTime();

                    dataABI.sSpeed = query.record().value("SPEED").toString();
                    dataABI.sLevel = query.record().value("FLEVEL").toString();
                    dataABI.sRoute = query.record().value("ROUTE").toString();
                    dataABI.sADES = query.record().value("ADES").toString();
                    dataABI.str_dtADES = query.record().value("EET").toString();
                    dataABI.sAlt1 = query.record().value("ALT1").toString();
                    dataABI.sAlt2 = query.record().value("ALT2").toString();
                    dataABI.sOther = query.record().value("OTHER").toString();
                    dataABI.sFirName = query.record().value("FIR_NAME").toString();

                    QSharedPointer<OLDIMesFPL> fplMessage = QSharedPointer<OLDIMesFPL>(new OLDIMesFPL(dataABI));
                    connect(fplMessage.data(), SIGNAL(messageAnswer(const OLDIMessage*, QStringList))
                            , this, SLOT(onOldiMessageSended(const OLDIMessage*,QStringList)));

                    connect(fplMessage.data(), SIGNAL(messageTimeOut(const OLDIMessage*))
                            , this, SLOT(messageTimeOut(const OLDIMessage*)));
                    pOLDIProc->SendMessage(fplMessage, false);
                }
            }

            setActive(true);

            // Clear oldi_fpl table.
            QSqlQuery queryOldiClear(db);
            if ( !queryOldiClear.exec("DELETE from oldi_fpl") )
                qlogwarn(tr("Query excution error. Executed query: %1\nError: %2")
                         .arg(queryOldiClear.lastQuery(), queryOldiClear.lastError().text()));

            setActive(true);

            transaction.commit();
        } else
        {
            qDebug() << query.lastError().text();
        }
    }
    {// SFPL
        QtAutoTransaction transaction(db);

        QSqlQuery query(db);
        if (query.exec("select sfpl_master_table.*, reciever from sfpl_master_table join oldi_sfpl on sfpl_master_table.aircraft_id = oldi_sfpl.aircraft_id AND sfpl_master_table.adep = oldi_sfpl.adep AND sfpl_master_table.ades = oldi_sfpl.ades"))
        {
            while (query.next())
            {
                QStringList receivers = settings()->value("oldi/cwp_name", "CWPA,CWPB,CWPC").toString().split(',');
                if (!query.record().value("reciever").isNull())
                {
                    receivers = QStringList(query.record().value("reciever").toString().split(","));
                }
                foreach(const QString& receiver, receivers)
                {
                    OLDIMesDataABI dataABI;
                    dataABI.iMesNum = pOLDIProc->GetNextMesNum();
                    dataABI.sFrom = settings()->value("oldi/owner_name", "FDP").toString();
                    dataABI.sTo = receiver;

                    dataABI.sACID = query.record().value("AIRCRAFT_ID").toString();
                    dataABI.sFRules = query.record().value("FRULES").toString();
                    dataABI.sTFlight = query.record().value("TFLIGHT").toString();
                    dataABI.iNum = query.record().value("NUM").toInt();
                    dataABI.sTPlane = query.record().value("TAIRCRAFT").toString();
                    dataABI.sTurbulence = query.record().value("WTC").toString();
                    dataABI.sEquipment = query.record().value("EQUIPMENT").toString();
                    dataABI.sADEP = query.record().value("ADEP").toString();

                    dataABI.dtADEP = query.record().value("TIME").toDateTime();

                    dataABI.sSpeed = query.record().value("SPEED").toString();
                    dataABI.sLevel = query.record().value("FLEVEL").toString();
                    dataABI.sRoute = query.record().value("ROUTE").toString();
                    dataABI.sADES = query.record().value("ADES").toString();
                    dataABI.str_dtADES = query.record().value("EET").toString();
                    dataABI.sAlt1 = query.record().value("ALT1").toString();
                    dataABI.sAlt2 = query.record().value("ALT2").toString();
                    dataABI.sOther = query.record().value("OTHER").toString();
                    dataABI.sFirName = query.record().value("FIR_NAME").toString();


                    dataABI.sCODE = QString("%1").arg(query.record().value("code").toUInt(), 4, 8, QLatin1Char('0'));
                    dataABI.sCOPin = query.record().value("copin").toString();
                    dataABI.sCOPinLevel = query.record().value("copinfl").toString();
                    dataABI.dtETOin = query.record().value("time_entry").toTime();
                    if (!dataABI.sCOPinLevel.isEmpty())
                    {
                        dataABI.sCOPinLevel = QString("F%1").arg(dataABI.sCOPinLevel.toDouble(), 3, 'f', 0, QLatin1Char('0'));
                    }
                    //dataABI.dtETOin = QTime::fromString(query.record().value("time_entry").toString(), "hh:mm");

                    QSharedPointer<OLDIMesABI> abiMessage = QSharedPointer<OLDIMesABI>(new OLDIMesABI(dataABI));
                    connect(abiMessage.data(), SIGNAL(messageAnswer(const OLDIMessage*, QStringList))
                            , this, SLOT(onOldiMessageSended(const OLDIMessage*,QStringList)));

                    connect(abiMessage.data(), SIGNAL(messageTimeOut(const OLDIMessage*))
                            , this, SLOT(messageTimeOut(const OLDIMessage*)));
                    pOLDIProc->SendMessage(abiMessage, false);

                    OLDIMesDataINF dataINFout;
                    dataINFout.sFrom = settings()->value("oldi/owner_name", "FDP").toString();
                    dataINFout.sTo = receiver;

                    dataINFout.sACID = dataABI.sACID;
                    dataINFout.sADEP = dataABI.sADEP;
                    dataINFout.sADES = dataABI.sADES;
                    dataINFout.sCODE = dataABI.sCODE;

                    dataINFout.sSector = query.record().value("ins").toString();

                    if (!dataINFout.sSector.isEmpty())
                    {
                        dataINFout.iMesNum = pOLDIProc->GetNextMesNum();
                        dataINFout.sTypeINF = oldi::INF_ACT;

                        QSharedPointer<OLDIMesINF> infMessage = QSharedPointer<OLDIMesINF>(new OLDIMesINF(dataINFout));
                        connect(infMessage.data(), SIGNAL(messageAnswer(const OLDIMessage*, QStringList))
                                , this, SLOT(onOldiMessageSended(const OLDIMessage*,QStringList)));

                        connect(infMessage.data(), SIGNAL(messageTimeOut(const OLDIMessage*))
                                , this, SLOT(messageTimeOut(const OLDIMessage*)));
                        pOLDIProc->SendMessage(infMessage, false);
                    }

                    dataINFout.sSector = query.record().value("cs").toString();

                    if (!dataINFout.sSector.isEmpty())
                    {
                        dataINFout.iMesNum = pOLDIProc->GetNextMesNum();
                        dataINFout.sTypeINF = oldi::INF_ASSUME;

                        QSharedPointer<OLDIMesINF> infMessage = QSharedPointer<OLDIMesINF>(new OLDIMesINF(dataINFout));
                        connect(infMessage.data(), SIGNAL(messageAnswer(const OLDIMessage*, QStringList))
                                , this, SLOT(onOldiMessageSended(const OLDIMessage*,QStringList)));

                        connect(infMessage.data(), SIGNAL(messageTimeOut(const OLDIMessage*))
                                , this, SLOT(messageTimeOut(const OLDIMessage*)));
                        pOLDIProc->SendMessage(infMessage, false);
                    }
                }
            }
            setActive(true);

            // Clear oldi_sfpl table.
            QSqlQuery queryOldiClear(db);
            if ( !queryOldiClear.exec("DELETE from oldi_sfpl") )
                qlogwarn(tr("Query excution error. Executed query: %1\nError: %2")
                         .arg(queryOldiClear.lastQuery(), queryOldiClear.lastError().text()));

            setActive(true);

            transaction.commit();
        } else
        {
            qDebug() << query.lastError().text();
        }
    }
}

void OldiWorkerBase::oldiMessageReceived(QSharedPointer<OLDIMessage> message)
{
    if ( !isDBConnectionValid() )
        return;

    if (!message)
    {
        qDebug() << "oldi message is null";
        return;
    }
    QString sMessageType = message->GetMesData().sType;
    if (oldi::MES_INF == sMessageType)
    {
        OLDIMesDataINF dataINF = message.staticCast<OLDIMesINF>()->GetDataINF();

        if (oldi::INF_FPL == dataINF.sTypeINF)
        {
            message->SetExecuted();
            if(message->GetMesData().bAnswer)
            {
                OLDIMesData dataLAM = message->GetLAM(pOLDIProc->GetNextMesNum());
                QSharedPointer<OLDIMessage> pMesLAM(new OLDIMesLAM(dataLAM));
                pOLDIProc->SendMessage(pMesLAM, false);
            }

            QSqlQuery query(QSqlDatabase::database(connectionName()));
            QString sQuery = QString("select fpl.* from fpl "
                                     "where fpl.time %1 >= SYS_EXTRACT_UTC(SYSTIMESTAMP) - interval '6' hour "
                                     "and fpl.time %2 <= SYS_EXTRACT_UTC(SYSTIMESTAMP) + interval '6' hour")
                    .arg(bAppend_ENTRY_FIR_EET ? "+ to_dsinterval('0 '||nvl(fpl.entry_fir_eet,'00:00')||':00')" : "")
                    .arg(bAppend_ENTRY_FIR_EET ? "+ to_dsinterval('0 '||nvl(fpl.entry_fir_eet,'00:00')||':00')" : "");
            if (query.exec(sQuery))
            {
                while (query.next())
                {
                    QStringList receivers = QStringList(dataINF.sFrom);
                    foreach(const QString& receiver, receivers)
                    {
                        OLDIMesDataABI dataABI;
                        dataABI.iMesNum = pOLDIProc->GetNextMesNum();
                        dataABI.sFrom = settings()->value("oldi/owner_name", "FDP").toString();
                        dataABI.sTo = receiver;

                        dataABI.sACID = query.record().value("AIRCRAFT_ID").toString();
                        dataABI.sFRules = query.record().value("FRULES").toString();
                        dataABI.sTFlight = query.record().value("TFLIGHT").toString();
                        dataABI.iNum = query.record().value("NUM").toInt();
                        dataABI.sTPlane = query.record().value("TAIRCRAFT").toString();
                        dataABI.sTurbulence = query.record().value("WTC").toString();
                        dataABI.sEquipment = query.record().value("EQUIPMENT").toString();
                        dataABI.sADEP = query.record().value("ADEP").toString();

                        dataABI.dtADEP = query.record().value("TIME").toDateTime();

                        dataABI.sSpeed = query.record().value("SPEED").toString();
                        dataABI.sLevel = query.record().value("FLEVEL").toString();
                        dataABI.sRoute = query.record().value("ROUTE").toString();
                        dataABI.sADES = query.record().value("ADES").toString();
                        dataABI.str_dtADES = query.record().value("EET").toString();
                        dataABI.sAlt1 = query.record().value("ALT1").toString();
                        dataABI.sAlt2 = query.record().value("ALT2").toString();
                        dataABI.sOther = query.record().value("OTHER").toString();
                        dataABI.sFirName = query.record().value("FIR_NAME").toString();

                        QSharedPointer<OLDIMesFPL> fplMessage = QSharedPointer<OLDIMesFPL>(new OLDIMesFPL(dataABI));
                        connect(fplMessage.data(), SIGNAL(messageAnswer(const OLDIMessage*, QStringList))
                                , this, SLOT(onOldiMessageSended(const OLDIMessage*,QStringList)));

                        connect(fplMessage.data(), SIGNAL(messageTimeOut(const OLDIMessage*))
                                , this, SLOT(messageTimeOut(const OLDIMessage*)));
                        pOLDIProc->SendMessage(fplMessage, false);
                    }
                }
            } else
            {
                qDebug() << query.lastError().text();
            }

            setActive(true);

            if (query.exec("select * from sfpl_master_table"))
            {
                while (query.next())
                {
                    QStringList receivers = QStringList(dataINF.sFrom);
                    foreach(const QString& receiver, receivers)
                    {
                        OLDIMesDataABI dataABI;
                        dataABI.iMesNum = pOLDIProc->GetNextMesNum();
                        dataABI.sFrom = settings()->value("oldi/owner_name", "FDP").toString();
                        dataABI.sTo = receiver;

                        dataABI.sACID = query.record().value("AIRCRAFT_ID").toString();
                        dataABI.sFRules = query.record().value("FRULES").toString();
                        dataABI.sTFlight = query.record().value("TFLIGHT").toString();
                        dataABI.iNum = query.record().value("NUM").toInt();
                        dataABI.sTPlane = query.record().value("TAIRCRAFT").toString();
                        dataABI.sTurbulence = query.record().value("WTC").toString();
                        dataABI.sEquipment = query.record().value("EQUIPMENT").toString();
                        dataABI.sADEP = query.record().value("ADEP").toString();

                        dataABI.dtADEP = query.record().value("TIME").toDateTime();

                        dataABI.sSpeed = query.record().value("SPEED").toString();
                        dataABI.sLevel = query.record().value("FLEVEL").toString();
                        dataABI.sRoute = query.record().value("ROUTE").toString();
                        dataABI.sADES = query.record().value("ADES").toString();
                        dataABI.str_dtADES = query.record().value("EET").toString();
                        dataABI.sAlt1 = query.record().value("ALT1").toString();
                        dataABI.sAlt2 = query.record().value("ALT2").toString();
                        dataABI.sOther = query.record().value("OTHER").toString();
                        dataABI.sFirName = query.record().value("FIR_NAME").toString();


                        dataABI.sCODE = QString("%1").arg(query.record().value("code").toUInt(), 4, 8, QLatin1Char('0'));
                        dataABI.sCOPin = query.record().value("copin").toString();
                        dataABI.sCOPinLevel = query.record().value("copinfl").toString();
                        dataABI.dtETOin = query.record().value("time_entry").toTime();
                        if (!dataABI.sCOPinLevel.isEmpty())
                        {
                            dataABI.sCOPinLevel = QString("F%1").arg(dataABI.sCOPinLevel.toDouble(), 3, 'f', 0, QLatin1Char('0'));
                        }
                        //dataABI.dtETOin = QTime::fromString(query.record().value("time_entry").toString(), "hh:mm");

                        QSharedPointer<OLDIMesABI> abiMessage = QSharedPointer<OLDIMesABI>(new OLDIMesABI(dataABI));
                        connect(abiMessage.data(), SIGNAL(messageAnswer(const OLDIMessage*, QStringList))
                                , this, SLOT(onOldiMessageSended(const OLDIMessage*,QStringList)));

                        connect(abiMessage.data(), SIGNAL(messageTimeOut(const OLDIMessage*))
                                , this, SLOT(messageTimeOut(const OLDIMessage*)));
                        pOLDIProc->SendMessage(abiMessage, false);

                        OLDIMesDataINF dataINFout;
                        dataINFout.sFrom = settings()->value("oldi/owner_name", "FDP").toString();
                        dataINFout.sTo = receiver;

                        dataINFout.sACID = dataABI.sACID;
                        dataINFout.sADEP = dataABI.sADEP;
                        dataINFout.sADES = dataABI.sADES;
                        dataINFout.sCODE = dataABI.sCODE;

                        dataINFout.sSector = query.record().value("ins").toString();

                        if (!dataINFout.sSector.isEmpty())
                        {
                            dataINFout.iMesNum = pOLDIProc->GetNextMesNum();
                            dataINFout.sTypeINF = oldi::INF_ACT;

                            QSharedPointer<OLDIMesINF> infMessage = QSharedPointer<OLDIMesINF>(new OLDIMesINF(dataINFout));
                            connect(infMessage.data(), SIGNAL(messageAnswer(const OLDIMessage*, QStringList))
                                    , this, SLOT(onOldiMessageSended(const OLDIMessage*,QStringList)));

                            connect(infMessage.data(), SIGNAL(messageTimeOut(const OLDIMessage*))
                                    , this, SLOT(messageTimeOut(const OLDIMessage*)));
                            pOLDIProc->SendMessage(infMessage, false);
                        }

                        dataINFout.sSector = query.record().value("cs").toString();

                        if (!dataINFout.sSector.isEmpty())
                        {
                            dataINFout.iMesNum = pOLDIProc->GetNextMesNum();
                            dataINFout.sTypeINF = oldi::INF_ASSUME;

                            QSharedPointer<OLDIMesINF> infMessage = QSharedPointer<OLDIMesINF>(new OLDIMesINF(dataINFout));
                            connect(infMessage.data(), SIGNAL(messageAnswer(const OLDIMessage*, QStringList))
                                    , this, SLOT(onOldiMessageSended(const OLDIMessage*,QStringList)));

                            connect(infMessage.data(), SIGNAL(messageTimeOut(const OLDIMessage*))
                                    , this, SLOT(messageTimeOut(const OLDIMessage*)));
                            pOLDIProc->SendMessage(infMessage, false);
                        }
                    }
                }
            } else
            {
                qDebug() << query.lastError().text();
            }
        } else if(oldi::INF_ACT == dataINF.sTypeINF)
        {

            QSqlQuery query(QSqlDatabase::database(connectionName()));
            query.prepare("UPDATE sfpl_master_table SET ins = ? WHERE AIRCRAFT_ID = ? and ADEP = ? and ADES = ?");
            query.addBindValue(dataINF.sSector);
            query.addBindValue(dataINF.sACID);
            query.addBindValue(dataINF.sADEP);
            query.addBindValue(dataINF.sADES);
            if (!query.exec())
            {
                qDebug() << query.lastError().text();
            }

            setActive(true);

            message->SetExecuted();
            if(message->GetMesData().bAnswer)
            {
                OLDIMesData dataLAM = message->GetLAM(pOLDIProc->GetNextMesNum());
                QSharedPointer<OLDIMessage> pMesLAM(new OLDIMesLAM(dataLAM));
                pOLDIProc->SendMessage(pMesLAM, false);
            }
        } else if (oldi::INF_ASSUME == dataINF.sTypeINF)
        {
            if (dataINF.sSector.isEmpty())
            {
                QSqlQuery query(QSqlDatabase::database(connectionName()));
                query.prepare("UPDATE sfpl_master_table SET cs = ? WHERE AIRCRAFT_ID = ? and ADEP = ? and ADES = ?");
                query.addBindValue(dataINF.sSector);
                query.addBindValue(dataINF.sACID);
                query.addBindValue(dataINF.sADEP);
                query.addBindValue(dataINF.sADES);
                if (!query.exec())
                {
                    qDebug() << query.lastError().text();
                }
            } else
            {
                QSqlQuery query(QSqlDatabase::database(connectionName()));
                query.prepare("UPDATE sfpl_master_table SET cs = ? WHERE AIRCRAFT_ID = ? and ADEP = ? and ADES = ?");
                query.addBindValue(dataINF.sSector);
                query.addBindValue(dataINF.sACID);
                query.addBindValue(dataINF.sADEP);
                query.addBindValue(dataINF.sADES);
                if (!query.exec())
                {
                    qDebug() << query.lastError().text();
                }
            }

            message->SetExecuted();
            if(message->GetMesData().bAnswer)
            {
                OLDIMesData dataLAM = message->GetLAM(pOLDIProc->GetNextMesNum());
                QSharedPointer<OLDIMessage> pMesLAM(new OLDIMesLAM(dataLAM));
                pOLDIProc->SendMessage(pMesLAM, false);
            }
        } else if (oldi::INF_UNDO_SFPL == dataINF.sTypeINF)
        {
            QSqlQuery query(QSqlDatabase::database(connectionName()));

            query.prepare("UPDATE fpl_master SET visibility = 1 WHERE aircraft_id = ? and adep = ? and ades = ?");
            query.addBindValue(dataINF.sACID);
            query.addBindValue(dataINF.sADEP);
            query.addBindValue(dataINF.sADES);
            if (!query.exec())
            {
                qDebug() << query.lastError().text();
            }

            query.prepare("DELETE FROM sfpl_master_table WHERE aircraft_id = ? and adep = ? and ades = ?");
            query.addBindValue(dataINF.sACID);
            query.addBindValue(dataINF.sADEP);
            query.addBindValue(dataINF.sADES);
            if (!query.exec())
            {
                qDebug() << query.lastError().text();
            }

            message->SetExecuted();
            if(message->GetMesData().bAnswer)
            {
                OLDIMesData dataLAM = message->GetLAM(pOLDIProc->GetNextMesNum());
                QSharedPointer<OLDIMessage> pMesLAM(new OLDIMesLAM(dataLAM));
                pOLDIProc->SendMessage(pMesLAM, false);
            }
        }
    } else if (oldi::MES_ACT == sMessageType)
    {
        OLDIMesDataABI dataABI = message.staticCast<OLDIMesACT>()->GetDataACT();

        QSqlQuery query(QSqlDatabase::database(connectionName()));
        query.prepare(
                "INSERT INTO sfpl_master_table (id, fir_name, aircraft_id, taircraft, frules, tflight, num, wtc, equipment, adep, time, speed, flevel, route, ades, eet, alt1, alt2, other, trtime, "
                "code, sectors, ins, cs, ns, state, copin, time_entry, copinfl) "
                "                       (SELECT id, fir_name, aircraft_id, taircraft, frules, tflight, num, wtc, equipment, adep, time, speed, flevel, route, ades, eet, alt1, alt2, other, trtime, "
                ":1, :2, :3, :4, :5, 0, :6, to_date(:7, 'HH24:MI'), :8 "
                "FROM fpl WHERE AIRCRAFT_ID = :9 and ADEP = :10 and ADES = :11)");

        query.addBindValue(dataABI.sCODE.toUInt(0, 8));
        query.addBindValue(QVariant(""));
        query.addBindValue(QVariant(""));
        query.addBindValue(QVariant(""));
        query.addBindValue(QVariant(""));
        query.addBindValue(dataABI.sCOPin);
        query.addBindValue(dataABI.dtETOin.toString("hh:mm"));
        QVariant scopinlevel(QVariant::Double);
        if(!dataABI.sCOPinLevel.isEmpty())
        {
            if("F" == dataABI.sCOPinLevel.left(1))
                scopinlevel = dataABI.sCOPinLevel.mid(1).toDouble();
        }
        query.addBindValue(scopinlevel);
        query.addBindValue(dataABI.sACID);
        query.addBindValue(dataABI.sADEP);
        query.addBindValue(dataABI.sADES);
        if (query.exec())
        {
        } else
        {
            qDebug() << query.lastError().text();
        }
        //query.prepare("delete FROM fpl WHERE AIRCRAFT_ID = ? and ADEP = ? and ADES = ?");
        //query.prepare("update fpl set visible = 0 WHERE AIRCRAFT_ID = ? and ADEP = ? and ADES = ?");
        //query.addBindValue(dataABI.sACID);
        //query.addBindValue(dataABI.sADEP);
        //query.addBindValue(dataABI.sADES);
        //        if (query.exec())
        //        {

        //        } else
        //        {
        //            qDebug() << query.lastError().text();
        //        }

        message->SetExecuted();
        if(message->GetMesData().bAnswer)
        {
            OLDIMesData dataLAM = message->GetLAM(pOLDIProc->GetNextMesNum());
            QSharedPointer<OLDIMessage> pMesLAM(new OLDIMesLAM(dataLAM));
            pOLDIProc->SendMessage(pMesLAM, false);
        }
    } else if (oldi::MES_FPL == sMessageType)
    {
        OLDIMesDataABI dataFPL = message.staticCast<OLDIMesFPL>()->GetDataFPL();
        message->SetExecuted();
        if(message->GetMesData().bAnswer)
        {
            OLDIMesData dataLAM = message->GetLAM(pOLDIProc->GetNextMesNum());
            QSharedPointer<OLDIMessage> pMesLAM(new OLDIMesLAM(dataLAM));
            pOLDIProc->SendMessage(pMesLAM, false);
        }
        QSqlQuery query(QSqlDatabase::database(connectionName()));
        QString preparedStatement;
        {

            QSqlRecord tableRecord = QSqlDatabase::database(connectionName()).record(TBL_FPL);
            QSqlRecord pseudoPrimaryKey;
            pseudoPrimaryKey.append(tableRecord.field(TBL_FPL_AIRCRAFT_ID));
            pseudoPrimaryKey.append(tableRecord.field(TBL_FPL_ADEP));
            pseudoPrimaryKey.append(tableRecord.field(TBL_FPL_ADES));
            pseudoPrimaryKey.setValue(TBL_FPL_AIRCRAFT_ID, dataFPL.sACID);
            pseudoPrimaryKey.setValue(TBL_FPL_ADEP, dataFPL.sADEP);
            pseudoPrimaryKey.setValue(TBL_FPL_ADES, dataFPL.sADES);

            QString whereStatement = QSqlDatabase::database(connectionName()).driver()->sqlStatement(QSqlDriver::WhereStatement, TBL_FPL, pseudoPrimaryKey, false);
            query.exec(QString("select * from %1 %2").arg(TBL_FPL, whereStatement));

            // if FPL exists, update it
            // else FPL don't exists, insert it
            if (query.next())
            {
                preparedStatement = QString(TBL_FPL_UPDATE_FORMAT).arg(TBL_FPL);
                preparedStatement.append(QLatin1Char(' '));
                preparedStatement.append(whereStatement);

                ///////////////!!!!!!!!///////////////
                return; //Pasha, may be go away?
                ///////////////!!!!!!!!///////////////
            }
            else
            {
                preparedStatement = QString(TBL_FPL_INSERT_FORMAT).arg(TBL_FPL);
            }
        }

        query.prepare(preparedStatement);
        query.addBindValue(dataFPL.sFirName);
        query.addBindValue(dataFPL.sACID);
        query.addBindValue(dataFPL.sFRules);
        query.addBindValue(dataFPL.sTFlight);
        query.addBindValue(dataFPL.iNum);
        query.addBindValue(dataFPL.sTPlane);
        query.addBindValue(dataFPL.sTurbulence);
        query.addBindValue(dataFPL.sEquipment);
        query.addBindValue(dataFPL.sADEP);
        query.addBindValue(dataFPL.dtADEP);
        query.addBindValue(dataFPL.sSpeed);
        query.addBindValue(dataFPL.sLevel);
        query.addBindValue(dataFPL.sRoute);
        query.addBindValue(dataFPL.sADES);
        query.addBindValue(dataFPL.str_dtADES);
        query.addBindValue(dataFPL.sAlt1);
        query.addBindValue(dataFPL.sAlt2);
        query.addBindValue(dataFPL.sOther);
        query.addBindValue(QVariant());
        query.addBindValue(QVariant());

        if (!query.exec())
        {
            qWarning() << Q_FUNC_INFO;
            qWarning() << query.lastError().text();
        } else
        {
            // ---------------
            QSqlQuery query3(QSqlDatabase::database(connectionName()));

            if (!query3.exec("SELECT DISTINCT client from oldi_clients"))
                qDebug() << query3.lastError().text();
            else
                while (query3.next())
                {
                    QSqlQuery query2(QSqlDatabase::database(connectionName()));

                    query2.prepare("INSERT INTO oldi_fpl VALUES (?, ?, ?, ?)");
                    query2.addBindValue(dataFPL.sACID);
                    query2.addBindValue(dataFPL.sADEP);
                    query2.addBindValue(dataFPL.sADES);
                    query2.addBindValue(query3.record().value("client"));

                    if (!query2.exec())
                    {
                        qDebug() << query2.lastError().text();
                    }
                }
            // -----------------
        }
    } else if (oldi::MES_URZ == sMessageType)
    {
        OLDIMesDataURZ dataURZ = message.staticCast<OLDIMesURZ>()->GetDataURZ();
        message->SetExecuted();
        if(message->GetMesData().bAnswer && dataURZ.sFrom != sSIMLName)
        {
            OLDIMesData dataLAM = message->GetLAM(pOLDIProc->GetNextMesNum());
            QSharedPointer<OLDIMessage> pMesLAM(new OLDIMesLAM(dataLAM));
            pOLDIProc->SendMessage(pMesLAM, false);
        }

        //save restriction zone in DB
        OLDIMesURZ::Type urzDBType(OLDIMesURZ::Type(dataURZ.sDBType.data()->toLatin1()));
        if(OLDIMesURZ::enInsert == urzDBType || OLDIMesURZ::enUpdate == urzDBType || OLDIMesURZ::enDelete == urzDBType)
        {
            if(!dataURZ.sFirIdName.isEmpty())
            {
                if(dataURZ.sFrom != sSIMLName)
                {
                    QByteArray arr = QByteArray::fromHex(dataURZ.sHexData.toLatin1());
                    QDataStream strm(&arr, QIODevice::ReadOnly);
                    CRestrictionUser urz;
                    strm >> urz;
                    if(!urz.GetGUID().isNull())
                    {
                        if(OLDIMesURZ::enInsert == urzDBType || OLDIMesURZ::enUpdate == urzDBType)
                        {
                            int idFir = Dao::GetZoneIdByIdName(QSqlDatabase::database(connectionName()), dataURZ.sFirIdName);
                            if(idFir)
                            {
                                urz.SetZoneID(idFir);
                                Dao::SaveUserRestriction(QSqlDatabase::database(connectionName()), urz.GetZoneID(), urz, true);
                            }
                            else
                            {
                                qDebug() << "Can't find FIR " << dataURZ.sFirIdName;
                            }
                        }
                        if(OLDIMesURZ::enDelete == urzDBType)
                        {
                            int restId = Dao::GetUserRestrictionIDbyGUID(QSqlDatabase::database(connectionName()), urz.GetGUID());

                            if(restId)
                            {
                                urz.SetID(restId);
                                Dao::DeleteUserRestriction(QSqlDatabase::database(connectionName()), urz);
                            }
                        }
                    }
                }

                //send to others
                QString sOriginFrom = dataURZ.sFrom;
                QSqlQuery query(QSqlDatabase::database(connectionName()));
                if (!query.exec("SELECT DISTINCT client from oldi_clients"))
                    qDebug() << query.lastError().text();
                else
                {
                    dataURZ.iMesNum = pOLDIProc->GetNextMesNum();
                    dataURZ.sFrom = settings()->value("oldi/owner_name", "FDP").toString();
                    while (query.next())
                    {
                        if(sOriginFrom != query.record().value("client").toString())
                        {
                            dataURZ.sTo = query.record().value("client").toString();

                            QSharedPointer<OLDIMesURZ> urzMessage = QSharedPointer<OLDIMesURZ>(new OLDIMesURZ(dataURZ));
                            pOLDIProc->SendMessage(urzMessage, false);
                        }
                    }
                }
            }
        }
        else if(OLDIMesURZ::enRequest == urzDBType)
        {
            //request zones
            if(!dataURZ.sFirIdName.isEmpty())
            {
                int idFir = Dao::GetZoneIdByIdName(QSqlDatabase::database(connectionName()), dataURZ.sFirIdName);
                if(idFir)
                {
                    QList<CRestrictionUser> list;
                    Dao::LoadUserRestrictions(QSqlDatabase::database(connectionName()), idFir, list);

                    OLDIMesDataURZ dataToSend;
                    dataToSend.sTo = dataURZ.sFrom;
                    dataToSend.sFrom = dataURZ.sTo;
                    dataToSend.sDBType = OLDIMesURZ::enInsert;
                    dataToSend.sFirIdName = dataURZ.sFirIdName;
                    foreach(const CRestrictionUser& restr, list)
                    {
                        dataToSend.iMesNum = pOLDIProc->GetNextMesNum();
                        QByteArray arr;
                        QDataStream strm(&arr, QIODevice::WriteOnly);
                        strm << restr;
                        dataToSend.sHexData = arr.toHex();
                        QSharedPointer<OLDIMessage> pMes(new OLDIMesURZ(dataToSend));
                        pOLDIProc->SendMessage(pMes, false);
                    }
                }
                else
                {
                    qDebug() << "Can't find FIR " << dataURZ.sFirIdName;
                }
            }
        }
        else if(OLDIMesURZ::enMeteo == urzDBType)
        {
            //send to others
            QString sOriginFrom = dataURZ.sFrom;
            QSqlQuery query(QSqlDatabase::database(connectionName()));
            if (!query.exec("SELECT DISTINCT client from oldi_clients"))
                qDebug() << query.lastError().text();
            else
            {
                dataURZ.iMesNum = pOLDIProc->GetNextMesNum();
                dataURZ.sFrom = settings()->value("oldi/owner_name", "FDP").toString();
                while (query.next())
                {
                    if(sOriginFrom != query.record().value("client").toString())
                    {
                        dataURZ.sTo = query.record().value("client").toString();

                        QSharedPointer<OLDIMesURZ> urzMessage = QSharedPointer<OLDIMesURZ>(new OLDIMesURZ(dataURZ));
                        pOLDIProc->SendMessage(urzMessage, false);
                    }
                }
            }
        }
    }

    setActive(true);
}

void OldiWorkerBase::onOldiMessageSended(const OLDIMessage *message, const QStringList &errors)
{
    if ( !isDBConnectionValid() )
        return;

    if (!message)
    {
        qDebug() << "message is null";
        return;
    }

    if (message->GetMesData().sType == oldi::MES_FPL)
    {
        const OLDIMesFPL* fplMes = dynamic_cast<const OLDIMesFPL*>(message);
        if (!fplMes)
        {
            qDebug() << "fpl message is empty";
            return;
        }
        OLDIMesDataABI dataABI = fplMes->GetDataFPL();

        //        QSqlQuery query(QSqlDatabase::database(connectionName()));
        //        query.prepare("DELETE FROM oldi_fpl WHERE AIRCRAFT_ID = ? and  ADEP = ? and ADES = ? and RECIEVER = ?");
        //        query.addBindValue(dataABI.sACID);
        //        query.addBindValue(dataABI.sADEP);
        //        query.addBindValue(dataABI.sADES);
        //        query.addBindValue(dataABI.sTo);
        //        if (!query.exec())
        //        {
        //            qDebug() << query.lastError().text();
        //        }
    }
}

void OldiWorkerBase::messageTimeOut(const OLDIMessage *message)
{
    if ( !isDBConnectionValid() )
        return;

    //    if (message->GetMesData().sType == oldi::MES_FPL)
    //    {
    //        OLDIMesDataABI dataABI = static_cast<const OLDIMesFPL*>(message)->GetDataFPL();

    //        QSqlQuery query(QSqlDatabase::database(connectionName()));
    //        query.prepare("INSERT INTO oldi_fpl VALUES (?, ?, ?, ?)");
    //        query.addBindValue(dataABI.sACID);
    //        query.addBindValue(dataABI.sADEP);
    //        query.addBindValue(dataABI.sADES);
    //        query.addBindValue(dataABI.sTo);
    //        if (!query.exec())
    //        {
    //            qDebug() << query.lastError().text();
    //        }
    //    }
}

void OldiWorkerBase::onTimerClearData()
{
    if ( !isDBConnectionValid() )
        return;

    if(pOLDIProc)
    {
        // delete SFPLs
        QSqlQuery query(QSqlDatabase::database(connectionName()));
        query.prepare(
                "SELECT c.client, f.aircraft_id, f.adep, f.ades "
                "FROM sfpl_master_table f, (SELECT DISTINCT client FROM oldi_clients) c "
                "WHERE systimestamp - f.trtime > numtodsinterval(?, 'HOUR')");
        query.addBindValue(settings()->value("oldi/clear_data/delete_sfpl_after", 6).toUInt());
        if(query.exec())
        {
            while(query.next())
            {
                OLDIMesDataINF dataINF;
                dataINF.iMesNum = pOLDIProc->GetNextMesNum();
                dataINF.sTypeINF = oldi::INF_DEL_SFPL;
                dataINF.sFrom = settings()->value("oldi/owner_name", "FDP").toString();
                dataINF.sTo = query.value(0).toString();
                dataINF.sACID = query.value(1).toString();
                dataINF.sADEP = query.value(2).toString();
                dataINF.sADES = query.value(3).toString();
                dataINF.sCODE = " ";
                dataINF.sSector = " ";

                QSharedPointer<OLDIMesINF> infMessage = QSharedPointer<OLDIMesINF>(new OLDIMesINF(dataINF));
                pOLDIProc->SendMessage(infMessage, false);
            }
        }
        else
        {
            qDebug() << query.lastError().text();
        }

        setActive(true);

        query.prepare(
                "DELETE FROM fpl "
                "WHERE id IN (SELECT id FROM sfpl_master_table WHERE systimestamp - trtime > numtodsinterval(?, 'HOUR'))");
        query.addBindValue(settings()->value("oldi/clear_data/delete_sfpl_after", 6).toUInt());
        if(!query.exec())
        {
            qDebug() << query.lastError().text();
        }

        setActive(true);

        query.prepare(
                "DELETE FROM fpl_master "
                "WHERE id IN (SELECT id FROM sfpl_master_table WHERE systimestamp - trtime > numtodsinterval(?, 'HOUR'))");
        query.addBindValue(settings()->value("oldi/clear_data/delete_sfpl_after", 6).toUInt());
        if(!query.exec())
        {
            qDebug() << query.lastError().text();
        }

        setActive(true);

        query.prepare(
                "DELETE FROM sfpl_master_table "
                "WHERE id IN (SELECT id FROM sfpl_master_table WHERE systimestamp - trtime > numtodsinterval(?, 'HOUR'))");
        query.addBindValue(settings()->value("oldi/clear_data/delete_sfpl_after", 6).toUInt());
        if(!query.exec())
        {
            qDebug() << query.lastError().text();
        }

        setActive(true);

        // delete FPLs
        query.prepare(
                "SELECT c.client, f.aircraft_id, f.adep, f.ades "
                "FROM fpl f, (SELECT DISTINCT client FROM oldi_clients) c "
                "WHERE systimestamp - f.trtime > numtodsinterval(?, 'HOUR')");
        query.addBindValue(settings()->value("oldi/clear_data/delete_fpl_after", 48).toUInt());
        if(query.exec())
        {
            while(query.next())
            {
                OLDIMesDataINF dataINF;
                dataINF.iMesNum = pOLDIProc->GetNextMesNum();
                dataINF.sTypeINF = oldi::INF_DEL_SFPL;
                dataINF.sFrom = settings()->value("oldi/owner_name", "FDP").toString();
                dataINF.sTo = query.value(0).toString();
                dataINF.sACID = query.value(1).toString();
                dataINF.sADEP = query.value(2).toString();
                dataINF.sADES = query.value(3).toString();
                dataINF.sCODE = " ";
                dataINF.sSector = " ";

                QSharedPointer<OLDIMesINF> infMessage = QSharedPointer<OLDIMesINF>(new OLDIMesINF(dataINF));
                pOLDIProc->SendMessage(infMessage, false);
            }
        }
        else
        {
            qDebug() << query.lastError().text();
        }

        setActive(true);

        query.prepare(
                "DELETE FROM sfpl_master_table "
                "WHERE id IN (SELECT id FROM fpl WHERE systimestamp - trtime > numtodsinterval(?, 'HOUR'))");
        query.addBindValue(settings()->value("oldi/clear_data/delete_fpl_after", 48).toUInt());
        if(!query.exec())
        {
            qDebug() << query.lastError().text();
        }

        setActive(true);

        query.prepare(
                "DELETE FROM fpl_master "
                "WHERE id IN (SELECT id FROM fpl WHERE systimestamp - trtime > numtodsinterval(?, 'HOUR'))");
        query.addBindValue(settings()->value("oldi/clear_data/delete_fpl_after", 48).toUInt());
        if(!query.exec())
        {
            qDebug() << query.lastError().text();
        }

        setActive(true);

        query.prepare(
                "DELETE FROM fpl "
                "WHERE id IN (SELECT id FROM fpl WHERE systimestamp - trtime > numtodsinterval(?, 'HOUR'))");
        query.addBindValue(settings()->value("oldi/clear_data/delete_fpl_after", 48).toUInt());
        if(!query.exec())
        {
            qDebug() << query.lastError().text();
        }
    }
}
