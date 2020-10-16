#ifndef MANAGEUSERSDIALOG_H
#define MANAGEUSERSDIALOG_H

#include "ui_ManageUsersDialog.h"

#include "usDeclare.h"

namespace UserSystem
{

class Groups;
class Users;
class UserClient;

}

class QStandardItemModel;

class ManageUsersDialog : public QDialog, private Ui::ManageUsersDialog
{
    Q_OBJECT

public:
    explicit ManageUsersDialog(QWidget *parent = 0);
    virtual ~ManageUsersDialog();

    void setUserClient(UserSystem::UserClient *userClient);

signals:
    void receiveUserList();
    void receiveGroupList();

    void removeUser(const QString &userName);
    void setPassword(const QString &userName, const QString &password);
    void changeGroup(const QString &userName, const QString &group);
    void changeDescription(const QString &userName, const QString &description);

public slots:
    void onUserListReceived(const UserSystem::Users &users);
    void onGroupListReceived(const UserSystem::Groups &groups);

    void onUserListResult(const UserSystem::Request::Result &result);
    void onGroupListResult(const UserSystem::Request::Result &result);

    void onRemoveUserResult(const UserSystem::Request::Result &result);
    void onSetPasswordResult(const UserSystem::Request::Result &result);
    void onChangeGroupResult(const UserSystem::Request::Result &result);
    void onChangeDescriptionResult(const UserSystem::Request::Result &result);

protected slots:
    void updateUserList();

    void addUserTriggered();
    void removeUserTriggered();
    void setPasswordTriggered();
    void changeGroupTriggered();
    void changeDescriptionTriggered();

    void onCurrentRowChanged(const QModelIndex &index);
    void onDoubleClicked(const QModelIndex &index);

protected:
    void closeEvent(QCloseEvent *);
    void invalidIndex();
    bool confirm(const QString &title, const QString &text);
    void disableButtons();

protected:
    QStandardItemModel *model;
    UserSystem::UserClient *mUserClient;
    QString mUserName;
};

#endif // MANAGEUSERSDIALOG_H
