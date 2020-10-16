#include "UserClient.h"

#include "usTransport.h"

namespace UserSystem
{

UserClient::UserClient(Transport *transport, QObject *parent) :
    QObject(parent)
{
    mTransport = transport;
    connect(transport, SIGNAL(receive(const QVariant &)), this, SLOT(onReceived(const QVariant &)));

    registerUserSystemMetatypes();
}

bool UserClient::testPassword(const QString &password)
{
    return (mUser.passHash() == passwordHash(password));
}

User UserClient::user() const
{
    return mUser;
}

void UserClient::login(const QString &name, const QString &password)
{
    LoginAttempt r;
    r.name = name.trimmed();
    r.password = password;
    mTransport->sendData(QVariant::fromValue(r));
}

void UserClient::logout()
{
    LogoutAttempt r;
    sendRequest(QVariant::fromValue(r));
}

void UserClient::changePassword(const QString &password)
{
    ChangePasswordAttempt r;
    r.password = password;
    sendRequest(QVariant::fromValue(r));
}

void UserClient::setPassword(const QString &name, const QString &password)
{
    SetPasswordAttempt r;
    r.name = name;
    r.password = password;
    sendRequest(QVariant::fromValue(r));
}

void UserClient::setDescription(const QString &name, const QString &description)
{
    SetDescriptionAttempt r;
    r.name = name;
    r.description = description;
    sendRequest(QVariant::fromValue(r));
}

void UserClient::addUser(const QString &name, const QString &password)
{
    AddUserAttempt r;
    r.name = name.trimmed();
    r.password = password;
    sendRequest(QVariant::fromValue(r));
}

void UserClient::removeUser(const QString &name)
{
    RemoveUserAttempt r;
    r.name = name;
    sendRequest(QVariant::fromValue(r));
}

void UserClient::addUserToGroup(const QString &name, const QString &group)
{
    AddUserToGroupAttempt r;
    r.name = name;
    r.group = group;
    sendRequest(QVariant::fromValue(r));
}

void UserClient::removeUserFromGroup(const QString &name, const QString &group)
{
    RemoveUserFromGroupAttempt r;
    r.name = name;
    r.group = group;
    sendRequest(QVariant::fromValue(r));
}

void UserClient::setUserGroup(const QString &name, const QString &group)
{
    SetUserGroupAttempt r;
    r.name = name;
    r.group = group;
    sendRequest(QVariant::fromValue(r));
}

void UserClient::getUserList()
{
    UserListAttempt r;
    sendRequest(QVariant::fromValue(r));
}

void UserClient::getGroupList()
{
    GroupListAttempt r;
    sendRequest(QVariant::fromValue(r));
}

void UserClient::sendRequest(const QVariant &request)
{
    RequestAttempt req;
    req.name = mUser.name();
    req.request = request;
    mTransport->sendData(QVariant::fromValue(req));
}

void UserClient::onReceived(const QVariant &data)
{
    if (data.canConvert< LoginResult >())
    {
        LoginResult r = data.value< LoginResult >();
        if (r.result == Request::Ok)
            login_p(r.user, r.allGroups);
        emit loginResult(r.result);
    } else if (data.canConvert< RequestResult >())
    {
        RequestResult r = data.value< RequestResult >();
        if (!tryProceedStandard(r.result))
            emit received(r.result);
    }
}

bool UserClient::tryProceedStandard(const QVariant &reply)
{
    if (reply.canConvert< UserResult >())
    {
        UserResult r = reply.value< UserResult >();
        switch (r.action)
        {
        case Request::Logout:
            logout_p();
            emit logoutResult(r.result);
            break;
        case Request::ChangePassword:
            emit changePasswordResult(r.result);
            break;
        case Request::SetPassword:
            emit setPasswordResult(r.result);
            break;
        case Request::SetDescription:
            emit setDescriptionResult(r.result);
            break;
        case Request::AddUser:
            emit addUserResult(r.result);
            break;
        case Request::RemoveUser:
            emit removeUserResult(r.result);
            break;
        case Request::AddUserToGroup:
            emit addUserToGroupResult(r.result);
            break;
        case Request::RemoveUserFromGroup:
            emit removeUserFromGroupResult(r.result);
            break;
        case Request::SetUserGroup:
            emit setUserGroupResult(r.result);
            break;
        }
        return true;
    } else if (reply.canConvert< UserListResult >())
    {
        UserListResult r = reply.value< UserListResult >();
        if (r.result == Request::Ok)
        {
            emit userListReceived(r.users);
        } else
        {
            emit getUserListResult(r.result);
        }
        if (r.deserialized)
        {
            foreach (const QString &key, r.users.keys())
                r.users.remove(key); // shared pointer will remove itself
        }
    } else if (reply.canConvert< GroupListResult >())
    {
        GroupListResult r = reply.value< GroupListResult >();
        if (r.result == Request::Ok)
        {
            mUser.setAllGroups(r.groups);
            emit groupListReceived(r.groups);
        } else
        {
            emit getGroupListResult(r.result);
        }
    }
    return false;
}

void UserClient::login_p(const User &newUser, const Groups &allGroups)
{
    logout_p();
    mUser = newUser;
    mUser.setAllGroups(allGroups);
}

void UserClient::logout_p()
{
    mUser.clear();
}

UserClient * userClient(Transport *transport, QObject *parent)
{
    static UserClient *mUserClient = new UserClient(transport, parent);
    return mUserClient;
}

bool userCan(UserSystem::Permission p)
{
    return UserSystem::userClient()->user().hasPermission(p);
}

} // namespace
