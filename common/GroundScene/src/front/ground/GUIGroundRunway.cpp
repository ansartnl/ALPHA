#include "GUIGroundRunway.h"
#include <QPainter>

#include "back/ground/GroundScene.h"
#include <QDebug>

GUIGroundRunway::GUIGroundRunway(QGraphicsItem *parent)
: QGraphicsItem(parent), mPenWidth(2), mColor(74,74,74)//Pasha mColor(Qt::darkGray)
{
    setZValue(gui::Zrunway);
}

GUIGroundRunway::~GUIGroundRunway()
{
}

QRectF GUIGroundRunway::boundingRect() const
{
    return mPainterPath.boundingRect();
}

QPainterPath GUIGroundRunway::shape() const
{
    return mPainterPath;
}

void GUIGroundRunway::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    //Pasha QPen qPen(Qt::lightGray);
    QPen qPen(Qt::black);
    qPen.setWidth(mPenWidth);
    painter->setPen(qPen);
    painter->setBrush(QBrush(mColor, Qt::SolidPattern));
    painter->drawPath(mPainterPath);        

    qPen.setColor(Qt::white);
    painter->setBrush(QBrush(Qt::white));
    painter->setPen(qPen);
    painter->drawPath(mMarkingPath);

    painter->restore();
}

void GUIGroundRunway::SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord &centralCoord)
{
    prepareGeometryChange();

    mPainterPath = QPainterPath();
    qreal RunwayLength = length() * dScale;
    qreal RunwayWidth = width() * dScale;

    QPointF RunwayPnt((coord().GetX() - qllXMin) * dScale,
                      (qllYMax - coord().GetY()) * dScale);
    RunwayPnt = mapFromScene(RunwayPnt);

    QRectF RunwayRect(RunwayPnt.x() - RunwayLength/2,
                      RunwayPnt.y() - RunwayWidth/2,
                      RunwayLength, RunwayWidth);

    double dStep = RunwayWidth / 11.;
    QRectF leftMarkRect(RunwayPnt.x() - RunwayLength/2 + dStep,
                        RunwayPnt.y() - RunwayWidth/2 + dStep,
                        350*dScale - dStep, RunwayWidth - 2*dStep);
    QRectF rightMarkRect(RunwayPnt.x() + RunwayLength/2 - dStep,
                        RunwayPnt.y() - RunwayWidth/2 + dStep,
                        -350*dScale + dStep, RunwayWidth - 2*dStep);

    QPolygonF RunwayPoly(RunwayRect);
    QTransform transform;
    transform.translate(RunwayPnt.x(), RunwayPnt.y());
    transform.rotate(-90);
    transform.rotate(azimuth());
    transform.translate(-RunwayPnt.x(), -RunwayPnt.y());
    mPainterPath.addPolygon(transform.map(RunwayPoly));

    mMarkingPath = QPainterPath();

    QPointF tlPoint(leftMarkRect.topLeft());
    QPointF trPoint(rightMarkRect.topLeft());

    for(int i = 0; i < 9; i++)
    {
        if(i != 4)  // mind blank space in a middle
        {// runway endings
            //left
            QPolygonF markPoly(QRectF(tlPoint.x(), tlPoint.y() + dScale,
                        50*dScale, dStep - 2*dScale));
            mMarkingPath.addPolygon(transform.map(markPoly));
            //right
            markPoly = QPolygonF(QRectF(trPoint.x()- 50*dScale, trPoint.y() + dScale,
                        50*dScale, dStep - 2*dScale));
            mMarkingPath.addPolygon(transform.map(markPoly));
        }

        if(i < 2 || i > 6)
        {// touchdown lines
            QPolygonF markPoly(QRectF(tlPoint.x() + leftMarkRect.width() - 50*dScale, tlPoint.y(),
                        50*dScale, dStep));
            mMarkingPath.addPolygon(transform.map(markPoly));

            markPoly = QPolygonF(QRectF(trPoint.x() - leftMarkRect.width(), trPoint.y(),
                        50*dScale, dStep));
            mMarkingPath.addPolygon(transform.map(markPoly));
        }
        tlPoint.setY(tlPoint.y() + dStep);
        trPoint.setY(trPoint.y() + dStep);
    }

    update();
}


