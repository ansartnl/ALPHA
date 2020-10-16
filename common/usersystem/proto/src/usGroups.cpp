#include "usGroups.h"

#include <QtCore/QRegExp>

namespace UserSystem
{

Groups::Groups() :
    GroupsMap()
{
}

Groups::Groups(const Groups &g) :
    GroupsMap(g)
{
}

Groups::~Groups()
{
}

QByteArray Groups::toByteArray() const
{
    QByteArray ret;
    ret += GroupsSignatures::s_signature;
    ret += GroupsSignatures::s_groups_open;
    foreach (const PGroup &g, *this)
    {
        ret += GroupsSignatures::s_group_open;
        ret += g->toByteArray();
        ret += GroupsSignatures::s_group_close;
    }
    ret += GroupsSignatures::s_groups_close;
    return ret;
}

bool Groups::fromByteArray(const QByteArray &array)
{
    clear();
    QRegExp reg(QString("^(%1)(%2)(.+)(%3)$")
                .arg(GroupsSignatures::s_signature)
                .arg(GroupsSignatures::s_groups_open)
                .arg(GroupsSignatures::s_groups_close)
                );
    reg.setMinimal(true);
    if (!reg.exactMatch(QString(array)))
        return false;
    QString allGroups = reg.cap(3);
    QRegExp regGroups(QString("(%1)(.+)(%2)")
                    .arg(GroupsSignatures::s_group_open)
                    .arg(GroupsSignatures::s_group_close)
                    );
    regGroups.setMinimal(true);
    while (regGroups.indexIn(allGroups) != -1)
    {
        PGroup g(new Group);
        if (!g->fromByteArray(regGroups.cap(2).toUtf8()))
            return false;
        insertGroup(g);
        allGroups.remove(regGroups.cap(0));
    }
    return allGroups.isEmpty();
}

void Groups::add(Group *g)
{
    insertGroup(PGroup(g));
}

void Groups::insertGroup(const PGroup &g)
{
    insert(g->name(), g);
}

} // namespace
