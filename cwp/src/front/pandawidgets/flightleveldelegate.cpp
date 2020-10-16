#include "flightleveldelegate.h"

#include "FlightLevelWidget.h"

FlightLevelDelegate::FlightLevelDelegate(QWidget *parent_widget, QObject *parent)
    : QStyledItemDelegate(parent),
      parent_widget_(parent_widget), bAccepted_(false)
{

}

QWidget *FlightLevelDelegate::createEditor(QWidget * /*parent*/, const QStyleOptionViewItem &option, const QModelIndex &) const
{
    const_cast<FlightLevelDelegate*>(this)->bAccepted_ = false;

    FlightLevelWidget *flight_level_widget = new FlightLevelWidget(QString(), QString(), parent_widget_);
    flight_level_widget->setTflMarkerVisible(false);
    //FlightLevelWidget *flight_level_widget = new FlightLevelWidget(QString(), QString(), parent);
    flight_level_widget->resize(90, 360);
    connect(flight_level_widget, SIGNAL(accepted(double, double, int)), this, SLOT(commitAndCloseEditor()));
    flight_level_widget->setFocusOnCustomEdit();
    return flight_level_widget;
}

void FlightLevelDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = 'F' + index.model()->data(index, Qt::EditRole).toString();
    FlightLevelWidget *flight_level_widget = static_cast<FlightLevelWidget*>(editor);
    flight_level_widget->setVisibleValue(value);
    flight_level_widget->setValueColor(value, Qt::yellow, Qt::black);
}

void FlightLevelDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(bAccepted_)
    {
        FlightLevelWidget *flight_level_widget = static_cast<FlightLevelWidget*>(editor);
        int value = flight_level_widget->value(FlightLevelWidget::FEET);
        model->setData(index, value, Qt::EditRole);
    }
}

void FlightLevelDelegate::updateEditorGeometry(QWidget *flight_level_widget, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    flight_level_widget->move(parent_widget_->pos() + option.rect.topLeft());
}

void FlightLevelDelegate::commitAndCloseEditor()
{
    bAccepted_ = true;
    QWidget* editor = qobject_cast<QWidget*>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}
