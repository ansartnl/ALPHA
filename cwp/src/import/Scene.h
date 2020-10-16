#ifndef SCENE_H
#define SCENE_H

#include "Airport.h"
#include "Route.h"
#include "Restriction.h"
#include "Holding.h"
#include "Wind.h"
#include "Dao.h"
#include "Zone.h"
#include "SvgItem.h"
#include "MapNote.h"
#include "UserPics.h"

#include <QtCore/QSharedPointer>
#include <QtCore/QMap>
#include <QHash>

class CCoord;
class CMaster;

//! Class describes zone structure
class Scene: public Zone, public ItemHandler
{
  public:
    //! List of squawks
    typedef QList<int> TSquawk;
    //! List of winds
    typedef QList<QSharedPointer<core::CWind> > TWindList;

    //! Constructor
    Scene();
    //! Constructor
    /*!
     \param Zone zone information
     */
    Scene(const Zone& Zone);
    //! Destructor
    virtual ~Scene();

    //! List of restriction zones
    typedef QList<QSharedPointer<CRestriction> > TRestrictionList;
    //! List of user restriction zones
    typedef QList<QSharedPointer<CRestrictionUser> > TRestrictionUserList;
    //! List of holding patterns
    typedef QList<QSharedPointer<CHolding> > THoldingList;
    //! Map of points. Key - name, ICAO and type of a point
    typedef QMap<CPointKey, QSharedPointer<CPoint> > TPointsMap;
    //! Map of routes. Key - route name
    typedef QMap<QString, QSharedPointer<CRoute> > TRoutsMap;
    //! Map of airports. Key - name & ICAO of a airport
    typedef QMap<CAirportKey, QSharedPointer<CAirport> > TAirportsMap;
    //! Pointer to child scene
    typedef QSharedPointer<Scene> TScenePtr;
    //! List of child scene
    typedef QList<TScenePtr> TSceneList;
    //! All map notes
    typedef QHash<quint32, QSharedPointer<MapNote> > TNotesMap;
    //! List of user pics
    typedef QList<QSharedPointer<CUserPics> > TUserPicsList;
    //! List of user SVG
    typedef QList<QSharedPointer<SvgItem> > TSvgItemList;
    //! Add boundary point
    /*!
     \param pPoint pointer to boundary point
     */
    virtual void AddBoundaryPoint(const QSharedPointer<CBoundaryPoint>& pPoint);
    //! Add point
    /*!
     \param pPoint pointer to point
     */
    virtual bool AddPoint(const QSharedPointer<CPoint>& pPoint);
    //! Add route
    /*!
     \param pRoute pointer to route
     */
    virtual bool AddRoute(const QSharedPointer<CRoute>& pRoute);
    //! Add pointer to airport
    /*!
     \param pAirport pointer to airport
     */
    virtual bool AddAirport(const QSharedPointer<CAirport>& pAirport);
    //! Add pointer to restriction zone
    /*!
     \param pRestriction pointer to restriction zone
     */
    virtual void AddRestriction(
      const QSharedPointer<CRestriction>& pRestriction);
    //! Add pointer to user's restriction zone
    /*!
     \param pRestriction pointer to user restriction zone
     */
    virtual void AddUserRestriction(
      const QSharedPointer<CRestrictionUser>& pRestriction);
    //! Add pointer to holding pattern
    /*!
     \param pHolding pointer to holding pattern
     */
    virtual void AddHolding(const QSharedPointer<CHolding>& pHolding);
    //! Add sector
    /*!
     \param pSector sector
     */
    virtual void AddSector(const QSharedPointer<Scene>& pSector);

    virtual void AddNote(const QSharedPointer<MapNote>& pNote);
    //! Add pointer to user pics
    /*!
     \param pUserPics pointer to user pics
     */
    virtual void AddUserPics( const QSharedPointer<CUserPics>& pUserPics);
    virtual void AddSvgItem(const QSharedPointer<SvgItem>& pSvgItem);
    //! Remove user's restriction zone from scene
    /*!
     \param id User restriction zone identifier
     */
    virtual QSharedPointer<CRestrictionUser> DelUserRestriction(quint32 id);
    virtual QSharedPointer<CUserPics> DelUserPics(quint32 id);
    virtual QSharedPointer<SvgItem> DelSVG(quint32 id);

    //! Get points
    /*!
     \return points
     */
    const TPointsMap& GetPoints() const
    {
      return m_Points;
    }
    //! Get points
    /*!
     \return points
     */
    TPointsMap& GetPoints()
    {
      return m_Points;
    }
    //! Get routes
    /*!
     \return routes
     */
    const TRoutsMap& GetRoutes() const
    {
      return m_Routs;
    }
    //! Get routes
    /*!
     \return routes
     */
    TRoutsMap& GetRoutes()
    {
      return m_Routs;
    }
    //! Get airports
    /*!
     \return airports
     */
    const TAirportsMap& GetAirports() const
    {
      return m_Airports;
    }
    bool IsAirportExistAndSvgAdded(const QString& code, const QString& name, QSharedPointer<SvgItem>& ptr);

    //! Get airports
    /*!
     \return airports
     */
    TAirportsMap& GetAirports()
    {
      return m_Airports;
    }

    virtual const TNotesMap& GetNotes() const { return m_Notes; }
    virtual TNotesMap& GetNotes() { return m_Notes; }

    //! Set central point of a zone
    /*!
     \param central point of a zone
     */
    void SetCentralPoint(const QSharedPointer<CPoint>& pCentral);
    //! Get central point of a zone
    /*!
     \return central point of a zone
     */
    const QSharedPointer<CPoint>& GetCentralPoint() const;
    //! Get restrinction zones
    /*!
     \return restrinction zones
     */
    TRestrictionList& GetRestriction()
    {
      return m_Restriction;
    }
    //! Get restrinction zones
    /*!
     \return user restrinction zones
     */
    TRestrictionUserList& GetRestrictionUser() { return m_RestrictionUser; }
    //! Get holding patterns
    /*!
     \return holding patterns
     */
    THoldingList& GetHolding()
    {
      return m_Holding;
    }
    //! Get user pics
    /*!
     \return user pics
     */
    TUserPicsList& GetUserPics()
    {
      return m_UserPics;
    }
    /*! \name Factories */
    //@{
    //! Create point
    /*!
     \return pointer to point
     */
    virtual QSharedPointer<CPoint> CreatePoint(CPoint::Type type) const;
    //! Create boundary point
    /*!
     \return pointer to boundary point
     */
    virtual QSharedPointer<CBoundaryPoint> CreateBoundaryPoint() const;
    //! Create route
    /*!
     \return pointer to route
     */
    virtual QSharedPointer<CRoute> CreateRoute() const;
    //! Create airport
    /*!
     \return pointer to airport
     */
    virtual QSharedPointer<CAirport> CreateAirport() const;
    //! Create runway
    /*!
     \return pointer to runway
     */
    virtual QSharedPointer<CRunway> CreateRunway() const;
    //! Create SSA(SID/STAR/Approach)
    /*!
     \return pointer to SSA
     */
    virtual QSharedPointer<CSSA> CreateSSA() const;
    //! Create restriction zone
    /*!
     \return pointer to restriction zone
     */
    virtual QSharedPointer<CRestriction> CreateRestriction() const;
    //! Create user's restriction zone
    /*!
     \return pointer to user restriction zone
     */
    virtual QSharedPointer<CRestrictionUser> CreateUserRestriction() const;
    //! Create holding pattern
    /*!
     \return pointer to holding pattern
     */
    virtual QSharedPointer<CHolding> CreateHolding() const;
    //! Create zone
    /*!
     \return pointer to zone
     */
    virtual QSharedPointer<Scene> CreateSector() const;

    virtual QSharedPointer<MapNote> CreateNote() const;
    //! Create user pics
    /*!
     \return pointer to user pics
     */
    virtual QSharedPointer<CUserPics> CreateUserPics() const;

    virtual QSharedPointer<SvgItem> CreateSvgItem(const QString& path) const;
    //@}

    //! Set FIR or sector activity and all subelemnets
    virtual void MarkItemAllActive(bool bActive);

    //! Define whether polygon includes coordinate
    /*!
     \param Coord coordinate
     \param iDeltaDistance delta relative to mim & max distance in meters
     \return true if coordinate is included
     */
    bool IsIncluded(CCoord& Coord, const int iDeltaDistance = 6000);
    //! Define whether approximate square includes coordinate
    /*!
     \param Coord coordinate
     \param iDelta delta relative to mim & max coordinate of a zone
     \return true if coordinate is included
     */
    bool IsSquareIncluded(CCoord& Coord, int iDelta);
    //! Define whether zone includes restriction zones
    /*!
     \param Restriction reference to restriction zone
     \return true if restriction zone is included
     */
    bool IsRestrictIncluded(CRestriction& Restriction);
    bool IsRestrictUserIncluded(CRestrictionUser& Restriction);

    //! Add child zone
    /*!
     \param pZone child zone
     */
    void AddZone(const TScenePtr& pZone);
    //! Get childs
    /*!
     \return zone list
     */
    TSceneList& GetSectorList();
    //! Find sector by identifier
    TScenePtr FindSector(uint uiSectorID);
    //! Find sectors by point and altitude
    TSceneList FindSectorsByPoint(QString sPointName, double dAltitudeMeters = 0, bool bInside = false);

   //! Add pointer to wind
    /*!
     \param pWind pointer to wind
     */
    virtual void AddWind(const QSharedPointer<core::CWind>& pWind);
    //! Add squawk to squawk list
    /*!
     \param iSquawk squawk
     */
    void AddSquawk(int iSquawk)
    {
      m_Squawk.append(iSquawk);
    }
    //! Get squawks
    /*!
     \return squawks
     */
    TSquawk& GetSquawk()
    {
      return m_Squawk;
    }
    //! Get winds
    /*!
     \return winds
     */
    TWindList& GetWinds()
    {
      return m_Wind;
    }
    bool IsMainFIR() const
    {
        return m_bIsMainFIR;
    }
    void SetMainFIR(bool b)
    {
        m_bIsMainFIR = b;
    }

  protected:
    /*! Central point */
    QSharedPointer<CPoint> m_pCentralPoint;
    /*! Minimum coordinate of boundary points*/
    CCoord m_BoundMin;
    /*! Maximum coordinate of boundary points*/
    CCoord m_BoundMax;
    /*! Points */
    TPointsMap m_Points;
    /*! Routs */
    TRoutsMap m_Routs;
    /*! Airports */
    TAirportsMap m_Airports;
    /*! Restriction */
    TRestrictionList m_Restriction;
    /*! Restriction */
    TRestrictionUserList m_RestrictionUser;
    /*! Holding */
    THoldingList m_Holding;
    //! Scene children
    TSceneList m_Childs;
    /*! Squawk list */
    TSquawk m_Squawk;
    /*! Wind */
    TWindList m_Wind;
    /*! Notes */
    TNotesMap m_Notes;
    /*! User pics */
    TUserPicsList m_UserPics;
    TSvgItemList m_SvgItems;
    bool    m_bIsMainFIR;
};

#endif // SCENE_H
