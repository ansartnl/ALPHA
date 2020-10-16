#include "GUIPeleng.h"
#include <QPen>
#include <QPainter>
#include "front/common/UserSettings.h"
#include "front/common/ZOrderValue.h"


//! Geodesy graphics engine
namespace geoGE {

GUIPeleng::GUIPeleng()
    : mChannelNumber(this), mIcon(QRect(-2,-2,4,4), this)
{
    setZValue( getZOrderValue(ZOrderPeleng) );
    SetColor( UserSettings::Accessor::instance().color(UserSettings::CtPeleng) );

    mChannelNumber.setFlag(QGraphicsItem::ItemIgnoresTransformations);
    mIcon.setFlag(QGraphicsItem::ItemIgnoresTransformations);
}

QRectF GUIPeleng::boundingRect() const
{
    return mBoundRect;
}

QPainterPath GUIPeleng::shape() const
{
    return mShape;
}

void GUIPeleng::paint(
	QPainter *painter,
	const QStyleOptionGraphicsItem *option,
	QWidget *widget)
{
    QPen qPen( GetColor() );
    qPen.setCosmetic(true);
    qPen.setWidth(2);
    painter->setPen(qPen);

    painter->drawPath(mShape);
}

void GUIPeleng::SetPosition(const TCoordSystem &cs)
{
    prepareGeometryChange();

    mShape = QPainterPath();

    QPointF pnt = cs->toScene( GetCoord() );
    const qreal radius = 100.*1000.; // 100 km

    QLineF line(pnt, QPointF(pnt.x()+1.0, pnt.y()));
    line.setLength( cs->distanceToScene(radius) );
    line.setAngle(90 - GetAngle());

    mShape.moveTo(pnt);
    mShape.lineTo(line.p2());

    mChannelNumber.setPlainText( QString::number(GetChannelNumber()) );
    mChannelNumber.setFont( UserSettings::Accessor::instance().font(UserSettings::FtGeodesy) );
    mChannelNumber.setDefaultTextColor( GetColor() );
    mChannelNumber.setPos(pnt);

    mIcon.setBrush( QBrush(GetColor()) );
    mIcon.setPen( QPen(GetColor()) );
    mIcon.setPos(pnt);

    mBoundRect = mShape.boundingRect();
    mBoundRect = mBoundRect.unite( mChannelNumber.boundingRect() );
    mBoundRect.adjust(-2,-2,2,2);

    update();
}

void GUIPeleng::SetColor(const QColor& color)
{
    mChannelNumber.setDefaultTextColor(color);

    mIcon.setBrush( QBrush(color) );
    mIcon.setPen( QPen(color) );

    GUIObject::SetColor(color);
}

void GUIPeleng::UpdateFont()
{
    mChannelNumber.setFont( UserSettings::Accessor::instance().font(UserSettings::FtGeodesy) );
}

}   // namespace geoGE
