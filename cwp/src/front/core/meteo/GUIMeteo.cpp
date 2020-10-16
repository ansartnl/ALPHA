#include "GUIMeteo.h"

#include <QPainter>

#include "front/common/XAppSettings.h"
#include "main.h"

CGUIMeteo::CGUIMeteo(QGraphicsItem *parent)
	: QGraphicsItem(parent), m_iPenWidth(1), m_color(Qt::blue)
{
    setZValue(gui::Zmeteo);
	setOpacity(0.15);

    setAcceptHoverEvents(true);
}

CGUIMeteo::~CGUIMeteo()
{

}

QRectF CGUIMeteo::boundingRect() const
{
	return m_painterPath.boundingRect();
}

QPainterPath CGUIMeteo::shape() const
{
	return m_painterPath;
}

void CGUIMeteo::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if ("no" == networkSettings()->value("users/draw_meteo", "no").toString())
        return;

	painter->save();
	QString sText;
	switch (GetType())
	{
	case Cloudburst:
		sText = QObject::tr("LVN");
		break;
	case LoStorm:
		sText = QObject::tr("(R)");
		break;
	case MedStorm:
		sText = QObject::tr("R)");
		break;
	case HiStorm:
		sText = QObject::tr("R");
		break;
	case Hail:
		sText = QObject::tr("G");
		break;
	case Squall:
		sText = QObject::tr("H00");
		break;
    case Snow:
        sText = QObject::tr("S");
        break;
    case Rain:
        sText = QObject::tr("N");
        break;
    case Fog:
        sText = QObject::tr("F");
        break;
	default:
		qWarning("unknow CMeteo::Type");
	}
	QPen qPen(m_color);
	qPen.setWidth(m_iPenWidth);
	painter->setPen(qPen);
	painter->setBrush(QBrush(m_color, Qt::SolidPattern));
	painter->drawPath(m_painterPath);
	painter->setOpacity(0.5);
    QFont font = painter->font();
    font.setPointSize(AppSettings::GetTextSize().GetCurrentTextSize());
    painter->setFont(font);
    painter->drawText(boundingRect(), Qt::AlignCenter, sText);
	painter->restore();
}

void CGUIMeteo::SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord& centralCoord)
{
	prepareGeometryChange();
	m_painterPath = QPainterPath();
	QPolygonF polygon;
	TPointList::const_iterator itScan = GetPoints().begin();
	for (; itScan != GetPoints().end(); ++itScan)
	{
        //Pasha
        if (!(*itScan)->GetCoord().IsNull() && !(*itScan)->GetCoord().IsCenter())
            (*itScan)->GetCoord().Center(centralCoord);
        ///////////////////
        QPointF point(
			((*itScan)->GetCoord().GetX() - qllXMin) * dScale,
			(qllYMax - (*itScan)->GetCoord().GetY()) * dScale);
		polygon << mapFromScene(point);
	}
	if (polygon.size())
	{
		if (!polygon.isClosed())
			polygon << polygon.first();
		m_painterPath.addPolygon(polygon);
	}
	update();
}

void CGUIMeteo::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QString toolTip;

    QTextStream stream(&toolTip);
    stream << GetName();
    if (IsCourse())
        stream << QString(" %1%2").arg(GetCourse()).arg(QString::fromUtf8("\u00B0"));
    if (IsSpeed())
        stream << QString(" %1km/h").arg(GetSpeed());

    if (IsHeightMin() && IsHeightMax())
    {
        stream << endl;
        stream << QString("%1 - %2 ft").arg(GetHeightMin() / 0.3048).arg(GetHeightMax() / 0.3048);
    }

    setToolTip(toolTip);

    QGraphicsItem::hoverEnterEvent(event);
}
