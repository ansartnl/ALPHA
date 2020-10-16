#include "timedelegate.h"

#include <QTimeEdit>
#include <QTime>
const QTime limit_time(23, 0);

TimeDelegate::TimeDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

QWidget *TimeDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
{
    QTimeEdit *editor = new QTimeEdit(parent);
    editor->setFrame(false);
    editor->setDisplayFormat("hh:mm");
    QTime current_time = QTime::currentTime();
    //current_time = current_time.addSecs(1200);
    if (current_time < limit_time)
        editor->setMinimumTime(current_time);
    return editor;
}

void TimeDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QTime value = QTime::fromString(index.model()->data(index, Qt::EditRole).toString(), "hh:mm");
    QTimeEdit *time_edit = static_cast<QTimeEdit*>(editor);
    time_edit->setTime(value);
}

void TimeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QTimeEdit *time_edit = static_cast<QTimeEdit*>(editor);
    QString value = time_edit->text();
    model->setData(index, value, Qt::EditRole);
}

void TimeDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
