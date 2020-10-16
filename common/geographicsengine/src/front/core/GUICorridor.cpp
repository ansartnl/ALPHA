#include "GUICorridor.h"
#include <QPen>
#include <QPainter>
#include <QGraphicsScene>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsView>
#include <QPainterPathStroker>
#include <QBitmap>
#include <QGraphicsSceneMouseEvent>
#include <math.h>
#include "front/common/UserSettings.h"
#include "front/common/ZOrderValue.h"


//! Geodesy graphics engine
namespace geoGE {

GUICorridor::GUICorridor()
{
    setZValue( getZOrderValue(ZOrderCorridor) );
    SetColor( UserSettings::Accessor::instance().color(UserSettings::CtCorridor) );
}

QRectF GUICorridor::boundingRect() const
{
    return m_ShapeStroker.boundingRect();
}

QPainterPath GUICorridor::shape() const
{
    return m_ShapeStroker;
}

void GUICorridor::paint(
	QPainter *painter,
	const QStyleOptionGraphicsItem *option,
	QWidget *widget)
{
    if ( !IsItemActive() )
        return;

    QPen qPen( GetColor() );
    qPen.setWidthF(0.4);
    qPen.setJoinStyle(Qt::RoundJoin);
    qPen.setCapStyle(Qt::RoundCap);
    painter->setPen(qPen);

    painter->drawPath(m_Shape);
}

void GUICorridor::SetPosition(const TCoordSystem &cs)
{
    if ( GetRoutPoints().size() < 3 )
        return;

    prepareGeometryChange();

    m_Shape = QPainterPath();

    arinc::Route::TPointMap::const_iterator itScan = GetRoutPoints().begin();

    const CCoord &coordFirst = (*itScan)->GetPoint()->GetCoord();
    QPointF pnt = cs->toScene(coordFirst);

    m_Shape.moveTo(pnt);

    for ( ; itScan != GetRoutPoints().end(); ++itScan )
    {
        const CCoord &coord = (*itScan)->GetPoint()->GetCoord();
        pnt = cs->toScene(coord);

        m_Shape.lineTo(pnt);
    }

    QPainterPathStroker pathStroker;
    pathStroker.setWidth(2);
    pathStroker.setJoinStyle(Qt::RoundJoin);
    pathStroker.setCapStyle(Qt::RoundCap);
    m_ShapeStroker = pathStroker.createStroke(m_Shape);

    update();
}

void GUICorridor::mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event )
{
	//QMessageBox::information(NULL, QObject::tr("Route"), GetName());
	QGraphicsItem::mouseDoubleClickEvent(event);
}

void GUICorridor::MarkItemActive(bool bActive)
{
    arinc::Route::MarkItemActive(bActive);
	setVisible(bActive);
}

}   // namespace geoGE
