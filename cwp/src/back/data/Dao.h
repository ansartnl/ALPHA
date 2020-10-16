#ifndef DATABASE_H
#define DATABASE_H

#include "BoundaryPoint.h"
#include "MeteoRunway.h"

#include "CFPL.h"
#include "OLDIClients.h"

#include <QtCore/QMap>
#include <QtCore/QPair>
#include <QtCore/QSharedPointer>
#include <QtCore/QList>
#include <QtCore/QDateTime>

#include <set>

struct QUuid;

class IScene;
class Scene;
class CView;
class CSSA;
class CAirplaneType;

class CRestriction;
class CRestrictionUser;
class CHolding;
class CCoord;
class CSFPL;
class CUser;
class CUserRole;
class CSectorZone;
class CSectoring;
class MeteoRunway;
class CBearingPoint;
class CUserPics;
class SvgItem;
namespace core {
    class CWind;
    class CMeteo;
    class CMeteoPoint;
}

namespace db {
    class CDBQuery;
}

class CAirport;
class CPoint;
class CRunway;
class CSSA;
class Zone;

//! Main class to work with database
class Dao: public QObject
//, public CDatabase
{
    Q_OBJECT
    public:
        Dao();
        virtual ~Dao();
        //! Map with zones. Key - unique FIR name
        typedef QMap<QString, QSharedPointer<Scene> > TFIRMap;
        //! Map with airplane types. Key - airplane name
        typedef QMap<QString, QSharedPointer<CAirplaneType> > TAirplaneTypeMap;
        //! Map of users
        typedef QMap<QString, QSharedPointer<CUser> > TUserMap;
        //! List of users
        typedef QList<QSharedPointer<CUser> > TUserList;
        //! List of user roles
        typedef QMap<uint, QSharedPointer<CUserRole> > TUserRoleList;
        //! Table sectoring_zone
        typedef QMultiMap<uint, QSharedPointer<CSectorZone> > TSectorZoneList;
        //! Table client_sectoring
        typedef QMultiMap<uint, uint> TUserSectoringList;
        //! Pointer to sectoring
        typedef QSharedPointer<CSectoring> TSectoringPtr;
        //! List of sectoring
        typedef QMap<uint, TSectoringPtr> TSectoringList;
        //! Map with zones. Key - zone identifier in database
        typedef QMap<quint32, QSharedPointer<Scene> > TSceneMap;

        //! Save FPL into database
        /*!
         \param FlightPlan FPL
         */
        void SaveFPL(CFPL& FlightPlan);
        //! Update FPL in database
        /*!
         \param FlightPlan FPL
         \param bAutoTransaction start ot not transaction
         */
        void UpdateFPL(CFPL& FlightPlan, bool bAutoTransaction = true);

        //! Update time and operator name from exercise in database
        /*!
         \param exerciseID exercise identiefier
         \param fplID FPL identifier
         \param seconds delay before start
         \param sOP operator name
         \param bAutoTransaction start ot not transaction
         */
        void SaveExerciseTimeAndOP(quint32 exerciseID, quint32 fplID,
            quint32 seconds, QString sOP, bool bAutoTransaction = true);

        void LoadFPLs(QList<CFPL>& FPLList, QString sWhere = QString::null) const;
        QSharedPointer<CFPL> LoadFPL(quint32 uiID) const;

        void LoadAirplaneTypes(TAirplaneTypeMap& AirTypes) const;
        void LoadSquawk(Scene& Scene) const;
        void LoadMeteo(uint uiZoneID, IScene& Scene) const;
        void LoadWinds(Scene& Scene) const;

        void LoadPlans(QList<QPair<int, QString> >& list, QString sWhere =
                QString::null) const;
        void LoadMasterPlans(QList<QPair<int, QString> >& list, QString sWhere =
                QString::null) const;

        void LoadMeteoRunway(const QString &airportName, QList<MeteoRunway> &meteos);
        void LoadMeteoAirportSettings(const QString &airportName, QDateTime &lastEditTime,
                                      QString &currentRunway);
        void SaveMeteoRunway(const QList<MeteoRunway> &meteos);
        void SaveMeteoAirportSettings(const QString &airportName, const QDateTime &lastEditTime,
                                      const QString &currentRunway);

        void LoadWinds(QList<core::CWind> &Winds, quint32 uiZoneID) const;
        void LoadMeteos(QList<core::CMeteo> &Meteos, quint32 uiZoneID);
        void LoadMeteoPoints(QList<core::CMeteoPoint> &MeteoPoints,
            quint32 uiMeteoID);
        void SaveWind(core::CWind &Wind);
        void SaveMeteo(core::CMeteo &Meteo);
        void SaveMeteoPoint(core::CMeteoPoint &MeteoPoint);
        void DeleteMeteo(core::CMeteo &Meteo);
        void DeleteMeteoPoint(core::CMeteoPoint &MeteoPoint);
        void DeleteFIRMeteo(QString sIdFir);
        void InsertMeteo(QSharedPointer<core::CMeteo> &Meteo);

        void SaveBearing(const QString sWorkstation, const QString sACID,
            bool bOnlyDelete);
        void GetBearingPoint(CBearingPoint& bearing);
        void LoadBearingList(const QString& sFIR, QList<CBearingPoint>& list);
        void LoadBearingList(quint32& idAirport, QList<CBearingPoint>& list);

        void ActivatePlane(const QVector<QVariant>& bindValues);
        void ClearActivate();

        void GetSysdate(QDateTime& dt);
        quint32 GetDBID();

        void InsertSquawk(quint32 zoneID, quint32 qSquawkFrom,
                    quint32 SquawkTo);
        void InsertWind(quint32 zoneID, quint32 qWindStep, quint32 qWindSpeed,
            quint32 qWindAzimuth);

        void InsertSFPL(const QSharedPointer<CSFPL>& pSFPL);
        void UpdateSFPL(const QSharedPointer<CSFPL>& pSFPL);
        void UndoSFPL(quint32 idSFPL);
        void LoadSFPLs(QList<QSharedPointer<CSFPL> >& SFPLList, quint32 idSFPL = 0);
        void MoveSFPLinRPL(const QSharedPointer<CSFPL>& pSFPL);
        void InsertFPLinFPL_MASTER(quint32 idFPL);
        void ClearFPLByKey(QString sACID, QString sADEP, QString sADES);

        //! Load user
        QSharedPointer<CUser> LoadUser(const QString& sUser);
        void LoadAllUsers(TUserMap& UserMap);
        void SaveAllUsers(const TUserList& UserList);
        void LoadUserRoles(TUserRoleList& UserRoles);
        //! Sectoring
        void LoadSectorZoneAll(TSectorZoneList& SectorZoneList);
        void SaveSectorZoneAll(const TSectorZoneList& SectorZoneList);
        void LoadUserSectorAll(TUserSectoringList& UserSectorList);
        void SaveUserSectorAll(const TUserSectoringList& UserSectorList);
        void LoadSectorIdName(quint32 idZone, QString* name);
        //! Load FIR's id
        void LoadIdFIREnvironment(uint uiSectoringID, QList<uint>& IdFIRList);
        void LoadIdSectorsEnvironment(uint uiSectoringID, uint uiIdFIR, QList<uint> &sectors);
        QString LoadSectoringTypePrivilege(uint uiSectoringID);
        QString LoadSectoringType(uint uiSectoringID);
        //! Load
        void LoadSectoringAll(TSectoringList& SectoringList);

        //! Load zone structure from database
        /*!
         \param Scene zone structure
         */
        void Load(uint uiZoneID, Scene& Scene);
        void Save(Scene& scene);

        //! Load points, which belong to zone
        /*!
         \param Scene zone
         */

        void LoadPoints(Scene& Scene) const;
        //! Load routes, which belong to zone
        /*!
         \param Scene zone
         */

        void LoadRoutes(Scene& Scene) const;
        void LoadSvg(Scene& scene) const;
        //! Load airports, which belong to zone
        /*!
         \param Scene zone
         */
        void LoadAirports(Scene& Scene) const;
        //! Load runways, which belong to airport
        /*!
         \param Scene zone
         \param Airport airport
         */
        void LoadRunways(const Scene& Scene, CAirport& Airport) const;
        //! Load SSA, which belong to runway
        /*!
         \param Scene zone
         \param Runway runway
         */
        void LoadSSA(const Scene& Scene, CRunway& Runway) const;
        //! Load SSA points, which belong to SSA
        /*!
         \param Scene zone
         \param SSA SSA
         */
        void LoadSSAPoints(const Scene& Scene, CSSA& SSA) const;

        void loadSpeedValues(QHash<int, std::set<float> > &values);
        void loadVerticalSpeedValues(QHash<int, std::set<int> > &values);

        //! Load restrictions, which belong to zone
        /*!
         \param Scene zone
         */
        void LoadRestrictions(Scene& Scene) const;
        //! Load restrictions (user zones), which belong to zone
        /*!
         \param Scene zone
         */
        void loadUserRestrictionsUids(int zone_id, QList<QUuid> &uids) const;
        void LoadUserRestrictions(Scene& Scene) const;
        void LoadUserRestriction(Scene& scene, quint32 id) const;
        void loadUserRestriction(Scene& scene, const QUuid &uid) const;
        quint32 SaveUserRestriction(quint32 zoneId, CRestrictionUser &r, bool bDelPoints);
        void UpdateUserRestriction(CRestrictionUser &r, bool bDelPoints);
        void DeleteUserRestriction(const QUuid &uid);
        quint32 GetUserRestrictionIDbyGUID(const QUuid &uid) const;
        //! Load holding patterns, which belong to zone
        /*!
         \param Scene zone
         */
        void LoadHoldings(Scene& Scene) const;

        //! Load Map Notes, which belong to zone
        /*!
            \param Scene zone
        */
        void LoadMapNotes(Scene& scene) const;
        void SaveMapNotes(const Scene& scene) const;
        //! Load zone
        /*!
         \param uiID zone identifier
         \param Scene result
         */
        void LoadZone(quint32 uiID, Scene& Scene) const;
        //! Load FIR by unique name(only CZone)
        /*!
         \param sFIR unique name
         \param Scene result
         */
        void LoadFir(const QString& sFIR, Scene& Scene) const;
        //! Load zone list(only CZone)
        /*!
         \param Zones result with zone list
         */
        void LoadZones(QList<Zone>& Zones, QString sWhere = "") const;
        //! Load FIR list
        /*!
         \param SceneList FIR list
         */
        void LoadFIRs(TSceneMap& SceneList) const;
        //! Load point
        /*!
         \param uiID point identifier
         \return pointer to point
         */
        QSharedPointer<CPoint> LoadPoint(uint uiID) const;

        //! Load holding pattern
        /*!
         \return pointer to holding pattern
         */
        QSharedPointer<CHolding> LoadHolding(quint32 uiHoldID) const;
        //! Load holdings
        /*!
         \param listHolding holding list
         \param sFIR FIR name
         */
        void LoadHoldings(QList<QSharedPointer<CHolding> >& listHolding,
            const QString& sFIR) const;

        //! Load airports
        /*!
         \param uiZoneID zone identifier
         \param AirList airport list
         */
        void LoadAirports(uint uiZoneID, QList<CAirport>& AirList) const;

        //! Load user pics, which belong to zone
        /*!
         \param Scene zone
         */
        void LoadUserPics(Scene& Scene) const;

        //! Conversion from boundary points to polygon
        /*!
         \param pCentral central point
         \param BoundaryList list of boundary point to conversion
         \param CoordList destination list of coordinate
         */
        void BoundaryToPolygon(const QSharedPointer<CPoint>& pCentral,
            const QList<QSharedPointer<CBoundaryPoint> >& BoundaryList, QList<
                    CCoord>& CoordList) const;
        //! init OLDI's tables
        void InitOLDI(const QStringList& listSectors, const bool bClearFPL, const bool bOnlySFPL) const;

        quint32 GetFPLIdByKey(QString sACID, QString sADEP, QString sADES, int iTable) const;
        void UpdateFPLTable(const QSharedPointer<CSFPL>& pSFPL);

        QString GetCWPbySector(const QString &sSector) const;
        QString GetSectorByCWP(const QString &sCWP) const;
        QString GetSectorBySectorShortName(const QString &sSector) const;

        void LoadCWPSectors(OLDIClients::TClientMap& mapCWP, QString sWhere = "");

        void LoadSectorAirports(QHash<quint32, QStringList>& data);
        void SaveSectorAirports(const QHash<quint32, QStringList>& data);
        void LoadAirportRunwaysByName(const QString& airportName, QStringList* runways);

        bool GetCentralPointForAllFIRs(CCoord& coord) const;

        quint32 GetIdAirport(const QString nameAirport);
        QString GetNameAirport(const quint32 idAirport);
        void GetOrcamList(const QString sector_name, QStringList &orcamList) const;
        void GetBearingList(QStringList &bearingList) const;
        void GetFrequencyList(const QString airport_name,QStringList &frequencyList) const;

        static const QString MASTER_FPL_TABLE;
        static const QString MASTER_SFPL_TABLE;
    private:
        //! Load boundary points, which belong to zone
        /*!
         \param Zone zone
         */
        void _LoadBoundaryPoints(Scene& Scene, Zone& Zone) const;

        void _BindSFPLValues(const QSharedPointer<CSFPL>& pSFPL, db::CDBQuery* query) const;

    private:
        quint32 m_DBID;
};
#endif // DATABASE_H
