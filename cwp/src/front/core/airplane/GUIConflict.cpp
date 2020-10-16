#include "GUIConflict.h"
#include "GUILabelText.h"
#include <QtGui/QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLinearLayout>
#include "back/core/Airplane.h"
#include "main.h"

CGUIConflict::CGUIConflict(QGraphicsItem * parent/* = 0*/)
    : QGraphicsWidget(parent)
    , m_iPenWidth(1), m_color(Qt::black), m_backgroundBrush(Qt::lightGray)
    , m_iCurIndex(0), m_pPlane(0)
{
    setFlag(ItemDoesntPropagateOpacityToChildren, true);
    setAcceptedMouseButtons(Qt::LeftButton);
    setOpacity(1);
    setVisible(false);

    QGraphicsLinearLayout* pLayout = new QGraphicsLinearLayout(Qt::Vertical, this);
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);
    adjustGrid();

    setPos(0, -m_clRect.height()-m_iPenWidth);
}

CGUIConflict::~CGUIConflict()
{

}

QRectF CGUIConflict::boundingRect() const
{
    return m_clRect.adjusted(-m_iPenWidth,-m_iPenWidth,m_iPenWidth,m_iPenWidth);
}

QPainterPath CGUIConflict::shape() const
{
    QPainterPath paintPath;
    paintPath.addRect(m_clRect);
    return paintPath;
}

//void CGUIConflict::paint(
//        QPainter *painter,
//        const QStyleOptionGraphicsItem *option,
//        QWidget *widget)
//{
//    painter->save();
//    QPen qPen(m_color);
//    qPen.setWidth(m_iPenWidth);
//    painter->setPen(qPen);
//    //painter->setBrush(m_backgroundBrush);
//    painter->setBrush(Qt::NoBrush);
//    if(globalSettings()->value("XMasterWindow/Settings/ShowFormularBorder", 1).toInt() & 2)
//        painter->drawRect(m_clRect);
//    painter->restore();
//}

void CGUIConflict::adjustGrid()
{
    prepareGeometryChange();
    if(parentItem())
    {
        QGraphicsWidget* pWnd = dynamic_cast<QGraphicsWidget*>(parentItem());
        if(pWnd) resize(pWnd->rect().width(), 0);
    }
    layout()->invalidate();
    layout()->activate();

    QRectF clRect = rect();
    clRect.adjust(-1,-1,1,1);
    m_clRect = clRect.normalized();
    update();
}

void CGUIConflict::SetConflicts(QList<QPair<QString, QString> > list)
{
    if (m_Conflicts != list)
    {
        m_Conflicts = list;

        QList<QPair<QString, QString> > conflicts;
        for (int i = 0; i < m_Conflicts.size(); ++i)
        {
            QPair<QString, QString> item = m_Conflicts.at(i);

            if (item.first == "INFO" && conflicts.contains(qMakePair(QString("ALARM"), item.second)))
                continue;

            if (item.first == "ALARM")
                conflicts.removeAll(qMakePair(QString("INFO"), item.second));

            conflicts.append(item);
        }

        qDeleteAll(m_items);
        m_items.clear();
        QGraphicsLinearLayout* pLayout = dynamic_cast<QGraphicsLinearLayout*>(layout());

        int width = 0;
        if(parentItem())
        {
            QGraphicsWidget* pWnd = dynamic_cast<QGraphicsWidget*>(parentItem());
            if(pWnd) width = pWnd->rect().width();
        }

        for (int i = 0; i < conflicts.size(); ++i)
        {
            CGUILabelText *item = new CGUILabelText(this);
            item->setText(conflicts[i].second);

            width = qMax(width, static_cast<int>(item->boundingRect().width()));

            if (conflicts[i].first == "ALARM")
            {
                item->SetTextColor("#FFFFFF");
                item->setBackgroundColor("#C81A1A");
            }
            else
            {
                item->SetTextColor("#454545");
                item->setBackgroundColor("#FFC618");
            }
            pLayout->addItem(item);

            m_items.append(item);
        }

        // update items width
        foreach (CGUILabelText *item, m_items)
        {
            QRectF r = item->boundingRect();
            r.setWidth(width);
            item->setBoundingRect(r);
        }

        if (m_items.size() > 0)
        {
            adjustGrid();
            setPos(0, -m_clRect.height()-m_iPenWidth);
            setVisible(true);
        }
        else
        {
            setVisible(false);
        }
    }
}


void CGUIConflict::UpdateWidth()
{
//	if(isVisible())
//		SetReminderText(m_iCurIndex);
}


void CGUIConflict::SetPlane(CAirplane *pPlane)
{
    m_pPlane = pPlane;
}
