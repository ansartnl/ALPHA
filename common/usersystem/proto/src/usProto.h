#ifndef PROTO_H
#define PROTO_H

#include "usGroups.h"
#include "usUsers.h"

#include "enumstreaming.h"

#include <QtCore/QString>

namespace UserSystem
{

void registerUserSystemMetatypes();

QT_DECLARE_ENUM_STREAMING(Request::Action)
QT_DECLARE_ENUM_STREAMING(Request::Result)

// login
struct LoginAttempt
{
    QString name;
    QString password;
};

struct LoginResult
{
    Request::Result result;
    User user;
    Groups allGroups;
};

// logout
struct LogoutAttempt
{
};

// change password
struct ChangePasswordAttempt
{
    QString password;
};

// set password
struct SetPasswordAttempt
{
    QString name;
    QString password;
};

// set description
struct SetDescriptionAttempt
{
    QString name;
    QString description;
};

// add user
struct AddUserAttempt
{
    QString name;
    QString password;
};

// remove user
struct RemoveUserAttempt
{
    QString name;
};

// add user to group
struct AddUserToGroupAttempt
{
    QString name;
    QString group;
};

// remove user from group
struct RemoveUserFromGroupAttempt
{
    QString name;
    QString group;
};

// remove user from group
struct SetUserGroupAttempt
{
    QString name;
    QString group;
};

// get user list
struct UserListAttempt
{
};

// reseive user list
struct 	UserListResult
{
    Request::Result result;
    Users users;
    bool deserialized;
    UserListResult() :
        deserialized(false)
    {}
};

// get group list
struct GroupListAttempt
{
};

// reseive group list
struct GroupListResult
{
    Request::Result result;
    Groups groups;
};

// result
struct UserResult
{
    QString name;
    Request::Action action;
    Request::Result result;
};

// manual request
struct RequestAttempt
{
    QString name;
    QVariant request;
};

struct RequestResult
{
    QVariant result;
};

inline QDataStream &operator<< (QDataStream &stream, const LoginAttempt &l)
{
    stream << l.name;
    stream << l.password;
    return stream;
}

inline QDataStream &operator>> (QDataStream &stream, LoginAttempt &l)
{
    stream >> l.name;
    stream >> l.password;
    return stream;
}

inline QDataStream &operator<< (QDataStream &stream, const LoginResult &l)
{
    stream << l.result;
    stream << l.user.toByteArray();
    stream << l.allGroups.toByteArray();
    return stream;
}

inline QDataStream &operator>> (QDataStream &stream, LoginResult &l)
{
    stream >> l.result;
    QByteArray a;
    stream >> a;
    l.user.fromByteArray(a);
    stream >> a;
    l.allGroups.fromByteArray(a);
    return stream;
}

inline QDataStream &operator<< (QDataStream &stream, const LogoutAttempt &)
{
    return stream;
}

inline QDataStream &operator>> (QDataStream &stream, LogoutAttempt &)
{
    return stream;
}

inline QDataStream &operator<< (QDataStream &stream, const ChangePasswordAttempt &l)
{
    stream << l.password;
    return stream;
}

inline QDataStream &operator>> (QDataStream &stream, ChangePasswordAttempt &l)
{
    stream >> l.password;
    return stream;
}

inline QDataStream &operator<< (QDataStream &stream, const SetPasswordAttempt &l)
{
    stream << l.name;
    stream << l.password;
    return stream;
}

inline QDataStream &operator>> (QDataStream &stream, SetPasswordAttempt &l)
{
    stream >> l.name;
    stream >> l.password;
    return stream;
}

inline QDataStream &operator<< (QDataStream &stream, const SetDescriptionAttempt &l)
{
    stream << l.name;
    stream << l.description;
    return stream;
}

inline QDataStream &operator>> (QDataStream &stream, SetDescriptionAttempt &l)
{
    stream >> l.name;
    stream >> l.description;
    return stream;
}

inline QDataStream &operator<< (QDataStream &stream, const AddUserAttempt &l)
{
    stream << l.name;
    stream << l.password;
    return stream;
}

inline QDataStream &operator>> (QDataStream &stream, AddUserAttempt &l)
{
    stream >> l.name;
    stream >> l.password;
    return stream;
}

inline QDataStream &operator<< (QDataStream &stream, const RemoveUserAttempt &l)
{
    stream << l.name;
    return stream;
}

inline QDataStream &operator>> (QDataStream &stream, RemoveUserAttempt &l)
{
    stream >> l.name;
    return stream;
}

inline QDataStream &operator<< (QDataStream &stream, const AddUserToGroupAttempt &l)
{
    stream << l.name;
    stream << l.group;
    return stream;
}

inline QDataStream &operator>> (QDataStream &stream, AddUserToGroupAttempt &l)
{
    stream >> l.name;
    stream >> l.group;
    return stream;
}

inline QDataStream &operator<< (QDataStream &stream, const RemoveUserFromGroupAttempt &l)
{
    stream << l.name;
    stream << l.group;
    return stream;
}

inline QDataStream &operator>> (QDataStream &stream, RemoveUserFromGroupAttempt &l)
{
    stream >> l.name;
    stream >> l.group;
    return stream;
}

inline QDataStream &operator<< (QDataStream &stream, const SetUserGroupAttempt &l)
{
    stream << l.name;
    stream << l.group;
    return stream;
}

inline QDataStream &operator>> (QDataStream &stream, SetUserGroupAttempt &l)
{
    stream >> l.name;
    stream >> l.group;
    return stream;
}

inline QDataStream &operator<< (QDataStream &stream, const UserListAttempt &)
{
    return stream;
}

inline QDataStream &operator>> (QDataStream &stream, UserListAttempt &)
{
    return stream;
}

inline QDataStream &operator<< (QDataStream &stream, const UserListResult &l)
{
    stream << l.result;
    stream << l.users.toByteArray();
    return stream;
}

inline QDataStream &operator>> (QDataStream &stream, UserListResult &l)
{
    stream >> l.result;
    QByteArray a;
    stream >> a;
    l.users.fromByteArray(a);
    l.deserialized = true;
    return stream;
}

inline QDataStream &operator<< (QDataStream &stream, const GroupListAttempt &)
{
    return stream;
}

inline QDataStream &operator>> (QDataStream &stream, GroupListAttempt &)
{
    return stream;
}

inline QDataStream &operator<< (QDataStream &stream, const GroupListResult &l)
{
    stream << l.result;
    stream << l.groups.toByteArray();
    return stream;
}

inline QDataStream &operator>> (QDataStream &stream, GroupListResult &l)
{
    stream >> l.result;
    QByteArray a;
    stream >> a;
    l.groups.fromByteArray(a);
    return stream;
}

inline QDataStream &operator<< (QDataStream &stream, const UserResult &l)
{
    stream << l.name;
    stream << l.action;
    stream << l.result;
    return stream;
}

inline QDataStream &operator>> (QDataStream &stream, UserResult &l)
{
    stream >> l.name;
    stream >> l.action;
    stream >> l.result;
    return stream;
}

inline QDataStream &operator<< (QDataStream &stream, const RequestAttempt &r)
{
    stream << r.name;
    stream << r.request;
    return stream;
}

inline QDataStream &operator>> (QDataStream &stream, RequestAttempt &r)
{
    stream >> r.name;
    stream >> r.request;
    return stream;
}

inline QDataStream &operator<< (QDataStream &stream, const RequestResult &r)
{
    stream << r.result;
    return stream;
}

inline QDataStream &operator>> (QDataStream &stream, RequestResult &r)
{
    stream >> r.result;
    return stream;
}

} // namespace

Q_DECLARE_METATYPE(UserSystem::LoginAttempt)
Q_DECLARE_METATYPE(UserSystem::LoginResult)
Q_DECLARE_METATYPE(UserSystem::LogoutAttempt)
Q_DECLARE_METATYPE(UserSystem::ChangePasswordAttempt)
Q_DECLARE_METATYPE(UserSystem::SetPasswordAttempt)
Q_DECLARE_METATYPE(UserSystem::SetDescriptionAttempt)
Q_DECLARE_METATYPE(UserSystem::AddUserAttempt)
Q_DECLARE_METATYPE(UserSystem::RemoveUserAttempt)
Q_DECLARE_METATYPE(UserSystem::AddUserToGroupAttempt)
Q_DECLARE_METATYPE(UserSystem::RemoveUserFromGroupAttempt)
Q_DECLARE_METATYPE(UserSystem::SetUserGroupAttempt)
Q_DECLARE_METATYPE(UserSystem::UserListAttempt)
Q_DECLARE_METATYPE(UserSystem::UserListResult)
Q_DECLARE_METATYPE(UserSystem::GroupListAttempt)
Q_DECLARE_METATYPE(UserSystem::GroupListResult)
Q_DECLARE_METATYPE(UserSystem::UserResult)
Q_DECLARE_METATYPE(UserSystem::RequestAttempt)
Q_DECLARE_METATYPE(UserSystem::RequestResult)

#endif // PROTO_H
