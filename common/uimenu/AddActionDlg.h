#ifndef ADDACTIONDLG_H
#define ADDACTIONDLG_H

#include "AbstractUiModel.h"

#include <QDialog>

namespace Ui {
    class AddActionDlg;
}
class QListWidgetItem;

namespace Uim {

//! Shows categories-commands groups in the list and used for choosing
//! a command from the application difined command groups.
class AddActionDlg : public QDialog
{
    Q_OBJECT

public:
    explicit AddActionDlg(const AbstractUiModel *model, QWidget *parent = 0);
    ~AddActionDlg();

    //! Retrieve selected action class name.
    const QString& selectedAction() const
    { return mSelectedAction; }

    virtual void accept();

protected:
    virtual void changeEvent(QEvent * event);

private slots:
    void on_listWidgetCat_itemSelectionChanged();
    void on_listWidgetCmd_itemDoubleClicked( QListWidgetItem * item );

private:
    void setupUi();

private:
    Ui::AddActionDlg *ui;
    /** Reference to the action model */
    const AbstractUiModel *mModel;
    /** Selected command name */
    QString     mSelectedAction;
};

}   // namespace Uim

#endif // ADDACTIONDLG_H
