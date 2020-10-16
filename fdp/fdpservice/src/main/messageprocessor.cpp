#include "messageprocessor.h"

#include "main.h"

#include "logger.h"

#include "DBSettings.h"
#include "pulsar_fdp_settings.h"
#include "qtautotransaction.h"
#include "qtxmlsettings.h"

#include "AftnTelegram.h"
#include "TelegramParser.h"
#include "TelegramTypes.h"

#if defined(DUSHANBE_BUILD)
#include "fpl/fpltan.h"
#include "DBOCI.h"
#endif

#include <QtCore/QRegExp>

#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

#define MESSAGE_CONCAT_SYMBOL     ('&')
#define MESSAGE_CONCAT_REPLACE    (' ')

#define qlogger() Log4Qt::Logger::logger(parent()->objectName())
#define qlogdebug(x) Log4Qt::Logger::logger(parent()->objectName())->debug((x))
#define qloginfo(x) Log4Qt::Logger::logger(parent()->objectName())->info((x))
#define qlogwarn(x) Log4Qt::Logger::logger(parent()->objectName())->warn((x))
#define qlogerror(x) Log4Qt::Logger::logger(parent()->objectName())->error((x))

MessageProcessor::MessageProcessor(const QSqlDatabase &db, AFTNMessageProcessor *processor, QObject *parent) :
    QObject(parent)
  , mDb(db)
  , mSettings(new QtXmlSettings(xmlPath(), this))
  , mSkipServiceMessages(true)
  , mAnswerOnError(false)
  , mProcessor(processor)
{
}

void MessageProcessor::setSkipServiceMessages(bool skip)
{
    mSkipServiceMessages = skip;
}

void MessageProcessor::setAnswerOnError(bool answer)
{
    mAnswerOnError = answer;
}

void MessageProcessor::setAftnServerName(const QString &name)
{
    mAftnServerName = name;
}

void MessageProcessor::setFilter(const QString &filter)
{
    mFilter = filter;
}

void MessageProcessor::processNetworkMessages(const QString &nonsplittedmessage)
{

    while (messageArray.size())
    {
        QString telegram;
        int index = z.indexIn(messageArray);
        if(index != -1)
            telegram = index ? messageArray.left(index) : messageArray.left(z.matchedLength());
        else
            telegram = messageArray;
        messageArray.remove(0, telegram.size());

        TelegramParser telParse;
        Telegram *pTel = telParse.parse(telegram);
        AftnTelegram *pAftnTel = static_cast < AftnTelegram * >(pTel);
        if (needSkip(pTel))
            continue;

        if (!pAftnTel)
        {
            AftnData aftnData(AftnTelegram(), "???", wholeMessage, QString("Message size: %1").arg(telegram.size()), telegram);
            if (!writeManual(aftnData))
                break;
            continue;
        }

        foreach (const QString &address, pAftnTel->mAftn.receiverAddresses)
            addAddress(address);
        addAddress(pAftnTel->mAftn.senderAddress);

        int messageBeginIndex = telegram.indexOf(MESSAGE_BEGIN);
        int messageEndIndex = telegram.indexOf(MESSAGE_END, messageBeginIndex + 1);
        if (messageBeginIndex == -1 || messageEndIndex == -1 || messageBeginIndex > messageEndIndex)
        {
            AftnData aftnData(*pAftnTel, "???", pAftnTel->body(), pAftnTel->header(), telegram);
            if (!writeManual(aftnData))
                break;
            continue;
        }

        QString header = telegram.left(messageBeginIndex - 1);
        foreach(const QString &message, pAftnTel->mAftn.text)
        {
            if (message.indexOf(QRegExp(mFilter)) != -1)
            {
                qloginfo(tr("File filtered"));
                continue;
            }

            if (!processMessage(telegram, header, message))
                continue;
        }
    }
}

bool MessageProcessor::needSkip(Telegram *pTel)
{
    AftnTelegram *pAftnTel = static_cast < AftnTelegram * >(pTel);
    bool state = ((pTel && TelegramType::isCh(pTel->type()))
                  || (mSkipServiceMessages && pAftnTel && (pAftnTel->mAftn.senderAddress == mAftnServerName))
                  || (pTel && pTel->sourceText == "XE")
                  || (pTel && pTel->sourceText.simplified().isEmpty()));
    if (state)
        qloginfo(tr("Skip message"));
    return state;
}

bool MessageProcessor::processMessage(const QString &telegram, const QString &header, const QString &message, bool answer)
{
    qloginfo(QString("Processing %1").arg(message));

    if (!writeArchive(header, message))
        return false;

    AFTNMessageProcessor::AftnMessageErrors errorCodes;
    QVariantMap messageMap = mProcessor->processMessage(message, errorCodes);

    if (errorCodes.isEmpty()) {
#if defined(DUSHANBE_BUILD)
        messageToTAN(header, message, messageMap, false);
#else
        mProcessor->insertMessage(messageMap, errorCodes);
#endif
        if (!errorCodes.isEmpty()) {
            // TODO send to some output
        }
    }
    TelegramParser telParse;
    Telegram *pTel = telParse.parse(telegram);
    AftnTelegram *pAftnTel = static_cast < AftnTelegram * >(pTel);

    if (!errorCodes.isEmpty()) {
        qlogwarn("Error");
        QStringList errors = mProcessor->errorStrings(errorCodes);
        qlogwarn(tr("Processing AFTN telegramm failed with errors %1").arg(errors.join("\n")));

        bool bUnconcernedRoute = false;
        qloginfo("MessageProcessor::processMessage 1");
#if defined(DUSHANBE_BUILD)
        if(!messageMap.value(AFTN_MESSAGE_ROUTE).toString().isEmpty() && mProcessor->containsError(AFTNMessageProcessor::NeededRouteNotExists, errorCodes))
        {
            qloginfo("MessageProcessor::processMessage 2");
            bUnconcernedRoute = !mProcessor->isFIRconcerned(messageMap);
        }
        qloginfo("MessageProcessor::processMessage 3");
        if(!bUnconcernedRoute)
            messageToTAN(header, message, messageMap, true);
        else
            qloginfo(tr("Unconcerned, skip"));
#endif
        qloginfo("MessageProcessor::processMessage 4");
        if(!bUnconcernedRoute)
        {
            if (answer)
                answerOnError(header, message, errorCodes);

            AftnData aftnData(*pAftnTel, messageMap.contains(AFTN_MESSAGE_TYPE) ? messageMap.value(AFTN_MESSAGE_TYPE).toString() : "???",
                              message, header, telegram);
            if (!writeManual(aftnData))
                return false;
        }
    } else
    {
        AftnData aftnData(*pAftnTel, messageMap.contains(AFTN_MESSAGE_TYPE) ? messageMap.value(AFTN_MESSAGE_TYPE).toString() : "???",
                          message, header, telegram);
        if (!writeReceived(aftnData))
            return false;
    }
    return true;
}

void MessageProcessor::addAddress(const QString &address)
{
    if(!address.isEmpty())
    {
        QSqlQuery select(mDb);
        select.prepare("SELECT id FROM aftn_abonent WHERE address = :address");
        select.bindValue(":address", address);
        if (!select.exec())
        {
            qlogwarn(tr("Query execution error. Executed query: %1\nError: %2")
                     .arg(select.lastQuery(), select.lastError().text()));
            return;
        }

        if (select.next())
            return;

        QSqlQuery insert(mDb);
        insert.prepare("INSERT INTO aftn_abonent (id, name, address) VALUES (default, :name, :address)");
        insert.bindValue(":name", address);
        insert.bindValue(":address", address);
        if (!insert.exec())
        {
            qlogwarn(tr("Query execution error. Executed query: %1\nError: %2")
                     .arg(insert.lastQuery(), insert.lastError().text()));
            return;
        }
    }
}

void MessageProcessor::messageToTAN(const QString &sHeader, const QString &sMessage, const QVariantMap &map, bool bError)
{
#if defined(DUSHANBE_BUILD)
    qlogdebug(tr("try insert step 1 into %1   %2").arg(FPLfromFDP::TBL_MSG_FROM_FDP, map.value(AFTN_MESSAGE_AIRCRAFT_ID).toString()));
    QtAutoTransaction transaction(mDb);
    db::CDBOCIQuery mesAftnQuery(mDb);
    qlogdebug(tr("try insert step 2 into %1   %2").arg(FPLfromFDP::TBL_MSG_FROM_FDP, map.value(AFTN_MESSAGE_AIRCRAFT_ID).toString()));

    FPLfromFDP Fpl(FPLfromFDP::TBL_MSG_FROM_FDP);
    Fpl.setId(0);
    Fpl.setFirName(mProcessor->FIRName().split(",").at(0));
    Fpl.setAircraftId(map.value(AFTN_MESSAGE_AIRCRAFT_ID).toString());
    Fpl.setFlightRules(map.value(AFTN_MESSAGE_FLIGHT_RULES).toString());
    Fpl.setFlightType(map.value(AFTN_MESSAGE_FLIGHT_TYPE).toString());
    Fpl.setAircraftNumber(map.value(AFTN_MESSAGE_AIRCRAFT_COUNT).toInt());
    Fpl.setAircraftType(map.value(AFTN_MESSAGE_AIRCRAFT_TYPE).toString());
    Fpl.setTurbulenceCategory(map.value(AFTN_MESSAGE_TURBULENCE).toString());
    Fpl.setEquipment(map.value(AFTN_MESSAGE_EQUIPMENT).toString());
    Fpl.setDepartureAerodrome(map.value(AFTN_MESSAGE_DEPARTURE_AERODROME).toString());
    Fpl.setDepartureDateTime(map.value(AFTN_MESSAGE_DEPARTURE_TIME).toDateTime());
    Fpl.setSpeed(map.value(AFTN_MESSAGE_CRUISING_SPEED_OR_MACH_NUMBER).toString());
    Fpl.setLevel(map.value(AFTN_MESSAGE_REQUESTED_CRUISING_LEVEL).toString());
    Fpl.setRoute(map.value(AFTN_MESSAGE_ROUTE).toString());
    Fpl.setDestAerodrome(map.value(AFTN_MESSAGE_DESTINATION_AERODROME).toString());
    Fpl.setEET(map.value(AFTN_MESSAGE_ELAPSED_TIME).toString());
    Fpl.setAltDestinationAerodrome(map.value(AFTN_MESSAGE_DISPERSAL_FIELD).toString());
    Fpl.setAltDestinationAerodrome2(map.value(AFTN_MESSAGE_DISPERSAL_FIELD_2).toString());
    Fpl.setOtherInformation(map.value(AFTN_MESSAGE_OTHER_INFO).toString());
    QVariantMap firEET = map.value(AFTN_MESSAGE_OTHER_INFO_FIR_EET).toMap();
    //Fpl.setEntryFIREET(firEET.value(mProcessor->FIRName().split(",").at(0)/*, "00:00"*/).toString());
    Fpl.setEntryFIREET(mProcessor->FIRName().split(","), firEET);

    //apply CHG message
    {
        QVariantMap changedMap = map.value(AFTN_MESSAGE_CHANGED_FIELDS).toMap();
        if(changedMap.size())
        {
            if (changedMap.contains(AFTN_MESSAGE_AIRCRAFT_ID))
            {
                Fpl.setAircraftId(changedMap.value(AFTN_MESSAGE_AIRCRAFT_ID).toString());
            }

            if (changedMap.contains(AFTN_MESSAGE_FLIGHT_RULES))
            {
                Fpl.setFlightRules(changedMap.value(AFTN_MESSAGE_FLIGHT_RULES).toString());
            }

            if (changedMap.contains(AFTN_MESSAGE_FLIGHT_TYPE))
            {
                Fpl.setFlightType(changedMap.value(AFTN_MESSAGE_FLIGHT_TYPE).toString());
            }

            if (changedMap.contains(AFTN_MESSAGE_AIRCRAFT_COUNT))
            {
                Fpl.setAircraftNumber(changedMap.value(AFTN_MESSAGE_AIRCRAFT_COUNT).toInt());
                Fpl.setAircraftType(changedMap.value(AFTN_MESSAGE_AIRCRAFT_TYPE).toString());
                Fpl.setTurbulenceCategory(changedMap.value(AFTN_MESSAGE_TURBULENCE).toString());
            }

            if (changedMap.contains(AFTN_MESSAGE_EQUIPMENT))
            {
                Fpl.setEquipment(changedMap.value(AFTN_MESSAGE_EQUIPMENT).toString());
            }

            if (changedMap.contains(AFTN_MESSAGE_DEPARTURE_AERODROME))
            {
                Fpl.setDepartureAerodrome(changedMap.value(AFTN_MESSAGE_DEPARTURE_AERODROME).toString());
                Fpl.setDepartureDateTime(changedMap.value(AFTN_MESSAGE_DEPARTURE_TIME).toDateTime());
            }
            else
            {
                if (!map.contains(AFTN_MESSAGE_DEPARTURE_TIME_ORIG))
                {
                    Fpl.setDepartureDateTime(QDateTime::fromString("1900-01-01 00:00:00", "yyyy-MM-dd hh:mm:ss"));
                }
                else
                {
                    if (changedMap.contains(AFTN_MESSAGE_OTHER_INFO_DOF))
                    {
                        QDate date = changedMap.value(AFTN_MESSAGE_OTHER_INFO_DOF).toDate();
                        Fpl.setDateDOF(date);
                    }

                }
            }

            if (changedMap.contains(AFTN_MESSAGE_CRUISING_SPEED_OR_MACH_NUMBER))
            {
                Fpl.setSpeed(changedMap.value(AFTN_MESSAGE_CRUISING_SPEED_OR_MACH_NUMBER).toString());
                Fpl.setLevel(changedMap.value(AFTN_MESSAGE_REQUESTED_CRUISING_LEVEL).toString());
                Fpl.setRoute(changedMap.value(AFTN_MESSAGE_ROUTE).toString());
            }

            if (changedMap.contains(AFTN_MESSAGE_DESTINATION_AERODROME))
            {
                Fpl.setDestAerodrome(changedMap.value(AFTN_MESSAGE_DESTINATION_AERODROME).toString());
                Fpl.setEET(changedMap.value(AFTN_MESSAGE_ELAPSED_TIME).toString());

                if (changedMap.contains(AFTN_MESSAGE_DISPERSAL_FIELD))
                {
                    Fpl.setAltDestinationAerodrome(changedMap.value(AFTN_MESSAGE_DISPERSAL_FIELD).toString());
                }

                if (changedMap.contains(AFTN_MESSAGE_DISPERSAL_FIELD_2))
                {
                    Fpl.setAltDestinationAerodrome2(changedMap.value(AFTN_MESSAGE_DISPERSAL_FIELD_2).toString());
                }
            }

            if (changedMap.contains(AFTN_MESSAGE_OTHER_INFO))
            {
                Fpl.setOtherInformation(changedMap.value(AFTN_MESSAGE_OTHER_INFO).toString());
            }

            if (changedMap.contains(AFTN_MESSAGE_OTHER_INFO_FIR_EET))
            {
                QVariantMap firEET = changedMap.value(AFTN_MESSAGE_OTHER_INFO_FIR_EET).toMap();
                //Fpl.setEntryFIREET(firEET.value(mProcessor->FIRName().split(",").at(0)/*, "00:00"*/).toString());
                Fpl.setEntryFIREET(mProcessor->FIRName().split(","), firEET);
            }
        }
    }

    Fpl.setOper(bError ? "E" : "I");
    Fpl.setOperTime(QDateTime::currentDateTime()); // may be QDateTime::currentDateTimeUtc()
    Fpl.setMessageText(QString(sHeader+sMessage).simplified());

    Fpl.prepareInsert(mesAftnQuery);
    if(!mesAftnQuery.exec())
    {
        qlogwarn(tr("Query execution error. Executed query: %1\nError: %2")
                 .arg(mesAftnQuery.lastQuery(), mesAftnQuery.lastError().text()));
    }
    else
    {
        transaction.commit();
        qlogdebug(tr("Insert into %1   %2").arg(Fpl.tableName(), Fpl.aircarftId()));
    }
#else
    Q_UNUSED(sHeader)
    Q_UNUSED(sMessage)
    Q_UNUSED(map)
    Q_UNUSED(bError)
#endif
}

void MessageProcessor::answerOnError(const QString &header, const QString &message, const AFTNMessageProcessor::AftnMessageErrors &errorCodes)
{
    checkParameters();
    if (mAnswerOnError && mDb.isOpen() && !errorCodes.isEmpty())
    {
        foreach(const AFTNErrorInfo& err, errorCodes)
            if(err.first < 4)
                return;

        TelegramParser telParse;
        Telegram *pTel = telParse.parse(header + message + "NNNN");
        AftnTelegram *pAftnTel = static_cast < AftnTelegram * >(pTel);
        if(pAftnTel)
        {
            QString sSender = pAftnTel->mAftn.senderAddress;
            if(!sSender.isNull())
            {
                QtAutoTransaction transaction(mDb);
                QSqlQuery query(mDb);
                query.prepare("INSERT INTO outputtelegrams(text, addresses) VALUES(:1, :2)");
                QString sText = mProcessor->errorStrings(errorCodes).join(" ");
                sText += " ";
                sText += message;
                query.bindValue(":1", sText);
                query.bindValue(":2", sSender);
                if(!query.exec())
                {
                    qlogwarn(tr("Query execution error. Executed query: %1\nError: %2")
                             .arg(query.lastQuery(), query.lastError().text()));
                }
                else
                {
                    transaction.commit();
                }
            }
        }
    }
}

void MessageProcessor::checkParameters()
{
    if (mDb.isOpen())
    {
        DBSettings dbsettings(mDb, this);
        QVariant val = dbsettings.value(PulsarSettings::autoAnswerOnError);
        if (!val.isNull())
        {
            bool bVal = val.toInt() == 1;
            if (bVal != mAnswerOnError)
            {
                mAnswerOnError = bVal;
                settings()->setValue("tcp_ip_connector/answer_on_error", mAnswerOnError);
            }
        }
    }
}

bool MessageProcessor::writeArchive(const QString &header, const QString &message)
{
    QSqlQuery query(mDb);
    query.prepare(TBL_ARC_AFTN_INSERT);
    query.addBindValue(message);
    query.addBindValue(header);
    if (!query.exec()) {
        qlogwarn(tr("Query execution error. Executed query: %1\nError: %2")
            .arg(query.lastQuery(), query.lastError().text()));
        return false;
    }
    return true;
}

bool MessageProcessor::writeManual(const AftnData &aftnData)
{
    QSqlQuery query(mDb);
    query.prepare(TBL_MAN_AFTN_INSERT);
    query.addBindValue(aftnData.text);
    query.addBindValue(aftnData.header);
    query.addBindValue(aftnData.message);
    query.addBindValue(aftnData.type);
    query.addBindValue(aftnData.mAftn.senderAddress);
    query.addBindValue(QString("%1%2%3")
                   .arg(aftnData.mAftn.fillingDate, 2, 10, QChar('0'))
                   .arg(aftnData.mAftn.fillingHour, 2, 10, QChar('0'))
                   .arg(aftnData.mAftn.fillingMinute, 2, 10, QChar('0')));

    if (!query.exec()) {
        qlogwarn(tr("Query execution error. Executed query: %1\nError: %2")
            .arg(query.lastQuery(), query.lastError().text()));
        return false;
    }
    return true;
}

bool MessageProcessor::writeReceived(const AftnData &aftnData)
{
    QSqlQuery query(mDb);
    query.prepare(TBL_RECEIVED_AFTN_INSERT);
    query.addBindValue(aftnData.text);
    query.addBindValue(aftnData.header);
    query.addBindValue(aftnData.message);
    query.addBindValue(aftnData.type);
    query.addBindValue(aftnData.mAftn.senderAddress);
    query.addBindValue(QString("%1%2%3")
                   .arg(aftnData.mAftn.fillingDate, 2, 10, QChar('0'))
                   .arg(aftnData.mAftn.fillingHour, 2, 10, QChar('0'))
                   .arg(aftnData.mAftn.fillingMinute, 2, 10, QChar('0')));

    if (!query.exec()) {
        qlogwarn(tr("Query execution error. Executed query: %1\nError: %2")
            .arg(query.lastQuery(), query.lastError().text()));
        return false;
    }
    return true;
}
