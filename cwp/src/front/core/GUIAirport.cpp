#include "GUIAirport.h"
#include <QtGui/QPen>
#include <QtGui/QPainter>
#include <QGraphicsScene>
#include <math.h>
#include "back/utils/ProfileSettings.h"
#include "front/common/XAppSettings.h"

CGUIAirport::CGUIAirport()
	: m_iPenWidth(1)
{
    setZValue(gui::Zairport);
    SetColor( UserProfile::CSettings::Instance().GetColor(UserProfile::CtMap) );
    setFlag(QGraphicsItem::ItemIgnoresTransformations);
}

QRectF CGUIAirport::boundingRect() const
{
	return m_Zone;
}

QPainterPath CGUIAirport::shape() const
{
	QPainterPath qPath;
	qPath.addRect(m_Zone);
	return qPath;
}

void CGUIAirport::paint(
	QPainter *painter,
	const QStyleOptionGraphicsItem *option,
	QWidget *widget)
{
	if (IsItemActive())
	{
		painter->save();

        QPen qPen( GetColor() );
		qPen.setWidth(m_iPenWidth);
		painter->setPen(qPen);
		painter->drawRect(m_Zone);

		QRectF TextRect(m_Zone);
		TextRect.adjust(4, 4, -4, -4);
        QFont font = painter->font();
        font.setPointSize(AppSettings::GetTextSize().GetCurrentTextSize());
        painter->setFont(font);
        painter->drawText(TextRect, GetName(), QTextOption(Qt::AlignCenter));
		painter->restore();
	}
}

void CGUIAirport::SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord& centralCoord)
{
	if (IsItemActive())
	{
		m_CenterPoint.setX((GetCoord().GetX() - qllXMin) * dScale);
		m_CenterPoint.setY((qllYMax - GetCoord().GetY()) * dScale);

        QFont font = scene()->font();
        font.setPointSize(AppSettings::GetTextSize().GetCurrentTextSize());
        QFontMetricsF metrics(font);
        m_Zone = (metrics.boundingRect(GetName()));
		m_Zone.adjust(-5, -5, 5, 5);
		m_Zone.moveTo(0, 0);

		setPos(m_CenterPoint - m_Zone.center());
	}
}

void CGUIAirport::MarkItemActive(bool bActive)
{
	CAirport::MarkItemActive(bActive);
	setVisible(bActive);
}
