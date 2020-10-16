#ifndef RPL_H
#define RPL_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QTime>
#include <QtCore/QDate>

#include "fpl.h"

#include <QtSql/QSqlRecord>

class QSqlQuery;

  /**
   * @brief Entity class for RPL table
   *
   */
  class RPL
  {
    protected:

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
      QString mEET;
      QString mDestinationAerodrome;
      QString mEntryFIREET;
      QString mAltAerodrome;
      QString mAltAerodrome2;
      QString mOther;
      bool mExFPL;
      QDateTime mtrTime;

      QDateTime mValidFrom;
      QDateTime mValidTo;

      QString mDays;
    public:

      /** Constructor */
      RPL();
      RPL(int iId);
      RPL(const QSqlRecord& record);

      /** Destructor */
      ~RPL(void);

    public:

      bool isNew() const;
      /** FPL Id. */
      int getId() const;

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
      bool setDepartureTime(const QTime& tDepartTime);

      /** Speed */
      const QString& speed() const;
      bool setSpeed(const QString& speed);

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

      /** Destination Aerodrome */
      const QString& destinationAerodrome() const;
      bool setDestinationAerodrome(const QString& sDestAero);

      /** EET */
      const QString EET() const;
      bool setEET(const QString& tDestTime);

      /** Alternate aerodromes */
      const QString& altDestinationAerodrome() const;
      bool setAltDestinationAerodrome(const QString& sAltAero);

      const QString& altdestinationAerodrome2() const;
      bool setAltDestinationAerodrome2(const QString& sAltAero);

      /** Other information */
      const QString& otherInfo() const;
      bool setOtherInfo(const QString& sOther);

    public:
      /** Fill query for INSERT request */
      virtual void prepareInsert(QSqlQuery &query);

      /** Update Id after INSERT request */
      virtual void updateId(QSqlQuery &query);

      /** Fill query for UPDATE request */
      virtual void prepareUpdate(QSqlQuery &query);

      /** Fill query for SELECT request */
      virtual void prepareSelect(QSqlQuery &query);

      /** Update information after SELECT request */
      virtual void updateInfo(QSqlQuery &query);
      virtual void updateInfo(const QSqlRecord& record);

      /** Fill query for DELETE request */
      virtual void prepareDelete(QSqlQuery &query);

      /** Set parameters to DB request */
      virtual void updateQuery(QSqlQuery &query);

      /** Fill query for SELECT ALL request */
      static void prepareSelectAll(QSqlQuery &query);

      /** Clone object (full copy without Id) */
      RPL clone() const;

    public:

      /** Valid From date */
      const QDate validFrom() const;
      bool setValidFrom(const QDate &date);

      /** Valid To date */
      const QDate validTo() const;
      bool setValidTo(const QDate &date);

      /** Flight Days */
      const QString &days() const;
      bool setDays(const QString &sDays);
  };

#endif // RPL_H
