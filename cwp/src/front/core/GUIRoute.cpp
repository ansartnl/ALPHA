#include "GUIRoute.h"
#include <QtGui/QPen>
#include <QtGui/QPainter>
#include <QGraphicsScene>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsView>
#include <QtGui/QBitmap>
#include <QGraphicsSceneMouseEvent>

#if defined(_MSC_VER) && QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include "back/utils/ProfileSettings.h"
#include "main.h"


CGUIRoute::CGUIRoute()
	: m_iPenWidth(1)
{
    setZValue(gui::Zroute);
    SetColor( UserProfile::CSettings::Instance().GetColor(UserProfile::CtMonitoringRoute) );
    SetCorridorColor( UserProfile::CSettings::Instance().GetColor(UserProfile::CtCorridor) );
    m_bRouteDashDotLine = globalSettings()->value("XMasterWindow/Settings/RouteDashDotLine", 0).toInt() != 0;
}

QRectF CGUIRoute::boundingRect() const
{
	return m_Shape.boundingRect();
}

QPainterPath CGUIRoute::shape() const
{
	return m_Shape;
}

void CGUIRoute::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if (IsItemActive())
	{
        const bool show_routes = networkSettings()->value("users/show_routes", "no").toString() == "yes";
        if (!show_routes)
            return;

        painter->save();
        const bool show_corridors = networkSettings()->value("users/corridor_route", "no").toString() == "yes";
        if (show_corridors) {
            QColor color(m_CorridorColor);
            painter->setPen(Qt::NoPen);
            painter->setBrush(QBrush(color));
            painter->drawPath(m_Shape);
        }

        QPen pen(GetColor());
        pen.setWidth(m_iPenWidth);
        if (m_bRouteDashDotLine) {
            QVector<qreal> dashes;
            static const qreal space = 5;
            dashes << 6 << space << 1 << space;
            pen.setDashPattern(dashes);
        }
        painter->setPen(pen);
        painter->drawLines(m_Lines);

        painter->restore();
	}
}

void CGUIRoute::SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord& centralCoord)
{
	if (IsItemActive())
	{
		if (GetRoutPoints().size() >= 2)
		{
			CRoute::TPointMap::const_iterator itScan = GetRoutPoints().begin();

			qreal qrX1 = ((*itScan)->GetPoint()->GetCoord().GetX() - qllXMin) * dScale;
			qreal qrY1 = (qllYMax - (*itScan)->GetPoint()->GetCoord().GetY()) * dScale;

			m_Lines.clear();

			qreal qrX2(0), qrY2(0);
			for(++itScan; itScan != GetRoutPoints().end(); ++itScan)
			{
				qrX2 = ((*itScan)->GetPoint()->GetCoord().GetX() - qllXMin) * dScale;
				qrY2 = (qllYMax - (*itScan)->GetPoint()->GetCoord().GetY()) * dScale;
				if (!(*(itScan-1))->IsEnd())
					m_Lines.push_back(QLineF(qrX1, qrY1, qrX2, qrY2));
				qrX1 = qrX2;
				qrY1 = qrY2;
			}
			m_Shape = QPainterPath();
			m_ShapeLines.clear();
			QVector<QLineF>::iterator itLine = m_Lines.begin();
			for(; itLine != m_Lines.end(); ++itLine)
			{

				qreal qangle = itLine->angle()*M_PI/180;
				qreal dx = 5000*dScale*sin(qangle);
				qreal dy = 5000*dScale*cos(qangle);
				if (itLine == m_Lines.begin())
					m_Shape.moveTo(itLine->x1() + dx, itLine->y1() + dy);
				else
					m_Shape.lineTo(itLine->x1() + dx, itLine->y1() + dy);
				m_ShapeLines.push_back(QLineF(
					itLine->x1() + dx,
					itLine->y1() + dy,
					itLine->x2() + dx,
					itLine->y2() + dy));

				m_Shape.lineTo(itLine->x2() + dx, itLine->y2() + dy);
			}
			if (!m_Lines.empty())
			{

			}
			do
			{
				--itLine;
				qreal qangle = itLine->angle()*M_PI/180;
				qreal dx = 5000*dScale*sin(qangle);
				qreal dy = 5000*dScale*cos(qangle);

				m_ShapeLines.push_back(QLineF(
					itLine->x2() - dx,
					itLine->y2() - dy,
					itLine->x1() - dx,
					itLine->y1() - dy));
				m_Shape.lineTo(itLine->x2() - dx, itLine->y2() - dy);
				m_Shape.lineTo(itLine->x1() - dx, itLine->y1() - dy);
			} while(itLine != m_Lines.begin());
			m_Shape.closeSubpath();

			itLine = m_Lines.begin();
			for(; itLine != m_Lines.end(); ++itLine)
			{
				itLine->setLine(
					itLine->x1() - m_Shape.boundingRect().left(),
					itLine->y1() - m_Shape.boundingRect().top(),
					itLine->x2() - m_Shape.boundingRect().left(),
					itLine->y2() - m_Shape.boundingRect().top());
			}

			setPos(m_Shape.boundingRect().topLeft());
			itLine = m_ShapeLines.begin();
			for(; itLine != m_ShapeLines.end(); ++itLine)
			{
				itLine->setLine(
					itLine->x1() - m_Shape.boundingRect().left(),
					itLine->y1() - m_Shape.boundingRect().top(),
					itLine->x2() - m_Shape.boundingRect().left(),
					itLine->y2() - m_Shape.boundingRect().top());
			}
			m_Shape.translate(-1*m_Shape.boundingRect().topLeft());

		}
	}
}

void CGUIRoute::mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event )
{
	QGraphicsItem::mouseDoubleClickEvent(event);
}

void CGUIRoute::MarkItemActive(bool bActive)
{
    CRoute::MarkItemActive(bActive);
    setVisible(bActive);
}

void CGUIRoute::SetCorridorColor(const QColor& color)
{
    m_CorridorColor = color;
}
