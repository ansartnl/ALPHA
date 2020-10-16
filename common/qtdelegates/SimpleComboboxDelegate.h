#ifndef SIMPLECOMBOBOXDELEGATE_H
#define SIMPLECOMBOBOXDELEGATE_H

#include <QStyledItemDelegate>

/*!
  * \class SimpleComboboxDelegate
  * \brief Делегат выпадающего списка
  *
  * Класс, реализующий отрисовку и поведение выпадающего списка таблицы.
  */

class SimpleComboboxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit SimpleComboboxDelegate(const QStringList &list, QObject *parent = 0);

    QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

protected slots:
    void commitAndCloseEditor();

protected:
    QStringList mList;
};

#endif // SIMPLECOMBOBOXDELEGATE_H
