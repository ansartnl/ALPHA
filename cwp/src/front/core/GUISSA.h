#ifndef GUISSA_H
#define GUISSA_H

#include "GUIObject.h"
#include "SSA.h"
#include "GUIInterface.h"

//! Class for the graphical representation of SSA
class CGUISSA
	: public CSSA,
    public CGUIObject,
	public IGUIItemHandler
{
public:
	//! Constructor
	CGUISSA();

	//@{
	//! Inherited from QGraphicsItem
	virtual QRectF boundingRect() const;
	//QPainterPath shape() const;
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
};

#endif // GUISSA_H
