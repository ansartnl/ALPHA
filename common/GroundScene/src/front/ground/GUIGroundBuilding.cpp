#include "GUIGroundBuilding.h"
#include <QPainter>

GUIGroundBuilding::GUIGroundBuilding(QGraphicsItem *parent)
: QGraphicsItem(parent), mPenWidth(2), mColor(Qt::blue)
{
    setZValue(gui::Zbuilding);
}

GUIGroundBuilding::~GUIGroundBuilding()
{
}

QRectF GUIGroundBuilding::boundingRect() const
{
    return mPainterPath.boundingRect();
}

QPainterPath GUIGroundBuilding::shape() const
{
    return mPainterPath;
}

void GUIGroundBuilding::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    QPen qPen(Qt::black);
    qPen.setWidth(mPenWidth);
    painter->setPen(qPen);
    painter->setBrush(QBrush(mColor, Qt::SolidPattern));
    painter->drawPath(mPainterPath);
    //painter->drawText(m_textRect, Qt::AlignCenter, GetName());
    painter->restore();
}

void GUIGroundBuilding::SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord &centralCoord)
{
    prepareGeometryChange();

    mPainterPath = QPainterPath();

    qreal BuildingLength = length() * dScale;
    qreal BuildingWidth = width() * dScale;
    QPointF BuildingPnt((coord().GetX() - qllXMin) * dScale,
                        (qllYMax - coord().GetY()) * dScale);
    BuildingPnt = mapFromScene(BuildingPnt);

    QRectF BuildingRect(BuildingPnt.x() - BuildingLength/2,
                        BuildingPnt.y() - BuildingWidth/2,
                        BuildingLength, BuildingWidth);
    QPolygonF BuildingPoly(BuildingRect);
    QTransform transform;
    transform.translate(BuildingPnt.x(), BuildingPnt.y());
    transform.rotate(-90);
    transform.rotate(azimuth());
    transform.translate(-BuildingPnt.x(), -BuildingPnt.y());
    mPainterPath.addPolygon(transform.map(BuildingPoly));

    update();
}


