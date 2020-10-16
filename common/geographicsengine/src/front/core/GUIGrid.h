#ifndef _GUI_GRID_H__
#define _GUI_GRID_H__

#include "GUIObject.h"
#include "GUIInterface.h"
#include "cs/CoordSystem.h"

#include <QFont>
class QGraphicsView;


//! Geodesy graphics engine
namespace geoGE {

//! Base class to draw grid relative to the center (anchor) point.
class GUIGrid
    : public GUIUntransformableObject,
    public IGUIItemHandler
{
public:
    //! Enum for simplify coding
    enum GridFlag
    {
        AutoLabelPosition   = 0x01
    };

    //! Typedef for simplify coding
    typedef int     GridFlags;

    //! Constructor
    GUIGrid(QGraphicsItem * parent = 0);

    //@{
    //! Interface IGUIItemHandler
    virtual void SetSI(qint16 SI) {}
    virtual bool IsTransformNeeded() const { return true; }
    //@}

    //! Update position of text labels.
    virtual void UpdateLabelPosition() {}

    //! Set font for drawing labels.
    void DefineFont();

    //! Set fullscreen mode.
    virtual void SetFullscreen(bool isFullscreen)
    { m_isFullscreen = isFullscreen; }

    //! Set flag to update label positions.
    void SetGridFlags(GridFlags gridFlags)
    { m_flags = gridFlags; }

    //! Set flag to update label positions.
    GridFlags GetGridFlags() const
    { return m_flags; }

protected:
    //! Retrieve reference to the first view scene belongs.
    QGraphicsView* view() const;

protected:
    QPainterPath	m_painterPath;
    QFont           m_font;
    bool            m_isFullscreen;
    GridFlags       m_flags;
};


//! Base class for drawing an azimuth or cartesian grids.
class GUIGridRange : public GUIGrid
{
public:
    //! Measure of distance (units).
    enum Units
    {
        Kilometers,
        NauticalMiles
    };

    //! Constructor
    GUIGridRange(QGraphicsItem * parent = 0)
        : GUIGrid(parent), m_majorNumber(8), m_distance(50),
        m_units(Kilometers), m_shiftText(0.0)
    {}

    //! Set units for measure the distance.
    void SetUnits(Units units)
    {
        m_units = units;
        m_majorNumber = units == Kilometers ? 8 : 5;
    }

    //! Set fullscreen mode.
    virtual void SetFullscreen(bool isFullscreen);

    //! Set range (distance) between 2 adjacent lines or circles.
    void SetDistance(int distance)
    { m_distance = distance; }

protected:
    //! Updates number of major intervals in the fullscreen mode.
    void updateMajorIntervalsCount(const TCoordSystem &cs);

protected:
    /** Number of major grid lines that are displayed via 50 km/NM */
    int     m_majorNumber;
    /** Distance between 2 adjacent lines or circles (km/MM) */
    int     m_distance;
    /** Measure of distance */
    Units   m_units;
    /** Distance between 2 adjacent text labels */
    qreal   m_shiftText;
};


//! Graphic object to draw azimuth grid.
class GUIGridAzimuth : public GUIGridRange
{
public:
    //! Constructor
    GUIGridAzimuth(QGraphicsItem * parent = 0)
        : GUIGridRange(parent)
    {}

    //@{
    //! Inherited from QGraphicsItem
    virtual QRectF boundingRect() const;
    QPainterPath shape() const;
    virtual void paint(
        QPainter *painter,
        const QStyleOptionGraphicsItem *option,
        QWidget *widget);
    //@}

    //! Interface IGUIItemHandler
    virtual void SetPosition(const TCoordSystem &cs);

    //! Set azimuth between 2 adjacent lines.
    void SetAzimuth(int azimuth)
    { m_azimuth = azimuth; }

private:
    /** Azimuth between 2 adjacent lines */
    int             m_azimuth;
    /** Draw major circles with thickness 3 */
    QPainterPath    m_pathThick3;
    /** Draw major circles with thickness 2 */
    QPainterPath    m_pathThick;
    /** Draw minor circles and azimuth lines */
    QPainterPath    m_pathThin;
    /** Draw dash lines */
    QPainterPath    m_pathDash;
};


//! Graphic object to draw cartesian grid.
class GUIGridCartesian : public GUIGridRange
{
public:
    //! Constructor
    GUIGridCartesian(QGraphicsItem * parent = 0)
        : GUIGridRange(parent)
    {}

    //@{
    //! Inherited from QGraphicsItem
    virtual QRectF boundingRect() const;
    QPainterPath shape() const;
    virtual void paint(
        QPainter *painter,
        const QStyleOptionGraphicsItem *option,
        QWidget *widget);
    //@}

    //! Interface IGUIItemHandler
    virtual void SetPosition(const TCoordSystem &cs);

    //! Update position of text labels.
    virtual void UpdateLabelPosition();

private:
    /** Bounding rectangle of the drawing */
    QRectF          m_boundingRect;
    /** Draw dash lines */
    QPainterPath    m_pathDash;
};


//! Graphic object to draw geodesic grid.
class GUIGridGeodesic : public GUIGrid
{
public:
    //! Constructor
    GUIGridGeodesic(QGraphicsItem * parent = 0)
        : GUIGrid(parent), m_latLineNumber(8), m_lonLineNumber(10)
    {}

    //@{
    //! Inherited from QGraphicsItem
    virtual QRectF boundingRect() const;
    QPainterPath shape() const;
    virtual void paint(
        QPainter *painter,
        const QStyleOptionGraphicsItem *option,
        QWidget *widget);
    //@}

    //! Interface IGUIItemHandler
    virtual void SetPosition(const TCoordSystem &cs);

    //! Set fullscreen mode.
    virtual void SetFullscreen(bool isFullscreen);

    //! Set number of latitude polylines
    void SetLatitudeNumber(int latNumber)
    { m_latLineNumber = latNumber; }

    //! Set number of latitude polylines
    void SetLongitudeNumber(int lonNumber)
    { m_lonLineNumber = lonNumber; }

    //! Update position of text labels.
    virtual void UpdateLabelPosition();

private:
    //! Updates number of major intervals in the fullscreen mode.
    void updateMajorIntervalsCount(const TCoordSystem &cs);

private:
    typedef QVector<QPointF>    VectorPoints;

    /** Bounding rectangle of the drawing */
    QRectF  m_boundingRect;
    /** Set number of latitude polylines */
    int     m_latLineNumber;
    /** Set number of longitude polylines */
    int     m_lonLineNumber;
    /** Array of positions to display latitude labels */
    VectorPoints    m_posLat;
    /** Array of positions to display longitude labels */
    VectorPoints    m_posLon;
    /** Latitude/longitude label box */
    QRect           m_labelBox;

    /** Cache of lat/lon cross lines positions */
    VectorPoints    m_points;
};

}   // namespace geoGE

#endif // _GUI_GRID_H__
