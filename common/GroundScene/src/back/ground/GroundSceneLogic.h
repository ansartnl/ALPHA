#ifndef GROUNDSCENELOGIC_H
#define GROUNDSCENELOGIC_H

#include "GroundScene.h"
#include <QObject>

#ifndef GROUND_CWP
class GroundSceneLogic:
        public QObject
{
    Q_OBJECT
public:
    GroundSceneLogic()
        :mScene(0)
    {}

    void setScene(GroundScene* scene);

    typedef QList<QSharedPointer<GroundPoint> > TPointList;
    typedef QList<QSharedPointer<GroundPath> > TPathList;
    typedef QHash<quint16, QSharedPointer<GroundPoint> > TPointMap;
    typedef QList<QSharedPointer<GroundStart> > TStartList;
    typedef QHash<QString, QSharedPointer<GroundParking> > TParkingMap;
    typedef QList<QSharedPointer<GroundRunway> > TRunwayList;

    typedef QHash<quint16, QString > TTaxiwayMap;
    typedef QMultiMap<QString, TaxiIndex> TTaxiIndexMap;
    typedef QPair<quint16, QString> TIndexCacheKey;
    typedef QHash<TIndexCacheKey, TPointList > TIndexCacheMap;

    void parseVehiclePath(QSharedPointer<Vehicle> vehicle, const CCoord& coord);

    //! Calculates path, specified via set of coordinates. Takes in account airport GroundPoints.
    QVector<QSharedPointer<atd::CRoutePoint> > computeVehiclePath(const QVector<CCoord>& coordinates);

    //! Parses command into complete point list
    /*!
      \param sPath - a path represented as white-space separated taxiway names [and] runway name (example: "A B 08L")
      \param planeID - airplane ID, for which path is plotted
      \param nParking - number of parking spot to start from
      */
    QVector<QSharedPointer<atd::CRoutePoint> > parseTakeOffPath(QString sPath, quint16 planeID, QString sParking);
    //! Parses path into point list and just returns it
    TPointList computeTakeOffPathPoints(QString sPath, quint16 planeID, QString sParking);
    //! Parses command into complete point list
    /*!
      \param sPath - a path represented as white-space separated taxiway names
      \param planeID - airplane ID, for which path is plotted
      */
    QVector<QSharedPointer<atd::CRoutePoint> > parseLandingPath(QString sPath, quint16 planeID);
    //! Parses path into point list and just returns it
    TPointList computeLandingPathPoints(QString sPath, quint16 planeID);

    //! Computes path between airplane position and random clicked point. Calculation takes in account airplane position
    //! on taxiways, orientation, current route, etc.
    /*!
      \param pPlane - a pointer to airplane
      \param pPoint - a random clicked point
      \param isLanding - determines if this is landing path being computed
      \return true - if some handling has occured, false - otherwise
      */
    bool parseCustomPath(QSharedPointer<CPlane> &pPlane, QSharedPointer<atd::CPoint>& pPoint, bool isLanding);

    void takeoff(QSharedPointer<CPlane> &pPlane);

    //! returns list of taxiway index names for taking off airplane
    QStringList getTakeoffIndexes(const QString& runway);

    //! returns list of taxiway index names for landing airplane
    QStringList getLandingIndexes(quint16 idPlane);
    //! returns taxiway index route for specified airplane and index name
    QString getLandingRoute(quint16 idPlane, QString sIndex);
    //! returns taxiway index route for specified takeoff index name
    QString getTakeoffRoute(QString sIndex);
    //! adds new taxiway index entry
    void addIndex(const QString &sRunway, QString sName, QString sPath);
    //! Removes on cached indexes
    void clearIndexCache();

    inline const CCoord& stuntCenter() const
    {return mStuntCenter;}

    inline void setStuntCenter(const CCoord& center)
    {mStuntCenter = center;}

    TTaxiwayMap& getTaxiways()
    {return mTaxiways;}

    //! returns runway on which specified airplane is
    QSharedPointer<GroundRunway> airplaneOnRunway(quint16 idPlane);

    void pointHoverEnter(IGroundItemGUI *item, int idx);
    void pointHoverLeave(IGroundItemGUI *item, int idx);
    void standHoverEnter(IGroundItemGUI *item, QString name);
    void standHoverLeave(IGroundItemGUI *item, QString name);

public slots:
    void onPlaneLanding(QSharedPointer<CAirplane> airplane);

protected:

    QSharedPointer<GroundPoint> divideLine(const CCoord& first, CCoord second, double ratio) const;
    int getAirplaneNearestPoint(quint32 airID);

    //! Returns reverse course as a string
    QString reverseCourse(const QString& course) const;
    QString reverseCourse(int course) const;
    //! Returns reverse designator name
    QString reverseDesignator(QString sDesignator);

    //! returns point for accurate turning on runway during taking off
    /*!
      \param idPlane - Airplane ID
      \param startCoord - coordinates of point, from where turning begins
      \param sRunway - runway name on which plane is taking off
      */
    QSharedPointer<GroundPoint> getTakeoffSupportPoint(quint16 idPlane, QString sRunway);
    QSharedPointer<GroundPoint> getTakeoffSupportPoint(CCoord startCoord, QString sRunway);    

    //! returns runway on which specified point is
    QSharedPointer<GroundRunway> pointOnRunway(quint16 idPoint);

    //! Marks plane's last custom stunt as parking stunt so it will end it's exrcise
    void markParkingStunt(QSharedPointer<CPlane> &pPlane, const QString& stand) const;

    //! Checks if airplane is on a runway and calculates it's landing path.
    void handleLanding(QSharedPointer<CAirplane> &pAirplane);

    //! Path finding functionality:
    //{@
    //! Returns all points, belonging to specified Taxiway
    TPointList getPointsByTaxiName(QString name);

    //! Find nearest point to the specified one
    /*!
      \param Location - point to search
      \param bExactMatch - makes this method find point index by it's coordinates
      \param dSearchRadius - if not bExactMatch, sets radius for searching (0 - infinite radius)
      \return point index or -1 if no point found
      */
    int findPoint(CCoord Location, bool bExactMatch, double dSearchRadius = 0);

    //! Finds shortest path between specified taxi points
    /*!
      \param from - starting point Index
      \param to - ending point Index
      \param bKeepTaxiway - determines if path should only include current taxiway
    */
    TPointList computePath(quint16 from, quint16 to, bool bKeepTaxiway = false);

    TPointList computePathToParking(quint16 fromPoint, QString toParking);

    //! Returns next not visited point (for "computePath" algorithm)
    QSharedPointer<GroundPoint> getNextSearchPoint(QVector<int> visited, QMap<int, double> pathLength, const TPointMap &points);

    //! Returns taxiways crossroads point
    QSharedPointer<GroundPoint> findTaxiwayIntersection(QString first, QString second);

    //! Returns points, bordering specified parking
    TPointList getParkingApproachPoints(QString sParking);

    //! Finds shortest path to specified taxiway
    TPointList computePathToTaxiway(quint16 from, QString to, bool bReverse = false);

    //! Returns index of the next GroundPoint for specified airplane (or -1 if something is wrong)
    int getPlaneNextPointIndex(quint16 planeID);

    //! Just converts array types
    QVector<QSharedPointer<atd::CRoutePoint> > toRoutePoints(TPointList points);

    //! Parses point list into stunts for specified CPlane. HOLD_SHORT points assumed as stunt endings.
    void createStunts(QSharedPointer<CPlane> &plane, TPointList points, bool bTakeOff);
    //@}

private:
    CCoord  mStuntCenter;
    GroundScene*     mScene;

    TTaxiwayMap     mTaxiways;
    TIndexCacheMap  mIndexCache;
    TTaxiIndexMap   mLandingIndexes;
    TTaxiIndexMap   mTakeoffIndexes;
};
#endif // GROUND_CWP
#endif // GROUNDSCENELOGIC_H
