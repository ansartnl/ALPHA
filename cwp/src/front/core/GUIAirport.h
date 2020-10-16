#ifndef GUIAIRPORT_H
#define GUIAIRPORT_H
#include "GUIObject.h"
#include "Airport.h"
#include "GUIInterface.h"

//! Class for the graphical representation of airport
class CGUIAirport
	: public CAirport,
	public CGUIObject,
	public IGUIItemHandler
{
public:
	//! Constructor
	CGUIAirport();

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
	QPointF m_CenterPoint;
	QRectF m_Zone;
	int m_iPenWidth;
};

#endif // GUIAIRPORT_H
