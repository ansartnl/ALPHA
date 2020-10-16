#include "qtcheckboxdelegate.h"

#include <QApplication>
#include <QPainter>
#include <QMouseEvent>

QtCheckBoxDelegate::QtCheckBoxDelegate(QObject *parent, const QVariant& trueValue, const QVariant& falseValue) :
    QStyledItemDelegate(parent), _trueValue(trueValue), _falseValue(falseValue)
{
    if (objectName().isEmpty())
        setObjectName("CheckBoxDelegate");
}

QWidget* QtCheckBoxDelegate::createEditor(QWidget* /*parent*/, const QStyleOptionViewItem& /*option*/,
    const QModelIndex& /*index*/) const
{
    return 0;
}

void QtCheckBoxDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItemV4 v4(option);
    initStyleOption(&v4, index);

    const QWidget *widget;
    widget = v4.widget;
    QStyle *style = widget ? widget->style() : QApplication::style();

    // Отключаем ChechStateRole и DisplayRole
    v4.features &= (~QStyleOptionViewItemV2::HasCheckIndicator);
    v4.features &= (~QStyleOptionViewItemV2::HasDisplay);
    //v4.checkState = Unchecked
    v4.text.clear();

    style->drawControl(QStyle::CE_ItemViewItem, &v4, painter, widget);

    QStyleOptionViewItem opt(option);
    opt.rect = checkRect(option, option.rect);
    opt.state = opt.state & ~QStyle::State_HasFocus;
    if (index.data() == _trueValue)
        opt.state |= QStyle::State_On;
    else
        opt.state |= QStyle::State_Off;
    //opt.state|=(index.data().toInt() > 0 ? QStyle::State_On : QStyle::State_Off);
    style->drawPrimitive(QStyle::PE_IndicatorViewItemCheck, &opt, painter);
}

QString QtCheckBoxDelegate::displayText(const QVariant &/*value*/
, const QLocale &/*locale*/) const
{
    return QString::null;
}

QRect QtCheckBoxDelegate::checkRect(const QStyleOptionViewItem& option, const QRect& bounding) const
{
    QStyleOptionButton opt;
    opt.QStyleOption::operator=(option);
    opt.rect = bounding;
    QRect cr = qApp->style()->subElementRect(QStyle::SE_ViewItemCheckIndicator, &opt);
    int deltaX = (bounding.width() - cr.width()) / 2;
    int deltaY = (bounding.height() - cr.height()) / 2;
    return QRect(bounding.left() + deltaX, bounding.top() + deltaY, cr.width(), cr.height());
}

bool QtCheckBoxDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option,
    const QModelIndex& index)
{
    if (!event || !model)
        return QStyledItemDelegate::editorEvent(event, model, option, index);

    Qt::ItemFlags flags = model->flags(index);
    if (!(option.state & QStyle::State_Enabled) || !(flags & Qt::ItemIsEnabled))
        return QStyledItemDelegate::editorEvent(event, model, option, index);

    switch (event->type())
    {
    case QEvent::MouseButtonRelease:
    case QEvent::MouseButtonDblClick:
    {
        QRect cr(checkRect(option, option.rect));
        QMouseEvent *me = static_cast<QMouseEvent*> (event);
        if (me->button() != Qt::LeftButton || !cr.contains(me->pos()))
            return QStyledItemDelegate::editorEvent(event, model, option, index);

        // eat the double click events inside the check rect
        if (event->type() == QEvent::MouseButtonDblClick)
            return true;
        break;
    }
    case QEvent::KeyPress:
    {
        QKeyEvent* kev = static_cast<QKeyEvent*> (event);
        if (kev->key() != Qt::Key_Space && kev->key() != Qt::Key_Select)
            return QStyledItemDelegate::editorEvent(event, model, option, index);
        break;
    }
    default:
        return false;
    }

    QVariant value;
    if (index.data() == _trueValue)
        value = _falseValue;
    else
        value = _trueValue;
    //bool value = !index.data().toBool();
    //if (index.flags() & Qt::ItemIsEditable)
    return model->setData(index, value, Qt::EditRole);
    //return true;
}

QVariant QtCheckBoxDelegate::trueValue() const
{
    return _trueValue;
}
void QtCheckBoxDelegate::setTrueValue(const QVariant& value)
{
    _trueValue = value;
}

QVariant QtCheckBoxDelegate::falseValue() const
{
    return _falseValue;
}
void QtCheckBoxDelegate::setFalseValue(const QVariant& value)
{
    _falseValue = value;
}
