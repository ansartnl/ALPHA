#ifndef DAO_DATABASE_H
#define DAO_DATABASE_H

#include <QtCore/QList>

class QSqlDatabase;
class QUuid;

class CRestrictionUser;

//! Main class to work with database
namespace Dao
{
    void LoadUserRestrictions(const QSqlDatabase& db, int zoneId, QList<CRestrictionUser> &list);
    int SaveUserRestriction(const QSqlDatabase& db, int zoneId, CRestrictionUser &r, bool bDelPoints);
    void UpdateUserRestriction(const QSqlDatabase& db, CRestrictionUser &r, bool bDelPoints);
    void DeleteUserRestriction(const QSqlDatabase& db, const CRestrictionUser &r);
    int GetUserRestrictionIDbyGUID(const QSqlDatabase& db, const QUuid& uid);

    int GetZoneIdByIdName(const QSqlDatabase& db, const QString& sID);
}

#endif //DAO_DATABASE_H
