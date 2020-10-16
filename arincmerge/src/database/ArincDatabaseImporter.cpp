#include "ArincDatabaseImporter.h"
#include "qtautotransaction.h"
#include "Database.h"
#include "Vincenty.h"
#include "OldMasterSupport.h"
#include <QtSql/QSqlError>
#include <QtCore/QVariant>
#include <QtCore/QLineF>
#include <QtGui/QPainterPath>
#include <math.h>

ArincDatabaseImporter::ArincDatabaseImporter(QObject *parent)
    : QObject(parent)
    , pos(0), bCancel(false)
    , bCanEmptyRW(true), sEmptyRW("<EMPTY_RW>")
    , bUniteSSA(true)
{}

void ArincDatabaseImporter::cancel()
{
    bCancel = true;
}

void ArincDatabaseImporter::movingToNextAndCheckCancelation()
{
    if (bCancel)
        throw Cancelation();
    emit posChanged(++pos);
}

int ArincDatabaseImporter::toOldBoundaryPointType(const BoundaryPoint::Type &type)
{
    if (BoundaryPoint::Circle == type || BoundaryPoint::CounterClockwiseArc == type || BoundaryPoint::ClockwiseArc == type)
        return int(type);
    return int(BoundaryPoint::GreatCircle);
}

int ArincDatabaseImporter::toOldPointType(char sectionCode, char subsectionCode)
{
    if (sectionCode == 'D' && subsectionCode == ' ')
        return 1;
    else if (sectionCode == 'D' && subsectionCode == 'B')
        return 2;
    else if (sectionCode == 'E' && subsectionCode == 'A')
        return 3;
    else if (sectionCode == 'P' && subsectionCode == 'C')
        return 6;
    else if (sectionCode == 'P' && subsectionCode == 'N')
        return 7;
    return 0;
}

void ArincDatabaseImporter::importFir(const ArincNativeStorage *st, int firPos, const QSqlDatabase &database)
{
    db = database;
    pos = 0;
    bCancel = false;

    try
    {
        if (!st || st->firs.size() < firPos)
            return;

        const FirUir &fir = st->firs[firPos];

        qDebug() << "Importing FIR" << fir.id << " - " << fir.name;

        if (fir.boundary.size() <= 2)
            return;

        estimate(st);

        emit statusChanged(tr("Opening transaction..."));
        QtAutoTransaction transaction(db);

        emit statusChanged(tr("Removing old FIR..."));
        QSharedPointer<db::CDBQuery> deleteFirQuery = db::CreateQuery(db);
        deleteFirQuery->Prepare("delete from zone where id_name = :id_name and name = :name and type = 1");
        deleteFirQuery->BindValue(":id_name", fir.id);
        deleteFirQuery->BindValue(":name", fir.name);
        if (!deleteFirQuery->exec())
        {
            qWarning() << __FILE__ << __LINE__ << deleteFirQuery->lastError().text() << fir.toString();
            return;
        }

        emit statusChanged(tr("Inserting FIR..."));
        QSharedPointer<db::CDBQuery> insertFirQuery = db::CreateQuery(db);
        insertFirQuery->Prepare("INSERT INTO zone(name, type, id_name, low_limit, up_limit) VALUES(:name, :type, :id_name, :low_limit, :up_limit)", "id");
        insertFirQuery->BindValue(":name", fir.name);
        insertFirQuery->BindValue(":type", 1);
        insertFirQuery->BindValue(":id_name", fir.id);
        insertFirQuery->BindValue(":low_limit", 0);
        insertFirQuery->BindValue(":up_limit", fir.firUpperLimit.isValid() ? fir.firUpperLimit.value : 0);
        if (!insertFirQuery->exec())
        {
            qWarning() << __FILE__ << __LINE__ << insertFirQuery->lastError().text() << fir.toString();
            return;
        }

        int firId = insertFirQuery->GetLastInsertID().toInt();

        // FIR boundary points sn podpora
        if(fir.boundary.size() && !fir.boundary.first().sn)
        {
            FirUir* temp_f(const_cast<FirUir*>(&fir));
            int sn = 10;
            for(QList<BoundaryPointFirUir>::iterator itPoint = temp_f->boundary.begin(); itPoint != temp_f->boundary.end(); ++itPoint)
            {
                itPoint->sn = sn;
                sn += 10;
            }
        }
        /////////////////////////////////////////////////

        QSharedPointer<db::CDBQuery> insertFirBundary = db::CreateQuery(db);
        insertFirBundary->Prepare("INSERT INTO zone_boundary(type, sn, latitude, longitude, arc_latitude, arc_longitude, arc_distance, zone_id) VALUES(:type, :sn, :latitude, :longitude, :arc_latitude, :arc_longitude, :arc_distance, :zone_id)");
        foreach (const BoundaryPointFirUir &p, fir.boundary)
        {
            insertFirBundary->BindValue(":type", toOldBoundaryPointType(p.type));
            insertFirBundary->BindValue(":sn", p.sn);
            insertFirBundary->BindValue(":latitude", p.coord.latitude());
            insertFirBundary->BindValue(":longitude", p.coord.longitude());
            insertFirBundary->BindValue(":arc_latitude", p.arcCoord.latitude());
            insertFirBundary->BindValue(":arc_longitude", p.arcCoord.longitude());
            insertFirBundary->BindValue(":arc_distance", p.arcDistance);
            insertFirBundary->BindValue(":zone_id", firId);
            if (!insertFirBundary->exec())
            {
                qWarning() << __FILE__ << __LINE__ << insertFirBundary->lastError().text() << fir.toString();
                continue;
            }
        }

        QPolygonF boundary;
        GeoPoint centerGeo = BoundaryPoint::Circle == fir.boundary[0].type ? fir.boundary[0].arcCoord : fir.boundary[0].coord;
        foreach (const BoundaryPointFirUir &p, fir.boundary)
        {
            if (BoundaryPoint::Circle == p.type)
                boundary.push_back(Vincenty::toCartesian(p.arcCoord, centerGeo));
            else
                boundary.push_back(Vincenty::toCartesian(p.coord, centerGeo));
        }

        // NULL sector id_name podpora
        {
            ArincNativeStorage* tempSt(const_cast<ArincNativeStorage*>(st));
            for(QList<ControlledAirspace>::iterator itSector = tempSt->sectors.begin(); itSector != tempSt->sectors.end(); ++itSector)
            {
                if(itSector->center.isEmpty())
                    itSector->center = fir.id;
            }
        }
        /////////////////////////////////////////////////

        insertControlledAirspace(boundary, firId, centerGeo, st->sectors);
        insertVhfNavaid(boundary, firId, centerGeo, st->vordme);
        insertNdbNavaid(boundary, firId, centerGeo, st->ndb);
        insertEnrouteWaypoint(boundary, firId, centerGeo, st->waypoints);
        insertRoute(firId, st->routes);
        insertAirports(boundary, firId, centerGeo, st->airports);
        insertTerminalWaypoint(boundary, firId, centerGeo, st->terminalWaypoints);
        insertNdbTerminal(boundary, firId, centerGeo, st->ndbterminals);
           insertRunway(firId, st->runways);
        insertSid(firId, st->sids);
        insertStar(firId, st->stars);
        insertApproach(firId, st->approachs);
        insertHolding(firId, centerGeo, st->holdings);
        insertRestriction(boundary, firId, centerGeo, st->restrictions);

        emit statusChanged(tr("Closing transaction..."));
        transaction.commit();
    }
    catch (const Cancelation &)
    {}
    db = QSqlDatabase();
}

void ArincDatabaseImporter::estimate(const ArincNativeStorage *st)
{
    emit statusChanged(tr("Estimating..."));
    int result(0);
    result += st->sectors.size();
    result += st->restrictions.size();
    result += st->vordme.size();
    result += st->ndb.size();
    result += st->waypoints.size();
    result += st->routes.size();
    result += st->holdings.size();
    result += st->airports.size();
    result += st->terminalWaypoints.size();
    result += st->sids.size();
    result += st->stars.size();
    result += st->approachs.size();
    result += st->runways.size();
    result += st->ndbterminals.size();
    emit rangeEvaluated(result);
}

void ArincDatabaseImporter::insertControlledAirspace(const QPolygonF &firBoundary, int firDbId, const GeoPoint &centerGeo, const QList<ControlledAirspace> &sectors)
{
    emit statusChanged(tr("Inserting sectors..."));
    QSharedPointer<db::CDBQuery> insertSectors = db::CreateQuery(db);
    insertSectors->Prepare("INSERT INTO zone(name, type, id_name, parent_id, low_limit, up_limit) VALUES(:name, :type, :id_name, :parent_id, :low_limit, :up_limit)", "id");
    QSharedPointer<db::CDBQuery> insertBoundary = db::CreateQuery(db);
    insertBoundary->Prepare("INSERT INTO zone_boundary(type, sn, latitude, longitude, arc_latitude, arc_longitude, arc_distance, zone_id) VALUES(:type, :sn, :latitude, :longitude, :arc_latitude, :arc_longitude, :arc_distance, :zone_id)");
    foreach (const ControlledAirspace &sector, sectors)
    {
        movingToNextAndCheckCancelation();
        bool isBelong = true;
        foreach (const BoundaryPoint &p, sector.boundary)
        {
            QPointF bp;
            if (BoundaryPoint::Circle == p.type)
                bp = Vincenty::toCartesian(p.arcCoord, centerGeo);
            else
                bp = Vincenty::toCartesian(p.coord, centerGeo);
            if (!isIncluded(firBoundary, bp))
            {
                isBelong = false;
                break;
            }
        }

        if (!isBelong)
            continue;

        if (sector.multipleCode >= 'A' && sector.multipleCode <= 'Z')
            insertSectors->BindValue(":name", QString("%1 (%2)").arg(sector.name).arg(QChar(sector.multipleCode)));
        else
            insertSectors->BindValue(":name", sector.name);
        insertSectors->BindValue(":type", 2);
        insertSectors->BindValue(":id_name", sector.center);
        insertSectors->BindValue(":parent_id", firDbId);
        insertSectors->BindValue(":low_limit", sector.lowerLimit.isValid() ? sector.lowerLimit.value : 0);
        insertSectors->BindValue(":up_limit", sector.upperLimit.isValid() ? sector.upperLimit.value : 0);
        if (!insertSectors->exec())
        {
            qWarning() << __FILE__ << __LINE__ << insertSectors->lastError().text() << sector.toString();
            continue;
        }

        int idSector = insertSectors->GetLastInsertID().toInt();

        // sector boundary points sn podpora
        if(sector.boundary.size() && !sector.boundary.first().sn)
        {
            ControlledAirspace* temp_f(const_cast<ControlledAirspace*>(&sector));
            int sn = 10;
            for(QList<BoundaryPoint>::iterator itPoint = temp_f->boundary.begin(); itPoint != temp_f->boundary.end(); ++itPoint)
            {
                itPoint->sn = sn;
                sn += 10;
            }
        }
        /////////////////////////////////////////////////

        foreach (const BoundaryPoint &p, sector.boundary)
        {
            insertBoundary->BindValue(":type", toOldBoundaryPointType(p.type));
            insertBoundary->BindValue(":sn", p.sn);
            insertBoundary->BindValue(":latitude", p.coord.latitude());
            insertBoundary->BindValue(":longitude", p.coord.longitude());
            insertBoundary->BindValue(":arc_latitude", p.arcCoord.latitude());
            insertBoundary->BindValue(":arc_longitude", p.arcCoord.longitude());
            insertBoundary->BindValue(":arc_distance", p.arcDistance);
            insertBoundary->BindValue(":zone_id", idSector);
            if (!insertBoundary->exec())
            {
                qWarning() << __FILE__ << __LINE__ << insertBoundary->lastError().text() << sector.toString();
                continue;
            }
        }
    }
}

void ArincDatabaseImporter::insertVhfNavaid(const QPolygonF &firBoundary, int firDbId, const GeoPoint &centerGeo, const QList<VhfNavaid> &data)
{
    emit statusChanged(tr("Inserting VHF navaid..."));
    QSharedPointer<db::CDBQuery> pointInsertRequest = db::CreateQuery(db);
    pointInsertRequest->Prepare("INSERT INTO point(type, name, latitude, longitude, icao, airport)"
                                 " VALUES(:type, :name, :latitude, :longitude, :icao, :airport)",
                                 "id");
    QSharedPointer<db::CDBQuery> pointSelectRequest = db::CreateQuery(db);
    pointSelectRequest->Prepare("SELECT id, latitude, longitude FROM point "
                                 "WHERE type = :type AND name = :name AND "
                                 "(icao IS NOT NULL AND icao = :icao OR icao IS NULL) AND "
                                 "(airport IS NOT NULL AND airport = :airport OR airport IS NULL)");
    QSharedPointer<db::CDBQuery> pointUpdateRequest = db::CreateQuery(db);
    pointUpdateRequest->Prepare("UPDATE point SET latitude = :latitude, longitude = :longitude WHERE id = :id");
    QSharedPointer<db::CDBQuery> insertZonePoint = db::CreateQuery(db);
    insertZonePoint->Prepare("INSERT INTO zone_point(zone_id, point_id) VALUES(:zone_id, :point_id)");
    foreach (const VhfNavaid &f, data)
    {
        movingToNextAndCheckCancelation();
        const GeoPoint &coord = 'V' == f.navaidClass[0] ? f.vorCoord : f.dmeCoord;

        if (!isIncluded(firBoundary, Vincenty::toCartesian(coord, centerGeo)))
            continue;

        pointSelectRequest->BindValue(":type", 1);
        pointSelectRequest->BindValue(":name", f.vorId);
        pointSelectRequest->BindValue(":icao", f.icaoCode2);
        pointSelectRequest->BindValue(":airport", f.airportIcaoId);
        if (!pointSelectRequest->exec())
        {
            qWarning() << __FILE__ << __LINE__ << pointSelectRequest->lastError().text() << f.toString();
            continue;
        }

        quint32 uiPointID;
        if (pointSelectRequest->next())
        {
            uiPointID = pointSelectRequest->value(0).toUInt();
            double d1 = pointSelectRequest->value(1).toDouble() - coord.latitude();
            double d2 = pointSelectRequest->value(2).toDouble() - coord.longitude();
            if (qFuzzyIsNull(d1) || qFuzzyIsNull(d2))
            {
                pointUpdateRequest->BindValue(":latitude", coord.latitude());
                pointUpdateRequest->BindValue(":longitude", coord.longitude());
                pointUpdateRequest->BindValue(":id", uiPointID);
                if (!pointUpdateRequest->exec())
                    qWarning() << __FILE__ << __LINE__ << pointSelectRequest->lastError().text() << f.toString();
            }
        }
        else
        {
            pointInsertRequest->BindValue(":type", 1);
            pointInsertRequest->BindValue(":name", f.vorId);
            pointInsertRequest->BindValue(":latitude", coord.latitude());
            pointInsertRequest->BindValue(":longitude", coord.longitude());
            pointInsertRequest->BindValue(":icao", f.icaoCode2);
            pointInsertRequest->BindValue(":airport", f.airportIcaoId);
            if (!pointInsertRequest->exec())
            {
                qWarning() << __FILE__ << __LINE__ << pointInsertRequest->lastError().text() << f.toString();
                continue;
            }
            uiPointID = pointInsertRequest->GetLastInsertID().toInt();
        }

        if (!checkZonePoint(firDbId, uiPointID))
        {
            insertZonePoint->BindValue(":zone_id", firDbId);
            insertZonePoint->BindValue(":point_id", uiPointID);
            if (!insertZonePoint->exec())
            {
                qWarning() << __FILE__ << __LINE__ << insertZonePoint->lastError().text() << f.toString();
                continue;
            }
        }
    }
}

void ArincDatabaseImporter::insertNdbNavaid(const QPolygonF &firBoundary, int firDbId, const GeoPoint &centerGeo, const QList<NdbNavaid> &data)
{
    emit statusChanged(tr("Inserting NDB navaid..."));
    QSharedPointer<db::CDBQuery> pointInsertRequest = db::CreateQuery(db);
    pointInsertRequest->Prepare("INSERT INTO point(type, name, latitude, longitude, icao, airport)"
                                 " VALUES(:type, :name, :latitude, :longitude, :icao, :airport)",
                                 "id");
    QSharedPointer<db::CDBQuery> pointSelectRequest = db::CreateQuery(db);
    pointSelectRequest->Prepare("SELECT id, latitude, longitude FROM point"
                                 " WHERE type = :type AND name = :name AND"
                                 " (icao IS NOT NULL AND icao = :icao OR icao IS NULL) AND"
                                 " (airport IS NOT NULL AND airport = :airport OR airport IS NULL)");
    QSharedPointer<db::CDBQuery> pointUpdateRequest = db::CreateQuery(db);
    pointUpdateRequest->Prepare("UPDATE point SET latitude = :latitude, longitude = :longitude WHERE id = :id");
    QSharedPointer<db::CDBQuery> insertZonePoint = db::CreateQuery(db);
    insertZonePoint->Prepare("INSERT INTO zone_point(zone_id, point_id) VALUES(:zone_id, :point_id)");
    foreach (const NdbNavaid &f, data)
    {
        movingToNextAndCheckCancelation();
        if (!isIncluded(firBoundary, Vincenty::toCartesian(f.ndbCoord, centerGeo)))
            continue;

        pointSelectRequest->BindValue(":type", 2);
        pointSelectRequest->BindValue(":name", f.ndbId);
        pointSelectRequest->BindValue(":icao", f.icaoCode2);
        pointSelectRequest->BindValue(":airport", f.airportIcaoId);
        if (!pointSelectRequest->exec())
        {
            qWarning() << __FILE__ << __LINE__ << pointSelectRequest->lastError().text() << f.toString();
            continue;
        }

        quint32 uiPointID;
        if (pointSelectRequest->next())
        {
            uiPointID = pointSelectRequest->value(0).toUInt();
            double d1 = pointSelectRequest->value(1).toDouble() - f.ndbCoord.latitude();
            double d2 = pointSelectRequest->value(2).toDouble() - f.ndbCoord.longitude();
            if (!qFuzzyIsNull(d1) || !qFuzzyIsNull(d2))
            {
                pointUpdateRequest->BindValue(":latitude", f.ndbCoord.latitude());
                pointUpdateRequest->BindValue(":longitude", f.ndbCoord.longitude());
                pointUpdateRequest->BindValue(":id", uiPointID);
                if (!pointUpdateRequest->exec())
                    qWarning() << __FILE__ << __LINE__ << pointSelectRequest->lastError().text() << f.toString();
            }
        }
        else
        {
            pointInsertRequest->BindValue(":type", 2);
            pointInsertRequest->BindValue(":name", f.ndbId);
            pointInsertRequest->BindValue(":latitude", f.ndbCoord.latitude());
            pointInsertRequest->BindValue(":longitude", f.ndbCoord.longitude());
            pointInsertRequest->BindValue(":icao", f.icaoCode2);
            pointInsertRequest->BindValue(":airport", f.airportIcaoId);
            if (!pointInsertRequest->exec())
            {
                qWarning() << __FILE__ << __LINE__ << pointInsertRequest->lastError().text() << f.toString();
                continue;
            }
            uiPointID = pointInsertRequest->GetLastInsertID().toInt();
        }

        if (!checkZonePoint(firDbId, uiPointID))
        {
            insertZonePoint->BindValue(":zone_id", firDbId);
            insertZonePoint->BindValue(":point_id", uiPointID);
            if (!insertZonePoint->exec())
            {
                qWarning() << __FILE__ << __LINE__ << insertZonePoint->lastError().text() << f.toString();
                continue;
            }
        }
    }
}

void ArincDatabaseImporter::insertEnrouteWaypoint(const QPolygonF &firBoundary, int firDbId, const GeoPoint &centerGeo, const QList<EnrouteWaypoint> &data)
{
    emit statusChanged(tr("Inserting enroute waypoints..."));
    QSharedPointer<db::CDBQuery> pointInsertRequest = db::CreateQuery(db);
    pointInsertRequest->Prepare("INSERT INTO point(type, name, latitude, longitude, icao, airport)"
                                 " VALUES(:type, :name, :latitude, :longitude, :icao, :airport)",
                                 "id");
    QSharedPointer<db::CDBQuery> pointSelectRequest = db::CreateQuery(db);
    pointSelectRequest->Prepare("SELECT id, latitude, longitude FROM point "
                                 "WHERE type = :type AND name = :name AND "
                                 "(icao IS NOT NULL AND icao = :icao OR icao IS NULL) AND "
                                 "(airport IS NOT NULL AND airport = :airport OR airport IS NULL)");
    QSharedPointer<db::CDBQuery> pointUpdateRequest = db::CreateQuery(db);
    pointUpdateRequest->Prepare("UPDATE point SET latitude = :latitude, longitude = :longitude WHERE id = :id");
    QSharedPointer<db::CDBQuery> insertZonePoint = db::CreateQuery(db);
    insertZonePoint->Prepare("INSERT INTO zone_point(zone_id, point_id) VALUES(:zone_id, :point_id)");
    foreach (const EnrouteWaypoint &f, data)
    {
        movingToNextAndCheckCancelation();
        if (!isIncluded(firBoundary, Vincenty::toCartesian(f.coord, centerGeo)))
            continue;

        pointSelectRequest->BindValue(":type", 3);
        pointSelectRequest->BindValue(":name", f.id);
        pointSelectRequest->BindValue(":icao", f.icaoCode2);
        pointSelectRequest->BindValue(":airport", QString());
        if (!pointSelectRequest->exec())
        {
            qWarning() << __FILE__ << __LINE__ << pointSelectRequest->lastError().text() << f.toString();
            continue;
        }

        quint32 uiPointID;
        if (pointSelectRequest->next())
        {
            uiPointID = pointSelectRequest->value(0).toUInt();
            double d1 = pointSelectRequest->value(1).toDouble() - f.coord.latitude();
            double d2 = pointSelectRequest->value(2).toDouble() - f.coord.longitude();
            if (qFuzzyIsNull(d1) || qFuzzyIsNull(d2))
            {
                pointUpdateRequest->BindValue(":latitude", f.coord.latitude());
                pointUpdateRequest->BindValue(":longitude", f.coord.longitude());
                pointUpdateRequest->BindValue(":id", uiPointID);
                if (!pointUpdateRequest->exec())
                    qWarning() << __FILE__ << __LINE__ << pointSelectRequest->lastError().text() << f.toString();
            }
        }
        else
        {
            pointInsertRequest->BindValue(":type", 3);
            pointInsertRequest->BindValue(":name", f.id);
            pointInsertRequest->BindValue(":latitude", f.coord.latitude());
            pointInsertRequest->BindValue(":longitude", f.coord.longitude());
            pointInsertRequest->BindValue(":icao", f.icaoCode2);
            pointInsertRequest->BindValue(":airport", QString());
            if (!pointInsertRequest->exec())
            {
                qWarning() << __FILE__ << __LINE__ << pointInsertRequest->lastError().text() << f.toString();
                continue;
            }
            uiPointID = pointInsertRequest->GetLastInsertID().toInt();
        }

        if (!checkZonePoint(firDbId, uiPointID))
        {
            insertZonePoint->BindValue(":zone_id", firDbId);
            insertZonePoint->BindValue(":point_id", uiPointID);
            if (!insertZonePoint->exec())
            {
                qWarning() << __FILE__ << __LINE__ << insertZonePoint->lastError().text() << f.toString();
                continue;
            }
        }
    }
}

void ArincDatabaseImporter::insertTerminalWaypoint(const QPolygonF &firBoundary, int firDbId, const GeoPoint &centerGeo, const QList<TerminalWaypoint> &data)
{
    emit statusChanged(tr("Inserting terminal waypoints..."));
    QSharedPointer<db::CDBQuery> pointInsertRequest = db::CreateQuery(db);
    pointInsertRequest->Prepare("INSERT INTO point(type, name, latitude, longitude, icao, airport)"
                                 " VALUES(:type, :name, :latitude, :longitude, :icao, :airport)",
                                 "id");
    QSharedPointer<db::CDBQuery> pointSelectRequest = db::CreateQuery(db);
    pointSelectRequest->Prepare("SELECT id, latitude, longitude FROM point "
                                 "WHERE type = :type AND name = :name AND "
                                 "(icao IS NOT NULL AND icao = :icao OR icao IS NULL) AND "
                                 "(airport IS NOT NULL AND airport = :airport OR airport IS NULL)");
    QSharedPointer<db::CDBQuery> pointUpdateRequest = db::CreateQuery(db);
    pointUpdateRequest->Prepare("UPDATE point SET latitude = :latitude, longitude = :longitude WHERE id = :id");
    QSharedPointer<db::CDBQuery> insertZonePoint = db::CreateQuery(db);
    insertZonePoint->Prepare("INSERT INTO zone_point(zone_id, point_id) VALUES(:zone_id, :point_id)");
    QSharedPointer<db::CDBQuery> selectAirport = db::CreateQuery(db);
    selectAirport->Prepare("SELECT id FROM airport WHERE name = :name");
    foreach (const TerminalWaypoint &f, data)
    {
        movingToNextAndCheckCancelation();
        if (!isIncluded(firBoundary, Vincenty::toCartesian(f.coord, centerGeo)))
            continue;

        selectAirport->BindValue(":name", f.regionCode);
        if (!selectAirport->exec())
        {
            qWarning() << __FILE__ << __LINE__ << selectAirport->lastError().text() << f.toString();
            continue;
        }
        if (!selectAirport->next())
            continue;

        pointSelectRequest->BindValue(":type", 6);
        pointSelectRequest->BindValue(":name", f.id);
        pointSelectRequest->BindValue(":icao", f.icaoCode2);
        pointSelectRequest->BindValue(":airport", f.regionCode);
        if (!pointSelectRequest->exec())
        {
            qWarning() << __FILE__ << __LINE__ << pointSelectRequest->lastError().text() << f.toString();
            continue;
        }

        quint32 uiPointID;
        if (pointSelectRequest->next())
        {
            uiPointID = pointSelectRequest->value(0).toUInt();
            double d1 = pointSelectRequest->value(1).toDouble() - f.coord.latitude();
            double d2 = pointSelectRequest->value(2).toDouble() - f.coord.longitude();
            if (qFuzzyIsNull(d1) || qFuzzyIsNull(d2))
            {
                pointUpdateRequest->BindValue(":latitude", f.coord.latitude());
                pointUpdateRequest->BindValue(":longitude", f.coord.longitude());
                pointUpdateRequest->BindValue(":id", uiPointID);
                if (!pointUpdateRequest->exec())
                    qWarning() << __FILE__ << __LINE__ << pointSelectRequest->lastError().text() << f.toString();
            }
        }
        else
        {
            pointInsertRequest->BindValue(":type", 6);
            pointInsertRequest->BindValue(":name", f.id);
            pointInsertRequest->BindValue(":latitude", f.coord.latitude());
            pointInsertRequest->BindValue(":longitude", f.coord.longitude());
            pointInsertRequest->BindValue(":icao", f.icaoCode2);
            pointInsertRequest->BindValue(":airport", f.regionCode);
            if (!pointInsertRequest->exec())
            {
                qWarning() << __FILE__ << __LINE__ << pointInsertRequest->lastError().text() << f.toString();
                continue;
            }
            uiPointID = pointInsertRequest->GetLastInsertID().toInt();
        }

        if (!checkZonePoint(firDbId, uiPointID))
        {
            insertZonePoint->BindValue(":zone_id", firDbId);
            insertZonePoint->BindValue(":point_id", uiPointID);
            if (!insertZonePoint->exec())
            {
                qWarning() << __FILE__ << __LINE__ << insertZonePoint->lastError().text() << f.toString();
                continue;
            }
        }
    }
}

void ArincDatabaseImporter::insertNdbTerminal(const QPolygonF &firBoundary, int firDbId, const GeoPoint &centerGeo, const QList<NdbTerminal> &data)
{
    emit statusChanged(tr("Inserting NDB terminals..."));
    QSharedPointer<db::CDBQuery> pointInsertRequest = db::CreateQuery(db);
    pointInsertRequest->Prepare("INSERT INTO point(type, name, latitude, longitude, icao, airport)"
                                 " VALUES(:type, :name, :latitude, :longitude, :icao, :airport)",
                                 "id");
    QSharedPointer<db::CDBQuery> pointSelectRequest = db::CreateQuery(db);
    pointSelectRequest->Prepare("SELECT id, latitude, longitude FROM point "
                                 "WHERE type = :type AND name = :name AND "
                                 "(icao IS NOT NULL AND icao = :icao OR icao IS NULL) AND "
                                 "(airport IS NOT NULL AND airport = :airport OR airport IS NULL)");
    QSharedPointer<db::CDBQuery> pointUpdateRequest = db::CreateQuery(db);
    pointUpdateRequest->Prepare("UPDATE point SET latitude = :latitude, longitude = :longitude WHERE id = :id");
    QSharedPointer<db::CDBQuery> insertZonePoint = db::CreateQuery(db);
    insertZonePoint->Prepare("INSERT INTO zone_point(zone_id, point_id) VALUES(:zone_id, :point_id)");
    foreach (const NdbTerminal &f, data)
    {
        movingToNextAndCheckCancelation();

        if (!isIncluded(firBoundary, Vincenty::toCartesian(f.ndbCoord, centerGeo)))
            continue;

        pointSelectRequest->BindValue(":type", 7);
        pointSelectRequest->BindValue(":name", f.ndbId);
        pointSelectRequest->BindValue(":icao", f.icaoCode2);
        pointSelectRequest->BindValue(":airport", f.airportIcaoId);
        if (!pointSelectRequest->exec())
        {
            qWarning() << __FILE__ << __LINE__ << pointSelectRequest->lastError().text() << f.toString();
            continue;
        }

        quint32 uiPointID;
        if (pointSelectRequest->next())
        {
            uiPointID = pointSelectRequest->value(0).toUInt();
            double d1 = pointSelectRequest->value(1).toDouble() - f.ndbCoord.latitude();
            double d2 = pointSelectRequest->value(2).toDouble() - f.ndbCoord.longitude();
            if (qFuzzyIsNull(d1) || qFuzzyIsNull(d2))
            {
                pointUpdateRequest->BindValue(":latitude", f.ndbCoord.latitude());
                pointUpdateRequest->BindValue(":longitude", f.ndbCoord.longitude());
                pointUpdateRequest->BindValue(":id", uiPointID);
                if (!pointUpdateRequest->exec())
                    qWarning() << __FILE__ << __LINE__ << pointSelectRequest->lastError().text() << f.toString();
            }
        }
        else
        {
            pointInsertRequest->BindValue(":type", 7);
            pointInsertRequest->BindValue(":name", f.ndbId);
            pointInsertRequest->BindValue(":latitude", f.ndbCoord.latitude());
            pointInsertRequest->BindValue(":longitude", f.ndbCoord.longitude());
            pointInsertRequest->BindValue(":icao", f.icaoCode2);
            pointInsertRequest->BindValue(":airport", f.airportIcaoId);
            if (!pointInsertRequest->exec())
            {
                qWarning() << __FILE__ << __LINE__ << pointInsertRequest->lastError().text() << f.toString();
                continue;
            }
            uiPointID = pointInsertRequest->GetLastInsertID().toInt();
        }

        if (!checkZonePoint(firDbId, uiPointID))
        {
            insertZonePoint->BindValue(":zone_id", firDbId);
            insertZonePoint->BindValue(":point_id", uiPointID);
            if (!insertZonePoint->exec())
            {
                qWarning() << __FILE__ << __LINE__ << insertZonePoint->lastError().text() << f.toString();
                continue;
            }
        }
    }
}

void ArincDatabaseImporter::insertRoute(int firDbId, const QList<Airway> &data)
{
    emit statusChanged(tr("Inserting routes..."));

    QSharedPointer<db::CDBQuery> insertRoute = db::CreateQuery(db);
    insertRoute->Prepare("INSERT INTO route(name, zone_id) VALUES(:name, :zone_id)", "id");
    QSharedPointer<db::CDBQuery> insertBoundaryPoint = db::CreateQuery(db);
    insertBoundaryPoint->Prepare("INSERT INTO route_point(sn, point_id, route_id, end_flag) VALUES(:sn, :point_id, :route_id, :end_flag)");
    QSharedPointer<db::CDBQuery> selectPoint = db::CreateQuery(db);
    selectPoint->Prepare("SELECT id FROM point where type = :type AND name = :name AND icao = :icao AND id IN (SELECT point_id FROM zone_point WHERE zone_id = :zone_id)");
    foreach (const Airway &f, data)
    {
        movingToNextAndCheckCancelation();

        // route points sn podpora
        if(f.points.size() && !f.points.first().sn)
        {
            Airway* temp_f(const_cast<Airway*>(&f));
            int sn = 10;
            for(QList<Airway::Point>::iterator itPoint = temp_f->points.begin(); itPoint != temp_f->points.end(); ++itPoint)
            {
                itPoint->sn = sn;
                sn += 10;
            }
        }
        /////////////////////////////////////////////////

        bool isRouteInserted = false;
        int idRoute = 0;
        foreach (const Airway::Point &p, f.points)
        {
            selectPoint->BindValue(":type", toOldPointType(p.sectionCode, p.subsectionCode));
            selectPoint->BindValue(":name", p.id);
            selectPoint->BindValue(":icao", p.icaoCode1);
            selectPoint->BindValue(":zone_id", firDbId);
            if (!selectPoint->exec())
            {
                qWarning() << __FILE__ << __LINE__ << selectPoint->lastError().text() << f.toString();
                continue;
            }

            if (selectPoint->next())
            {
                if (!isRouteInserted)
                {
                    isRouteInserted = true;
                    insertRoute->BindValue(":name", f.id);
                    insertRoute->BindValue(":zone_id", firDbId);
                    if (!insertRoute->exec())
                    {
                        qWarning() << __FILE__ << __LINE__ << insertRoute->lastError().text() << f.toString();
                        break;
                    }
                    idRoute = insertRoute->GetLastInsertID().toInt();
                }
                insertBoundaryPoint->BindValue(":sn", p.sn);
                insertBoundaryPoint->BindValue(":point_id", selectPoint->value(0).toInt());
                insertBoundaryPoint->BindValue(":route_id", idRoute);
                insertBoundaryPoint->BindValue(":end_flag", 0);
                if (!insertBoundaryPoint->exec())
                {
                    qWarning() << __FILE__ << __LINE__ << insertBoundaryPoint->lastError().text() << f.toString();
                    continue;
                }
            }
        }
    }

    QSharedPointer<db::CDBQuery> deleteInvalidRoutes = db::CreateQuery(db);
    deleteInvalidRoutes->Prepare("DELETE FROM route WHERE id IN (SELECT route_id FROM route_point GROUP BY route_id HAVING count(*) < 2) AND zone_id = :zone_id");
    deleteInvalidRoutes->BindValue(":zone_id", firDbId);
    if (!deleteInvalidRoutes->exec())
        qWarning() << __FILE__ << __LINE__ << deleteInvalidRoutes->lastError().text();
}

void ArincDatabaseImporter::insertAirports(const QPolygonF &firBoundary, int firDbId, const GeoPoint &centerGeo, const QList<AirportRefPoint> &data)
{
    emit statusChanged(tr("Inserting airport reference points..."));

    QSharedPointer<db::CDBQuery> insertAirport = db::CreateQuery(db);
    insertAirport->Prepare("INSERT INTO airport(name, latitude, longitude, icao, altitude, declination) VALUES(:name, :latitude, :longitude, :icao, :altitude, :declination)", "id");
    QSharedPointer<db::CDBQuery> insertZoneAirport = db::CreateQuery(db);
    insertZoneAirport->Prepare("INSERT INTO zone_airport(zone_id, airport_id) VALUES(:zone_id, :airport_id)", "id");
    QSharedPointer<db::CDBQuery> selectAirport = db::CreateQuery(db);
    selectAirport->Prepare("SELECT id, latitude, longitude, altitude FROM airport WHERE icao = :icao AND name = :name");
    QSharedPointer<db::CDBQuery> updateAirport = db::CreateQuery(db);
    updateAirport->Prepare("UPDATE airport SET latitude = :latitude, longitude = :longitude, altitude = :altitude, declination = :declination WHERE id = :id");
    QSharedPointer<db::CDBQuery> airportRemoveElements = db::CreateQuery(db);
    airportRemoveElements->Prepare("DELETE FROM runway WHERE airport_id = :airport_id");
    foreach (const AirportRefPoint &f, data)
    {
        movingToNextAndCheckCancelation();

        if (!isIncluded(firBoundary, Vincenty::toCartesian(f.coord, centerGeo)))
            continue;

        selectAirport->BindValue(":icao", f.icaoCode);
        selectAirport->BindValue(":name", f.airportIcaoId);
        if (!selectAirport->exec())
        {
            qWarning() << __FILE__ << __LINE__ << selectAirport->lastError().text() << f.toString();
            continue;
        }

        int airportDbId;
        if (selectAirport->next())
        {
            airportDbId = selectAirport->value(0).toInt();
            updateAirport->BindValue(":latitude", f.coord.latitude());
            updateAirport->BindValue(":longitude", f.coord.longitude());
            updateAirport->BindValue(":altitude", f.elevation);
            updateAirport->BindValue(":id", airportDbId);
            updateAirport->BindValue(":declination", QString(f.magneticVariation));

            if (!updateAirport->exec())
            {
                qWarning() << __FILE__ << __LINE__ << updateAirport->lastError().text() << f.toString();
                continue;
            }

            airportRemoveElements->BindValue(":airport_id", airportDbId);
            if (!airportRemoveElements->exec())
            {
                qWarning() << __FILE__ << __LINE__ << airportRemoveElements->lastError().text() << f.toString();
                continue;
            }
        }
        else
        {
            insertAirport->BindValue(":name", f.airportIcaoId);
            insertAirport->BindValue(":latitude", f.coord.latitude());
            insertAirport->BindValue(":longitude", f.coord.longitude());
            insertAirport->BindValue(":icao", f.icaoCode);
            insertAirport->BindValue(":altitude", f.elevation);
            insertAirport->BindValue(":declination", QString(f.magneticVariation));
            if (!insertAirport->exec())
            {
                qWarning() << __FILE__ << __LINE__ << insertAirport->lastError().text() << f.toString();
                continue;
            }
            airportDbId = insertAirport->GetLastInsertID().toInt();
        }

        insertZoneAirport->BindValue(":zone_id", firDbId);
        insertZoneAirport->BindValue(":airport_id", airportDbId);
        if (!insertZoneAirport->exec())
        {
            qWarning() << __FILE__ << __LINE__ << insertZoneAirport->lastError().text() << f.toString();
            continue;
        }
    }
}

void ArincDatabaseImporter::insertRunway(int firDbId, const QList<Runway> &data)
{
    emit statusChanged(tr("Inserting runways..."));

    QSharedPointer<db::CDBQuery> insertRunway = db::CreateQuery(db);
    insertRunway->Prepare("INSERT INTO runway(name, latitude, longitude, length, azimuth, airport_id, width) VALUES (:name, :latitude, :longitude, :length, :azimuth, :airport_id, :width)", "id");
    QSharedPointer<db::CDBQuery> selectAirport = db::CreateQuery(db);
    selectAirport->Prepare("SELECT id FROM airport WHERE name = :name AND icao = :icao AND id in (SELECT airport_id FROM zone_airport WHERE zone_id = :zone_id)");
    foreach (const Runway &f, data)
    {
        movingToNextAndCheckCancelation();

        selectAirport->BindValue(":name", f.airportIcaoId);
        selectAirport->BindValue(":icao", f.icaoCode);
        selectAirport->BindValue(":zone_id", firDbId);
        if (!selectAirport->exec())
        {
            qWarning() << __FILE__ << __LINE__ << selectAirport->lastError().text() << f.toString();
            continue;
        }

        if (selectAirport->next())
        {
            insertRunway->BindValue(":name", f.id);
            insertRunway->BindValue(":latitude", f.coord.latitude());
            insertRunway->BindValue(":longitude", f.coord.longitude());
            insertRunway->BindValue(":length", f.length);
            insertRunway->BindValue(":azimuth", f.magneticBearing);
            insertRunway->BindValue(":airport_id", selectAirport->value(0).toInt());
            insertRunway->BindValue(":width", f.width);
            if (!insertRunway->exec())
            {
                qWarning() << __FILE__ << __LINE__ << insertRunway->lastError().text() << f.toString();
                continue;
            }
        }
    }
}

void ArincDatabaseImporter::insertSid(int firDbId, const QList<Sid> &data)
{
    emit statusChanged(tr("Inserting SIDs..."));

    QSharedPointer<db::CDBQuery> insertSSA = db::CreateQuery(db);
    insertSSA->Prepare("INSERT into ssa(name, type, runway_id, trans_alt) VALUES (:name, :type, :runway_id, :trans_alt)", "id");
    QSharedPointer<db::CDBQuery> selectPoint1 = db::CreateQuery(db);
    selectPoint1->Prepare("select id from point where name = :name and icao = :icao and type = :type");
    QSharedPointer<db::CDBQuery> selectPoint2 = db::CreateQuery(db);
    selectPoint2->Prepare("select id from point where name = :name and icao = :icao and type = :type and airport = :airport");
    QSharedPointer<db::CDBQuery> insertSsaPoint = db::CreateQuery(db);
    insertSsaPoint->Prepare("INSERT INTO ssa_point(sn, point_id, ssa_id, type, course, turn, altitude1, fly_over) VALUES (:sn, :point_id, :ssa_id, :type, :course, :turn, :altitude1, :fly_over)", "id");
    QSharedPointer<db::CDBQuery> selectRunway = db::CreateQuery(db);
    selectRunway->Prepare("SELECT id from runway where name = :name and airport_id = :airport_id");
    QSharedPointer<db::CDBQuery> selectAirport = db::CreateQuery(db);
    selectAirport->Prepare("SELECT id FROM airport WHERE name = :name and id in (SELECT airport_id FROM zone_airport WHERE zone_id = :zone_id)");
    QSharedPointer<db::CDBQuery> insertRunway = db::CreateQuery(db);
    insertRunway->Prepare("INSERT INTO runway(name, latitude, longitude, length, azimuth, airport_id, width) VALUES (:name, :latitude, :longitude, :length, :azimuth, :airport_id, :width)", "id");
    foreach (const Sid &f, data)
    {
        movingToNextAndCheckCancelation();

        selectAirport->BindValue(":name", f.airportId);
        selectAirport->BindValue(":zone_id", firDbId);
        if (!selectAirport->exec())
        {
            qWarning() << __FILE__ << __LINE__ << selectAirport->lastError().text() << f.toString();
            continue;
        }

        if (selectAirport->next())
        {
            ////////////////////////////////////////////////////////////
            if(bCanEmptyRW && f.transitionId.isEmpty())
                *(const_cast<QString*>(&f.transitionId)) = sEmptyRW;
            ////////////////////////////////////////////////////////////

            selectRunway->BindValue(":name", f.transitionId);
            selectRunway->BindValue(":airport_id", selectAirport->value(0).toInt());
            if (!selectRunway->exec())
            {
                qWarning() << __FILE__ << __LINE__ << selectRunway->lastError().text() << f.toString();
                continue;
            }

            int runwayId(0);
            if (!selectRunway->next())
            {
                if (f.transitionId.isEmpty())
                    continue;
                insertRunway->BindValue(":name", f.transitionId);
                insertRunway->BindValue(":latitude", 0);
                insertRunway->BindValue(":longitude", 0);
                insertRunway->BindValue(":length", 0);
                insertRunway->BindValue(":azimuth", 0);
                insertRunway->BindValue(":airport_id", selectAirport->value(0).toInt());
                insertRunway->BindValue(":width", 0);
                if (!insertRunway->exec())
                {
                    qWarning() << __FILE__ << __LINE__ << insertRunway->lastError().text() << f.toString();
                    continue;
                }
                runwayId = insertRunway->GetLastInsertID().toInt();
            }
            else
                runwayId = selectRunway->value(0).toInt();

            int idSSA = checkSSA(f.id, 1, runwayId);
            if(!idSSA)
            {
                insertSSA->BindValue(":name", f.id);
                insertSSA->BindValue(":type", 1);
                insertSSA->BindValue(":runway_id", runwayId);
                insertSSA->BindValue(":trans_alt", f.points.isEmpty() ? 0 : f.points.at(0).transitionAltitude);
                if (!insertSSA->exec())
                {
                    qWarning() << __FILE__ << __LINE__ << insertSSA->lastError().text() << f.toString();
                    continue;
                }
                idSSA = insertSSA->GetLastInsertID().toInt();
            }

            // SSA points sn podpora
            if(f.points.size() && !f.points.first().sn)
            {
                Sid* temp_f(const_cast<Sid*>(&f));
                int sn = 10;
                for(QList<SSA::Point>::iterator itPoint = temp_f->points.begin(); itPoint != temp_f->points.end(); ++itPoint)
                {
                    itPoint->sn = sn;
                    sn += 10;
                }
            }
            /////////////////////////////////////////////////

            foreach (const SSA::Point &p, f.points)
            {
                int pointId = 0;
                int oldType = toOldPointType(p.sectionCode, p.subsrectionCode);
                if (1 <= oldType && oldType <= 3)
                {
                    selectPoint1->BindValue(":name", p.id);
                    selectPoint1->BindValue(":icao", p.icaoCode);
                    selectPoint1->BindValue(":type", oldType);
                    if (!selectPoint1->exec())
                    {
                        qWarning() << __FILE__ << __LINE__ << selectPoint1->lastError().text() << f.toString();
                        continue;
                    }
                    if (selectPoint1->next())
                        pointId = selectPoint1->value(0).toInt();
                }
                else
                {
                    selectPoint2->BindValue(":name", p.id);
                    selectPoint2->BindValue(":icao", p.icaoCode);
                    selectPoint2->BindValue(":type", oldType);
                    selectPoint2->BindValue(":airport", f.airportId);
                    if (!selectPoint2->exec())
                    {
                        qWarning() << __FILE__ << __LINE__ << selectPoint2->lastError().text() << f.toString();
                        continue;
                    }
                    if (selectPoint2->next())
                        pointId = selectPoint2->value(0).toInt();
                }

                insertSsaPoint->BindValue(":sn", p.sn);
                insertSsaPoint->BindValue(":point_id", pointId);
                insertSsaPoint->BindValue(":ssa_id", idSSA);
                if ("IF" == p.path)
                    insertSsaPoint->BindValue(":type", 1);
                else if ("TF" == p.path)
                    insertSsaPoint->BindValue(":type", 2);
                else if ("CF" == p.path)
                    insertSsaPoint->BindValue(":type", 3);
                else if ("DF" == p.path)
                    insertSsaPoint->BindValue(":type", 4);
                else if ("CA" == p.path)
                    insertSsaPoint->BindValue(":type", 5);
                else if ("CI" == p.path)
                    insertSsaPoint->BindValue(":type", 6);
                else
                    insertSsaPoint->BindValue(":type", 0);
                insertSsaPoint->BindValue(":course", p.course);
                insertSsaPoint->BindValue(":turn", int(p.turn));
                insertSsaPoint->BindValue(":altitude1", p.altitude1);
                insertSsaPoint->BindValue(":fly_over", p.descriptionCode[1] == 'Y' || p.descriptionCode[1] == 'B' ? 1 : 0);
                if (!insertSsaPoint->exec())
                {
                    qWarning() << __FILE__ << __LINE__ << insertSsaPoint->lastError().text() << f.toString();
                    continue;
                }
            }
        }
    }
}

void ArincDatabaseImporter::insertStar(int firDbId, const QList<Star> &data)
{
    emit statusChanged(tr("Inserting STARs..."));

    QSharedPointer<db::CDBQuery> insertSSA = db::CreateQuery(db);
    insertSSA->Prepare("INSERT into ssa(name, type, runway_id, trans_alt) VALUES (:name, :type, :runway_id, :trans_alt)", "id");
    QSharedPointer<db::CDBQuery> selectPoint1 = db::CreateQuery(db);
    selectPoint1->Prepare("select id from point where name = :name and icao = :icao and type = :type");
    QSharedPointer<db::CDBQuery> selectPoint2 = db::CreateQuery(db);
    selectPoint2->Prepare("select id from point where name = :name and icao = :icao and type = :type and airport = :airport");
    QSharedPointer<db::CDBQuery> insertSsaPoint = db::CreateQuery(db);
    insertSsaPoint->Prepare("INSERT INTO ssa_point(sn, point_id, ssa_id, type, course, turn, altitude1, fly_over) VALUES (:sn, :point_id, :ssa_id, :type, :course, :turn, :altitude1, :fly_over)", "id");
    QSharedPointer<db::CDBQuery> selectRunway = db::CreateQuery(db);
    selectRunway->Prepare("SELECT id from runway where name = :name and airport_id = :airport_id");
    QSharedPointer<db::CDBQuery> selectAirport = db::CreateQuery(db);
    selectAirport->Prepare("SELECT id FROM airport WHERE name = :name and id in (SELECT airport_id FROM zone_airport WHERE zone_id = :zone_id)");
    QSharedPointer<db::CDBQuery> insertRunway = db::CreateQuery(db);
    insertRunway->Prepare("INSERT INTO runway(name, latitude, longitude, length, azimuth, airport_id, width) VALUES (:name, :latitude, :longitude, :length, :azimuth, :airport_id, :width)", "id");
    foreach (const Star &f, data)
    {
        movingToNextAndCheckCancelation();

        selectAirport->BindValue(":name", f.airportId);
        selectAirport->BindValue(":zone_id", firDbId);
        if (!selectAirport->exec())
        {
            qWarning() << __FILE__ << __LINE__ << selectAirport->lastError().text() << f.toString();
            continue;
        }

        if (selectAirport->next())
        {
            ////////////////////////////////////////////////////////////
            if(bCanEmptyRW && f.transitionId.isEmpty())
                *(const_cast<QString*>(&f.transitionId)) = sEmptyRW;
            ////////////////////////////////////////////////////////////

            selectRunway->BindValue(":name", f.transitionId);
            selectRunway->BindValue(":airport_id", selectAirport->value(0).toInt());
            if (!selectRunway->exec())
            {
                qWarning() << __FILE__ << __LINE__ << selectRunway->lastError().text() << f.toString();
                continue;
            }

            int runwayId(0);

            if (!selectRunway->next())
            {
                if (f.transitionId.isEmpty())
                    continue;
                insertRunway->BindValue(":name", f.transitionId);
                insertRunway->BindValue(":latitude", 0);
                insertRunway->BindValue(":longitude", 0);
                insertRunway->BindValue(":length", 0);
                insertRunway->BindValue(":azimuth", 0);
                insertRunway->BindValue(":airport_id", selectAirport->value(0).toInt());
                insertRunway->BindValue(":width", 0);
                if (!insertRunway->exec())
                {
                    qWarning() << __FILE__ << __LINE__ << insertRunway->lastError().text() << f.toString();
                    continue;
                }
                runwayId = insertRunway->GetLastInsertID().toInt();
            }
            else
                runwayId = selectRunway->value(0).toInt();

            int idSSA = checkSSA(f.id, 2, runwayId);
            if(!idSSA)
            {
                insertSSA->BindValue(":name", f.id);
                insertSSA->BindValue(":type", 2);
                insertSSA->BindValue(":runway_id", runwayId);
                insertSSA->BindValue(":trans_alt", f.points.isEmpty() ? 0 : f.points.at(0).transitionAltitude);
                if (!insertSSA->exec())
                {
                    qWarning() << __FILE__ << __LINE__ << insertSSA->lastError().text() << f.toString();
                    continue;
                }
                idSSA = insertSSA->GetLastInsertID().toInt();
            }

            // SSA points sn podpora
            if(f.points.size() && !f.points.first().sn)
            {
                Star* temp_f(const_cast<Star*>(&f));
                int sn = 10;
                for(QList<SSA::Point>::iterator itPoint = temp_f->points.begin(); itPoint != temp_f->points.end(); ++itPoint)
                {
                    itPoint->sn = sn;
                    sn += 10;
                }
            }
            /////////////////////////////////////////////////

            foreach (const SSA::Point &p, f.points)
            {
                int pointId = 0;
                int oldType = toOldPointType(p.sectionCode, p.subsrectionCode);
                if (1 <= oldType && oldType <= 3)
                {
                    selectPoint1->BindValue(":name", p.id);
                    selectPoint1->BindValue(":icao", p.icaoCode);
                    selectPoint1->BindValue(":type", oldType);
                    if (!selectPoint1->exec())
                    {
                        qWarning() << __FILE__ << __LINE__ << selectPoint1->lastError().text() << f.toString();
                        continue;
                    }
                    if (selectPoint1->next())
                        pointId = selectPoint1->value(0).toInt();
                }
                else
                {
                    selectPoint2->BindValue(":name", p.id);
                    selectPoint2->BindValue(":icao", p.icaoCode);
                    selectPoint2->BindValue(":type", oldType);
                    selectPoint2->BindValue(":airport", f.airportId);
                    if (!selectPoint2->exec())
                    {
                        qWarning() << __FILE__ << __LINE__ << selectPoint2->lastError().text() << f.toString();
                        continue;
                    }
                    if (selectPoint2->next())
                        pointId = selectPoint2->value(0).toInt();
                }

                insertSsaPoint->BindValue(":sn", p.sn);
                insertSsaPoint->BindValue(":point_id", pointId);
                insertSsaPoint->BindValue(":ssa_id", idSSA);
                if ("IF" == p.path)
                    insertSsaPoint->BindValue(":type", 1);
                else if ("TF" == p.path)
                    insertSsaPoint->BindValue(":type", 2);
                else if ("CF" == p.path)
                    insertSsaPoint->BindValue(":type", 3);
                else if ("DF" == p.path)
                    insertSsaPoint->BindValue(":type", 4);
                else if ("CA" == p.path)
                    insertSsaPoint->BindValue(":type", 5);
                else if ("CI" == p.path)
                    insertSsaPoint->BindValue(":type", 6);
                else
                    insertSsaPoint->BindValue(":type", 0);
                insertSsaPoint->BindValue(":course", p.course);
                insertSsaPoint->BindValue(":turn", int(p.turn));
                insertSsaPoint->BindValue(":altitude1", p.altitude1);
                insertSsaPoint->BindValue(":fly_over", p.descriptionCode[1] == 'Y' || p.descriptionCode[1] == 'B' ? 1 : 0);
                if (!insertSsaPoint->exec())
                {
                    qWarning() << __FILE__ << __LINE__ << insertSsaPoint->lastError().text() << f.toString();
                    continue;
                }
            }
        }
    }
}

void ArincDatabaseImporter::insertApproach(int firDbId, const QList<Approach> &data)
{
    emit statusChanged(tr("Inserting approachs..."));

    QSharedPointer<db::CDBQuery> insertSSA = db::CreateQuery(db);
    insertSSA->Prepare("INSERT into ssa(name, type, runway_id, trans_alt) VALUES (:name, :type, :runway_id, :trans_alt)", "id");
    QSharedPointer<db::CDBQuery> selectPoint1 = db::CreateQuery(db);
    selectPoint1->Prepare("select id from point where name = :name and icao = :icao and type = :type");
    QSharedPointer<db::CDBQuery> selectPoint2 = db::CreateQuery(db);
    selectPoint2->Prepare("select id from point where name = :name and icao = :icao and type = :type and airport = :airport");
    QSharedPointer<db::CDBQuery> insertSsaPoint = db::CreateQuery(db);
    insertSsaPoint->Prepare("INSERT INTO ssa_point(sn, point_id, ssa_id, type, course, turn, altitude1, fly_over) VALUES (:sn, :point_id, :ssa_id, :type, :course, :turn, :altitude1, :fly_over)", "id");
    QSharedPointer<db::CDBQuery> selectRunway = db::CreateQuery(db);
    selectRunway->Prepare("SELECT id from runway where name = :name and airport_id = :airport_id");
    QSharedPointer<db::CDBQuery> selectAirport = db::CreateQuery(db);
    selectAirport->Prepare("SELECT id FROM airport WHERE name = :name and id in (SELECT airport_id FROM zone_airport WHERE zone_id = :zone_id)");
    QSharedPointer<db::CDBQuery> insertRunway = db::CreateQuery(db);
    insertRunway->Prepare("INSERT INTO runway(name, latitude, longitude, length, azimuth, airport_id, width) VALUES (:name, :latitude, :longitude, :length, :azimuth, :airport_id, :width)", "id");
    foreach (const Approach &f, data)
    {
        movingToNextAndCheckCancelation();

        selectAirport->BindValue(":name", f.airportId);
        selectAirport->BindValue(":zone_id", firDbId);
        if (!selectAirport->exec())
        {
            qWarning() << __FILE__ << __LINE__ << selectAirport->lastError().text() << f.toString();
            continue;
        }

        if (selectAirport->next())
        {
            ////////////////////////////////////////////////////////////
            if(bCanEmptyRW && f.transitionId.isEmpty())
                *(const_cast<QString*>(&f.transitionId)) = sEmptyRW;
            ////////////////////////////////////////////////////////////

            selectRunway->BindValue(":name", f.transitionId);
            selectRunway->BindValue(":airport_id", selectAirport->value(0).toInt());
            if (!selectRunway->exec())
            {
                qWarning() << __FILE__ << __LINE__ << selectRunway->lastError().text() << f.toString();
                continue;
            }

            int runwayId(0);
            if (!selectRunway->next())
            {
                if (f.transitionId.isEmpty())
                    continue;
                insertRunway->BindValue(":name", f.transitionId);
                insertRunway->BindValue(":latitude", 0);
                insertRunway->BindValue(":longitude", 0);
                insertRunway->BindValue(":length", 0);
                insertRunway->BindValue(":azimuth", 0);
                insertRunway->BindValue(":airport_id", selectAirport->value(0).toInt());
                insertRunway->BindValue(":width", 0);
                if (!insertRunway->exec())
                {
                    qWarning() << __FILE__ << __LINE__ << insertRunway->lastError().text() << f.toString();
                    continue;
                }
                runwayId = insertRunway->GetLastInsertID().toInt();
            }
            else
                runwayId = selectRunway->value(0).toInt();

            int idSSA = checkSSA(f.id, 3, runwayId);
            if(!idSSA)
            {
                insertSSA->BindValue(":name", f.id);
                insertSSA->BindValue(":type", 3);
                insertSSA->BindValue(":runway_id", runwayId);
                insertSSA->BindValue(":trans_alt", f.points.isEmpty() ? 0 : f.points.at(0).transitionAltitude);
                if (!insertSSA->exec())
                {
                    qWarning() << __FILE__ << __LINE__ << insertSSA->lastError().text() << f.toString();
                    continue;
                }
                idSSA = insertSSA->GetLastInsertID().toInt();
            }

            // SSA points sn podpora
            if(f.points.size() && !f.points.first().sn)
            {
                Approach* temp_f(const_cast<Approach*>(&f));
                int sn = 10;
                for(QList<SSA::Point>::iterator itPoint = temp_f->points.begin(); itPoint != temp_f->points.end(); ++itPoint)
                {
                    itPoint->sn = sn;
                    sn += 10;
                }
            }
            /////////////////////////////////////////////////

            foreach (const SSA::Point &p, f.points)
            {
                int pointId = 0;
                int oldType = toOldPointType(p.sectionCode, p.subsrectionCode);
                if (1 <= oldType && oldType <= 3)
                {
                    selectPoint1->BindValue(":name", p.id);
                    selectPoint1->BindValue(":icao", p.icaoCode);
                    selectPoint1->BindValue(":type", oldType);
                    if (!selectPoint1->exec())
                    {
                        qWarning() << __FILE__ << __LINE__ << selectPoint1->lastError().text() << f.toString();
                        continue;
                    }
                    if (selectPoint1->next())
                        pointId = selectPoint1->value(0).toInt();
                }
                else
                {
                    selectPoint2->BindValue(":name", p.id);
                    selectPoint2->BindValue(":icao", p.icaoCode);
                    selectPoint2->BindValue(":type", oldType);
                    selectPoint2->BindValue(":airport", f.airportId);
                    if (!selectPoint2->exec())
                    {
                        qWarning() << __FILE__ << __LINE__ << selectPoint2->lastError().text() << f.toString();
                        continue;
                    }
                    if (selectPoint2->next())
                        pointId = selectPoint2->value(0).toInt();
                }

                insertSsaPoint->BindValue(":sn", p.sn);
                insertSsaPoint->BindValue(":point_id", pointId);
                insertSsaPoint->BindValue(":ssa_id", idSSA);
                if ("IF" == p.path)
                    insertSsaPoint->BindValue(":type", 1);
                else if ("TF" == p.path)
                    insertSsaPoint->BindValue(":type", 2);
                else if ("CF" == p.path)
                    insertSsaPoint->BindValue(":type", 3);
                else if ("DF" == p.path)
                    insertSsaPoint->BindValue(":type", 4);
                else if ("CA" == p.path)
                    insertSsaPoint->BindValue(":type", 5);
                else if ("CI" == p.path)
                    insertSsaPoint->BindValue(":type", 6);
                else
                    insertSsaPoint->BindValue(":type", 0);
                insertSsaPoint->BindValue(":course", p.course);
                insertSsaPoint->BindValue(":turn", int(p.turn));
                insertSsaPoint->BindValue(":altitude1", p.altitude1);
                insertSsaPoint->BindValue(":fly_over", p.descriptionCode[1] == 'Y' || p.descriptionCode[1] == 'B' ? 1 : 0);
                if (!insertSsaPoint->exec())
                {
                    qWarning() << __FILE__ << __LINE__ << insertSsaPoint->lastError().text() << f.toString();
                    continue;
                }
            }
        }
    }
}

void ArincDatabaseImporter::insertHolding(int firDbId, const GeoPoint &centerGeo, const QList<Holding> &data)
{
    emit statusChanged(tr("Inserting holdings..."));

    QSharedPointer<db::CDBQuery> insertHolding = db::CreateQuery(db);
    insertHolding->Prepare("INSERT INTO holding(point_id, type, in_course, turn_direction, leg_length, leg_time, speed, zone_id) VALUES(:point_id, :type, :in_course, :turn_direction, :leg_length, :leg_time, :speed, :zone_id)", "id");
    QSharedPointer<db::CDBQuery> selectPoint = db::CreateQuery(db);
    selectPoint->Prepare("SELECT id, longitude, latitude FROM point where type = :type AND name = :name AND icao = :icao AND (airport IS NOT NULL AND airport = :airport OR airport IS NULL)");
    foreach (const Holding &f, data)
    {
        movingToNextAndCheckCancelation();

        int pointType = toOldPointType(f.pointSectionCode, f.pointSubsectionCode);
        selectPoint->BindValue(":type", pointType);
        selectPoint->BindValue(":name", f.pointId);
        selectPoint->BindValue(":icao", f.pointIcaoCode);
        selectPoint->BindValue(":airport", f.regionCode);
        if (!selectPoint->exec())
        {
            qWarning() << __FILE__ << __LINE__ << selectPoint->lastError().text() << f.toString();
            continue;
        }

        if (selectPoint->next())
        {
            insertHolding->BindValue(":point_id", selectPoint->value(0).toInt());
            insertHolding->BindValue(":type", int(f.type));
            insertHolding->BindValue(":in_course", f.inboundHoldingCourse);
            insertHolding->BindValue(":turn_direction", int(f.turn));
            insertHolding->BindValue(":leg_length", f.legLength);
            insertHolding->BindValue(":leg_time", f.legTime);
            insertHolding->BindValue(":speed", f.speed);
            insertHolding->BindValue(":zone_id", firDbId);
            if (!insertHolding->exec())
            {
                qWarning() << __FILE__ << __LINE__ << insertHolding->lastError().text() << f.toString();
                continue;
            }
            int idHolding = insertHolding->GetLastInsertID().toInt();

            QList<oldmaster::OldMasterPoint> points = oldmaster::createHoldingPointsForOldMaster(GeoPoint(selectPoint->value(1).toDouble(), selectPoint->value(2).toDouble()), centerGeo, f);
            QSharedPointer<db::CDBQuery> insertHoldingPoint = db::CreateQuery(db);
            insertHoldingPoint->Prepare("INSERT INTO holding_point_master(holding_id, latitude, longitude, sn) VALUES(:holding_id, :latitude, :longitude, :sn)");
            foreach (const oldmaster::OldMasterPoint &p, points)
            {
                insertHoldingPoint->BindValue(":holding_id", idHolding);
                insertHoldingPoint->BindValue(":latitude", p.coord.latitude());
                insertHoldingPoint->BindValue(":longitude", p.coord.longitude());
                insertHoldingPoint->BindValue(":sn", p.sn);
                if (!insertHoldingPoint->exec())
                {
                    qWarning() << __FILE__ << __LINE__ << insertHoldingPoint->lastError().text() << f.toString();
                    continue;
                }
            }
        }
    }
}

void ArincDatabaseImporter::insertRestriction(const QPolygonF &firBoundary, int firDbId, const GeoPoint &centerGeo, const QList<RestrictedAirspace> &data)
{
    emit statusChanged(tr("Inserting restrictions..."));
    QSharedPointer<db::CDBQuery> insertRestriction = db::CreateQuery(db);
    insertRestriction->Prepare("INSERT INTO restriction(name, type, designation, multi_code, zone_id) VALUES(:name, :type, :designation, :multi_code, :zone_id)", "id");
    QSharedPointer<db::CDBQuery> insertRetrinctionPoint = db::CreateQuery(db);
    insertRetrinctionPoint->Prepare("INSERT INTO restriction_point(sn, type, latitude, longitude, arc_latitude, arc_longitude, arc_distance, restriction_id) VALUES (:sn, :type, :latitude, :longitude, :arc_latitude, :arc_longitude, :arc_distance, :restriction_id)");
    foreach (const RestrictedAirspace &f, data)
    {
        movingToNextAndCheckCancelation();

        bool isBelong = false;
        foreach (const BoundaryPoint &p, f.boundary)
        {
            QPointF bp;
            if (BoundaryPoint::Circle == p.type)
                bp = Vincenty::toCartesian(p.arcCoord, centerGeo);
            else
                bp = Vincenty::toCartesian(p.coord, centerGeo);
            if (isIncluded(firBoundary, bp))
            {
                isBelong = true;
                break;
            }
        }

        if (!isBelong)
            continue;

        insertRestriction->BindValue(":name", f.name);
        insertRestriction->BindValue(":type", int(f.type));
        insertRestriction->BindValue(":designation", f.designation);
        insertRestriction->BindValue(":multi_code", QString(f.multipleCode));
        insertRestriction->BindValue(":zone_id", firDbId);
        if (!insertRestriction->exec())
        {
            qWarning() << __FILE__ << __LINE__ << insertRestriction->lastError().text() << f.toString();
            continue;
        }
        int idRestriction = insertRestriction->GetLastInsertID().toInt();

        // restriction points sn podpora
        if(f.boundary.size() && !f.boundary.first().sn)
        {
            RestrictedAirspace* temp_f(const_cast<RestrictedAirspace*>(&f));
            int sn = 10;
            for(QList<BoundaryPoint>::iterator itPoint = temp_f->boundary.begin(); itPoint != temp_f->boundary.end(); ++itPoint)
            {
                itPoint->sn = sn;
                sn += 10;
            }
        }
        /////////////////////////////////////////////////

        foreach (const BoundaryPoint &p, f.boundary)
        {
            insertRetrinctionPoint->BindValue(":sn", p.sn);
            insertRetrinctionPoint->BindValue(":type", toOldBoundaryPointType(p.type));
            insertRetrinctionPoint->BindValue(":latitude", p.coord.latitude());
            insertRetrinctionPoint->BindValue(":longitude", p.coord.longitude());
            insertRetrinctionPoint->BindValue(":arc_latitude", p.arcCoord.latitude());
            insertRetrinctionPoint->BindValue(":arc_longitude", p.arcCoord.longitude());
            insertRetrinctionPoint->BindValue(":arc_distance", p.arcDistance);
            insertRetrinctionPoint->BindValue(":restriction_id", idRestriction);
            if (!insertRetrinctionPoint->exec())
            {
                qWarning() << __FILE__ << __LINE__ << insertRetrinctionPoint->lastError().text() << f.toString();
                continue;
            }
        }

        QList<oldmaster::OldMasterPoint> points = oldmaster::createRestrictionPointsForOldMaster(centerGeo, f);
        QSharedPointer<db::CDBQuery> insertRestrictionPointMaster = db::CreateQuery(db);
        insertRestrictionPointMaster->Prepare("INSERT INTO restriction_point_master(restriction_id, latitude, longitude, sn) VALUES(:restriction_id, :latitude, :longitude, :sn)");
        foreach (const oldmaster::OldMasterPoint &p, points)
        {
            insertRestrictionPointMaster->BindValue(":restriction_id", idRestriction);
            insertRestrictionPointMaster->BindValue(":latitude", p.coord.latitude());
            insertRestrictionPointMaster->BindValue(":longitude", p.coord.longitude());
            insertRestrictionPointMaster->BindValue(":sn", p.sn);
            if (!insertRestrictionPointMaster->exec())
            {
                qWarning() << __FILE__ << __LINE__ << insertRestrictionPointMaster->lastError().text() << f.toString();
                continue;
            }
        }
    }
}

bool ArincDatabaseImporter::isIncluded(const QPolygonF &boubdary, const QPointF &point, double BoundaryZoneSize)
{
    bool  result = false;
    // Starts from zero line: first point and the last point
    QVector<QPointF>::const_iterator itTemp = boubdary.end() - 1;
    qreal xold = itTemp->x();
    qreal yold = itTemp->y();
    for(QVector<QPointF>::const_iterator it = boubdary.begin(), end = boubdary.end(); it != end; ++it)
    {
        qreal xnew = it->x();
        qreal ynew = it->y();

        // Always should be x1 < x2
        qreal x1, x2, y1, y2;
        if (xnew > xold)
        {
            x1 = xold;
            x2 = xnew;
            y1 = yold;
            y2 = ynew;
        } else {
            x1 = xnew;
            x2 = xold;
            y1 = ynew;
            y2 = yold;
        }

        // Calculation is the point belong to the boundary. We are rotate coordinate system paralel boundary line and move 1st point to (0,0). Check is point belongs to some area around bpundary line.
        // Coordinate rotation xn = x*cos(a)+y*sin(a); yn = -x*sin(a)+y*cos(a);
        qreal DeltaX = x2 - x1;
        qreal DeltaY = y2 - y1;
        qreal LineX = pow(DeltaX*DeltaX + DeltaY*DeltaY, 0.5);
        // BoundaryZoneSize is the size of zone around boundary line belongs to boundary in meters
        if (!qFuzzyIsNull(LineX))
        {
            qreal PointX = ((point.x() - x1)*DeltaX + (point.y() - y1)*DeltaY) / LineX;
            if (PointX >= -BoundaryZoneSize && PointX <= LineX + BoundaryZoneSize)
            {
                qreal PointY = ((x1 - point.x())*DeltaY + (point.y() - y1)*DeltaX) / LineX;
                if (PointY >= -BoundaryZoneSize && PointY <= BoundaryZoneSize)
                    return true;
            }
        }

        // Check is the point inside the polygon of boundary points
        if ((xnew < point.x()) == (point.x() <= xold) && (point.y() - y1)*(x2 - x1) <= (y2 - y1)*(point.x() - x1))
            result = !result;
        xold = xnew;
        yold = ynew;
    }
    return result;
}

int ArincDatabaseImporter::checkSSA(const QString& sName, int iType, int iRunwayID)
{
    int idSSA = 0;
    if (bUniteSSA)
    {
        QSharedPointer<db::CDBQuery> selectSSA = db::CreateQuery(db);
        selectSSA->Prepare("SELECT id FROM ssa WHERE name = :name AND type = :type AND runway_id = :runway_id");
        selectSSA->BindValue(":name", sName);
        selectSSA->BindValue(":type", iType);
        selectSSA->BindValue(":runway_id", iRunwayID);
        if (!selectSSA->exec())
        {
            qWarning() << __FILE__ << __LINE__ << selectSSA->lastError().text();
            return 0;
        }
        if (selectSSA->next())
        {
            idSSA = selectSSA->value(0).toInt();
        }
    }
    return idSSA;
}

bool ArincDatabaseImporter::checkZonePoint(int firDbId, quint32 uiPointID)
{
    bool bExist(false);
    QSharedPointer<db::CDBQuery> selectZonePoint = db::CreateQuery(db);
    selectZonePoint->Prepare("SELECT id FROM zone_point WHERE zone_id = :zone_id AND point_id = :point_id");
    selectZonePoint->BindValue(":zone_id", firDbId);
    selectZonePoint->BindValue(":point_id", uiPointID);
    if (!selectZonePoint->exec())
    {
        qWarning() << __FILE__ << __LINE__ << selectZonePoint->lastError().text();
        return bExist;
    }
    if (selectZonePoint->next())
    {
        bExist = true;
    }
    return bExist;
}
