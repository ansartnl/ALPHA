#include "GUIRoute.h"
#include <QPen>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsView>
#include <QMessageBox>
#include <QBitmap>
#include <QGraphicsSceneMouseEvent>
#include <math.h>
#include "front/common/UserSettings.h"
#include "front/common/ZOrderValue.h"


//! Geodesy graphics engine
namespace geoGE {

GUIRoute::GUIRoute()
{
    setZValue( getZOrderValue(ZOrderRoute) );
    SetColor( UserSettings::Accessor::instance().color(UserSettings::CtMap) );
}

QRectF GUIRoute::boundingRect() const
{
    return m_Shape.boundingRect();
}

QPainterPath GUIRoute::shape() const
{
    return m_Shape;
}

void GUIRoute::paint(
	QPainter *painter,
	const QStyleOptionGraphicsItem *option,
	QWidget *widget)
{
    if ( !IsItemActive() )
        return;

    QPen qPen( GetColor() );
    qPen.setCosmetic(true);
//    qPen.setStyle(Qt::DashLine);
    painter->setPen(qPen);

    painter->drawPath(m_Shape);
}

void GUIRoute::SetPosition(const TCoordSystem &cs)
{
    if ( GetRoutPoints().size() < 2 )
        return;

    prepareGeometryChange();

    m_Shape = QPainterPath();

    Route::TPointMap::const_iterator itScan = GetRoutPoints().begin();

    const CCoord &coordFirst = (*itScan)->GetPoint()->GetCoord();
    QPointF pnt = cs->toScene(coordFirst);

    m_Shape.moveTo(pnt);

    for ( ; itScan != GetRoutPoints().end(); ++itScan )
    {
        const CCoord &coord = (*itScan)->GetPoint()->GetCoord();
        pnt = cs->toScene(coord);

        m_Shape.lineTo(pnt);
    }

    update();
}

void GUIRoute::MarkItemActive(bool bActive)
{
    Route::MarkItemActive(bActive);
	setVisible(bActive);
}

}   // namespace geoGE
