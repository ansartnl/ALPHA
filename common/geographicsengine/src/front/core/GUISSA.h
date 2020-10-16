#ifndef _GUI_SSA_H__
#define _GUI_SSA_H__

#include "GUIObject.h"
#include "core/airport/SSA.h"
#include "GUIInterface.h"
#include "cs/CoordSystem.h"

//! Geodesy graphics engine
namespace geoGE {

class GUIScene;
//! Class for the graphical representation of SSA
class GUISSA
    : public SSA,
    public GUIObject,
	public IGUIItemHandler
{
public:
	//! Constructor
    GUISSA(const GUIScene* pFIR);

	//@{
    //! This function defines the outer bounds of the item as a rectangle
	virtual QRectF boundingRect() const;
	//QPainterPath shape() const;
    //! This function paints the contents of an item in local coordinates
	virtual void paint(
		QPainter *painter,
		const QStyleOptionGraphicsItem *option,
		QWidget *widget);
	//@}

	//@{
	//! Interface IGUIItemHandler
    virtual void SetPosition(const TCoordSystem &cs);   // throw (QString)
    virtual void SetSI(qint16 SI) {}
    virtual bool IsTransformNeeded() const;
    //@}

	//! Inherited from ItemHandler
	virtual void MarkItemActive(bool bActive);

protected:
	//void mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event );

private:
    /** Reference to the FIR object */
    const GUIScene*     m_pFIR;

    int                 m_iPenWidth;
};

}   // namespace geoGE

#endif // _GUI_SSA_H__
