#include <QtCore/QVariant>

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>

#include "notam.h"
#include "aftnmessageprocessor.h"

/** Constructor */
Notam::Notam(void)
{
  mId = -1;
  mValidUntil = mValidFrom = QDateTime::currentDateTime().toUTC();
  mQLowerLimit = mRadius = mReplSnumber = mReplYear = mSnumber = mQUpperLimit
      = mYear = 0;
}

Notam::Notam(int iId)
{
  mId = iId;
  mValidUntil = mValidFrom = QDateTime::currentDateTime().toUTC();
  mQLowerLimit = mRadius = mReplSnumber = mReplYear = mSnumber = mQUpperLimit
      = mYear = 0;
}

Notam::Notam(const QSqlRecord& record)
{
  updateInfo(record);
}

/** Destructor */
Notam::~Notam(void)
{
}

/** Fill query for SELECT request */
void Notam::prepareSelect(QSqlQuery& query)
{
  query.prepare("SELECT "
  TBL_NOTAM_ID ", "
  TBL_NOTAM_SERIES ", "
  TBL_NOTAM_SNUMBER ", "
  TBL_NOTAM_YEAR ", "
  TBL_NOTAM_NATURE ", "
  TBL_NOTAM_REPLACE_SNUMBER ", "
  TBL_NOTAM_REPLACE_YEAR ", "
  TBL_NOTAM_FIR ", "
  TBL_NOTAM_SUBJECT ", "
  TBL_NOTAM_CONDITION ", "
  TBL_NOTAM_TRAFFIC ", "
  TBL_NOTAM_PURPOSE ", "
  TBL_NOTAM_SCOPE ", "
  TBL_NOTAM_Q_LOWER_LIMIT ", "
  TBL_NOTAM_Q_UPPER_LIMIT ", "
  TBL_NOTAM_LOCATION ", "
  TBL_NOTAM_RADIUS ", "
  TBL_NOTAM_AERODROME ", "
  TBL_NOTAM_VALID_FROM ", "
  TBL_NOTAM_VALID_UNTIL ", "
  TBL_NOTAM_VALID_UNTIL_T ", "
  TBL_NOTAM_SCHEDULE ", "
  TBL_NOTAM_DESCRIPTION ", "
  TBL_NOTAM_LOWER_LIMIT ", "
  TBL_NOTAM_UPPER_LIMIT
  " FROM "
  TBL_NOTAM
  " WHERE "
  TBL_NOTAM_ID "= :" TBL_NOTAM_ID);

  updateQuery(query);
}

/** Update information after SELECT request */
void Notam::updateInfo(QSqlQuery& query)
{
  updateInfo(query.record());
}
/** Update information after SELECT request */
void Notam::updateInfo(const QSqlRecord& record)
{

  mId = record.value(TBL_NOTAM_ID).toInt();
  mSeries = record.value(TBL_NOTAM_SERIES).toString();
  mSnumber = record.value(TBL_NOTAM_SNUMBER).toInt();
  mYear = record.value(TBL_NOTAM_YEAR).toInt();
  mNature = record.value(TBL_NOTAM_NATURE).toString();
  mReplSnumber = record.value(TBL_NOTAM_REPLACE_SNUMBER).toInt();
  mReplYear = record.value(TBL_NOTAM_REPLACE_YEAR).toInt();
  mFir = record.value(TBL_NOTAM_FIR).toString();
  mSubject = record.value(TBL_NOTAM_SUBJECT).toString();
  mCondition = record.value(TBL_NOTAM_CONDITION).toString();
  mTraffic = record.value(TBL_NOTAM_TRAFFIC).toString();
  mPurpose = record.value(TBL_NOTAM_PURPOSE).toString();
  mScope = record.value(TBL_NOTAM_SCOPE).toString();
  mQLowerLimit = record.value(TBL_NOTAM_Q_LOWER_LIMIT).toInt();
  mQUpperLimit = record.value(TBL_NOTAM_Q_UPPER_LIMIT).toInt();
  mLocation = record.value(TBL_NOTAM_LOCATION).toString();
  mRadius = record.value(TBL_NOTAM_RADIUS).toInt();
  mAerodrom = record.value(TBL_NOTAM_AERODROME).toString();
  mValidFrom = record.value(TBL_NOTAM_VALID_FROM).toDateTime();
  mValidUntil = record.value(TBL_NOTAM_VALID_UNTIL).toDateTime();
  mValidUntilT = record.value(TBL_NOTAM_VALID_UNTIL_T).toString();
  mSchedule = record.value(TBL_NOTAM_SCHEDULE).toString();
  mDescription = record.value(TBL_NOTAM_DESCRIPTION).toString();
  mLowerLimit = record.value(TBL_NOTAM_LOWER_LIMIT).toString();
  mUpperLimit = record.value(TBL_NOTAM_UPPER_LIMIT).toString();
}

/** Fill query for DELETE request */
void Notam::prepareDelete(QSqlQuery& query)
{
  query.prepare("DELETE FROM " TBL_NOTAM
  " WHERE " TBL_NOTAM_ID " = :" TBL_NOTAM_ID);

  updateQuery(query);
}

/** Set parameters to DB request */
void Notam::updateQuery(QSqlQuery& query)
{
  mValidFrom.setDate(QDateTime::currentDateTime().toUTC().date());
  mValidUntil.setDate(QDateTime::currentDateTime().toUTC().date());

  query.bindValue(":" TBL_NOTAM_ID, mId);
}

/** Clone object (full copy without Id) */
Notam Notam::clone() const
{
  Notam Notam = *this;
  return Notam;
}
