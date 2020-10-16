#include "RearrangeDlg.h"
#include "ui_RearrangeDlg.h"

#include <QtGui/QMenu>
#include <QtGui/QButtonGroup>
#include <QtGui/QInputDialog>
#include <QtGui/QItemDelegate>
#include <QtGui/QMessageBox>

#include "AddActionDlg.h"
#include "RearrangeModels.h"
#include "CommandDelegate.h"
#include "CellEditorDlg.h"

namespace Uim
{

// Identifier for parent combo items
const int gMenubarID = 100;
const int gToolbarID = 200;
const int gToolbarMenuID = 201;
const int gToolbarRightBarID = 202;
const int gSubmenuID = 300;
// Identifier for child items
const int gChildID = 0;
const int gToolbarMenuChildID = 1;
// Group separator
const QString gGroupSeparator = " | ";
// Translations
static const char *gMenubarText = QT_TRANSLATE_NOOP("Uim::RearrangeDlg", "Menubar");
static const char *gToolbarText = QT_TRANSLATE_NOOP("Uim::RearrangeDlg", "Toolbar");
static const char *gToolbarMenuText = QT_TRANSLATE_NOOP("Uim::RearrangeDlg", "Menu");
static const char *gRightbarText = QT_TRANSLATE_NOOP("Uim::RearrangeDlg", "Rightbar");
static const char *gSubmenuText = QT_TRANSLATE_NOOP("Uim::RearrangeDlg", "Submenus");


RearrangeDlg::RearrangeDlg(const UiModel &modelUi, const AbstractUiModel *modelCmd, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RearrangeDlg),
    mModelCmd(modelCmd),
    mUiModelBase(modelUi)
{
    Q_ASSERT(mModelCmd);

    ui->setupUi(this);

    mUiModel = mUiModelBase;
    mIsPrebuiltMenuExist = mModelCmd->menus().count() > 0;

    setupUi();
    retranslateUi();
}

RearrangeDlg::~RearrangeDlg()
{
    delete ui;
}

void RearrangeDlg::setupUi()
{
    mAddMenu = new QMenu(this);
    ui->toolButtonAdd->setMenu(mAddMenu);
    connect(ui->toolButtonAdd, SIGNAL(clicked()), ui->toolButtonAdd, SLOT(showMenu()));

    mModifyMenu = new QMenu(this);
    ui->toolButtonModify->setMenu(mModifyMenu);
    connect(ui->toolButtonModify, SIGNAL(clicked()), ui->toolButtonModify, SLOT(showMenu()));

    mMenuGroup = new QButtonGroup(this);
    mMenuGroup->addButton(ui->radioButtonMenu);
    mMenuGroup->addButton(ui->radioButtonToolbar);
    mMenuGroup->addButton(ui->radioButtonSubmenu);

    connect(mMenuGroup, SIGNAL(buttonClicked(QAbstractButton *)),
            SLOT(on_buttonClicked( QAbstractButton *)));

    // Actions
    mAddSeparatorAction = new QAction(this);
    connect(mAddSeparatorAction, SIGNAL(triggered()), SLOT(on_addSeparator()));
    mAddActionAction = new QAction(this);
    connect(mAddActionAction, SIGNAL(triggered()), SLOT(on_addAction()));
    mAddSubmenuAction = new QAction(this);
    connect(mAddSubmenuAction, SIGNAL(triggered()), SLOT(on_addSubmenu()));
    mAddPrebuiltMenuAction = new QAction(this);
    connect(mAddPrebuiltMenuAction, SIGNAL(triggered()), SLOT(on_addPrebuiltMenu()));
    mAddGroupAction = new QAction(this);
    connect(mAddGroupAction, SIGNAL(triggered()), SLOT(on_addGroup()));
    //
    mModifyNameAction = new QAction(this);
    connect(mModifyNameAction, SIGNAL(triggered()), SLOT(on_modifyName()));
    mModifyToolTipAction = new QAction(this);
    connect(mModifyToolTipAction, SIGNAL(triggered()), SLOT(on_modifyToolTip()));
    mModifyStyleIconAction = new QAction(this);
    mModifyStyleIconAction->setCheckable(true);
    connect(mModifyStyleIconAction, SIGNAL(triggered()), SLOT(on_modifyStyleIcon()));
    mModifyStyleTextAction = new QAction(this);
    mModifyStyleTextAction->setCheckable(true);
    connect(mModifyStyleTextAction, SIGNAL(triggered()), SLOT(on_modifyStyleText()));
    mModifyStyleTextBesideIconAction = new QAction(this);
    mModifyStyleTextBesideIconAction->setCheckable(true);
    connect(mModifyStyleTextBesideIconAction, SIGNAL(triggered()), SLOT(on_modifyStyleTextBesideIcon()));
    mModifyStyleTextUnderIconAction = new QAction(this);
    mModifyStyleTextUnderIconAction->setCheckable(true);
    connect(mModifyStyleTextUnderIconAction, SIGNAL(triggered()), SLOT(on_modifyStyleTextUnderIcon()));
    //
    mModifyRow = new QAction(this);
    connect(mModifyRow, SIGNAL(triggered()), SLOT(on_modifyRow()));
    mModifyColumn = new QAction(this);
    connect(mModifyColumn, SIGNAL(triggered()), SLOT(on_modifyColumn()));
    mModifyAlignment = new QAction(this);
    connect(mModifyAlignment, SIGNAL(triggered()), SLOT(on_modifyAlignment()));
    //
    mModifyIconSize = new QAction(this);
    connect(mModifyIconSize, SIGNAL(triggered()), SLOT(on_modifyIconSize()));
    mModifySizePolicyFixed = new QAction(this);
    mModifySizePolicyFixed->setCheckable(true);
    connect(mModifySizePolicyFixed, SIGNAL(triggered()), SLOT(on_modifySizePolicyFixed()));
    mModifySizePolicyMinimum = new QAction(this);
    mModifySizePolicyMinimum->setCheckable(true);
    connect(mModifySizePolicyMinimum, SIGNAL(triggered()), SLOT(on_modifySizePolicyMinimum()));

    // Action style group.
    QActionGroup *actionStyleGroup = new QActionGroup(this);
    actionStyleGroup->addAction(mModifyStyleIconAction);
    actionStyleGroup->addAction(mModifyStyleTextAction);
    actionStyleGroup->addAction(mModifyStyleTextBesideIconAction);
    actionStyleGroup->addAction(mModifyStyleTextUnderIconAction);

    // Size policy group.
    QActionGroup *actionSizePolicyGroup = new QActionGroup(this);
    actionSizePolicyGroup->addAction(mModifySizePolicyFixed);
    actionSizePolicyGroup->addAction(mModifySizePolicyMinimum);

    ui->listView->setItemDelegate(new CommandDelegate(ui->listView));

    // Setup "Menu" combobox
    updateMenuCombo(true, false, false);
    // Setup "Toolbar" combobox
    updateMenuCombo(false, true, false);
    // Setup "Submenus" combobox
    updateMenuCombo(false, false, true);

    // Create default model.
    ui->radioButtonMenu->setChecked(true);
    on_buttonClicked(ui->radioButtonMenu);

    if ( ui->comboBoxMenu->count() > 0 )
        ui->comboBoxMenu->setCurrentIndex(0);
    if ( ui->comboBoxToolbar->count() > 0 )
        ui->comboBoxToolbar->setCurrentIndex(0);
    if ( ui->comboBoxSubmenu->count() > 0 )
        ui->comboBoxSubmenu->setCurrentIndex(0);

    on_comboBoxMenu_currentIndexChanged(0);
}

void RearrangeDlg::retranslateUi()
{
    mAddSeparatorAction->setText( tr("Separator") );
    mAddActionAction->setText( tr("Command") );
    mAddSubmenuAction->setText( tr("Submenu") );
    mAddPrebuiltMenuAction->setText( tr("Prebuilt Menu") );
    mAddGroupAction->setText( tr("Group") );

    mModifyNameAction->setText( tr("Name") );
    mModifyToolTipAction->setText( tr("ToolTip") );
    mModifyStyleIconAction->setText( tr("Icon Only") );
    mModifyStyleTextAction->setText( tr("Text Only") );
    mModifyStyleTextBesideIconAction->setText( tr("Text Beside Icon") );
    mModifyStyleTextUnderIconAction->setText( tr("Text Under Icon") );

    mModifyRow->setText( tr("Row") );
    mModifyColumn->setText( tr("Column") );
    mModifyAlignment->setText( tr("Alignment") );

    mModifyIconSize->setText( tr("Icon Size") );
    mModifySizePolicyFixed->setText( tr("Fixed Size") );
    mModifySizePolicyMinimum->setText( tr("Minimum Size") );

    if ( ui->comboBoxMenu->count() > 0 )
    {
        for ( int i = 0; i < ui->comboBoxMenu->count(); i++ )
        {
            int itemData = ui->comboBoxMenu->itemData(i).toInt();
            if ( itemData == gMenubarID )
                ui->comboBoxMenu->setItemText(i, tr(gMenubarText));
        }
    }
    if ( ui->comboBoxToolbar->count() > 0 )
    {
        for ( int i = 0; i < ui->comboBoxToolbar->count(); i++ )
        {
            int itemData = ui->comboBoxToolbar->itemData(i).toInt();
            if ( itemData == gToolbarID )
                ui->comboBoxToolbar->setItemText(i, tr(gToolbarText));
            else if ( itemData == gToolbarMenuID )
                ui->comboBoxToolbar->setItemText(i, tr(gToolbarMenuText));
            else if ( itemData == gToolbarRightBarID )
                ui->comboBoxToolbar->setItemText(i, tr(gRightbarText));
        }
    }
    if ( ui->comboBoxSubmenu->count() > 0 )
    {
        for ( int i = 0; i < ui->comboBoxSubmenu->count(); i++ )
        {
            int itemData = ui->comboBoxSubmenu->itemData(i).toInt();
            if ( itemData == gSubmenuID )
                ui->comboBoxSubmenu->setItemText(i, tr(gSubmenuText));
        }
    }
}

void RearrangeDlg::updateMenuCombo(bool isMenuChecked, bool isToolbarChecked,
                                   bool isSubmenuChecked)
{
    if ( isMenuChecked )
    {
        ui->comboBoxMenu->blockSignals(true);
        QString comboText = ui->comboBoxMenu->currentText();
        ui->comboBoxMenu->clear();
        // Parse main menu groups and fill combo box.
        ui->comboBoxMenu->addItem(QIcon(":/images/Menubar.png"), tr(gMenubarText), gMenubarID);
        setupMenuCombo(ui->comboBoxMenu, mUiModel.mRootMenu, QString(""), gChildID);
        // Restore last combo selection.
        if ( !comboText.isEmpty() )
            ui->comboBoxMenu->setCurrentIndex( ui->comboBoxMenu->findText(comboText) );
        ui->comboBoxMenu->blockSignals(false);
    }
    else if ( isToolbarChecked )
    {
        ui->comboBoxToolbar->blockSignals(true);
        QString comboText = ui->comboBoxToolbar->currentText();
        ui->comboBoxToolbar->clear();
        // Parse toolbar groups and fill combo box.
        ui->comboBoxToolbar->addItem(QIcon(":/images/Toolbar.png"), tr(gToolbarText), gToolbarID);
        setupMenuCombo(ui->comboBoxToolbar, mUiModel.mToolbar, QString(""), gChildID);
        ui->comboBoxToolbar->addItem(QIcon(":/images/Menubar.png"), tr(gToolbarMenuText), gToolbarMenuID);
        setupMenuCombo(ui->comboBoxToolbar, mUiModel.mToolbarMenu, QString(""), gToolbarMenuChildID);
        ui->comboBoxToolbar->addItem(QIcon(":/images/Rightbar.png"), tr(gRightbarText), gToolbarRightBarID);
        // Restore last combo selection.
        if ( !comboText.isEmpty() )
            ui->comboBoxToolbar->setCurrentIndex( ui->comboBoxToolbar->findText(comboText) );
        ui->comboBoxToolbar->blockSignals(false);
    }
    else if ( isSubmenuChecked )
    {
        ui->comboBoxSubmenu->blockSignals(true);
        QString comboText = ui->comboBoxSubmenu->currentText();
        ui->comboBoxSubmenu->clear();
        // Parse main menu groups and fill combo box.
        ui->comboBoxSubmenu->addItem(QIcon(":/images/Menubar.png"), tr(gSubmenuText), gSubmenuID);
        setupMenuCombo(ui->comboBoxSubmenu, mUiModel.mSubMenus, QString(""), gChildID);
        // Restore last combo selection.
        if ( !comboText.isEmpty() )
            ui->comboBoxSubmenu->setCurrentIndex( ui->comboBoxSubmenu->findText(comboText) );
        ui->comboBoxSubmenu->blockSignals(false);
    }
}

void RearrangeDlg::setupMenuCombo(QComboBox *combo, const TGroupItem &group,
                                  const QString &groupText, int childID)
{
    const GroupItem::ItemsList& items = group->children();
    foreach ( const TBaseItem &item, items )
    {
        if ( item->isGroup() )
        {
            TGroupItem groupCurrent = item.staticCast<GroupItem>();

            QString textPath;
            if ( !groupText.isEmpty() )
                textPath += groupText + gGroupSeparator;
            textPath += groupCurrent->text();

            combo->addItem(QIcon(":/images/Transparent.png"), textPath, childID);

            setupMenuCombo(combo, groupCurrent, textPath, childID);
        }
    }
}

void RearrangeDlg::on_buttonClicked( QAbstractButton * button )
{
    BaseUiModel *baseUiModel = 0;

    if ( ui->listView->selectionModel() )
        ui->listView->selectionModel()->clear();

    if ( button == ui->radioButtonMenu || button == ui->radioButtonSubmenu )
    {
        if ( !dynamic_cast<MenuUiModel *>(ui->listView->model()) )
            baseUiModel = new MenuUiModel(mModelCmd, ui->listView);

        if ( button == ui->radioButtonMenu )
        {
            ui->comboBoxMenu->setEnabled(true);
            ui->comboBoxSubmenu->setEnabled(false);
        }
        else
        {
            ui->comboBoxMenu->setEnabled(false);
            ui->comboBoxSubmenu->setEnabled(true);
        }
        ui->comboBoxToolbar->setEnabled(false);
    }
    else if ( button == ui->radioButtonToolbar )
    {
        int itemData = ui->comboBoxToolbar->itemData(ui->comboBoxToolbar->currentIndex()).toInt();
        if ( itemData == gToolbarID || itemData == gChildID )
        {
            if ( !dynamic_cast<ToolbarUiModel *>(ui->listView->model()) )
                baseUiModel = new ToolbarUiModel(mModelCmd, ui->listView);
        }
        // Tollbar menu and right bar is represented as model with actions - menu.
        else
        {
            if ( !dynamic_cast<MenuUiModel *>(ui->listView->model()) )
                baseUiModel = new MenuUiModel(mModelCmd, ui->listView);
        }

        ui->comboBoxMenu->setEnabled(false);
        ui->comboBoxToolbar->setEnabled(true);
        ui->comboBoxSubmenu->setEnabled(false);
    }
    else
        Q_ASSERT(false);

    if ( baseUiModel )
    {
        ui->listView->setModel(baseUiModel);

        connect(ui->listView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
                SLOT(on_listView_selectionChanged(const QItemSelection &, const QItemSelection &)));
    }

    // Fill model with data.
    if ( button == ui->radioButtonMenu )
        on_comboBoxMenu_currentIndexChanged(ui->comboBoxMenu->currentIndex());
    else if ( button == ui->radioButtonToolbar )
        on_comboBoxToolbar_currentIndexChanged(ui->comboBoxToolbar->currentIndex());
    else if ( button == ui->radioButtonSubmenu )
        on_comboBoxSubmenu_currentIndexChanged(ui->comboBoxSubmenu->currentIndex());
}

void RearrangeDlg::on_comboBoxMenu_currentIndexChanged( int index )
{
    if ( !ui->radioButtonMenu->isChecked() )
        return;

    if ( ui->listView->selectionModel() )
        ui->listView->selectionModel()->clear();

    TGroupItem groupItem;

    int itemData = ui->comboBoxMenu->itemData(index).toInt();
    if ( itemData == gMenubarID )
        groupItem = mUiModel.mRootMenu;
    else
        groupItem = findGroup(ui->comboBoxMenu->currentText(), mUiModel.mRootMenu);

    BaseUiModel *baseUiModel = dynamic_cast<BaseUiModel *>( ui->listView->model() );
    if ( baseUiModel && groupItem )
    {
        baseUiModel->setData(groupItem);

        if ( groupItem->count() )
        {
            ui->listView->selectionModel()->select(baseUiModel->index(0,0),
                                                   QItemSelectionModel::SelectCurrent);
            // FIXME: in cases when item is changed in combo box select does not
            // emit selectionChange signal.
            QModelIndex index0 = baseUiModel->index(0,0);
            on_listView_selectionChanged(QItemSelection(index0, index0),
                                         QItemSelection(index0, index0));
        }
        else
            on_listView_selectionChanged(QItemSelection(), QItemSelection());
    }

    // Set menu path in format "/View/Geodesy/Load"
    QString path = ui->comboBoxMenu->currentText();
    path.replace(gGroupSeparator, "/");
    ui->labelPath->setText( QString("/%1").arg(path) );
}

void RearrangeDlg::on_comboBoxToolbar_currentIndexChanged( int index )
{
    if ( !ui->radioButtonToolbar->isChecked() )
        return;

    if ( ui->listView->selectionModel() )
        ui->listView->selectionModel()->clear();

    TGroupItem groupItem;

    int itemData = ui->comboBoxToolbar->itemData(index).toInt();
    QString text = ui->comboBoxToolbar->currentText();

    BaseUiModel *baseUiModel = 0;

    // Update model if necessary.
    if ( itemData == gToolbarID || itemData == gChildID )
    {
        if ( !dynamic_cast<ToolbarUiModel *>(ui->listView->model()) )
            baseUiModel = new ToolbarUiModel(mModelCmd, ui->listView);
    }
    // Tollbar menu and right bar is represented as model with actions - menu.
    else
    {
        if ( !dynamic_cast<MenuUiModel *>(ui->listView->model()) )
            baseUiModel = new MenuUiModel(mModelCmd, ui->listView);
    }

    if ( baseUiModel )
    {
        ui->listView->setModel(baseUiModel);

        connect(ui->listView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
                SLOT(on_listView_selectionChanged(const QItemSelection &, const QItemSelection &)));
    }

    if ( itemData == gToolbarID )
        groupItem = mUiModel.mToolbar;
    else if ( itemData == gToolbarMenuID )
        groupItem = mUiModel.mToolbarMenu;
    else if ( itemData == gToolbarRightBarID )
        groupItem = mUiModel.mRightBar;
    else if ( itemData == gChildID )
        groupItem = findGroup(text, mUiModel.mToolbar);
    else if ( itemData == gToolbarMenuChildID )
        groupItem = findGroup(text, mUiModel.mToolbarMenu);

    baseUiModel = dynamic_cast<BaseUiModel *>( ui->listView->model() );
    if ( baseUiModel && groupItem )
    {
        baseUiModel->setData(groupItem);

        if ( groupItem->count() )
        {
            ui->listView->selectionModel()->select(baseUiModel->index(0,0),
                                                   QItemSelectionModel::SelectCurrent);
            // FIXME: in cases when item is changed in combo box select does not
            // emit selectionChange signal.
            QModelIndex index0 = baseUiModel->index(0,0);
            on_listView_selectionChanged(QItemSelection(index0, index0),
                                         QItemSelection(index0, index0));
        }
        else
            on_listView_selectionChanged(QItemSelection(), QItemSelection());
    }

    // Set menu path in format "/View/Geodesy/Load"
    QString path = text;
    path.replace(gGroupSeparator, "/");
    ui->labelPath->setText( QString("/%1").arg(path) );
}

void RearrangeDlg::on_comboBoxSubmenu_currentIndexChanged( int index )
{
    if ( !ui->radioButtonSubmenu->isChecked() )
        return;

    if ( ui->listView->selectionModel() )
        ui->listView->selectionModel()->clear();

    TGroupItem groupItem;

    int itemData = ui->comboBoxSubmenu->itemData(index).toInt();
    if ( itemData == gSubmenuID )
        groupItem = mUiModel.mSubMenus;
    else
        groupItem = findGroup(ui->comboBoxSubmenu->currentText(), mUiModel.mSubMenus);

    BaseUiModel *baseUiModel = dynamic_cast<BaseUiModel *>( ui->listView->model() );
    if ( baseUiModel && groupItem )
    {
        baseUiModel->setData(groupItem);

        if ( groupItem->count() )
        {
            ui->listView->selectionModel()->select(baseUiModel->index(0,0),
                                                   QItemSelectionModel::SelectCurrent);
            // FIXME: in cases when item is changed in combo box select does not
            // emit selectionChange signal.
            QModelIndex index0 = baseUiModel->index(0,0);
            on_listView_selectionChanged(QItemSelection(index0, index0),
                                         QItemSelection(index0, index0));
        }
        else
            on_listView_selectionChanged(QItemSelection(), QItemSelection());
    }

    // Set menu path in format "/View/Geodesy/Load"
    QString path = ui->comboBoxSubmenu->currentText();
    path.replace(gGroupSeparator, "/");
    ui->labelPath->setText( QString("/%1").arg(path) );
}

void RearrangeDlg::on_listView_selectionChanged( const QItemSelection & selected,
                                                 const QItemSelection & deselected )
{
    BaseUiModel *baseUiModel = dynamic_cast<BaseUiModel *>( ui->listView->model() );
    if ( !baseUiModel )
        return;

    QModelIndex index;
    QModelIndexList indexes = selected.indexes();
    if ( indexes.count() > 0 )
        index = indexes.at(0);

    mAddMenu->clear();
    mModifyMenu->clear();

    // Main menu changes
    if ( ui->radioButtonMenu->isChecked() )
    {
        int itemData = ui->comboBoxMenu->itemData(ui->comboBoxMenu->currentIndex()).toInt();
        // Fill Add and Modify buttons menu.
        if ( itemData == gMenubarID )
            fillMenus(true, 0);
        else
            fillMenus(false, baseUiModel->data(index, BaseUiModel::userRoleItemType).toInt());
    }
    // Toolbar changes
    else if ( ui->radioButtonToolbar->isChecked() )
    {
        int itemData = ui->comboBoxToolbar->itemData(ui->comboBoxToolbar->currentIndex()).toInt();
        // Main toolbar tab widget.
        if ( itemData == gToolbarID )
        {
            // Fill Add button menu.
            mAddMenu->addAction(mAddGroupAction);
            // Fill Modify selection menu.
            ui->toolButtonModify->setEnabled(true);
            mModifyMenu->addAction(mModifyNameAction);
        }
        // Toolbar child item. Can be group or toolbutton.
        else if ( itemData == gChildID )
        {
            QString pathText = ui->comboBoxToolbar->currentText();
            QStringList pathList = pathText.split(gGroupSeparator, QString::SkipEmptyParts);
            // Add group in the tab widget.
            if ( pathList.count() < 2 )
            {
                // Fill Add button menu.
                mAddMenu->addAction(mAddGroupAction);
                // Fill Modify selection menu.
                ui->toolButtonModify->setEnabled(true);
                mModifyMenu->addAction(mModifyNameAction);
            }
            // Add toolbutton into the group.
            else
            {
                // Fill Add button menu.
                mAddMenu->addAction(mAddActionAction);
                if ( mUiModel.mSubMenus->count() )
                    mAddMenu->addAction(mAddSubmenuAction);
                if ( mIsPrebuiltMenuExist )
                    mAddMenu->addAction(mAddPrebuiltMenuAction);
                // Fill Modify selection menu.
                ui->toolButtonModify->setEnabled(true);
                mModifyMenu->addAction(mModifyNameAction);
                mModifyMenu->addAction(mModifyToolTipAction);
                mModifyMenu->addSeparator();
                mModifyMenu->addAction(mModifyStyleIconAction);
                mModifyMenu->addAction(mModifyStyleTextAction);
                mModifyMenu->addAction(mModifyStyleTextBesideIconAction);
                mModifyMenu->addAction(mModifyStyleTextUnderIconAction);
                mModifyMenu->addSeparator();
                mModifyMenu->addAction(mModifyRow);
                mModifyMenu->addAction(mModifyColumn);
                mModifyMenu->addAction(mModifyAlignment);
                mModifyMenu->addSeparator();
                mModifyMenu->addAction(mModifyIconSize);
                mModifyMenu->addSeparator();
                mModifyMenu->addAction(mModifySizePolicyFixed);
                mModifyMenu->addAction(mModifySizePolicyMinimum);
            }
        }
        // Toolbar right menu.
        else if ( itemData == gToolbarMenuID || itemData == gToolbarMenuChildID )
        {
            fillMenus(false, baseUiModel->data(index, BaseUiModel::userRoleItemType).toInt());
        }
        // Tollbar right bar with buttons.
        else if ( itemData == gToolbarRightBarID )
        {
            // Fill Add button menu.
            mAddMenu->addAction(mAddActionAction);
            // Fill Modify selection menu.
            ui->toolButtonModify->setEnabled(true);
            mModifyMenu->addAction(mModifyNameAction);
            mModifyMenu->addAction(mModifyToolTipAction);
            mModifyMenu->addSeparator();
            mModifyMenu->addAction(mModifyStyleIconAction);
            mModifyMenu->addAction(mModifyStyleTextAction);
            mModifyMenu->addAction(mModifyStyleTextBesideIconAction);
        }
    }
    // Submenu changes
    else if ( ui->radioButtonSubmenu->isChecked() )
    {
        int itemData = ui->comboBoxSubmenu->itemData(ui->comboBoxSubmenu->currentIndex()).toInt();
        // Fill Add and Modify buttons menu.
        if ( itemData == gSubmenuID )
            fillMenus(true, 0);
        else
            fillMenus(false, baseUiModel->data(index, BaseUiModel::userRoleItemType).toInt(),
                      false);
    }

    ActionItem::Style styleItem = (ActionItem::Style)
                                  baseUiModel->data(index, BaseUiModel::userRoleStyle).toInt();
    if ( styleItem == ActionItem::IconOnly )
        mModifyStyleIconAction->setChecked(true);
    else if ( styleItem == ActionItem::TextOnly )
        mModifyStyleTextAction->setChecked(true);
    else if ( styleItem == ActionItem::TextBesideIcon )
        mModifyStyleTextBesideIconAction->setChecked(true);
    else if ( styleItem == ActionItem::TextUnderIcon )
        mModifyStyleTextUnderIconAction->setChecked(true);

    ToolbuttonItem::SizePolicy szPolicy = (ToolbuttonItem::SizePolicy)
                                          baseUiModel->data(index, ToolbarUiModel::userRoleSizePolicy).toInt();
    if ( szPolicy == ToolbuttonItem::Fixed )
        mModifySizePolicyFixed->setChecked(true);
    else if ( szPolicy == ToolbuttonItem::Minimum )
        mModifySizePolicyMinimum->setChecked(true);
}

void RearrangeDlg::fillMenus(bool isMainbar, int typeItem, bool isAddSubmenu)
{
    if ( isMainbar )
    {
        // Fill Add button menu.
        mAddMenu->addAction(mAddGroupAction);
        // Fill Modify selection menu.
        ui->toolButtonModify->setEnabled(true);
        mModifyMenu->addAction(mModifyNameAction);
    }
    else
    {
        // Fill Add button menu.
        mAddMenu->addAction(mAddGroupAction);
        mAddMenu->addAction(mAddSeparatorAction);
        mAddMenu->addAction(mAddActionAction);
        if ( isAddSubmenu )
        {
            if ( mUiModel.mSubMenus->count() )
                mAddMenu->addAction(mAddSubmenuAction);
            if ( mIsPrebuiltMenuExist )
                mAddMenu->addAction(mAddPrebuiltMenuAction);
        }

        if ( typeItem == (int) BaseUiModel::Separator )
        {
            // Fill Modify selection menu.
            ui->toolButtonModify->setEnabled(false);
        }
        else if ( typeItem == (int) BaseUiModel::Group )
        {
            // Fill Modify selection menu.
            ui->toolButtonModify->setEnabled(true);
            mModifyMenu->addAction(mModifyNameAction);
        }
        else if ( typeItem == (int) BaseUiModel::Action )
        {
            // Fill Modify selection menu.
            ui->toolButtonModify->setEnabled(true);
            mModifyMenu->addAction(mModifyNameAction);
            mModifyMenu->addSeparator();
            mModifyMenu->addAction(mModifyStyleTextAction);
            mModifyMenu->addAction(mModifyStyleTextBesideIconAction);
        }
        else
        {
            ui->toolButtonModify->setEnabled(false);
        }
    }
}

TGroupItem RearrangeDlg::findGroup(const QString &path, const TGroupItem &root) const
{
    QStringList splittedPath = path.split(gGroupSeparator, QString::SkipEmptyParts);
    if ( splittedPath.count() < 1 )
        return TGroupItem();

    QString textCurrent = splittedPath[0];
    // Make path without first item from the string list.
    QString pathNext;
    for ( int i = 1; i < splittedPath.count(); i++ )
        pathNext += splittedPath[i] + gGroupSeparator;
    if ( !pathNext.isEmpty() )
        pathNext = pathNext.left(pathNext.length() - gGroupSeparator.count());

    const GroupItem::ItemsList &items = root->children();
    foreach ( const TBaseItem &item, items )
    {
        if ( item->isGroup() )
        {
            TGroupItem child = item.staticCast<GroupItem>();
            if ( child->text() == textCurrent )
            {
                TGroupItem groupFound = findGroup(pathNext, child);
                return ( groupFound ? groupFound : item.staticCast<GroupItem>() );
            }
        }
    }

    return TGroupItem();
}

void RearrangeDlg::on_addSeparator()
{
    QModelIndex index;
    QModelIndexList indexes = ui->listView->selectionModel()->selectedRows();
    if ( !indexes.empty() )
        index = indexes.at(0);

    BaseUiModel *baseUiModel = dynamic_cast<BaseUiModel *>( ui->listView->model() );
    if ( baseUiModel )
    {
        bool isModelEmpty = baseUiModel->rowCount() < 1;
        int row = isModelEmpty ? 0 : (index.row()+1);

        baseUiModel->insertSeparator( row );

        if ( isModelEmpty )
        {
            ui->listView->selectionModel()->select(baseUiModel->index(0,0),
                                                   QItemSelectionModel::ClearAndSelect);
            // FIXME: in cases when item is changed in combo box select does not
            // emit selectionChange signal.
            QModelIndex index0 = baseUiModel->index(0,0);
            on_listView_selectionChanged(QItemSelection(index0, index0),
                                         QItemSelection(index0, index0));
        }
    }
}

void RearrangeDlg::on_addAction()
{
    QModelIndex index;
    QModelIndexList indexes = ui->listView->selectionModel()->selectedRows();
    if ( !indexes.empty() )
        index = indexes.at(0);

    AddActionDlg dlg(mModelCmd, this);
    if ( dlg.exec() == QDialog::Accepted )
    {
        BaseUiModel *baseUiModel = dynamic_cast<BaseUiModel *>( ui->listView->model() );
        if ( baseUiModel )
        {
            bool isModelEmpty = baseUiModel->rowCount() < 1;
            int row = isModelEmpty ? 0 : (index.row()+1);

            baseUiModel->insertAction( row, dlg.selectedAction() );

            if ( isModelEmpty )
            {
                ui->listView->selectionModel()->select(baseUiModel->index(0,0),
                                                       QItemSelectionModel::ClearAndSelect);
                // FIXME: in cases when item is changed in combo box select does not
                // emit selectionChange signal.
                QModelIndex index0 = baseUiModel->index(0,0);
                on_listView_selectionChanged(QItemSelection(index0, index0),
                                             QItemSelection(index0, index0));
            }
        }
    }
}

void RearrangeDlg::on_addSubmenu()
{
    QModelIndex index;
    QModelIndexList indexes = ui->listView->selectionModel()->selectedRows();
    if ( !indexes.empty() )
        index = indexes.at(0);

    bool ok;
    QString text = QInputDialog::getText(this, tr("Enter item name"), tr("Name:"),
                                         QLineEdit::Normal, QString(), &ok);

    if ( !(ok && !text.isEmpty()) )
        return;

    QStringList submenuList;
    const GroupItem::ItemsList &submenuChildren = mUiModel.mSubMenus->children();
    // Make list of submenus.
    foreach ( const TBaseItem &item, submenuChildren )
    {
        if ( item->isGroup() )
            submenuList << static_cast<GroupItem *>(item.data())->text();
    }

    QString submenu = QInputDialog::getItem(this, tr("Choose submenu"), tr("Submenus:"),
                                            submenuList, 0, false, &ok);

    if ( ok && !submenu.isEmpty() )
    {
        BaseUiModel *baseUiModel = dynamic_cast<BaseUiModel *>( ui->listView->model() );
        if ( baseUiModel )
        {
            bool isModelEmpty = baseUiModel->rowCount() < 1;
            int row = isModelEmpty ? 0 : (index.row()+1);
            baseUiModel->insertAction( row, submenu, true );
            baseUiModel->setData( baseUiModel->index(row,0), text, Qt::DisplayRole );

            if ( isModelEmpty )
            {
                ui->listView->selectionModel()->select(baseUiModel->index(0,0),
                                                       QItemSelectionModel::ClearAndSelect);
                // FIXME: in cases when item is changed in combo box select does not
                // emit selectionChange signal.
                QModelIndex index0 = baseUiModel->index(0,0);
                on_listView_selectionChanged(QItemSelection(index0, index0),
                                             QItemSelection(index0, index0));
            }
        }
    }
}

void RearrangeDlg::on_addPrebuiltMenu()
{
    QModelIndex index;
    QModelIndexList indexes = ui->listView->selectionModel()->selectedRows();
    if ( !indexes.empty() )
        index = indexes.at(0);

    bool ok;
    QString text = QInputDialog::getText(this, tr("Enter item name"), tr("Name:"),
                                         QLineEdit::Normal, QString(), &ok);

    if ( !(ok && !text.isEmpty()) )
        return;

    // first - menu title, second - menu class name.
    typedef QMap<QString, QString>  MenuNameMap;
    // This cache is used to find selected prebuilt menu easier.
    MenuNameMap menuNames;

    QStringList items;
    foreach ( const QString &menuClassName, mModelCmd->menus() )
    {
        QMenu *menu = mModelCmd->findMenu(menuClassName);
        if ( menu )
        {
            items << menu->title();
            menuNames[menu->title()] = menuClassName;
        }
    }

    QString prebuiltMenu = QInputDialog::getItem(this, tr("Choose prebuilt menu"), tr("Menu:"),
                                                 menuNames.keys(), 0, false, &ok);

    if ( ok && !prebuiltMenu.isEmpty() )
    {
        // Prebuilt menu is saved as submenu but when menubar or toolbar is built
        // first search is performed on prebuilt menus and then submenus.
        // Thus submenu should have different names.
        BaseUiModel *baseUiModel = dynamic_cast<BaseUiModel *>( ui->listView->model() );
        if ( baseUiModel )
        {
            bool isModelEmpty = baseUiModel->rowCount() < 1;
            int row = isModelEmpty ? 0 : (index.row()+1);
            baseUiModel->insertAction( row, menuNames[prebuiltMenu], true );
            baseUiModel->setData( baseUiModel->index(row,0), text, Qt::DisplayRole );

            if ( isModelEmpty )
            {
                ui->listView->selectionModel()->select(baseUiModel->index(0,0),
                                                       QItemSelectionModel::ClearAndSelect);
                // FIXME: in cases when item is changed in combo box select does not
                // emit selectionChange signal.
                QModelIndex index0 = baseUiModel->index(0,0);
                on_listView_selectionChanged(QItemSelection(index0, index0),
                                             QItemSelection(index0, index0));
            }
        }
    }
}

void RearrangeDlg::on_addGroup()
{
    QModelIndex index;
    QModelIndexList indexes = ui->listView->selectionModel()->selectedRows();
    if ( !indexes.empty() )
        index = indexes.at(0);

    bool ok;
    QString text = QInputDialog::getText(this, tr("Enter group name"), tr("Name:"),
                                         QLineEdit::Normal, QString(), &ok);

    if ( ok && !text.isEmpty() )
    {
        BaseUiModel *baseUiModel = dynamic_cast<BaseUiModel *>( ui->listView->model() );
        if ( baseUiModel )
        {
            bool isModelEmpty = baseUiModel->rowCount() < 1;
            int row = isModelEmpty ? 0 : (index.row()+1);
            baseUiModel->insertGroup(row);
            baseUiModel->setData(baseUiModel->index(row,0), text, Qt::DisplayRole);

            if ( isModelEmpty )
            {
                ui->listView->selectionModel()->select(baseUiModel->index(0,0),
                                                       QItemSelectionModel::ClearAndSelect);
                // FIXME: in cases when item is changed in combo box select does not
                // emit selectionChange signal.
                QModelIndex index0 = baseUiModel->index(0,0);
                on_listView_selectionChanged(QItemSelection(index0, index0),
                                             QItemSelection(index0, index0));
            }
        }
    }

    // Update combo boxes - add new group.
    updateMenuCombo( ui->radioButtonMenu->isChecked(),
                     ui->radioButtonToolbar->isChecked(),
                     ui->radioButtonSubmenu->isChecked() );
}

void RearrangeDlg::on_modifyName()
{
    QModelIndexList indexes = ui->listView->selectionModel()->selectedRows();
    if ( indexes.empty() )
        return;

    QModelIndex index = indexes.at(0);

    BaseUiModel *baseUiModel = dynamic_cast<BaseUiModel *>( ui->listView->model() );
    if ( !baseUiModel || !index.isValid() )
        return;

    QString itemText = baseUiModel->data(index, BaseUiModel::userRoleText).toString();

    bool ok;
    QString text = QInputDialog::getText(this, tr("Change name"), tr("New name:"),
                                         QLineEdit::Normal, itemText, &ok);

    if ( ok && !text.isEmpty() && index.isValid() )
    {
        baseUiModel->setData(index, text, Qt::DisplayRole);

        // Update combo boxes - add new group.
        updateMenuCombo( ui->radioButtonMenu->isChecked(),
                         ui->radioButtonToolbar->isChecked(),
                         ui->radioButtonSubmenu->isChecked() );
    }
}

void RearrangeDlg::on_modifyToolTip()
{
    QModelIndexList indexes = ui->listView->selectionModel()->selectedRows();
    if ( indexes.empty() )
        return;

    QModelIndex index = indexes.at(0);

    BaseUiModel *baseUiModel = dynamic_cast<BaseUiModel *>( ui->listView->model() );
    if ( !baseUiModel || !index.isValid() )
        return;

    QString itemText = baseUiModel->data(index, BaseUiModel::userRoleToolTip).toString();

    bool ok;
    QString text = QInputDialog::getText(this, tr("Change ToolTip"), tr("ToolTip:"),
                                         QLineEdit::Normal, itemText, &ok);

    if ( ok && !text.isEmpty() && index.isValid() )
    {
        baseUiModel->setData(index, text, BaseUiModel::userRoleToolTip);
    }
}

void RearrangeDlg::modifyActionStyle(ActionItem::Style style)
{
    QModelIndexList indexes = ui->listView->selectionModel()->selectedRows();
    if ( indexes.empty() )
        return;

    QModelIndex index = indexes.at(0);

    BaseUiModel *baseUiModel = dynamic_cast<BaseUiModel *>( ui->listView->model() );
    if ( baseUiModel && index.isValid() )
    {
        baseUiModel->setData(index, int(style), BaseUiModel::userRoleStyle);
    }
}

void RearrangeDlg::on_modifyStyleIcon()
{
    modifyActionStyle(ActionItem::IconOnly);
}

void RearrangeDlg::on_modifyStyleText()
{
    modifyActionStyle(ActionItem::TextOnly);
}

void RearrangeDlg::on_modifyStyleTextBesideIcon()
{
    modifyActionStyle(ActionItem::TextBesideIcon);
}

void RearrangeDlg::on_modifyStyleTextUnderIcon()
{
    modifyActionStyle(ActionItem::TextUnderIcon);
}

void RearrangeDlg::modifyActionCell(int roleCell, int roleSpan,
                                    const QString &title, const QString &label)
{
    QModelIndexList indexes = ui->listView->selectionModel()->selectedRows();
    if ( indexes.empty() )
        return;

    QModelIndex index = indexes.at(0);

    BaseUiModel *baseUiModel = dynamic_cast<BaseUiModel *>( ui->listView->model() );
    if ( baseUiModel && index.isValid() )
    {
        int rowCol = baseUiModel->data(index, roleCell).toInt();
        int rowColSpan = baseUiModel->data(index, roleSpan).toInt();

        bool ok;
        rowCol = CellEditorDlg::getCell(this, title, label, rowCol, rowColSpan, &ok);

        if ( ok && index.isValid() )
        {
            baseUiModel->setData(index, rowCol, roleCell);
            baseUiModel->setData(index, rowColSpan, roleSpan);
        }
    }
}

void RearrangeDlg::on_modifyRow()
{
    modifyActionCell(ToolbarUiModel::userRoleRow, ToolbarUiModel::userRoleRowSpan,
                     tr("Change row number"), tr("Row"));
}

void RearrangeDlg::on_modifyColumn()
{
    modifyActionCell(ToolbarUiModel::userRoleColumn, ToolbarUiModel::userRoleColumnSpan,
                     tr("Change column number"), tr("Column"));
}

void RearrangeDlg::on_modifyAlignment()
{
    QModelIndexList indexes = ui->listView->selectionModel()->selectedRows();
    if ( indexes.empty() )
        return;

    QModelIndex index = indexes.at(0);

    ToolbarUiModel *toolbarUiModel = dynamic_cast<ToolbarUiModel *>( ui->listView->model() );
    if ( toolbarUiModel && index.isValid() )
    {
        int alignment = toolbarUiModel->data(index, ToolbarUiModel::userRoleAlignment).toInt();

        bool ok;
        QString alignmentString = QInputDialog::getItem(this, tr("Choose cell alignment"), tr("Alignment:"),
                                                        toolbarUiModel->alignmentList(), alignment, false, &ok);

        if ( ok && !alignmentString.isEmpty() && index.isValid() )
            toolbarUiModel->setData(index, alignmentString, ToolbarUiModel::userRoleAlignment);
    }
}

void RearrangeDlg::on_modifyIconSize()
{
    QModelIndexList indexes = ui->listView->selectionModel()->selectedRows();
    if ( indexes.empty() )
        return;

    QModelIndex index = indexes.at(0);

    BaseUiModel *baseUiModel = dynamic_cast<BaseUiModel *>( ui->listView->model() );
    if ( baseUiModel && index.isValid() )
    {
        int iconSize = baseUiModel->data(index, ToolbarUiModel::userRoleIconSize).toInt();

        bool ok;
        iconSize = QInputDialog::getInt(this, tr("Enter icon size"), tr("Icon size:"),
                                        iconSize, -1, 100, 1, &ok);

        if ( ok && index.isValid() )
        {
            baseUiModel->setData(index, iconSize, ToolbarUiModel::userRoleIconSize);
        }
    }
}

void RearrangeDlg::modifyToolbuttonSizePolicy(ToolbuttonItem::SizePolicy sizePolicy)
{
    QModelIndexList indexes = ui->listView->selectionModel()->selectedRows();
    if ( indexes.empty() )
        return;

    QModelIndex index = indexes.at(0);

    BaseUiModel *baseUiModel = dynamic_cast<BaseUiModel *>( ui->listView->model() );
    if ( baseUiModel && index.isValid() )
    {
        baseUiModel->setData(index, int(sizePolicy), ToolbarUiModel::userRoleSizePolicy);
    }
}

void RearrangeDlg::on_modifySizePolicyFixed()
{
    modifyToolbuttonSizePolicy(ToolbuttonItem::Fixed);
}

void RearrangeDlg::on_modifySizePolicyMinimum()
{
    modifyToolbuttonSizePolicy(ToolbuttonItem::Minimum);
}

void RearrangeDlg::on_pushButtonDelete_clicked()
{
    QModelIndexList indexes = ui->listView->selectionModel()->selectedRows();
    if ( indexes.empty() )
        return;

    QModelIndex index = indexes.at(0);

    BaseUiModel *baseUiModel = dynamic_cast<BaseUiModel *>( ui->listView->model() );
    if ( baseUiModel && index.isValid() )
    {
        BaseUiModel::ItemType typeItem = (BaseUiModel::ItemType)
            baseUiModel->data(index, BaseUiModel::userRoleItemType).toInt();

        baseUiModel->removeRow(index.row());

        // Update combo boxes if group was deleted.
        if ( typeItem == BaseUiModel::Group )
            updateMenuCombo( ui->radioButtonMenu->isChecked(),
                             ui->radioButtonToolbar->isChecked(),
                             ui->radioButtonSubmenu->isChecked() );
    }
}

void RearrangeDlg::swapRows(int increment)
{
    QModelIndexList indexes = ui->listView->selectionModel()->selectedRows();
    if ( indexes.empty() )
        return;

    QModelIndex index = indexes.at(0);
    int rowNew = index.row()+increment;

    BaseUiModel *baseUiModel = dynamic_cast<BaseUiModel *>( ui->listView->model() );
    if ( baseUiModel && index.isValid() && rowNew >= 0 && rowNew < baseUiModel->rowCount() )
    {
        if ( index.row() > rowNew )
            baseUiModel->swapRows(index.row(), rowNew);
        else
            baseUiModel->swapRows(rowNew, index.row());
    }
}

void RearrangeDlg::on_pushButtonUp_clicked()
{
    swapRows(-1);
}

void RearrangeDlg::on_pushButtonDown_clicked()
{
    swapRows(1);
}

void RearrangeDlg::on_pushButtonReset_clicked()
{
    mUiModel = mUiModelBase;

    // Update "Menu" combobox
    updateMenuCombo(true, false, false);
    // Update "Toolbar" combobox
    updateMenuCombo(false, true, false);
    // Update "Submenus" combobox
    updateMenuCombo(false, false, true);

    // Create default model.
    ui->radioButtonMenu->setChecked(true);
    on_buttonClicked(ui->radioButtonMenu);

    if ( ui->comboBoxMenu->count() > 0 )
        ui->comboBoxMenu->setCurrentIndex(0);
    if ( ui->comboBoxToolbar->count() > 0 )
        ui->comboBoxToolbar->setCurrentIndex(0);
    if ( ui->comboBoxSubmenu->count() > 0 )
        ui->comboBoxSubmenu->setCurrentIndex(0);

    on_comboBoxMenu_currentIndexChanged(0);
}

void RearrangeDlg::accept()
{
    QDialog::accept();
}

const UiModel& RearrangeDlg::currentUiModel() const
{
    return mUiModel;
}

void RearrangeDlg::changeEvent(QEvent * event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
        retranslateUi();
    }

    QDialog::changeEvent(event);
}

}   // namespace Uim
