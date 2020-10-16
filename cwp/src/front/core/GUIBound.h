#ifndef GUIBOUND_H
#define GUIBOUND_H

#include <QGraphicsItem>
#include "import/Point.h"
#include "import/BoundaryPoint.h"
#include "GUIInterface.h"

//! Class for the graphical representation of FIR's boundary point
class CGUIBound
	: public QGraphicsItem,
	public IGUIItemHandler
{
public:
	typedef QList<QSharedPointer<CBoundaryPoint> > TPoints;
	//! Constructor
	/*! \param refPointList a list of points */
	CGUIBound(const TPoints& refPointList);

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

private:
	QPainterPath m_Path;
	int m_iPenWidth;
	const TPoints& m_refPointList;
};

#endif // GUIBOUND_H
