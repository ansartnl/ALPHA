#include "GUIReminder.h"
#include "GUILabelText.h"
#include <QtGui/QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLinearLayout>
#include "back/core/Airplane.h"
#include "main.h"

CGUIReminder::CGUIReminder(QGraphicsItem * parent/* = 0*/)
	: QGraphicsWidget(parent)
	, m_iPenWidth(1), m_color(Qt::black), m_backgroundBrush(Qt::lightGray)
    , m_iCurIndex(0), m_pPlane(0)
{
	setFlag(ItemDoesntPropagateOpacityToChildren, true);
	setAcceptedMouseButtons(Qt::LeftButton);
    //setOpacity(0.75);
    setOpacity(1);
    setVisible(false);

	m_itemText = new CGUILabelText(this);
	m_itemText->setText(" ");
	m_itemText->SetTextColor(Qt::darkRed);

	m_itemClose = new CGUILabelText(this);
	m_itemClose->setText("[X]");
	//m_itemClose->SetTextColor(Qt::darkRed);

	QGraphicsLinearLayout* pLayout = new QGraphicsLinearLayout(Qt::Horizontal, this);
	pLayout->setSpacing(5);
	pLayout->setContentsMargins(2,2,2,2);

	pLayout->addItem(m_itemText);
	pLayout->setStretchFactor(m_itemText, 1);
	pLayout->addItem(m_itemClose);
	setLayout(pLayout);
	adjustGrid();

	setPos(0, -m_clRect.height()-m_iPenWidth);
}

CGUIReminder::~CGUIReminder()
{

}

QRectF CGUIReminder::boundingRect() const
{
	return m_clRect.adjusted(-m_iPenWidth,-m_iPenWidth,m_iPenWidth,m_iPenWidth);
}

QPainterPath CGUIReminder::shape() const
{
	QPainterPath paintPath;
	paintPath.addRect(m_clRect);
	return paintPath;
}

void CGUIReminder::paint(
		QPainter *painter,
		const QStyleOptionGraphicsItem *option,
		QWidget *widget)
{
	painter->save();
	QPen qPen(m_color);
	qPen.setWidth(m_iPenWidth);
	painter->setPen(qPen);
    //painter->setBrush(m_backgroundBrush);
    painter->setBrush(Qt::NoBrush);
    if(globalSettings()->value("XMasterWindow/Settings/ShowFormularBorder", 1).toInt() & 2)
        painter->drawRect(m_clRect);
	painter->restore();
}

void CGUIReminder::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsWidget::mousePressEvent(event);

	if(event->button() == Qt::LeftButton)
	{
		QPointF point = event->pos();
		QRectF rect = m_itemClose->boundingRect();
		rect = m_itemClose->mapRectToParent(rect);
		if(rect.contains(point))
		{
            if(m_listReminder[m_iCurIndex]->IsClosable())
            {
                //delete reminder
                if(m_pPlane && (m_itemClose->text() == "[X]"))
                {
                    if(m_pPlane->CanRemoveReminder(m_listReminder[m_iCurIndex]))
                    {
                        m_pPlane->RemoveReminder(m_listReminder[m_iCurIndex]);
                        m_listReminder.removeAt(m_iCurIndex);
                        SetReminderText(--m_iCurIndex);
                    }
                }
                event->accept();
                return;
            }
		}
        //changeReminder
		SetReminderText(++m_iCurIndex);
		event->accept();
	}
}

void CGUIReminder::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsWidget::mouseDoubleClickEvent(event);
    event->accept();

    if(event->button() == Qt::LeftButton)
    {
        QPointF point = event->pos();
        QRectF rect = m_itemClose->boundingRect();
        rect = m_itemClose->mapRectToParent(rect);
        if(rect.contains(point))
        {
            if(m_listReminder[m_iCurIndex]->IsClosable())
            {
                //delete reminder
                if(m_pPlane && (m_itemClose->text() != "[X]"))
                {
                    if(m_pPlane->CanRemoveReminder(m_listReminder[m_iCurIndex]))
                    {
                        m_pPlane->RemoveReminder(m_listReminder[m_iCurIndex]);
                        m_listReminder.removeAt(m_iCurIndex);
                        SetReminderText(--m_iCurIndex);
                    }
                }
            }
        }
    }
}

void CGUIReminder::adjustGrid()
{
	prepareGeometryChange();
	if(parentItem())
	{
		QGraphicsWidget* pWnd = dynamic_cast<QGraphicsWidget*>(parentItem());
		if(pWnd) resize(pWnd->rect().width(), rect().height());
	}
	layout()->invalidate();
	layout()->activate();

	QRectF clRect = rect();
	clRect.adjust(-1,-1,1,1);
	m_clRect = clRect.normalized();
	update();
}

void CGUIReminder::SetReminders(const CReminder::TListReminder& list)
{
	CReminder::TListReminder listTemp;
	for(int i = 0; i < list.size(); ++i)
	{
		if(list[i]->GetComplete())
			listTemp << list[i];
	}
	bool bEquival(listTemp.size() == m_listReminder.size());
	for(int i = 0; bEquival && i < listTemp.size(); ++i)
	{
		bEquival = false;
		for(int j = 0; j < m_listReminder.size(); ++j)
		{
			if(*(listTemp[i].data()) == *(m_listReminder[j].data()))
			{
				bEquival = true;
				break;
			}
		}
	}
	if(!bEquival)
	{
		m_listReminder.clear();
		m_listReminder.append(listTemp);
		qSort(m_listReminder.begin(), m_listReminder.end(), CReminder::LessReminder);
		SetReminderText(m_listReminder.size()-1);
	}
}

void CGUIReminder::SetReminderText(int index)
{
	if(m_listReminder.size())
	{
		index = qBound(0, index, m_listReminder.size());
		index %= m_listReminder.size();
		m_iCurIndex = index;
		CReminder::TReminder& pReminder = m_listReminder[index];
        if(pReminder->IsClosable())
            m_itemClose->setText("[X]");
        else
            m_itemClose->setText(" ");
        if(pReminder->GetType() == CReminder::Assume || pReminder->GetType() == CReminder::Transfer
           || pReminder->GetType() == CReminder::TrHOP || pReminder->GetType() == CReminder::TrROF
           || pReminder->GetType() == CReminder::Transfer_Com || pReminder->GetType() == CReminder::TimeOutROF)
        {
            m_itemClose->setText(pReminder->GetValue());
        }
        if(m_itemClose->text().isEmpty()) m_itemClose->setText("?");

        QString sText;
		if(m_listReminder.size() > 1)
        {
            QString sTemp;
            CReminder::TListReminder::const_iterator itReminder = m_listReminder.constBegin();
            for(; itReminder != m_listReminder.constEnd(); ++itReminder)
            {
                if((*itReminder)->GetLevel() == CReminder::Alarm)
                {
                    sTemp = "!";
                    break;
                }
            }

            //sText += "(" + sTemp + QString::number(index+1) + " )";
            sText += "(" + sTemp + QString::number(m_listReminder.size()) + " )";
        }
		sText += GetShortName(pReminder);
		//try cut text
		int othersWidth = m_itemClose->boundingRect().width() + 5 + 2 + 2;
		int freeWidth(0);
		if(parentItem())
		{
			QGraphicsWidget* pWnd = dynamic_cast<QGraphicsWidget*>(parentItem());
			if(pWnd) freeWidth = pWnd->rect().width();
		}
		while(sText.length() > 3)
		{
			m_itemText->setText(sText);
			if(m_itemText->boundingRect().width() <= freeWidth-othersWidth) break;
			sText.chop(4);
			sText += "...";
		}
		////////////////////
		m_itemText->setText(sText);
        switch(pReminder->GetLevel())
        {
        case CReminder::Normal:
            m_itemText->SetTextColor(Qt::white);
            m_itemClose->SetTextColor(Qt::white);
            break;
        case CReminder::Warning:
            m_itemText->SetTextColor(Qt::yellow);
            m_itemClose->SetTextColor(Qt::yellow);
            break;
        case CReminder::Alarm:
            m_itemText->SetTextColor(Qt::darkRed);
            m_itemClose->SetTextColor(Qt::darkRed);
            break;
        }
        adjustGrid();
		setVisible(true);
	}
	else
		setVisible(false);
}

void CGUIReminder::UpdateWidth()
{
	if(isVisible())
		SetReminderText(m_iCurIndex);
}

QString CGUIReminder::GetShortName(CReminder::TReminder& pReminder)
{
	QString sText;
	switch(pReminder->GetType())
	{
    case CReminder::Airport:
    case CReminder::SpecCodes:
    case CReminder::SPI:
    case CReminder::TRACK:
        sText = pReminder->GetValue();
		break;
    case CReminder::FL:
        sText = tr("FL") + " " + pReminder->GetValue();
        break;
    case CReminder::PassFL:
        //sText = tr("Passing FL");// + " " + pReminder->GetValue();
        sText = tr("PFL");// + " " + pReminder->GetValue();
        break;
    case CReminder::Local:
		sText = tr("Landing");
		break;
	case CReminder::Point:
		{
			QStringList sList = pReminder->GetValue().split(" ", QString::SkipEmptyParts);
			sText = sList.size() > 2 ? sList[2] : QString::null;
			break;
		}
    case CReminder::Assume:
    case CReminder::Transfer:
    case CReminder::Transfer_Com:
    case CReminder::TrHOP:
    case CReminder::TrROF:
    case CReminder::TimeOutROF:
    case CReminder::LeaveRoute:
    case CReminder::DoubleCode:
        sText = pReminder->GetReminderName(pReminder->GetType());
        break;
    default:
        sText = pReminder->GetReminderName(pReminder->GetType()) + " " + pReminder->GetValue();
		break;
	}
	return sText;
}

void CGUIReminder::SetPlane(CAirplane *pPlane)
{
	m_pPlane = pPlane;
}

void CGUIReminder::Translate()
{
    if(isVisible())
        SetReminderText(m_iCurIndex);
}
