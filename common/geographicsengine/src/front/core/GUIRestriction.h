#ifndef _GUI_RESTRICTION_H__
#define _GUI_RESTRICTION_H__

#include "GUIObject.h"
#include "GUIInterface.h"
#include "orm/Restriction.h"
#include "cs/CoordSystem.h"


//! Geodesy graphics engine
namespace geoGE {

//! Class for the graphical representation of restriction zone
class GUIRestriction
    : public arinc::Restriction,
    public GUIUntransformableObject,
	public IGUIItemHandler
{
public:
	//! Constructor
    GUIRestriction(QGraphicsItem * parent = 0);
	//! Destructor
    virtual ~GUIRestriction();

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
    virtual bool IsTransformNeeded() const { return true; }
    //@}

    //! Inherited from ItemHandler
	virtual void MarkItemActive(bool bActive);

private:
	int				m_iPenWidth;
	QPainterPath	m_painterPath;
    QRectF          m_boundingRect;
};

}   // namespace geoGE

#endif // _GUI_RESTRICTION_H__
