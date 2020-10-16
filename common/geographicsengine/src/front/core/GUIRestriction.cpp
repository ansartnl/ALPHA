#include "GUIRestriction.h"
#include <QPainter>
#include <math.h>
#include <QMessageBox>
#include "front/common/UserSettings.h"
#include "front/common/ZOrderValue.h"


//! Geodesy graphics engine
namespace geoGE {

GUIRestriction::GUIRestriction(QGraphicsItem *parent)
    : GUIUntransformableObject(parent), m_iPenWidth(1)
{
    setZValue( getZOrderValue(ZOrderRestriction) );
	setOpacity(0.15);
    SetColor( Qt::red );
}

GUIRestriction::~GUIRestriction()
{

}

QRectF GUIRestriction::boundingRect() const
{
    return m_boundingRect;
}

QPainterPath GUIRestriction::shape() const
{
    return m_painterPath;
}

void GUIRestriction::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if ( IsItemActive() )
	{
		QString sText;
		switch(GetType())
		{
		case enAlert:
			sText = QObject::tr("A");
			break;
		case enCaution:
			sText = QObject::tr("C");
			break;
		case enDanger:
			sText = QObject::tr("D");
			break;
		case enMilitary:
			sText = QObject::tr("M");
			break;
		case enProhibited:
			sText = QObject::tr("P");
			break;
		case enRestricted:
			sText = QObject::tr("R");
			break;
		case enTraining:
			sText = QObject::tr("T");
			break;
		case enWarning:
			sText = QObject::tr("W");
			break;
		case enUnknown:
			sText = QObject::tr("U");
			break;
		default:
			qWarning("unknow CRestriction::Type");
		}

        painter->setFont( UserSettings::Accessor::instance().font(UserSettings::FtGeodesy) );

        QPen qPen( GetColor() );
		qPen.setWidth(m_iPenWidth);
        qPen.setCosmetic(true);
		painter->setPen(qPen);
        painter->setBrush(QBrush(GetColor(), Qt::SolidPattern));
		painter->drawPath(m_painterPath);
		painter->setOpacity(0.5);

        // Do not hesitate to call rect of painter path as painter path caches rect
        // once it is requested.
        painter->drawText(m_painterPath.boundingRect(), Qt::AlignCenter, sText);
	}
}


void GUIRestriction::SetPosition(const TCoordSystem &cs)
{
    prepareGeometryChange();

    m_painterPath = QPainterPath();
    TPointList::const_iterator itScan = GetPoints().begin();
    for(; itScan != GetPoints().end(); ++itScan)
    {
        const CCoord &coord = (*itScan)->GetCoord();
        QPointF point = transformToRatio(
                cs->toSceneWithTransform(QVector3D(coord.GetX(), coord.GetY(), coord.GetZ())) );

        const CCoord &arcCoord = (*itScan)->GetArcCoord();
        QPointF pointArc = transformToRatio(
                cs->toSceneWithTransform(QVector3D(arcCoord.GetX(), arcCoord.GetY(), arcCoord.GetZ())) );

        TPointList::const_iterator itScanNext = itScan+1;
        if(itScanNext == GetPoints().end()) itScanNext = GetPoints().begin();

        const CCoord &coordNext = (*itScanNext)->GetCoord();
        QPointF pointNext = transformToRatio(
                cs->toSceneWithTransform(QVector3D(coordNext.GetX(), coordNext.GetY(), coordNext.GetZ())) );

        qreal qRadius = cs->distanceToSceneWT( (*itScan)->GetArcDistance() ) *
                        transformToRatio(QPointF(1.0, 1.0)).x();

        if(itScan == GetPoints().begin()) m_painterPath.moveTo(point);

        switch((*itScan)->GetType())
        {
        case arinc::RestrictPoint::enLine:
            m_painterPath.lineTo(pointNext);
            break;
        case arinc::RestrictPoint::enCircle:
            m_painterPath.addEllipse(pointArc, qRadius, qRadius);
            break;
        case arinc::RestrictPoint::enLeftArc:
        case arinc::RestrictPoint::enRightArc:
            {
                QRectF rect(QPointF(pointArc.x()-qRadius, pointArc.y()-qRadius), QSizeF(2*qRadius, 2*qRadius));
                QLineF line1(pointArc, point);
                QLineF line2(pointArc, pointNext);
                if((*itScan)->GetType() == arinc::RestrictPoint::enLeftArc)
                    m_painterPath.arcTo(rect, line1.angle(), line1.angleTo(line2));
                else
                    m_painterPath.arcTo(rect, line1.angle(), line1.angleTo(line2)-360);
            }
            break;
        default:
            qWarning("unknow CRestrictPoint::Type");
        }
    }

    m_boundingRect = m_painterPath.boundingRect();
    // Needed for printing as during calling bounding rect for untransformable graphics items
    // Qt does check bounding rect for the scaled (with ratio) items in scene coordinates.
    if ( isTransformEnabled() )
    {
        // Restore bounding rect to scene coordinates without applying the ratio.
        m_boundingRect = QTransform().scale(1.0/m_xratio, 1.0/m_yratio).mapRect(m_boundingRect);
        m_boundingRect = cs->getSceneTransform().inverted().mapRect(m_boundingRect);
    }

    update();
}

void GUIRestriction::MarkItemActive(bool bActive)
{
    Restriction::MarkItemActive(bActive);
	if(GetType() != enDanger) setVisible(bActive);
}

}   // namespace geoGE
