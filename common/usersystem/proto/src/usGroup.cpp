#include "usGroup.h"

#include "usUsers.h"

#include <QtCore/QByteArray>

namespace UserSystem
{

Group::Group()
{
}

Group::Group(QString name)
{
    mName = name;
}

Group::Group(QString name, Permission p)
{
    mName = name;
    addPermission(p);
}

Group::Group(BuiltInGroups::BuiltInGroup g)
{
    mName = builtInGroupName(g);
    switch (g)
    {
    case BuiltInGroups::Default:
        addPermissions(Read | List);
        break;
    case BuiltInGroups::Users:
        addPermissions(Read | List | Execute | ReadAttributes | ReadExAttributes | ChangePassword);
        break;
    case BuiltInGroups::Operators:
        addPermissions(Read | List | Execute | ReadAttributes | ReadExAttributes
                       | Write | Rewrite | WriteAttributes | ChangeAttributes | ChangePassword);
        break;
    case BuiltInGroups::AdvancedOperators:
        addPermissions(Read | List | Execute | ReadAttributes | ReadExAttributes
                       | Write | Rewrite | WriteAttributes | ChangeAttributes
                       | WriteExAttributes | ChangeExAttributes | ChangePassword);
        break;
    case BuiltInGroups::Administrators:
        addPermissions(All);
        break;
    case BuiltInGroups::Root:
        addPermissions(AllRoot);
        break;
    }
}

QByteArray Group::toByteArray() const
{
    QByteArray ret;
    ret += GroupSignatures::s_signature;
    ret += GroupSignatures::s_group_open;
    ret += GroupSignatures::s_name_open;
    ret += mName.toUtf8();
    ret += GroupSignatures::s_name_close;
    ret += GroupSignatures::s_permissions_open;
    ret += QByteArray::number(quint32(mPermissions));
    ret += GroupSignatures::s_permissions_close;
    ret += GroupSignatures::s_users_open;
    foreach (const QString &u, mUsers)
    {
        ret += GroupSignatures::s_user_open;
        ret += u.toUtf8();
        ret += GroupSignatures::s_user_close;
    }
    ret += GroupSignatures::s_users_close;
    ret += GroupSignatures::s_group_close;
    return ret;
}

bool Group::fromByteArray(const QByteArray &array)
{
    clear();
    QRegExp reg(QString("^(%1)(%2)(%3)(.+)(%4)(%5)(\\d+)(%6)(%7)(.{0,})(%8)(%9)$")
                .arg(GroupSignatures::s_signature)
                .arg(GroupSignatures::s_group_open)
                .arg(GroupSignatures::s_name_open)
                .arg(GroupSignatures::s_name_close)
                .arg(GroupSignatures::s_permissions_open)
                .arg(GroupSignatures::s_permissions_close)
                .arg(GroupSignatures::s_users_open)
                .arg(GroupSignatures::s_users_close)
                .arg(GroupSignatures::s_group_close)
                );
    reg.setMinimal(true);
    if (!reg.exactMatch(QString(array)))
        return false;
    mName = reg.cap(4);
    mPermissions = Permissions(reg.cap(7).toUInt());
    QString allUsers = reg.cap(10);
    QRegExp regUsers(QString("(%1)(.+)(%2)")
                    .arg(GroupSignatures::s_user_open)
                    .arg(GroupSignatures::s_user_close)
                    );
    regUsers.setMinimal(true);
    while (regUsers.indexIn(allUsers) != -1)
    {
        mUsers.append(regUsers.cap(2));
        allUsers.remove(regUsers.cap(0));
    }
    return allUsers.isEmpty();
}

void Group::clear()
{
    mName.clear();
    mPermissions = 0;
    mUsers.clear();
}

void Group::setName(const QString &name)
{
    mName = name;
}

void Group::addUser(const QString &user)
{
    mUsers.append(user);
}

void Group::removeUser(const QString &user)
{
    mUsers.removeAll(user);
}

QString Group::name() const
{
    return mName;
}

Permissions Group::permissions() const
{
    return mPermissions;
}

QStringList Group::users() const
{
    return mUsers;
}

bool Group::operator ==(const Group &g) const
{
    return (mPermissions == g.mPermissions);
}

void Group::addPermission(UserSystem::Permission p)
{
    mPermissions |= p;
}

void Group::removePermission(UserSystem::Permission p)
{
    mPermissions &= ~p;
}

void Group::addPermissions(UserSystem::Permissions p)
{
    mPermissions |= p;
}

void Group::removePermissions(UserSystem::Permissions p)
{
    mPermissions &= ~p;
}

bool Group::contains(Permission p) const
{
    return mPermissions.testFlag(p);
}

bool Group::contains(Permissions p) const
{
    return ((mPermissions & p) == p);
}

bool Group::contains(Group g) const
{
    return ((mPermissions & g.mPermissions) == g.mPermissions);
}

GroupDefault::GroupDefault() :
    Group(BuiltInGroups::Default)
{
}

QString GroupDefault::name()
{
    return builtInGroupName(BuiltInGroups::Default);
}

GroupUsers::GroupUsers() :
    Group(BuiltInGroups::Users)
{
}

QString GroupUsers::name()
{
    return builtInGroupName(BuiltInGroups::Users);
}

GroupOperators::GroupOperators() :
    Group(BuiltInGroups::Operators)
{
}

QString GroupOperators::name()
{
    return builtInGroupName(BuiltInGroups::Operators);
}

GroupAdvancedOperators::GroupAdvancedOperators() :
    Group(BuiltInGroups::AdvancedOperators)
{
}

QString GroupAdvancedOperators::name()
{
    return builtInGroupName(BuiltInGroups::AdvancedOperators);
}

GroupAdministrators::GroupAdministrators() :
    Group(BuiltInGroups::Administrators)
{
}

QString GroupAdministrators::name()
{
    return builtInGroupName(BuiltInGroups::Administrators);
}

GroupRoot::GroupRoot() :
    Group(BuiltInGroups::Root)
{
}

QString GroupRoot::name()
{
    return builtInGroupName(BuiltInGroups::Root);
}

} // namespace
