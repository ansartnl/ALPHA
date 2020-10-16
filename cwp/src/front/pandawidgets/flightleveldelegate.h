#ifndef FLIGHTLEVELDELEGATE_H
#define FLIGHTLEVELDELEGATE_H

#include <QStyledItemDelegate>


class FlightLevelDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit FlightLevelDelegate(QWidget *parent_widget, QObject *parent = 0);

    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    virtual void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;

private slots:
    void commitAndCloseEditor();

private:
    QWidget *parent_widget_;
    bool bAccepted_;
};

#endif
