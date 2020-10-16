#ifndef _GUI_PELENG_H__
#define _GUI_PELENG_H__

#include "GUIObject.h"
#include "back/core/Peleng.h"
#include "GUIInterface.h"
#include "cs/CoordSystem.h"
#include <QtGui/QGraphicsTextItem>
#include <QtGui/QGraphicsEllipseItem>


//! Geodesy graphics engine
namespace geoGE {

//! Class for the graphical representation of peleng object
class GUIPeleng
    : public Peleng,
    public GUIObject,
	public IGUIItemHandler
{
public:
	//! Constructor
    GUIPeleng();

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

    //! Set new item's color.
    virtual void SetColor(const QColor& color);

    //! Update font for child graphics items.
    void UpdateFont();

private:
    QRectF              mBoundRect;
    QPainterPath        mShape;

    /** Untransformable channel number at origin of peleng location */
    QGraphicsTextItem       mChannelNumber;
    /** Untransformable icon - ellipse */
    QGraphicsEllipseItem    mIcon;
};

}   // namespace geoGE

#endif // _GUI_PELENG_H__
