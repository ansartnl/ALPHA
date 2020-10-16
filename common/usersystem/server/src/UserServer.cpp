#include "UserServer.h"

#include "usHelperFunctions.h"
#include "usTransport.h"

namespace UserSystem
{

UserServer::UserServer(QObject *parent) :
    QObject(parent)
{
    registerUserSystemMetatypes();
}

void UserServer::addTransport(Transport *transport)
{
    connect(transport, SIGNAL(receive(QVariant)), SLOT(onReceived(QVariant)));
    mTransportPool.append(transport);
}

void UserServer::removeTransport(Transport *transport)
{
    disconnect(transport, 0, this, 0);
    mTransportPool.removeAll(transport);
}

void UserServer::fromByteArray(const QByteArray &groups, const QByteArray &users)
{
    mGroups.fromByteArray(groups);
    mUsers.fromByteArray(users);
    if (mGroups.isEmpty())
    {
        mGroups.add(new GroupRoot);
        emit groupsEmpty();
    }
    if (mUsers.isEmpty())
    {
        createRoot();
    }
    mUsers.setAllGroups(mGroups);
}

QByteArray UserServer::groupsToByteArray()
{
    return mGroups.toByteArray();
}

QByteArray UserServer::usersToByteArray()
{
    return mUsers.toByteArray();
}

void UserServer::addGroup(Group *group)
{
    mGroups.add(group);
}

User * UserServer::user(const QString &name) const
{
    return mUsers.value(name).data();
}

bool UserServer::checkPermission(const QString &user, const Permission &p) const
{
    if (!mUsers.contains(user))
        return false;
    return mUsers.value(user)->hasPermission(p);
}

bool UserServer::checkPermissions(const QString &user, const Permissions &p) const
{
    if (!mUsers.contains(user))
        return false;
    return mUsers.value(user)->hasPermissions(p);
}

bool UserServer::userIsInGroup(const QString &user, const QString &g) const
{
    if (!mUsers.contains(user))
        return false;
    return mUsers.value(user)->isInGroup(g);
}

void UserServer::sendReply(Transport *t, const QVariant &reply)
{
    RequestResult req;
    req.result = reply;
    t->sendData(QVariant::fromValue(req));
}

bool UserServer::groupsIsEmpty()
{
    return (mGroups.count() < 2);
}

void UserServer::onReceived(const QVariant &data)
{
    Transport *t = qobject_cast< Transport * >(sender());
    if (data.canConvert< LoginAttempt >())
    {
        LoginAttempt l = data.value< LoginAttempt >();
        login_p(t, l.name, l.password);
    } else if (data.canConvert< RequestAttempt >())
    {
        RequestAttempt l = data.value< RequestAttempt >();
        if (!tryProceedStandard(t, l.name, l.request))
        {
            emit received(t, l.name, l.request);
        }
    }
    emit dataReceived(data);
}

bool UserServer::tryProceedStandard(Transport *t, const QString &name, const QVariant &request)
{
    if (request.canConvert< LogoutAttempt >())
    {
        UserResult r;
        r.name = name;
        r.action = Request::Logout;
        if (mUsers.contains(name))
        {
            logout_p(t, name);
            r.result = Request::Ok;
        } else
        {
            r.result = Request::NoUser;
        }
        sendReply(t, QVariant::fromValue(r));
        return true;
    } else if (request.canConvert< ChangePasswordAttempt >())
    {
        ChangePasswordAttempt l = request.value< ChangePasswordAttempt >();
        UserResult r;
        r.name = name;
        r.action = Request::ChangePassword;
        if (mUsers.contains(name))
        {
            if (mUsers.value(name)->hasPermission(ChangePassword))
            {
                mUsers.value(name)->setPassword(l.password);
                r.result = Request::Ok;
            } else
            {
                r.result = Request::NotEnoughPrivileges;
            }
        } else
        {
            r.result = Request::NoUser;
        }
        sendReply(t, QVariant::fromValue(r));
        return true;
    } else if (request.canConvert< SetPasswordAttempt >())
    {
        SetPasswordAttempt l = request.value< SetPasswordAttempt >();
        UserResult r;
        r.name = name;
        r.action = Request::SetPassword;
        if (mUsers.contains(name))
        {
            if (mUsers.value(name)->hasPermission(SetPassword))
            {
                if (mUsers.contains(l.name))
                {
                    mUsers.value(l.name)->setPassword(l.password);
                    r.result = Request::Ok;
                } else
                {
                    r.result = Request::NotExists;
                }
            } else
            {
                r.result = Request::NotEnoughPrivileges;
            }
        } else
        {
            r.result = Request::NoUser;
        }
        sendReply(t, QVariant::fromValue(r));
        return true;
    } else if (request.canConvert< SetDescriptionAttempt >())
    {
        SetDescriptionAttempt l = request.value< SetDescriptionAttempt >();
        UserResult r;
        r.name = name;
        r.action = Request::SetDescription;
        if (mUsers.contains(name))
        {
            if (mUsers.value(name)->hasPermission(ChangeUserInfo))
            {
                if (mUsers.contains(l.name))
                {
                    mUsers.value(l.name)->setDescription(l.description);
                    r.result = Request::Ok;
                } else
                {
                    r.result = Request::NotExists;
                }
            } else
            {
                r.result = Request::NotEnoughPrivileges;
            }
        } else
        {
            r.result = Request::NoUser;
        }
        sendReply(t, QVariant::fromValue(r));
        return true;
    } else if (request.canConvert< AddUserAttempt >())
    {
        AddUserAttempt l = request.value< AddUserAttempt >();
        UserResult r;
        r.name = name;
        r.action = Request::AddUser;
        if (mUsers.contains(name))
        {
            if (mUsers.value(name)->hasPermission(AddUser))
            {
                if (!mUsers.contains(l.name))
                {
                    User *user = new User;
                    user->setName(l.name);
                    user->setPassword(l.password);
                    user->setDescription(l.name);
                    user->setAllGroups(mGroups);
                    mUsers.add(user);
                    r.result = Request::Ok;
                } else
                {
                    r.result = Request::AlreadyExists;
                }
            } else
            {
                r.result = Request::NotEnoughPrivileges;
            }
        } else
        {
            r.result = Request::NoUser;
        }
        sendReply(t, QVariant::fromValue(r));
        return true;
    } else if (request.canConvert< RemoveUserAttempt >())
    {
        RemoveUserAttempt l = request.value< RemoveUserAttempt >();
        UserResult r;
        r.name = name;
        r.action = Request::RemoveUser;
        if (mUsers.contains(name))
        {
            if (mUsers.value(name)->hasPermission(RemoveUser))
            {
                if (mUsers.contains(l.name))
                {
                    if (name != l.name)
                    {
                        clearUser(l.name);
                        User *u = user(l.name);
                        mUsers.remove(l.name);
                        delete u;
                        r.result = Request::Ok;
                    } else
                    {
                        r.result = Request::RemoveSelf;
                    }
                } else
                {
                    r.result = Request::NotExists;
                }
            } else
            {
                r.result = Request::NotEnoughPrivileges;
            }
        } else
        {
            r.result = Request::NoUser;
        }
        sendReply(t, QVariant::fromValue(r));
        return true;
    } else if (request.canConvert< AddUserToGroupAttempt >())
    {
        AddUserToGroupAttempt l = request.value< AddUserToGroupAttempt >();
        UserResult r;
        r.name = name;
        r.action = Request::AddUserToGroup;
        if (mUsers.contains(name))
        {
            if (mUsers.value(name)->hasPermission(AddPermissions))
            {
                if (mUsers.contains(l.name))
                {
                    mUsers.value(l.name)->addToGroup(l.group);
                    mGroups.value(l.group)->addUser(l.name);
                    r.result = Request::Ok;
                } else
                {
                    r.result = Request::NotExists;
                }
            } else
            {
                r.result = Request::NotEnoughPrivileges;
            }
        } else
        {
            r.result = Request::NoUser;
        }
        sendReply(t, QVariant::fromValue(r));
        return true;
    } else if (request.canConvert< RemoveUserFromGroupAttempt >())
    {
        RemoveUserFromGroupAttempt l = request.value< RemoveUserFromGroupAttempt >();
        UserResult r;
        r.name = name;
        r.action = Request::RemoveUserFromGroup;
        if (mUsers.contains(name))
        {
            if (mUsers.value(name)->hasPermission(RemovePermissions))
            {
                if (mUsers.contains(l.name))
                {
                    mUsers.value(l.name)->removeFromGroup(l.group);
                    mGroups.value(l.group)->removeUser(l.name);
                    r.result = Request::Ok;
                } else
                {
                    r.result = Request::NotExists;
                }
            } else
            {
                r.result = Request::NotEnoughPrivileges;
            }
        } else
        {
            r.result = Request::NoUser;
        }
        sendReply(t, QVariant::fromValue(r));
        return true;
    } else if (request.canConvert< SetUserGroupAttempt >())
    {
        SetUserGroupAttempt l = request.value< SetUserGroupAttempt >();
        UserResult r;
        r.name = name;
        r.action = Request::SetUserGroup;
        if (mUsers.contains(name))
        {
            if (mUsers.value(name)->hasPermission(RemovePermissions))
            {
                if (mUsers.contains(l.name))
                {
                    clearUser(l.name);
                    mUsers.value(l.name)->setGroup(l.group);
                    mGroups.value(l.group)->addUser(l.name);
                    r.result = Request::Ok;
                } else
                {
                    r.result = Request::NotExists;
                }
            } else
            {
                r.result = Request::NotEnoughPrivileges;
            }
        } else
        {
            r.result = Request::NoUser;
        }
        sendReply(t, QVariant::fromValue(r));
        return true;
    } else if (request.canConvert< UserListAttempt >())
    {
        UserListResult r;
        if (mUsers.contains(name))
        {
            if (mUsers.value(name)->hasPermission(ReadPermissions))
            {
                r.users = mUsers;
                r.result = Request::Ok;
            } else
            {
                r.result = Request::NotEnoughPrivileges;
            }
        } else
        {
            r.result = Request::NoUser;
        }
        sendReply(t, QVariant::fromValue(r));
        return true;
    } else if (request.canConvert< GroupListAttempt >())
    {
        GroupListResult r;
        if (mUsers.contains(name))
        {
            if (mUsers.value(name)->hasPermission(ReadPermissions))
            {
                r.groups = mGroups;
                r.result = Request::Ok;
            } else
            {
                r.result = Request::NotEnoughPrivileges;
            }
        } else
        {
            r.result = Request::NoUser;
        }
        sendReply(t, QVariant::fromValue(r));
        return true;
    }
    return false;
}

void UserServer::login_p(Transport *t, const QString &name, const QString &password)
{
    LoginResult r;
    if (mUsers.contains(name) && mUsers.value(name)->passHash() == passwordHash(password))
    {
        User *u = user(name);
        if (u->groups().isEmpty())
        {
            r.result = Request::NotEnoughPrivileges;
        } else
        {
            r.user = *u;
            r.result = Request::Ok;
            r.allGroups = mGroups;
            t->setUserName(name);
            emit logined(t, name);
        }
    } else
    {
        if (!mUsers.contains(name))
            r.result = Request::NoUser;
        else
            r.result = Request::WrongPassword;
    }
    t->sendData(QVariant::fromValue(r));
}

void UserServer::logout_p(Transport *t, const QString &name)
{
    t->setUserName("");
    emit logouted(t, name);
}

void UserServer::clearUser(const QString &name)
{
    QStringList groups = mUsers.value(name)->groups();
    foreach (const QString &group, groups)
    {
        mGroups.value(group)->removeUser(name);
    }
}

void UserServer::createRoot()
{
    mUsers.clear();
    User *root = new User;
    root->setName("root");
    root->setDescription("root");
    root->setGroup("Root");
    root->setPassword("root");
    mUsers.add(root);
    mGroups.value("Root")->addUser(root->name());
}

UserServer * userServer(QObject *parent)
{
    static UserServer *mUserServer = new UserServer(parent);
    return mUserServer;
}

} // namespace
