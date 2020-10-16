#ifndef _GUI_CORRIDOR_H__
#define _GUI_CORRIDOR_H__

#include "GUIObject.h"
#include "GUIInterface.h"
#include "orm/Route.h"
#include "cs/CoordSystem.h"


//! Geodesy graphics engine
namespace geoGE {

//! Class for the graphical representation of corridor
class GUICorridor
    : public arinc::Route,
    public GUIObject,
	public IGUIItemHandler
{
public:
	//! Constructor
    GUICorridor();

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

protected:
    //! This event handler receive mouse doubleclick events for this item
	virtual void mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event );

private:
	QPainterPath m_Shape;
    QPainterPath m_ShapeStroker;
};

}   // namespace geoGE

#endif // _GUI_CORRIDOR_H__
