#ifndef _GUI_METEO_CONTOUR_H__
#define _GUI_METEO_CONTOUR_H__

#include "GUIObject.h"
#include "GUIInterface.h"
#include "back/core/MeteoContour.h"
#include "cs/CoordSystem.h"


//! Geodesy graphics engine
namespace geoGE {

//! Class for displaying meteo zones
class GUIMeteoContour
    : public MeteoContour,
    public GUIObject,
	public IGUIItemHandler
{
public:
	//! Constructor
    GUIMeteoContour(QGraphicsItem * parent = 0);
	//! Destructor

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
    QPainterPath    mPainterPath;
    QPainterPath    mShapeStroker;
};

}   // namespace geoGE

#endif // _GUI_METEO_CONTOUR_H__
