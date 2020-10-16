#include "GUIGroundStart.h"
#include <QPainter>

GUIGroundStart::GUIGroundStart(QGraphicsItem *parent)
    : QGraphicsItem(parent), mPenWidth(1), mColor(Qt::red)
{
    setZValue(gui::Zstart);
}

GUIGroundStart::~GUIGroundStart()
{
}

QRectF GUIGroundStart::boundingRect() const
{
    return mPainterPath.boundingRect();
}

QPainterPath GUIGroundStart::shape() const
{
    return mPainterPath;
}

void GUIGroundStart::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    QPen qPen(mColor);
    qPen.setWidth(mPenWidth);
    painter->setPen(qPen);
    //painter->setBrush(QBrush(GetColor(), Qt::SolidPattern));
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(mPainterPath);
    painter->restore();
}

void GUIGroundStart::SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord &centralCoord)
{
    prepareGeometryChange();

    mPainterPath = QPainterPath();
    QPointF point((coord().GetX() - qllXMin) * dScale,
                      (qllYMax - coord().GetY()) * dScale);
    point = mapFromScene(point);

    QLineF tempLine(point, QPointF(point.x()+ 10*dScale, point.y()));

    qreal angle = 360-azimuth()+90;    
    while(angle >= 360) angle -= 360;

    tempLine.setAngle(angle);

    double r1 = 10 * dScale;
    double r2 = 3 * dScale;
    mPainterPath.addEllipse(tempLine.p1(), r1, r1);
    mPainterPath.addEllipse(tempLine.p2(), r2, r2);

    update();
}


