#ifndef USER_H
#define USER_H

#include "usGroups.h"

#include <QtCore/QByteArray>
#include <QtCore/QStringList>

namespace UserSystem
{

class User
{
public:
    User();
    User(const User &u);

    QByteArray toByteArray() const;
    bool fromByteArray(const QByteArray &array);

    void clear();

    void setName(const QString &name);
    void setPassword(const QString &password);
    void setDescription(const QString &description);
    void setAllGroups(const Groups &allGroups);

    void setGroup(const QString &g);
    void addToGroup(const QString &g);
    void removeFromGroup(const QString &g);

    QString name() const;
    QString passHash() const;
    QString description() const;
    QStringList groups() const;

    bool isInGroup(const QString &g) const;
    bool hasPermission(const Permission &p) const;
    bool hasPermissions(const Permissions &p) const;

    User & operator =(const User &u);

protected:
    QString mName;
    QString mPassHash;
    QString mDescription;
    QStringList mGroups;
    Groups mAllGroups;
};

} // namespace

#endif // USER_H
