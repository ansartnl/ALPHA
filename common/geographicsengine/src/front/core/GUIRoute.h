#ifndef _GUI_ROUTE_H__
#define _GUI_ROUTE_H__

#include "GUIObject.h"
#include "orm/Route.h"
#include "GUIInterface.h"
#include "cs/CoordSystem.h"

//! Geodesy graphics engine
namespace geoGE {

//! Class for the graphical representation of route
class GUIRoute
    : public arinc::Route,
    public GUIObject,
	public IGUIItemHandler
{
public:
	//! Constructor
    GUIRoute();

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

private:
    QPainterPath m_Shape, m_ShapeStroker;
};

}   // namespace geoGE

#endif // _GUI_ROUTE_H__
