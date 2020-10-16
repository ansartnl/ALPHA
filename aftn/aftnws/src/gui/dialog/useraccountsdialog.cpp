#include "useraccountsdialog.h"

#include "translations.h"

#include "userpassworddialog.h"
#include "qtcheckboxdelegate.h"

#include "objectsmanager.h"

#include "main.h"

#include <QtDebug>

#include <QtCore/QString>
#include <QtCore/QList>

#include <QtGui/QDataWidgetMapper>
#include <QtGui/QCloseEvent>
#include <QtGui/QMessageBox>

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlError>
#include <QtSql/QSqlTableModel>

UserAccountsDialog::UserAccountsDialog(QWidget* parent) :
    AbstractDialog(parent), newUserState(false)
{
    setupUi(this);

    usersModel = new QSqlTableModel(this);
    usersModel->setTable("users");
    usersModel->setFilter("id != 0");
    usersModel->sort(usersModel->fieldIndex("user_name"), Qt::AscendingOrder);
    usersModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    usersModel->select();

    if (usersModel->lastError().isValid())
    {
        qWarning() << Q_FUNC_INFO;
        qWarning() << usersModel->lastError().text();
    }

    usersList->setModel(usersModel);
    usersList->hideColumn(usersModel->fieldIndex("id"));
    usersList->hideColumn(usersModel->fieldIndex("created"));
    usersList->hideColumn(usersModel->fieldIndex("password"));
    usersList->hideColumn(usersModel->fieldIndex("lasttouch"));

    usersList->setItemDelegateForColumn(usersModel->fieldIndex("isactive"), new QtCheckBoxDelegate(this, 1, 0));

    Translations::translateSqlTableModel(usersModel);

    objectTree->setModel(ObjectsManager::instance()->model());

    connect(usersList->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this,
        SLOT(usersListActivateUser(QModelIndex)));
    connect(objectTree->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this,
        SLOT(objectTree_clicked(QModelIndex)));

    objectRightsModel = new QSqlTableModel(this);
    objectRightsModel->setTable("user_rights");
    objectRightsModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    objectRightsModel->select();

    errorLabel->clear();

    restoreState(staticMetaObject.className());
}

void UserAccountsDialog::on_buttonBox_clicked(QAbstractButton* button)
{
    if (buttonBox->buttonRole(button) == QDialogButtonBox::ApplyRole)
    {
        QString error;

        if (objectRightsModel->submitAll())
        {
            if (usersModel->submitAll())
                return;
        }

        if (objectRightsModel->lastError().isValid())
        {
            error.append('\n');
            error.append(objectRightsModel->lastError().text());
            objectRightsModel->revertAll();
        }

        if (usersModel->lastError().isValid())
        {
            error.append('\n');
            error.append(usersModel->lastError().text());
            usersModel->revertAll();
        }

        errorLabel->setText(error);
    }
}

void UserAccountsDialog::usersListActivateUser(const QModelIndex& index)
{
    if (!index.isValid())
    {
        databaseObjects->setEnabled(false);

        changePassword->setEnabled(false);
        delUserButton->setEnabled(false);
        return;
    }

    errorLabel->clear();

    int userId = usersModel->record(index.row()).value("id").toInt();
    QString filter = QString(" user_id = %1 ").arg(userId);

    if (userId == SUPERUSER_ID)
    {
        databaseObjects->setEnabled(false);
        delUserButton->setEnabled(false);
//        usersList->indexWidget(usersModel->index(index.row(), usersModel->fieldIndex("isactive")))
//                ->setEnabled(false);
    }
    else
    {
        databaseObjects->setEnabled(true);
        delUserButton->setEnabled(userId != currentUserId);
//        int col = usersModel->fieldIndex("isactive");
//        QModelIndex i = usersModel->index(index.row(), col);
//        QWidget *w = usersList->indexWidget(i);
//        w->setEnabled(userId != currentUserId);
    }
    changePassword->setEnabled(true);
    errorLabel->clear();
    objectRightsModel->setFilter(filter);
    objectRightsModel->select();

    objectTree_clicked(objectTree->currentIndex());
}

void UserAccountsDialog::objectTree_clicked(const QModelIndex& index)
{
    allow->setEnabled(true);

    QString objectId = ObjectsManager::instance()->objectIdFromIndex(index);

    if (!usersList->currentIndex().isValid() || !index.isValid() || objectId.isEmpty())
    {
        allow->setChecked(false);
        allow->setEnabled(false);
        return;
    }

    int rights = 0;

    for (int row = 0; row < objectRightsModel->rowCount(); ++row)
    {
        if (objectRightsModel->record(row).value("object_id").toString() == objectId)
        {
            rights = objectRightsModel->record(row).value("rights").toInt();
        }
    }

    allow->setChecked(rights);
}

void UserAccountsDialog::on_allow_stateChanged(int checked)
{
    int userId = usersModel->record(usersList->currentIndex().row()).value("id").toInt();
    QString objectId = ObjectsManager::instance()->objectIdFromIndex(objectTree->currentIndex());

    if(objectId.isEmpty() || userId <= SUPERUSER_ID)
    {
        allow->setChecked(false);
        allow->setEnabled(false);
        return;
    }

    int finded = false;
    QSqlRecord record = objectRightsModel->record();

    for (int row = 0; row < objectRightsModel->rowCount(); ++row)
    {
        if (objectRightsModel->record(row).value("object_id").toString() == objectId)
        {
            record = objectRightsModel->record(row);
            record.setValue("rights", checked);
            objectRightsModel->setRecord(row, record);
            finded = true;
        }
    }

    if (!finded)
    {
        record.setValue("user_id", userId);
        record.setValue("object_id", objectId);
        record.setValue("rights", checked);
        objectRightsModel->insertRecord(-1, record);
    }
}

void UserAccountsDialog::on_newUserButton_clicked()
{
    UserPasswordDialog dialog(this);
    dialog.setWindowTitle(tr("Create user"));

    if (dialog.exec() == UserPasswordDialog::Accepted)
    {

        QSqlRecord record = usersModel->record();

        record.setValue("user_name", dialog.userName->text());

        QString hash = passwordHash(dialog.password->text());

        record.setValue("password", hash);
        record.setValue("isactive", 1);

        usersModel->insertRecord(-1, record);
        usersModel->submitAll();

        if (usersModel->lastError().isValid())
        {
            QMessageBox::critical(this, tr("Error"), tr("Unable to create user.\nSql error: %1").arg(
                usersModel->lastError().text()));
            usersModel->revertAll();
        }
    }
}

void UserAccountsDialog::on_changePassword_clicked()
{
    UserPasswordDialog dialog(this);
    dialog.setWindowTitle(tr("Change password"));

    QModelIndex index = usersList->currentIndex();
    QSqlRecord record = usersModel->record(index.row());

    dialog.userName->setText(record.value("user_name").toString());
    dialog.userName->setReadOnly(true);

    if (dialog.exec() == UserPasswordDialog::Accepted)
    {
        QString hash = passwordHash(dialog.password->text());

        record.setValue("password", hash);

        usersModel->setRecord(index.row(), record);
        usersModel->submitAll();

        if (usersModel->lastError().isValid())
        {
            QMessageBox::critical(this, tr("Error"), tr("Unable to change password.\nSql error: %1").arg(
                usersModel->lastError().text()));
            usersModel->revertAll();
        }

        errorLabel->setText(tr("Password successfully changed"));
    }
}

void UserAccountsDialog::on_delUserButton_clicked()
{
    QModelIndex index = usersList->currentIndex();
    if(!index.isValid()) return;

    int userID = usersModel->record(index.row()).value("id").toInt();
    if((userID == SUPERUSER_ID) || (userID == currentUserId)) return;

    QMessageBox dlg(QMessageBox::Warning, tr("Delete user"),
                    tr("Delete user '%1'?").arg(usersModel->record(index.row()).value("user_name").toString()),
                    QMessageBox::Yes|QMessageBox::No, this);
    dlg.setDefaultButton(QMessageBox::No);
    int btn = dlg.exec();
    if(btn == int(QMessageBox::Yes))
    {
        usersModel->removeRow(index.row());
        usersModel->submitAll();

        if (usersModel->lastError().isValid())
        {
            QMessageBox::critical(this, tr("Error"), tr("Unable to delete user.\nSql error: %1").arg(
                usersModel->lastError().text()));
            usersModel->revertAll();
        }

        errorLabel->setText(tr("Delete user successfully"));
    }
}

void UserAccountsDialog::done(int code)
{
    saveState(staticMetaObject.className());
    QDialog::done(code);
}

void UserAccountsDialog::changeEvent(QEvent* event)
{
    QDialog::changeEvent(event);

    switch (event->type())
    {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void UserAccountsDialog::closeEvent(QCloseEvent *event)
{
    saveState(staticMetaObject.className());
    AbstractDialog::closeEvent(event);
}

// ---------------------------------------------------------------------
UserAccountsDialogHelper::UserAccountsDialogHelper(QObject* parent) :
    ObjectsManagerHelper(parent), mAction(0)
{
    mAction = new QAction(icon(), name(), this);
    mAction->setObjectName(id());
    mAction->setToolTip(description());
    mAction->setStatusTip(description());
}

QString UserAccountsDialogHelper::id() const
{
    return UserAccountsDialog::staticMetaObject.className();
}

QString UserAccountsDialogHelper::path() const
{
    return tr("/Administration/Users");
}

QString UserAccountsDialogHelper::name() const
{
    return tr("User accounts dialog");
}

QString UserAccountsDialogHelper::description() const
{
    return tr("Add/change user permissions");
}

QWidget* UserAccountsDialogHelper::widget(QWidget* parent) const
{
    return new UserAccountsDialog(parent);
}

QAction* UserAccountsDialogHelper::action() const
{
    return mAction;
}

void UserAccountsDialogHelper::retranslateUi()
{
    mAction->setText(name());
    mAction->setIcon(icon());
    mAction->setToolTip(description());
    mAction->setStatusTip(description());
}
