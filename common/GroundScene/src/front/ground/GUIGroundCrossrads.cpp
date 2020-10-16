#include "GUIGroundCrossrads.h"
#include <QPainter>
#include <math.h>

GUIGroundCrossroads::GUIGroundCrossroads(QGraphicsItem *parent)
: QGraphicsItem(parent), mPenWidth(1), mColor(171,171,171)//Pasha mColor(Qt::darkGray)
{
    setZValue(gui::Zcrossroads);
}

GUIGroundCrossroads::~GUIGroundCrossroads()
{
}

QRectF GUIGroundCrossroads::boundingRect() const
{
    return mRoundingPath.boundingRect();
}

QPainterPath GUIGroundCrossroads::shape() const
{
    return mRoundingPath;
}

void GUIGroundCrossroads::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    QPen roundPen(mColor);
    roundPen.setWidth(mPenWidth);
    painter->setPen(roundPen);
    painter->setBrush(QBrush(mColor, Qt::SolidPattern));
    painter->drawPath(mRoundingPath);

    painter->restore();
}

void GUIGroundCrossroads::SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord &centralCoord)
{
    prepareGeometryChange();

    QPointF point(
        (coord().GetX() - qllXMin) * dScale,
        (qllYMax - coord().GetY()) * dScale);
    point = mapFromScene(point);

    //calculating rouning shape for intersecting paths
    if(azimuths().count() > 1)
    {
        mRoundingPath = QPainterPath();
        mRoundingPath.setFillRule(Qt::WindingFill);
        for(int i = 0; i < azimuths().count();i++)
            for(int j = 0; j < azimuths().count();j++)
            {
                if(i != j)
                {
                    double dDelta = azimuths()[j] - azimuths()[i];    //next - first
                    if ((dDelta >= 15 && dDelta <= 165) ||
                        (dDelta >= -345 && dDelta <= -195))
                    {//right
                        QLineF right(point,
                                     mapFromScene(QPointF(0,0)));
                        right.setLength( (getWidths()[i]-1)*dScale / 2);
                        right.setAngle(90 - azimuths()[i] - 90);

                        right.setP1(right.p2());
                        right.setP2(mapFromScene(QPointF(0,0)));
                        right.setLength(20);
                        right.setAngle(90 - azimuths()[i]);

                        QLineF left(point,
                                     mapFromScene(QPointF(0,0)));
                        left.setLength( (getWidths()[j]-1)*dScale / 2);
                        left.setAngle(90 - azimuths()[j] + 90);

                        left.setP1(left.p2());
                        left.setP2(mapFromScene(QPointF(0,0)));
                        left.setLength(20);
                        left.setAngle(90 - azimuths()[j]);

                        QPointF intersection;
                        if(right.intersect(left, &intersection) != QLineF::NoIntersection)
                        {
                            double dLen = QLineF(point, intersection).length();

                            if(dLen > dScale * ( getWidths()[i] + getWidths()[j]) )
                            {
                                QLineF intersectLine(point, intersection);
                                intersectLine.setLength( dScale * ( qMax(getWidths()[i],  getWidths()[j])) );
                                intersection = intersectLine.p2();
                                dLen = QLineF(point, intersection).length();
                            }

                            if( qAbs(dDelta) >= 45)
                                dLen *= 2;

                            right.setLength(dLen);
                            left.setLength(dLen);

                            mRoundingPath.moveTo(point);
                            mRoundingPath.lineTo(mapFromScene(right.p2()));
                            mRoundingPath.quadTo(mapFromScene(intersection), mapFromScene(left.p2()));
                            mRoundingPath.lineTo(point);
                        }

                    }
                }
            }
    }
    update();
}
