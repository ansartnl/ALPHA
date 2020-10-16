#include "GUIMeteoVector.h"
#include <QPainter>
#include <QPainterPathStroker>
#include "front/common/UserSettings.h"
#include "front/common/ZOrderValue.h"


//! Geodesy graphics engine
namespace geoGE {

GUIMeteoVector::GUIMeteoVector(QGraphicsItem *parent)
    : GUIObject(parent)
{
    setZValue( getZOrderValue(ZOrderMeteoVector) );
    SetColor( UserSettings::Accessor::instance().color(UserSettings::CtMeteo) );
}

QRectF GUIMeteoVector::boundingRect() const
{
    return mShapeStroker.boundingRect();
}

QPainterPath GUIMeteoVector::shape() const
{
    return mShapeStroker;
}

void GUIMeteoVector::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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

void GUIMeteoVector::SetPosition(const TCoordSystem &cs)
{
    prepareGeometryChange();

    mPainterPath = QPainterPath();

    const TVectors& meteoVecs = GetVectors();
    for ( TVectors::const_iterator itc = meteoVecs.begin(); itc != meteoVecs.end(); ++itc )
    {
        const TVector& vec = *itc;
        QPointF pointS = cs->toScene(vec.first),
                pointE = cs->toScene(vec.second);

        mPainterPath.moveTo(pointS);
        mPainterPath.lineTo(pointE);
    }

    QPainterPathStroker pathStroker;
    pathStroker.setWidth(GetIntencity()+1);
    mShapeStroker = pathStroker.createStroke(mPainterPath);

    update();
}

}   // namespace geoGE
