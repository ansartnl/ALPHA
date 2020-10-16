#ifndef USERCLIENT_H
#define USERCLIENT_H

#include "usHelperFunctions.h"
#include "usProto.h"
#include "usUser.h"

#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QVariant>

namespace UserSystem
{

class Transport;

class UserClient : public QObject
{
    Q_OBJECT
public:
    explicit UserClient(Transport *transport, QObject *parent = 0);

    bool testPassword(const QString &password);
    User user() const;

public slots:
    void login(const QString &name, const QString &password);
    void logout();
    void changePassword(const QString &password);
    void setPassword(const QString &name, const QString &password);
    void setDescription(const QString &name, const QString &description);

    void addUser(const QString &name, const QString &password);
    void removeUser(const QString &name);
    void addUserToGroup(const QString &name, const QString &group);
    void removeUserFromGroup(const QString &name, const QString &group);
    void setUserGroup(const QString &name, const QString &group);

    void getUserList();
    void getGroupList();

    void sendRequest(const QVariant &request);

protected slots:
    void onReceived(const QVariant &data);

signals:
    void loginResult(const UserSystem::Request::Result &result);
    void logoutResult(const UserSystem::Request::Result &result);
    void changePasswordResult(const UserSystem::Request::Result &result);
    void setPasswordResult(const UserSystem::Request::Result &result);
    void setDescriptionResult(const UserSystem::Request::Result &result);

    void addUserResult(const UserSystem::Request::Result &result);
    void removeUserResult(const UserSystem::Request::Result &result);
    void addUserToGroupResult(const UserSystem::Request::Result &result);
    void removeUserFromGroupResult(const UserSystem::Request::Result &result);
    void setUserGroupResult(const UserSystem::Request::Result &result);

    void getUserListResult(const UserSystem::Request::Result &result);
    void userListReceived(const UserSystem::Users &users);

    void getGroupListResult(const UserSystem::Request::Result &result);
    void groupListReceived(const UserSystem::Groups &groups);

    void received(const QVariant &data);

protected:
    bool tryProceedStandard(const QVariant &reply);
    void login_p(const User &newUser, const Groups &allGroups);
    void logout_p();

protected:
    Transport *mTransport;
    User mUser;
};

UserClient * userClient(Transport *transport = 0, QObject *parent = 0);

bool userCan(UserSystem::Permission p);

} // namespace

#endif // USERCLIENT_H
