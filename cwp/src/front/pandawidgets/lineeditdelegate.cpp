#include "lineeditdelegate.h"

#include <QLineEdit>
#include <QValidator>
#include <QCompleter>

LineEditDelegate::LineEditDelegate(QSharedPointer<QValidator> validator, QObject *parent, bool ReadOnly)
    : QStyledItemDelegate(parent),
      validator_(validator), completer_(0), m_read_only(ReadOnly)
{
}

QWidget *LineEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
{
    QLineEdit *editor = new QLineEdit(parent);
    editor->setReadOnly(m_read_only);
    editor->setFrame(false);
    editor->setValidator(validator_.data());
    if (completer_)
        editor->setCompleter(completer_);
    connect(editor, SIGNAL(textEdited(const QString &)), SLOT(toUpper(const QString &)));
    return editor;
}

void LineEditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();
    QLineEdit *line_edit = static_cast<QLineEdit*>(editor);
    line_edit->setText(value);
}

void LineEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *line_edit = static_cast<QLineEdit*>(editor);
    QString value = line_edit->text();
    model->setData(index, value, Qt::EditRole);
}

void LineEditDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}

void LineEditDelegate::toUpper(const QString &text)
{
    QLineEdit *le = qobject_cast<QLineEdit *>(sender());
    if (!le)
        return;
    le->setText(text.toUpper());
}

void LineEditDelegate::setCompleter(const QStringList &completeList)
{
    if(completer_)
    {
        delete completer_;
        completer_ = 0;
    }
    if(completeList.size())
    {
        completer_ = new QCompleter(completeList, this);

        //completer_->setCompletionMode(QCompleter::PopupCompletion);
        //completer_->setCompletionMode(QCompleter::InlineCompletion);
        completer_->setCompletionMode(QCompleter::UnfilteredPopupCompletion);

    }
}
