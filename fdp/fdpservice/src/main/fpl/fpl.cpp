#include <QtCore/QVariant>
#include <QtCore/QRegExp>
#include <QStringList>
#include <QtSql/QSqlRecord>
#include "DBQuery.h"

#include "fpl.h"

#include "plvalidator.h"

/** Constructor */
FPL::FPL(const QString& table, const QString& tableID) :
    mId(-1), mNumber(1), mDepartureTime(QDateTime::currentDateTimeUtc()), mVisibility(0), table(table), tableId(tableID)
{
}

FPL::FPL(int id, const QString& table, const QString& tableID) :
    mId(id), mNumber(1), mDepartureTime(QDateTime::currentDateTimeUtc()), mVisibility(0), table(table), tableId(tableID)
{
}

FPL::FPL(const QSqlRecord& record, const QString& table, const QString& tableID) :
    mId(-1), mNumber(1), mDepartureTime(QDateTime::currentDateTimeUtc()), mVisibility(0), table(table), tableId(tableID)
{
    updateInfo(record);
}

/** Destructor */
FPL::~FPL(void)
{
}

/** Fill query for INSERT request */
void FPL::prepareInsert(db::CDBQuery& query)
{
    query.Prepare("INSERT INTO " + table + "("
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
                  TBL_FPL_EXFPL " "
                  ") VALUES ( "
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
                  ":" TBL_FPL_EXFPL " "
                  ")");

    updateQuery(query);
}

/** Update Id after INSERT request */
void FPL::updateId(db::CDBQuery& query)
{
    mId = query.GetLastInsertID().toInt();
}

/** Fill query for UPDATE request */
void FPL::prepareUpdate(db::CDBQuery& query)
{
    query.Prepare("UPDATE " + table + " SET "
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
                  TBL_FPL_EXFPL " = :"TBL_FPL_EXFPL " "
                  "WHERE " + tableId + " = :" TBL_FPL_ID);

    updateQuery(query);
}

/** Fill query for SELECT request */
void FPL::prepareSelect(db::CDBQuery& query)
{
    query.Prepare("SELECT "
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
                  TBL_FPL_EXFPL " "
                  "FROM " + table + " "
                  "WHERE " + tableId + "= :" TBL_FPL_ID);

    updateQuery(query);
}

/** Update information after SELECT request */
void FPL::updateInfo(db::CDBQuery& query)
{
    updateInfo(query.record());
}

void FPL::updateInfo(const QSqlRecord& record)
{
    mId = record.value(TBL_FPL_ID).toInt();
    mFIRName = record.value(TBL_FPL_FIR_NAME).toString();
    mAircraftId = record.value(TBL_FPL_AIRCRAFT_ID).toString();
    mFlightRules = record.value(TBL_FPL_FRULES).toString();
    mFlightType = record.value(TBL_FPL_TFLIGHT).toString();
    mNumber = record.value(TBL_FPL_NUM).toInt();
    mAircraftType = record.value(TBL_FPL_TAIRCRAFT).toString();
    mTurbulenceCategory = record.value(TBL_FPL_TURBULENCE_CATEGORY).toString();
    mEquipment = record.value(TBL_FPL_EQUIPMENT).toString();
    mDepartureAerodrome = record.value(TBL_FPL_ADEP).toString();
    mDepartureTime = record.value(TBL_FPL_TIME).toDateTime();
    mSpeed = record.value(TBL_FPL_SPEED).toString();
    mLevel = record.value(TBL_FPL_FLEVEL).toString();
    mRoute = record.value(TBL_FPL_ROUTE).toString();
    mDestinationAerodrome = record.value(TBL_FPL_ADES).toString();
    mEET = record.value(TBL_FPL_EET).toString();
    mAltAerodrome = record.value(TBL_FPL_ALT1).toString();
    mAltAerodrome2 = record.value(TBL_FPL_ALT2).toString();
    mOther = record.value(TBL_FPL_OTHER).toString();
    mEntryFIREET = record.value(TBL_FPL_ENTRY_FIR_EET).toString();
    mVisibility = record.value(TBL_FPL_VISIBILITY).toInt();
    mExFPL = record.value(TBL_FPL_EXFPL).toString();
}

/** Fill query for DELETE request */
void FPL::prepareDelete(db::CDBQuery& query)
{
    query.Prepare("DELETE FROM " + table + " "
                  "WHERE " + tableId + " = :" TBL_FPL_ID);

    updateQuery(query);
}

/** Set parameters to DB request */
void FPL::updateQuery(db::CDBQuery& query)
{
    //mDepartureTime.setDate(QDateTime::currentDateTime().toUTC().date());

    query.BindValue(":" TBL_FPL_ID, mId);
    query.BindValue(":" TBL_FPL_FIR_NAME, mFIRName);
    query.BindValue(":" TBL_FPL_AIRCRAFT_ID, mAircraftId);
    query.BindValue(":" TBL_FPL_FRULES, mFlightRules);
    query.BindValue(":" TBL_FPL_TFLIGHT, mFlightType);
    query.BindValue(":" TBL_FPL_NUM, mNumber);
    query.BindValue(":" TBL_FPL_TAIRCRAFT, mAircraftType);
    query.BindValue(":" TBL_FPL_TURBULENCE_CATEGORY, mTurbulenceCategory);
    query.BindValue(":" TBL_FPL_EQUIPMENT, mEquipment);
    query.BindValue(":" TBL_FPL_ADEP, mDepartureAerodrome);
    query.BindValue(":" TBL_FPL_TIME, mDepartureTime);
    query.BindValue(":" TBL_FPL_SPEED, mSpeed);
    query.BindValue(":" TBL_FPL_FLEVEL, mLevel);
    query.BindValue(":" TBL_FPL_ROUTE, mRoute);
    query.BindValue(":" TBL_FPL_ADES, mDestinationAerodrome);
    query.BindValue(":" TBL_FPL_EET, mEET);
    query.BindValue(":" TBL_FPL_ALT1, mAltAerodrome);
    query.BindValue(":" TBL_FPL_ALT2, mAltAerodrome2);
    query.BindValue(":" TBL_FPL_OTHER, mOther);
    query.BindValue(":" TBL_FPL_ENTRY_FIR_EET, mEntryFIREET);
    query.BindValue(":" TBL_FPL_VISIBILITY, mVisibility);
    query.BindValue(":" TBL_FPL_EXFPL, mExFPL);
}

/** Fill query for SELECT ALL request */
void FPL::prepareSelectAll(db::CDBQuery& query, const QString& sTable, const QString& sWhere)
{
    query.Prepare("SELECT "
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
                  TBL_FPL_EXFPL " "
                  "FROM " +
                  sTable +
                  sWhere);
}

/** Clone object (full copy without Id) */
FPL FPL::clone(const QString &sTable) const
{
    FPL Fpl = *this;
    Fpl.mId = -1;
    if(!sTable.isEmpty())
        Fpl.table = sTable;
    return Fpl;
}

/** TRUE, if FPL is new */
bool FPL::isNew() const
{
    return mId <= 0;
}

/** FPL Id. */
int FPL::id() const
{
    return mId;
}
void FPL::setId(int id)
{
    mId = id;
}

/** Aircraft Id */
const QString& FPL::aircarftId() const
{
    return mAircraftId;
}
bool FPL::setAircraftId(const QString& sAircraftId)
{
    QString sId = sAircraftId.toUpper();

    if (!PlValidator::ValidateAircraftId(sId))
        return false;

    mAircraftId = sId;
    return true;
}

/** FIR Name */
const QString& FPL::firName() const
{
    return mFIRName;
}
bool FPL::setFirName(const QString& sFirName)
{
    QString sId = sFirName.toUpper();
    mFIRName = sId;
    return true;
}

/** Aircraft Type */
const QString& FPL::aircraftType() const
{
    return mAircraftType;
}
bool FPL::setAircraftType(const QString& sAircraftType)
{
    QString sType = sAircraftType.toUpper();

    if (!PlValidator::ValidateAircraftType(sType))
        return false;

    mAircraftType = sAircraftType;
    return true;
}

/** Turbulentus */
const QString& FPL::turbulenceCategory() const
{
    return mTurbulenceCategory;
}
bool FPL::setTurbulenceCategory(const QString& sTurbulentus)
{
    QString sTurb = sTurbulentus.toUpper();

    if (!PlValidator::validateTurbulenceCategory(sTurb))
        return false;

    mTurbulenceCategory = sTurb;
    return true;
}

/** Flight rules */
const QString& FPL::flightRules() const
{
    return mFlightRules;
}
bool FPL::setFlightRules(const QString& sFlightRules)
{
    QString sRule = sFlightRules.toUpper();

    if (!PlValidator::validateFlightRule(sRule))
        return false;

    mFlightRules = sRule;
    return true;
}

/** Flight type */
const QString& FPL::flightType() const
{
    return mFlightType;
}
bool FPL::setFlightType(const QString& sFlightType)
{
    QString sType = sFlightType.toUpper();

    if (!sType.isEmpty() && !PlValidator::validateFlightType(sType))
        return false;

    mFlightType = sType;
    return true;
}

/** Airplanes aircraftNumber */
int FPL::aircraftNumber() const
{
    return mNumber;
}
bool FPL::setAircraftNumber(int iNumber)
{
    if (!PlValidator::ValidateAirNumber(iNumber))
        return false;

    mNumber = iNumber;
    return true;
}

/** Equipment */
const QString& FPL::equipment() const
{
    return mEquipment;
}
bool FPL::setEquipment(const QString& sEquipment)
{
    QString sEquip = sEquipment.toUpper();

    if (!PlValidator::ValidateEquipment(sEquip))
        return false;

    mEquipment = sEquip;
    return true;
}

/** Departure Aerodrome */
const QString& FPL::departureAerodrome() const
{
    return mDepartureAerodrome;
}

bool FPL::setDepartureAerodrome(const QString& sDepartAero)
{
    QString sAero = sDepartAero.toUpper();

    if (!PlValidator::ValidateAerodrome(sDepartAero))
        return false;

    mDepartureAerodrome = sAero;
    return true;
}

/** Departure Time */
const QTime FPL::departureTime() const
{
    return mDepartureTime.time();
}
const QDateTime FPL::departureDateTime() const
{
    return mDepartureTime;
}
bool FPL::setDepartureTime(const QTime& tDepartTime)
{
    if (!PlValidator::ValidateDepTime(tDepartTime))
        return false;

    mDepartureTime.setTime(tDepartTime);

    return true;
}
bool FPL::setDepartureDateTime(const QDateTime& tDepartTime)
{
    if (!PlValidator::ValidateDepTime(tDepartTime.time()))
        return false;

    mDepartureTime = tDepartTime;

    return true;
}
bool FPL::setDateDOF(const QDate& tDate)
{
    if(tDate.isValid())
    {
        mDepartureTime.setDate(tDate);
        return true;
    }
    return false;
}

/** Speed */
const QString& FPL::speed() const
{
    return mSpeed;
}
bool FPL::setSpeed(const QString& sValue)
{
    QString sSpeed = sValue.toUpper();

    if (!PlValidator::ValidateSpeed(sSpeed))
        return false;

    mSpeed = sSpeed;
    return true;
}

QString FPL::speedUnit() const
{
    if (mSpeed.isEmpty())
        return QString::null;
    else
        return mSpeed.left(1);
}
QString FPL::speedValue() const
{
    if (mSpeed.isEmpty())
        return QString();
    else
        return mSpeed.mid(1);
}

bool FPL::setSpeed(const QString& unit, const QString& value)
{
    return setSpeed(unit + value);
}

/** Level */
const QString& FPL::level() const
{
    return mLevel;
}
bool FPL::setLevel(const QString& value)
{
    QString sLevel = value.toUpper();

    if (!PlValidator::ValidateLevel(value))
        return false;

    mLevel = sLevel;
    return true;
}

QString FPL::levelUnit() const
{
    if (mLevel.isEmpty())
        return QString::null;
    else
        return mLevel.left(1);
}
QString FPL::levelValue() const
{
    if (mLevel.isEmpty())
        return QString();
    else
        return mLevel.mid(1);
}

bool FPL::setLevel(const QString& unit, const QString& value)
{
    return setLevel(unit + value);
}

/** Route */
const QString& FPL::route() const
{
    return mRoute;
}
bool FPL::setRoute(const QString& sValue)
{
    QString sRoute = sValue.toUpper();
    //if(!PlValidator::ValidateRoute(sRoute))
    //   return false;

    mRoute = sRoute;
    return true;
}

/** Destination Aerodrome */
const QString& FPL::destinationAerodrome() const
{
    return mDestinationAerodrome;
}
bool FPL::setDestAerodrome(const QString& sDestAero)
{
    QString sAero = sDestAero.toUpper();

    if (!PlValidator::ValidateAerodrome(sAero))
        return false;

    mDestinationAerodrome = sAero;
    return true;
}

/** Destination Time */
const QString FPL::EET() const
{
    return mEET;
}
bool FPL::setEET(const QString& eet)
{
    QRegExp rexp("\\d{1,2}:\\d{2}");
    if (!rexp.exactMatch(eet))
        return FALSE;
    // Если время меньше, чем отправления, прибытие будет завтра
    mEET = eet;
    return true;
}

/** Alternate aerodromes */
const QString& FPL::altDestinationAerodrome() const
{
    return mAltAerodrome;
}

bool FPL::setAltDestinationAerodrome(const QString& sAltAero)
{
    QString sAero = sAltAero.toUpper();

    if (!sAero.isEmpty() && !PlValidator::ValidateAerodrome(sAero))
        return false;

    mAltAerodrome = sAero;
    return true;
}

const QString& FPL::altDestinationAero2() const
{
    return mAltAerodrome2;
}
bool FPL::setAltDestinationAerodrome2(const QString& sAltAero)
{
    QString sAero = sAltAero.toUpper();

    if (!sAero.isEmpty() && !PlValidator::ValidateAerodrome(sAero))
        return false;

    mAltAerodrome2 = sAero;
    return true;
}

/** Other information */
const QString& FPL::otherInformation() const
{
    return mOther;
}
bool FPL::setOtherInformation(const QString& sOther)
{
    mOther = sOther;
    return true;
}

QString FPL::entryFIREET() const
{
    return mEntryFIREET;
}
bool FPL::setEntryFIREET(const QString& time)
{
    QRegExp rexp("\\d{1,2}:\\d{2}");
    if (rexp.exactMatch(time)) {
        mEntryFIREET = time;
        return true;
    }
    return false;
}

QString FPL::exFPL() const
{
    return mExFPL;
}

void FPL::setExFPL(const QString& exFPL)
{
    // WORKAROUND
    mExFPL = exFPL.simplified();
}

int FPL::visibility() const
{
    return mVisibility;
}

void FPL::setVisibility(const int& iVisibility)
{
    mVisibility = iVisibility;
}

bool FPL::setEntryFIREET(const QStringList& sFIRs, const QVariantMap& vEETs)
{
    bool bRes = false;
    foreach(const QString& sFIR, sFIRs)
    {
        if(vEETs.contains(sFIR))
        {
            bRes = setEntryFIREET(vEETs.value(sFIR).toString());
            if(bRes)
            {
                setFirName(sFIR);
#if defined(DUSHANBE_BUILD)
                if(sFIR == "UTDL")
                    setFirName("UTDD");
#endif
            }
            return bRes;
        }
    }
    return bRes;
}
