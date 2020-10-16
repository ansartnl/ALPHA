#include "RearrangeModels.h"

#include <QtGui/QMenu>

namespace Uim
{

QIcon BaseUiModel::iconTransparent;
const QString gSeparatorLine = "----------------------";

static const char *gAlignLeftText = QT_TRANSLATE_NOOP("ToolbarUiModel", "AlignLeft");
static const char *gAlignRightText = QT_TRANSLATE_NOOP("ToolbarUiModel", "AlignRight");
static const char *gAlignHCenterText = QT_TRANSLATE_NOOP("ToolbarUiModel", "AlignHCenter");
static const char *gAlignTopText = QT_TRANSLATE_NOOP("ToolbarUiModel", "AlignTop");
static const char *gAlignBottomText = QT_TRANSLATE_NOOP("ToolbarUiModel", "AlignBottom");
static const char *gAlignVCenterText = QT_TRANSLATE_NOOP("ToolbarUiModel", "AlignVCenter");
static const char *gAlignCenterText = QT_TRANSLATE_NOOP("ToolbarUiModel", "AlignCenter");


////////////////////////////////////////////////////////////////////////////////////
// BaseUiModel implementation

BaseUiModel::BaseUiModel(const AbstractUiModel *modelCmd, QObject *parent)
    : mModelCmd(modelCmd)
{
    Q_ASSERT(mModelCmd);

    // Load icon only once.
    if ( iconTransparent.isNull() )
        iconTransparent.addFile(":/images/Transparent.png", QSize(), QIcon::Normal, QIcon::Off);
}

void BaseUiModel::setData(const TGroupItem &groupItem)
{
    mGroupItem = groupItem;

    emit layoutChanged();
}

QModelIndex BaseUiModel::index(int row, int column,
                               const QModelIndex &parent) const
{
    return createIndex(row, column);
}

QModelIndex BaseUiModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

int BaseUiModel::rowCount(const QModelIndex &parent) const
{
    return ( mGroupItem ? mGroupItem->count() : 0 );
}

int BaseUiModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant BaseUiModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() || !mGroupItem ||
         index.row() < 0 || index.row() >= mGroupItem->count() )
        return QVariant();

    if ( role == userRoleText )
    {
        const TBaseItem &item = mGroupItem->children().at( index.row() );
        if ( item->isGroup() )
            return static_cast<GroupItem *>(item.data())->text();
        else if ( item->isAction() )
        {
            ActionItem *actionItem = static_cast<ActionItem *>(item.data());
            QString text = actionItem->mText;
            // If text was not defined by the user - use translated action text instead.
            if ( text.isEmpty() )
            {
                if ( actionItem->mIsSubMenuEnabled )
                {
                    QMenu *menu = mModelCmd->findMenu(actionItem->mName);
                    if ( menu )
                        text = menu->title();
                }
                else
                {
                    QAction *action = mModelCmd->findAction(actionItem->mName);
                    if ( action )
                        text = action->text();
                }
            }
            return text;
        }
    }
    else if ( role == Qt::DecorationRole )
    {
        const TBaseItem &item = mGroupItem->children().at( index.row() );
        if ( item->isAction() )
        {
            ActionItem *actionItem = static_cast<ActionItem *>(item.data());
            QAction *cmd = mModelCmd->findAction(actionItem->mName);
            QIcon icon;
            if ( cmd )
                icon = cmd->icon();
            if ( icon.isNull() || actionItem->mStyle == ActionItem::TextOnly )
                icon = iconTransparent;
            return icon;
        }
        else
        {
            return iconTransparent;
        }
    }
    else if ( role == userRoleItemType )
    {
        const TBaseItem &item = mGroupItem->children().at( index.row() );
        if ( item->isSeparator() )
            return int(Separator);
        if ( item->isGroup() )
            return int(Group);
        else if ( item->isAction() )
        {
            return ( static_cast<ActionItem *>(item.data())->isToolbutton() ?
                     int(Toolbutton) : int(Action) );
        }
    }
    else if ( role == userRoleStyle )
    {
        const TBaseItem &item = mGroupItem->children().at( index.row() );
        if ( item->isAction() )
            return static_cast<ActionItem *>(item.data())->mStyle;
    }
    else if ( role == userRoleToolTip )
    {
        const TBaseItem &item = mGroupItem->children().at( index.row() );
        if ( item->isAction() )
            return static_cast<ActionItem *>(item.data())->mToolTip;
    }

    return QVariant();
}

bool BaseUiModel::insertSeparator(int row)
{
    Q_ASSERT( mGroupItem );

    beginInsertRows(QModelIndex(), row, row);

    mGroupItem->addSeparator();
    mGroupItem->move(mGroupItem->count()-1, row);

    endInsertRows();

    return true;
}

bool BaseUiModel::insertGroup(int row)
{
    Q_ASSERT( mGroupItem );

    beginInsertRows(QModelIndex(), row, row);

    // Add empty group.
    mGroupItem->addGroup(QString(""));
    mGroupItem->move(mGroupItem->count()-1, row);

    endInsertRows();
    return true;
}

bool BaseUiModel::removeRow(int row)
{
    Q_ASSERT( mGroupItem );

    if ( row < 0 || row >= mGroupItem->count() )
        return false;

    beginRemoveRows(QModelIndex(), row, row);

    mGroupItem->removeAt(row);

    endRemoveRows();
    return true;
}

void BaseUiModel::swapRows(int i, int j)
{
    Q_ASSERT( mGroupItem );

    if ( i == j || i < 0 || i >= mGroupItem->count() || j < 0 || j >= mGroupItem->count() )
        return;

    beginMoveRows(QModelIndex(), i, i, QModelIndex(), j);

    mGroupItem->swap(i, j);

    endMoveRows();
}

bool BaseUiModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if ( !index.isValid() || !mGroupItem ||
         index.row() < 0 || index.row() >= mGroupItem->count() )
        return false;

    if ( role == Qt::DisplayRole )
    {
        const TBaseItem &item = mGroupItem->children().at( index.row() );
        if ( item->isAction() )
        {
            static_cast<ActionItem *>(item.data())->mText = value.toString();
            emit dataChanged(index, index);
        }
        else if ( item->isGroup() )
        {
            static_cast<GroupItem *>(item.data())->setText( value.toString() );
            emit dataChanged(index, index);
        }
    }
    else if ( role == userRoleStyle )
    {
        const TBaseItem &item = mGroupItem->children().at( index.row() );
        if ( item->isAction() )
        {
            static_cast<ActionItem *>(item.data())->mStyle = ActionItem::Style( value.toInt() );
            emit dataChanged(index, index);
        }
    }
    else if ( role == userRoleToolTip )
    {
        const TBaseItem &item = mGroupItem->children().at( index.row() );
        if ( item->isAction() )
        {
            static_cast<ActionItem *>(item.data())->mToolTip = value.toString();
            emit dataChanged(index, index);
        }
    }

    return false;
}


////////////////////////////////////////////////////////////////////////////////////
// MenuUiModel implementation

QVariant MenuUiModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() || !mGroupItem ||
         index.row() < 0 || index.row() >= mGroupItem->count() )
        return QVariant();

    if ( role == Qt::DisplayRole )
    {
        const TBaseItem &item = mGroupItem->children().at( index.row() );
        if ( item->isSeparator() )
            return gSeparatorLine;
        else if ( item->isGroup() )
        {
            GroupItem *groupItem = static_cast<GroupItem *>(item.data());
            return ( groupItem->text() + tr(" : popup menu"));
        }
        else if ( item->isAction() )
        {
            ActionItem *actionItem = static_cast<ActionItem *>(item.data());
            QString displayText;
            if ( actionItem->mStyle != ActionItem::IconOnly )
            {
                displayText = actionItem->mText;
                // If text was not defined by the user - use translated action text instead.
                if ( displayText.isEmpty() )
                {
                    if ( actionItem->mIsSubMenuEnabled )
                    {
                        QMenu *menu = mModelCmd->findMenu(actionItem->mName);
                        if ( menu )
                            displayText = menu->title();
                    }
                    else
                    {
                        QAction *action = mModelCmd->findAction(actionItem->mName);
                        if ( action )
                            displayText = action->text();
                    }
                }
            }
            if ( actionItem->mIsSubMenuEnabled )
                return ( displayText + tr(" : submenu \"%1\"").arg(actionItem->mName));
            return displayText;
        }
    }

    return BaseUiModel::data(index, role);
}

bool MenuUiModel::insertAction(int row, const QString &actionClassName, bool isSubmenu)
{
    Q_ASSERT( mGroupItem );

    QAction *cmd = 0;
    if ( !isSubmenu )
    {
        cmd = mModelCmd->findAction(actionClassName);
        if ( !cmd )
            return false;
    }

    beginInsertRows(QModelIndex(), row, row);

    // Add empty action.
    mGroupItem->addAction(QString(), actionClassName,
                          ActionItem::TextBesideIcon, isSubmenu);
    mGroupItem->move(mGroupItem->count()-1, row);

    endInsertRows();
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
// ToolbarUiModel implementation

QVariant ToolbarUiModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() || !mGroupItem ||
         index.row() < 0 || index.row() >= mGroupItem->count() )
        return QVariant();

    if ( role == Qt::DisplayRole )
    {
        const TBaseItem &item = mGroupItem->children().at( index.row() );
        if ( item->isGroup() )
        {
            GroupItem *groupItem = static_cast<GroupItem *>(item.data());
            return ( groupItem->text() + tr(" : group"));
        }
        else if ( item->isAction() )
        {
            ActionItem *actionItem = static_cast<ActionItem *>(item.data());
            if ( actionItem->isToolbutton() )
            {
                ToolbuttonItem *toolbuttonItem = static_cast<ToolbuttonItem *>(item.data());

                QString displayText;
                if ( actionItem->mStyle != ActionItem::IconOnly )
                {
                    displayText = actionItem->mText;
                    // If text was not defined by the user - use translated action/menu text instead.
                    if ( displayText.isEmpty() )
                    {
                        if ( actionItem->mIsSubMenuEnabled )
                        {
                            QMenu *menu = mModelCmd->findMenu(actionItem->mName);
                            if ( menu )
                                displayText = menu->title();
                        }
                        else
                        {
                            QAction *action = mModelCmd->findAction(actionItem->mName);
                            if ( action )
                                displayText = action->text();
                        }
                    }
                }
                displayText += QString(" :");
                if ( actionItem->mIsSubMenuEnabled )
                    displayText += tr(" submenu \"%1\"").arg(actionItem->mName);
                displayText += tr(" row = \"%1\" column = \"%2\"").arg(toolbuttonItem->mRow).arg(toolbuttonItem->mColumn);
                if ( toolbuttonItem->mRowSpan > 1 )
                    displayText += tr(" rowSpan = \"%1\"").arg(toolbuttonItem->mRowSpan);
                if ( toolbuttonItem->mColumnSpan > 1 )
                    displayText += tr(" columnSpan = \"%1\"").arg(toolbuttonItem->mColumnSpan);
                if ( toolbuttonItem->mAlignment != ToolbuttonItem::AlignLeft )
                    displayText += tr(" alignment = \"%1\"").arg(
                            toolbuttonItem->mAlignment == ToolbuttonItem::AlignRight ? tr( gAlignRightText ) :
                            (toolbuttonItem->mAlignment == ToolbuttonItem::AlignHCenter ? tr( gAlignHCenterText ) :
                             (toolbuttonItem->mAlignment == ToolbuttonItem::AlignTop ? tr( gAlignTopText ) :
                              (toolbuttonItem->mAlignment == ToolbuttonItem::AlignBottom ? tr( gAlignBottomText ) :
                               (toolbuttonItem->mAlignment == ToolbuttonItem::AlignVCenter ? tr( gAlignVCenterText ) :
                                (toolbuttonItem->mAlignment == ToolbuttonItem::AlignCenter ? tr( gAlignCenterText ) :
                                 tr( gAlignLeftText )))))));
                if ( toolbuttonItem->mIconSize > 0 )
                    displayText += tr(" iconSize = \"%1\"").arg(toolbuttonItem->mIconSize);
                if ( toolbuttonItem->mSizePolicy != ToolbuttonItem::Fixed )
                    displayText += tr(" sizePolicy = \"%1\"").arg(
                            toolbuttonItem->mSizePolicy == ToolbuttonItem::Minimum ?
                            tr("minimum") : tr("fixed"));
                if ( !toolbuttonItem->mToolTip.isEmpty() )
                    displayText += tr(" toolTip = \"%1\"").arg(toolbuttonItem->mToolTip);
                return displayText;
            }
        }
    }
    else if ( role == userRoleRow )
    {
        const TBaseItem &item = mGroupItem->children().at( index.row() );
        if ( item->isAction() )
        {
            ActionItem *actionItem = static_cast<ActionItem *>(item.data());
            if ( actionItem->isToolbutton() )
                return static_cast<ToolbuttonItem *>(item.data())->mRow;
        }
    }
    else if ( role == userRoleRowSpan )
    {
        const TBaseItem &item = mGroupItem->children().at( index.row() );
        if ( item->isAction() )
        {
            ActionItem *actionItem = static_cast<ActionItem *>(item.data());
            if ( actionItem->isToolbutton() )
                return static_cast<ToolbuttonItem *>(item.data())->mRowSpan;
        }
    }
    else if ( role == userRoleColumn )
    {
        const TBaseItem &item = mGroupItem->children().at( index.row() );
        if ( item->isAction() )
        {
            ActionItem *actionItem = static_cast<ActionItem *>(item.data());
            if ( actionItem->isToolbutton() )
                return static_cast<ToolbuttonItem *>(item.data())->mColumn;
        }
    }
    else if ( role == userRoleColumnSpan )
    {
        const TBaseItem &item = mGroupItem->children().at( index.row() );
        if ( item->isAction() )
        {
            ActionItem *actionItem = static_cast<ActionItem *>(item.data());
            if ( actionItem->isToolbutton() )
                return static_cast<ToolbuttonItem *>(item.data())->mColumnSpan;
        }
    }
    else if ( role == userRoleAlignment )
    {
        const TBaseItem &item = mGroupItem->children().at( index.row() );
        if ( item->isAction() )
        {
            ActionItem *actionItem = static_cast<ActionItem *>(item.data());
            if ( actionItem->isToolbutton() )
                return int( static_cast<ToolbuttonItem *>(item.data())->mAlignment );
        }
    }
    else if ( role == userRoleIconSize )
    {
        const TBaseItem &item = mGroupItem->children().at( index.row() );
        if ( item->isAction() )
        {
            ActionItem *actionItem = static_cast<ActionItem *>(item.data());
            if ( actionItem->isToolbutton() )
                return static_cast<ToolbuttonItem *>(item.data())->mIconSize;
        }
    }
    else if ( role == userRoleSizePolicy )
    {
        const TBaseItem &item = mGroupItem->children().at( index.row() );
        if ( item->isAction() )
        {
            ActionItem *actionItem = static_cast<ActionItem *>(item.data());
            if ( actionItem->isToolbutton() )
                return int( static_cast<ToolbuttonItem *>(item.data())->mSizePolicy );
        }
    }

    return BaseUiModel::data(index, role);
}

bool ToolbarUiModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if ( !index.isValid() || !mGroupItem ||
         index.row() < 0 || index.row() >= mGroupItem->count() )
        return false;

    // Change row in a grid layout.
    if ( role == userRoleRow )
    {
        const TBaseItem &item = mGroupItem->children().at( index.row() );
        if ( item->isAction() )
        {
            ActionItem *actionItem = static_cast<ActionItem *>(item.data());
            if ( actionItem->isToolbutton() )
            {
                ToolbuttonItem *toolbuttonItem = static_cast<ToolbuttonItem *>(item.data());
                toolbuttonItem->mRow = value.toInt();

                emit dataChanged(index, index);
            }
        }
    }
    // Change row in a grid layout.
    else if ( role == userRoleRowSpan )
    {
        const TBaseItem &item = mGroupItem->children().at( index.row() );
        if ( item->isAction() )
        {
            ActionItem *actionItem = static_cast<ActionItem *>(item.data());
            if ( actionItem->isToolbutton() )
            {
                ToolbuttonItem *toolbuttonItem = static_cast<ToolbuttonItem *>(item.data());
                toolbuttonItem->mRowSpan = value.toInt();

                emit dataChanged(index, index);
            }
        }
    }
    // Change column in a grid layout.
    else if ( role == userRoleColumn )
    {
        const TBaseItem &item = mGroupItem->children().at( index.row() );
        if ( item->isAction() )
        {
            ActionItem *actionItem = static_cast<ActionItem *>(item.data());
            if ( actionItem->isToolbutton() )
            {
                ToolbuttonItem *toolbuttonItem = static_cast<ToolbuttonItem *>(item.data());
                toolbuttonItem->mColumn = value.toInt();

                emit dataChanged(index, index);
            }
        }
    }
    // Change column spanning in a grid layout.
    else if ( role == userRoleColumnSpan )
    {
        const TBaseItem &item = mGroupItem->children().at( index.row() );
        if ( item->isAction() )
        {
            ActionItem *actionItem = static_cast<ActionItem *>(item.data());
            if ( actionItem->isToolbutton() )
            {
                ToolbuttonItem *toolbuttonItem = static_cast<ToolbuttonItem *>(item.data());
                toolbuttonItem->mColumnSpan = value.toInt();

                emit dataChanged(index, index);
            }
        }
    }
    // Set alignment of a cell in a grid layout.
    else if ( role == userRoleAlignment )
    {
        const TBaseItem &item = mGroupItem->children().at( index.row() );
        if ( item->isAction() )
        {
            ActionItem *actionItem = static_cast<ActionItem *>(item.data());
            if ( actionItem->isToolbutton() )
            {
                ToolbuttonItem *toolbuttonItem = static_cast<ToolbuttonItem *>(item.data());
                if ( value.type() != QVariant::String )
                    toolbuttonItem->mAlignment = (ToolbuttonItem::Alignment) value.toInt();
                else
                {
                    int i = 0;
                    QString valueAlignment = value.toString();
                    foreach ( const QString &align, alignmentList() )
                    {
                        if ( valueAlignment == align )
                        {
                            toolbuttonItem->mAlignment = (ToolbuttonItem::Alignment) i;
                            break;
                        }
                        i++;
                    }
                }

                emit dataChanged(index, index);
            }
        }
    }
    // Change icon size of a toolbutton.
    else if ( role == userRoleIconSize )
    {
        const TBaseItem &item = mGroupItem->children().at( index.row() );
        if ( item->isAction() )
        {
            ActionItem *actionItem = static_cast<ActionItem *>(item.data());
            if ( actionItem->isToolbutton() )
            {
                ToolbuttonItem *toolbuttonItem = static_cast<ToolbuttonItem *>(item.data());
                toolbuttonItem->mIconSize = value.toInt();

                emit dataChanged(index, index);
            }
        }
    }
    // Change size policy on width and height of a toolbutton.
    else if ( role == userRoleSizePolicy )
    {
        const TBaseItem &item = mGroupItem->children().at( index.row() );
        if ( item->isAction() )
        {
            ActionItem *actionItem = static_cast<ActionItem *>(item.data());
            if ( actionItem->isToolbutton() )
            {
                ToolbuttonItem *toolbuttonItem = static_cast<ToolbuttonItem *>(item.data());
                toolbuttonItem->mSizePolicy = ToolbuttonItem::SizePolicy( value.toInt() );

                emit dataChanged(index, index);
            }
        }
    }

    return BaseUiModel::setData(index, value, role);
}

bool ToolbarUiModel::insertAction(int row, const QString &actionClassName, bool isSubmenu)
{
    Q_ASSERT( mGroupItem );

    QAction *cmd = 0;
    if ( !isSubmenu )
    {
        cmd = mModelCmd->findAction(actionClassName);
        if ( !cmd )
            return false;
    }

    beginInsertRows(QModelIndex(), row, row);

    // Add empty toolbutton.
    mGroupItem->addToolbutton(QString(), actionClassName,
                              ActionItem::TextBesideIcon, isSubmenu, -1, -1);
    mGroupItem->move(mGroupItem->count()-1, row);

    endInsertRows();
    return true;
}

void ToolbarUiModel::setGridPosition(const QModelIndex &index, int row, int column)
{
    if ( !index.isValid() || index.row() < 0 || index.row() >= mGroupItem->count() )
        return;

    const TBaseItem &item = mGroupItem->children().at( index.row() );
    if ( item->isAction() )
    {
        ActionItem *actionItem = static_cast<ActionItem *>(item.data());
        if ( actionItem->isToolbutton() )
        {
            ToolbuttonItem *toolbuttonItem = static_cast<ToolbuttonItem *>(item.data());
            toolbuttonItem->mRow = row;
            toolbuttonItem->mColumn = column;
        }
    }
}

QStringList ToolbarUiModel::alignmentList() const
{
    QStringList aligns;
    aligns << tr( gAlignLeftText )
            << tr( gAlignRightText )
            << tr( gAlignHCenterText )
            << tr( gAlignTopText )
            << tr( gAlignBottomText )
            << tr( gAlignVCenterText )
            << tr( gAlignCenterText );
    return aligns;
}

}   // namespace Uim
