#ifndef CHECKCOMBOBOX_H
#define CHECKCOMBOBOX_H

#include <qxtcheckcombobox.h>

class CheckComboBox : public QxtCheckComboBox
{
public:
    explicit CheckComboBox(QWidget* parent = 0);

protected:
    void wheelEvent(QWheelEvent *event);
};

#endif // CHECKCOMBOBOX_H
