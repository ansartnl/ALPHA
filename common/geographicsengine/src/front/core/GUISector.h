#ifndef _GUI_SECTOR_H__
#define _GUI_SECTOR_H__

#include "GUIObject.h"
#include "orm/Scene.h"
#include "GUIInterface.h"
#include "cs/CoordSystem.h"

//! Geodesy graphics engine
namespace geoGE {

//! Graphic zone extension
class GUISector
    : public arinc::Scene,
    public GUIObject,
	public IGUIItemHandler
{
public:
    GUISector();
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

private:
    QRectF          m_boundingRect;
    QPainterPath    m_Path;
    int             m_iPenWidth;
};

}   // namespace geoGE

#endif // _GUI_SECTOR_H__
