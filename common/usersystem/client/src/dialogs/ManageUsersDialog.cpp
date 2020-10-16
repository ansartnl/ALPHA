#include "ManageUsersDialog.h"

#include "usHelperFunctions.h"
#include "UserClient.h"
#include "AddUserDialog.h"

#include <QtCore/QSettings>

#include <QtGui/QInputDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QStandardItemModel>

const QString sysManageUsersDialog = "ManageUsersDialog";

const QString sysWindowGeometry = "WindowGeometry";
const QString sysHeaderState = "HeaderState";

ManageUsersDialog::ManageUsersDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    model = new QStandardItemModel(this);
    view->setModel(model);

    mUserClient = 0;

    connect(addUserButton, SIGNAL(clicked()), this, SLOT(addUserTriggered()));
    connect(removeUserButton, SIGNAL(clicked()), this, SLOT(removeUserTriggered()));
    connect(setPasswordButton, SIGNAL(clicked()), this, SLOT(setPasswordTriggered()));
    connect(changeGroupButton, SIGNAL(clicked()), this, SLOT(changeGroupTriggered()));
    connect(changeDescriptionButton, SIGNAL(clicked()), this, SLOT(changeDescriptionTriggered()));
    connect(updateListButton, SIGNAL(clicked()), this, SLOT(updateUserList()));

    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

    connect(view->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex))
            , this, SLOT(onCurrentRowChanged(QModelIndex)));
    connect(view, SIGNAL(doubleClicked(QModelIndex))
            , this, SLOT(onDoubleClicked(QModelIndex)));

    QSettings settings;
    settings.beginGroup(sysManageUsersDialog);
    restoreGeometry(settings.value(sysWindowGeometry).toByteArray());
    settings.endGroup();

    disableButtons();
}

ManageUsersDialog::~ManageUsersDialog()
{
    QSettings settings;
    settings.beginGroup(sysManageUsersDialog);
    settings.setValue(sysWindowGeometry, saveGeometry());
    settings.endGroup();
}

void ManageUsersDialog::setUserClient(UserSystem::UserClient *userClient)
{
    mUserClient = userClient;

    connect(this, SIGNAL(receiveUserList()), userClient, SLOT(getUserList()));
    connect(userClient, SIGNAL(userListReceived(UserSystem::Users))
            , this, SLOT(onUserListReceived(UserSystem::Users)));
    connect(userClient, SIGNAL(getUserListResult(UserSystem::Request::Result))
            , this, SLOT(onUserListResult(UserSystem::Request::Result)));

    connect(this, SIGNAL(receiveGroupList()), userClient, SLOT(getGroupList()));
    connect(userClient, SIGNAL(groupListReceived(UserSystem::Groups))
            , this, SLOT(onGroupListReceived(UserSystem::Groups)));
    connect(userClient, SIGNAL(getGroupListResult(UserSystem::Request::Result))
            , this, SLOT(onGroupListResult(UserSystem::Request::Result)));

    connect(this, SIGNAL(removeUser(QString)), userClient, SLOT(removeUser(QString)));
    connect(this, SIGNAL(setPassword(QString,QString))
            , userClient, SLOT(setPassword(QString,QString)));
    connect(this, SIGNAL(changeGroup(QString,QString))
            , userClient, SLOT(setUserGroup(QString,QString)));
    connect(this, SIGNAL(changeDescription(QString,QString))
            , userClient, SLOT(setDescription(QString,QString)));

    connect(userClient, SIGNAL(removeUserResult(UserSystem::Request::Result))
            , this, SLOT(onRemoveUserResult(UserSystem::Request::Result)));
    connect(userClient, SIGNAL(setPasswordResult(UserSystem::Request::Result))
            , this, SLOT(onSetPasswordResult(UserSystem::Request::Result)));
    connect(userClient, SIGNAL(setUserGroupResult(UserSystem::Request::Result))
            , this, SLOT(onChangeGroupResult(UserSystem::Request::Result)));
    connect(userClient, SIGNAL(setDescriptionResult(UserSystem::Request::Result))
            , this, SLOT(onChangeDescriptionResult(UserSystem::Request::Result)));

    emit receiveUserList();
}

void ManageUsersDialog::onUserListReceived(const UserSystem::Users &users)
{
    model->clear();
    QStandardItem *root = model->invisibleRootItem();
    QList<QStandardItem *> row;
    foreach (const UserSystem::PUser &user, users)
    {
        row.append(new QStandardItem(user->name()));
        row.append(new QStandardItem(user->description()));
        if (!user->groups().isEmpty())
            row.append(new QStandardItem(UserSystem::toDescription(user->groups().at(0))));
        else
            row.append(new QStandardItem(UserSystem::toDescription(QString(""))));
        root->appendRow(row);
        row.clear();
    }
    model->setHorizontalHeaderLabels(QStringList() << tr("Login")
                                     << tr("Description") << tr("Group"));

    QSettings settings;
    settings.beginGroup(sysManageUsersDialog);
    view->horizontalHeader()->restoreState(settings.value(sysHeaderState).toByteArray());
    settings.endGroup();

    disableButtons();
}

void ManageUsersDialog::onGroupListReceived(const UserSystem::Groups &groups)
{
    QStringList groupNames;
    foreach (const UserSystem::PGroup &group, groups)
    {
        if (group->name() != UserSystem::builtInGroupName(UserSystem::BuiltInGroups::Root))
            groupNames << UserSystem::toDescription(group->name());
    }
    QString oldGroup = model->index(view->currentIndex().row(), 2).data().toString();
    int i = groupNames.indexOf(oldGroup);
    bool ok;
    QString group = QInputDialog::getItem(this, tr("Changing group")
                                          , tr("Select new group for user %1:")
                                          .arg(mUserName), groupNames, i, false, &ok);
    if (!ok)
        return;
    emit changeGroup(mUserName, UserSystem::fromDescription(group));
}

void ManageUsersDialog::onUserListResult(const UserSystem::Request::Result &result)
{
    QMessageBox::warning(this, tr("Receiving user list error"), UserSystem::toString(result));
}

void ManageUsersDialog::onGroupListResult(const UserSystem::Request::Result &result)
{
    QMessageBox::warning(this, tr("Receiving group list error"), UserSystem::toString(result));
}

void ManageUsersDialog::onRemoveUserResult(const UserSystem::Request::Result &result)
{
    if (result == UserSystem::Request::Ok)
    {
        QMessageBox::information(this, tr("Removing user"), tr("User removed successfully"));
        updateUserList();
    } else
    {
        QMessageBox::warning(this, tr("Removing user error"), UserSystem::toString(result));
    }
}

void ManageUsersDialog::onSetPasswordResult(const UserSystem::Request::Result &result)
{
    if (result == UserSystem::Request::Ok)
    {
        QMessageBox::information(this, tr("Changing password")
                                 , tr("Password changed successfully"));
    } else
    {
        QMessageBox::warning(this, tr("Changing password error"), UserSystem::toString(result));
    }
}

void ManageUsersDialog::onChangeGroupResult(const UserSystem::Request::Result &result)
{
    if (result == UserSystem::Request::Ok)
    {
        QMessageBox::information(this, tr("Changing group"), tr("Group changed successfully"));
        updateUserList();
    } else
    {
        QMessageBox::warning(this, tr("Changing group error"), UserSystem::toString(result));
    }
}

void ManageUsersDialog::onChangeDescriptionResult(const UserSystem::Request::Result &result)
{
    if (result == UserSystem::Request::Ok)
    {
        QMessageBox::information(this, tr("Changing description")
                                 , tr("Description changed successfully"));
        updateUserList();
    } else
    {
        QMessageBox::warning(this, tr("Changing description error")
                             , UserSystem::toString(result));
    }
}

void ManageUsersDialog::updateUserList()
{
    emit receiveUserList();
}

void ManageUsersDialog::addUserTriggered()
{
    AddUserDialog dialog(this);
    dialog.setUserClient(mUserClient);
    if (dialog.exec() == QDialog::Accepted)
        updateUserList();
}

void ManageUsersDialog::removeUserTriggered()
{
    if (view->currentIndex().isValid())
    {
        mUserName = model->index(view->currentIndex().row(), 0).data().toString();
        if (confirm(tr("Removing user..."), tr("Are you sure, you want to remove user %1?")
                    .arg(mUserName)))
        {
            emit removeUser(mUserName);
        }
    } else
    {
        invalidIndex();
    }
}

void ManageUsersDialog::setPasswordTriggered()
{
    if (view->currentIndex().isValid())
    {
        mUserName = model->index(view->currentIndex().row(), 0).data().toString();
        bool ok;
        QString password = QInputDialog::getText(this, tr("Changing password")
                                                 , tr("Input new password for user %1:")
                                                 .arg(mUserName)
                                                 , QLineEdit::Normal
                                                 , QString(), &ok);
        if (!ok)
            return;
        emit setPassword(mUserName, password);
    } else
    {
        invalidIndex();
    }
}

void ManageUsersDialog::changeGroupTriggered()
{
    if (view->currentIndex().isValid())
    {
        mUserName = model->index(view->currentIndex().row(), 0).data().toString();
        if (mUserName == "root")
            return;
        emit receiveGroupList();
    } else
    {
        invalidIndex();
    }
}

void ManageUsersDialog::changeDescriptionTriggered()
{
    if (view->currentIndex().isValid())
    {
        mUserName = model->index(view->currentIndex().row(), 0).data().toString();
        QString init = model->index(view->currentIndex().row(), 1).data().toString();
        bool ok;
        QString description = QInputDialog::getText(this, tr("Changing description")
                                                    , tr("Input new description for user %1:")
                                                    .arg(mUserName)
                                                    , QLineEdit::Normal
                                                    , init
                                                    , &ok);
        if (!ok)
            return;
        emit changeDescription(mUserName, description);
    } else
    {
        invalidIndex();
    }
}

void ManageUsersDialog::onCurrentRowChanged(const QModelIndex &index)
{
    disableButtons();
    if (index.isValid())
    {
        if (model->index(index.row(), 2).data().toString() != UserSystem::toDescription("Root"))
        {
            removeUserButton->setEnabled(true);
            changeGroupButton->setEnabled(true);
        }
        setPasswordButton->setEnabled(true);
        changeDescriptionButton->setEnabled(true);
    }
}

void ManageUsersDialog::onDoubleClicked(const QModelIndex &index)
{
    switch(index.column())
    {
    case 1:
        changeDescriptionTriggered();
        break;
    case 2:
        changeGroupTriggered();
        break;
    }
}

void ManageUsersDialog::closeEvent(QCloseEvent *)
{
    QSettings settings;
    settings.beginGroup(sysManageUsersDialog);
    settings.setValue(sysHeaderState, view->horizontalHeader()->saveState());
    settings.endGroup();
}

void ManageUsersDialog::invalidIndex()
{
    QMessageBox::warning(this, tr("Error"), tr("Select user"));
}

bool ManageUsersDialog::confirm(const QString &title, const QString &text)
{
    return (QMessageBox::Yes == QMessageBox::question(this, title, text
                                                      , QMessageBox::Yes | QMessageBox::No));
}

void ManageUsersDialog::disableButtons()
{
    removeUserButton->setEnabled(false);
    setPasswordButton->setEnabled(false);
    changeGroupButton->setEnabled(false);
    changeDescriptionButton->setEnabled(false);
}
