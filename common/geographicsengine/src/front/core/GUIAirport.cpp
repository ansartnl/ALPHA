#include "GUIAirport.h"
#include <QPen>
#include <QPainter>
#include <QGraphicsScene>
#include <math.h>
#include "front/common/UserSettings.h"
#include "front/common/ZOrderValue.h"

//! Geodesy graphics engine
namespace geoGE {

static const qreal gRadius = 4;
static const qreal gStroke = 6;

GUIAirport::GUIAirport()
	: m_iPenWidth(1)
{
    setZValue( getZOrderValue(ZOrderAirport) );
    SetColor( UserSettings::Accessor::instance().color(UserSettings::CtMap) );
}

QRectF GUIAirport::boundingRect() const
{
	return m_Zone;
}

QPainterPath GUIAirport::shape() const
{
	QPainterPath qPath;
    qPath.addRect(m_Zone);
	return qPath;
}

void GUIAirport::paint(
	QPainter *painter,
	const QStyleOptionGraphicsItem *option,
	QWidget *widget)
{
	if (IsItemActive())
	{
        QPen qPen( GetColor() );
        qPen.setCosmetic(true);
		qPen.setWidth(m_iPenWidth);
		painter->setPen(qPen);

        QFontMetricsF metrics(painter->font());
        QRectF rectAirport = metrics.boundingRect(GetName());
        rectAirport.moveCenter( QPointF(0., -gStroke-1.-rectAirport.height()/2.) );
        rectAirport.adjust(-1.,-1.,1.,1.);

        painter->drawText(rectAirport, Qt::AlignCenter, GetName());

        painter->drawPath(m_path);
    }
}

void GUIAirport::SetPosition(const TCoordSystem &cs)
{
    prepareGeometryChange();

    const CCoord &coord = GetCoord();
    m_CenterPoint = cs->toScene(QVector3D(coord.GetX(), coord.GetY(), coord.GetZ()));

    m_path = QPainterPath();

    m_path.addEllipse(QPointF(0.0,0.0), gRadius,gRadius);
    m_path.moveTo(0., -gStroke);
    m_path.lineTo(0., gStroke);
    m_path.moveTo(-gStroke, 0.);
    m_path.lineTo(gStroke, 0.);

    QFontMetricsF metrics(scene()->font());
    m_Zone = metrics.boundingRect(GetName());
    m_Zone.moveCenter( QPointF(0., -gStroke-1.-m_Zone.height()/2.) );
    m_Zone.adjust(-1.,-1.,1.,1.);

    m_Zone = m_Zone.united(m_path.boundingRect());
    // Needed for printing as during calling bounding rect for untransformable graphics items
    // Qt does check bounding rect for the scaled (with ratio) items in scene coordinates.
    if ( isTransformEnabled() )
    {
        // Restore bounding rect to scene coordinates without applying the ratio.
        m_Zone = QTransform().scale(1.0/m_xratio, 1.0/m_yratio).mapRect(m_Zone);
        m_Zone = cs->getSceneTransform().inverted().mapRect(m_Zone);
    }

    setPos(m_CenterPoint);
}

void GUIAirport::MarkItemActive(bool bActive)
{
    arinc::Airport::MarkItemActive(bActive);
	setVisible(bActive);
}

GUIAirport::TGUIRunway GUIAirport::CreateRunway(const QString &name)
{
    const TRunways &runways = GetRunways();
    TRunways::const_iterator itc = runways.find(name);
    if ( itc != runways.constEnd() )
    {
        if ( !(*itc)->GetCoord().IsNull() )
        {
            TGUIRunway runwayGui(new GUIRunway(*itc));
            mRunways.append(runwayGui);

            return runwayGui;
        }
    }
    return TGUIRunway();
}


///////////////////////////////////////////////////////////////////////////////////
// GUIRunways implementation

GUIRunway::GUIRunway(const TRunway &runway)
    : mRunway(runway)
{
    setZValue( getZOrderValue(ZOrderAirport) );
    SetColor( UserSettings::Accessor::instance().color(UserSettings::CtMap) );
}

QRectF GUIRunway::boundingRect() const
{
    return mBoundingRect;
}

QPainterPath GUIRunway::shape() const
{
    return mPath;
}

void GUIRunway::paint(
    QPainter *painter,
    const QStyleOptionGraphicsItem *option,
    QWidget *widget)
{
    if ( !IsItemActive() )
        return;

    QPen qPen( GetColor() );
    qPen.setCosmetic(true);
    painter->setPen(qPen);

    painter->setFont( UserSettings::Accessor::instance().font(UserSettings::FtGeodesy) );

    QFontMetricsF metrics(painter->font());
    QRectF rectText = metrics.boundingRect( mRunway->GetName() );
    rectText.moveCenter( mTextPos );
    rectText.adjust(-1.,-1.,1.,1.);

    painter->drawText(rectText, Qt::AlignCenter, mRunway->GetName());

    painter->drawPath(mPath);
}

void GUIRunway::SetPosition(const TCoordSystem &cs)
{
    prepareGeometryChange();

    mPath = QPainterPath();

    const CCoord& coord = mRunway->GetCoord();

    // Find correction angle to the north direction of current runway.
    qreal angleCorrection = cs->findAngleToNorth(coord.GetLatitude().GetAngle(),
                                                 coord.GetLongitude().GetAngle());

    QPointF point = transformToRatio(
            cs->toSceneWithTransform( QVector3D(coord.GetX(), coord.GetY(), coord.GetZ())) );
    mTextPos = point;

    qreal ratioFactor = transformToRatio(QPointF(1.0, 1.0)).x();

    QPointF pointLength(cs->distanceToSceneWT(mRunway->GetLength())*ratioFactor, 0.0);
    QLineF lineDir(point, point + pointLength);
    qreal angle = fmod(90 - mRunway->GetAzimuth(), 360);
    lineDir.setAngle(angle-angleCorrection);

    QLineF normal = lineDir.normalVector().unitVector();
    normal = QLineF(QPointF(0.0, 0.0), normal.p2()-normal.p1());
    normal.setLength( cs->distanceToSceneWT(mRunway->GetWidth())*ratioFactor );

    mPath.moveTo( lineDir.p1() + normal.p2() );
    mPath.lineTo( lineDir.p2() + normal.p2() );
    mPath.lineTo( lineDir.p2() - normal.p2() );
    mPath.lineTo( lineDir.p1() - normal.p2() );
    mPath.closeSubpath();

    mBoundingRect = mPath.boundingRect();

    QFontMetricsF metrics(scene()->font());
    QRectF rectName = metrics.boundingRect(mRunway->GetName());
    rectName.moveCenter( point );
    rectName.adjust(-1.,-1.,1.,1.);

    mBoundingRect = mBoundingRect.united(rectName);
    // Needed for printing as during calling bounding rect for untransformable graphics items
    // Qt does check bounding rect for the scaled (with ratio) items in scene coordinates.
    if ( isTransformEnabled() )
    {
        // Restore bounding rect to scene coordinates without applying the ratio.
        mBoundingRect = QTransform().scale(1.0/m_xratio, 1.0/m_yratio).mapRect(mBoundingRect);
        mBoundingRect = cs->getSceneTransform().inverted().mapRect(mBoundingRect);
    }

    update();
}

}   // namespace geoGE
