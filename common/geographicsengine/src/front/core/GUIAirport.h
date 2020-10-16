#ifndef _GUI_AIRPORT_H__
#define _GUI_AIRPORT_H__

#include "GUIObject.h"
#include "GUIInterface.h"
#include "orm/Airport.h"
#include "cs/CoordSystem.h"


//! Geodesy graphics engine
namespace geoGE {

class GUIRunway;
//! Class for the graphical representation of airport
class GUIAirport
    : public arinc::Airport,
    public GUIUntransformableObject,
	public IGUIItemHandler
{
public:
    typedef QSharedPointer<GUIRunway>   TGUIRunway;
    typedef QList<TGUIRunway>           TGUIRunways;

    //! Constructor
    GUIAirport();

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

	//! Inherited from ItemHandler
	virtual void MarkItemActive(bool bActive);

    //! Create runway graphics object.
    TGUIRunway CreateRunway(const QString &name);

    //! Retrieve list of runway graphic items.
    const TGUIRunways& GetRunwayItems() const
    { return mRunways; }
    //! Retrieve list of runway graphic items.
    TGUIRunways& GetRunwayItems()
    { return mRunways; }

private:
	QPointF m_CenterPoint;
	QRectF m_Zone;
	int m_iPenWidth;
    QPainterPath m_path;

    /** Runway list of graphic items */
    TGUIRunways     mRunways;
};


//! Class for the graphical representation of airport runway
class GUIRunway
    : public GUIUntransformableObject,
    public IGUIItemHandler,
    public ItemHandler
{
public:
    typedef QSharedPointer<arinc::Runway>    TRunway;

    //! Constructor
    GUIRunway(const TRunway &runway);

    //@{
    //! Inherited from QGraphicsItem
    virtual QRectF boundingRect() const;
    QPainterPath shape() const;
    virtual void paint(
        QPainter *painter,
        const QStyleOptionGraphicsItem *option,
        QWidget *widget);
    //@}

    //@{
    //! Interface IGUIItemHandler
    virtual void SetPosition(const TCoordSystem &cs);
    virtual bool IsTransformNeeded() const { return true; }
    //@}

private:
    /** Runway reference */
    TRunway         mRunway;
    QRectF          mBoundingRect;
    QPainterPath    mPath;
    QPointF         mTextPos;
};

}   // namespace geoGE

#endif // _GUI_AIRPORT_H__
