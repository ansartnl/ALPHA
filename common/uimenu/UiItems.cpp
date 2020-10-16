#include "UiItems.h"


namespace Uim
{

void GroupItem::addAction(const QString &text, const QString &nameAction,
                          ActionItem::Style style, bool isSubMenuEnabled,
                          const QString &toolTip)
{
    ActionItem *actionItem = new ActionItem(text, nameAction, style, isSubMenuEnabled);
    actionItem->mToolTip = toolTip;
    mItems.append( TBaseItem(actionItem) );
}

void GroupItem::addToolbutton(const QString &text, const QString &nameAction,
                              ActionItem::Style style, bool isSubMenuEnabled,
                              int row, int column, int iconSize,
                              ToolbuttonItem::SizePolicy sizePolicy,
                              int rowSpan, int columnSpan,
                              ToolbuttonItem::Alignment alignment,
                              const QString &toolTip)
{
    ToolbuttonItem *toolbuttonItem = new ToolbuttonItem(text, nameAction, style, isSubMenuEnabled);
    toolbuttonItem->mToolTip = toolTip;
    toolbuttonItem->mRow = row;
    toolbuttonItem->mColumn = column;
    toolbuttonItem->mRowSpan = rowSpan;
    toolbuttonItem->mColumnSpan = columnSpan;
    toolbuttonItem->mAlignment = alignment;
    toolbuttonItem->mIconSize = iconSize;
    toolbuttonItem->mSizePolicy = sizePolicy;
    mItems.append( TBaseItem(toolbuttonItem) );
}

void GroupItem::addSeparator()
{
    mItems.append( TBaseItem(new SeparatorItem) );
}

TGroupItem GroupItem::addGroup(const QString &text)
{
    TGroupItem groupItem = TGroupItem(new GroupItem(text));

    mItems.append( groupItem );
    return groupItem;
}

void GroupItem::swap(int i, int j)
{
    if ( i < 0 || j < 0 || i >= count() || j >= count() )
        return;

    mItems.swap(i, j);
}

void GroupItem::move(int from, int to)
{
    if ( from < 0 || to < 0 || from >= count() || to >= count() )
        return;

    mItems.move(from, to);
}

void GroupItem::removeAt(int i)
{
    if ( i >= 0 && i < count() )
        mItems.removeAt(i);
}

GroupItem& GroupItem::operator = (const GroupItem &item)
{
    mText = item.mText;
    mItems.clear();

    foreach ( const TBaseItem &baseItem, item.mItems )
    {
        if ( baseItem->isSeparator() )
            mItems.append( TBaseItem(new SeparatorItem) );
        else if ( baseItem->isGroup() )
        {
            TGroupItem groupItemCurrent = baseItem.staticCast<GroupItem>();
            TGroupItem groupItem(new GroupItem(groupItemCurrent->text()));
            *groupItem = *groupItemCurrent;

            mItems.append( groupItem );
        }
        else if ( baseItem->isAction() )
        {
            TBaseItem actionItem;
            TActionItem actionItemCurrent = baseItem.staticCast<ActionItem>();
            if ( !actionItemCurrent->isToolbutton() )
            {
                actionItem = TActionItem(new ActionItem(actionItemCurrent->mText,
                                                        actionItemCurrent->mName,
                                                        actionItemCurrent->mStyle,
                                                        actionItemCurrent->mIsSubMenuEnabled));
                static_cast<ActionItem *>(actionItem.data())->mToolTip = actionItemCurrent->mToolTip;
            }
            else
            {
                TToolbuttonItem toolbuttonItemCurrent = baseItem.staticCast<ToolbuttonItem>();
                ToolbuttonItem *toolbuttonItem = new ToolbuttonItem(toolbuttonItemCurrent->mText,
                                                                    toolbuttonItemCurrent->mName,
                                                                    toolbuttonItemCurrent->mStyle,
                                                                    toolbuttonItemCurrent->mIsSubMenuEnabled);
                toolbuttonItem->mToolTip = toolbuttonItemCurrent->mToolTip;
                toolbuttonItem->mRow = toolbuttonItemCurrent->mRow;
                toolbuttonItem->mColumn = toolbuttonItemCurrent->mColumn;
                toolbuttonItem->mRowSpan = toolbuttonItemCurrent->mRowSpan;
                toolbuttonItem->mColumnSpan = toolbuttonItemCurrent->mColumnSpan;
                toolbuttonItem->mAlignment = toolbuttonItemCurrent->mAlignment;
                toolbuttonItem->mIconSize = toolbuttonItemCurrent->mIconSize;
                toolbuttonItem->mSizePolicy = toolbuttonItemCurrent->mSizePolicy;

                actionItem = TToolbuttonItem(toolbuttonItem);
            }

            mItems.append( actionItem );
        }
    }

    return *this;
}

bool GroupItem::operator == (const GroupItem &item) const
{
    if ( mText != item.mText ||
         mItems.count() != item.mItems.count() )
        return false;

    ItemsList::const_iterator itcThis = mItems.constBegin();
    ItemsList::const_iterator itcItem = item.mItems.constBegin();
    for ( ; itcThis != mItems.constEnd(); ++itcThis, ++itcItem )
    {
        const TBaseItem &baseItemThis = *itcThis;
        const TBaseItem &baseItem = *itcItem;

        if ( baseItemThis->isSeparator() && baseItem->isSeparator() )
            continue;
        else if ( baseItemThis->isGroup() && baseItem->isGroup() )
        {
            if ( *static_cast<GroupItem *>(baseItemThis.data()) != *static_cast<GroupItem *>(baseItem.data()) )
                return false;
            continue;
        }
        else if ( baseItemThis->isAction() && baseItem->isAction() )
        {
            TActionItem actionItemThis = baseItemThis.staticCast<ActionItem>();
            TActionItem actionItem = baseItem.staticCast<ActionItem>();

            if ( actionItemThis->isToolbutton() && actionItem->isToolbutton() )
            {
                if ( *static_cast<ToolbuttonItem *>(baseItemThis.data()) != *static_cast<ToolbuttonItem *>(baseItem.data()) )
                    return false;
            }
            else
            {
                if ( *static_cast<ActionItem *>(baseItemThis.data()) != *static_cast<ActionItem *>(baseItem.data()) )
                    return false;
            }
            continue;
        }

        // Items are different - quit.
        return false;
    }

    return true;
}

bool GroupItem::operator != (const GroupItem &item) const
{
    return !( operator == (item) );
}

}   // namespace Uim
