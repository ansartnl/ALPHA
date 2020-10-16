#include "SimpleComboboxDelegate.h"

#include <QComboBox>

SimpleComboboxDelegate::SimpleComboboxDelegate(const QStringList &list, QObject *parent) :
    QStyledItemDelegate(parent)
{
    mList = list;
}

QWidget * SimpleComboboxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &
                                               , const QModelIndex &) const
{
    QComboBox *combo = new QComboBox(parent);
    combo->addItems(mList);
    connect(combo, SIGNAL(activated(QString)), SLOT(commitAndCloseEditor()));
    return combo;
}

void SimpleComboboxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *combo = qobject_cast < QComboBox * >(editor);
    combo->setCurrentIndex(combo->findText(index.data().toString()));
}

void SimpleComboboxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model
                                          , const QModelIndex &index) const
{
    QComboBox *combo = qobject_cast < QComboBox * >(editor);
    model->setData(index, combo->currentText());
}

void SimpleComboboxDelegate::commitAndCloseEditor()
{
    QComboBox *combo = qobject_cast < QComboBox * >(sender());
    emit commitData(combo);
    emit closeEditor(combo);
}
