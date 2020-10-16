#include <QtCore/QVariant>
#include <QtCore/QRegExp>

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlField>

#include "rpl.h"
#include "plvalidator.h"

/** Constructor */
RPL::RPL()
{
  mId = -1;
  mDepartureTime = QDateTime::currentDateTime().toUTC();
  mEET = "01:00";
  mEntryFIREET = "01:00";

  mNumber = 0;
}

RPL::RPL(int iId)
{
  mId = iId;
  mDepartureTime = QDateTime::currentDateTime().toUTC();
  mEET = "01:00";
  mEntryFIREET = "01:00";
  mNumber = 0;
}

RPL::RPL(const QSqlRecord& record)
{
  mId = -1;
  mDepartureTime = QDateTime::currentDateTime().toUTC();
  mEET = "01:00";
  mEntryFIREET = "01:00";
  updateInfo(record);
}

/** Destructor */
RPL::~RPL()
{

}

/** Fill query for INSERT request */
void RPL::prepareInsert(QSqlQuery& query)
{
  query.prepare("INSERT INTO " TBL_RPL "("
  TBL_RPL_FIR_NAME ", "
  TBL_RPL_AIRCRAFT_ID ", "
  TBL_RPL_FRULES ", "
  TBL_RPL_TFLIGHT ", "
  TBL_RPL_NUM ", "
  TBL_RPL_TAIRCRAFT ", "
  TBL_RPL_TURBULENCE_CATEGORY ", "
  TBL_RPL_EQUIPMENT ", "
  TBL_RPL_ADEP ", "
  TBL_RPL_TIME ", "
  TBL_RPL_SPEED ", "
  TBL_RPL_FLEVEL ", "
  TBL_RPL_ROUTE ", "
  TBL_RPL_ADES ", "
  TBL_RPL_EET ", "
  TBL_RPL_ALT1 ", "
  TBL_RPL_ALT2 ", "
  TBL_RPL_OTHER ", "
  TBL_RPL_DAYS ", "
  TBL_RPL_ENTRY_FIR_EET ", "
  TBL_RPL_VALID_FROM ", "
  TBL_RPL_VALID_TO " "
  ") VALUES ( "
  ":" TBL_RPL_FIR_NAME ", "
  ":" TBL_RPL_AIRCRAFT_ID ", "
  ":" TBL_RPL_FRULES ", "
  ":" TBL_RPL_TFLIGHT ", "
  ":" TBL_RPL_NUM ", "
  ":" TBL_RPL_TAIRCRAFT ", "
  ":" TBL_RPL_TURBULENCE_CATEGORY ", "
  ":" TBL_RPL_EQUIPMENT ", "
  ":" TBL_RPL_ADEP ", "
  ":" TBL_RPL_TIME ", "
  ":" TBL_RPL_SPEED ", "
  ":" TBL_RPL_FLEVEL ", "
  ":" TBL_RPL_ROUTE ", "
  ":" TBL_RPL_ADES ", "
  ":" TBL_RPL_EET ", "
  ":" TBL_RPL_ALT1 ", "
  ":" TBL_RPL_ALT2 ", "
  ":" TBL_RPL_OTHER ", "
  ":" TBL_RPL_DAYS ", "
  ":" TBL_RPL_ENTRY_FIR_EET ", "
  ":" TBL_RPL_VALID_FROM ", "
  ":" TBL_RPL_VALID_TO " "
  ")"
  "RETURN "
  TBL_RPL_ID " "
  "INTO "
  ":" TBL_RPL_ID);
  updateQuery(query);
}

/** Update Id after INSERT request */
void RPL::updateId(QSqlQuery& query)
{
  mId = query.boundValue(":"TBL_RPL_ID).toInt();
}

/** Fill query for UPDATE request */
void RPL::prepareUpdate(QSqlQuery& query)
{
  query.prepare("UPDATE " TBL_RPL " SET "
  TBL_RPL_FIR_NAME " = :" TBL_RPL_FIR_NAME ", "
  TBL_RPL_AIRCRAFT_ID " = :" TBL_RPL_AIRCRAFT_ID ", "
  TBL_RPL_FRULES " = :" TBL_RPL_FRULES ", "
  TBL_RPL_TFLIGHT " = :" TBL_RPL_TFLIGHT ", "
  TBL_RPL_NUM " = :" TBL_RPL_NUM ", "
  TBL_RPL_TAIRCRAFT " = :" TBL_RPL_TAIRCRAFT ", "
  TBL_RPL_TURBULENCE_CATEGORY " = :" TBL_RPL_TURBULENCE_CATEGORY ", "
  TBL_RPL_EQUIPMENT " = :" TBL_RPL_EQUIPMENT ", "
  TBL_RPL_ADEP " = :" TBL_RPL_ADEP ", "
  TBL_RPL_TIME " = :" TBL_RPL_TIME ", "
  TBL_RPL_SPEED " = :" TBL_RPL_SPEED ", "
  TBL_RPL_FLEVEL " = :" TBL_RPL_FLEVEL ", "
  TBL_RPL_ROUTE " = :" TBL_RPL_ROUTE ", "
  TBL_RPL_ADES " = :" TBL_RPL_ADES ", "
  TBL_RPL_EET " = :" TBL_RPL_EET ", "
  TBL_RPL_ALT1 " = :" TBL_RPL_ALT1 ", "
  TBL_RPL_ALT2 " = :" TBL_RPL_ALT2 ", "
  TBL_RPL_OTHER " = :" TBL_RPL_OTHER ", "
  TBL_RPL_DAYS " = :" TBL_RPL_DAYS ", "
  TBL_RPL_ENTRY_FIR_EET " = :" TBL_RPL_ENTRY_FIR_EET ", "
  TBL_RPL_VALID_FROM " = :"TBL_RPL_VALID_FROM", "
  TBL_RPL_VALID_TO " = :"TBL_RPL_VALID_TO" "
  "WHERE "
  TBL_RPL_ID " = :" TBL_RPL_ID);
  updateQuery(query);
}

/** Fill query for SELECT request */
void RPL::prepareSelect(QSqlQuery& query)
{
  query.prepare("SELECT "
  TBL_RPL_ID ", "
  TBL_RPL_FIR_NAME ", "
  TBL_RPL_AIRCRAFT_ID ", "
  TBL_RPL_FRULES ", "
  TBL_RPL_TFLIGHT ", "
  TBL_RPL_NUM ", "
  TBL_RPL_TAIRCRAFT ", "
  TBL_RPL_TURBULENCE_CATEGORY ", "
  TBL_RPL_EQUIPMENT ", "
  TBL_RPL_ADEP ", "
  TBL_RPL_TIME ", "
  TBL_RPL_SPEED ", "
  TBL_RPL_FLEVEL ", "
  TBL_RPL_ROUTE ", "
  TBL_RPL_ADES ", "
  TBL_RPL_EET ", "
  TBL_RPL_ALT1 ", "
  TBL_RPL_ALT2 ", "
  TBL_RPL_OTHER ", "
  TBL_RPL_TRTIME ", "
  TBL_RPL_DAYS ", "
  TBL_RPL_ENTRY_FIR_EET ", "
  TBL_RPL_VALID_FROM ", "
  TBL_RPL_VALID_TO " "
  "FROM "
  TBL_RPL " "
  "WHERE "
  TBL_RPL_ID "= :" TBL_RPL_ID);
  updateQuery(query);
}

/** Update information after SELECT request */
void RPL::updateInfo(QSqlQuery& query)
{
  updateInfo(query.record());
}

/** Fill query for DELETE request */
void RPL::prepareDelete(QSqlQuery& query)
{
  query.prepare("DELETE FROM " TBL_RPL "WHERE " TBL_RPL_ID " = :" TBL_RPL_ID);
  query.addBindValue(mId);
}

/** Set parameters to DB request */
void RPL::updateQuery(QSqlQuery& query)
{
  query.bindValue(":" TBL_RPL_ID, mId);
  query.bindValue(":" TBL_RPL_FIR_NAME, mFIRName);
  query.bindValue(":" TBL_RPL_AIRCRAFT_ID, mAircraftId);
  query.bindValue(":" TBL_RPL_FRULES, mFlightRules);
  query.bindValue(":" TBL_RPL_TFLIGHT, mFlightType);
  query.bindValue(":" TBL_RPL_NUM, mNumber);
  query.bindValue(":" TBL_RPL_TAIRCRAFT, mAircraftType);
  query.bindValue(":" TBL_RPL_TURBULENCE_CATEGORY, mTurbulenceCategory);
  query.bindValue(":" TBL_RPL_EQUIPMENT, mEquipment);
  query.bindValue(":" TBL_RPL_ADEP, mDepartureAerodrome);
  query.bindValue(":" TBL_RPL_TIME, mDepartureTime);
  query.bindValue(":" TBL_RPL_SPEED, mSpeed);
  query.bindValue(":" TBL_RPL_FLEVEL, mLevel);
  query.bindValue(":" TBL_RPL_ROUTE, mRoute);
  query.bindValue(":" TBL_RPL_ADES, mDestinationAerodrome);
  query.bindValue(":" TBL_RPL_EET, mEET);
  query.bindValue(":" TBL_RPL_ALT1, mAltAerodrome);
  query.bindValue(":" TBL_RPL_ALT2, mAltAerodrome2);
  query.bindValue(":" TBL_RPL_OTHER, mOther);
  query.bindValue(":" TBL_RPL_DAYS, mDays);
  query.bindValue(":" TBL_RPL_ENTRY_FIR_EET, mEntryFIREET);
  query.bindValue(":" TBL_RPL_VALID_FROM, mValidFrom);
  query.bindValue(":" TBL_RPL_VALID_TO, mValidTo);
}

void RPL::updateInfo(const QSqlRecord& record)
{
  mId = record.value(TBL_RPL_ID).toInt();
  mFIRName = record.value(TBL_RPL_FIR_NAME).toString();
  mAircraftId = record.value(TBL_RPL_AIRCRAFT_ID).toString();
  mFlightRules = record.value(TBL_RPL_FRULES).toString();
  mFlightType = record.value(TBL_RPL_TFLIGHT).toString();
  mNumber = record.value(TBL_RPL_NUM).toInt();
  mAircraftType = record.value(TBL_RPL_TAIRCRAFT).toString();
  mTurbulenceCategory = record.value(TBL_RPL_TURBULENCE_CATEGORY).toString();
  mEquipment = record.value(TBL_RPL_EQUIPMENT).toString();
  mDepartureAerodrome = record.value(TBL_RPL_ADEP).toString();
  mDepartureTime = record.value(TBL_RPL_TIME).toDateTime();
  mSpeed = record.value(TBL_RPL_SPEED).toString();
  mLevel = record.value(TBL_RPL_FLEVEL).toString();
  mRoute = record.value(TBL_RPL_ROUTE).toString();
  mDestinationAerodrome = record.value(TBL_RPL_ADES).toString();
  mEET = record.value(TBL_RPL_EET).toString();
  mAltAerodrome = record.value(TBL_RPL_ALT1).toString();
  mAltAerodrome2 = record.value(TBL_RPL_ALT2).toString();
  mOther = record.value(TBL_RPL_OTHER).toString();
  mDays = record.value(TBL_RPL_DAYS).toString();
  mEntryFIREET = record.value(TBL_RPL_ENTRY_FIR_EET).toString();
  mValidFrom = record.value(TBL_RPL_VALID_FROM).toDateTime();
  mValidTo = record.value(TBL_RPL_VALID_TO).toDateTime();
}

/** Fill query for SELECT ALL request */
void RPL::prepareSelectAll(QSqlQuery& query)
{
  query.prepare("SELECT "
  TBL_RPL_ID ", "
  TBL_RPL_FIR_NAME ", "
  TBL_RPL_AIRCRAFT_ID ", "
  TBL_RPL_FRULES ", "
  TBL_RPL_TFLIGHT ", "
  TBL_RPL_NUM ", "
  TBL_RPL_TAIRCRAFT ", "
  TBL_RPL_TURBULENCE_CATEGORY ", "
  TBL_RPL_EQUIPMENT ", "
  TBL_RPL_ADEP ", "
  TBL_RPL_TIME ", "
  TBL_RPL_SPEED ", "
  TBL_RPL_FLEVEL ", "
  TBL_RPL_ROUTE ", "
  TBL_RPL_ADES ", "
  TBL_RPL_EET ", "
  TBL_RPL_ALT1 ", "
  TBL_RPL_ALT2 ", "
  TBL_RPL_OTHER ", "
  TBL_RPL_TRTIME ", "
  TBL_RPL_DAYS ", "
  TBL_RPL_ENTRY_FIR_EET ", "
  TBL_RPL_VALID_FROM ", "
  TBL_RPL_VALID_TO ", "
  "FROM "
  TBL_RPL);
}

/** Clone object (full copy without Id) */
RPL RPL::clone() const
{
  RPL rpl = *this;
  rpl.mId = -1;
  return rpl;
}

/** Valid From date */
const QDate RPL::validFrom() const
{
  return mValidFrom.date();
}

bool RPL::setValidFrom(const QDate& dDate)
{
  mValidFrom.setDate(dDate);
  return true;
}

/** Valid To date */
const QDate RPL::validTo() const
{
  return mValidTo.date();
}

bool RPL::setValidTo(const QDate& date)
{
  if (date < mValidFrom.date())
    return false;
  mValidTo.setDate(date);
  return true;
}
/** Flight Days */
const QString& RPL::days() const
{
  return mDays;
}
bool RPL::setDays(const QString& sDays)
{
  if (!PlValidator::ValidateFlightDays(sDays))
    return false;

  mDays = sDays;
  return true;
}

bool RPL::isNew() const
{
  return mId < 0;
}

int RPL::getId() const
{
  return mId;
}

/** Aircraft Id */
const QString& RPL::aircarftId() const
{
  return mAircraftId;
}
bool RPL::setAircraftId(const QString& sAircraftId)
{
  QString sId = sAircraftId.toUpper();

  if (!PlValidator::ValidateAircraftId(sId))
    return false;

  mAircraftId = sId;
  return true;
}

/** FIR Name */
const QString& RPL::firName() const
{
  return mFIRName;
}
bool RPL::setFirName(const QString& sFirName)
{
  QString sId = sFirName.toUpper();
  mFIRName = sId;
  return true;
}

/** Aircraft Type */
const QString& RPL::aircraftType() const
{
  return mAircraftType;
}
bool RPL::setAircraftType(const QString& sAircraftType)
{
  QString sType = sAircraftType.toUpper();

  if (!PlValidator::ValidateAircraftType(sType))
    return false;

  mAircraftType = sAircraftType;
  return true;
}

/** Turbulentus */
const QString& RPL::turbulenceCategory() const
{
  return mTurbulenceCategory;
}
bool RPL::setTurbulenceCategory(const QString& turbulence)
{
  QString turbulenceUp = turbulence.toUpper();

  if (!PlValidator::validateTurbulenceCategory(turbulenceUp))
    return false;

  mTurbulenceCategory = turbulenceUp;
  return true;
}

/** Flight rules */
const QString& RPL::flightRules() const
{
  return mFlightRules;
}
bool RPL::setFlightRules(const QString& sFlightRules)
{
  QString sRule = sFlightRules.toUpper();

  if (!PlValidator::validateFlightRule(sRule))
    return false;

  mFlightRules = sRule;
  return true;
}

/** Flight type */
const QString& RPL::flightType() const
{
  return mFlightType;
}
bool RPL::setFlightType(const QString& sFlightType)
{
  QString sType = sFlightType.toUpper();

  if (!sType.isEmpty() && !PlValidator::validateFlightType(sType))
    return false;

  mFlightType = sType;
  return true;
}

/** Airplanes aircraftNumber */
int RPL::aircraftNumber() const
{
  return mNumber;
}
bool RPL::setAircraftNumber(int iNumber)
{
  if (!PlValidator::ValidateAirNumber(iNumber))
    return false;

  mNumber = iNumber;
  return true;
}

/** Equipment */
const QString& RPL::equipment() const
{
  return mEquipment;
}
bool RPL::setEquipment(const QString& sEquipment)
{
  QString sEquip = sEquipment.toUpper();

  if (!PlValidator::ValidateEquipment(sEquip))
    return false;

  mEquipment = sEquip;
  return true;
}

/** Departure Aerodrome */
const QString& RPL::departureAerodrome() const
{
  return mDepartureAerodrome;
}
bool RPL::setDepartureAerodrome(const QString& sDepartAero)
{
  QString sAero = sDepartAero.toUpper();

  if (!PlValidator::ValidateAerodrome(sDepartAero))
    return false;

  mDepartureAerodrome = sAero;
  return true;
}

/** Departure Time */
const QTime RPL::departureTime() const
{
  return mDepartureTime.time();
}

bool RPL::setDepartureTime(const QTime& departureTime)
{
  mDepartureTime.setTime(departureTime);
  return true;
}

/** Speed */
const QString& RPL::speed() const
{
  return mSpeed;
}
bool RPL::setSpeed(const QString& value)
{
  QString speed = value.toUpper();

  if (!PlValidator::ValidateSpeed(speed))
    return false;

  mSpeed = speed;
  return true;
}

QString RPL::speedUnit() const
{
  if (mSpeed.isEmpty())
    return QString::null;
  else
    return mSpeed.at(0);
}
QString RPL::speedValue() const
{
  if (mSpeed.isEmpty())
    return QString();
  else
    return mSpeed.mid(1);
}

bool RPL::setSpeed(const QString& unit, const QString& value)
{
  return setSpeed(unit + value);
}

/** Level */
const QString& RPL::level() const
{
  return mLevel;
}
bool RPL::setLevel(const QString& sValue)
{
  QString sLevel = sValue.toUpper();

  if (!PlValidator::ValidateLevel(sValue))
    return false;

  mLevel = sLevel;
  return true;
}

QString RPL::levelUnit() const
{
  if (mLevel.isEmpty())
    return QString::null;
  else
    return mLevel.at(0);
}
QString RPL::levelValue() const
{
  if (mLevel.isEmpty())
    return QString();
  else
    return mLevel.mid(1);
}

bool RPL::setLevel(const QString& unit, const QString& value)
{
  return setLevel(unit + value);
}

/** Route */
const QString& RPL::route() const
{
  return mRoute;
}
bool RPL::setRoute(const QString& sValue)
{
  QString sRoute = sValue.toUpper();
  //if(!PlValidator::ValidateRoute(sRoute))
  //   return false;

  mRoute = sRoute;
  return true;
}

/** Destination Aerodrome */
const QString& RPL::destinationAerodrome() const
{
  return mDestinationAerodrome;
}
bool RPL::setDestinationAerodrome(const QString& sDestAero)
{
  QString sAero = sDestAero.toUpper();

  if (!PlValidator::ValidateAerodrome(sAero))
    return false;

  mDestinationAerodrome = sAero;
  return true;
}

/** Destination Time */
const QString RPL::EET() const
{
  return mEntryFIREET;
}

bool RPL::setEET(const QString& eet)
{
  QRegExp rexp("\\d{1,2}:\\d{2}");
  if (!rexp.exactMatch(eet))
    return false;

  mEntryFIREET = eet;
  return true;
}

/** Alternate aerodromes */
const QString& RPL::altDestinationAerodrome() const
{
  return mAltAerodrome;
}

bool RPL::setAltDestinationAerodrome(const QString& sAltAero)
{
  QString sAero = sAltAero.toUpper();

  if (!sAero.isEmpty() && !PlValidator::ValidateAerodrome(sAero))
    return false;

  mAltAerodrome = sAero;
  return true;
}

const QString& RPL::altdestinationAerodrome2() const
{
  return mAltAerodrome2;
}
bool RPL::setAltDestinationAerodrome2(const QString& sAltAero)
{
  QString sAero = sAltAero.toUpper();

  if (!sAero.isEmpty() && !PlValidator::ValidateAerodrome(sAero))
    return false;

  mAltAerodrome2 = sAero;
  return true;
}

/** Other information */
const QString& RPL::otherInfo() const
{
  return mOther;
}
bool RPL::setOtherInfo(const QString& sOther)
{
  mOther = sOther;
  return true;
}

QString RPL::entryFIREET() const
{
  return mEET;
}

bool RPL::setEntryFIREET(const QString& time)
{
  QRegExp rexp("\\d{1,2}:\\d{2}");
  if (rexp.exactMatch(time)) {
    mEET = time;
    return true;
  }
  return false;
}
