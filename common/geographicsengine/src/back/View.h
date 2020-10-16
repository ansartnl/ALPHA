#ifndef _VIEW_H__
#define _VIEW_H__

#include "orm/Scene.h"
#include "db/orm/Area.h"
#include "cs/CoordSystem.h"
#include "back/core/MeteoVector.h"
#include "back/core/MeteoContour.h"
#include "back/core/Peleng.h"
#include "XtObject.h"

class PlotContainer;
class VirtualPlotIdAdapter;

//! Geodesy graphics engine
namespace geoGE {

//! Class includes scene list
class View
{
public:
    //! Map of meteo vectors
    typedef QMap<quint16, QSharedPointer<MeteoVector> > TMeteoVecMap;
    //! Map of meteo contours
    typedef QMap<quint16, QSharedPointer<MeteoContour> > TMeteoZoneMap;
    //! Map of peleng objects
    typedef QMap<quint16, QSharedPointer<Peleng> > TPelengMap;
    //! Pointer to scene
    typedef arinc::Scene Scene;
    //! Typedef for the shared pointer to the Scene
    typedef QSharedPointer<Scene> TScenePtr;
	//! Map of scene pointers. Key - unique scene name
	typedef QMap<QString, TScenePtr> TSceneList;

    //! Constructor
    View();
	//! Destructor
    virtual ~View();

	//! Add scene to list
	/*!
		\param sKey key name
		\param pScene pointer to scene
	*/
	virtual void AddScene(const QString& sKey, const TScenePtr& pScene);
    //! Remove scene from the list
    /*!
        \param sKey key name
    */
    virtual void RemoveScene(const QString& sKey);
    //! Get scene list
	/*!
		\return scene list
	*/
	TSceneList& GetScenes();

    //! Create meteo vector
    virtual QSharedPointer<MeteoVector> CreateMeteoVec() const;
    //! Create meteo zone
    virtual QSharedPointer<MeteoContour> CreateMeteoZone() const;
    //! Create peleng object
    virtual QSharedPointer<Peleng> CreatePeleng() const;

    //! Update meteo information
    /*!
        \param meteoVector List of meteo vectors
    */
    virtual void UpdateMeteo(const MeteoVector& meteoVector);
    //! Update meteo information
    /*!
        \param meteoZone Meteo zone
    */
    virtual void UpdateMeteo(const MeteoContour& meteoZone);
    //! Update peleng information
    /*!
        \param peleng Peleng data object
    */
    virtual void UpdatePeleng(const Peleng& peleng);
    //! Add pointer to meteo vector
    virtual void AddMeteo(const QSharedPointer<MeteoVector>& pMeteoVec);
    //! Add pointer to meteo zone
    virtual void AddMeteo(const QSharedPointer<MeteoContour>& pMeteoZone);
    //! Add pointer to peleng object
    virtual void AddPeleng(const QSharedPointer<Peleng>& pPeleng);

    //! Get meteo vectors list
    TMeteoVecMap& GetMeteoVec() { return mMeteoVectors; }
    //! Get meteo vectors list
    const TMeteoVecMap& GetMeteoVec() const { return mMeteoVectors; }
    //! Get meteo zone list
    TMeteoZoneMap& GetMeteoZones() { return mMeteoZones; }
    //! Get meteo zone list
    const TMeteoZoneMap& GetMeteoZones() const { return mMeteoZones; }

    //! Get peleng object list
    TPelengMap& GetPelengs() { return mPelengs; }
    //! Get peleng object list
    const TPelengMap& GetPelengs() const { return mPelengs; }

    //! Get reference to coordinate system object.
    const TCoordSystem& GetCoordSys() const { return m_pCS; }
    //! Get reference to coordinate system object.
    TCoordSystem& GetCoordSys() { return m_pCS; }

    //! Set new center (anchor) point.
    /*!
        \param latitude Latitude in degree
        \param longitude Longitude in degree
        \param angle Rotation angle in degree
    */
    virtual void SetCenterPoint(qreal latitude, qreal longitude, qreal angle);

    //! Update current projection
    /*!
        \param nameProj Name of projection
        \param a Equatorial radius, m
        \param e Eccentricity
        \param k0 Scale
    */
    virtual void UpdateProjection(const QString &nameProj, qreal a, qreal e, qreal k0);
    //! Define projection and model.
    bool DefineProjection(const QString &nameProj, qreal a, qreal e, qreal k0);

    //! Clear scene and view variables.
    virtual void Clear();

protected:
    //! Define center point for coordinate system only.
    void DefineCenterPoint(qreal latitude, qreal longitude, qreal angle);

    //! Update model coordinates of all objects that needs to be rendered.
    virtual void UpdateModelCoords();

	//! Scene list
	TSceneList					m_SceneList;

private:
    /** Meteo vector list */
    TMeteoVecMap    mMeteoVectors;
    /** Meteo zone list */
    TMeteoZoneMap   mMeteoZones;
    /** Peleng object list */
    TPelengMap      mPelengs;

    /*! Coordinate system */
    TCoordSystem    m_pCS;
};

}   // namespace geoGE

#endif // _VIEW_H__
