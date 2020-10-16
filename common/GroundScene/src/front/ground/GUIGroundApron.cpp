#include "GUIGroundApron.h"
#include <QPainter>
#include <QDebug>

GUIGroundApron::GUIGroundApron(QGraphicsItem *parent)
    : QGraphicsItem(parent), mPenWidth(2), mColor(200,200,200)//Pasha mColor(Qt::lightGray)
{
    setZValue(gui::Zapron);
}

GUIGroundApron::~GUIGroundApron()
{
}

QRectF GUIGroundApron::boundingRect() const
{
    return mPainterPath.boundingRect();
}

QPainterPath GUIGroundApron::shape() const
{
    return mPainterPath;
}

void GUIGroundApron::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    //Pasha QPen qPen(Qt::darkBlue);
    QPen qPen(Qt::black);
    qPen.setWidth(mPenWidth);
    painter->setPen(qPen);
    painter->setBrush(QBrush(mColor, Qt::SolidPattern));
    painter->drawPath(mPainterPath);
    painter->restore();
}

void GUIGroundApron::SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord &centralCoord)
{
    prepareGeometryChange();

    mPainterPath = QPainterPath();

    QVector<QPointF> apronPoints;
    foreach(CCoord pnt, vertex())
    {
        QPointF apronPnt((pnt.GetX() - qllXMin) * dScale,
                          (qllYMax - pnt.GetY()) * dScale);
        apronPoints.append( mapFromScene(apronPnt) );
    }

    QPolygonF apronPoly(apronPoints);
    mPainterPath.addPolygon(apronPoly);

    update();
}


