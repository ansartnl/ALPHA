#include "coorddelegate.h"

CoordDelegate::CoordDelegate(CoordEdit::CoordType type, QObject *parent)
    : QStyledItemDelegate(parent), m_CoordType(type), m_ReadOnly(false)
{
}

CoordDelegate::CoordDelegate(CoordEdit::CoordType type, bool readOnly, QObject *parent)
    : QStyledItemDelegate(parent), m_CoordType(type), m_ReadOnly(readOnly)
{
}

bool CoordDelegate::isReadOnly() const
{
    return m_ReadOnly;
}

void CoordDelegate::setReadOnly(bool readOnly)
{
    m_ReadOnly = readOnly;
}

QString CoordDelegate::displayText(const QVariant &value, const QLocale &) const
{
    return CoordEdit::toString(m_CoordType, value);
}

QWidget* CoordDelegate::createEditor(QWidget* parent,
        const QStyleOptionViewItem &,
        const QModelIndex &) const
{
    if (isReadOnly())
        return 0;
    CoordEdit* pEdit = new CoordEdit(m_CoordType, parent);
    connect(pEdit, SIGNAL(editingFinished()), this, SLOT(commitAndCloseEditor()));
    return pEdit;
}

void CoordDelegate::setEditorData(QWidget* editor,
                                  const QModelIndex &index) const
{
    CoordEdit* pEdit = qobject_cast<CoordEdit*>(editor);
    if(pEdit)
        pEdit->setValue(index.data());
}

void CoordDelegate::setModelData(QWidget* editor,
                                 QAbstractItemModel *model,
                                 const QModelIndex &index) const
{
    CoordEdit* pEdit = qobject_cast<CoordEdit*>(editor);
    if(pEdit)
        model->setData(index, pEdit->getValue());
}

void CoordDelegate::commitAndCloseEditor()
{
    CoordEdit* pEdit = qobject_cast<CoordEdit*>(sender());
    emit commitData(pEdit);
    emit closeEditor(pEdit);
}
