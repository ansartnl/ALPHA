#include "ListACHeaderView.h"
#include "ListACModel.h"
#include <QPainter>
#include <QHoverEvent>

CListACHeaderView::CListACHeaderView(Qt::Orientation orientation, QWidget *parent)
    :QHeaderView(orientation, parent),
      isScrolled(false),
      m_pHeaders(0),
      hoverIndex(-1)
{
    sPlanType = QString("");
}

void CListACHeaderView::setHeaders(QStringList headers)
{
    if(m_pHeaders)
        delete m_pHeaders;
    m_pHeaders = new QStringList(headers);
}

void CListACHeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    if(!m_pHeaders){
        QHeaderView::paintSection(painter, rect, logicalIndex);
        return;
    }

    if(hoverIndex == logicalIndex)
        painter->fillRect(rect, Qt::gray);
    else
        painter->fillRect(rect, Qt::white);

    painter->setPen(Qt::lightGray);
    painter->drawLine(rect.x(), rect.y()+rect.height()-1, rect.x()+rect.width(), rect.y()+rect.height()-1);
    painter->drawLine(rect.x()+rect.width()-1, rect.y(), rect.x()+rect.width()-1, rect.y()+rect.height());

    painter->setPen(Qt::black);
    if(logicalIndex < m_pHeaders->count()){        
        if(logicalIndex == CListACModel::Scroll){
            QPixmap pixmap(isScrolled ? ":/Arrows/RightArrow" : ":/Arrows/LeftArrow");
            int _x = rect.x() + (rect.width() - pixmap.width()) / 2;
            int _y = rect.y() + (rect.height() - pixmap.height()) / 2;
            painter->drawPixmap(_x, _y, pixmap);            
        }
        else{
            painter->drawText(rect, Qt::AlignCenter, m_pHeaders->at(logicalIndex));                        
        }
    }
}

bool CListACHeaderView::event(QEvent* event)
{
    switch(event->type())
    {
    case QEvent::HoverEnter:
        {
            QHoverEvent* hover = static_cast<QHoverEvent*>(event);
            hoverIndex = logicalIndexAt(hover->pos());
            break;
        }
    case QEvent::HoverLeave:
        {
            hoverIndex = -1;
            break;
        }
    case QEvent::HoverMove:
        {
            QHoverEvent* hover = static_cast<QHoverEvent*>(event);
            hoverIndex = logicalIndexAt(hover->pos());
            break;
        }    
    default:
        break;
    }
    return QHeaderView::event(event);
}

QSize CListACHeaderView::sectionSizeFromContents(int logicalIndex) const
{
    if(!m_pHeaders){
        return QHeaderView::sectionSizeFromContents(logicalIndex);
    }

    Q_ASSERT(logicalIndex >= 0);

    ensurePolished();

    QStyleOptionHeader opt;
    initStyleOption(&opt);
    opt.section = logicalIndex;
    QVariant var = QVariant(QFont());
    QFont fnt;
    if (var.isValid() && qVariantCanConvert<QFont>(var))
        fnt = qvariant_cast<QFont>(var);
    else
        fnt = font();
    fnt.setBold(true);
    opt.fontMetrics = QFontMetrics(fnt);

    opt.text = QString();
    opt.icon = qvariant_cast<QIcon>(QVariant());
    if(logicalIndex == CListACModel::Scroll)
        opt.icon = QPixmap(isScrolled ? ":/Arrows/RightArrow" : ":/Arrows/LeftArrow");
    else
        opt.text = m_pHeaders->at(logicalIndex);

    QSize size = style()->sizeFromContents(QStyle::CT_HeaderSection, &opt, QSize(), this);
    if (isSortIndicatorShown()) {
        int margin = style()->pixelMetric(QStyle::PM_HeaderMargin, &opt, this);
        size.rwidth() += size.height() + margin;
    }
    return size;
}

