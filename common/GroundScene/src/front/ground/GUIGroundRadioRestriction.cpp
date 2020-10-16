#include "GUIGroundRadioRestriction.h"
#include <QPainter>

GUIGroundRadioRestriction::GUIGroundRadioRestriction(QGraphicsItem *parent)
: QGraphicsItem(parent), mPenWidth(1), mColor(Qt::red)
{
    setZValue(gui::ZRadioRestriction);
}

GUIGroundRadioRestriction::~GUIGroundRadioRestriction()
{
}

QRectF GUIGroundRadioRestriction::boundingRect() const
{
    return mPainterPath.boundingRect();
}

QPainterPath GUIGroundRadioRestriction::shape() const
{
    return mPainterPath;
}

void GUIGroundRadioRestriction::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    QPen qPen(mColor);
    qPen.setWidth(mPenWidth);
    painter->setPen(qPen);
    painter->setBrush(QBrush(mColor, Qt::BDiagPattern));
    painter->drawPath(mPainterPath);
    painter->restore();
}

void GUIGroundRadioRestriction::SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord &centralCoord)
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


