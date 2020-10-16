#include "GUIGroundPoint.h"
#include <QPainter>
#include <math.h>

GUIGroundPoint::GUIGroundPoint(QGraphicsItem *parent)
: QGraphicsItem(parent), mPenWidth(1), mBorderColor(216,216,216), mFillColor(216,216,216)
,mIsHovered(false)
{
    setZValue(gui::Zpoint);
    setAcceptHoverEvents(true);
}

GUIGroundPoint::~GUIGroundPoint()
{
}

void GUIGroundPoint::setType(GroundPoint::Type val)
{
    GroundPoint::setType(val);
    setupColors();
}

QRectF GUIGroundPoint::boundingRect() const
{
    return mPainterPath.boundingRect();//.united(m_roundingPath.boundingRect());
}

QPainterPath GUIGroundPoint::shape() const
{
    return mHoverPath;
}

void GUIGroundPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    QPen qPen(mBorderColor);
    qPen.setWidth(mPenWidth);
    painter->setPen(qPen);
    painter->setBrush(QBrush(mFillColor, Qt::SolidPattern));
    painter->drawPath(mPainterPath);

    painter->restore();
}

void GUIGroundPoint::SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord &centralCoord)
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

    scaledRadius = ground_point::HOVER_RADIUS *dScale;
    mHoverPath.addEllipse(point, scaledRadius, scaledRadius);

    update();
}

void GUIGroundPoint::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    mIsHovered = true;
    setupColors();
    update();
    emit hoverEnter(index());
}

void GUIGroundPoint::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    mIsHovered = false;
    setupColors();
    update();
    emit hoverLeave(index());
}

void GUIGroundPoint::setupColors()
{
    if(mIsHovered)
    {
        switch(GroundPoint::type())
        {
        case Normal:
        case HOLD_SHORT:
            mBorderColor = QColor(56,109,172);
            mFillColor = QColor(56,109,172);
            break;
        case START:
            mBorderColor = QColor(216,216,216);
            mFillColor = Qt::red;
            break;
        default:
            break;
        }
    }
    else
    {
        switch(GroundPoint::type())
        {
        case Normal:
            mBorderColor = QColor(216,216,216);
            mFillColor = QColor(216,216,216);
            break;
        case HOLD_SHORT:
            mBorderColor = QColor(216,216,216);
            mFillColor = QColor(56,109,172);
            break;
        case START:
            mBorderColor = Qt::red;
            mFillColor = Qt::red;
            break;
        default:
            break;
        }
    }
}


