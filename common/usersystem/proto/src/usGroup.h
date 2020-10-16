#ifndef GROUP_H
#define GROUP_H

#include <QtCore/QHash>
#include <QtCore/QStringList>

#include "usDeclare.h"

namespace UserSystem
{

namespace BuiltInGroups
{

enum BuiltInGroup {
    Default,
    Users,
    Operators,
    AdvancedOperators,
    Administrators,
    Root
};

}

inline QString builtInGroupName(BuiltInGroups::BuiltInGroup g)
{
    switch (g)
    {
    case BuiltInGroups::Default:
        return "Default";
    case BuiltInGroups::Users:
        return "Users";
    case BuiltInGroups::Operators:
        return "Operators";
    case BuiltInGroups::AdvancedOperators:
        return "Advanced operators";
    case BuiltInGroups::Administrators:
        return "Administrators";
    case BuiltInGroups::Root:
        return "Root";
    }
    return QString();
}

inline QString toDescription(const QString &group)
{
    if (group == builtInGroupName(BuiltInGroups::Default))
        return QObject::tr("Default");
    if (group == builtInGroupName(BuiltInGroups::Users))
        return QObject::tr("Users");
    if (group == builtInGroupName(BuiltInGroups::Operators))
        return QObject::tr("Operators");
    if (group == builtInGroupName(BuiltInGroups::AdvancedOperators))
        return QObject::tr("Advanced operators");
    if (group == builtInGroupName(BuiltInGroups::Administrators))
        return QObject::tr("Administrators");
    if (group == builtInGroupName(BuiltInGroups::Root))
        return QObject::tr("Root");
    return QString();
}

inline QString fromDescription(const QString &group)
{
    if (group == QObject::tr("Default"))
        return builtInGroupName(BuiltInGroups::Default);
    if (group == QObject::tr("Users"))
        return builtInGroupName(BuiltInGroups::Users);
    if (group == QObject::tr("Operators"))
        return builtInGroupName(BuiltInGroups::Operators);
    if (group == QObject::tr("Advanced operators"))
        return builtInGroupName(BuiltInGroups::AdvancedOperators);
    if (group == QObject::tr("Administrators"))
        return builtInGroupName(BuiltInGroups::Administrators);
    if (group == QObject::tr("Root"))
        return builtInGroupName(BuiltInGroups::Root);
    return QString();
}

class Group
{
public:
    Group();
    Group(QString name);
    Group(QString name, Permission p);

    Group(BuiltInGroups::BuiltInGroup g);

    QByteArray toByteArray() const;
    bool fromByteArray(const QByteArray &array);

    void clear();

    void setName(const QString &name);
    void addUser(const QString &user);
    void removeUser(const QString &user);

    QString name() const;
    Permissions permissions() const;
    QStringList users() const;

    bool operator ==(const Group &g) const;

    void addPermission(Permission p);
    void removePermission(Permission p);

    void addPermissions(Permissions p);
    void removePermissions(Permissions p);

    bool contains(Permission p) const;
    bool contains(Permissions p) const;
    bool contains(Group g) const;

protected:
    QString mName;
    Permissions mPermissions;
    QStringList mUsers;
};

inline uint qHash(const Group &g)
{
    return qHash(g.toByteArray());
}

class GroupDefault : public Group
{
public:
    GroupDefault();
    static QString name();
};

class GroupUsers : public Group
{
public:
    GroupUsers();
    static QString name();
};

class GroupOperators : public Group
{
public:
    GroupOperators();
    static QString name();
};

class GroupAdvancedOperators : public Group
{
public:
    GroupAdvancedOperators();
    static QString name();
};

class GroupAdministrators : public Group
{
public:
    GroupAdministrators();
    static QString name();
};

class GroupRoot : public Group
{
public:
    GroupRoot();
    static QString name();
};


} // namespace

#endif // GROUP_H
