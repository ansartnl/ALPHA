#ifndef GROUPS_H
#define GROUPS_H

#include "usGroup.h"

#include <QtCore/QMap>
#include <QtCore/QSharedPointer>

namespace UserSystem
{

typedef QSharedPointer < Group > PGroup;
typedef QMap < QString, PGroup > GroupsMap;

class Groups : public GroupsMap
{
public:
    Groups();
    Groups(const Groups &g);
    ~Groups();

    QByteArray toByteArray() const;
    bool fromByteArray(const QByteArray &array);

    void add(Group *g);

private:
    void insertGroup(const PGroup &g);
};

} // namespace

#endif // GROUPS_H
