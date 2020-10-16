#ifndef TIMEDELEGATE_H
#define TIMEDELEGATE_H

#include <QStyledItemDelegate>

class TimeDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit TimeDelegate(QObject *parent = 0);

    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    virtual void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:

};

#endif
