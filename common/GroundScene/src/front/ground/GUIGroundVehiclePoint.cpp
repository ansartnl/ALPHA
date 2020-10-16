#include "GUIGroundVehiclePoint.h"
#include <QPainter>
#include "GUIGroundVehicle.h"

GUIGroundVehiclePoint::GUIGroundVehiclePoint(QGraphicsItem * parent)
    : QGraphicsItem(parent)
    , m_iPenWidth(2), m_color(Qt::black), m_pVehicle(0)
{
    setZValue(50);
}

QRectF GUIGroundVehiclePoint::boundingRect() const
{
    QRectF rect(m_painterPath.boundingRect());
    rect.adjust(-1,-1,1,1);
    return rect;
}

QPainterPath GUIGroundVehiclePoint::shape() const
{
    QTransform transform;
    transform.scale(2, 2);
    return transform.map(m_painterPath);
}

void GUIGroundVehiclePoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(m_pVehicle )
    {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        QColor color(m_color);

        QPen qPen(color);
        qPen.setWidth(m_iPenWidth);
        qPen.setCapStyle(Qt::RoundCap);
        painter->setPen(qPen);
        painter->setBrush(color);
        painter->drawPath(m_painterPath);
        painter->restore();
    }
}

void GUIGroundVehiclePoint::SetPosition(QPointF point, CCoord coord)
{
    m_CenterPoint = point;

    int iLen = 0;
    if(m_pVehicle)
    {
        iLen = 2;
        m_Coord.SetX(coord.GetX() + iLen*m_pVehicle->GetVelocity().GetVx());
        m_Coord.SetY(coord.GetY() + iLen*m_pVehicle->GetVelocity().GetVy());
    }
}

void GUIGroundVehiclePoint::UpdatePosition(qint64 qllXMin, qint64 qllYMax, double dScale)
{
    prepareGeometryChange();
    m_painterPath = QPainterPath();
    int iRadius = 10;

    //m_painterPath.addEllipse(mapFromParent(m_CenterPoint), iRadius, iRadius);
    m_painterPath.addRect(mapFromParent(m_CenterPoint).x() - iRadius/2,
                          mapFromParent(m_CenterPoint).y() - iRadius/2,
                          iRadius, iRadius);

    QPointF tempPoint;
    tempPoint.setX((m_Coord.GetX()-qllXMin)*dScale);
    tempPoint.setY((qllYMax-m_Coord.GetY())*dScale);
    QLineF line(mapFromParent(m_CenterPoint), mapFromScene(tempPoint));

    m_painterPath.moveTo(line.p1());
    m_painterPath.lineTo(line.p2());
    update();
}

void GUIGroundVehiclePoint::SetVehicle(GUIGroundVehicle *pVehicle)
{
    m_pVehicle = pVehicle;
}

void GUIGroundVehiclePoint::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseDoubleClickEvent(event);
}
