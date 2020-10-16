#include "GUIHolding.h"
#include <QtGui/QPainter>

#if defined(_MSC_VER) && QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include "back/common/Calculation.h"
#include "back/utils/ProfileSettings.h"

CGUIHolding::CGUIHolding(QGraphicsItem *parent)
	: CGUIObject(parent), m_iPenWidth(1)
{
    setZValue(gui::Zholding);
	setOpacity(1);
    //SetColor( UserProfile::CSettings::Instance().GetColor(UserProfile::CtMap) );
    SetColor( UserProfile::CSettings::Instance().GetColor(UserProfile::CtHolding) );
}

CGUIHolding::~CGUIHolding()
{

}

QRectF CGUIHolding::boundingRect() const
{
	return m_painterPath.boundingRect();
}

QPainterPath CGUIHolding::shape() const
{
	return m_painterPath;
}

void CGUIHolding::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if (IsItemActive())
	{
		painter->save();
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
		painter->restore();
	}
}


void CGUIHolding::SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord& centralCoord)
{
	if (IsItemActive())
	{
		prepareGeometryChange();
		m_painterPath = QPainterPath();
		QPointF point(
			(GetPoint()->GetCoord().GetX() - qllXMin) * dScale,
			(qllYMax - GetPoint()->GetCoord().GetY()) * dScale);
		point = mapFromScene(point);

		QLineF tempLine(point, QPointF(point.x()+10, point.y()));
		qreal angle = 360-GetInCourse()+90;
		angle += 180;
		while(angle >= 360) angle -= 360;
		tempLine.setAngle(angle);
		qreal speed = GetSpeed() ? GetSpeed() : m_qDefaultSpeed;
		qreal radius = avia::GetTurningRadius(speed, 25*M_PI/180) * dScale;
		qreal len = GetLegLength() * dScale;
		if(!len)
			len = speed * GetLegTime() * dScale;
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
}

void CGUIHolding::MarkItemActive(bool bActive)
{
	CHolding::MarkItemActive(bActive);
	setVisible(bActive);
}
