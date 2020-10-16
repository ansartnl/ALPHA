#ifndef URZCOMBOBOX_H
#define URZCOMBOBOX_H

#include "treecombobox.h"

// User's restriction zone types combobox
class URZComboBox : public TreeComboBox
{
Q_OBJECT
public:
    URZComboBox(bool bAdmin, QWidget* parent = 0);
    virtual ~URZComboBox();

    virtual void hidePopup();
};

#endif // URZCOMBOBOX_H
