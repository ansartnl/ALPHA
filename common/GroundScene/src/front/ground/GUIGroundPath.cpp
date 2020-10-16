#include "GUIGroundPath.h"
#include <QPainter>

GUIGroundPath::GUIGroundPath(QGraphicsItem *parent)
    : QGraphicsItem(parent), mPenWidth(2), mPathColor(171,171,171)//Pasha mPathColor(Qt::darkGray)
{
    setZValue(gui::Zpath);
}

GUIGroundPath::~GUIGroundPath()
{
}

QRectF GUIGroundPath::boundingRect() const
{
    return mPainterSurfacePath.boundingRect();
}

QPainterPath GUIGroundPath::shape() const
{
    return mPainterSurfacePath;
}

void GUIGroundPath::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    if(GroundPath::type() != Runway)
    {
        QPen qPen(mPathColor);
        qPen.setWidth(mPenWidth);
        painter->setPen(qPen);
        painter->setBrush(QBrush(mPathColor, Qt::SolidPattern));        
        painter->drawPath(mPainterSurfacePath);        
    }

//    qPen.setColor(Qt::black);
//    qPen.setStyle(Qt::DotLine);
//    painter->setPen(qPen);
//    painter->setBrush(Qt::NoBrush);
//    painter->drawPath(m_painterPath);

    painter->restore();
}

void GUIGroundPath::SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord &centralCoord)
{
    prepareGeometryChange();

    mPainterSurfacePath = QPainterPath();
    mPainterSurfacePath.setFillRule(Qt::WindingFill);

    QPointF startPnt((startPointCoord().GetX() - qllXMin) * dScale,
                      (qllYMax - startPointCoord().GetY()) * dScale);
    QPointF endPnt((endPointCoord().GetX() - qllXMin) * dScale,
                      (qllYMax - endPointCoord().GetY()) * dScale);

    startPnt = mapFromScene(startPnt);
    endPnt = mapFromScene(endPnt);    

    double scaledWidth = width() * dScale;

    QLineF tempLine(startPnt, endPnt);
    QRectF pathRect(startPnt.x(),
                    startPnt.y() - scaledWidth/2,
                    tempLine.length(), scaledWidth);
    QPolygonF pathPoly(pathRect);
    QTransform transform;
    transform.translate(startPnt.x(), startPnt.y());
    transform.rotate(-tempLine.angle());
    transform.translate(-startPnt.x(), -startPnt.y());

    mPainterSurfacePath.addPolygon(transform.map(pathPoly));    
    mPainterSurfacePath.addEllipse(startPnt, scaledWidth/2, scaledWidth/2);
    mPainterSurfacePath.addEllipse(endPnt, scaledWidth/2, scaledWidth/2);

    update();
}


