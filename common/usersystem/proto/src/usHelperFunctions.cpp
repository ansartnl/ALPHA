#include "usHelperFunctions.h"

#include <QtCore/QByteArray>
#include <QtCore/QCryptographicHash>
#include <QtCore/QObject>

namespace UserSystem
{

void setSalt(const QString &s)
{
    UserSystem::m_salt = s;
}

QString salt()
{
    return m_salt;
}

QString passwordHash(const QString &password, const QString &s)
{
    QString passWithSalt = password + s;
    return QCryptographicHash::hash(passWithSalt.toUtf8().data(), QCryptographicHash::Md5).toHex();
}

QString toString(const Request::Action &a)
{
    switch (a)
    {
    case Request::Logout:
        return QObject::tr("Logout");
    case Request::ChangePassword:
        return QObject::tr("Change password");
    case Request::SetPassword:
        return QObject::tr("Set password");
    case Request::SetDescription:
        return QObject::tr("Set description");
    case Request::AddUser:
        return QObject::tr("Add user");
    case Request::RemoveUser:
        return QObject::tr("Remove user");
    case Request::AddUserToGroup:
        return QObject::tr("Add user to group");
    case Request::RemoveUserFromGroup:
        return QObject::tr("Remove user from group");
    case Request::SetUserGroup:
        return QObject::tr("Set user group");
    }
    return QString();
}

QString toString(const Request::Result &r)
{
    switch (r)
    {
    case Request::Ok:
        return QObject::tr("Ok");
    case Request::NoUser:
        return QObject::tr("No such user");
    case Request::WrongPassword:
        return QObject::tr("Wrong password");
    case Request::NotEnoughPrivileges:
        return QObject::tr("Not enough privileges");
    case Request::AlreadyExists:
        return QObject::tr("Already exists");
    case Request::NotExists:
        return QObject::tr("Not exists");
    case Request::RemoveSelf:
        return QObject::tr("You cannot remove yourself");
    }
    return QString();
}

} // namespace
