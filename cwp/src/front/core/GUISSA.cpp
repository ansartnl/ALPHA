#include "GUISSA.h"

#include "ProfileSettings.h"

#include "main.h"

#include <QtGui/QPen>
#include <QtGui/QPainter>
#include <QGraphicsScene>
#include <QMessageBox>

CGUISSA::CGUISSA()
		: m_iPenWidth(1)
{
    setZValue(gui::Zssa);
    SetColor( UserProfile::CSettings::Instance().GetColor(UserProfile::CtBlastoff));
}

QRectF CGUISSA::boundingRect() const
{
	return GetPath().boundingRect();
}

/*QPainterPath CGUISSA::shape() const
{
	return GetPath();
}*/

void CGUISSA::paint(
	QPainter *painter,
	const QStyleOptionGraphicsItem *option,
	QWidget *widget)
{
	if (IsItemActive())
	{
        if(GetEnable())
        {
            painter->save();
            QPen qPen( GetColor() );
            qPen.setWidth(m_iPenWidth);
            painter->setPen(qPen);
            painter->drawPath(GetPath());
            painter->restore();
        }
	}
}

void CGUISSA::SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord& centralCoord)
{
	if (IsItemActive())
	{
//        if((GetType() != CSSA::enSID) && (GetType() != CSSA::enSTAR))
//            return;

        TVA_CATCHALL_TRY
		{
			if(GetEnable())
			{
                CalculatePath(qllXMin, qllYMax, dScale, false, false);

				if (!GetPath().isEmpty())
				{
					setPos(GetPath().boundingRect().topLeft());
					GetPath().translate(-GetPath().boundingRect().topLeft());
				}
			}
		} TVA_CATCHALL(err)
		{
			err.setReason("SSA " + GetName());
			LOG_STR(Log4Qt::Level::ERROR_INT, err.reason());
			SetEnable(false);
		}
	}
}

void CGUISSA::MarkItemActive(bool bActive)
{
	CSSA::MarkItemActive(bActive);
	setVisible(bActive);
}
