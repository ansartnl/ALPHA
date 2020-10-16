#ifndef GUIHOLDING_H
#define GUIHOLDING_H

#include "GUIObject.h"
#include "GUIInterface.h"
#include "Holding.h"

//! Class for the graphical representation of holding zone
class CGUIHolding
	: public CHolding,
    public CGUIObject,
	public IGUIItemHandler
{
public:
	//! Constructor
	CGUIHolding(QGraphicsItem * parent = 0);
	//! Destructor
	virtual ~CGUIHolding();

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
	virtual void SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord& centralCoord);
	virtual void SetSI(qint16 SI) {}
	//@}

	//! Inherited from ItemHandler
	virtual void MarkItemActive(bool bActive);

private:
	int				m_iPenWidth;
	QPainterPath	m_painterPath;
};

#endif // GUIHOLDING_H
