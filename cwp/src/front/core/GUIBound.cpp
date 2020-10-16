#include "GUIBound.h"
#include <QtGui/QPen>
#include <QtGui/QPainter>
#include <QGraphicsScene>

CGUIBound::CGUIBound(const TPoints& refPointList)
		: m_iPenWidth(1), m_refPointList(refPointList)
{
    setZValue(gui::Zbound);
}

QRectF CGUIBound::boundingRect() const
{
	return m_Path.boundingRect();
}

QPainterPath CGUIBound::shape() const
{
	return m_Path;
}

void CGUIBound::paint(
	QPainter *painter,
	const QStyleOptionGraphicsItem *option,
	QWidget *widget)
{
	painter->save();
	QPen qPen(Qt::black);
	qPen.setWidth(m_iPenWidth);
	painter->setPen(qPen);
	painter->setBrush(QBrush(Qt::darkGray));
	painter->drawPath(m_Path);
	/*QPolygonF qPolygon;
	qPolygon.push_back(QPointF(m_CenterPoint.x(), m_CenterPoint.y() - 5));
	qPolygon.push_back(QPointF(m_CenterPoint.x() + 5, m_CenterPoint.y() + 4));
	qPolygon.push_back(QPointF(m_CenterPoint.x() - 5, m_CenterPoint.y() + 4));
	painter->drawPolygon(qPolygon);*/
	painter->restore();
}

void CGUIBound::SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord& centralCoord)
{
	m_Path = QPainterPath();
	TPoints::const_iterator itScan = m_refPointList.begin();
	for(; itScan != m_refPointList.end(); ++itScan)
	{
		QPointF pnt(
			((*itScan)->GetCoord().GetX() - qllXMin) * dScale,
			(qllYMax - (*itScan)->GetCoord().GetY()) * dScale);
		if (itScan == m_refPointList.begin())
			m_Path.moveTo(pnt);
		else
			m_Path.lineTo(pnt);
	}
	m_Path.closeSubpath();
	setPos(m_Path.boundingRect().topLeft());
	m_Path.translate(-m_Path.boundingRect().topLeft());


	/*m_CenterPoint.setX((GetCoord().GetX() - qllXMin) * dScale);
	m_CenterPoint.setY((qllYMax - GetCoord().GetY()) * dScale);
	m_NextPoint.setX((GetNext()->GetCoord().GetX() - qllXMin) * dScale);
	m_NextPoint.setY((qllYMax - GetNext()->GetCoord().GetY()) * dScale);

	QPointF LeftTop;
	m_Zone.setRect(0, 0, 0, 0);;
	if (m_CenterPoint.x() < m_NextPoint.x())
	{
		LeftTop.setX(m_CenterPoint.x());
		m_Zone.setWidth(m_NextPoint.x());
	} else
	{
		LeftTop.setX(m_NextPoint.x());
		m_Zone.setWidth(m_CenterPoint.x());
	}
	if (m_CenterPoint.y() <  m_NextPoint.y())
	{
		LeftTop.setY(m_CenterPoint.y());
		m_Zone.setHeight(m_NextPoint.y());
	} else
	{
		LeftTop.setY(m_NextPoint.y());
		m_Zone.setHeight(m_CenterPoint.y());
	}
	LeftTop -= (m_CenterPoint - LeftTop);
	setPos(LeftTop);
	m_CenterPoint -= LeftTop;
	m_NextPoint -= LeftTop;
	m_Zone.setBottomRight(m_Zone.bottomRight() - LeftTop);*/



	/*QPointF LeftTop;
	if (m_CenterPoint.x() - 4 - m_iPenWidth < m_NextPoint.x())
		LeftTop.setX(m_CenterPoint.x() - 4 - m_iPenWidth);
	else
		LeftTop.setX(m_NextPoint.x());
	if (m_CenterPoint.y() - 5 - m_iPenWidth < m_NextPoint.y())
		LeftTop.setY(m_CenterPoint.y() - 5 - m_iPenWidth);
	else
		LeftTop.setY(m_NextPoint.y());
	setPos(LeftTop);
	m_CenterPoint.rx() -= LeftTop.x();
	m_CenterPoint.ry() -= LeftTop.y();
	m_NextPoint.rx() -= LeftTop.x();
	m_NextPoint.ry() -= LeftTop.y();

	m_Zone.setTop(0);
	m_Zone.setLeft(0);
	m_Zone.setRight(abs(m_NextPoint.x() - m_CenterPoint.x()) + 4 + m_iPenWidth);
	m_Zone.setBottom(abs(m_NextPoint.y() - m_CenterPoint.y()) + 5 + m_iPenWidth);
	if (m_Zone.right() < 10 + 2*m_iPenWidth)
		m_Zone.setRight(10 + 2*m_iPenWidth);
	if (m_Zone.bottom() < 9 + 2*m_iPenWidth)
		m_Zone.setBottom(9 + 2*m_iPenWidth);*/

}
