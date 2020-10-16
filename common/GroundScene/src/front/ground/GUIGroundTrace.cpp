#include "GUIGroundTrace.h"
#include <QPainter>

GUIGroundTrace::GUIGroundTrace(QGraphicsItem *parent)
    : QGraphicsItem(parent), mPenWidth(2), mColor(216,216,216)//Pasha mColor(Qt::black)
{
    setZValue(gui::Ztrace);
}

GUIGroundTrace::~GUIGroundTrace()
{
}

QRectF GUIGroundTrace::boundingRect() const
{
    return mPainterPath.boundingRect();
}

QPainterPath GUIGroundTrace::shape() const
{
    return mPainterPath;
}

void GUIGroundTrace::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing);

    QPen qPen(mColor);
    qPen.setWidth(mPenWidth);

    if( GroundTrace::type() == Runway)
        qPen.setStyle(Qt::DashLine);
    else
        qPen.setStyle(Qt::DotLine);

    painter->setPen(qPen);
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(mPainterPath);

    painter->restore();
}

void GUIGroundTrace::SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord &centralCoord)
{
    prepareGeometryChange();

    mPainterPath = QPainterPath();

    QPointF startPnt((startPointCoord().GetX() - qllXMin) * dScale,
                      (qllYMax - startPointCoord().GetY()) * dScale);
    QPointF endPnt((endPointCoord().GetX() - qllXMin) * dScale,
                      (qllYMax - endPointCoord().GetY()) * dScale);

    startPnt = mapFromScene(startPnt);
    endPnt = mapFromScene(endPnt);

    QLineF tempLine(startPnt, endPnt);
    mPainterPath.moveTo(tempLine.p2());
    mPainterPath.lineTo(tempLine.p1());

    update();
}
