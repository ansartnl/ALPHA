#include "usUsers.h"

#include "usGroup.h"

#include <QtCore/QRegExp>

namespace UserSystem
{

Users::Users() :
    UsersMap()
{
}

Users::~Users()
{
}

QByteArray Users::toByteArray() const
{
    QByteArray ret;
    ret += UsersSignatures::s_signature;
    ret += UsersSignatures::s_users_open;
    foreach (const PUser &u, *this)
    {
        ret += UsersSignatures::s_user_open;
        ret += u->toByteArray();
        ret += UsersSignatures::s_user_close;
    }
    ret += UsersSignatures::s_users_close;
    return ret;
}

bool Users::fromByteArray(const QByteArray &array)
{
    clear();
    QRegExp reg(QString("^(%1)(%2)(.+)(%3)$")
                .arg(UsersSignatures::s_signature)
                .arg(UsersSignatures::s_users_open)
                .arg(UsersSignatures::s_users_close)
                );
    reg.setMinimal(true);
    if (!reg.exactMatch(QString(array)))
        return false;
    QString allUsers = reg.cap(3);
    QRegExp regUsers(QString("(%1)(.+)(%2)")
                     .arg(UsersSignatures::s_user_open)
                     .arg(UsersSignatures::s_user_close)
                     );
    regUsers.setMinimal(true);
    while (regUsers.indexIn(allUsers) != -1)
    {
        PUser u(new User);
        if (!u->fromByteArray(regUsers.cap(2).toUtf8()))
            return false;
        insertUser(u);
        allUsers.remove(regUsers.cap(0));
    }
    return allUsers.isEmpty();
}

void Users::add(User *u)
{
    insertUser(PUser(u));
}

void Users::setAllGroups(const Groups &groups)
{
    foreach (const PUser &u, *this)
    {
        u->setAllGroups(groups);
    }
}

void Users::insertUser(const PUser &u)
{
    insert(u->name(), u);
}

} // namespace
