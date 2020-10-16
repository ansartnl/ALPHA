#ifndef LISTWIDGETDELEGATE_H
#define LISTWIDGETDELEGATE_H

#include <QPainter>
#include <QAbstractItemDelegate>

class ListWidgetDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
public:

    explicit ListWidgetDelegate(QObject *parent = 0);

    ~ListWidgetDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

};

#endif // LISTWIDGETDELEGATE_H
