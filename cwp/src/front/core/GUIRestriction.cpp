#include "GUIRestriction.h"
#include <QtGui/QPainter>
#include <QtCore/QTimer>
#include <QtCore/QTextStream>
#include <math.h>
#include "back/utils/ProfileSettings.h"
#include "front/common/XAppSettings.h"

CGUIRestriction::CGUIRestriction(QGraphicsItem *parent)
    : QGraphicsItem(parent), m_iPenWidth(1), m_color(Qt::red)
{
    setZValue(gui::Zrestriction);
	setOpacity(0.15);
}

CGUIRestriction::~CGUIRestriction()
{

}

QRectF CGUIRestriction::boundingRect() const
{
	return m_painterPath.boundingRect();
}

QPainterPath CGUIRestriction::shape() const
{
	return m_painterPath;
}

void CGUIRestriction::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if ((GetType() == enDanger) || IsItemActive())
    {
            painter->save();
            QString sText;
            switch(GetType())
            {
            case enAlert:
                    sText = QObject::tr("AA");
                    break;
            case enCaution:
                    sText = QObject::tr("CA");
                    break;
            case enDanger:
                    break;
            case enMilitary:
                    sText = QObject::tr("MA");
                    break;
            case enProhibited:
                    sText = QObject::tr("PA");
                    break;
            case enRestricted:
                    sText = QObject::tr("RA");
                    break;
            case enTraining:
                    sText = QObject::tr("TA");
                    break;
            case enWarning:
                    sText = QObject::tr("WA");
                    break;
            case enUnknown:
                    sText = QObject::tr("U");
                    break;
            default:
                    qWarning("unknow CRestriction::Type");
            }

            QColor colorRestr = m_color;
            QColor colorTemp = UserProfile::CSettings::Instance().GetColor(UserProfile::CtRestr);
            if (colorTemp.isValid())
                colorRestr = colorTemp;

            QPen qPen(colorRestr);
            qPen.setWidth(m_iPenWidth);
            painter->setPen(qPen);
            painter->setBrush(QBrush(colorRestr, Qt::SolidPattern));
            painter->drawPath(m_painterPath);
            painter->setOpacity(0.5);
            QFont font = painter->font();
            font.setPointSize(AppSettings::GetTextSize().GetCurrentTextSize());
            painter->setFont(font);
            painter->drawText(boundingRect(), Qt::AlignCenter, sText);
            painter->restore();
    }
}

void CGUIRestriction::SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord& centralCoord)
{
	if ((GetType() == enDanger) || IsItemActive())
	{
		prepareGeometryChange();
		m_painterPath = QPainterPath();
		TPointList::const_iterator itScan = GetPoints().begin();
		for(; itScan != GetPoints().end(); ++itScan)
		{
			QPointF point(
				((*itScan)->GetCoord().GetX() - qllXMin) * dScale,
				(qllYMax - (*itScan)->GetCoord().GetY()) * dScale);
			point = mapFromScene(point);
			QPointF pointArc(
				((*itScan)->GetArcCoord().GetX() - qllXMin) * dScale,
				(qllYMax - (*itScan)->GetArcCoord().GetY()) * dScale);
			pointArc = mapFromScene(pointArc);

			TPointList::const_iterator itScanNext = itScan+1;
			if(itScanNext == GetPoints().end()) itScanNext = GetPoints().begin();

			QPointF pointNext(
				((*itScanNext)->GetCoord().GetX() - qllXMin) * dScale,
				(qllYMax - (*itScanNext)->GetCoord().GetY()) * dScale);
			pointNext = mapFromScene(pointNext);

			qreal qRadius = (*itScan)->GetArcDistance() * dScale;

			if(itScan == GetPoints().begin()) m_painterPath.moveTo(point);

			switch((*itScan)->GetType())
			{
			case CRestrictPoint::enLine:
				m_painterPath.lineTo(pointNext);
				break;
			case CRestrictPoint::enCircle:
				m_painterPath.addEllipse(pointArc, qRadius, qRadius);
				break;
			case CRestrictPoint::enLeftArc:
			case CRestrictPoint::enRightArc:
				{
					QRectF rect(QPointF(pointArc.x()-qRadius, pointArc.y()-qRadius), QSizeF(2*qRadius, 2*qRadius));
					QLineF line1(pointArc, point);
					QLineF line2(pointArc, pointNext);
					if((*itScan)->GetType() == CRestrictPoint::enLeftArc)
						m_painterPath.arcTo(rect, line1.angle(), line1.angleTo(line2));
					else
						m_painterPath.arcTo(rect, line1.angle(), line1.angleTo(line2)-360);
				}
				break;
			default:
				qWarning("unknow CRestrictPoint::Type");
			}
		}
		update();
	}
}

void CGUIRestriction::MarkItemActive(bool bActive)
{
	CRestriction::MarkItemActive(bActive);
	if(GetType() != enDanger) setVisible(bActive);
}


////////////////////////////////////////////////////////////////////////////////////
// CGUIRestrictionUser implementation

CGUIRestrictionUser::CGUIRestrictionUser(QGraphicsItem *parent)
    : QGraphicsObject(parent), m_iPenWidth(1), m_color(QColor(Qt::red)),
      m_highlighted(false)
{
    setZValue(gui::Zuserrestriction);
    setOpacity(0.15);

    setAcceptHoverEvents(true);

    m_timer = new QTimer(this);
    m_timer->setSingleShot(true);
    connect(m_timer, SIGNAL(timeout()), SLOT(onTimeout()));
}

CGUIRestrictionUser::~CGUIRestrictionUser()
{
}

void CGUIRestrictionUser::updateVisibility()
{
    // Start timer or show restriction zone if it is in
    // the range of begin and end dateTimes.
    onTimeout();
}

QRectF CGUIRestrictionUser::boundingRect() const
{
    return m_painterPath.boundingRect();
}

QPainterPath CGUIRestrictionUser::shape() const
{
    return m_painterPath;
}

void CGUIRestrictionUser::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (!IsItemActive())
        return;
    if (GetType() == enNRI)
        return;

    QColor colorPen = m_color;
    QColor colorBrush = m_color;
    {
        QColor colorTemp = UserProfile::CSettings::Instance().GetColor(UserProfile::CtRestrUser);
        if(colorTemp.isValid())
        {
            colorPen = colorTemp;
            colorBrush = colorTemp;
        }
    }
    if ( enObstacle == GetType() )
    {
        colorPen = Qt::blue;
        colorBrush = Qt::blue;
        {
            QColor colorTemp = UserProfile::CSettings::Instance().GetColor(UserProfile::CtRestrUserObstacle);
            if(colorTemp.isValid())
            {
                colorPen = colorTemp;
                colorBrush = colorTemp;
            }
        }
    }
    else if ( enNFZ == GetType() )
    {
        colorPen = Qt::yellow;
        colorBrush = Qt::yellow;
        {
            QColor colorTemp = UserProfile::CSettings::Instance().GetColor(UserProfile::CtRestrUserNFZ);
            if(colorTemp.isValid())
            {
                colorPen = colorTemp;
                colorBrush = colorTemp;
            }
        }
    }
    else if ( enMap == GetType() )
    {
        colorPen = Qt::yellow;
        colorBrush = Qt::transparent;
        m_iPenWidth = 3;
        {
            QColor colorTemp = UserProfile::CSettings::Instance().GetColor(UserProfile::CtRestrUserMap);
            if(colorTemp.isValid())
            {
                colorPen = colorTemp;
            }
        }
    }

    if (m_highlighted)
        colorPen = Qt::green;

    painter->save();
    QString sText;
    switch(GetType())
    {
    case enObstacle:
            sText = QObject::tr("MSAW");
            break;
    case enNFZ:
            sText = QObject::tr("NFZ");
            break;
    case enNRI:
            sText = QObject::tr("NRI");
            break;
    case enAlert:
            sText = QObject::tr("AA");
            break;
    case enCaution:
            sText = QObject::tr("CA");
            break;
    case enDanger:
            break;
    case enMilitary:
            sText = QObject::tr("MA");
            break;
    case enProhibited:
            sText = QObject::tr("PA");
            break;
    case enRestricted:
            sText = QObject::tr("RA");
            break;
    case enTraining:
            sText = QObject::tr("TA");
            break;
    case enWarning:
            sText = QObject::tr("WA");
            break;
    case enUnknown:
            sText = QObject::tr("U");
            break;
    case enMap:
            sText = QObject::tr("MAP");
            break;
    default:
            qWarning("unknow CRestrictionUser::Type");
    }

    QPen qPen(colorPen);
    qPen.setWidth(m_iPenWidth);
    painter->setPen(qPen);
    painter->setBrush(QBrush(colorBrush, Qt::SolidPattern));
    painter->drawPath(m_painterPath);
    painter->setOpacity(0.5);
    QFont font = painter->font();
    font.setPointSize(AppSettings::GetTextSize().GetCurrentTextSize());
    painter->setFont(font);
    painter->drawText(boundingRect(), Qt::AlignCenter, sText);
    painter->restore();
}

void CGUIRestrictionUser::SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord& centralCoord)
{
    if (!IsItemActive())
        return;
    if (GetType() == enNRI)
        return;

    prepareGeometryChange();
    m_painterPath = QPainterPath();
    TPointList::const_iterator itScan = GetPoints().begin();
    for (; itScan != GetPoints().end(); ++itScan)
    {
            QPointF point(
                    ((*itScan)->GetCoord().GetX() - qllXMin) * dScale,
                    (qllYMax - (*itScan)->GetCoord().GetY()) * dScale);
            point = mapFromScene(point);
            QPointF pointArc(
                    ((*itScan)->GetArcCoord().GetX() - qllXMin) * dScale,
                    (qllYMax - (*itScan)->GetArcCoord().GetY()) * dScale);
            pointArc = mapFromScene(pointArc);

            TPointList::const_iterator itScanNext = itScan + 1;
            if (itScanNext == GetPoints().end())
                itScanNext = GetPoints().begin();

            QPointF pointNext(
                    ((*itScanNext)->GetCoord().GetX() - qllXMin) * dScale,
                    (qllYMax - (*itScanNext)->GetCoord().GetY()) * dScale);
            pointNext = mapFromScene(pointNext);

            qreal qRadius = (*itScan)->GetArcDistance() * dScale;

            if (itScan == GetPoints().begin())
                m_painterPath.moveTo(point);

            switch((*itScan)->GetType())
            {
            case CRestrictPoint::enLine:
                    m_painterPath.lineTo(pointNext);
                    break;
            case CRestrictPoint::enCircle:
                    m_painterPath.addEllipse(pointArc, qRadius, qRadius);
                    break;
            case CRestrictPoint::enLeftArc:
            case CRestrictPoint::enRightArc:
                    {
                            QRectF rect(QPointF(pointArc.x()-qRadius, pointArc.y()-qRadius), QSizeF(2*qRadius, 2*qRadius));
                            QLineF line1(pointArc, point);
                            QLineF line2(pointArc, pointNext);
                            if ((*itScan)->GetType() == CRestrictPoint::enLeftArc)
                                    m_painterPath.arcTo(rect, line1.angle(), line1.angleTo(line2));
                            else
                                    m_painterPath.arcTo(rect, line1.angle(), line1.angleTo(line2)-360);
                    }
                    break;
            default:
                    qWarning("unknow CRestrictPoint::Type");
            }
    }

    update();
}

void CGUIRestrictionUser::MarkItemActive(bool bActive)
{
    CRestrictionUser::MarkItemActive(bActive);
    //setVisible(bActive);
}

void CGUIRestrictionUser::highlight(bool on, int interval)
{
    if (m_highlighted == on)
        return;

    m_highlighted = on;
    update();

    if ( interval > 0 )
        QTimer::singleShot(interval, this, SLOT(on_highlight_stop()));
}

void CGUIRestrictionUser::on_highlight_stop()
{
    m_highlighted = false;
    update();
}

void CGUIRestrictionUser::onTimeout()
{
    m_timer->stop();

    const Qt::CheckState actual_value = actual();
    if (Qt::Unchecked == actual_value) {
        setVisible(false);
        return;
    } else if (Qt::Checked == actual_value) {
        setVisible(true);
        return;
    }

    if (GetType() == enMap) {
        setVisible(true);
        return;
    }

    QDateTime currentDT = QDateTime::currentDateTime();

    // Hide restriction zone if current time is less than start working zone time.

    int days = currentDT.daysTo( GetTimeBegin() );
    if (days > 20)
    {
        m_timer->start(20*24*3600*1000);
        setVisible(false);
        return;
    }
    else
    {
        int secs = currentDT.secsTo( GetTimeBegin() );
        if (secs > 0)
        {
            m_timer->start(secs*1000);
            setVisible(false);
            return;
        }
    }

    // Show restriction zone if current time is in range of working zone time.

    if (GetTimeEnd().isNull())
    {
        setVisible(true);
        return;
    }

    days = currentDT.daysTo( GetTimeEnd() );
    if (days > 20)
    {
        m_timer->start(20*24*3600*1000);
        setVisible(true);
        return;
    }
    else
    {
        int secs = currentDT.secsTo( GetTimeEnd() );
        if (secs > 0)
        {
            m_timer->start(secs*1000);
            setVisible(true);
            return;
        }

        // Current dateTime should not be bigger than end time or
        // restriction zone does not work any more.
        setVisible(false);
    }
}

void CGUIRestrictionUser::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QString tooltipString;
    if (GetType() == enMap)
    {
        tooltipString = GetName();
    }
    else
    {
        if(CRestrictionUser::actual() == 1)
        {
            QTextStream stream(&tooltipString);
            stream << tr("%1").arg(GetName()) << endl
                   << tr("%1 - %2 ft").arg(GetHMin() / 0.3048).arg(GetHMax() / 0.3048) << endl
                   << tr("%1 - %2").arg(GetTimeBegin().toString("hh:mm")).arg(GetTimeEnd().toString("hh:mm"));
        }
        if(CRestrictionUser::actual() == 2)
        {
            QTextStream stream(&tooltipString);
            stream << tr("%1").arg(GetName()) << endl
                   << tr("%1 - %2 ft").arg(GetHMin() / 0.3048).arg(GetHMax() / 0.3048);
        }
    }
    setToolTip(tooltipString);

    QGraphicsObject::hoverEnterEvent(event);
}
