#include "usUser.h"

#include "usHelperFunctions.h"

#include <QtCore/QRegExp>

namespace UserSystem
{

User::User()
{
}

User::User(const User &u)
{
    mName = u.mName;
    mPassHash = u.mPassHash;
    mDescription = u.mDescription;
    mGroups = u.mGroups;
    mAllGroups = u.mAllGroups;
}

QByteArray User::toByteArray() const
{
    QByteArray ret;
    ret += UserSignatures::s_signature;
    ret += UserSignatures::s_user_open;
    ret += UserSignatures::s_name_open;
    ret += mName.toUtf8();
    ret += UserSignatures::s_name_close;
    ret += UserSignatures::s_pass_open;
    ret += mPassHash.toUtf8();
    ret += UserSignatures::s_pass_close;
    ret += UserSignatures::s_desc_open;
    ret += mDescription.toUtf8();
    ret += UserSignatures::s_desc_close;
    ret += UserSignatures::s_groups_open;
    foreach (const QString &g, mGroups)
    {
        ret += UserSignatures::s_group_open;
        ret += g.toUtf8();
        ret += UserSignatures::s_group_close;
    }
    ret += UserSignatures::s_groups_close;
    ret += UserSignatures::s_user_close;
    return ret;
}

bool User::fromByteArray(const QByteArray &array)
{
    clear();
    QRegExp reg(QString("^(%1)(%2)(%3)(.+)(%4)(%5)(.{0,})(%6)(%7)(.{0,})(%8)(%9)(.{0,})(%10)(%11)$")
                .arg(UserSignatures::s_signature)
                .arg(UserSignatures::s_user_open)
                .arg(UserSignatures::s_name_open)
                .arg(UserSignatures::s_name_close)
                .arg(UserSignatures::s_pass_open)
                .arg(UserSignatures::s_pass_close)
                .arg(UserSignatures::s_desc_open)
                .arg(UserSignatures::s_desc_close)
                .arg(UserSignatures::s_groups_open)
                .arg(UserSignatures::s_groups_close)
                .arg(UserSignatures::s_user_close)
                );
    reg.setMinimal(true);
    if (!reg.exactMatch(QString(array)))
        return false;
    mName = reg.cap(4);
    mPassHash = reg.cap(7);
    mDescription = reg.cap(10);
    QString allGroups = reg.cap(13);
    QRegExp regGroups(QString("(%1)(.+)(%2)")
                      .arg(UserSignatures::s_group_open)
                      .arg(UserSignatures::s_group_close)
                      );
    regGroups.setMinimal(true);
    while (regGroups.indexIn(allGroups) != -1)
    {
        mGroups.append(regGroups.cap(2));
        allGroups.remove(regGroups.cap(0));
    }
    return allGroups.isEmpty();
}

void User::clear()
{
    mName.clear();
    mPassHash.clear();
    mDescription.clear();
    mGroups.clear();
}

void User::setName(const QString &name)
{
    mName = name;
}

void User::setPassword(const QString &password)
{
    mPassHash = passwordHash(password);
}

void User::setDescription(const QString &description)
{
    mDescription = description;
}

void User::setAllGroups(const Groups &allGroups)
{
    mAllGroups = allGroups;
}

void User::setGroup(const QString &g)
{
    mGroups.clear();
    addToGroup(g);
}

void User::addToGroup(const QString &g)
{
    mGroups.append(g);
}

void User::removeFromGroup(const QString &g)
{
    mGroups.removeAll(g);
}

QString User::name() const
{
    return mName;
}

QString User::passHash() const
{
    return mPassHash;
}

QString User::description() const
{
    return mDescription;
}

QStringList User::groups() const
{
    return mGroups;
}

bool User::isInGroup(const QString &g) const
{
    return mGroups.contains(g);
}

bool User::hasPermission(const Permission &p) const
{
    foreach (const QString &g, mGroups)
    {
        if (mAllGroups.contains(g))
        {
            if (mAllGroups.value(g)->contains(p))
                return true;
        }
    }
    return false;
}

bool User::hasPermissions(const Permissions &p) const
{
    foreach (const QString &g, mGroups)
    {
        if (mAllGroups.contains(g))
        {
            if (mAllGroups.value(g)->contains(p))
                return true;
        }
    }
    return false;
}

User & User::operator =(const User &u)
{
    mName = u.mName;
    mPassHash = u.mPassHash;
    mDescription = u.mDescription;
    mGroups = u.mGroups;
    mAllGroups = u.mAllGroups;
    return *this;
}

} // namespace
