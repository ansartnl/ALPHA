#include "FirBoundaryReplacer.h"
#include "ArincNativeStorage.h"
#include "Database.h"

#include <QtSql/QSqlError>
#include <QtCore/QVariant>


FirBoundaryReplacer::FirBoundaryReplacer(int indexFir, ArincNativeStorage *st)
    : mIndexFir(indexFir), mStorage(st)
{}

FirBoundaryReplacer::TFirMap FirBoundaryReplacer::loadExisted() const
{
    TFirMap firsExisted;

    if ( !mStorage || mStorage->firs.size() <= mIndexFir )
    {
        Q_ASSERT(false);
        return firsExisted;
    }

    const FirUir &fir = mStorage->firs[mIndexFir];

    QSharedPointer<db::CDBQuery> query = db::CreateQuery(QSqlDatabase::database());
    query->Prepare(
            "SELECT id, name "
            "FROM zone "
            "WHERE id_name = :1 and type = 1");
    query->BindValue(":1", fir.id);

    if (!query->exec())
    {
        qWarning() << __FILE__ << __LINE__ << tr(" Error: ") << query->lastError().text()
                   << tr(" Query: ") << fir.toString();
        return firsExisted;
    }

    while (query->next())
    {
        firsExisted[query->value(1).toString()] = query->value(0).toUInt();
    }

    return firsExisted;
}

bool FirBoundaryReplacer::replace(int id)
{
    if ( !mStorage || mStorage->firs.size() <= mIndexFir )
    {
        Q_ASSERT(false);
        return false;
    }

    FirUir &fir = mStorage->firs[mIndexFir];

    QSharedPointer<db::CDBQuery> query = db::CreateQuery(QSqlDatabase::database());
    if ( id == -1 )
    {
        query->Prepare(
                "SELECT zb.type, zb.sn, zb.longitude, zb.latitude, zb.arc_longitude, zb.arc_latitude, zb.arc_distance "
                "FROM zone_boundary zb, zone z "
                "WHERE zb.zone_id = z.id and z.id_name = :1 "
                "ORDER BY zb.sn");
        query->BindValue(":1", fir.id);
    }
    else
    {
        query->Prepare(
                "SELECT zb.type, zb.sn, zb.longitude, zb.latitude, zb.arc_longitude, zb.arc_latitude, zb.arc_distance "
                "FROM zone_boundary zb, zone z "
                "WHERE zb.zone_id = z.id and z.id = :1 "
                "ORDER BY zb.sn");
        query->BindValue(":1", id);
    }

    if (!query->exec())
    {
        qWarning() << __FILE__ << __LINE__ << tr(" Error: ") << query->lastError().text()
                   << tr(" Query: ") << fir.toString();
        return false;
    }

    // Replace FIR points in the storage to FIR points from the database.
    fir.boundary.clear();
    while (query->next())
    {
        BoundaryPointFirUir point;
        point.type = (BoundaryPoint::Type) query->value(0).toUInt();
        point.sn = query->value(1).toInt();
        if (BoundaryPoint::ArcByEdge == point.type // A
            || BoundaryPoint::GreatCircle == point.type // G
            || BoundaryPoint::RhumbLine == point.type // H
            || BoundaryPoint::CounterClockwiseArc == point.type // L
            || BoundaryPoint::ClockwiseArc == point.type) // R
        {
            point.coord.set(query->value(2).toDouble(), query->value(3).toDouble());
        }
        if (BoundaryPoint::ArcByEdge == point.type // A
            || BoundaryPoint::Circle == point.type // C
            || BoundaryPoint::CounterClockwiseArc == point.type // L
            || BoundaryPoint::ClockwiseArc == point.type) // R
        {
            point.arcCoord.set(query->value(4).toDouble(), query->value(5).toDouble());
            point.arcDistance = query->value(6).toDouble();
        }
        fir.boundary.push_back(point);
    }

    return true;
}
