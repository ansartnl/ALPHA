#ifndef _COMMAND_DELEGATE_H__
#define _COMMAND_DELEGATE_H__

#include <QtGui/QItemDelegate>


namespace Uim {

//! View delegate for command items.
class CommandDelegate : public QItemDelegate
{
public:
    explicit CommandDelegate(QObject *parent = 0)
        : QItemDelegate(parent)
    {}

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const
    {
        QSize sz = QItemDelegate::sizeHint(option, index);
        int heightFont = option.fontMetrics.height();
        // Set height of the row not less that 24 pixels.
        const int heightRow = 24;
        sz.rheight() = heightFont < heightRow ? heightRow : heightFont;
        return sz;
    }
};

}   // namespace Uim

#endif // _COMMAND_DELEGATE_H__
