#ifndef TREECOMBOBOX_H
#define TREECOMBOBOX_H

#include <QComboBox>

// QComboBox with QTreeView
class TreeComboBox : public QComboBox
{
Q_OBJECT
public:
    TreeComboBox(QWidget* parent = 0);
    virtual ~TreeComboBox();

    virtual void showPopup();
    virtual void hidePopup();
protected:
    virtual bool eventFilter(QObject* object, QEvent* event);
private:
    bool    mSkipNextHide;
};

#endif // TREECOMBOBOX_H
