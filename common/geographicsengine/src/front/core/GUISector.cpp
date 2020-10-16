#include "GUISector.h"
#include <QPen>
#include <QPainter>
#include <QGraphicsScene>
#include "front/common/UserSettings.h"
#include "front/common/ZOrderValue.h"

//! Geodesy graphics engine
namespace geoGE {

GUISector::GUISector()
        : m_iPenWidth(0)
{
	MarkItemActive(false);
    setZValue( getZOrderValue(ZOrderSector) );

    SetColor( UserSettings::Accessor::instance().color(UserSettings::CtSector) );
}

QRectF GUISector::boundingRect() const
{
    return m_boundingRect;
}

QPainterPath GUISector::shape() const
{
	return m_Path;
}

void GUISector::paint(
	QPainter *painter,
	const QStyleOptionGraphicsItem *option,
	QWidget *widget)
{
    QPen qPen( GetColor() );
    qPen.setCosmetic(true);
//    qPen.setWidth(m_iPenWidth);
//    qPen.setStyle(Qt::DashLine);
    painter->setPen(qPen);

    painter->drawPath(m_Path);
}

void GUISector::SetPosition(const TCoordSystem &cs)
{
    prepareGeometryChange();

    m_Path = QPainterPath();

//    QString name = GetName();
//    if ( name != QString("LIEPAJA TMA SEKTOR C") )
//        return;

    TBoundary& BoundaryList = GetBoundary();

    TBoundary::const_iterator itScan = GetBoundary().begin();
    for(; itScan != GetBoundary().end(); ++itScan)
    {
        QPointF point = cs->toScene( (*itScan)->GetCoord() );
        QPointF pointArc = cs->toScene( (*itScan)->GetArcCoord() );

        TBoundary::const_iterator itScanNext = itScan+1;
        if(itScanNext == BoundaryList.end()) itScanNext = BoundaryList.begin();

        QPointF pointNext = cs->toScene( (*itScanNext)->GetCoord() );

        qreal qRadius = cs->distanceToScene( (*itScan)->GetArcDistance() );

        if(itScan == BoundaryList.begin()) m_Path.moveTo(point);

        switch((*itScan)->GetType())
        {
        case arinc::RestrictPoint::enLine:
            m_Path.lineTo(pointNext);
            break;
        case arinc::RestrictPoint::enCircle:
            m_Path.addEllipse(pointArc, qRadius, qRadius);
            break;
        case arinc::RestrictPoint::enLeftArc:
        case arinc::RestrictPoint::enRightArc:
            {
                QRectF rect(QPointF(pointArc.x()-qRadius, pointArc.y()-qRadius), QSizeF(2*qRadius, 2*qRadius));
                QLineF line1(pointArc, point);
                QLineF line2(pointArc, pointNext);

                if((*itScan)->GetType() == arinc::RestrictPoint::enLeftArc)
                    m_Path.arcTo(rect, line1.angle(), line1.angleTo(line2));
                else
                    m_Path.arcTo(rect, line1.angle(), line1.angleTo(line2)-360);
            }
            break;
        default:
            qWarning("unknow CRestrictPoint::Type");
        }
    }
    m_Path.closeSubpath();

    m_boundingRect = m_Path.boundingRect();
    m_boundingRect.adjust(-2,-2,2,2);

    update();

/*
    m_Path = QPainterPath();
    TCoordList::const_iterator itScan = GetPolygonCoord().begin();
    for(; itScan != GetPolygonCoord().end(); ++itScan)
    {
        QPointF pnt = cs->toScene(QVector3D(itScan->GetX(), itScan->GetY(), itScan->GetZ()));

        if (itScan == GetPolygonCoord().begin())
            m_Path.moveTo(pnt);
        else
            m_Path.lineTo(pnt);
    }
    m_Path.closeSubpath();
//    setPos(m_Path.boundingRect().topLeft());
//    m_Path.translate(-m_Path.boundingRect().topLeft());
*/
}

}   // namespace geoGE
