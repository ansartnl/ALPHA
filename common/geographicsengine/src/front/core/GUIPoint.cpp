#include "GUIPoint.h"
#include <QPen>
#include <QPainter>
#include <QGraphicsScene>
#include <QPrinter>
#include <QBitmap>
#include <QIcon>
#include <math.h>
#include "front/common/UserSettings.h"
#include "front/common/ZOrderValue.h"


//! Geodesy graphics engine
namespace geoGE {

//! Singleton for point resources (sign masks) - QBitmaps.
class PointResources
{
public:
    //! Default constructor - called once.
    PointResources()
    {
        const int countMasks = 7;
        QString pathMasks[countMasks] = { ":/Map/Point", ":/Map/KTAPoint", ":/Map/TWPPoint",
                                      ":/Map/BeaconNDB", ":/Map/BeaconVOR", ":/Map/BeaconDME",
                                  ":/Map/BeaconVORDME" };
        QBitmap* masks[countMasks] = { &maskPoint, &maskKTA, &maskTWP, &maskBeaconNDB, &maskBeaconVOR,
                                       &maskBeaconDME, &maskBeaconVORDME };

        for ( int i = 0; i < countMasks; i++ )
        {
            QIcon icon;
            icon.addFile(pathMasks[i], QSize(), QIcon::Normal, QIcon::Off);
            Q_ASSERT(!icon.isNull());
            *masks[i] = icon.pixmap(QSize(gIconSize,gIconSize)).createMaskFromColor(Qt::black, Qt::MaskOutColor);
        }
    }

    static PointResources& instance()
    {
        static PointResources res;
        return res;
    }

public:
    /** Point, airport point and TWP point */
    QBitmap maskPoint, maskKTA, maskTWP;
    /** Beacon masks */
    QBitmap maskBeaconNDB, maskBeaconVOR, maskBeaconDME, maskBeaconVORDME;
    /** Icon size */
    static const int gIconSize = 16;
};


GUIPoint::GUIPoint()
	: m_iPenWidth(1)
{
    setZValue( getZOrderValue(ZOrderPoint) );
    SetColor( UserSettings::Accessor::instance().color(UserSettings::CtMap) );
}

QRectF GUIPoint::boundingRect() const
{
    return m_path.boundingRect();
    // return m_Zone;
}

QPainterPath GUIPoint::shape() const
{
    return m_path;
}

void GUIPoint::paint(
	QPainter *painter,
	const QStyleOptionGraphicsItem *option,
	QWidget *widget)
{
    if ( !IsItemActive() )
        return;

    QPen qPen( GetColor() );
    qPen.setWidth(m_iPenWidth);
    qPen.setCosmetic(true);
    painter->setPen(qPen);

    painter->setFont( UserSettings::Accessor::instance().font(UserSettings::FtGeodesy) );

    const int halfIconSize = PointResources::gIconSize/2;

    QPixmap *icon = 0;
    switch ( GetType() )
    {
    case enAirportPoint:
        icon = static_cast<QPixmap *>(&PointResources::instance().maskTWP);
        break;
    case enVHFNavAid:
        icon = static_cast<QPixmap *>(&PointResources::instance().maskBeaconVORDME);
        break;
    case enNDBNavAid:
    case enAirportNDB:
        icon = static_cast<QPixmap *>(&PointResources::instance().maskBeaconNDB);
        break;
    default:
        icon = static_cast<QPixmap *>(&PointResources::instance().maskPoint);
    }

    if ( icon )
        painter->drawPixmap(-halfIconSize, -halfIconSize, *icon);

    QFontMetricsF metrics(painter->font());

    QRectF TextRect = metrics.boundingRect(GetName());
    TextRect.moveCenter( QPointF(0., -halfIconSize-1.-TextRect.height()/2.) );
    TextRect.adjust(-1.,-1.,1.,1.);

    painter->drawText(TextRect, GetName(), QTextOption(Qt::AlignCenter));
}

void GUIPoint::SetPosition(const TCoordSystem &cs)
{
    const CCoord &coord = GetCoord();
    QPointF pnt = cs->toScene(QVector3D(coord.GetX(), coord.GetY(), coord.GetZ()));

    const int halfIconSize = PointResources::gIconSize/2;

    QFontMetricsF metrics(scene()->font());
    QRectF boundRect = metrics.boundingRect(GetName());
    boundRect.moveCenter( QPointF(0., -halfIconSize-1.-boundRect.height()/2.) );
    boundRect.adjust(-1.,-1.,1.,1.);

    boundRect = boundRect.united(QRectF(-halfIconSize,-halfIconSize,
                                        PointResources::gIconSize,PointResources::gIconSize));

    // Needed for printing as during calling bounding rect for untransformable graphics items
    // Qt does check bounding rect for the scaled (with ratio) items in scene coordinates.
    if ( isTransformEnabled() )
    {
        // Restore bounding rect to scene coordinates without applying the ratio.
        boundRect = QTransform().scale(1.0/m_xratio, 1.0/m_yratio).mapRect(boundRect);
        boundRect = cs->getSceneTransform().inverted().mapRect(boundRect);
    }

    m_path.addRect(boundRect);

    setPos(pnt);
}

void GUIPoint::MarkItemActive(bool bActive)
{
    arinc::Point::MarkItemActive(bActive);
	setVisible(bActive);
}

}   // namespace geoGE
