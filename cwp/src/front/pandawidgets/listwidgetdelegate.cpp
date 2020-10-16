#include "listwidgetdelegate.h"

ListWidgetDelegate::ListWidgetDelegate(QObject *parent) :
    QAbstractItemDelegate(parent)
{

}

ListWidgetDelegate::~ListWidgetDelegate()
{

}

void ListWidgetDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QRect r = option.rect;

    QPen fontPen(Qt::black, 1, Qt::SolidLine);

    QBrush brush = Qt::white;
    if (option.state & QStyle::State_Selected) {
        brush = Qt::gray;
    } else {
        if (index.data(Qt::UserRole).toBool()) {
            brush = QColor(0, 0, 255, 128);
        }
        else if (!index.data(Qt::UserRole + 1).isValid()) { // separator
            brush = Qt::darkGray;
        }
    }

    painter->setBrush(brush);

    painter->drawRect(r);

    painter->setPen(fontPen);

    QString title = index.data(Qt::DisplayRole).toString();


    painter->setFont(QFont("Courier", 10, QFont::Normal));
    painter->drawText(r.left() + 10, r.top(), r.width(), r.height(), Qt::AlignVCenter | Qt::AlignLeft, title, &r);
}

QSize ListWidgetDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    QVariant role = index.data(Qt::SizeHintRole);
    if (role.isValid())
        return role.toSize();

    return QSize(160, 30);
}







