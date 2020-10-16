#ifndef USUSERS_H
#define USUSERS_H

#include "usUser.h"

#include <QtCore/QMap>
#include <QtCore/QSharedPointer>

namespace UserSystem
{

typedef QSharedPointer < User > PUser;
typedef QMap < QString, PUser > UsersMap;

class Users : public UsersMap
{
public:
    Users();
    ~Users();

    QByteArray toByteArray() const;
    bool fromByteArray(const QByteArray &array);

    void add(User *u);
    void setAllGroups(const Groups &groups);

private:
    void insertUser(const PUser &u);
};

} // namespace

#endif // USUSERS_H
