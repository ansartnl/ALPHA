#ifndef _GUI_SCENE_H__
#define _GUI_SCENE_H__

#include "GUIObject.h"
#include "GUIInterface.h"
#include "orm/Scene.h"
#include <QGraphicsItemGroup>
#include "cs/CoordSystem.h"

//! Geodesy graphics engine
namespace geoGE {

class GUISceneBorder;

//! Class for the graphical representation of FIR
class GUIScene
    : public arinc::Scene,
    public GUIObject,
	public IGUIItemHandler

{
    friend class GUISceneBorder;

public:
    //! Typedef to simplify coding
	typedef QList<QGraphicsItem*> TListItem;
	//! Constructor
    GUIScene();

    //! Adds point to the list
    virtual void AddPoint(const QSharedPointer<arinc::Point>& pPoint);
    //! Adds airport to the list
    virtual void AddAirport(const QSharedPointer<arinc::Airport>& pAirport);
    //! Adds route to the list
    virtual void AddRoute(const QSharedPointer<arinc::Route>& pRoute);
    //! Adds corridor to the list
    virtual void AddCorridor(const QSharedPointer<arinc::Route>& pRoute);
    //! Adds restriction to the list
    virtual void AddRestriction(const QSharedPointer<arinc::Restriction>& pRestriction);
    //! Adds holding to the list
    virtual void AddHolding(const QSharedPointer<arinc::Holding>& pHolding);
    //! Adds sector to the list
    virtual void AddSector(const QSharedPointer<arinc::Scene>& pSector);

    //! Creates new point
    virtual QSharedPointer<arinc::Point> CreatePoint(arinc::Point::Type type) const;
    //! Creates new airport
    virtual QSharedPointer<arinc::Airport> CreateAirport() const;
    //! Creates new route
    virtual QSharedPointer<arinc::Route> CreateRoute() const;
    //! Creates new corridor
    virtual QSharedPointer<arinc::Route> CreateCorridor() const;
    //! Creates new restriction
    virtual QSharedPointer<arinc::Restriction> CreateRestriction() const;
    //! Creates new holding
    virtual QSharedPointer<arinc::Holding> CreateHolding() const;
    //! Creates new SSA
    virtual QSharedPointer<arinc::SSA> CreateSSA() const;
    //! Creates new sector
    virtual QSharedPointer<arinc::Scene> CreateSector() const;

	void Prepare(qint64& iXMin, qint64& iYMin, qint64& iXMax, qint64& iYMax);
    //! Retrieve center point of the scene.
    /*!
        \param b Latitude in degree
        \param l Longitude in degree
    */
    void Prepare(qreal& b, qreal& l) const;

	//@{
    //! This function defines the outer bounds of the item as a rectangle
	virtual QRectF boundingRect() const;
    //! Returns the shape of this item as a QPainterPath in local coordinates
	QPainterPath shape() const;
    //! This function paints the contents of an item in local coordinates
	virtual void paint(
		QPainter *painter,
		const QStyleOptionGraphicsItem *option,
		QWidget *widget);
	//@}

	//@{
	//! Interface IGUIItemHandler
    virtual void SetPosition(const TCoordSystem &cs);
    virtual void SetSI(qint16 SI) {}
	//@}

	//! Get all items
    TListItem GetItemList() const;

	//! Assign layouts
    virtual void SetLayouts() {}

    //! Sets specified color to corridor
    void SetCorridorColor(const QColor& color);
    //! Sets specified color to map
    void SetMapColor(const QColor& color);
    //! Sets specified color to border
    void SetBorderColor(const QColor& color);
    //! Sets specified color to sector
    void SetSectorColor(const QColor& color);
    //! Sets specified color to approach
    void SetApproachColor(const QColor& color);

private:
    //! Cached center point.
    struct CenterPoint
    {
        //! Constructor
        CenterPoint()
            : m_b(2000000.0), m_l(2000000.0)
        {}

        /** Latitude */
        qreal   m_b;
        /** Longitude */
        qreal   m_l;

        //! Returns true if point was defined.
        bool isNull() const
        { return m_b > 10000.0; }
    };

private:
	int			m_iScale;
	qint64		m_qXMin;
	qint64		m_qYMin;
	qint64		m_qXSize;
	qint64		m_qYSize;

    typedef QScopedPointer<GUISceneBorder>     TGUISceneBorder;
    TGUISceneBorder m_Border;

    QRectF      m_boundingRect;
	QPainterPath m_Path;
	TListItem	m_ItemList;

    /** Center point of this FIR */
    mutable CenterPoint m_centerPoint;
};


//! Class for the graphical representation of FIR border
/*!
    FIR should have color + different color for its border.
    Sector is smaller unit than a FIR and sector is displayed above
    FIR -> thus using QGraphicsScene concept new item should be created
    to display FIR's border above border of sectors.
*/
class GUISceneBorder : public QGraphicsItem
{
public:
    //! Constructor
    /*! \param scene a pointer to GUIScene */
    GUISceneBorder(GUIScene *scene = 0);

    //! Set new color for the border.
    void SetColor(const QColor &color)
    { mColor = color; }

    //@{
    //! Inherited from QGraphicsItem
    virtual QRectF boundingRect() const;
    QPainterPath shape() const;
    virtual void paint(
        QPainter *painter,
        const QStyleOptionGraphicsItem *option,
        QWidget *widget);
    //@}

private:
    /** Reference to the scene object */
    GUIScene   *mScene;
    /** Border color */
    QColor      mColor;
};

}   // namespace geoGE

#endif // _GUI_SCENE_H__
