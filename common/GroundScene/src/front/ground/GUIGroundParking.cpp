#include "GUIGroundParking.h"
#include <QPainter>

GUIGroundParking::GUIGroundParking(QGraphicsItem *parent)
: QGraphicsItem(parent), mPenWidth(1), mColor(Qt::black)//Pasha mColor(Qt::darkRed)
{
    setZValue(gui::Zparking);
    setAcceptHoverEvents(true);
    QSettings settings;
    mFontSize = settings.value("GroundScene/Parking/FontSize", 5).toInt();
    settings.setValue("GroundScene/Parking/FontSize", mFontSize);
}

GUIGroundParking::~GUIGroundParking()
{
}

QRectF GUIGroundParking::boundingRect() const
{
    return mPainterPath.boundingRect();
}

QPainterPath GUIGroundParking::shape() const
{
    //return m_painterPath;
    return mHoverPath;
}

void GUIGroundParking::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    QPen qPen(mColor);
    qPen.setWidth(mPenWidth);
    painter->setPen(qPen);
    //Pasha painter->setBrush(QBrush(mColor, Qt::Dense5Pattern));//Qt::SolidPattern));
    painter->setBrush(QBrush(QColor(200,200,200), Qt::SolidPattern));
    painter->drawPath(mPainterPath);
    //Pasha
    QFont font = painter->font();
    font.setPointSize(mFontSize);
    painter->setFont(font);
    painter->drawText(boundingRect(), Qt::AlignCenter, name());
    //
    painter->restore();
}

void GUIGroundParking::SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord &centralCoord)
{
    prepareGeometryChange();
    mPainterPath = QPainterPath();
    QPointF point(
        (coord().GetX() - qllXMin) * dScale,
        (qllYMax - coord().GetY()) * dScale);
    point = mapFromScene(point);

    double scaledRadius = radius() *dScale;
    mPainterPath.addEllipse(point, scaledRadius, scaledRadius);

    mHoverPath = QPainterPath();
    scaledRadius = qMax( (double)ground_parking::HOVER_RADIUS, radius())  *dScale;
    mHoverPath.addEllipse(point, scaledRadius, scaledRadius);

    setToolTip(name());

    update();
}

void GUIGroundParking::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    //Pasha mColor = Qt::yellow;
    mColor = Qt::white;
    update();
    emit hoverEnter(name());
}

void GUIGroundParking::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    //Pasha mColor = Qt::darkRed;
    mColor = Qt::black;
    update();
    emit hoverLeave(name());
}
