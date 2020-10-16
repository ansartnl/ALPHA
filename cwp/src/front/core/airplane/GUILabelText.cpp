#include "GUILabelText.h"
#include "main.h"

#include <QFont>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsGridLayout>
#include <QGraphicsLinearLayout>
#include <QPainter>

#include "src/front/core/GUIInterface.h"

CGUILabelText::CGUILabelText(QGraphicsItem *parent/* = 0*/)
	 : QGraphicsSimpleTextItem(parent), QGraphicsLayoutItem()
	 , m_Visible(true), m_bReadOnly(true), m_type(CGUIControlDlg::ListType)
     , bold_font_(false), isFrame(false)
 {
	 setGraphicsItem(this);
     setZValue(gui::Zlabelitem);
	 //setFlag(QGraphicsItem::ItemIsSelectable, true);
     setFlag(QGraphicsItem::ItemIsFocusable, true);
	 setAcceptedMouseButtons(Qt::NoButton);
	 setOpacity(1);
     setAcceptHoverEvents(true);
     UpdateXFont();
}

CGUILabelText::~CGUILabelText()
{

}

void CGUILabelText::setGeometry(const QRectF &geom)
{
	prepareGeometryChange();
	QGraphicsLayoutItem::setGeometry(geom);
	QPointF point(geom.topLeft());
	QGraphicsLayoutItem* pParentLayout = parentLayoutItem();
	if(pParentLayout)
	{
		//QGraphicsGridLayout* pGrid = dynamic_cast<QGraphicsGridLayout*>(pParentLayout);
        	QGraphicsLinearLayout* pGrid = dynamic_cast<QGraphicsLinearLayout*>(pParentLayout);
		if(pGrid)
		{
			Qt::Alignment alignment = pGrid->alignment(this);

			if(alignment & Qt::AlignLeft)
			{
			}
			if(alignment & Qt::AlignRight)
			{
				point = QPointF(geom.topRight());
				point.rx() -= boundingRect().width();
			}
			if(alignment & Qt::AlignHCenter)
			{
				point.rx() += geom.width()/2 - boundingRect().width()/2;
			}
		}
	}
	setPos(point);
}

QSizeF CGUILabelText::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
	switch (which)
	{
		case Qt::MinimumSize:
		case Qt::PreferredSize:
			return boundingRect().size();
		case Qt::MaximumSize:
			return QSize(1000, 1000);
		default:
			break;
	}
	return constraint;
 }

void CGUILabelText::updateGeometry()
{
	prepareGeometryChange();
	QGraphicsLayoutItem::updateGeometry();
}

void CGUILabelText::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (background_color_.isValid()) {
        painter->setPen(background_color_);
        painter->setBrush(background_color_);
        painter->drawRect(boundingRect());
    }
    if(isFrame)
    {
        painter->setPen(Qt::black);
        painter->drawRect(boundingRect());
    }
    QFont item_font(font());
    item_font.setBold(bold_font_);
    setFont(item_font);
    QGraphicsSimpleTextItem::paint(painter, option, widget);
}

QRectF CGUILabelText::boundingRect() const
{
    if (!m_boundingRect.isNull())
        return m_boundingRect;

    return QGraphicsSimpleTextItem::boundingRect();
}

void CGUILabelText::OnChoose()
{
	if (m_bReadOnly || !m_Visible)
        return;

    QWidget *viewport = scene()->views().first()->viewport();

    CGUIControlDlg *control_dlg = new CGUIControlDlg(m_type);

    control_dlg->SetACID(m_ACID);
    control_dlg->SetCODE(m_code);
	control_dlg->SetValues(m_sValues1, m_sValues2);
	control_dlg->Init(m_sCurSel.isEmpty() ? text() : m_sCurSel);
    control_dlg->Show(QCursor::pos(), QRect(viewport->mapToGlobal(QPoint(0, 0)), viewport->size()));
	connect(control_dlg, SIGNAL(itemChanged(const QStringList)),
					this, SLOT(OnItemChanged(const QStringList)));
    connect(control_dlg, SIGNAL(destroyed(QObject*)), this, SIGNAL(dialogClosed()));

    emit dialogOpened();
}

void CGUILabelText::OnItemChanged(const QStringList sValues)
{
    emit endEdit(this, sValues);
}

void CGUILabelText::setText(const QString& text)
{
	QString sText(text);
	if(sText.isEmpty()) sText = " "; //Pasha
	if(sText == "00" || /*sText == "000" ||*/ sText == "0000") sText = "0";
	QGraphicsSimpleTextItem::setText(sText);
	updateGeometry();
}

void CGUILabelText::SetTextColor(const QColor &color)
{
	setBrush(color);
}

void CGUILabelText::UpdateXFont()
{
    QFont font1 = font();
    font1.setPointSize(AppSettings::GetTextSize().GetCurrentTextSize());
    setFont(font1);
}

void CGUILabelText::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    setFocus();
    background_color_old = background_color_;
    setBackgroundColor(qRgb(255, 250, 205));
    QGraphicsSimpleTextItem::hoverEnterEvent(event);
}

void CGUILabelText::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    clearFocus();
    background_color_ = background_color_old;
    QGraphicsSimpleTextItem::hoverLeaveEvent(event);
}

void CGUILabelText::focusOutEvent(QFocusEvent* event)
{
    clearFocus();
    background_color_ = background_color_old;
    QGraphicsSimpleTextItem::focusOutEvent(event);
}

void CGUILabelText::setFrame()
{
    isFrame = true;
}

void CGUILabelText::setBoundingRect(const QRectF &rect)
{
    prepareGeometryChange();
    m_boundingRect = rect;
}
