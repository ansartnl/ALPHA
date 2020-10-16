#include <QtCore/QVariant>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include "fpltan.h"

const QString FPLfromTAN::TBL_FPL_CHANGES = "fpl_changes_master";
const QString FPLfromTAN::TBL_FPL_CHANGES_OPER = "oper";
const QString FPLfromTAN::TBL_FPL_CHANGES_OPER_TIME = "oper_time";
const QString FPLfromTAN::TBL_FPL_CHANGES_MESSAGE_TEXT = "message_text";

const QString FPLfromFDP::TBL_FPL_CHANGES = "fpl_changes_master";
const QString FPLfromFDP::TBL_FPL_CHANGES_OPER = "oper";
const QString FPLfromFDP::TBL_FPL_CHANGES_OPER_TIME = "oper_time";
const QString FPLfromFDP::TBL_FPL_CHANGES_MESSAGE_TEXT = "message_text";
const QString FPLfromFDP::TBL_FPL_CHANGES_STATUS = "status";
const QString FPLfromFDP::TBL_FPL_CHANGES_POD_IN = "pod_in";
const QString FPLfromFDP::TBL_FPL_CHANGES_T_IN = "t_in";
const QString FPLfromFDP::TBL_FPL_CHANGES_H_IN = "h_in";
const QString FPLfromFDP::TBL_FPL_CHANGES_POD_OUT = "pod_out";
const QString FPLfromFDP::TBL_FPL_CHANGES_T_OUT = "t_out";
const QString FPLfromFDP::TBL_FPL_CHANGES_H_OUT = "h_out";
const QString FPLfromFDP::TBL_FPL_CHANGES_FPL_ID = "fpl_id";

const QString FPLfromFDP::TBL_MSG_FROM_FDP = "msg_from_fdp";

//================================   FPLfromTAN   ===================================================

/** Constructor */
FPLfromTAN::FPLfromTAN(const QString& table, const QString& tableID) :
    FPL(table, tableID)
{
}

/** Destructor */
FPLfromTAN::~FPLfromTAN()
{
}

/** Fill query for INSERT request */
void FPLfromTAN::prepareInsert(QSqlQuery& query)
{
    query.prepare("INSERT INTO " + table + "("
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
                  TBL_FPL_ENTRY_FIR_EET ", "
                  TBL_FPL_VISIBILITY ", "
                  TBL_FPL_EXFPL ", " +
                  TBL_FPL_CHANGES_OPER + ", " +
                  TBL_FPL_CHANGES_OPER_TIME + ", " +
                  TBL_FPL_CHANGES_MESSAGE_TEXT + " "
                  ") VALUES ( "
                  ":" TBL_FPL_ID ", "
                  ":" TBL_FPL_FIR_NAME ", "
                  ":" TBL_FPL_AIRCRAFT_ID ", "
                  ":" TBL_FPL_FRULES ", "
                  ":" TBL_FPL_TFLIGHT ", "
                  ":" TBL_FPL_NUM ", "
                  ":" TBL_FPL_TAIRCRAFT ", "
                  ":" TBL_FPL_TURBULENCE_CATEGORY ", "
                  ":" TBL_FPL_EQUIPMENT ", "
                  ":" TBL_FPL_ADEP ", "
                  ":" TBL_FPL_TIME ", "
                  ":" TBL_FPL_SPEED ", "
                  ":" TBL_FPL_FLEVEL ", "
                  ":" TBL_FPL_ROUTE ", "
                  ":" TBL_FPL_ADES ", "
                  ":" TBL_FPL_EET ", "
                  ":" TBL_FPL_ALT1 ", "
                  ":" TBL_FPL_ALT2 ", "
                  ":" TBL_FPL_OTHER ", "
                  ":" TBL_FPL_ENTRY_FIR_EET ","
                  ":" TBL_FPL_VISIBILITY ","
                  ":" TBL_FPL_EXFPL ","
                  ":" + TBL_FPL_CHANGES_OPER + ","
                  ":" + TBL_FPL_CHANGES_OPER_TIME + ","
                  ":" + TBL_FPL_CHANGES_MESSAGE_TEXT + " "
                  ")");

    updateQuery(query);
}

/** Fill query for UPDATE request */
void FPLfromTAN::prepareUpdate(QSqlQuery& query)
{
    query.prepare("UPDATE " + table + " SET "
                  TBL_FPL_FIR_NAME " = :" TBL_FPL_FIR_NAME ", "
                  TBL_FPL_AIRCRAFT_ID " = :" TBL_FPL_AIRCRAFT_ID ", "
                  TBL_FPL_FRULES " = :" TBL_FPL_FRULES ", "
                  TBL_FPL_TFLIGHT " = :" TBL_FPL_TFLIGHT ", "
                  TBL_FPL_NUM " = :" TBL_FPL_NUM ", "
                  TBL_FPL_TAIRCRAFT " = :" TBL_FPL_TAIRCRAFT ", "
                  TBL_FPL_TURBULENCE_CATEGORY " = :" TBL_FPL_TURBULENCE_CATEGORY ", "
                  TBL_FPL_EQUIPMENT " = :" TBL_FPL_EQUIPMENT ", "
                  TBL_FPL_ADEP " = :" TBL_FPL_ADEP ", "
                  TBL_FPL_TIME " = :" TBL_FPL_TIME ", "
                  TBL_FPL_SPEED " = :" TBL_FPL_SPEED ", "
                  TBL_FPL_FLEVEL " = :" TBL_FPL_FLEVEL ", "
                  TBL_FPL_ROUTE " = :" TBL_FPL_ROUTE ", "
                  TBL_FPL_ADES " = :" TBL_FPL_ADES ", "
                  TBL_FPL_EET " = :" TBL_FPL_EET ", "
                  TBL_FPL_ALT1 " = :" TBL_FPL_ALT1 ", "
                  TBL_FPL_ALT2 " = :" TBL_FPL_ALT2 ", "
                  TBL_FPL_OTHER " = :" TBL_FPL_OTHER ", "
                  TBL_FPL_ENTRY_FIR_EET " = :" TBL_FPL_ENTRY_FIR_EET ","
                  TBL_FPL_VISIBILITY " = :" TBL_FPL_VISIBILITY ","
                  TBL_FPL_EXFPL " = :" TBL_FPL_EXFPL ","
                  + TBL_FPL_CHANGES_OPER + " = :" + TBL_FPL_CHANGES_OPER + ","
                  + TBL_FPL_CHANGES_OPER_TIME + " = :" + TBL_FPL_CHANGES_OPER_TIME + ","
                  + TBL_FPL_CHANGES_MESSAGE_TEXT + " = :" + TBL_FPL_CHANGES_MESSAGE_TEXT + " "
                  "WHERE " + tableId + " = :" TBL_FPL_ID);

    updateQuery(query);
}

/** Fill query for SELECT request */
void FPLfromTAN::prepareSelect(QSqlQuery& query)
{
    query.prepare("SELECT "
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
                  TBL_FPL_ENTRY_FIR_EET ", "
                  TBL_FPL_VISIBILITY ", "
                  TBL_FPL_EXFPL ", "
                  + TBL_FPL_CHANGES_OPER + ", "
                  + TBL_FPL_CHANGES_OPER_TIME + ", "
                  + TBL_FPL_CHANGES_MESSAGE_TEXT + " "
                  "FROM " + table + " "
                  "WHERE " + tableId + "= :" TBL_FPL_ID);

    updateQuery(query);
}

/** Update information after SELECT request */
void FPLfromTAN::updateInfo(QSqlQuery& query)
{
    updateInfo(query.record());
}

void FPLfromTAN::updateInfo(const QSqlRecord& record)
{
    FPL::updateInfo(record);
    mOper = record.value(TBL_FPL_CHANGES_OPER).toString();
    mOperTime = record.value(TBL_FPL_CHANGES_OPER_TIME).toDateTime();
    mMessageText = record.value(TBL_FPL_CHANGES_MESSAGE_TEXT).toString();
}

/** Set parameters to DB request */
void FPLfromTAN::updateQuery(QSqlQuery& query)
{
    FPL::updateQuery(query);
    query.bindValue(":" + TBL_FPL_CHANGES_OPER, mOper);
    query.bindValue(":" + TBL_FPL_CHANGES_OPER_TIME, mOperTime);
    query.bindValue(":" + TBL_FPL_CHANGES_MESSAGE_TEXT, mMessageText);
}

/** Fill query for SELECT ALL request */
void FPLfromTAN::prepareSelectAll(QSqlQuery& query, const QString& sTable, const QString& sWhere)
{
    query.prepare("SELECT "
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
                  TBL_FPL_ENTRY_FIR_EET ", "
                  TBL_FPL_VISIBILITY ", "
                  TBL_FPL_EXFPL ", "
                  + TBL_FPL_CHANGES_OPER + ", "
                  + TBL_FPL_CHANGES_OPER_TIME + ", "
                  + TBL_FPL_CHANGES_MESSAGE_TEXT + " "
                  "FROM " +
                  sTable +
                  sWhere);
}

/** Clone object (full copy without Id) */
FPLfromTAN FPLfromTAN::clone(const QString &sTable) const
{
    FPLfromTAN Fpl = *this;
    Fpl.setId(-1);
    if(!sTable.isEmpty())
        Fpl.table = sTable;
    return Fpl;
}

/** Type operation */
const QString& FPLfromTAN::oper() const
{
    return mOper;
}
bool FPLfromTAN::setOper(const QString& sOper)
{
    mOper = sOper;
    return true;
}

/** Time operation */
const QDateTime& FPLfromTAN::operTime() const
{
    return mOperTime;
}
bool FPLfromTAN::setOperTime(const QDateTime& dtOperTime)
{
    mOperTime = dtOperTime;
    return true;
}

/** Message text */
const QString& FPLfromTAN::messageText() const
{
    return mMessageText;
}
bool FPLfromTAN::setMessageText(const QString& sMessageText)
{
    mMessageText = sMessageText;
    return true;
}


//================================   FPLfromFDP   ===================================================

/** Constructor */
FPLfromFDP::FPLfromFDP(const QString& table, const QString& tableID) :
    FPL(table, tableID), mStatus(0)
{
}

FPLfromFDP::FPLfromFDP(const FPL &fpl) :
    FPL(fpl), mStatus(0)
{

}

/** Destructor */
FPLfromFDP::~FPLfromFDP()
{
}

/** Fill query for INSERT request */
void FPLfromFDP::prepareInsert(QSqlQuery& query)
{
    query.prepare("INSERT INTO " + table + "("
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
                  TBL_FPL_ENTRY_FIR_EET ", "
                  TBL_FPL_VISIBILITY ", "
                  TBL_FPL_EXFPL ", " +
                  TBL_FPL_CHANGES_OPER + ", " +
                  TBL_FPL_CHANGES_OPER_TIME + ", " +
                  TBL_FPL_CHANGES_MESSAGE_TEXT + ", " +
                  TBL_FPL_CHANGES_STATUS + ", " +
                  TBL_FPL_CHANGES_POD_IN + ", " +
                  TBL_FPL_CHANGES_T_IN + ", " +
                  TBL_FPL_CHANGES_H_IN + ", " +
                  TBL_FPL_CHANGES_POD_OUT + ", " +
                  TBL_FPL_CHANGES_T_OUT + ", " +
                  TBL_FPL_CHANGES_H_OUT + ", " +
                  TBL_FPL_CHANGES_FPL_ID + " "
                  ") VALUES ( "
                  ":" TBL_FPL_ID ", "
                  ":" TBL_FPL_FIR_NAME ", "
                  ":" TBL_FPL_AIRCRAFT_ID ", "
                  ":" TBL_FPL_FRULES ", "
                  ":" TBL_FPL_TFLIGHT ", "
                  ":" TBL_FPL_NUM ", "
                  ":" TBL_FPL_TAIRCRAFT ", "
                  ":" TBL_FPL_TURBULENCE_CATEGORY ", "
                  ":" TBL_FPL_EQUIPMENT ", "
                  ":" TBL_FPL_ADEP ", "
                  ":" TBL_FPL_TIME ", "
                  ":" TBL_FPL_SPEED ", "
                  ":" TBL_FPL_FLEVEL ", "
                  ":" TBL_FPL_ROUTE ", "
                  ":" TBL_FPL_ADES ", "
                  ":" TBL_FPL_EET ", "
                  ":" TBL_FPL_ALT1 ", "
                  ":" TBL_FPL_ALT2 ", "
                  ":" TBL_FPL_OTHER ", "
                  ":" TBL_FPL_ENTRY_FIR_EET ","
                  ":" TBL_FPL_VISIBILITY ","
                  ":" TBL_FPL_EXFPL ","
                  ":" + TBL_FPL_CHANGES_OPER + ","
                  ":" + TBL_FPL_CHANGES_OPER_TIME + ","
                  ":" + TBL_FPL_CHANGES_MESSAGE_TEXT + ","
                  ":" + TBL_FPL_CHANGES_STATUS + ","
                  ":" + TBL_FPL_CHANGES_POD_IN + ","
                  ":" + TBL_FPL_CHANGES_T_IN + ","
                  ":" + TBL_FPL_CHANGES_H_IN + ","
                  ":" + TBL_FPL_CHANGES_POD_OUT + ","
                  ":" + TBL_FPL_CHANGES_T_OUT + ","
                  ":" + TBL_FPL_CHANGES_H_OUT + ","
                  ":" + TBL_FPL_CHANGES_FPL_ID + " "
                  ")");

    updateQuery(query);
}

/** Fill query for UPDATE request */
void FPLfromFDP::prepareUpdate(QSqlQuery& query)
{
    query.prepare("UPDATE " + table + " SET "
                  TBL_FPL_FIR_NAME " = :" TBL_FPL_FIR_NAME ", "
                  TBL_FPL_AIRCRAFT_ID " = :" TBL_FPL_AIRCRAFT_ID ", "
                  TBL_FPL_FRULES " = :" TBL_FPL_FRULES ", "
                  TBL_FPL_TFLIGHT " = :" TBL_FPL_TFLIGHT ", "
                  TBL_FPL_NUM " = :" TBL_FPL_NUM ", "
                  TBL_FPL_TAIRCRAFT " = :" TBL_FPL_TAIRCRAFT ", "
                  TBL_FPL_TURBULENCE_CATEGORY " = :" TBL_FPL_TURBULENCE_CATEGORY ", "
                  TBL_FPL_EQUIPMENT " = :" TBL_FPL_EQUIPMENT ", "
                  TBL_FPL_ADEP " = :" TBL_FPL_ADEP ", "
                  TBL_FPL_TIME " = :" TBL_FPL_TIME ", "
                  TBL_FPL_SPEED " = :" TBL_FPL_SPEED ", "
                  TBL_FPL_FLEVEL " = :" TBL_FPL_FLEVEL ", "
                  TBL_FPL_ROUTE " = :" TBL_FPL_ROUTE ", "
                  TBL_FPL_ADES " = :" TBL_FPL_ADES ", "
                  TBL_FPL_EET " = :" TBL_FPL_EET ", "
                  TBL_FPL_ALT1 " = :" TBL_FPL_ALT1 ", "
                  TBL_FPL_ALT2 " = :" TBL_FPL_ALT2 ", "
                  TBL_FPL_OTHER " = :" TBL_FPL_OTHER ", "
                  TBL_FPL_ENTRY_FIR_EET " = :" TBL_FPL_ENTRY_FIR_EET ","
                  TBL_FPL_VISIBILITY " = :" TBL_FPL_VISIBILITY ","
                  TBL_FPL_EXFPL " = :" TBL_FPL_EXFPL ","
                  + TBL_FPL_CHANGES_OPER + " = :" + TBL_FPL_CHANGES_OPER + ","
                  + TBL_FPL_CHANGES_OPER_TIME + " = :" + TBL_FPL_CHANGES_OPER_TIME + ","
                  + TBL_FPL_CHANGES_MESSAGE_TEXT + " = :" + TBL_FPL_CHANGES_MESSAGE_TEXT + ","
                  + TBL_FPL_CHANGES_STATUS + " = :" + TBL_FPL_CHANGES_STATUS + ","
                  + TBL_FPL_CHANGES_POD_IN + " = :" + TBL_FPL_CHANGES_POD_IN + ","
                  + TBL_FPL_CHANGES_T_IN + " = :" + TBL_FPL_CHANGES_T_IN + ","
                  + TBL_FPL_CHANGES_H_IN + " = :" + TBL_FPL_CHANGES_H_IN + ","
                  + TBL_FPL_CHANGES_POD_OUT + " = :" + TBL_FPL_CHANGES_POD_OUT + ","
                  + TBL_FPL_CHANGES_T_OUT + " = :" + TBL_FPL_CHANGES_T_OUT + ","
                  + TBL_FPL_CHANGES_H_OUT + " = :" + TBL_FPL_CHANGES_H_OUT + ","
                  + TBL_FPL_CHANGES_FPL_ID + " = :" + TBL_FPL_CHANGES_FPL_ID + " "
                  "WHERE " + tableId + " = :" TBL_FPL_ID);

    updateQuery(query);
}

/** Fill query for SELECT request */
void FPLfromFDP::prepareSelect(QSqlQuery& query)
{
    query.prepare("SELECT "
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
                  TBL_FPL_ENTRY_FIR_EET ", "
                  TBL_FPL_VISIBILITY ", "
                  TBL_FPL_EXFPL ", "
                  + TBL_FPL_CHANGES_OPER + ", "
                  + TBL_FPL_CHANGES_OPER_TIME + ", "
                  + TBL_FPL_CHANGES_MESSAGE_TEXT + ", "
                  + TBL_FPL_CHANGES_STATUS + ", "
                  + TBL_FPL_CHANGES_POD_IN + ", "
                  + TBL_FPL_CHANGES_T_IN + ", "
                  + TBL_FPL_CHANGES_H_IN + ", "
                  + TBL_FPL_CHANGES_POD_OUT + ", "
                  + TBL_FPL_CHANGES_T_OUT + ", "
                  + TBL_FPL_CHANGES_H_OUT + ", "
                  + TBL_FPL_CHANGES_FPL_ID + " "
                  "FROM " + table + " "
                  "WHERE " + tableId + "= :" TBL_FPL_ID);

    updateQuery(query);
}

/** Update information after SELECT request */
void FPLfromFDP::updateInfo(QSqlQuery& query)
{
    updateInfo(query.record());
}

void FPLfromFDP::updateInfo(const QSqlRecord& record)
{
    FPL::updateInfo(record);
    mOper = record.value(TBL_FPL_CHANGES_OPER).toString();
    mOperTime = record.value(TBL_FPL_CHANGES_OPER_TIME).toDateTime();
    mMessageText = record.value(TBL_FPL_CHANGES_MESSAGE_TEXT).toString();
    mStatus = record.value(TBL_FPL_CHANGES_STATUS).toInt();
    mPodIn = record.value(TBL_FPL_CHANGES_POD_IN).toString();
    mTin = record.value(TBL_FPL_CHANGES_T_IN).toDateTime();
    mHin = record.value(TBL_FPL_CHANGES_H_IN).toString();
    mPodOut = record.value(TBL_FPL_CHANGES_POD_OUT).toString();
    mTout = record.value(TBL_FPL_CHANGES_T_OUT).toDateTime();
    mHout = record.value(TBL_FPL_CHANGES_H_OUT).toString();
    mFPL_ID = record.value(TBL_FPL_CHANGES_FPL_ID).toString();
}

/** Set parameters to DB request */
void FPLfromFDP::updateQuery(QSqlQuery& query)
{
    FPL::updateQuery(query);
    query.bindValue(":" + TBL_FPL_CHANGES_OPER, mOper);
    query.bindValue(":" + TBL_FPL_CHANGES_OPER_TIME, mOperTime);
    query.bindValue(":" + TBL_FPL_CHANGES_MESSAGE_TEXT, mMessageText);
    query.bindValue(":" + TBL_FPL_CHANGES_STATUS, mStatus);
    query.bindValue(":" + TBL_FPL_CHANGES_POD_IN, mPodIn);
    query.bindValue(":" + TBL_FPL_CHANGES_T_IN, mTin);
    query.bindValue(":" + TBL_FPL_CHANGES_H_IN, mHin);
    query.bindValue(":" + TBL_FPL_CHANGES_POD_OUT, mPodOut);
    query.bindValue(":" + TBL_FPL_CHANGES_T_OUT, mTout);
    query.bindValue(":" + TBL_FPL_CHANGES_H_OUT, mHout);
    query.bindValue(":" + TBL_FPL_CHANGES_FPL_ID, mFPL_ID);
}

/** Fill query for SELECT ALL request */
void FPLfromFDP::prepareSelectAll(QSqlQuery& query, const QString& sTable, const QString& sWhere)
{
    query.prepare("SELECT "
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
                  TBL_FPL_ENTRY_FIR_EET ", "
                  TBL_FPL_VISIBILITY ", "
                  TBL_FPL_EXFPL ", "
                  + TBL_FPL_CHANGES_OPER + ", "
                  + TBL_FPL_CHANGES_OPER_TIME + ", "
                  + TBL_FPL_CHANGES_MESSAGE_TEXT + ", "
                  + TBL_FPL_CHANGES_STATUS + ", "
                  + TBL_FPL_CHANGES_POD_IN + ", "
                  + TBL_FPL_CHANGES_T_IN + ", "
                  + TBL_FPL_CHANGES_H_IN + ", "
                  + TBL_FPL_CHANGES_POD_OUT + ", "
                  + TBL_FPL_CHANGES_T_OUT + ", "
                  + TBL_FPL_CHANGES_H_OUT + ", "
                  + TBL_FPL_CHANGES_FPL_ID + " "
                  "FROM " +
                  sTable +
                  sWhere);
}

/** Clone object (full copy without Id) */
FPLfromFDP FPLfromFDP::clone(const QString &sTable) const
{
    FPLfromFDP Fpl = *this;
    Fpl.setId(-1);
    if(!sTable.isEmpty())
        Fpl.table = sTable;
    return Fpl;
}

/** Type operation */
const QString& FPLfromFDP::oper() const
{
    return mOper;
}
bool FPLfromFDP::setOper(const QString& sOper)
{
    mOper = sOper;
    return true;
}

/** Time operation */
const QDateTime& FPLfromFDP::operTime() const
{
    return mOperTime;
}
bool FPLfromFDP::setOperTime(const QDateTime& dtOperTime)
{
    mOperTime = dtOperTime;
    return true;
}

/** Message text */
const QString& FPLfromFDP::messageText() const
{
    return mMessageText;
}
bool FPLfromFDP::setMessageText(const QString& sMessageText)
{
    mMessageText = sMessageText;
    return true;
}

/** Entry point */
const QString& FPLfromFDP::podIn() const
{
    return mPodIn;
}
bool FPLfromFDP::setPodIn(const QString& podIn)
{
    mPodIn = podIn;
    return true;
}

const QDateTime& FPLfromFDP::tIn() const
{
    return mTin;
}
bool FPLfromFDP::setTIn(const QDateTime& dt)
{
    mTin = dt;
    return true;
}

const QString& FPLfromFDP::hIn() const
{
    return mHin;
}
bool FPLfromFDP::setHIn(const QString& hIn)
{
    mHin = hIn;
    return true;
}

/** Exit point */
const QString& FPLfromFDP::podOut() const
{
    return mPodOut;
}
bool FPLfromFDP::setPodOut(const QString& podOut)
{
    mPodOut = podOut;
    return true;
}

const QDateTime& FPLfromFDP::tOut() const
{
    return mTout;
}
bool FPLfromFDP::setTOut(const QDateTime& dt)
{
    mTout = dt;
    return true;
}

const QString& FPLfromFDP::hOut() const
{
    return mHout;
}
bool FPLfromFDP::setHOut(const QString& hOut)
{
    mHout = hOut;
    return true;
}

/** schema master ID */
const QString& FPLfromFDP::fpl_ID() const
{
    return mFPL_ID;
}
bool FPLfromFDP::setFPL_ID(const QString& fpl_ID)
{
    mFPL_ID = fpl_ID;
    return true;
}
