#ifndef REARRANGEDLG_H
#define REARRANGEDLG_H

#include "UiModel.h"
#include "AbstractUiModel.h"

#include <QDialog>

namespace Ui {
    class RearrangeDlg;
}
class QItemSelection;
class QMenu;
class QButtonGroup;
class QAbstractButton;
class QComboBox;
class QModelIndex;

namespace Uim {

//! Rearrange dialog for changing toolbar and menu interface's view.
class RearrangeDlg : public QDialog
{
    Q_OBJECT

public:
    explicit RearrangeDlg(const UiModel &modelUi, const AbstractUiModel *modelCmd, QWidget *parent = 0);
    ~RearrangeDlg();

    //! Retrieve current Ui model.
    const UiModel& currentUiModel() const;

    virtual void accept();

protected:
    virtual void changeEvent(QEvent * event);

private slots:
    void on_listView_selectionChanged( const QItemSelection & selected,
                                       const QItemSelection & deselected );
    void on_buttonClicked( QAbstractButton * button );

    void on_comboBoxMenu_currentIndexChanged( int );
    void on_comboBoxToolbar_currentIndexChanged( int );
    void on_comboBoxSubmenu_currentIndexChanged( int );

    void on_addSeparator();
    void on_addAction();
    void on_addSubmenu();
    void on_addPrebuiltMenu();
    void on_addGroup();
    void on_modifyName();
    void on_modifyToolTip();
    void on_modifyStyleIcon();
    void on_modifyStyleText();
    void on_modifyStyleTextBesideIcon();
    void on_modifyStyleTextUnderIcon();
    void on_modifyRow();
    void on_modifyColumn();
    void on_modifyAlignment();
    void on_modifyIconSize();
    void on_modifySizePolicyFixed();
    void on_modifySizePolicyMinimum();

    void on_pushButtonDelete_clicked();
    void on_pushButtonUp_clicked();
    void on_pushButtonDown_clicked();

    void on_pushButtonReset_clicked();

private:
    void setupUi();
    void retranslateUi();
    void setupMenuCombo(QComboBox *combo, const TGroupItem &group,
                        const QString &groupText, int childID);
    void updateMenuCombo(bool isMenuChecked, bool isToolbarChecked, bool isSubmenuChecked);

    //! Find menu group item by path.
    /*!
        \param path Menu path: "View | Geodesy | Load"
        \param root Root item search starts
    */
    TGroupItem findGroup(const QString &path, const TGroupItem &root) const;

    //! Modify style of an action.
    void modifyActionStyle(ActionItem::Style style);
    //! Modify row/column of an action.
    /*!
        \param roleCell Model role: row or column number
        \param roleSpan Model role: row or column spanning
        \param title QInputDialog title text
        \param title QInputDialog label text
    */
    void modifyActionCell(int roleCell, int roleSpan, const QString &title, const QString &label);

    //! Modify size policy of selected toolbutton.
    void modifyToolbuttonSizePolicy(ToolbuttonItem::SizePolicy sizePolicy);

    //! Fill "Add" and "Modify Selection" buttons menu for menu role.
    /*!
        \param isMainbar True if main bar menu group should be created
        \param typeItem BaseUiModel::ItemType enumeration
        \param isAddSubmenu True if submenu and prebuilt menu should be added for Add menu toolbutton
    */
    void fillMenus(bool isMainbar, int typeItem, bool isAddSubmenu = true);

    //! Swap rows in the list view.
    void swapRows(int increment);

private:
    Ui::RearrangeDlg *ui;

    /** Reference to Ui actions model */
    const AbstractUiModel   *mModelCmd;
    /** Reference to the base Ui model (toolbar, menu, submenu) */
    const UiModel           &mUiModelBase;

    /** Currently modified Ui model (toolbar, menu, submenu) */
    UiModel         mUiModel;

    /** True if ui model has prebuilt models */
    bool            mIsPrebuiltMenuExist;

    QMenu           *mAddMenu;
    QMenu           *mModifyMenu;

    QAction         *mAddSeparatorAction;
    QAction         *mAddActionAction;
    QAction         *mAddSubmenuAction;
    QAction         *mAddPrebuiltMenuAction;
    QAction         *mAddGroupAction;

    QAction         *mModifyNameAction;
    QAction         *mModifyToolTipAction;
    QAction         *mModifyStyleIconAction;
    QAction         *mModifyStyleTextAction;
    QAction         *mModifyStyleTextBesideIconAction;
    QAction         *mModifyStyleTextUnderIconAction;

    QAction         *mModifyRow;
    QAction         *mModifyColumn;
    QAction         *mModifyAlignment;

    QAction         *mModifyIconSize;
    QAction         *mModifySizePolicyFixed;
    QAction         *mModifySizePolicyMinimum;

    /** Group of current menu(toolbar) radio buttons */
    QButtonGroup    *mMenuGroup;
};

}   // namespace Uim

#endif // REARRANGEDLG_H
