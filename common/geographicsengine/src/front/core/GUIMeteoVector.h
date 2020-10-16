#ifndef _GUI_METEO_VECTOR_H__
#define _GUI_METEO_VECTOR_H__

#include "front/core/GUIObject.h"
#include "GUIInterface.h"
#include "back/core/MeteoVector.h"
#include "cs/CoordSystem.h"


//! Geodesy graphics engine
namespace geoGE {

//! Class for displaying meteo vectors
class GUIMeteoVector
    : public MeteoVector,
    public GUIObject,
	public IGUIItemHandler
{
public:
	//! Constructor
    GUIMeteoVector(QGraphicsItem * parent = 0);

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

#endif // _GUI_METEO_VECTOR_H__
