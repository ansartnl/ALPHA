#ifndef DIALOGMANAGER_H
#define DIALOGMANAGER_H

#include <QtCore/QObject>

#include "usDeclare.h"

namespace UserSystem
{

class User;
class UserClient;

}

class DialogManager : public QObject
{
    Q_OBJECT
public:
    explicit DialogManager(UserSystem::UserClient *userClient
                           , QWidget *widget, QObject *parent = 0);
    void setUsingSettings(bool use = true);
    void setUsersIcon(const QString &iconFile = QString());

public slots:
    void forceLogout();
    void reLogin();

signals:
    void userAdded();
    void passwordChanged();
    void logined();
    void logouted();

public slots:
    void addUser();
    void changePassword();
    void login();
    void logout();
    void manageUsers();

protected slots:
    void logoutResult(const UserSystem::Request::Result &result);

protected:
    UserSystem::UserClient *mUserClient;
    QWidget *mWidget;
    bool mUseSettings;
    QString mIcon;
    bool loginning;
};

#endif // DIALOGMANAGER_H
