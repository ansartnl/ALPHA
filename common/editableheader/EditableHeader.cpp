#include "EditableHeader.h"

#include "EditHeaderDialog.h"

#ifdef QT_TRANSLATOR_INCLUDED
#include "qttranslator.h"
#endif

#include <QAction>
#include <QMenu>
#include <QMouseEvent>

EditableHeader::EditableHeader(Qt::Orientation orientation, QWidget *parent) :
    QHeaderView(orientation, parent), mContextMenuElements(CME_ALL)
{
    setVisible(true);

    manualSectionMove = false;

    connect(this, SIGNAL(sectionMoved(int,int,int)), SLOT(onSectionMoved(int,int,int)));

    lockMovingAction = new QAction(this);
    lockMovingAction->setCheckable(true);
    connect(lockMovingAction, SIGNAL(toggled(bool)), SLOT(onLockMoving(bool)));

    removeSortAction = new QAction(this);
    connect(removeSortAction, SIGNAL(triggered()), SLOT(onRemoveSort()));

    editHeaderAction = new QAction(this);
    connect(editHeaderAction, SIGNAL(triggered()), SLOT(onEditHeader()));
}

void EditableHeader::setEditHeaderInterface(QSharedPointer<EditHeaderInterface> &edit_interface)
{
    editInterface = edit_interface;

    reloadItems();
}

EditableHeader::ContextMenuElements EditableHeader::contextMenuElements() const
{
    return mContextMenuElements;
}

void EditableHeader::setContextMenuElements(const EditableHeader::ContextMenuElements &el)
{
    mContextMenuElements = el;
}

QMenu * EditableHeader::standardMenu(QWidget *parent)
{
    QMenu *menu = new QMenu(parent);

    if (mContextMenuElements.testFlag(CME_LOCK_MOVING))
    {
        if (editInterface)
        {
            lockMovingAction->setText(tr("Lock moving"));
            lockMovingAction->setChecked(!isMovable());
            menu->addAction(lockMovingAction);
        }
    }

    if (mContextMenuElements.testFlag(CME_REMOVE_SORT))
    {
        removeSortAction->setText(tr("Remove sort"));
        if (isSortIndicatorShown() && (sortIndicatorSection() >= 0))
            menu->addAction(removeSortAction);
    }

    if (mContextMenuElements.testFlag(CME_VISIBILITY_MENU))
    {
        QMenu *visibilityMenu = new QMenu(tr("Visible"), menu);
        foreach (QAction *a, visibilityActions)
        {
            a->setText(text(a->data().toInt()));
            a->setChecked(mItems[a->data().toInt()].visible);
            visibilityMenu->addAction(a);
        }
        if (!visibilityMenu->isEmpty())
            menu->addMenu(visibilityMenu);
    }

    if (mContextMenuElements.testFlag(CME_EDIT_HEADER))
    {
        editHeaderAction->setText(tr("Advanced..."));
        if (editInterface)
        {
            if (!menu->isEmpty())
                menu->addSeparator();
            menu->addAction(editHeaderAction);
        }
    }

    return menu;
}

void EditableHeader::setEnabledContextMenu(bool enabled)
{
    lockMovingAction->setEnabled(enabled);
    foreach (QAction *a, visibilityActions)
        a->setEnabled(enabled);
    editHeaderAction->setEnabled(enabled);
}

void EditableHeader::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (isClickable())
        {
            foreach (int section, mSectionRects.keys())
            {
                if (mSectionRects.value(section).contains(event->pos()))
                {
                    if (!isSortIndicatorShown())
                        setSortIndicatorShown(true);
                    break;
                }
            }
        }
    }
    if (event->button() == Qt::RightButton)
    {
        switch (contextMenuPolicy())
        {
        case Qt::NoContextMenu:
            break;
        case Qt::PreventContextMenu:
            event->ignore();
            break;
        case Qt::DefaultContextMenu:
            showMenu();
            event->ignore();
            break;
        case Qt::ActionsContextMenu:
            showActionsMenu();
            event->ignore();
            break;
        case Qt::CustomContextMenu:
            emit customContextMenuRequested(event->pos());
            event->ignore();
            break;
        }
    }

    QHeaderView::mousePressEvent(event);
}

void EditableHeader::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    mSectionRects.insert(logicalIndex, rect);
    QHeaderView::paintSection(painter, rect, logicalIndex);
}

void EditableHeader::reloadItems()
{
    mItems.clear();

    EditHeaderItems items = editInterface->items();
    foreach (const EditHeaderItem &item, items)
        mItems.insert(item.number, item);

    foreach (QAction *a, visibilityActions)
        a->deleteLater();
    visibilityActions.clear();

    foreach (const EditHeaderItem &item, mItems.values())
    {
        QAction *a = new QAction(this);
        a->setCheckable(true);
        a->setChecked(item.visible);
        a->setData(item.number);
        connect(a, SIGNAL(toggled(bool)), SLOT(onVisibilityAction(bool)));
        visibilityActions << a;
    }
}

void EditableHeader::showMenu()
{
    QMenu *menu = standardMenu(this);

    menu->setAttribute(Qt::WA_DeleteOnClose);

    if (!menu->isEmpty())
        menu->exec(QCursor::pos());
    else
        menu->deleteLater();
}

void EditableHeader::showActionsMenu()
{
    QMenu menu(this);

    foreach (QAction *a, actions())
        menu.addAction(a);

    if (!menu.isEmpty())
        menu.exec(QCursor::pos());
}

QString EditableHeader::text(int number) const
{
    QString ret;

    EditHeaderItem item = mItems.value(number);
    int l = lang();

    QStringList texts = item.texts;
    if (texts.count() >= l && l >= 0)
        ret = texts[l];
    else
        ret = item.identifier;

    return ret;
}

int EditableHeader::lang() const
{
    int ret = 0;
#ifdef QT_TRANSLATOR_INCLUDED
    QStringList languages = editInterface->languages();
    for (int i = 0; i < languages.count(); ++i)
    {
        if (QtTranslator::instance()->currentLanguage() == languages[i])
            ret = i;
    }
#endif
    return ret;
}

void EditableHeader::onSectionMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex)
{
    Q_UNUSED(logicalIndex)
    if (!manualSectionMove)
    {
        int first = fromVisualToRealIndex(oldVisualIndex) + 1;
        int last = fromVisualToRealIndex(newVisualIndex) + 1;
        int step = (newVisualIndex > oldVisualIndex) ? 1 : -1;
        while (first != last)
        {
            EditHeaderItem item1 = mItems[first];
            EditHeaderItem item2 = mItems[first + step];

            ::swap(item1, item2);

            mItems[item1.number] = item1;
            mItems[item2.number] = item2;

            first += step;
        }

        editInterface->setItems(mItems.values());

        manualSectionMove = true;
        moveSection(newVisualIndex, oldVisualIndex);
    } else
    {
        manualSectionMove = false;
    }
}


int EditableHeader::fromVisualToRealIndex(int visualIndex)
{
    foreach(const EditHeaderItem &arg, mItems)
        if (arg.number-1 < visualIndex)
        {
            if (arg.visible == false)
                ++visualIndex;
        } else
        {
            break;
        }
    return visualIndex;
}

void EditableHeader::onLockMoving(bool toggled)
{
    setMovable(!toggled);
}

void EditableHeader::onRemoveSort()
{
    setSortIndicator(-1, Qt::AscendingOrder);
    setSortIndicatorShown(false);
}

void EditableHeader::onVisibilityAction(bool toggled)
{
    mItems[qobject_cast < QAction * >(sender())->data().toInt()].visible = toggled;
    editInterface->setItems(mItems.values());
}

void EditableHeader::onEditHeader()
{
    EditHeaderDialog dialog(this);
    dialog.setEditHeaderInterface(editInterface);
    dialog.exec();

    reloadItems();
}
