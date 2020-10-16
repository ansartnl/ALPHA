#include "DialogManager.h"

#include "AddUserDialog.h"
#include "ChangePasswordDialog.h"
#include "LoginDialog.h"
#include "ManageUsersDialog.h"
#include "UserClient.h"

#include <QtCore/QSettings>

#include <QtGui/QInputDialog>

DialogManager::DialogManager(UserSystem::UserClient *userClient, QWidget *widget, QObject *parent) :
    QObject(parent)
{
    mUserClient = userClient;
    mWidget = widget;
    mUseSettings = true;
    loginning = false;

    connect(mUserClient, SIGNAL(logoutResult(UserSystem::Request::Result))
            , this, SLOT(logoutResult(UserSystem::Request::Result)));
}

void DialogManager::setUsingSettings(bool use)
{
    mUseSettings = use;
}

void DialogManager::setUsersIcon(const QString &iconFile)
{
    mIcon = iconFile;
}

void DialogManager::forceLogout()
{
    emit logouted();
}

void DialogManager::reLogin()
{
    if (loginning)
        return;

    QString name = mUserClient->user().name();
    if (name.isEmpty())
        return;
    loginning = true;
    LoginDialog dialog(mWidget);
    dialog.setUserClient(mUserClient);
    dialog.setUserName(name);
    if (dialog.exec() == QDialog::Accepted)
        emit logined();
    else
        mUserClient->logout();
    loginning = false;
}

void DialogManager::addUser()
{
    AddUserDialog dialog(mWidget);
    dialog.setUserClient(mUserClient);
    if (dialog.exec() == QDialog::Accepted)
        emit userAdded();
}

void DialogManager::changePassword()
{
    ChangePasswordDialog dialog(mWidget);
    dialog.setUserClient(mUserClient);
    if (dialog.exec() == QDialog::Accepted)
        emit passwordChanged();
}

void DialogManager::login()
{
    if (loginning)
        return;

    loginning = true;

    QSettings settings;
    LoginDialog dialog(mWidget);
    dialog.setUserClient(mUserClient);
    if (mUseSettings)
        dialog.setUserName(settings.value(s_userName, "root").toString());
    if (dialog.exec() == QDialog::Accepted)
    {
        emit logined();
        if (mUseSettings)
            settings.setValue(s_userName, dialog.userName());
    }

    loginning = false;
}

void DialogManager::logout()
{
    mUserClient->logout();
}

void DialogManager::manageUsers()
{
    ManageUsersDialog dialog(mWidget);
    dialog.setUserClient(mUserClient);
    dialog.setWindowIcon(QIcon(mIcon));
    dialog.exec();
}

void DialogManager::logoutResult(const UserSystem::Request::Result &result)
{
    if (result == UserSystem::Request::Ok)
        emit logouted();
}
