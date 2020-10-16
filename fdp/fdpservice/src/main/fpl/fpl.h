#ifndef FPL_H
#define FPL_H

#include "aftnmessageprocessor.h"

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QTime>

namespace db
{
    class CDBQuery;
}

/**
 * @brief Entity class for FPL, FFPL tables
 *
 */
class FPL
{
    int mId;
    QString mFIRName;
    QString mAircraftId;
    QString mAircraftType;
    QString mTurbulenceCategory;
    QString mFlightRules;
    QString mFlightType;
    int mNumber;
    QString mEquipment;
    QString mDepartureAerodrome;
    QDateTime mDepartureTime;
    QString mSpeed;
    QString mLevel;
    QString mRoute;
    QString mEntryFIREET;
    QString mDestinationAerodrome;
    QString mEET;
    QString mAltAerodrome;
    QString mAltAerodrome2;
    QString mOther;
    QString mExFPL;
    QDateTime mtrTime;
    int mVisibility;
  public:

    /** Constructor */
    FPL(const QString& table = TBL_FPL, const QString& tableID = TBL_FPL_ID);
    FPL(int id, const QString& table = TBL_FPL, const QString& tableID =
        TBL_FPL_ID);
    FPL(const QSqlRecord& record, const QString& table = TBL_FPL,
      const QString& tableID = TBL_FPL_ID);

    /** Destructor */
    virtual ~FPL();

  public:

    /** Fill query for INSERT request */
    virtual void prepareInsert(db::CDBQuery &query);

    /** Update Id after INSERT request */
    virtual void updateId(db::CDBQuery &query);

    /** Fill query for UPDATE request */
    virtual void prepareUpdate(db::CDBQuery &query);

    /** Fill query for SELECT request */
    virtual void prepareSelect(db::CDBQuery &query);

    /** Update information after SELECT request */
    virtual void updateInfo(db::CDBQuery &query);

    virtual void updateInfo(const QSqlRecord& record);

    /** Fill query for DELETE request */
    virtual void prepareDelete(db::CDBQuery &query);

    /** Set parameters to DB request */
    virtual void updateQuery(db::CDBQuery &query);

    /** Fill query for SELECT ALL request */
    static void prepareSelectAll(db::CDBQuery &query, const QString& sTable = TBL_FPL, const QString& sWhere = QString::null);

    /** Clone object (full copy without Id) */
    FPL clone(const QString& sTable = QString::null) const;

  public:

    bool isNew() const;

    /** FPL Id. */
    int id() const;
    void setId(int id);

    /** FIR Name */
    const QString& firName() const;
    bool setFirName(const QString& sFirName);

    /** Aircraft Id */
    const QString& aircarftId() const;
    bool setAircraftId(const QString& sAircraftId);

    /** Aircraft Type */
    const QString& aircraftType() const;
    bool setAircraftType(const QString& sAircraftType);

    /** Turbulentus */
    const QString& turbulenceCategory() const;
    bool setTurbulenceCategory(const QString& sTurbulentus);

    /** Flight rules */
    const QString& flightRules() const;
    bool setFlightRules(const QString& sFlightRules);

    /** Flight type */
    const QString& flightType() const;
    bool setFlightType(const QString& sFlightType);

    /** Airplanes aircraftNumber */
    int aircraftNumber() const;
    bool setAircraftNumber(int iNumber);

    /** Equipment */
    const QString& equipment() const;
    bool setEquipment(const QString& sEquipment);

    /** Departure Aerodrome */
    const QString& departureAerodrome() const;
    bool setDepartureAerodrome(const QString& sDepartAero);

    /** Departure Time */
    const QTime departureTime() const;
    const QDateTime departureDateTime() const;
    bool setDepartureTime(const QTime& tDepartTime);
    bool setDepartureDateTime(const QDateTime& tDepartTime);
    bool setDateDOF(const QDate& tDate);

    /** Speed */
    const QString& speed() const;
    bool setSpeed(const QString& sSpeed);

    QString speedUnit() const;
    QString speedValue() const;
    bool setSpeed(const QString& unit, const QString& value);

    /** Level */
    const QString& level() const;
    bool setLevel(const QString& sLevel);

    QString levelUnit() const;
    QString levelValue() const;
    bool setLevel(const QString& unit, const QString& value);

    /** Route */
    const QString& route() const;
    bool setRoute(const QString& sRoute);

    /** Enter fir time */
    QString entryFIREET() const;
    bool setEntryFIREET(const QString& time);
    bool setEntryFIREET(const QStringList& sFIRs, const QVariantMap& vEETs);

    /** Destination Aerodrome */
    const QString& destinationAerodrome() const;
    bool setDestAerodrome(const QString& sDestAero);

    /** Destination Time */
    const QString EET() const;
    bool setEET(const QString& tDestTime);

    /** Alternate aerodromes */
    const QString& altDestinationAerodrome() const;
    bool setAltDestinationAerodrome(const QString& sAltAero);

    const QString& altDestinationAero2() const;
    bool setAltDestinationAerodrome2(const QString& sAltAero);

    /** Other information */
    const QString& otherInformation() const;
    bool setOtherInformation(const QString& sOther);

    QString exFPL() const;
    void setExFPL(const QString& exFPL);

    int visibility() const;
    void setVisibility(const int& iVisibility);

    /** sys info */
    QString tableName() const
    { return table; }

  protected:
    QString table;
    QString tableId;
};

#endif // FPL_H
