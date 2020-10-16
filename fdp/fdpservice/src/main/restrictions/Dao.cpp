#include "Dao.h"
#include "BoundaryPoint.h"
#include "Restriction.h"

#include "qtautotransaction.h"
#include "QOciQuery.h"

#include <QtDebug>
#include <QtSql/QSqlError>

namespace Dao
{
    int SaveUserRestriction(const QSqlDatabase& db, int zoneId, CRestrictionUser &r, bool bDelPoints)
    {
        int restId = 0;

        if(!r.GetGUID().isNull())
        {
            restId = GetUserRestrictionIDbyGUID(db, r.GetGUID());
            if(restId)
            {
                r.SetID(restId);
                for ( CRestrictionUser::TPointList::iterator itc = r.GetPoints().begin();
                      itc != r.GetPoints().end(); ++itc ) (*itc)->SetRestrictID(restId);
                UpdateUserRestriction(db, r, bDelPoints);
                return restId;
            }
        }
        else
        {
            r.SetGUID(QUuid::createUuid());
        }

        QtAutoTransaction transaction(db);

        QOciQuery query(db);
        // Add new record into restriction_user
        query.prepare(
                "INSERT INTO restriction_user (name, h_min, h_max, time_start, time_end, zone_id, type, guid) VALUES(:1, :2, :3, :4, :5, :6, :7, :8)",
                "id");
        query.bindValue(":1", r.GetName());
        query.bindValue(":2", r.GetHMin());
        query.bindValue(":3", r.GetHMax());
        query.bindValue(":4", r.GetTimeBegin());
        query.bindValue(":5", r.GetTimeEnd());
        query.bindValue(":6", zoneId);
        query.bindValue(":7", r.GetType());
        query.bindValue(":8", r.GetGUID().toString());

        if(!query.exec())
        {
            qDebug() << query.lastError().text();
            return 0;
        }

        restId = query.GetLastInsertID().toUInt();
        r.SetID(restId);

        QOciQuery queryPoints(db);
        // Add new records into restriction_point_user
        if(bDelPoints)
        {
            queryPoints.prepare(
                    "DELETE FROM restriction_point_user WHERE restriction_id = :1");
            queryPoints.bindValue(":1", restId);
            if(!queryPoints.exec())
            {
                qDebug() << queryPoints.lastError().text();
                return 0;
            }
        }

        queryPoints.prepare(
                "INSERT INTO restriction_point_user (sn, type, latitude, longitude, arc_latitude, arc_longitude, arc_distance, restriction_id) VALUES(:1, :2, :3, :4, :5, :6, :7, :8)",
                "id");

        for ( CRestrictionUser::TPointList::iterator itc = r.GetPoints().begin();
              itc != r.GetPoints().end(); ++itc )
        {
            CRestrictionUser::TPoint &point = *itc;

            queryPoints.bindValue(":1", point->GetSN());
            queryPoints.bindValue(":2", (int)point->GetType());
            queryPoints.bindValue(":3", point->GetCoord().GetLatitude().GetAngle());
            queryPoints.bindValue(":4", point->GetCoord().GetLongitude().GetAngle());
            queryPoints.bindValue(":5", point->GetArcCoord().GetLatitude().GetAngle());
            queryPoints.bindValue(":6", point->GetArcCoord().GetLongitude().GetAngle());
            queryPoints.bindValue(":7", point->GetArcDistance());
            queryPoints.bindValue(":8", restId);

            if(!queryPoints.exec())
            {
                qDebug() << queryPoints.lastError().text();
                return 0;
            }

            point->SetRestrictID(restId);
            point->SetID(query.GetLastInsertID().toUInt());
        }

        transaction.commit();

        return restId;
    }

    void UpdateUserRestriction(const QSqlDatabase& db, CRestrictionUser &r, bool bDelPoints)
    {
        QtAutoTransaction transaction(db);

        QOciQuery query(db);
        query.prepare("UPDATE restriction_user SET "
                      "name = :2, type = :3, h_min = :4, h_max = :5, "
                      "time_start = :6, time_end = :7 "
                      "WHERE id = :1");
        query.bindValue(":1", r.GetID());
        query.bindValue(":2", r.GetName());
        query.bindValue(":3", r.GetType());
        query.bindValue(":4", r.GetHMin());
        query.bindValue(":5", r.GetHMax());
        query.bindValue(":6", r.GetTimeBegin());
        query.bindValue(":7", r.GetTimeEnd());

        if(!query.exec())
        {
            qDebug() << query.lastError().text();
            return;
        }

        QOciQuery queryPoints(db);
        // Add new records into restriction_point_user
        if(bDelPoints)
        {
            queryPoints.prepare(
                    "DELETE FROM restriction_point_user WHERE restriction_id = :1");
            queryPoints.bindValue(":1", r.GetID());
            if(!queryPoints.exec())
            {
                qDebug() << queryPoints.lastError().text();
                return;
            }
        }

        queryPoints.prepare(
                "INSERT INTO restriction_point_user (sn, type, latitude, longitude, arc_latitude, arc_longitude, arc_distance, restriction_id) VALUES(:1, :2, :3, :4, :5, :6, :7, :8)",
                "id");

        for ( CRestrictionUser::TPointList::iterator itc = r.GetPoints().begin();
              itc != r.GetPoints().end(); ++itc )
        {
            CRestrictionUser::TPoint &point = *itc;

            queryPoints.bindValue(":1", point->GetSN());
            queryPoints.bindValue(":2", (int)point->GetType());
            queryPoints.bindValue(":3", point->GetCoord().GetLatitude().GetAngle());
            queryPoints.bindValue(":4", point->GetCoord().GetLongitude().GetAngle());
            queryPoints.bindValue(":5", point->GetArcCoord().GetLatitude().GetAngle());
            queryPoints.bindValue(":6", point->GetArcCoord().GetLongitude().GetAngle());
            queryPoints.bindValue(":7", point->GetArcDistance());
            queryPoints.bindValue(":8", r.GetID());

            if(!queryPoints.exec())
            {
                qDebug() << queryPoints.lastError().text();
                return;
            }

            point->SetRestrictID(r.GetID());
            point->SetID(query.GetLastInsertID().toUInt());
        }

        transaction.commit();
    }

    void DeleteUserRestriction(const QSqlDatabase& db, const CRestrictionUser &r)
    {
        QtAutoTransaction transaction(db);
        QOciQuery query(db);
        if(!r.GetGUID().isNull())
        {
            query.prepare("DELETE FROM restriction_user WHERE guid = :1");
            query.bindValue(":1", r.GetGUID().toString());
        }
        else
        {
            query.prepare("DELETE FROM restriction_user WHERE id = :1");
            query.bindValue(":1", r.GetID());
        }
        if(!query.exec())
        {
            qDebug() << query.lastError().text();
            return;
        }
        transaction.commit();
    }

    int GetUserRestrictionIDbyGUID(const QSqlDatabase& db, const QUuid& uid)
    {
        int qID(0);
        if(!uid.isNull())
        {
            QOciQuery query(db);
            query.prepare("SELECT id FROM restriction_user WHERE guid = :1");
            query.bindValue(":1", uid.toString());

            if (query.exec() && query.next())
            {
                qID = query.value(0).toInt();
            }
        }
        return qID;
    }

    int GetZoneIdByIdName(const QSqlDatabase& db, const QString& sID)
    {
        int qID(0);
        QOciQuery query(db);
        query.prepare("SELECT id FROM zone WHERE id_name = :1");
        query.bindValue(":1", sID);
        if(!query.exec())
        {
            qDebug() << query.lastError().text();
            return 0;
        }
        if (query.next())
        {
            qID = query.value(0).toInt();
        }
        return qID;
    }

    void LoadUserRestrictions(const QSqlDatabase& db, int zoneId, QList<CRestrictionUser> &list)
    {
        QOciQuery query(db);
        query.prepare(
                "SELECT id, name, type, h_min, h_max, time_start, time_end, guid FROM restriction_user WHERE zone_id = :1");
        query.bindValue(":1", zoneId);
        QOciQuery queryPoints(db);
        queryPoints.prepare(
                "SELECT id, sn, type, latitude, longitude, arc_latitude, arc_longitude, arc_distance "
                "FROM restriction_point_user WHERE restriction_id = :1 "
                "ORDER BY sn");

        if(!query.exec())
        {
            qDebug() << query.lastError().text();
            return;
        }
        while (query.next())
        {
            CRestrictionUser restriction;
            restriction.SetID(query.value(0).toUInt());
            restriction.SetName(query.value(1).toString());
            restriction.SetType((CRestrictionUser::Type) query.value(2).toUInt());
            restriction.SetHMin(query.value(3).toDouble());
            restriction.SetHMax(query.value(4).toDouble());
            restriction.SetTimeBegin(query.value(5).toDateTime());
            restriction.SetTimeEnd(query.value(6).toDateTime());
            restriction.SetGUID(QUuid(query.value(7).toString()));

            restriction.SetZoneID(zoneId);

            queryPoints.bindValue(":1", restriction.GetID());
            if(!queryPoints.exec())
            {
                qDebug() << queryPoints.lastError().text();
                return;
            }
            while (queryPoints.next())
            {
                QSharedPointer < CRestrictPoint > pRestrPoint(new CRestrictPoint());
                pRestrPoint->SetID(queryPoints.value(0).toUInt());
                pRestrPoint->SetSN(queryPoints.value(1).toInt());
                pRestrPoint->SetType((CRestrictPoint::Type) queryPoints.value(2).toInt());
                pRestrPoint->SetCoord(CCoord(queryPoints.value(4).toDouble(),
                                             queryPoints.value(3).toDouble()));
                pRestrPoint->SetArcCoord(CCoord(queryPoints.value(6).toDouble(),
                                                queryPoints.value(5).toDouble()));
                pRestrPoint->SetArcDistance(queryPoints.value(7).toDouble());
                pRestrPoint->SetRestrictID(restriction.GetID());
                restriction.AddPoint(pRestrPoint);
            }
            list.append(restriction);
        }
    }
}
