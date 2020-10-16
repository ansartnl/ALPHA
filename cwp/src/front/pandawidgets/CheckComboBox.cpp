#include <QWheelEvent>

#include "CheckComboBox.h"

CheckComboBox::CheckComboBox(QWidget *parent): QxtCheckComboBox(parent)
{
}

void CheckComboBox::wheelEvent(QWheelEvent *event)
{
    event->ignore();
}
