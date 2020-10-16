#ifndef EDITABLEHEADER_H
#define EDITABLEHEADER_H

#include "EditHeaderInterface.h"

#include "QtCheckHeaderView.h"

class EditableHeader : public QHeaderView
{
    Q_OBJECT
public:
    explicit EditableHeader(Qt::Orientation orientation, QWidget *parent = 0);

    void setEditHeaderInterface(QSharedPointer<EditHeaderInterface> &edit_interface);

    enum ContextMenuElement {
        CME_NONE = 0x00000000
        , CME_REMOVE_SORT = 0x00000001
        , CME_VISIBILITY_MENU = 0x00000002
        , CME_EDIT_HEADER = 0x00000004
        , CME_LOCK_MOVING = 0x00000008

        , CME_ALL = CME_REMOVE_SORT | CME_VISIBILITY_MENU | CME_EDIT_HEADER | CME_LOCK_MOVING
    };
    typedef QFlags < ContextMenuElement > ContextMenuElements;

    ContextMenuElements contextMenuElements() const;
    void setContextMenuElements(const ContextMenuElements &el);

    QMenu * standardMenu(QWidget *parent = 0);

public slots:
    void setEnabledContextMenu(bool enabled);

protected:
    void mousePressEvent(QMouseEvent* event);

    void paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const;

    void reloadItems();

    void showMenu();
    void showActionsMenu();

    QString text(int number) const;
    int lang() const;

    int fromVisualToRealIndex(int visualIndex);

protected slots:
    void onSectionMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex);

    void onLockMoving(bool toggled);
    void onRemoveSort();
    void onVisibilityAction(bool toggled);
    void onEditHeader();

protected:
    mutable QMap < int, QRect > mSectionRects;
    bool manualSectionMove;

    QSharedPointer<EditHeaderInterface> editInterface;
    QMap < int, EditHeaderItem > mItems;

    ContextMenuElements mContextMenuElements;

    QAction *lockMovingAction;
    QAction *removeSortAction;
    QList < QAction * > visibilityActions;
    QAction *editHeaderAction;
};

#endif // EDITABLEHEADER_H
