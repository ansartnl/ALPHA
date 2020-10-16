#ifndef GROUNDSCENE_H
#define GROUNDSCENE_H

#include <QObject>

#include "IScene.h"
#include "GroundMain.h"
#include "GroundApron.h"
#include "GroundBuilding.h"
#include "GroundParking.h"
#include "GroundPath.h"
#include "GroundPoint.h"
#include "GroundRunway.h"
#include "GroundStart.h"
#include "GroundTower.h"
#include "GroundTrace.h"
#include "GroundSign.h"
#include "GroundCrossroads.h"
#include "GroundRadioRestriction.h"
#include "GroundVehicle.h"
#include "GroundWaypointMarker.h"
#include "front/ground/GUIGroundPath.h"
#include "front/ground/GUIGroundRunway.h"
#include "front/ground/GUIGroundPoint.h"

#include "back/core/Airplane.h"
#include "front/core/GUIAirplane.h"

#include "GroundTVATimer.h"

#include "import/Point.h"
#include "common/AirplaneState.h"

class CCoord;
class CPlane;
class Vehicle;
struct VehicleState;

#ifndef GROUND_CWP
class CSimulator;
class GroundSceneLogic;
#endif

struct TaxiIndex{
    QString sRunway;
    QString sName;
    QString sPath;
};

//! Small object for emitting signals for GroundScene
//! Implemented for avoiding multiple QObject inheritance in GUIGroundScene
class Emitter:
        public QObject
{
    Q_OBJECT
signals:
    void planeLanding(QSharedPointer<CAirplane> pAirplane);
    void newCourse(QString course);

public:

    Emitter(){}

    inline void handleLanding(QSharedPointer<CAirplane> airplane)    
    {emit planeLanding(airplane);}    
};


class GroundScene
        : public IScene
{
public:

    typedef QList<QSharedPointer<GroundApron> > TApronList;
    typedef QList<QSharedPointer<GroundBuilding> > TBuildingList;
    typedef QHash<QString, QSharedPointer<GroundParking> > TParkingMap;
    typedef QList<QSharedPointer<GroundPath> > TPathList;
    typedef QList<QSharedPointer<GroundTrace> > TTraceList;
    typedef QList<QSharedPointer<GroundPoint> > TPointList;
    typedef QHash<quint16, QSharedPointer<GroundPoint> > TPointMap;
    typedef QList<QSharedPointer<GroundRunway> > TRunwayList;
    typedef QList<QSharedPointer<GroundStart> > TStartList;
    typedef QList<QSharedPointer<GroundTower> > TTowerList;
    typedef QList<QSharedPointer<GroundSign> > TSignList;
    typedef QHash<quint16, QSharedPointer<GroundCrossroads> > TCrossroadsMap;
    typedef QHash<quint16, QSharedPointer<CAirplane> > TAirplanesMap;
    typedef QHash<quint16, QSharedPointer<GroundVehicle> > TVehicleMap;
    typedef QList<QSharedPointer<GroundRadioRestriction> > TRadioRestrictionList;
    typedef QHash<int, QSharedPointer<GroundWaypointMarker> > TWaypointMarkerMap;

    //! Constructor
    GroundScene()
        :
          mAltitude(0),
#ifndef GROUND_CWP
          m_pSimulator(0),
          mLogic(0),
#endif
          mEmitter(),
          mRouteHovering(false)
    {}

    inline const Emitter& emitter()
    {return mEmitter;}

#ifndef GROUND_CWP
    inline GroundSceneLogic* logic() const
    {return mLogic;}

    inline void setLogic(GroundSceneLogic* logic)
    {mLogic = logic;}

    inline void setAltitude(double altitude)
    {mAltitude = altitude;}

    //! Set pointer to simulator
    /*!
        \param pSimulator pointer to simulator
    */
    virtual void SetSimulator(CSimulator* pSimulator);

    inline CSimulator* getSimulator() const
    {return m_pSimulator;}

    void SetCentralPoint(const QSharedPointer<atd::CPoint>& pCentral);
    const QSharedPointer<atd::CPoint>& GetCentralPoint() const;
#else
    void SetCentralPoint(const QSharedPointer<CPoint>& pCentral);
    const QSharedPointer<CPoint>& GetCentralPoint() const;
#endif

    //! Destructor
    virtual ~GroundScene();

    virtual void SetFormularDefaultPos();

    inline void setName(const QString& name)
    {mName = name;}

    inline const QString& name() const
    {return mName;}

    inline void setRouteHovering(bool hovering)
    {mRouteHovering = hovering;}

    inline bool routeHovering() const
    {return mRouteHovering;}

    virtual void AddAirplane(const QSharedPointer<CAirplane>& pAirplane);
    virtual void AddVehicle(const QSharedPointer<GroundVehicle>& pVehicle);
    virtual void AddPoint(const QSharedPointer<GroundPoint>& pPoint);
    virtual void AddTower(const QSharedPointer<GroundTower>& pTower);
    virtual void AddStart(const QSharedPointer<GroundStart>& pStart);
    virtual void AddRunway(const QSharedPointer<GroundRunway>& pRunway);
    virtual void AddApron(const QSharedPointer<GroundApron>& pApron);
    virtual void AddPath(const QSharedPointer<GroundPath>& pPath);
    virtual void AddTrace(const QSharedPointer<GroundTrace>& pTrace);
    virtual void AddParking(const QSharedPointer<GroundParking>& pParking);
    virtual void AddBuilding(const QSharedPointer<GroundBuilding>& pBuilding);
    virtual void AddSign(const QSharedPointer<GroundSign>& pSign);
    virtual void AddCrossroads(const QSharedPointer<GroundCrossroads>& pCrossroads);
    virtual void AddRadioRestriction(const QSharedPointer<GroundRadioRestriction>& pRestriction);
    virtual void AddWaypointMarker(const QSharedPointer<GroundWaypointMarker>& pMarker);

    virtual void DelWaypointMarker(
            QSharedPointer<GroundWaypointMarker> pMarker);
    inline TWaypointMarkerMap& GetWaypointMarkers() {return m_WaypointMarkers;}
    //@{
    //! IScene
    //! Update airplane information
    /*!
        \param AirplaneState airplane information
    */
    virtual void UpdateAirplane(CAirplaneState& AirplaneState);
    inline TAirplanesMap& GetAirplanes() {return m_Airplanes;}
    virtual void DelAirplane(
            QSharedPointer<CAirplane> pAirplane,
            bool bSend = true);

    virtual void UpdateVehicle(const VehicleState& vehicleState);
    inline TVehicleMap& getVehicles() {return m_Vehicles;}
    virtual void DelVehicle(
            QSharedPointer<GroundVehicle> pVehicle,
            bool bSend = true);

    virtual void BearingData(CBearingData& BearingData);
    virtual void SetSTCA(AirplaneCollision& ac);
    //@}
    bool IsSquareIncluded(CCoord& Coord, int iDelta);

    inline const TApronList& getAprons() {return m_Aprons;}
    inline const TBuildingList& getBuildings() {return m_Buildings;}
    inline const TParkingMap& getParkings() {return m_Parkings;}
    inline const TPathList& getPaths() {return m_Paths;}
    inline const TTraceList& getTraces() {return m_Traces;}
    inline const TPointMap& getPoints() {return m_Points;}
    inline const TRunwayList& getRunways() {return m_Runways;}
    inline const TStartList& getStarts() {return m_Starts;}
    inline const TTowerList& getTowers() {return m_Towers;}
    inline const TSignList& getSigns() {return m_Signs;}
    inline const TCrossroadsMap& getCrossroads() {return m_Crossroads;}    
    inline const TRadioRestrictionList& getRadioRestricions(){ return m_RadioRestrictions;}

    virtual QSharedPointer<GroundPoint> CreatePoint() const;
    virtual QSharedPointer<GroundTower> CreateTower() const;
    virtual QSharedPointer<GroundStart> CreateStart() const;
    virtual QSharedPointer<GroundRunway> CreateRunway() const;
    virtual QSharedPointer<GroundApron> CreateApron() const;
    virtual QSharedPointer<GroundPath> CreatePath() const;
    virtual QSharedPointer<GroundTrace> CreateTrace() const;
    virtual QSharedPointer<GroundParking> CreateParking() const;
    virtual QSharedPointer<GroundBuilding> CreateBuilding() const;
    virtual QSharedPointer<GroundSign> CreateSign() const;
    virtual QSharedPointer<GroundCrossroads> CreateCrossroads() const;
    virtual QSharedPointer<CAirplane> CreateAirplane() const;
    virtual QSharedPointer<GroundVehicle> CreateVehicle() const;
    virtual QSharedPointer<GroundRadioRestriction> CreateRadioRestriction() const;
    virtual QSharedPointer<GroundWaypointMarker> CreateWaypointMarker() const;

protected:
    CCoord          m_BoundMin;
    CCoord          m_BoundMax;

    QVector<CCoord> m_Boundary;

private:
    //! Coordinates for centering points in global (FIR's) coordinate system    

    QString     mName;
    double      mAltitude;

    TApronList      m_Aprons;
    TBuildingList   m_Buildings;
    TParkingMap    m_Parkings;
    TPathList       m_Paths;
    TTraceList      m_Traces;
    TPointMap       m_Points;
    TRunwayList     m_Runways;
    TStartList      m_Starts;
    TTowerList      m_Towers;
    TSignList      m_Signs;
    TCrossroadsMap m_Crossroads;
    TAirplanesMap   m_Airplanes;
    TVehicleMap     m_Vehicles;
    TRadioRestrictionList m_RadioRestrictions;
    TWaypointMarkerMap  m_WaypointMarkers;

#ifndef GROUND_CWP
    /*! Pointer to simulator */
    CSimulator*        m_pSimulator;
    GroundSceneLogic* mLogic;
    /*! Central point */
    QSharedPointer<atd::CPoint> m_pCentralPoint;
#else
    /*! Central point */
    QSharedPointer<CPoint> m_pCentralPoint;
#endif

    Emitter mEmitter;
    bool mRouteHovering;
};

#endif // GROUNDSCENE_H
