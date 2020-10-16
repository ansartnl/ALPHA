#include "viewdelegate.h"
#include <QItemEditorFactory>
#include <QDoubleSpinBox>
#include <QLineEdit>

#include "QDebug"

// class CModifyDelegate
ModifyDelegate::ModifyDelegate(QObject* pObject) :
    QItemDelegate(pObject)
{
}

QWidget* ModifyDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    emit change();
    return QItemDelegate::createEditor(parent, option, index);
}

// class CRelModifyDelegate
RelModifyDelegate::RelModifyDelegate(QObject* pObject) :
    QSqlRelationalDelegate(pObject), mReadOnly(false)
{
}

void RelModifyDelegate::setReadOnly(bool readOnly)
{
    mReadOnly = readOnly;
}

QWidget* RelModifyDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem & option,
        const QModelIndex & index) const
{
    Q_UNUSED(option)
    if (mReadOnly)
        return 0;

    const QSqlRelationalTableModel *sqlModel = qobject_cast<const QSqlRelationalTableModel *> (index.model());
    QSqlTableModel *childModel = sqlModel ? sqlModel->relationModel(index.column()) : 0;
    if (!childModel)
    {
        //qDebug() << sqlModel->filter();

        if (!index.isValid())
            return 0;
        QVariant::Type t = static_cast<QVariant::Type> (index.data(Qt::EditRole).userType());
        TColMap::const_iterator itFind = m_mapColType.find(index.column());
        if (itFind != m_mapColType.end())
            t = itFind.value();
        const QItemEditorFactory *factory = QItemEditorFactory::defaultFactory();
        QWidget* pEditor = factory->createEditor(t, parent);
        if (dynamic_cast<QDoubleSpinBox*> (pEditor))
            dynamic_cast<QDoubleSpinBox*> (pEditor)->setDecimals(4);
        return pEditor;
    }

    //qDebug() << childModel->filter();

    QComboBox *combo = new QComboBox(parent);
    combo->setModel(childModel);
    combo->setModelColumn(childModel->fieldIndex(sqlModel->relation(index.column()).displayColumn()));
    combo->installEventFilter(const_cast<RelModifyDelegate *> (this));

    return combo;
}

void RelModifyDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QVariant::Type t = static_cast<QVariant::Type> (index.data(Qt::EditRole).userType());
    TColMap::const_iterator itFind = m_mapColType.find(index.column());
    if (itFind != m_mapColType.end())
        t = itFind.value();
    const QSqlRelationalTableModel *sqlModel = qobject_cast<const QSqlRelationalTableModel *> (index.model());
    QSqlTableModel *childModel = sqlModel ? sqlModel->relationModel(index.column()) : 0;
    if ((t == QVariant::UInt || t == QVariant::Int) && !childModel) {
        quint32 iVal = index.model()->data(index, Qt::DisplayRole).toUInt();
        drawDisplay(painter, option, option.rect, QString::number(iVal));
        drawFocus(painter, option, option.rect);
        return;
    }
    QSqlRelationalDelegate::paint(painter, option, index);
}

NullDelegate::NullDelegate(QObject* pObject) :
    RelModifyDelegate(pObject)
{
}

QWidget* NullDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    QWidget* pWidget = RelModifyDelegate::createEditor(parent, option, index);
    QComboBox* pCombo = dynamic_cast<QComboBox*> (pWidget);
    if (pCombo)
    {
        pCombo->addItem("_NULL_", QVariant());
    }
    return pWidget;

}

// class COctetDelegate
OctetDelegate::OctetDelegate(QObject* pObject) :
    RelModifyDelegate(pObject)
{
}

QWidget* OctetDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    QLineEdit* pEdit = new QLineEdit(parent);
    QRegExp regExp("[0-7]{1,4}");
    pEdit->setValidator(new QRegExpValidator(regExp, pEdit));
    connect(pEdit, SIGNAL(editingFinished()), this, SLOT(commitAndCloseEditor()));
    return pEdit;
}

void OctetDelegate::setEditorData(QWidget* editor, const QModelIndex &index) const
{
    quint32 iVal = index.model()->data(index, Qt::DisplayRole).toUInt();
    QLineEdit* pEdit = qobject_cast<QLineEdit*> (editor);
    if (pEdit)
        pEdit->setText(QString::number(iVal, 8).rightJustified(4, '0'));
}

void OctetDelegate::setModelData(QWidget* editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit* pEdit = qobject_cast<QLineEdit*> (editor);
    if (pEdit)
        model->setData(index, pEdit->text().toUInt(0, 8));
}

void OctetDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    quint32 iVal = index.model()->data(index, Qt::DisplayRole).toUInt();
    QString sText(QString::number(iVal, 8).rightJustified(4, '0'));
    drawDisplay(painter, option, option.rect, sText);
    drawFocus(painter, option, option.rect);
}

void OctetDelegate::commitAndCloseEditor()
{
    QLineEdit* editor = qobject_cast<QLineEdit*> (sender());
    emit commitData(editor);
    emit closeEditor(editor);
}


UpperCaseDelegate::UpperCaseDelegate(QObject *parent) :
    RelModifyDelegate(parent),
    mRegExpPattern("\\w*")
{
}

UpperCaseDelegate::UpperCaseDelegate(int length, QObject *parent) :
    RelModifyDelegate(parent),
    mRegExpPattern(QString("\\w{%1}").arg(length))
{
}

UpperCaseDelegate::UpperCaseDelegate(int minLength, int maxLength, QObject *parent) :
    RelModifyDelegate(parent),
    mRegExpPattern(QString("\\w{%1,%2}").arg(minLength).arg(maxLength))
{
}

QWidget * UpperCaseDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    QLineEdit *pEdit = new QLineEdit(parent);
    pEdit->setValidator(new QRegExpValidator(QRegExp(mRegExpPattern), pEdit));

    connect(pEdit, SIGNAL(textEdited(QString)), SLOT(onEdit(QString)));
    connect(pEdit, SIGNAL(editingFinished()), SLOT(commitAndCloseEditor()));
    return pEdit;
}

void UpperCaseDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QLineEdit *pEdit = qobject_cast < QLineEdit * >(editor);
    if (pEdit)
        pEdit->setText(index.model()->data(index, Qt::DisplayRole).toString());
}

void UpperCaseDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *pEdit = qobject_cast < QLineEdit * >(editor);
    if (pEdit)
        model->setData(index, pEdit->text());
}

void UpperCaseDelegate::onEdit(const QString &text)
{
    QLineEdit *pEdit = qobject_cast < QLineEdit * >(sender());
    Q_ASSERT(pEdit);
    if (text == text.toUpper())
        return;
    int pos = pEdit->cursorPosition();
    pEdit->setText(text.toUpper());
    pEdit->setCursorPosition(pos);
}

void UpperCaseDelegate::commitAndCloseEditor()
{
    QLineEdit *editor = qobject_cast < QLineEdit * >(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}


YesNoDelegate::YesNoDelegate(QObject *parent) :
    RelModifyDelegate(parent)
{
}

void YesNoDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    bool iVal = index.model()->data(index, Qt::DisplayRole).toBool();
    QString sText = tr("No");
    if (iVal)
        sText = tr("Yes");
    drawDisplay(painter, option, option.rect, sText);
    drawFocus(painter, option, option.rect);
}

QWidget * YesNoDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    QComboBox *pEdit = new QComboBox(parent);
    pEdit->insertItem(kYes, tr("Yes"));
    pEdit->insertItem(kNo, tr("No"));
    connect(pEdit, SIGNAL(currentIndexChanged(int)), SLOT(commitAndCloseEditor()));
    return pEdit;
}

void YesNoDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *pEdit = qobject_cast < QComboBox * >(editor);
    bool iVal = index.model()->data(index, Qt::DisplayRole).toBool();
    if (iVal)
        pEdit->setCurrentIndex(kYes);
    else
        pEdit->setCurrentIndex(kNo);
}

void YesNoDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *pEdit = qobject_cast < QComboBox * >(editor);
    if (pEdit)
        model->setData(index, pEdit->currentIndex() == kYes);
}

void YesNoDelegate::commitAndCloseEditor()
{
    QComboBox *editor = qobject_cast < QComboBox * >(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}
