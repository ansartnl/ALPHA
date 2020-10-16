#include "treecombobox.h"
#include <QTreeView>
#include <QEvent>
#include <QMouseEvent>
#include <QModelIndex>
#include <QStandardItemModel>

TreeComboBox::TreeComboBox(QWidget* parent)
    : QComboBox(parent), mSkipNextHide(false)
{
    QTreeView* pView = new QTreeView(this);
    pView->setHeaderHidden(true);
    pView->setAllColumnsShowFocus(true);
    pView->setRootIsDecorated(true);
    setView(pView);
    setModel(new QStandardItemModel(this));
    view()->viewport()->installEventFilter(this);
    setSizeAdjustPolicy(AdjustToContents);
}

TreeComboBox::~TreeComboBox()
{

}

bool TreeComboBox::eventFilter(QObject* object, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonPress && object == view()->viewport())
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        QModelIndex index = view()->indexAt(mouseEvent->pos());
        if (!view()->visualRect(index).contains(mouseEvent->pos()))
            mSkipNextHide = true;
    }
    return false;
}

void TreeComboBox::showPopup()
{
    QComboBox::showPopup();
}

void TreeComboBox::hidePopup()
{
    setCurrentIndex(view()->currentIndex().row());
    if (mSkipNextHide)
        mSkipNextHide = false;
    else
        QComboBox::hidePopup();
}
