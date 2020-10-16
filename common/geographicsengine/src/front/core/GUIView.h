#ifndef _GUI_VIEW_H__
#define _GUI_VIEW_H__

#include "back/View.h"
#include <QGraphicsView>
#include <QTimer>
#include <QScopedPointer>
#include <QRubberBand>
#include "GUIGrid.h"
#include "front/misc/XScaleRuler.h"


//! Geodesy graphics engine
namespace geoGE {

//! Class graphical implementation of view
class GUIView
	: public QGraphicsView,
    public View
{
	Q_OBJECT

public:
	//! Interface to CGUIScene
	class IParent
	{
	public:
        //! Destructor
        virtual ~IParent() {}
        //! This event handler must be implemented to receive zoom events
		virtual void ZoomEvent (int iCurScale) = 0;
        //! This event handler must be implemented to receive changed coordinates event
		virtual void CoordinateEvent(const CCoord& Coordinate) = 0;
	};

    //! Current mode of graphics view window.
    enum ViewMode
    {
        VM_None,                //!< mode is not set
        VM_MoveCenter,          //!< Move picked point to center
        VM_ZoomRect             //!< Zoom using rectangle
    };

    //! Max available scale
    static const int MAX_SCALE = 80;
	//! Constructor
	/*! \param pParent a pointer to IParent */
    GUIView(IParent* pParent);
	//! Destructor
    virtual ~GUIView();

    //! Set new mode of graphics view.
    virtual void SetViewMode(int mode);
    //! Retrieve current mode of graphics view.
    int GetViewMode() const { return (int)m_viewMode; }

    //! Adds the scene to the list
	virtual void AddScene(const QString& sKey, const TScenePtr& pScene);
    //! Removes the scene from the list
    virtual void RemoveScene(const QString& sKey);
    //! Adds the MeteoVector to the list
    virtual void AddMeteo(const QSharedPointer<MeteoVector>& pMeteoVec);
    //! Adds the MeteoContour to the list
    virtual void AddMeteo(const QSharedPointer<MeteoContour>& pMeteoZone);
    //! Adds the Peleng to the list
    virtual void AddPeleng(const QSharedPointer<Peleng>& pPeleng);
    //! Creates new MeteoVector
    virtual QSharedPointer<MeteoVector> CreateMeteoVec() const;
    //! Creates new MeteoContour
    virtual QSharedPointer<MeteoContour> CreateMeteoZone() const;
    //! Creates new Peleng
    virtual QSharedPointer<Peleng> CreatePeleng() const;

	//! Zooming
	void SetZoom(int iValue);
    //! Reset zooming
    void ResetZoom();

	//! Drawing all graphics objects
    /*!
        bForce Force to draw all the graphics items.
    */
    virtual void Draw(bool bForce = false);

    //! Renders the source rect, which is in view coordinates, from the scene into target,
    //! which is in paint device coordinates, using painter.
    void RenderDocument(QPainter * painter, const QRectF & target = QRectF(),
                        const QRect & source = QRect(),
                        Qt::AspectRatioMode aspectRatioMode = Qt::KeepAspectRatio);

	//! Before draw
    void Prepare(qreal b, qreal l, qreal angle = 0.0);
    //! Update scene rectangle
    void UpdateSceneRect();

	//! Assign layouts
	void SetLayouts();

	//! Calculate geographic coordinate on graphic point
	CCoord GetCoordAtPoint(QPointF pos) const;

    //! Update grid settings.
    void OnUpdateGrid();
    //! Update azimuth grid settings.
    void OnUpdateAzimuthGrid();
    //! Update cartesian grid settings.
    void OnUpdateCartesianGrid();
    //! Update geodesic grid settings.
    void OnUpdateGeodesicGrid();
    //! Update scale ruler.
    void OnUpdateScaleRuler();

    //! Update font for maps
    void OnUpdateFont();

    //! Set color for grid items.
    void SetGridColor(const QColor& color);
    //! Set color for map items.
    void SetMapColor(const QColor& color);
    //! Set color for meteo items.
    void SetMeteoColor(const QColor& color);
    //! Set color for peleng items.
    void SetPelengColor(const QColor& color);

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

    //! Set new center (anchor) point.
    /*!
        \param latitude Latitude in degree
        \param longitude Longitude in degree
    */
    virtual void SetCenterPoint(qreal latitude, qreal longitude, qreal angle);

    //! Clear scene and view variables.
    virtual void Clear();

signals:
    void zoomChanged(const QRect &zoomRect);

protected:
	//@{
	//! Inherited from QGraphicsView
	virtual void wheelEvent (QWheelEvent * event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);
    virtual void scrollContentsBy(int dx, int dy);
    virtual void drawForeground(QPainter *painter, const QRectF &rect);
    //@}

    void setCursor(const QCursor& cursor);
    void unsetCursor();

    virtual void Zoom(const QPoint& MousePos, int iCurScale);

    //! Prepare empty scene.
    void PrepareEmpty(qreal b, qreal l, qreal angle = 0.0);

    //! Retrieve number of meters in centimeter.
    qreal distanceModelInCm() const;
    //! Update item visibility relative to scale.
    /*!
        \param distMInCm Current distance in meters/centimeter
        \param minDist Minimum distance when item is still visible
        \param maxDist Maximum distance when item is still visible
    */
    template <class T>
    void updateItemVisibility(T *guiItem, qreal distMInCm, qreal minDist, qreal maxDist);

private:
    //! Move map to specified position
    void moveToPos(const QPoint &pos);

private slots:
    //! Used to call it for delayed centering the scene.
    void onCenterOn(QPointF CenterPoint);

protected:
	int			m_iScale;
	IParent*	m_pParent;
	bool		m_bPull;
    QPointF		m_PullPoint;

    CXScaleRuler    m_scaleRuler;

    /** Ratio factor (difference between source and target rect while printing) */
    qreal           m_xratio, m_yratio;

    typedef QMap<QString, QSharedPointer<GUIGrid> >     TGridMap;
    /** Map of grids: azimuth, cartesian or geodesic */
    TGridMap        m_grids;

    /** Current mode of graphics view. (ViewMode or ViewModeEx in child classes) */
    int             m_viewMode;

    /** Zoom rectangle */
    QRubberBand     *mZoomRect;
};

}   // namespace geoGE

#endif // _GUI_VIEW_H__
