#ifndef GUIROUTE_H
#define GUIROUTE_H

#include "GUIObject.h"
#include "Route.h"
#include "GUIInterface.h"

//! Class for the graphical representation of route
class CGUIRoute
	: public CRoute,
    public CGUIObject,
	public IGUIItemHandler
{
public:
	//! Constructor
	CGUIRoute();

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

    void SetCorridorColor(const QColor& color);

protected:
	//! Inherited from QGraphicsItem
	virtual void mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event );

private:

	QVector<QLineF> m_Lines;
	QVector<QLineF> m_ShapeLines;
	QRectF m_Zone;
	QPainterPath m_Shape;
	int m_iPenWidth;
    QColor  m_CorridorColor;
    bool    m_bRouteDashDotLine;
};

#endif // GUIROUTE_H
