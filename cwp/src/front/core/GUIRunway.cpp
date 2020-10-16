#include "GUIRunway.h"
#include <QPainter>
#include <QGraphicsScene>
#include <math.h>
#include "back/common/Calculation.h"
#include "back/utils/ProfileSettings.h"
#include "front/common/XAppSettings.h"

CGUIRunway::CGUIRunway(QGraphicsItem *parent)
    : CGUIObject(parent), m_iPenWidth(1)
{
    setZValue(gui::Zrunway);
	setOpacity(1);
    SetColor( UserProfile::CSettings::Instance().GetColor(UserProfile::CtApproach) );
}

CGUIRunway::~CGUIRunway()
{

}

QRectF CGUIRunway::boundingRect() const
{
    return m_painterPath.boundingRect() | m_textRect;
}

QPainterPath CGUIRunway::shape() const
{
	return m_painterPath;
}

void CGUIRunway::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (IsItemActive())
	{
		painter->save();
        QPen qPen( GetColor() );
		qPen.setWidth(m_iPenWidth);
		painter->setPen(qPen);
        //painter->setBrush(QBrush(GetColor(), Qt::SolidPattern));
        painter->setBrush(Qt::NoBrush);
        painter->drawPath(m_painterPath);
        QFont font = painter->font();
        font.setPointSize(AppSettings::GetTextSize().GetCurrentTextSize());
        painter->setFont(font);
        painter->drawText(m_textRect, Qt::AlignCenter, GetName());
		painter->restore();
	}
}


void CGUIRunway::SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord& centralCoord)
{
	if (IsItemActive())
    {
        prepareGeometryChange();

		m_painterPath = QPainterPath();
        QPointF RunwayPnt((GetCoord().GetX() - qllXMin) * dScale,
                          (qllYMax - GetCoord().GetY()) * dScale);
        RunwayPnt = mapFromScene(RunwayPnt);
        qreal RunwayLength = GetLength() * dScale;
        qreal RunwayWidth = GetWidth() * dScale;
        QRectF RunwayRect(RunwayPnt.x(), RunwayPnt.y() - RunwayWidth/2, RunwayLength, RunwayWidth);
        QPolygonF RunwayPoly(RunwayRect);
        QTransform transform;
        transform.translate(RunwayPnt.x(), RunwayPnt.y());
        transform.rotate(-90);
        transform.rotate(GetAzimuth() + GetDeclination());
        transform.translate(-RunwayPnt.x(), -RunwayPnt.y());
        m_painterPath.addPolygon(transform.map(RunwayPoly));

        QFont font = scene()->font();
        font.setPointSize(AppSettings::GetTextSize().GetCurrentTextSize());
        QFontMetricsF metrics(font);
        QRectF rectText = metrics.boundingRect(GetName());
        rectText.adjust(-m_iPenWidth, -m_iPenWidth, m_iPenWidth, m_iPenWidth);
        m_textRect = QRectF(RunwayPnt.x() - rectText.width()/2, RunwayPnt.y() - rectText.height()/2
                            , rectText.width(), rectText.height());

        update();
	}
}

void CGUIRunway::MarkItemActive(bool bActive)
{
    CRunway::MarkItemActive(bActive);
	setVisible(bActive);
}
