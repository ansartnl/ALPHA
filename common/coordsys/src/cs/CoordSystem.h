#ifndef COORD_SYSTEM_H
#define COORD_SYSTEM_H

#include "cs/Coordinate.h"
#include "coordsys_global.h"
#include <QString>
#include <QVector3D>
#include <QPointF>
#include <QTransform>
#include <QSharedPointer>


//! Coordinate conversions.
class COORDSYS_EXPORT CCoordSystem
{
public:
    //! Retrieve projection name.
    virtual QString GetProjectionName() = 0;

    //! Conversion from geodesic to 3D model coordinate.
    /*!
        \param pointGeo Geodesic coordinate: x,y - radians, h - height.
        \return X,Y,Z coordinate in 3D space.
    */
    virtual QVector3D toModel(const QVector3D &pointGeo) const = 0;

    //! Conversion from geodesic to 3D model coordinate.
    /*!
        \param pointGeo Geodesic coordinate: x,y - degree, h - height.
        \return X,Y,Z coordinate in 3D space.
    */
    virtual QVector3D toModelDegree(const QVector3D &pointGeoDegree) const = 0;

    //! Conversion from geodesic to 3D model coordinate.
    /*!
        \param coord Coordinate in degree.
        \return Coordinate in 3D space.
    */
    virtual void toModel(CCoord &coord) const = 0;

    //! Conversion from 3D model to geodesic coordinate.
    /*!
        \param pointModel Coordinate in 3D space.
        \return Geodesic coordinate: x,y - radians, h - height.
    */
    virtual QVector3D toGeo(const QVector3D &pointModel) const = 0;

    //! Conversion from 3D model to geodesic coordinate.
    /*!
        \param pointModel Coordinate in 3D space.
        \return Geodesic coordinate: x,y - degree, h - height.
    */
    virtual QVector3D toGeoDegree(const QVector3D &pointModel) const = 0;

    //! Conversion from 3D model to geodesic coordinate.
    /*!
        \param coord Coordinate in 3D space.
        \return Geodesic coordinate in degree.
    */
    virtual void toGeo(CCoord &coord) const = 0;

    //! Conversion from scene to 3D model coordinate.
    virtual QVector3D toModel(const QPointF &pointScene) const = 0;

    //! Projection of 3D model coordinate to scene coordinate.
    virtual QPointF toScene(const QVector3D &pointModel) const = 0;

    //! Projection of 3D model coordinate to scene coordinate.
    virtual QPointF toScene(const CCoord &coord) const = 0;

    //! Projection of 3D model coordinate to scene coordinate
    /*! Conversion to scene coordinates without applying rotation
        to coordinate.
    */
    virtual QPointF toSceneNoRotation(const QVector3D &pointModel) const = 0;

    //! Conversion of scene point to screen coordinates.
    virtual QPointF toScreen(const QPointF &pointScene) const;

    //! Conversion of a point from screen to scene coordinates.
    virtual QPointF fromScreen(const QPointF &pointScreen) const;

    //! Returns true if model is binded to center point.
    /*!
        To make drawing faster some models can be binded to center point.
        Thus if center point is changed model coordinates should be changed as well.
    */
    virtual bool isModelCentered() const = 0;

    //! Retrieve distance between 2 coordinates.
    virtual qreal distance(const CCoord &coord1, const CCoord &coord2) const = 0;

    //! Projection of 3D model coordinate to scene coordinate using manual
    //! transformation to graphics scene coordinates.
    QPointF toSceneWithTransform(const QVector3D &pointModel) const;

    //! Convert global distance to scene distance.
    /*!
        \param distModel Distance in 3D Model meters.
    */
    qreal distanceToScene(qreal distModel) const;

    //! Convert global distance to scene distance using manual
    //! transformation to graphics scene coordinates.
    /*!
        \param distModel Distance in 3D Model meters.
    */
    qreal distanceToSceneWT(qreal distModel) const;

    //! Convert scene distance to global distance.
    /*!
        \param distScene Distance in scene coordinates.
    */
    qreal distanceToModel(qreal distScene) const;

    //! Set scale factor for scene transform.
    /*!
        \param distance Meters in 1 centimeter
        \param pxlInMmWidth Number of pixels in one millimeter for display width
        \param pxlInMmHeight Number of pixels in one millimeter for display height
    */
    void setSceneTransform(qreal distance, qreal pxlInMmWidth, qreal pxlInMmHeight);

    //! Add transformation matrix of graphics scene.
    void setSceneTransform(const QTransform &mtx)
    { m_mtxScene = mtx; }

    //! Retrieve current scene transformation matrix.
    const QTransform& getSceneTransform() const
    { return m_mtxScene; }

    //! Add transformation matrix of viewport.
    void setViewportTransform(const QTransform &mtx);

    //! Define rotation of projection to the specified RLS.
    /*!
        \param angle Angle in degree - clockwise rotation
    */
    void defineProjectionRotation(qreal angle);

    //! Retrieve rotation of projection to the specified RLS.
    qreal getProjectionRotation() const
    { return m_angle; }

    //! Retrieve reference to rotation matrix.
    const QTransform& getRotationTransform() const
    { return m_mtxAngle; }

    //! Find difference angle to north direction.
    /*! Find difference between direction to north in squared projection
        and direction to north in current projection

        \param latitude Origin latitude to calculate difference angle
        \param longitude Origin longitude to calculate difference angle
        \return Difference angle in degree
    */
    virtual qreal findAngleToNorth(qreal latitude, qreal longitude) const
    { return 0.0; }

    //! Retrieve latitude of center point in degrees.
    qreal getCenterLatitude() const
    { return m_CenterBL.x(); }
    //! Retrieve longitude of center point in degrees.
    qreal getCenterLongitude() const
    { return m_CenterBL.y(); }

    //! Retrieve number of pixels in one millimeter along X axis.
    qreal getPixelInMmX() const
    { return m_pxlInMmWidth; }
    //! Retrieve number of pixels in one millimeter along Y axis.
    qreal getPixelInMmY() const
    { return m_pxlInMmHeight; }

protected:
    CCoordSystem()
        : m_ScaleFactorW(1.0), m_ScaleFactorH(1.0),
        m_pxlInMmWidth(4.0), m_pxlInMmHeight(4.0), m_CenterBL(0.0, 0.0),
        m_angle(0.0)
    {}

protected:
    /** Graphics scene transformation matrix. */
    QTransform  m_mtxScene;
    /** Viewport transformation matrix. */
    QTransform  m_mtxViewport;
    /** Viewport inverted transformation matrix. */
    QTransform  m_mtxViewportInverted;
    /** Scale factor for scene transformations (for width (X)) */
    qreal       m_ScaleFactorW;
    /** Scale factor for scene transformations (for height (Y)) */
    qreal       m_ScaleFactorH;
    /** Number of pixels in one millimeter - (screen device) */
    qreal       m_pxlInMmWidth;
    /** Number of pixels in one millimeter - (screen device) */
    qreal       m_pxlInMmHeight;
    /** Center point in BL coordinates (x - latitude / y - longitude) (degrees) */
    QPointF     m_CenterBL;

    /** Rotation of projection to the specified angle */
    qreal       m_angle;
    /** Matrix. Rotation  of projection */
    QTransform  m_mtxAngle;
};

typedef QSharedPointer<CCoordSystem>    TCoordSystem;

#endif // COORD_SYSTEM_H
