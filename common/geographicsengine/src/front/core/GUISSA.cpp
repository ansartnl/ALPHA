#include "GUISSA.h"
#include "GUIScene.h"

#include <QPen>
#include <QPainter>
#include <QGraphicsScene>
#include <QMessageBox>
#include "front/common/UserSettings.h"
#include "front/common/ZOrderValue.h"


//! Geodesy graphics engine
namespace geoGE {

GUISSA::GUISSA(const GUIScene* pFIR)
        : m_pFIR(pFIR), m_iPenWidth(1)
{
    setZValue( getZOrderValue(ZOrderSSA) );
//    SetColor(Qt::lightGray);
    SetColor( UserSettings::Accessor::instance().color(UserSettings::CtMap) );
}

QRectF GUISSA::boundingRect() const
{
	return GetPath().boundingRect();
}

/*QPainterPath GUISSA::shape() const
{
	return GetPath();
}*/

void GUISSA::paint(
	QPainter *painter,
	const QStyleOptionGraphicsItem *option,
	QWidget *widget)
{
	if (IsItemActive())
	{
        QPen qPen( GetColor() );
		/*if (GetType() == enSID)
			qPen.setColor(Qt::green);
		else
			qPen.setColor(Qt::blue);*/
        qPen.setCosmetic(true);
		qPen.setWidth(m_iPenWidth);
		painter->setPen(qPen);
		painter->drawPath(GetPath());
	}
}

bool GUISSA::IsTransformNeeded() const
{
    return true;
}

void GUISSA::SetPosition(const TCoordSystem &cs)
{
//    return;

//        if (GetName() != ("GOVI4A"))
//            return;
//        if (GetName() != ("ROMA3A"))
//            return;
//        if ( GetID() != 19917 )
//            return;

    prepareGeometryChange();

    try
    {
        if(GetEnable())
        {
            qreal angleCorrection = 0.0;
            // Find correction angle to the north direction of current FIR.
            if ( m_pFIR )
            {
                qreal b, l;
                // Retrieve center point of a FIR (scene)
                m_pFIR->Prepare(b, l);
                angleCorrection = cs->findAngleToNorth(b, l);
            }

            CalculatePath(cs, false, angleCorrection);
        }
    }
    catch ( QString &ex )
    {
        SetEnable(false);
        update();

        throw QString("SSA '%1': %2").arg(GetName()).arg(ex);
    }

    update();
}

/*void GUISSA::mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event )
{
	QMessageBox::information(NULL, QObject::tr("SSA"), GetName());
	QGraphicsItem::mouseDoubleClickEvent(event);
}*/

void GUISSA::MarkItemActive(bool bActive)
{
    SSA::MarkItemActive(bActive);
	setVisible(bActive);
}

}   // namespace geoGE
