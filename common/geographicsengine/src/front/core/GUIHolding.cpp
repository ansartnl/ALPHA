#include "GUIHolding.h"
#include "GUIScene.h"
#include <QPainter>
#include <math.h>
#include "back/common/Calculation.h"
#include <QMessageBox>
#include "front/common/UserSettings.h"
#include "front/common/ZOrderValue.h"


//! Geodesy graphics engine
namespace geoGE {

GUIHolding::GUIHolding(const GUIScene* pFIR, QGraphicsItem *parent)
    : GUIObject(parent), m_pFIR(pFIR), m_iPenWidth(1)
{
    setZValue( getZOrderValue(ZOrderHolding) );
	setOpacity(1);
//    SetColor( Qt::lightGray );
    SetColor( UserSettings::Accessor::instance().color(UserSettings::CtMap) );
}

GUIHolding::~GUIHolding()
{

}

QRectF GUIHolding::boundingRect() const
{
	return m_painterPath.boundingRect();
}

QPainterPath GUIHolding::shape() const
{
	return m_painterPath;
}

void GUIHolding::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if (IsItemActive())
	{
		//QString sText;
		/*switch(GetType())
		{
		case enUndef:
			sText = QObject::tr("0");
			break;
		case enHighAlt:
			sText = QObject::tr("1");
			break;
		case enLowAlt:
			sText = QObject::tr("2");
			break;
		case enSID:
			sText = QObject::tr("3");
			break;
		case enSTAR:
			sText = QObject::tr("4");
			break;
		case enAppr:
			sText = QObject::tr("5");
			break;
		case enMisAppr:
			sText = QObject::tr("6");
			break;
		case enUndef2:
			sText = QObject::tr("7");
			break;
		default:
			qWarning("unknow CHolding::Type");
		}*/
        QPen qPen( GetColor() );
        qPen.setCosmetic(true);
		qPen.setWidth(m_iPenWidth);
		painter->setPen(qPen);
		//painter->setBrush(QBrush(m_color, Qt::SolidPattern));
		painter->setBrush(Qt::NoBrush);
		painter->drawPath(m_painterPath);
		/////////////////
		//painter->setOpacity(0.5);
		//sText = GetPoint()->GetName();
		//painter->drawText(boundingRect(), Qt::AlignCenter, sText);
		/////////////////
	}
}


void GUIHolding::SetPosition(const TCoordSystem &cs)
{
    prepareGeometryChange();
    m_painterPath = QPainterPath();

    qreal angleCorrection = 0.0;
    // Find correction angle to the north direction of current FIR.
    if ( m_pFIR )
    {
        qreal b, l;
        // Retrieve center point of a FIR (scene)
        m_pFIR->Prepare(b, l);
        angleCorrection = cs->findAngleToNorth(b, l);
    }

    const CCoord &coord = GetPoint()->GetCoord();
    QPointF point = cs->toScene(QVector3D(coord.GetX(), coord.GetY(), coord.GetZ()));

    point = mapFromScene(point);

    QLineF tempLine(point, QPointF(point.x()+10, point.y()));
    qreal angle = 360-GetInCourse()+90 - angleCorrection;
    angle += 180;
    while(angle >= 360) angle -= 360;
    tempLine.setAngle(angle);
    qreal speed = GetSpeed() ? GetSpeed() : m_qDefaultSpeed;

    qreal radius = cs->distanceToScene( avia::GetTurningRadius(speed, 25*M_PI/180) );
    qreal len = cs->distanceToScene( GetLegLength() );
    if(!len)
        len = cs->distanceToScene( speed * GetLegTime() );
    tempLine.setLength(len);

    //leg1
    m_painterPath.moveTo(tempLine.p2());
    m_painterPath.lineTo(tempLine.p1());

    //arc1
    QLineF normal = tempLine.normalVector();
    normal.setLength(2*radius);
    if(GetTurn() == enLeft)
    {
        normal = normal.normalVector();
        normal = normal.normalVector();
    }

    tempLine.setP1(normal.p2());
    tempLine.setP2(normal.p1());

    normal.setLength(radius);
    QRectF rect(QPointF(normal.p2().x()-radius, normal.p2().y()-radius), QSizeF(2*radius, 2*radius));
    if(GetTurn() == enLeft)
        m_painterPath.arcTo(rect, tempLine.angle(), 180);
    else
        m_painterPath.arcTo(rect, tempLine.angle(), -180);

    //leg2
    normal = tempLine.normalVector();
    normal.setLength(len);
    if(GetTurn() == enLeft)
    {
        normal = normal.normalVector();
        normal = normal.normalVector();
    }
    m_painterPath.lineTo(normal.p2());

    //arc2
    tempLine.setP1(normal.p2());
    tempLine.setP2(normal.p1());
    tempLine.setLength(2*radius);
    normal = tempLine.normalVector();
    if(GetTurn() == enLeft)
    {
        normal = normal.normalVector();
        normal = normal.normalVector();
    }
    tempLine.setP1(normal.p2());
    tempLine.setP2(normal.p1());

    normal.setLength(radius);
    rect = QRectF(QPointF(normal.p2().x()-radius, normal.p2().y()-radius), QSizeF(2*radius, 2*radius));
    if(GetTurn() == enLeft)
        m_painterPath.arcTo(rect, tempLine.angle(), 180);
    else
        m_painterPath.arcTo(rect, tempLine.angle(), -180);

    update();
}

void GUIHolding::MarkItemActive(bool bActive)
{
    Holding::MarkItemActive(bActive);
	setVisible(bActive);
}

}   // namespace geoGE
