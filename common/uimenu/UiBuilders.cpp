#include "UiBuilders.h"
#include "ActionUiGroup.h"

#include <QtCore/QMap>
#include <QtGui/QMenuBar>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QContextMenuEvent>
#include <QtGui/QApplication>

namespace Uim
{

static const char *g_textMenu = QT_TRANSLATE_NOOP("UiBuilder", "Menu");


// Forward declaration.
namespace MenuGroupBuilder
{
    void build(QMenu *menuGroup, const TGroupItem &groupItem,
               QMainWindow *mainWindow, const UiModel &uiModel,
               const AbstractUiModel &cmdModel);
}


//! Builds and stores submenus for the application.
class SubmenuBuilder
{
public:
    SubmenuBuilder(QMainWindow *mainWindow, const UiModel &uiModel,
                   const AbstractUiModel &cmdModel)
        : mMainWindow(mainWindow), mUiModel(uiModel), mCmdModel(cmdModel)
    {}

    //! Finds Qt menu or creates it if it was not built yet.
    QMenu* find(const QString &name)
    {
        SubmenuMap::const_iterator itc = mSubmenuMap.find(name);
        if ( itc != mSubmenuMap.constEnd() )
            return *itc;

        // Find submenu in the prebuilt menu group.
        QMenu *menuPrebuilt = mCmdModel.findMenu(name);
        if ( menuPrebuilt )
            return menuPrebuilt;

        // Build submenu ...

        const GroupItem::ItemsList &items = mUiModel.mSubMenus->children();
        foreach ( const TBaseItem &item, items )
        {
            if ( item->isGroup() )
            {
                TGroupItem groupItem = item.staticCast<GroupItem>();
                Q_ASSERT(groupItem);

                if ( groupItem->text() == name && !groupItem->isEmpty() )
                {
                    QMenu *menuGroup = new QMenu(name, mMainWindow);
                    MenuGroupBuilder::build(menuGroup, groupItem, mMainWindow, mUiModel, mCmdModel);

                    mSubmenuMap.insert(name, menuGroup);
                    return menuGroup;
                }
            }
        }

        return 0;
    }

private:
    /** first - submenu name, second - submenu object */
    typedef QMap<QString, QMenu*>   SubmenuMap;
    SubmenuMap          mSubmenuMap;

    //! Reference to the main window.
    QMainWindow             *mMainWindow;
    /** Reference to the submenu group */
    const UiModel           &mUiModel;
    /** Reference to the actions model */
    const AbstractUiModel   &mCmdModel;
};


namespace MenuGroupBuilder
{
    //! Builds menu group.
    /*!
        \param menuGroup Menu to add items
        \param groupItem Container of items for menu
        \param mainWindow Reference to the main window os application
        \param uiModel Reference to the Ui items' containers
        \param cmdModel Reference to the available commands in the application
    */
    void build(QMenu *menuGroup, const TGroupItem &groupItem,
               QMainWindow *mainWindow, const UiModel &uiModel,
               const AbstractUiModel &cmdModel)
    {
        if ( !menuGroup || !groupItem )
            return;

        const GroupItem::ItemsList &items = groupItem->children();
        foreach ( const TBaseItem &item, items )
        {
            if ( item->isGroup() )
            {
                TGroupItem groupItemNext = item.staticCast<GroupItem>();
                Q_ASSERT(groupItemNext);

                QMenu *menuGroupNext = menuGroup->addMenu(groupItemNext->text());
                build(menuGroupNext, groupItemNext, mainWindow, uiModel, cmdModel);
            }
            else if ( item->isSeparator() )
                menuGroup->addSeparator();
            else if ( item->isAction() )
            {
                ActionItem *actionItem = static_cast<ActionItem *>(item.data());
                if ( actionItem->mIsSubMenuEnabled )
                {
                    QMenu *submenu = SubmenuBuilder(mainWindow, uiModel, cmdModel).find(actionItem->mName);
                    if ( submenu )
                    {
                        // If user changed name of menu title new title should be used instead
                        // of fixed menu title.
                        submenu->setTitle(actionItem->mText.isEmpty() ? submenu->title() : actionItem->mText);
                        // Need this property to prompt about text translation on the application side.
                        submenu->setProperty(userTextProperty, !actionItem->mText.isEmpty());
                        menuGroup->addMenu(submenu);
                    }
                }
                else
                {
                    QAction *cmd = cmdModel.findAction(actionItem->mName);
                    if ( cmd )
                    {
                        cmd->setIconVisibleInMenu(actionItem->mStyle != ActionItem::TextOnly);
                        // If user changed name of action new text should be used instead
                        // of fixed action text.
                        cmd->setText(actionItem->mText.isEmpty() ? cmd->text() : actionItem->mText);
                        // Need this property to prompt about text translation on the application side.
                        cmd->setProperty(userTextProperty, !actionItem->mText.isEmpty());
                        menuGroup->addAction(cmd);
                    }
                }
            }
        }
    }
}

class ToolbarBuilder
{
public:
    ToolbarBuilder(QMainWindow *mainWindow, const UiModel &uiModel,
                   const AbstractUiModel &cmdModel)
        : mMainWindow(mainWindow), mUiModel(uiModel), mCmdModel(cmdModel)
    {}

    void build(QTabWidget *tabWidget)
    {
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy);
        tabWidget->setFocusPolicy(Qt::NoFocus);
        tabWidget->setTabShape(QTabWidget::Rounded);
        tabWidget->setElideMode(Qt::ElideNone);
        tabWidget->setUsesScrollButtons(true);
        tabWidget->setDocumentMode(false);
        tabWidget->setTabsClosable(false);
        tabWidget->setMovable(false);

        const GroupItem::ItemsList &items = mUiModel.mToolbar->children();
        foreach ( const TBaseItem &item, items )
        {
            if ( item->isGroup() )
            {
                TGroupItem groupItem = item.staticCast<GroupItem>();
                Q_ASSERT(groupItem);

                QWidget *tab = buildTab(groupItem);
                if ( tab )
                {
                    tabWidget->addTab(tab, QString());
                    tabWidget->setTabText(tabWidget->indexOf(tab), groupItem->text());
                }
            }
        }

        tabWidget->setCurrentIndex(0);
    }

private:
    typedef QPair<int, int>                     TKeyCell;
    typedef QMap<TKeyCell, ToolbuttonItem *>    TToolbuttons;

    QWidget* buildTab(const TGroupItem &tabGroupItem)
    {
        QWidget *tab = new QWidget();
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tab->sizePolicy().hasHeightForWidth());
        tab->setSizePolicy(sizePolicy);

        QHBoxLayout *horizontalLayout = new QHBoxLayout(tab);
        horizontalLayout->setSpacing(2);
        horizontalLayout->setContentsMargins(2, 2, 2, 2);

        const GroupItem::ItemsList &items = tabGroupItem->children();
        foreach ( const TBaseItem &item, items )
        {
            if ( item->isGroup() )
            {
                TGroupItem groupItem = item.staticCast<GroupItem>();
                Q_ASSERT(groupItem);

                if ( groupItem->count() < 1 )
                    continue;

                QGroupBox *groupBox = buildGroup(tab, groupItem);
                if ( groupBox )
                    horizontalLayout->addWidget(groupBox);
            }
        }

        QSpacerItem *horizontalSpacer = new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        return tab;
    }

    QGroupBox* buildGroup(QWidget *parentWidget, const TGroupItem &groupItem)
    {
        QGroupBox *groupBox = new QGroupBox(parentWidget);

        QVBoxLayout *verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setSpacing(2);
        verticalLayout->setContentsMargins(0, 0, 0, 0);

        QWidget *widget = new QWidget(groupBox);

        buildButtons(widget, groupItem);

        verticalLayout->addWidget(widget);

        QLabel *label = new QLabel(groupBox);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 170, 255);\n"
                                "color: rgb(0, 85, 255);"));
        label->setAlignment(Qt::AlignCenter);
        label->setText(groupItem->text());

        verticalLayout->addWidget(label);

        return groupBox;
    }

    void buildButtons(QWidget *parentWidget, const TGroupItem &groupItem)
    {
        const GroupItem::ItemsList &items = groupItem->children();
        if ( items.count() < 1 )
            return;

        TToolbuttons toolbuttonMap;
        orderToolbuttons(groupItem, toolbuttonMap);

        // Not all [row,column] pairs were set properly - build Vertical or Horizontal
        // layout then.
        if ( toolbuttonMap.count() != items.count() )
        {
            // 1, or 2 buttons we add in a vertical layout.
            // More than 2 buttons we add in a horizontal layout.
            QBoxLayout *layout = items.size() < 2 ?
                                 (static_cast<QBoxLayout *>(new QVBoxLayout(parentWidget))) :
                                 (static_cast<QBoxLayout *>(new QHBoxLayout(parentWidget)));
            layout->setSpacing(2);
            layout->setContentsMargins(2, 2, 2, 2);

            foreach ( const TBaseItem &item, items )
            {
                if ( item->isAction() )
                {
                    ToolbuttonItem *toolButtonItem = static_cast<ToolbuttonItem *>(item.data());

                    QToolButton *toolButton = buildToolbutton(parentWidget, toolButtonItem);
                    if ( toolButton )
                        layout->addWidget(toolButton);
                }
            }
        }
        // Build Grid layout depending on the [row,column] cell values.
        else
        {
            QGridLayout *gridLayout = new QGridLayout(parentWidget);
            gridLayout->setSpacing(2);
            gridLayout->setContentsMargins(2, 2, 2, 2);

            for ( TToolbuttons::const_iterator itc = toolbuttonMap.constBegin();
                itc != toolbuttonMap.constEnd(); ++itc )
            {
                int row = itc.key().first,
                    column = itc.key().second;
                if ( row < 0 || column < 0 )
                    continue;

                const ToolbuttonItem *toolButtonItem = *itc;

                QToolButton *toolButton = buildToolbutton(parentWidget, toolButtonItem);
                if ( toolButton )
                    gridLayout->addWidget(toolButton, row, column,
                                          toolButtonItem->mRowSpan, toolButtonItem->mColumnSpan,
                                          toQtAlignment(toolButtonItem->mAlignment));
            }
        }
    }

    QToolButton* buildToolbutton(QWidget *parentWidget, const ToolbuttonItem *item)
    {
        QToolButton *toolButton = new QToolButton(parentWidget);

        QIcon icon;
        QString text;
        if ( item->mIsSubMenuEnabled )
        {
            QMenu *submenu = SubmenuBuilder(mMainWindow, mUiModel, mCmdModel).find(item->mName);
            if ( submenu )
            {
                icon = submenu->icon();
                text = submenu->title();

                submenu->setTitle(item->mText.isEmpty() ? text : item->mText);
                // Need this property to prompt about text translation on the application side.
                submenu->setProperty(userTextProperty, !item->mText.isEmpty());

                toolButton->setPopupMode(QToolButton::MenuButtonPopup);
                toolButton->setMenu(submenu);
                toolButton->setDefaultAction(submenu->menuAction());
                // Lets show popup menu when click on the text area as well as click on the
                // down arrow on the left of toolButton.
                QObject::connect(toolButton, SIGNAL(clicked()), toolButton, SLOT(showMenu()));
            }
        }
        else
        {
            QAction *cmd = mCmdModel.findAction(item->mName);
            if ( cmd )
            {
                toolButton->setDefaultAction(cmd);
                icon = cmd->icon();
                text = cmd->text();

                // Need this property to prompt about text translation on the application side.
                cmd->setProperty(userTextProperty, !item->mText.isEmpty());
                // As default action is used for a toolbutton the only way to change
                // a toolbutton's text is to change actions text.
                cmd->setText(item->mText.isEmpty() ? text : item->mText);
                // Need this property to prompt about tool tip translation on the application side.
                cmd->setToolTip(item->mToolTip.isEmpty() ? cmd->toolTip() : item->mToolTip);
            }
        }

        if ( item->mStyle != ActionItem::TextOnly && !icon.isNull() )
        {
            toolButton->setIcon(icon);
            Qt::ToolButtonStyle style = item->mStyle == ActionItem::IconOnly ? Qt::ToolButtonIconOnly :
                                        (item->mStyle == ActionItem::TextUnderIcon ? Qt::ToolButtonTextUnderIcon :
                                         Qt::ToolButtonTextBesideIcon);
            toolButton->setToolButtonStyle(style);
        }
        else
            toolButton->setToolButtonStyle(Qt::ToolButtonTextOnly);

        if ( item->mIconSize > 0 )
            toolButton->setIconSize(QSize(item->mIconSize, item->mIconSize));

        if ( item->mSizePolicy == ToolbuttonItem::Minimum )
        {
            QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
            sizePolicy.setHorizontalStretch(0);
            sizePolicy.setVerticalStretch(0);
            sizePolicy.setHeightForWidth(toolButton->sizePolicy().hasHeightForWidth());
            toolButton->setSizePolicy(sizePolicy);

            if ( item->mIconSize > 0 )
                toolButton->setMinimumSize(QSize(item->mIconSize, item->mIconSize));
        }

        return toolButton;
    }

    void orderToolbuttons(const TGroupItem &groupItem, TToolbuttons &toolbuttonMap)
    {
        const GroupItem::ItemsList &items = groupItem->children();
        foreach ( const TBaseItem &item, items )
        {
            if ( item->isAction() )
            {
                ToolbuttonItem *toolButtonItem = static_cast<ToolbuttonItem *>(item.data());
                if ( toolButtonItem->mRow < 0 || toolButtonItem->mColumn < 0 )
                    continue;

                toolbuttonMap.insert( TKeyCell(toolButtonItem->mRow, toolButtonItem->mColumn),
                                      toolButtonItem );
            }
        }
    }

    Qt::Alignment toQtAlignment(ToolbuttonItem::Alignment alignment)
    {
        Qt::Alignment alignmentQt = 0;
        if ( alignment == ToolbuttonItem::AlignLeft )
            alignmentQt = 0;
        else if ( alignment == ToolbuttonItem::AlignRight )
            alignmentQt = Qt::AlignRight;
        else if ( alignment == ToolbuttonItem::AlignHCenter )
            alignmentQt = Qt::AlignHCenter;
        else if ( alignment == ToolbuttonItem::AlignTop )
            alignmentQt = Qt::AlignTop;
        else if ( alignment == ToolbuttonItem::AlignBottom )
            alignmentQt = Qt::AlignBottom;
        else if ( alignment == ToolbuttonItem::AlignVCenter )
            alignmentQt = Qt::AlignVCenter;
        else if ( alignment == ToolbuttonItem::AlignCenter )
            alignmentQt = Qt::AlignCenter;
        return alignmentQt;
    }

private:
    //! Reference to the main window.
    QMainWindow             *mMainWindow;
    /** Reference to the submenu group */
    const UiModel           &mUiModel;
    /** Reference to the actions model */
    const AbstractUiModel   &mCmdModel;
};

namespace ToolbarMenuBuilder
{
    void build(QMainWindow *mainWindow, QTabWidget *tabWidget,
               const UiModel &uiModel, const AbstractUiModel &cmdModel)
    {
        // Menu tool button
        QToolButton* toolButtonMenu = new QToolButton(tabWidget);
        toolButtonMenu->setGeometry(QRect(0, 0, 23, 19));
        toolButtonMenu->setPopupMode(QToolButton::MenuButtonPopup);
        toolButtonMenu->setAutoRaise(true);
        QFont fontMenu;
        fontMenu.setBold(true);
        fontMenu.setWeight(75);
        toolButtonMenu->setFont(fontMenu);
        // Lets show popup menu when click on the text area as well as click on the
        // down arrow on the left of toolButton.
        QObject::connect(toolButtonMenu, SIGNAL(clicked()), toolButtonMenu, SLOT(showMenu()));

        // Create menu group.

        QMenu *menuGroup = new QMenu(toolButtonMenu);
        MenuGroupBuilder::build(menuGroup, uiModel.mToolbarMenu, mainWindow, uiModel, cmdModel);

        toolButtonMenu->setMenu(menuGroup);

        // Set text for toolbutton.
        toolButtonMenu->setText( QApplication::translate("UiBuilder", g_textMenu) );

        tabWidget->setCornerWidget(toolButtonMenu, Qt::TopLeftCorner);
    }
}

class ToolbarRightBarBuilder
{
public:
    ToolbarRightBarBuilder(QMainWindow *mainWindow, const UiModel &uiModel,
                           const AbstractUiModel &cmdModel)
        : mMainWindow(mainWindow), mUiModel(uiModel), mCmdModel(cmdModel)
    {}

    void build(QTabWidget *tabWidget)
    {
        const GroupItem::ItemsList &items = mUiModel.mRightBar->children();

        int countActions = 0;
        foreach ( const TBaseItem &item, items )
        {
            if ( item->isAction() )
                countActions++;
        }
        // Make sure actions are defined for the right bar.
        if ( countActions < 1 )
            return;

        QWidget *widgetContainer = new QWidget(tabWidget);

        QHBoxLayout *horizontalLayout = new QHBoxLayout(widgetContainer);
        horizontalLayout->setSpacing(2);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);

        foreach ( const TBaseItem &item, items )
        {
            if ( item->isAction() )
            {
                ActionItem *actionItem = static_cast<ActionItem *>(item.data());

                QToolButton* toolButton = buildToolbutton(widgetContainer, actionItem);
                if ( toolButton )
                    horizontalLayout->addWidget(toolButton);
            }
        }

        tabWidget->setCornerWidget(widgetContainer, Qt::TopRightCorner);
    }

private:
    QToolButton* buildToolbutton(QWidget *parentWidget, const ActionItem *item)
    {
        QToolButton *toolButton = new QToolButton(parentWidget);
        toolButton->setAutoRaise(true);

        QIcon icon;
        QString text;
        if ( item->mIsSubMenuEnabled )
        {
            QMenu *submenu = SubmenuBuilder(mMainWindow, mUiModel, mCmdModel).find(item->mName);
            if ( submenu )
            {
                icon = submenu->icon();
                text = submenu->title();

                submenu->setTitle(item->mText.isEmpty() ? text : item->mText);
                // Need this property to prompt about text translation on the application side.
                submenu->setProperty(userTextProperty, !item->mText.isEmpty());

                toolButton->setPopupMode(QToolButton::MenuButtonPopup);
                toolButton->setMenu(submenu);
                toolButton->setDefaultAction(submenu->menuAction());
                // Lets show popup menu when click on the text area as well as click on the
                // down arrow on the left of toolButton.
                QObject::connect(toolButton, SIGNAL(clicked()), toolButton, SLOT(showMenu()));
            }

            toolButton->setText(item->mText.isEmpty() ? text : item->mText);
        }
        else
        {
            QAction *cmd = mCmdModel.findAction(item->mName);
            if ( cmd )
            {
                toolButton->setDefaultAction(cmd);
                icon = cmd->icon();
                text = cmd->text();

                // Need this property to prompt about text translation on the application side.
                cmd->setProperty(userTextProperty, !item->mText.isEmpty());

                // As default action is used for a toolbutton the only way to change
                // a toolbutton's text is to change actions text.
                cmd->setText(item->mText.isEmpty() ? text : item->mText);
                // Need this property to prompt about tool tip translation on the application side.
                cmd->setToolTip(item->mToolTip.isEmpty() ? cmd->toolTip() : item->mToolTip);
            }
        }

        if ( item->mStyle != ActionItem::TextOnly && !icon.isNull() )
        {
            toolButton->setIcon(icon);
            Qt::ToolButtonStyle style = item->mStyle == ActionItem::IconOnly ? Qt::ToolButtonIconOnly :
                                        (item->mStyle == ActionItem::TextUnderIcon ? Qt::ToolButtonTextUnderIcon :
                                         Qt::ToolButtonTextBesideIcon);
            toolButton->setToolButtonStyle(style);
        }
        else
            toolButton->setToolButtonStyle(Qt::ToolButtonTextOnly);

        return toolButton;
    }

private:
    //! Reference to the main window.
    QMainWindow             *mMainWindow;
    /** Reference to the submenu group */
    const UiModel           &mUiModel;
    /** Reference to the actions model */
    const AbstractUiModel   &mCmdModel;
};

//! Tab widget with prebuilt context menu support.
class TabWidget : public QTabWidget
{
public:
    explicit TabWidget(QWidget *parent = 0)
        : QTabWidget(parent),
        mMainWindow(dynamic_cast<QMainWindow *>(parent))
    {}

protected:
    virtual void contextMenuEvent( QContextMenuEvent * event )
    {
        QTabWidget::contextMenuEvent(event);

        // Show main window's popup menu.
        QMenu *menuPopup = mMainWindow ? mMainWindow->createPopupMenu() : 0;
        if ( menuPopup )
        {
            if ( !menuPopup->isEmpty() )
            {
                menuPopup->setAttribute(Qt::WA_DeleteOnClose);
                menuPopup->popup(event->globalPos());
                event->accept();
            }
            else
                delete menuPopup;
        }
    }

private:
    /** Reference to the main window */
    QMainWindow     *mMainWindow;
};



namespace UiBuilder
{
    void destroyPreviousMenu(QMainWindow *mainWindow)
    {
        QTabWidget *tabWidget = dynamic_cast<QTabWidget *>( mainWindow->menuWidget() );
        if ( tabWidget )
            mainWindow->setMenuWidget(0);

        mainWindow->setMenuBar(0);
    }

    void buildMenu(QMainWindow *mainWindow, const UiModel &uiModel,
                   const AbstractUiModel &cmdModel)
    {
        if ( uiModel.mRootMenu->isEmpty() )
            return;

        destroyPreviousMenu(mainWindow);

        static bool gIsPopupMenuUpdated = false;
        QMenu *menuPopup = mainWindow->createPopupMenu();
        if ( menuPopup && !gIsPopupMenuUpdated )
        {
            menuPopup->addSeparator();
            menuPopup->addAction( CustomizeCmd::action(mainWindow) );
            gIsPopupMenuUpdated = true;
        }

        QMenuBar *menuBar = new QMenuBar(mainWindow);

        const GroupItem::ItemsList &items = uiModel.mRootMenu->children();
        foreach ( const TBaseItem &item, items )
        {
            if ( item->isGroup() )
            {
                TGroupItem groupItem = item.staticCast<GroupItem>();
                Q_ASSERT(groupItem);

                QMenu *menuGroup = menuBar->addMenu(groupItem->text());
                MenuGroupBuilder::build(menuGroup, groupItem, mainWindow, uiModel, cmdModel);
            }
        }

        mainWindow->setMenuBar(menuBar);
    }

    void buildToolbar(QMainWindow *mainWindow, const UiModel &uiModel,
                      const AbstractUiModel &cmdModel)
    {
        if ( uiModel.mToolbar->isEmpty() )
            return;

        destroyPreviousMenu(mainWindow);

        TabWidget *tabWidget = new TabWidget(mainWindow);

        // Build toolbar - tab control.
        ToolbarBuilder(mainWindow, uiModel, cmdModel).build(tabWidget);

        // Build toolbar's menu - right side menu.
        ToolbarMenuBuilder::build(mainWindow, tabWidget, uiModel, cmdModel);

        // Build toolbar's right bar with buttons.
        ToolbarRightBarBuilder(mainWindow, uiModel, cmdModel).build(tabWidget);

        // Set tab control as menu widget.
        mainWindow->setMenuWidget(tabWidget);
    }

    void retranslateUi(QMainWindow *mainWindow)
    {
        QTabWidget *tabWidget = dynamic_cast<QTabWidget *>( mainWindow->menuWidget() );
        if ( tabWidget )
        {
            QToolButton *toolButtonMenu = dynamic_cast<QToolButton *>(
                    tabWidget->cornerWidget(Qt::TopLeftCorner) );

            if ( toolButtonMenu )
                toolButtonMenu->setText( QApplication::translate("UiBuilder", g_textMenu) );
        }
    }
}

}   // namespace Uim
