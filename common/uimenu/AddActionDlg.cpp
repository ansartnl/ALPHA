#include "AddActionDlg.h"
#include "ui_AddActionDlg.h"

#include "CommandDelegate.h"

#include <QtCore/QDebug>

namespace Uim
{

// Action class name - user role for list widget item
const int gActionClassRole = Qt::UserRole;


AddActionDlg::AddActionDlg(const AbstractUiModel *model, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddActionDlg),
    mModel(model)
{
    Q_ASSERT(mModel);
    ui->setupUi(this);
    setupUi();
}

AddActionDlg::~AddActionDlg()
{
    delete ui;
}

void AddActionDlg::setupUi()
{
    ui->listWidgetCmd->setItemDelegate(new CommandDelegate(ui->listWidgetCmd));

    // Load categories.
    QStringList cats = mModel->categories();

    foreach ( const QString &category, cats )
    {
        new QListWidgetItem(category, ui->listWidgetCat);
    }

    if ( ui->listWidgetCat->count() > 0 )
        ui->listWidgetCat->setCurrentRow(0);
}

void AddActionDlg::on_listWidgetCat_itemSelectionChanged()
{
    QList<QListWidgetItem *> items = ui->listWidgetCat->selectedItems();
    if ( items.empty() )
        return;

    QListWidgetItem *itemCat = items.at(0);

    // Remove old items.
    while ( ui->listWidgetCmd->count() )
    {
        QListWidgetItem *itemCmd = ui->listWidgetCmd->takeItem(0);
        if ( itemCmd )
            delete itemCmd;
    }

    QStringList actions = mModel->actions(itemCat->text());
    foreach ( const QString &actionClassName, actions )
    {
        QAction *cmd = mModel->findAction(actionClassName);
        if ( !cmd )
        {
            qDebug("Command \"%s\" does not have an associated QAction", actionClassName.toLatin1().constData());
            continue;
        }

        QListWidgetItem *item = new QListWidgetItem(cmd->text(), ui->listWidgetCmd);
        item->setData(gActionClassRole, actionClassName);
        QIcon icon = cmd->icon();
        if ( !icon.isNull() )
            item->setIcon(icon);
        else
            item->setIcon(QIcon(":/images/Transparent.png"));
    }

    if ( ui->listWidgetCmd->count() > 0 )
        ui->listWidgetCmd->setCurrentRow(0);
}

void AddActionDlg::on_listWidgetCmd_itemDoubleClicked( QListWidgetItem * item )
{
    AddActionDlg::accept();
}

void AddActionDlg::accept()
{
    if ( ui->listWidgetCmd->count() < 1)
    {
        QDialog::reject();
        return;
    }

    QList<QListWidgetItem *> items = ui->listWidgetCmd->selectedItems();
    if ( !items.empty() )
    {
        QListWidgetItem *item = items.at(0);

        mSelectedAction = item->data(gActionClassRole).toString();

        QDialog::accept();
    }
}

void AddActionDlg::changeEvent(QEvent * event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }

    QDialog::changeEvent(event);
}

}   // namespace Uim
