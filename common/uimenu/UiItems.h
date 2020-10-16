#ifndef _UI_ITEMS_H__
#define _UI_ITEMS_H__

#include <QtCore/QSharedPointer>


namespace Uim {

//! Base config item.
class BaseItem
{
public:
    //! Returns true when item is separator.
    virtual bool isSeparator() const = 0;
    //! Returns true when item is an action.
    virtual bool isAction() const = 0;
    //! Returns true when item is a group of items.
    virtual bool isGroup() const = 0;
};

typedef QSharedPointer<BaseItem>    TBaseItem;

//! Action (command) config item.
class ActionItem : public BaseItem
{
public:
    enum Style
    {
        IconOnly,
        TextOnly,
        TextBesideIcon,
        TextUnderIcon
    };

    ActionItem(const QString &text, const QString &name, Style style = TextBesideIcon,
               bool isSubMenuEnabled = false)
        : mText(text), mName(name), mIsSubMenuEnabled(isSubMenuEnabled), mStyle(style)
    {}

    //! Not a separator item.
    virtual bool isSeparator() const
    { return false; }

    //! Tells that item is action.
    virtual bool isAction() const
    { return true; }

    //! Not a group item.
    virtual bool isGroup() const
    { return false; }

    //! Not a toolbutton item.
    virtual bool isToolbutton() const
    { return false; }

    virtual bool operator == (const ActionItem &item) const
    {
        return ( mText == item.mText && mName == item.mName && mToolTip == item.mToolTip &&
                 mIsSubMenuEnabled == item.mIsSubMenuEnabled && mStyle == item.mStyle );
    }
    virtual bool operator != (const ActionItem &item) const
    { return !( operator == (item) ); }

    /** Visible text of item */
    QString     mText;
    /** Action name when mIsSubMenuEnabled is false or sub menu name instead */
    QString     mName;
    /** Tooltip of item */
    QString     mToolTip;
    /** True when action should show sub menu */
    bool        mIsSubMenuEnabled;
    /** Current action style */
    Style       mStyle;
};

typedef QSharedPointer<ActionItem>  TActionItem;

//! Toolbutton config item.
/*!
    Item is available only for toolbar.
*/
class ToolbuttonItem : public ActionItem
{
public:
    enum SizePolicy
    {
        Fixed,
        Minimum
    };

    enum Alignment {
        AlignLeft,
        AlignRight,
        AlignHCenter,
        AlignTop,
        AlignBottom,
        AlignVCenter,
        AlignCenter
    };

    ToolbuttonItem(const QString &text, const QString &name, Style style = TextBesideIcon,
                   bool isSubMenuEnabled = false)
        : ActionItem(text, name, style, isSubMenuEnabled),
        mRow(-1), mColumn(-1), mRowSpan(1), mColumnSpan(1), mAlignment(AlignLeft),
        mIconSize(-1), mSizePolicy(Fixed)
    {}

    //! Tells that item is toolbutton.
    virtual bool isToolbutton() const
    { return true; }

    virtual bool operator == (const ToolbuttonItem &item) const
    {
        return ( mRow == item.mRow && mColumn == item.mColumn &&
                 mRowSpan == item.mRowSpan && mColumnSpan == item.mColumnSpan &&
                 mAlignment == item.mAlignment &&
                 mIconSize == item.mIconSize && mSizePolicy == item.mSizePolicy &&
                 ActionItem::operator == (item) );
    }
    virtual bool operator != (const ToolbuttonItem &item) const
    { return !( operator == (item) ); }

    /** Row position in a group */
    int         mRow;
    /** Column position in a group */
    int         mColumn;
    /** Row spanning in a group */
    int         mRowSpan;
    /** Column spanning in a group */
    int         mColumnSpan;
    /** Alignment of a grid cell */
    Alignment   mAlignment;
    /** Quadrangle icons size: for width and height */
    int         mIconSize;
    /** Size policy for width and height */
    SizePolicy  mSizePolicy;
};

typedef QSharedPointer<ToolbuttonItem>  TToolbuttonItem;

//! Separator config item.
/*!
    Item is available only for menu.
*/
struct SeparatorItem : public BaseItem
{
public:
    //! Tells that item is separator.
    virtual bool isSeparator() const
    { return true; }

    //! Not an action item.
    virtual bool isAction() const
    { return false; }

    //! Not a group item.
    virtual bool isGroup() const
    { return false; }
};

class GroupItem;
typedef QSharedPointer<GroupItem>   TGroupItem;

//! Group of items - config item.
class GroupItem : public BaseItem
{
public:
    typedef QList<TBaseItem>    ItemsList;

    GroupItem(const QString &text = QString())
    { mText = text; }

    //! Not a separator item.
    virtual bool isSeparator() const
    { return false; }

    //! Not an action item.
    virtual bool isAction() const
    { return false; }

    //! Tells that this item is group.
    virtual bool isGroup() const
    { return true; }

    //! Creates an action (command) and adds it to the end of the items list.
    void addAction(const QString &text, const QString &nameAction,
                   ActionItem::Style style, bool isSubMenuEnabled,
                   const QString &toolTip = QString());
    //! Creates a toolbutton and adds it to the end of the items list.
    void addToolbutton(const QString &text, const QString &nameAction,
                       ActionItem::Style style, bool isSubMenuEnabled,
                       int row, int column, int iconSize = -1,
                       ToolbuttonItem::SizePolicy sizePolicy = ToolbuttonItem::Fixed,
                       int rowSpan = 1, int columnSpan = 1,
                       ToolbuttonItem::Alignment alignment = ToolbuttonItem::AlignLeft,
                       const QString &toolTip = QString());

    //! Creates a menu separator and adds it to the end of the items list.
    void addSeparator();

    //! Creates new group of items.
    /*!
        \return Return new group item.
    */
    TGroupItem addGroup(const QString &text);

    //! Sets new item name.
    void setText(const QString &textNew)
    { mText = textNew; }
    //! Retrieves current name of the item.
    const QString& text() const
    { return mText; }

    //! Retrirves children list of this group.
    const ItemsList& children() const
    { return mItems; }

    //! Swap position of 2 items.
    void swap(int index1, int index2);
    //! Moves the item at index position from to index position to..
    void move(int from, int to);

    //! Returns number of items in the group.
    int count() const
    { return mItems.count(); }

    //! Returns true if group does not have items.
    bool isEmpty() const
    { return ( count() < 1 ); }

    //! Removes all items from the group and makes group empty.
    void clear()
    { mItems.clear(); }

    //! Remove item from the group.
    void removeAt(int i);

    //! Replaces current group with the new one.
    GroupItem& operator = (const GroupItem &item);

    //! Compares this and specified group.
    bool operator == (const GroupItem &item) const;
    bool operator != (const GroupItem &item) const;

private:
    //! Name of this group.
    QString     mText;
    //! List of children.
    ItemsList   mItems;
};

}   // namespace Uim

#endif // _MENU_UI_MODEL_H__
