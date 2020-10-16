#include "XLabel.h"
#include <QtGui/QPainter>
#include <QGraphicsSceneMouseEvent>

#ifdef _MSC_VER
#define UTF8_QSTRING(str) QString::fromWCharArray(L##str)
#else
#define UTF8_QSTRING(str) QString::fromUtf8(str)
#endif

CXLabelItem::CXLabelItem(ILabelHandler* pLabel, XGridItem::LabelItemType type, QGraphicsItem* parent)
	: QGraphicsSimpleTextItem(parent), m_Type(type)
	, m_Foreground(Qt::black), m_Background(Qt::lightGray), m_Transparent(25)
	, m_pLabel(pLabel), m_bMoving(false)
{
	setFlag(ItemIsMovable, true);
	setAcceptedMouseButtons(Qt::LeftButton);
    QGraphicsSimpleTextItem::setBrush(m_Foreground);
    setZValue(10);
	Translate();
}

CXLabelItem::~CXLabelItem()
{

}

void CXLabelItem::Translate()
{
	switch (m_Type)
	{
	case XGridItem::ACID:
		setText(tr(" ACID "));
		break;
	case XGridItem::CODE:
		setText(tr(" CODE "));
		break;
	case XGridItem::LX:
		setText(tr(" LX "));
		break;
	case XGridItem::NS:
		setText(tr(" NS "));
		break;
	case XGridItem::AFL:
		setText(tr(" AFL "));
		break;
	case XGridItem::CFL:
		setText(tr(" CFL "));
		break;
	case XGridItem::TREND:
		setText(" " + UTF8_QSTRING("\u2191") + " ");
		break;
	case XGridItem::VRC:
		setText(tr(" VRC "));
		break;
	case XGridItem::HDG:
		setText(tr(" HDG "));
		break;
	case XGridItem::BANK:
		setText(tr(" BANK "));
		break;
	case XGridItem::NextPoint:
		setText(tr(" Next point "));
		break;
	case XGridItem::SPD1:
		setText(tr(" SPD1 "));
		break;
	case XGridItem::SPD2:
		setText(tr(" SPD2 "));
		break;
	case XGridItem::V:
		setText(tr(" V "));
		break;
	case XGridItem::GOV:
		setText(tr(" GOV "));
		break;
	case XGridItem::FUEL:
		setText(tr(" FUEL "));
		break;
    case XGridItem::TFL:
        setText(tr(" TFL "));
        break;
    case XGridItem::HDG2:
        setText(tr(" HDG2 "));
        break;
    case XGridItem::UVD:
        setText(tr(" UVD "));
        break;
    case XGridItem::PEL:
        setText(tr(" PEL "));
        break;
    case XGridItem::RFL:
        setText(tr(" RFL "));
        break;
    case XGridItem::AZIMUTH:
        setText(tr(" Azimuth "));
        break;
    case XGridItem::DISTANCE:
        setText(tr(" Distance "));
        break;
    case XGridItem::QNH:
        setText(tr(" QNH "));
        break;
    case XGridItem::SPD:
        setText(tr(" SPD "));
        break;
    case XGridItem::GSP:
        setText(tr(" GSP "));
        break;
    case XGridItem::W:
        setText(tr(" W "));
        break;
    case XGridItem::None:
		break;
	}
}

void CXLabelItem::paint(
		QPainter *painter,
		const QStyleOptionGraphicsItem *option,
		QWidget *widget)
{
    painter->save();
    QRectF rect = boundingRect();
    rect.adjust(1,1,-1,-1);
    QColor colorBackground(m_Background);
    int iVal = m_Transparent * 2.55;
    iVal %= 256;
    colorBackground.setAlpha(255 - iVal);
    painter->setBrush(QBrush(colorBackground));
    painter->drawRect(rect);
    painter->restore();

	QGraphicsSimpleTextItem::paint(painter, option, widget);
}

void CXLabelItem::SetForeground(const QColor &V)
{
    m_Foreground = V;
    QGraphicsSimpleTextItem::setBrush(m_Foreground);
}

QVariant CXLabelItem::itemChange(GraphicsItemChange change,
						  const QVariant& value)
{
	if(change == QGraphicsItem::ItemPositionChange)
		m_bMoving = true;
	return QGraphicsSimpleTextItem::itemChange(change, value);
}

void CXLabelItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	m_bMoving = false;
	QGraphicsSimpleTextItem::mousePressEvent(event);
}

void CXLabelItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsSimpleTextItem::mouseReleaseEvent(event);
	if(/*m_bMoving &&*/(event->button() == Qt::LeftButton))
	{
		if(m_pLabel) m_pLabel->SetPos(this, event->scenePos());
	}
}
