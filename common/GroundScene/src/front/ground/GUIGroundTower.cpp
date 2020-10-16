#include "GUIGroundTower.h"
#include <QPainter>

GUIGroundTower::GUIGroundTower(QGraphicsItem *parent)
: QGraphicsItem(parent), mPenWidth(1), mColor(Qt::yellow)
{
    setZValue(gui::Ztower);
}

GUIGroundTower::~GUIGroundTower()
{
}

QRectF GUIGroundTower::boundingRect() const
{
    return mPainterPath.boundingRect();
}

QPainterPath GUIGroundTower::shape() const
{
    return mPainterPath;
}

void GUIGroundTower::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    QPen qPen(Qt::black);
    qPen.setWidth(mPenWidth);
    painter->setPen(qPen);
    painter->setBrush(QBrush(mColor, Qt::SolidPattern));
    painter->drawPath(mPainterPath);

    painter->restore();
}

void GUIGroundTower::SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord &centralCoord)
{
    prepareGeometryChange();
    mPainterPath = QPainterPath();
    QPointF point(
        (coord().GetX() - qllXMin) * dScale,
        (qllYMax - coord().GetY()) * dScale);
    point = mapFromScene(point);

    mPainterPath.addEllipse(point, 5*dScale, 5*dScale);
    update();
}


