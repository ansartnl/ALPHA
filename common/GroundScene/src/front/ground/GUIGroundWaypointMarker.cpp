#include "GUIGroundWaypointMarker.h"
#include <QPainter>
#include <math.h>
#include <QGraphicsSceneMouseEvent>

GUIGroundWaypointMarker::GUIGroundWaypointMarker(QObject *parent) :
    QObject(parent), mPenWidth(1), mColor(Qt::yellow)
{
    setZValue(gui::ZwaypointMarker);
    setAcceptHoverEvents(true);
    setFlag(ItemIsMovable, true);
    setAcceptedMouseButtons(Qt::LeftButton);
}

GUIGroundWaypointMarker::~GUIGroundWaypointMarker()
{
}

QRectF GUIGroundWaypointMarker::boundingRect() const
{
    return mPainterPath.boundingRect();
}

QPainterPath GUIGroundWaypointMarker::shape() const
{
    return mPainterPath;
}

void GUIGroundWaypointMarker::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    QPen qPen(mColor);
    qPen.setWidth(mPenWidth);
    qPen.setCapStyle(Qt::RoundCap);
    painter->setPen(qPen);
    painter->setBrush(QBrush(mColor, Qt::SolidPattern));
    painter->drawPath(mPainterPath);

    painter->restore();
}

void GUIGroundWaypointMarker::SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord &centralCoord)
{
    prepareGeometryChange();
    mPainterPath = QPainterPath();
    mHoverPath = QPainterPath();

    QPointF point(
        (coord().GetX() - qllXMin) * dScale,
        (qllYMax - coord().GetY()) * dScale);
    point = mapFromScene(point);

    double scaledRadius = 12 *dScale;
    mPainterPath.addEllipse(point, scaledRadius, scaledRadius);

//    double scaledRadius = 10;
//    mPainterPath.moveTo(point.x() - scaledRadius, point.y() - scaledRadius);
//    mPainterPath.lineTo(point.x() + scaledRadius, point.y() + scaledRadius);
//    mPainterPath.moveTo(point.x() + scaledRadius, point.y() - scaledRadius);
//    mPainterPath.lineTo(point.x() - scaledRadius, point.y() + scaledRadius);
//    mHoverPath.addEllipse(point, scaledRadius, scaledRadius);

    update();
}


void GUIGroundWaypointMarker::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
}

void GUIGroundWaypointMarker::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
}

void GUIGroundWaypointMarker::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    mDragStartPos = pos();
    event->accept();
}

void GUIGroundWaypointMarker::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    if(mDragStartPos != pos())
    {
        QPointF mapped = mapToScene(event->pos());
        emit isMoved(SN(), mapped);
    }
}

void GUIGroundWaypointMarker::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    emit isDeleted(SN());
}
