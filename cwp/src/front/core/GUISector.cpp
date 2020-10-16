#include "GUISector.h"
#include <QtGui/QPen>
#include <QtGui/QPainter>
#include <QGraphicsScene>
#include "back/utils/ProfileSettings.h"
#include "main.h"

CGUISector::CGUISector()
		: m_iPenWidth(1), m_bBorderDraw(true)
{
	MarkItemActive(false);
    setZValue(gui::Zsector);

    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();
    penColor = stg.GetColor(UserProfile::CtBorder);
    brushColor = stg.GetColor(UserProfile::CtSector);

    m_Border.reset(new GUISectorBorder(this));
}

QRectF CGUISector::boundingRect() const
{
	return m_Path.boundingRect();
}

QPainterPath CGUISector::shape() const
{
	return m_Path;
}

void CGUISector::paint(
	QPainter *painter,
	const QStyleOptionGraphicsItem *option,
	QWidget *widget)
{
    if (IsItemActive())
	{
		painter->save();
        painter->setPen(Qt::NoPen);
        painter->setBrush(brushColor);
		painter->drawPath(m_Path);
		painter->restore();
	}
}

void CGUISector::SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord& centralCoord)
{
    if (IsItemActive() || m_bBorderDraw)
	{
        prepareGeometryChange();
        m_Path = QPainterPath();
		TCoordList::const_iterator itScan = GetPolygonCoord().begin();
		for(; itScan != GetPolygonCoord().end(); ++itScan)
		{
			QPointF pnt(
				(itScan->GetX() - qllXMin) * dScale,
				(qllYMax - itScan->GetY()) * dScale);
			if (itScan == GetPolygonCoord().begin())
				m_Path.moveTo(pnt);
			else
				m_Path.lineTo(pnt);
		}
		m_Path.closeSubpath();
        QPointF ltPoint = m_Path.boundingRect().topLeft();
        setPos(ltPoint);
        m_Path.translate(-m_Path.boundingRect().topLeft());
        if(m_Border)
            m_Border->setPos(ltPoint);
    }
}

void CGUISector::SetBorderVisible(bool bVisible)
{
	m_bBorderDraw = bVisible;
}

/////////////////////////////////////////////////////////////////////////////////////
// GUISectorBorder implementation.

GUISectorBorder::GUISectorBorder(CGUISector *scene)
    : mScene(scene)
{
    setZValue(gui::Zsectorborder);
}

QRectF GUISectorBorder::boundingRect() const
{
    return mScene->boundingRect();
}

QPainterPath GUISectorBorder::shape() const
{
    return mScene->m_Path;
}

void GUISectorBorder::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (mScene->m_bBorderDraw && (networkSettings()->value("users/sector_border", "yes").toString() == "yes"))
    {
        painter->save();
        QPen qPen(mScene->penColor);
        qPen.setWidth(mScene->m_iPenWidth);
        painter->setPen(qPen);
        painter->setBrush(Qt::NoBrush);
        painter->drawPath(mScene->m_Path);
        painter->restore();
    }
}
