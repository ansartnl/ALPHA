#include "GUIMeteoContour.h"
#include <QPainter>
#include <QPainterPathStroker>
#include "front/common/UserSettings.h"
#include "front/common/ZOrderValue.h"


//! Geodesy graphics engine
namespace geoGE {

GUIMeteoContour::GUIMeteoContour(QGraphicsItem *parent)
    : GUIObject(parent)
{
    setZValue( getZOrderValue(ZOrderMeteoContour) );
    SetColor( UserSettings::Accessor::instance().color(UserSettings::CtMeteo) );
}

QRectF GUIMeteoContour::boundingRect() const
{
    return mShapeStroker.boundingRect();
}

QPainterPath GUIMeteoContour::shape() const
{
    return mShapeStroker;
}

void GUIMeteoContour::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen penOld = painter->pen();

    QColor color = GetColor();

    for ( int i = GetIntencity()+1; i > 0; i-- )
    {
        QPen qPen(color);
        qPen.setWidth(i);
        qPen.setCosmetic(true);
        painter->setPen(qPen);

        painter->drawPath(mPainterPath);

        color = color.lighter(110);
    }

    painter->setPen(penOld);
}

void GUIMeteoContour::SetPosition(const TCoordSystem &cs)
{
    prepareGeometryChange();

    mPainterPath = QPainterPath();
    mShapeStroker = QPainterPath();

    bool isFirstPoint = true;
    const TPoints& meteoContour = GetContour();
    // Make sure this is a contour.
    if ( meteoContour.size() < 3 )
        return;

    for ( TPoints::const_iterator itc = meteoContour.begin(); itc != meteoContour.end(); ++itc )
    {
        QPointF pnt = cs->toScene(*itc);

        if ( isFirstPoint )
        {
            mPainterPath.moveTo(pnt);
            isFirstPoint = false;
        }
        else
            mPainterPath.lineTo(pnt);
    }
    if ( !mPainterPath.isEmpty() )
        mPainterPath.closeSubpath();

    QPainterPathStroker pathStroker;
    pathStroker.setWidth(GetIntencity()+1);
    mShapeStroker = pathStroker.createStroke(mPainterPath);

    update();
}

}   // namespace geoGE
