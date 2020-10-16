#ifndef _GUI_HOLDING_H__
#define _GUI_HOLDING_H__

#include "GUIObject.h"
#include "GUIInterface.h"
#include "orm/Holding.h"
#include "cs/CoordSystem.h"

//! Geodesy graphics engine
namespace geoGE {

class GUIScene;
//! Class for the graphical representation of holding zone
class GUIHolding
    : public arinc::Holding,
    public GUIObject,
	public IGUIItemHandler
{
public:
	//! Constructor
    GUIHolding(const GUIScene* pFIR, QGraphicsItem * parent = 0);
	//! Destructor
    virtual ~GUIHolding();

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
    /** Reference to the FIR object */
    const GUIScene*     m_pFIR;

    int                 m_iPenWidth;
    QPainterPath        m_painterPath;
};

}   // namespace geoGE

#endif // _GUI_HOLDING_H__
