#include "Dao.h"
#include "View.h"

#include "AirplaneType.h"

#include "Wind.h"

#include "Meteo.h"
#include "MeteoPoint.h"
#include "MeteoRunway.h"

#include "front/core/airplane/AClist/SFPL.h"
#include "dbconnectionsmanager.h"

#include "User.h"
#include "Sectoring.h"
#include "BoundaryPoint.h"
#include "Restriction.h"
#include "BearingPoint.h"
#include "SvgItem.h"
#include "TVAError.h"

#include "qtautotransaction.h"

#include <QtDebug>

#include <QtCore/QVariant>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QSet>

#include "Database.h"
#include <QtSql/QSqlError>
#include <QtCore/QMap>
#include <QtSql/QSqlRecord>


#include "main.h"
#include "Master.h"
#include "redisdb.h"

const QString Dao::MASTER_FPL_TABLE     = "fpl_master";
const QString Dao::MASTER_SFPL_TABLE    = "sfpl_master_table";

Dao::Dao()
    : m_DBID(0)
{

}

Dao::~Dao()
{

}

void Dao::SaveFPL(CFPL& FlightPlan)
{
#if defined(REDIS)
    QSharedPointer<CSFPL> sfpl(new CSFPL(FlightPlan));
    if(master() && master()->getRedisDB())
    {
        if (master()->getRedisDB()->getConnectionStatus() != 1)
            return;
        if(master()->getRedisDB()->saveSFPL(sfpl))
            FlightPlan.SetUid(sfpl->GetUid());
    }
#else
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    // TODO change fpl_master
    query->Prepare(
            "INSERT INTO " + MASTER_FPL_TABLE + "(fir_name, aircraft_id, taircraft, adep, speed, flevel, route, ades) VALUES(:1, :2, :3, :4, :5, :6, :7, :8)",
            "id");
    query->BindValue(":1", FlightPlan.GetFirName());
    query->BindValue(":2", FlightPlan.GetACID());
    query->BindValue(":3", FlightPlan.GetAirplanePtr() ? FlightPlan.GetAirplanePtr()->GetName() : "");
    query->BindValue(":4", FlightPlan.GetDepAerodrome());
    query->BindValue(":5", FlightPlan.GetSpeedS());
    query->BindValue(":6", FlightPlan.GetLevelS());
    query->BindValue(":7", FlightPlan.GetRoute());
    query->BindValue(":8", FlightPlan.GetDestAerodrome());
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    FlightPlan.SetID(query->GetLastInsertID().toUInt());
#endif
}

void Dao::UpdateFPL(CFPL& FlightPlan, bool bAutoTransaction)
{
#if defined(REDIS)
    QSharedPointer<CSFPL> sfpl(new CSFPL(FlightPlan));
    if(master() && master()->getRedisDB())
    {
        if (master()->getRedisDB()->getConnectionStatus() != 1)
            return;
        if(master()->getRedisDB()->saveSFPL(sfpl))
            FlightPlan.SetUid(sfpl->GetUid());
    }
#else
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QtAutoTransaction transaction(QSqlDatabase::database());
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare("UPDATE " + MASTER_FPL_TABLE + " SET equipment = :2, "
                  "route = :3, flevel = :4 "
                  "WHERE id = :1");
    query->BindValue(":1", FlightPlan.GetID());
    query->BindValue(":2", FlightPlan.GetEquipment());
    query->BindValue(":3", FlightPlan.GetRoute());
    query->BindValue(":4", FlightPlan.GetLevelS());
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    transaction.commit();
#endif
}

void Dao::LoadFPLs(QList<CFPL>& FPLList, QString sWhere) const
{
#if defined(REDIS)
    QList<QSharedPointer<CSFPL> > list;
    if(master() && master()->getRedisDB())
    {
        if (master()->getRedisDB()->getConnectionStatus() != 1)
            return;
        if(master()->getRedisDB()->loadSFPLs(list))
        {
            foreach(const QSharedPointer<CSFPL>& sfpl, list)
            {
                CFPL fpl(*sfpl);
                FPLList.append(fpl);
            }
        }
    }
#else
    TAirplaneTypeMap AirList;
    LoadAirplaneTypes(AirList);

    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare(
            "SELECT fir_name, aircraft_id, taircraft, frules, tflight, num, wtc, "
            "equipment, adep, time, speed, flevel, "
            "route, ades, eet, alt1, alt2, other, id "
            "FROM " + MASTER_FPL_TABLE + (sWhere.isEmpty() ? "" : " WHERE "+sWhere));
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    while (query->next()) {
        CFPL FPL;
        FPL.SetFirName(query->value(0).toString());
        FPL.SetACID(query->value(1).toString());
		QSharedPointer<CAirplaneType> pPlaneType;
        TAirplaneTypeMap::iterator itFindPlaneType = AirList.find(query->value(2).toString());
		if(itFindPlaneType != AirList.end())
		{
			pPlaneType = *itFindPlaneType;
		}
		else
		{
			pPlaneType = QSharedPointer<CAirplaneType>(new CAirplaneType);
            pPlaneType->SetName(query->value(2).toString());
		}
        FPL.SetAirplanePtr(pPlaneType);
        FPL.SetFRules(query->value(3).toString());
        FPL.SetTypeFlight(query->value(4).toString());
        FPL.SetNum(query->value(5).toInt());
        FPL.SetTurbulence(query->value(6).toString());
        FPL.SetEquipment(query->value(7).toString());
        FPL.SetDepAerodrome(query->value(8).toString());
        FPL.SetDepTime(query->value(9).toDateTime());
        FPL.SetSpeed(query->value(10).toString());
        FPL.SetLevel(query->value(11).toString());
        FPL.SetRoute(query->value(12).toString());
        FPL.SetDestAerodrome(query->value(13).toString());
        FPL.SetDestTime(query->value(14).toString());
        FPL.SetAlt1Aerodrome(query->value(15).toString());
        FPL.SetAlt2Aerodrome(query->value(16).toString());
        FPL.SetOther(query->value(17).toString());
        FPL.SetID(query->value(18).toUInt());
        FPLList.append(FPL);
    }
#endif
}

QSharedPointer<CFPL> Dao::LoadFPL(quint32 uiID) const
{
#if defined(REDIS)
    return QSharedPointer<CFPL>();
#else
    QSharedPointer < CFPL > pRes(new CFPL());

    TAirplaneTypeMap AirList;
    LoadAirplaneTypes(AirList);

    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare(
            "SELECT fir_name, aircraft_id, taircraft, frules, tflight, num, wtc, "
            "equipment, adep, time, speed, flevel, "
            "route, ades, eet, alt1, alt2, other, id "
            "FROM " + MASTER_FPL_TABLE + " WHERE id = :1");
    query->BindValue(":1", uiID);
    TVA_CHECK(query->exec() && query->next(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    pRes->SetFirName(query->value(0).toString());
    pRes->SetACID(query->value(1).toString());
	QSharedPointer<CAirplaneType> pPlaneType;
    TAirplaneTypeMap::iterator itFindPlaneType = AirList.find(query->value(2).toString());
	if(itFindPlaneType != AirList.end())
	{
		pPlaneType = *itFindPlaneType;
	}
	else
	{
		pPlaneType = QSharedPointer<CAirplaneType>(new CAirplaneType);
        pPlaneType->SetName(query->value(2).toString());
	}
    pRes->SetAirplanePtr(pPlaneType);
    pRes->SetFRules(query->value(3).toString());
    pRes->SetTypeFlight(query->value(4).toString());
    pRes->SetNum(query->value(5).toInt());
    pRes->SetTurbulence(query->value(6).toString());
    pRes->SetEquipment(query->value(7).toString());
    pRes->SetDepAerodrome(query->value(8).toString());
    pRes->SetDepTime(query->value(9).toDateTime());
    pRes->SetSpeed(query->value(10).toString());
    pRes->SetLevel(query->value(11).toString());
    pRes->SetRoute(query->value(12).toString());
    pRes->SetDestAerodrome(query->value(13).toString());
    pRes->SetDestTime(query->value(14).toString());
    pRes->SetAlt1Aerodrome(query->value(15).toString());
    pRes->SetAlt2Aerodrome(query->value(16).toString());
    pRes->SetOther(query->value(17).toString());
    pRes->SetID(query->value(18).toUInt());
    return pRes;
#endif
}


void Dao::LoadAirplaneTypes(TAirplaneTypeMap& AirTypes) const
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare(
            "SELECT name, level_max, bank_max, bank_default FROM airplane_type");
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    QSharedPointer<db::CDBQuery> speedQuery = db::CreateQuery();
    speedQuery->Prepare("SELECT id, flevel, v_climb, v_climb_max, "
                       "v_descend, v_descend_max, ias, ias_min, ias_max "
                       "FROM airplane_speed "
                       "WHERE airplane_id = :1 ORDER BY flevel ASC");
    while (query->next()) {
        speedQuery->BindValue(":1", query->value(0).toString());
        QSharedPointer < CAirplaneType > pType(new CAirplaneType);
        pType->SetName(query->value(0).toString());
        pType->SetLevelMax(query->value(1).toDouble());
        pType->SetBankMax(query->value(2).toInt());
        pType->SetBankDef(query->value(3).toInt());
        TVA_CHECK(speedQuery->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
        while (speedQuery->next()) {
            QSharedPointer < CAirplaneSpeed > pSpeed(new CAirplaneSpeed);
            pSpeed->SetID(speedQuery->value(0).toUInt());
            pSpeed->SetLevel(speedQuery->value(1).toDouble());
            pSpeed->SetVClimb(speedQuery->value(2).toDouble());
            pSpeed->SetVClimbMax(speedQuery->value(3).toDouble());
            pSpeed->SetVDescend(speedQuery->value(4).toDouble());
            pSpeed->SetVDescendMax(speedQuery->value(5).toDouble());
            pSpeed->SetIAS(speedQuery->value(6).toDouble());
            pSpeed->SetIASMin(speedQuery->value(7).toDouble());
            pSpeed->SetIASMax(speedQuery->value(8).toDouble());
            pSpeed->SetAirTypeID(pType->GetName());
            pType->AddSpeed(pSpeed);
        }

        AirTypes[pType->GetName()] = pType;
    }
}

void Dao::LoadSquawk(Scene& Scene) const
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare("SELECT squawk FROM squawk WHERE zone_id = :1");
    query->BindValue(":1", Scene.GetID());
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    while (query->next())
        Scene.AddSquawk(query->value(0).toInt());
}

void Dao::LoadMeteo(uint uiZoneID, IScene &Scene) const
{
    CView* scene = dynamic_cast<CView*>(&Scene);
    if(!scene)
        return;

    scene->DelMeteo(uiZoneID);

    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> dbQuery = db::CreateQuery();
    dbQuery->Prepare("SELECT id, name, type, course, speed, h_min, h_max FROM meteo WHERE zone_id = :1");
    dbQuery->BindValue(":1", uiZoneID);
    QSharedPointer<db::CDBQuery> dbQueryPoints = db::CreateQuery();
    dbQueryPoints->Prepare("SELECT id, sn, latitude, longitude "
        "FROM meteo_point WHERE meteo_id = :1 "
        "ORDER BY sn");

    TVA_CHECK(dbQuery->exec(), (tr("Execution error") + ". %1").arg(dbQuery->lastError().text()), E_FAIL);
    while (dbQuery->next())
    {
        QSharedPointer<core::CMeteo> pMeteo(scene->CreateMeteo());
        pMeteo->SetID(dbQuery->value(0).toUInt());
        pMeteo->SetName(dbQuery->value(1).toString());
        pMeteo->SetType((core::CMeteo::Type) dbQuery->value(2).toInt());

        QVariant course = dbQuery->value(3);
        if (!course.isNull())
            pMeteo->SetCourse(course.toInt());

        QVariant speed = dbQuery->value(4);
        if (!speed.isNull())
            pMeteo->SetSpeed(speed.toInt());

        QVariant heightMin = dbQuery->value(5);
        if (!heightMin.isNull())
            pMeteo->SetHeightMin(heightMin.toInt());

        QVariant heightMax = dbQuery->value(6);
        if (!heightMax.isNull())
            pMeteo->SetHeightMax(heightMax.toInt());

        pMeteo->SetZoneID(uiZoneID);

        dbQueryPoints->BindValue(":1", pMeteo->GetID());
        TVA_CHECK(dbQueryPoints->exec(), (tr("Execution error") + ". %1").arg(
            dbQueryPoints->lastError().text()), E_FAIL);
        while (dbQueryPoints->next())
        {
            QSharedPointer<core::CMeteoPoint> pMeteoPoint(new core::CMeteoPoint());
            pMeteoPoint->SetID(dbQueryPoints->value(0).toUInt());
            pMeteoPoint->SetSN(dbQueryPoints->value(1).toInt());
            pMeteoPoint->SetCoord(CCoord(dbQueryPoints->value(3).toDouble(), dbQueryPoints->value(2).toDouble()));
            pMeteo->AddPoint(pMeteoPoint);
        }
        scene->AddMeteo(pMeteo);
    }
}

void Dao::LoadWinds(Scene& Scene) const
{
    QList < core::CWind > Winds;
    LoadWinds(Winds, Scene.GetID());
    QList<core::CWind>::const_iterator itScan = Winds.begin();
    for (; itScan != Winds.end(); ++itScan) {
        QSharedPointer < core::CWind > pWind(new core::CWind(*itScan));
        Scene.AddWind(pWind);
    }
}

void Dao::LoadPlans(QList<QPair<int, QString> >& list, QString sWhere) const
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QString sSelect("SELECT id, aircraft_id FROM fpl");
    if (!sWhere.isNull())
        sSelect += " WHERE " + sWhere;
    sSelect += " ORDER BY aircraft_id";
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare(sSelect);
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    while (query->next()) {
        list.append(QPair<int, QString> (query->value(0).toInt(),
                                         query->value(1).toString()));
    }
}

void Dao::LoadMasterPlans(QList<QPair<int, QString> >& list, QString sWhere) const
{
#if defined(REDIS)
#else
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QString sSelect("SELECT id, aircraft_id FROM " + MASTER_FPL_TABLE);
    if (!sWhere.isNull())
        sSelect += " WHERE " + sWhere;
    sSelect += " ORDER BY aircraft_id";
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare(sSelect);
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    while (query->next()) {
        list.append(QPair<int, QString> (query->value(0).toInt(),
                                         query->value(1).toString()));
    }
#endif
}

void Dao::LoadWinds(QList<core::CWind> &Winds, quint32 uiZoneID) const
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QString sSelect(
            "SELECT id, zone_id, flevel_start, flevel_end, speed, azimuth "
            "FROM wind "
            "WHERE zone_id = :1 "
            "ORDER BY flevel_start");

    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare(sSelect);
    query->BindValue(":1", uiZoneID);
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);

    while (query->next()) {
        core::CWind Wind;

        Wind.SetID(query->value(0).toUInt());
        Wind.SetZoneID(query->value(1).toUInt());
        Wind.SetFLevelStart(query->value(2).toDouble());
        Wind.SetFLevelEnd(query->value(3).toDouble());
        Wind.SetSpeed(query->value(4).toDouble());
        Wind.SetAzimuth(query->value(5).toDouble());

        Winds.append(Wind);
    }
}

void Dao::SaveWind(core::CWind &Wind)
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QString
            sUpdate(
                    "UPDATE wind "
                    "SET zone_id = :2, flevel_start = :3, flevel_end = :4, speed = :5, azimuth = :6 "
                    "WHERE id = :1");

    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare(sUpdate);
    query->BindValue(":1", Wind.GetID());
    query->BindValue(":2", Wind.GetZoneID());
    query->BindValue(":3", Wind.GetFLevelStart());
    query->BindValue(":4", Wind.GetFLevelEnd());
    query->BindValue(":5", Wind.GetSpeed());
    query->BindValue(":6", Wind.GetAzimuth());
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
}

void Dao::LoadMeteos(QList<core::CMeteo> &Meteos, quint32 uiZoneID)
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QString sSelect("SELECT id, zone_id, name, type "
                    "FROM meteo "
                    "WHERE zone_id = :1 "
                    "ORDER BY id");

    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare(sSelect);
    query->BindValue(":1", uiZoneID);
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);

    while (query->next()) {
        core::CMeteo Meteo;

        Meteo.SetID(query->value(0).toUInt());
        Meteo.SetZoneID(query->value(1).toUInt());
        Meteo.SetName(query->value(2).toString());
        Meteo.SetType((core::CMeteo::Type) query->value(3).toInt());

        Meteos.append(Meteo);
    }
}

void Dao::LoadMeteoPoints(QList<core::CMeteoPoint> &MeteoPoints,
                          quint32 uiMeteoID)
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QString sSelect("SELECT id, meteo_id, latitude, longitude, sn "
                    "FROM meteo_point "
                    "WHERE meteo_id = :1 "
                    "ORDER BY sn");

    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare(sSelect);
    query->BindValue(":1", uiMeteoID);
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);

    while (query->next()) {
        core::CMeteoPoint MeteoPoint;

        MeteoPoint.SetID(query->value(0).toUInt());
        MeteoPoint.SetMeteoID(query->value(1).toUInt());
        MeteoPoint.GetPosition().SetLatitude(CAngle(query->value(2).toDouble()));
        MeteoPoint.GetPosition().SetLongitude(CAngle(query->value(3).toDouble()));
        MeteoPoint.SetSN(query->value(4).toInt());

        MeteoPoints.append(MeteoPoint);
    }
}

void Dao::SaveMeteo(core::CMeteo &Meteo)
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    if (0 == Meteo.GetID()) {
        QString sQuery;
        sQuery = "INSERT INTO meteo (zone_id, name, type) "
                 "VALUES (:2, :3, :4)";
        query->Prepare(sQuery, "Id");
    } else {
        QString sQuery;
        sQuery = "UPDATE meteo "
                 "SET zone_id = :2, name = :3, type = :4 "
                 "WHERE id = :1";
        query->Prepare(sQuery);
    }

    query->BindValue(":1", Meteo.GetID());
    query->BindValue(":2", Meteo.GetZoneID());
    query->BindValue(":3", Meteo.GetName());
    query->BindValue(":4", (int) Meteo.GetType());

    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);

    if (0 == Meteo.GetID())
        Meteo.SetID(query->GetLastInsertID().toUInt());
}

void Dao::SaveMeteoPoint(core::CMeteoPoint &MeteoPoint)
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    if (0 == MeteoPoint.GetID()) {
        QString sQuery;
        sQuery = "INSERT INTO meteo_point (meteo_id, latitude, longitude, sn) "
                 "VALUES (:2, :3, :4, :5)";
        query->Prepare(sQuery, "Id");
    } else {
        QString sQuery;
        sQuery = "UPDATE meteo_point "
                 "SET meteo_id = :2, latitude = :3, longitude = :4, sn = :5 "
                 "WHERE id = :1";
        query->Prepare(sQuery);
    }

    query->BindValue(":1", MeteoPoint.GetID());
    query->BindValue(":2", MeteoPoint.GetMeteoID());
    query->BindValue(":3", MeteoPoint.GetPosition().GetLatitude().GetAngle());
    query->BindValue(":4", MeteoPoint.GetPosition().GetLongitude().GetAngle());
    query->BindValue(":5", MeteoPoint.GetSN());

    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);

    if (0 == MeteoPoint.GetID())
        MeteoPoint.SetID(query->GetLastInsertID().toUInt());
}

void Dao::DeleteMeteo(core::CMeteo &Meteo)
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QtAutoTransaction transaction(QSqlDatabase::database());

    {
        QString sDelete = "DELETE FROM meteo WHERE id = :1";

        QSharedPointer<db::CDBQuery> query = db::CreateQuery();
        query->Prepare(sDelete);
        query->BindValue(":1", Meteo.GetID());

        TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    }

    {
        QString sDelete = "DELETE FROM meteo_point WHERE meteo_id = :1";

        QSharedPointer<db::CDBQuery> query = db::CreateQuery();
        query->Prepare(sDelete);
        query->BindValue(":1", Meteo.GetID());

        TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    }

    transaction.commit();
}

void Dao::DeleteMeteoPoint(core::CMeteoPoint &MeteoPoint)
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QString sDelete = "DELETE FROM meteo_point WHERE id = :1";

    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare(sDelete);
    query->BindValue(":1", MeteoPoint.GetID());

    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
}

void Dao::DeleteFIRMeteo(QString sIdFir)
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QtAutoTransaction transaction(QSqlDatabase::database());
    QString sDelete = "DELETE FROM meteo WHERE zone_id = (SELECT id FROM zone WHERE type = 1 AND ID_NAME = :1)";
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare(sDelete);
    query->BindValue(":1", sIdFir);
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    transaction.commit();
}

void Dao::InsertMeteo(QSharedPointer<core::CMeteo> &Meteo)
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QtAutoTransaction transaction(QSqlDatabase::database());
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    QString sQuery;
    sQuery = "INSERT INTO meteo (zone_id, name, type) "
             "VALUES (:2, :3, :4)";
    query->Prepare(sQuery, "Id");
    query->BindValue(":2", Meteo->GetZoneID());
    query->BindValue(":3", Meteo->GetName());
    query->BindValue(":4", (int) Meteo->GetType());

    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    Meteo->SetID(query->GetLastInsertID().toUInt());

    QSharedPointer<db::CDBQuery> queryPoint = db::CreateQuery();
    sQuery = "INSERT INTO meteo_point (meteo_id, latitude, longitude, sn) "
             "VALUES (:2, :3, :4, :5)";
    queryPoint->Prepare(sQuery, "Id");
    core::CMeteo::TPointList::iterator itPoint = Meteo->GetPoints().begin();
    for(; itPoint != Meteo->GetPoints().end(); ++itPoint)
    {
        queryPoint->BindValue(":2", Meteo->GetID());
        (*itPoint)->SetMeteoID(Meteo->GetID());
        queryPoint->BindValue(":3", (*itPoint)->GetPosition().GetLatitude().GetAngle());
        queryPoint->BindValue(":4", (*itPoint)->GetPosition().GetLongitude().GetAngle());
        queryPoint->BindValue(":5", (*itPoint)->GetSN());

        TVA_CHECK(queryPoint->exec(), tr("Execution error. %1").arg(queryPoint->lastError().text()), E_FAIL);
        (*itPoint)->SetID(queryPoint->GetLastInsertID().toUInt());
    }

    transaction.commit();
}

void Dao::SaveBearing(const QString sWorkstation, const QString sACID,
                      bool bOnlyDelete)
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QtAutoTransaction transaction(QSqlDatabase::database());
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare("DELETE FROM bearing WHERE workstation = :1");
    query->BindValue(":1", sWorkstation);
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);

    if (!bOnlyDelete) {
        query->Prepare("INSERT INTO bearing(workstation, acid) VALUES(:1, :2)");
        query->BindValue(":1", sWorkstation);
        query->BindValue(":2", sACID);
        TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    }
    transaction.commit();
}

void Dao::SaveExerciseTimeAndOP(quint32 exerciseID, quint32 fplID,
                                quint32 seconds, QString sOP, bool bAutoTransaction/* = true*/)
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QtAutoTransaction transaction(QSqlDatabase::database());
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare("UPDATE exercise_plane SET start_time = :3, OP = :4 "
                  "WHERE id_exercise = :1 AND id_plane = :2");
    query->BindValue(":1", exerciseID);
    query->BindValue(":2", fplID);
    query->BindValue(":3", seconds);
    query->BindValue(":4", sOP);
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    transaction.commit();
}

void Dao::GetBearingPoint(CBearingPoint& bearing)
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare(
            "SELECT rdf_latitude, rdf_longitude, id FROM bearing_point "
            "WHERE airport_id = :1 AND sac = :2 AND sic = :3");
    //query->BindValue(":1", bearing.GetFIR_ID_NAME());
    query->BindValue(":1", bearing.GetAirportID());
    query->BindValue(":2", bearing.GetSAC());
    query->BindValue(":3", bearing.GetSIC());
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    if (query->next())
    {
        bearing.SetCoord(CCoord(query->value(1).toDouble(), query->value(0).toDouble()));
        bearing.SetID(query->value(2).toUInt());
    }
}

void Dao::LoadBearingList(const QString& sFIR, QList<CBearingPoint>& list)
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
     query->Prepare(
             "SELECT id, name, fir_name, rdf_latitude, rdf_longitude, sac, sic, channel, declination FROM bearing_point "
             "WHERE fir_name = :1");
     query->BindValue(":1", sFIR);
     TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
     while (query->next())
     {
         CBearingPoint bearing;
         bearing.SetID(query->value(0).toUInt());
         bearing.SetName(query->value(1).toString());
         bearing.SetFIR_ID_NAME(query->value(2).toString());
         bearing.SetCoord(CCoord(query->value(4).toDouble(), query->value(3).toDouble()));
         bearing.SetSAC(query->value(5).toUInt());
         bearing.SetSIC(query->value(6).toUInt());
         bearing.SetChannel(query->value(7).toUInt());
         bearing.SetDeclination(query->value(8).toUInt());

         list.append(bearing);
     }
}

void Dao::LoadBearingList(quint32& idAirport, QList<CBearingPoint>& list)
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare(
            "SELECT id, name, rdf_latitude, rdf_longitude, sac, sic, channel, declination, airport_id FROM bearing_point "
            "WHERE airport_id = :1");
    query->BindValue(":1", idAirport);
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    while (query->next())
    {
        CBearingPoint bearing;
        bearing.SetID(query->value(0).toUInt());
        bearing.SetName(query->value(1).toString());
        //bearing.SetFIR_ID_NAME(query->value(2).toString());
        bearing.SetCoord(CCoord(query->value(3).toDouble(), query->value(2).toDouble()));
        bearing.SetSAC(query->value(4).toUInt());
        bearing.SetSIC(query->value(5).toUInt());
        bearing.SetChannel(query->value(6).toUInt());
        bearing.SetDeclination(query->value(7).toUInt());
        bearing.SetAirportID(query->value(8).toUInt());

        list.append(bearing);
    }
}

void Dao::ActivatePlane(const QVector<QVariant>& bindValues)
{
    if (bindValues.size() < 5)
        return;
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QtAutoTransaction transaction(QSqlDatabase::database());
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare("DELETE FROM activation WHERE acid = :1 AND code = :2");
    query->BindValue(":1", bindValues[0]);
    query->BindValue(":2", bindValues[1]);
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);

    query->Prepare("INSERT INTO activation"
                  "(acid, code, first_point, start_time, echelon) "
                  "VALUES(:1, :2, :3, :4, :5)");
    for (int i = 1; i <= bindValues.size(); ++i)
        query->BindValue(QString::number(i), bindValues[i - 1]);
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    transaction.commit();
}

void Dao::ClearActivate()
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QtAutoTransaction transaction(QSqlDatabase::database());
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare("DELETE FROM activation");
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    transaction.commit();
}

void Dao::GetSysdate(QDateTime& dt)
{
    TVA_CATCHALL_TRY
    {
        QSharedPointer<db::CDBQuery> query = db::CreateQuery();
        if(DBConnectionsManager::instance().getConnectionStatus() != 1)
            return;
        query->Prepare("SELECT " + query->GetSystimestampStr() + " " + query->GetFromDualStr());
        TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
        if (query->next()) {
            dt = query->value(0).toDateTime();
        }
    }TVA_CATCHALL(err) { LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err); }
}

quint32 Dao::GetDBID()
{
    TVA_CATCHALL_TRY
    {
        if(!m_DBID)
        {
            QSharedPointer<db::CDBQuery> query = db::CreateQuery();
            QString sSelect = query->GetSelectDbId();
            if(!sSelect.isEmpty())
            {
                query->Prepare(sSelect);
                TVA_CHECK(query->exec() && query->next(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
                m_DBID = query->value(0).toUInt();
            }
        }
    }TVA_CATCHALL(err) { LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err); }
    return m_DBID;
}

void Dao::InsertSquawk(quint32 zoneID, quint32 qSquawkFrom, quint32 qSquawkTo)
{
    if (qSquawkFrom > qSquawkTo)
        return;
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QtAutoTransaction transaction(QSqlDatabase::database());
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare("DELETE FROM squawk WHERE zone_id = :1");
    query->BindValue(":1", zoneID);
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    query->Prepare("INSERT INTO squawk(zone_id, squawk) "
                  "VALUES(:1, :2)");
    query->BindValue(":1", zoneID);
    for (quint32 i = qSquawkFrom; i <= qSquawkTo; ++i) {
        query->BindValue(":2", i);
        TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    }
    transaction.commit();
}

void Dao::InsertWind(quint32 zoneID, quint32 qWindStep, quint32 qWindSpeed,
                     quint32 qWindAzimuth)
{
    if (qWindStep == 0)
        return;
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    const quint32 maxLevel = 15000;
    QtAutoTransaction transaction(QSqlDatabase::database());
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare("DELETE FROM wind WHERE zone_id = :1");
    query->BindValue(":1", zoneID);
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    query->Prepare(
            "INSERT INTO wind(zone_id, flevel_start, flevel_end, speed, azimuth) "
            "VALUES(:1, :2, :3, :4, :5)");
    query->BindValue(":1", zoneID);
    for (quint32 i = 0; i < maxLevel; i += qWindStep) {
        query->BindValue(":2", i);
        query->BindValue(":3", i + qWindStep > maxLevel ? maxLevel : i + qWindStep);
        query->BindValue(":4", qWindSpeed);
        query->BindValue(":5", qWindAzimuth);
        TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    }
    transaction.commit();
}

void Dao::InsertSFPL(const QSharedPointer<CSFPL>& pSFPL)
{
#if defined(REDIS)
    if(master() && master()->getRedisDB())
    {
        if (master()->getRedisDB()->getConnectionStatus() != 1)
            return;
        master()->getRedisDB()->saveSFPL(pSFPL);
    }
#else
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QtAutoTransaction transaction(QSqlDatabase::database());
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare(
            "INSERT INTO "+MASTER_SFPL_TABLE+" (id, fir_name, aircraft_id, taircraft, frules, tflight, num, wtc, equipment, adep, time, speed, flevel, route, ades, eet, alt1, alt2, other, trtime, "
            "code, sectors, ins, cs, ns, state, copin, time_entry, copinfl) "
            "(SELECT id, fir_name, aircraft_id, taircraft, frules, tflight, num, wtc, equipment, adep, time, speed, flevel, route, ades, eet, alt1, alt2, other, trtime, "
            ":2, :3, :4, :5, :6, 0, :7, to_date(:8, 'HH24:MI'), :9 "
            "FROM "+MASTER_FPL_TABLE+" WHERE id = :1)");
    query->BindValue(":1", pSFPL->GetID());
    query->BindValue(":2", pSFPL->GetCODE());
    query->BindValue(":3", pSFPL->GetAllSectors());
    query->BindValue(":4", pSFPL->GetInSector());
    query->BindValue(":5", pSFPL->GetCurSector());
    query->BindValue(":6", pSFPL->GetNextSector());
    query->BindValue(":7", pSFPL->GetCOPin());
    query->BindValue(":8", pSFPL->GetETOin().toString("hh:mm"));
    query->BindValue(":9", pSFPL->GetCOPinFL().GetF());
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    query->Prepare("UPDATE "+MASTER_SFPL_TABLE+" SET trtime = " + query->GetSystimestampStr() + " WHERE id = :1");
    query->BindValue(":1", pSFPL->GetID());
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    query->Prepare("UPDATE "+MASTER_FPL_TABLE+" SET visibility = 0 WHERE id = :1");
    query->BindValue(":1", pSFPL->GetID());
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    transaction.commit();
#endif
}

void Dao::UpdateSFPL(const QSharedPointer<CSFPL>& pSFPL)
{
#if defined(REDIS)
    if(master() && master()->getRedisDB())
    {
        if (master()->getRedisDB()->getConnectionStatus() != 1)
            return;
        master()->getRedisDB()->saveSFPL(pSFPL);
    }
#else
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QtAutoTransaction transaction(QSqlDatabase::database());
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare("UPDATE "+MASTER_SFPL_TABLE+" SET "
                  "fir_name = :0, aircraft_id = :1, taircraft = :2, frules = :3, tflight = :4, num = :5, "
                  "wtc = :6, equipment = :7, adep = :8, time = :9, speed = :10, flevel = :11, route = :12, "
                  "ades = :13, eet = :14, alt1 = :15, alt2 = :16, other = :17, "
                  "code = :19, sectors = :20, ins = :21, cs = :22, ns = :23, "
                  "copin = :24, time_entry = to_date(:25, 'HH24:MI'), copinfl = :26 "
                  "WHERE id = :18");
    _BindSFPLValues(pSFPL, query);
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    transaction.commit();
#endif
}

void Dao::UndoSFPL(quint32 idSFPL)
{
#if defined(REDIS)
#else
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QtAutoTransaction transaction(QSqlDatabase::database());
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare("UPDATE " + MASTER_FPL_TABLE + " SET visibility = 1 WHERE id = :1");
    query->BindValue(":1", idSFPL);
    TVA_CHECK(query->exec(), (tr("Execution error. %1")).arg(query->lastError().text()), E_FAIL);
    query->Prepare("DELETE FROM "+ MASTER_SFPL_TABLE + " WHERE id = :1");
    query->BindValue(":1", idSFPL);
    TVA_CHECK(query->exec(), (tr("Execution error. %1")).arg(query->lastError().text()), E_FAIL);
    transaction.commit();
#endif
}

void Dao::LoadSFPLs(QList<QSharedPointer<CSFPL> >& SFPLList, quint32 idSFPL)
{
    TAirplaneTypeMap AirList;
    if(DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    LoadAirplaneTypes(AirList);

#if defined(REDIS)
    if(master() && master()->getRedisDB())
    {
        master()->getRedisDB()->loadSFPLs(SFPLList);

//        for (int i = 0; i < SFPLList.size(); ++i)
//        {
//            TAirplaneTypeMap::iterator itFindPlaneType = AirList.find(SFPLList[i]->GetAirplanePtr()->GetName());
//            if (itFindPlaneType != AirList.end())
//            {
//                SFPLList[i]->SetAirplanePtr(*itFindPlaneType);
//            }
//            else
//            {
//                itFindPlaneType = AirList.find("B737");
//                if (itFindPlaneType != AirList.end())
//                {
//                    SFPLList[i]->SetAirplanePtr(*itFindPlaneType);
//                }
//            }
//        }
    }
#else
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare(
            "SELECT fir_name, aircraft_id, taircraft, frules, tflight, num, wtc, "
            "equipment, adep, time, speed, flevel, "
            "route, ades, eet, alt1, alt2, other, id, "
            "code, sectors, ins, cs, ns, copin, time_entry, copinfl "
            "FROM "+MASTER_SFPL_TABLE+
            (idSFPL ? " WHERE id = :1" : ""));
    query->BindValue(":1", idSFPL);
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    while (query->next()) {
        QSharedPointer < CSFPL > pSFPL(new CSFPL);
        pSFPL->SetFirName(query->value(0).toString());
        pSFPL->SetACID(query->value(1).toString());
		QSharedPointer<CAirplaneType> pPlaneType;
        TAirplaneTypeMap::iterator itFindPlaneType = AirList.find(query->value(2).toString());
		if(itFindPlaneType != AirList.end())
		{
			pPlaneType = *itFindPlaneType;
		}
		else
		{
			pPlaneType = QSharedPointer<CAirplaneType>(new CAirplaneType);
            pPlaneType->SetName(query->value(2).toString());
		}
        pSFPL->SetAirplanePtr(pPlaneType);
        pSFPL->SetFRules(query->value(3).toString());
        pSFPL->SetTypeFlight(query->value(4).toString());
        pSFPL->SetNum(query->value(5).toInt());
        pSFPL->SetTurbulence(query->value(6).toString());
        pSFPL->SetEquipment(query->value(7).toString());
        pSFPL->SetDepAerodrome(query->value(8).toString());
        pSFPL->SetDepTime(query->value(9).toDateTime());
        pSFPL->SetSpeed(query->value(10).toString());
        pSFPL->SetLevel(query->value(11).toString());
        pSFPL->SetRoute(query->value(12).toString());
        pSFPL->SetDestAerodrome(query->value(13).toString());
        pSFPL->SetDestTime(query->value(14).toString());
        pSFPL->SetAlt1Aerodrome(query->value(15).toString());
        pSFPL->SetAlt2Aerodrome(query->value(16).toString());
        pSFPL->SetOther(query->value(17).toString());
        pSFPL->SetID(query->value(18).toUInt());

        pSFPL->SetCODE(query->value(19).toUInt());
        pSFPL->SetAllSectors(query->value(20).toString());
        pSFPL->SetInSector(query->value(21).toString());
        pSFPL->SetCurSector(query->value(22).toString());
        pSFPL->SetNextSector(query->value(23).toString());
        pSFPL->SetCOPin(query->value(24).toString());
        pSFPL->SetETOin(query->value(25).toTime());
        pSFPL->SetCOPinFL(query->value(26).toDouble());
        SFPLList.append(pSFPL);
    }
#endif
}

void Dao::MoveSFPLinRPL(const QSharedPointer<CSFPL>& pSFPL)
{
#if defined(REDIS)
#else
    if(pSFPL)
    {
        if (DBConnectionsManager::instance().getConnectionStatus() != 1)
            return;
        QtAutoTransaction transaction(QSqlDatabase::database());
        QSharedPointer<db::CDBQuery> query = db::CreateQuery();
        // insert in RPL
        // Pasha ???
        query->Prepare("DELETE FROM "+ MASTER_SFPL_TABLE + " WHERE id = :1");
        query->BindValue(":1", pSFPL->GetID());
        TVA_CHECK(query->exec(), (tr("Execution error. %1")).arg(query->lastError().text()), E_FAIL);
        transaction.commit();
    }
#endif
}

QSharedPointer<CUser> Dao::LoadUser(const QString& sUser)
{
    QSharedPointer < CUser > pUser(new CUser());
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return pUser;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare(
            "SELECT client.id, client.password, client.role_id, client_role.name, client_role.permission, client.full_name "
            "FROM client, client_role where client.name = :1 AND client_role.id = client.role_id");
    query->BindValue(":1", sUser);
    TVA_CHECK(query->exec() && query->next(), tr("Wrong user name or/and password"), E_FAIL)

    pUser->SetID(query->value(0).toUInt());
    pUser->SetName(sUser);
    pUser->SetPassHash(query->value(1).toByteArray());
    pUser->SetRole(query->value(2).toUInt());
    pUser->SetFullName(query->value(5).toString());
    QSharedPointer < CUserRole > pUserRole(new CUserRole());
    pUserRole->SetID(query->value(2).toInt());
    pUserRole->SetName(query->value(3).toString());
    pUserRole->SetPermission(query->value(4).toString());
    pUser->SetRoleObj(pUserRole);
    return pUser;
}

void Dao::LoadAllUsers(TUserMap& UserMap)
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare("SELECT id, name, password, role_id, full_name FROM client");
    TVA_CHECK(query->exec(), tr("Wrong user name or/and password"), E_FAIL);
    UserMap.clear();
    while (query->next()) {
        QSharedPointer < CUser > pUser(new CUser());
        pUser->SetID(query->value(0).toUInt());
        pUser->SetName(query->value(1).toString());
        pUser->SetPassHash(query->value(2).toByteArray());
        pUser->SetRole(query->value(3).toUInt());
        pUser->SetFullName(query->value(4).toString());
        UserMap[pUser->GetName()] = pUser;
    }
}

void Dao::SaveAllUsers(const TUserList& UserList)
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QtAutoTransaction transaction(QSqlDatabase::database());
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    QSharedPointer<db::CDBQuery> sqlUpdate = db::CreateQuery();
    sqlUpdate->Prepare(
            "UPDATE client SET name = :1, password = :2, role_id = :3, full_name = :4 WHERE id = :5");
    QSharedPointer<db::CDBQuery> sqlInsert = db::CreateQuery();
    sqlInsert->Prepare(
            "INSERT INTO client(name, password, role_id, full_name) VALUES(:1, :2, :3, :4)",
            "id");

    TUserList::const_iterator itUser = UserList.begin();
    for (; itUser != UserList.end(); ++itUser) {
        // Insert or update information
        if ((*itUser)->GetID() == 0) {
            sqlInsert->BindValue(":1", (*itUser)->GetName());
            sqlInsert->BindValue(":2", (*itUser)->GetPassHash());
            sqlInsert->BindValue(":3", (*itUser)->GetRole());
            sqlInsert->BindValue(":4", (*itUser)->GetFullName());
            TVA_CHECK(sqlInsert->exec(), tr("Execution error. %1").arg(sqlInsert->lastError().text()), E_FAIL);
        } else {
            sqlUpdate->BindValue(":1", (*itUser)->GetName());
            sqlUpdate->BindValue(":2", (*itUser)->GetPassHash());
            sqlUpdate->BindValue(":3", (*itUser)->GetRole());
            sqlUpdate->BindValue(":4", (*itUser)->GetFullName());
            sqlUpdate->BindValue(":5", (*itUser)->GetID());
            TVA_CHECK(sqlUpdate->exec(), tr("Execution error. %1").arg(sqlUpdate->lastError().text()), E_FAIL);
        }
        if ((*itUser)->GetID() == 0)
            (*itUser)->SetID(query->GetLastInsertID().toUInt());
    }
    // Remove extra records
    if (!UserList.isEmpty()) {
        QString sDelete("DELETE FROM client WHERE id NOT IN(");
        itUser = UserList.begin();
        for (; itUser != UserList.end(); ++itUser) {
            if (itUser != UserList.begin())
                sDelete += ", ";
            sDelete += QString::number((*itUser)->GetID());
        }
        query->Prepare(sDelete + ")");
        TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    }

    transaction.commit();
}

void Dao::LoadUserRoles(TUserRoleList& UserRoles)
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare("SELECT id, name, permission FROM client_role");
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    UserRoles.clear();
    while (query->next()) {
        QSharedPointer < CUserRole > pUserRole(new CUserRole());
        pUserRole->SetID(query->value(0).toUInt());
        pUserRole->SetName(query->value(1).toString());
        pUserRole->SetPermission(query->value(2).toString());
        UserRoles[pUserRole->GetID()] = pUserRole;
    }
}

void Dao::LoadSectorZoneAll(TSectorZoneList& SectorZoneList)
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare("SELECT id, sect_id, zone_id, main_fir FROM sectoring_zone");
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    SectorZoneList.clear();
    while (query->next()) {
        QSharedPointer < CSectorZone > pSectorZone(new CSectorZone);
        pSectorZone->SetID(query->value(0).toUInt());
        pSectorZone->SetSectoringID(query->value(1).toUInt());
        pSectorZone->SetZoneID(query->value(2).toUInt());
        pSectorZone->SetMainFIR(query->value(3).toInt() != 0);
        SectorZoneList.insert(pSectorZone->GetSectoringID(), pSectorZone);
    }
}

void Dao::SaveSectorZoneAll(const TSectorZoneList& SectorZoneList)
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QtAutoTransaction transaction(QSqlDatabase::database());
    TSectoringList SectoringList;
    LoadSectoringAll(SectoringList);

    QSharedPointer<db::CDBQuery> query = db::CreateQuery();

    query->Prepare("DELETE FROM sectoring_zone");
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    query->Prepare(
            "INSERT INTO sectoring_zone (sect_id, zone_id, main_fir) VALUES(:1, :2, :3)",
            "id");
    TSectorZoneList::const_iterator itSZ = SectorZoneList.begin();
    for (; itSZ != SectorZoneList.end(); ++itSZ) {
        if (SectoringList.find(itSZ.value()->GetSectoringID())
            != SectoringList.end()) {
            query->BindValue(":1", itSZ.value()->GetSectoringID());
            query->BindValue(":2", itSZ.value()->GetZoneID());
            query->BindValue(":3", (itSZ.value()->GetMainFIR() == true) ? 1 : 0);
            TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
        }
    }
    transaction.commit();
}

void Dao::LoadUserSectorAll(TUserSectoringList& UserSectorList)
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare("SELECT client_role_id, sectoring_id FROM client_sectoring");
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    UserSectorList.clear();
    while (query->next())
        UserSectorList.insert(query->value(0).toUInt(), query->value(1).toUInt());
}

void Dao::SaveUserSectorAll(const TUserSectoringList& UserSectorList)
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QtAutoTransaction transaction(QSqlDatabase::database());
    TUserRoleList GroupMap;
    LoadUserRoles(GroupMap);
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();

    query->Prepare("DELETE FROM client_sectoring");
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    query->Prepare(
            "INSERT INTO client_sectoring (client_role_id, sectoring_id) VALUES(:1, :2)",
            "id");
    TUserSectoringList::const_iterator itSZ = UserSectorList.begin();
    for (; itSZ != UserSectorList.end(); ++itSZ) {
        if (GroupMap.find(itSZ.key()) != GroupMap.end()) {
            query->BindValue(":1", itSZ.key());
            query->BindValue(":2", itSZ.value());
            TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
        }
    }
    transaction.commit();
}

void Dao::LoadSectorIdName(quint32 idZone, QString* name)
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare("SELECT ID_NAME FROM ZONE where ID=:1");
    query->BindValue(":1", idZone);
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    name->clear();
    if(query->next())
        *name = query->value(0).toString();
}

void Dao::LoadIdFIREnvironment(uint uiSectoringID, QList<uint>& IdFIRList)
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare(
            "SELECT sectoring_zone.zone_id "
            "FROM sectoring_zone, zone "
            "WHERE sectoring_zone.sect_id = :1 AND zone.id = sectoring_zone.zone_id AND zone.type = 1 "
            "ORDER BY main_fir DESC");
    query->BindValue(":1", uiSectoringID);
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    IdFIRList.clear();
    while (query->next())
        IdFIRList.append(query->value(0).toUInt());
}

void Dao::LoadIdSectorsEnvironment(uint uiSectoringID, uint uiIdFIR, QList<uint> &sectors)
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare(
            "SELECT sectoring_zone.zone_id "
            "FROM sectoring_zone, zone "
            "WHERE sectoring_zone.sect_id = :1 AND zone.id = sectoring_zone.zone_id AND zone.type = 2 "
            "AND zone.parent_id = :2 "
			"ORDER BY sectoring_zone.main_fir DESC");
    query->BindValue(":1", uiSectoringID);
    query->BindValue(":2", uiIdFIR);
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    sectors.clear();
    while (query->next())
        sectors.append(query->value(0).toUInt());
}

QString Dao::LoadSectoringTypePrivilege(uint uiSectoringID)
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return QString();
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare("SELECT sectoring_type.permission  "
                  "FROM sectoring_type, sectoring "
                  "WHERE sectoring_type.id = sectoring.type_id AND sectoring.id = :1");
    query->BindValue(":1", uiSectoringID);
    TVA_CHECK(query->exec() && query->next(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    return query->value(0).toString();
}

QString Dao::LoadSectoringType(uint uiSectoringID)
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return QString();
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare("SELECT sectoring_type.name  "
                  "FROM sectoring_type, sectoring "
                  "WHERE sectoring_type.id = sectoring.type_id AND sectoring.id = :1");
    query->BindValue(":1", uiSectoringID);
    TVA_CHECK(query->exec() && query->next(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    return query->value(0).toString();
}

void Dao::LoadSectoringAll(TSectoringList& SectoringList)
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare("SELECT id, name "
                  "FROM sectoring");
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    SectoringList.clear();
    while (query->next()) {
        TSectoringPtr pSectoring(new CSectoring());
        pSectoring->SetID(query->value(0).toUInt());
        pSectoring->SetName(query->value(1).toString());
        SectoringList[pSectoring->GetID()] = pSectoring;
    }
}

void Dao::LoadSvg(Scene& scene) const
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare(
            "SELECT id, latitude_start, longitude_start, latitude_finish, longitude_finish, filename, name, code, firname FROM svg_items");
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    while (query->next())
    {
        QString firname = query->value(8).toString();
        //qDebug() << scene.GetName();
        if(scene.GetName() != firname)
        {
            continue;
        }
        QString fileName = QApplication::applicationDirPath() + "/../share/images/";
        fileName += query->value(5).toString();
        QFile file(fileName);
        if(file.exists())
        {
            int id = query->value(0).toInt();
            QString code = query->value(7).toString();
            QString name = query->value(6).toString();

            QSharedPointer < SvgItem > pSvgItem(scene.CreateSvgItem(fileName));
            pSvgItem->SetPath(fileName);
            pSvgItem->SetSvgItemID(id);
            CAngle Longitude1;
            Longitude1.SetAngle(query->value(2).toDouble());
            CAngle Latitude1;
            Latitude1.SetAngle(query->value(1).toDouble());
            CCoord coord1(Longitude1, Latitude1);
            CAngle Longitude2;
            Longitude2.SetAngle(query->value(4).toDouble());
            CAngle Latitude2;
            Latitude2.SetAngle(query->value(3).toDouble());
            CCoord coord2(Longitude2, Latitude2);
            pSvgItem->SetCoordStart(coord1);
            pSvgItem->SetCoordFinish(coord2);
            pSvgItem->SetName(name);
            pSvgItem->SetCode(code);
            if(!scene.IsAirportExistAndSvgAdded(code, name, pSvgItem))
            {
                scene.AddSvgItem(pSvgItem);
            }

        }
    }
}

void Dao::Load(uint uiZoneID, Scene& scene)
{
    LoadZone(uiZoneID, scene);
    // Points
    LoadPoints(scene);
    // Routes
    LoadRoutes(scene);
    // Airports
    LoadAirports(scene);
    // Restrictions
    LoadRestrictions(scene);
    LoadUserRestrictions(scene);
    // Holdings
    LoadHoldings(scene);
    // Map notes
    LoadMapNotes(scene);
    // User pics
    LoadUserPics(scene);
    // Svg items
    LoadSvg(scene);
}

void Dao::Save(Scene &scene)
{
    SaveMapNotes(scene);
}

void Dao::LoadPoints(Scene &scene) const
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    // Points
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare(
            "SELECT point.id, name, longitude, latitude, icao, type, airport, zone_point.is_fir_border, zone_point.is_sector_border "
            "FROM point, zone_point "
            "WHERE zone_point.point_id = point.id AND zone_id = :1 AND type != 4");
    // Boundary points
    query->BindValue(":1", scene.GetID());
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    while (query->next()) {
        QSharedPointer < CPoint > pPoint(scene.CreatePoint(
                (CPoint::Type) query->value(5).toInt()));
        pPoint->SetID(query->value(0).toUInt());
        pPoint->SetName(query->value(1).toString());
        pPoint->SetCoord(CCoord(query->value(2).toDouble(),
                                query->value(3).toDouble()));
        pPoint->SetICAO(query->value(4).toString());
        pPoint->SetType((CPoint::Type) query->value(5).toInt());
        pPoint->SetAirportName(query->value(6).toString());
        pPoint->SetFirBorder(query->value(7).toBool());
        pPoint->SetSectorBorder(query->value(8).toBool());

        scene.AddPoint(pPoint);
    }
}

void Dao::LoadRoutes(Scene& Scene) const
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare("SELECT id, name FROM route WHERE zone_id = :1");
    // Zone points
    QSharedPointer<db::CDBQuery> SqlRoutPoints = db::CreateQuery();
    SqlRoutPoints->Prepare(
            "SELECT route_point.id, sn, point.name, point.type, point.icao, end_flag FROM route_point, point WHERE route_point.point_id = point.id AND route_id = :1");

    query->BindValue(":1", Scene.GetID());
    query->exec();
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    while (query->next()) {
        QSharedPointer < CRoute > pRoute(Scene.CreateRoute());
        pRoute->SetZoneID(Scene.GetID());
        pRoute->SetID(query->value(0).toUInt());
        pRoute->SetName(query->value(1).toString());

        SqlRoutPoints->BindValue(":1", pRoute->GetID());
        SqlRoutPoints->exec();
        TVA_CHECK(SqlRoutPoints->exec(), tr("Execution error. %1").arg(SqlRoutPoints->lastError().text()), E_FAIL);
        while (SqlRoutPoints->next()) {
            QSharedPointer < CRoutePoint > pRoutePoint(new CRoutePoint);
            pRoutePoint->SetRoutID(pRoute->GetID());
            pRoutePoint->SetID(SqlRoutPoints->value(0).toUInt());
            pRoutePoint->SetSN(SqlRoutPoints->value(1).toInt());
            Scene::TPointsMap::const_iterator itFind = Scene.GetPoints().find(
                    CPointKey(SqlRoutPoints->value(2).toString(),
                              (CPoint::Type) SqlRoutPoints->value(3).toInt(),
                              SqlRoutPoints->value(4).toString()));
            pRoutePoint->SetPoint(*itFind);
            pRoutePoint->SetEnd(SqlRoutPoints->value(5).toBool());
            pRoute->AddRoutPoint(pRoutePoint);
        }
        Scene.AddRoute(pRoute);
    }
}

void Dao::LoadAirports(Scene& Scene) const
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare("SELECT airport.id, name, longitude, latitude, icao, altitude, declination "
                  "FROM airport, zone_airport "
                  "WHERE zone_airport.airport_id = airport.id AND zone_id = :1");
    query->BindValue(":1", Scene.GetID());
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    while (query->next()) {
        QSharedPointer < CAirport > pAirport(Scene.CreateAirport());
        pAirport->SetID(query->value(0).toUInt());
        pAirport->SetName(query->value(1).toString());
        pAirport->SetCoord(CCoord(query->value(2).toDouble(),
                                  query->value(3).toDouble()));
        pAirport->SetICAO(query->value(4).toString());
        pAirport->SetAlt(query->value(5).toDouble());
        pAirport->SetDeclination(query->value(6).toString());
        LoadRunways(Scene, *pAirport);
        Scene.AddAirport(pAirport);
    }
}

void Dao::LoadRunways(const Scene& Scene, CAirport& Airport) const
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare(
            "SELECT id, name, longitude, latitude, length, azimuth, width FROM runway WHERE airport_id = :1");
    query->BindValue(":1", Airport.GetID());
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    while (query->next()) {
        QSharedPointer < CRunway > pRunway(Scene.CreateRunway());
        pRunway->SetParent(&Airport);
        pRunway->SetID(query->value(0).toUInt());
        pRunway->SetName(query->value(1).toString());
        pRunway->SetCoord(CCoord(query->value(2).toDouble(),
                                 query->value(3).toDouble()));
        pRunway->SetLength(query->value(4).toInt());
        pRunway->SetAzimuth(query->value(5).toDouble());
        pRunway->SetWidth(query->value(6).toDouble());
        pRunway->SetAirportID(Airport.GetID());
        Airport.AddRunway(pRunway);
        LoadSSA(Scene, *pRunway);
    }
}

void Dao::LoadSSA(const Scene& Scene, CRunway& Runway) const
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare(
            "SELECT id, name, type, trans_alt FROM ssa WHERE runway_id = :1");
    query->BindValue(":1", Runway.GetID());
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    while (query->next()) {
        QSharedPointer < CSSA > pSSA(Scene.CreateSSA());
        pSSA->SetParent(&Runway);
        pSSA->SetID(query->value(0).toUInt());
        pSSA->SetName(query->value(1).toString());
        pSSA->SetType((CSSA::Type) query->value(2).toInt());
        if (!query->value(3).isNull())
            pSSA->SetTransitAlt(query->value(3).toDouble());
        pSSA->SetRunwayID(Runway.GetID());
        Runway.AddSSA(pSSA);
        LoadSSAPoints(Scene, *pSSA);
    }
}

void Dao::LoadSSAPoints(const Scene& Scene, CSSA& SSA) const
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare(
            "SELECT ssa_point.id, ssa_point.sn, point.name, point.type, point.icao, "
            "point.airport, ssa_point.type, ssa_point.course, ssa_point.turn, "
            "ssa_point.altitude1, ssa_point.fly_over "
            "FROM ssa_point LEFT OUTER JOIN point ON point_id = point.id WHERE ssa_id = :1");
    query->BindValue(":1", SSA.GetID());
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    while (query->next()) {
        QSharedPointer < CSSAPoint > pSSAPoint(new CSSAPoint);
        pSSAPoint->SetSSAID(SSA.GetID());
        pSSAPoint->SetID(query->value(0).toUInt());
        pSSAPoint->SetSN(query->value(1).toInt());
        if (!query->value(2).isNull()) {
            Scene::TPointsMap::const_iterator itFind = Scene.GetPoints().find(
                    CPointKey(query->value(2).toString(),
                              (CPoint::Type) query->value(3).toInt(), query->value(4).toString(),
                              query->value(5).toString()));
            if (itFind != Scene.GetPoints().end())
                pSSAPoint->SetPoint(*itFind);
            pSSAPoint->SetName(query->value(2).toString());
        }
        pSSAPoint->SetType((CSSAPoint::Type) query->value(6).toInt());
        if (!query->value(7).isNull())
            pSSAPoint->SetCourse(query->value(7).toDouble());
        if (!query->value(8).isNull())
            pSSAPoint->SetTurn((CSSAPoint::Turn) query->value(8).toInt());
        if (!query->value(9).isNull())
            pSSAPoint->SetAlt1(/*(CSSAPoint::Turn) */query->value(9).toDouble());
        pSSAPoint->SetFlyOver(query->value(10).toInt());

        SSA.AddPoint(pSSAPoint);
    }
}

void Dao::LoadRestrictions(Scene& scene) const
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare(
            "SELECT id, name, type, designation, multi_code FROM restriction WHERE zone_id = :1");
    query->BindValue(":1", scene.GetID());
    QSharedPointer<db::CDBQuery> queryPoints = db::CreateQuery();
    queryPoints->Prepare(
            "SELECT id, sn, type, latitude, longitude, arc_latitude, arc_longitude, arc_distance "
            "FROM restriction_point WHERE restriction_id = :1 "
            "ORDER BY sn");

    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    while (query->next()) {
        QSharedPointer < CRestriction > pRestriction(scene.CreateRestriction());
        pRestriction->SetID(query->value(0).toUInt());
        pRestriction->SetName(query->value(1).toString());
        pRestriction->SetType((CRestriction::Type) query->value(2).toInt());
        pRestriction->SetDesignation(query->value(3).toString());
        pRestriction->SetMultiCode(query->value(4).toString());

        pRestriction->SetZoneID(scene.GetID());

        queryPoints->BindValue(":1", pRestriction->GetID());
        TVA_CHECK(
                queryPoints->exec(),
                tr("Execution error. %1").arg(queryPoints->lastError().text()),
                E_FAIL);
        while (queryPoints->next()) {
            QSharedPointer < CRestrictPoint > pRestrPoint(new CRestrictPoint());
            pRestrPoint->SetID(queryPoints->value(0).toUInt());
            pRestrPoint->SetSN(queryPoints->value(1).toInt());
            pRestrPoint->SetType((CRestrictPoint::Type) queryPoints->value(2).toInt());
            pRestrPoint->SetCoord(CCoord(queryPoints->value(4).toDouble(),
                                         queryPoints->value(3).toDouble()));
            pRestrPoint->SetArcCoord(CCoord(queryPoints->value(6).toDouble(),
                                            queryPoints->value(5).toDouble()));
            pRestrPoint->SetArcDistance(queryPoints->value(7).toDouble());
            pRestrPoint->SetRestrictID(pRestriction->GetID());
            pRestriction->AddPoint(pRestrPoint);
        }
        scene.AddRestriction(pRestriction);
    }
}

void Dao::loadUserRestrictionsUids(int zone_id, QList<QUuid> &uids) const
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare("SELECT guid FROM restriction_user WHERE zone_id = :1");
    query->BindValue(":1", zone_id);

    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    while (query->next()) {
        uids.append(QUuid(query->value(0).toString()));
    }
}

void Dao::loadSpeedValues(QHash<int, std::set<float> > &speed_values)
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare("SELECT value FROM speed_in_mach");
    std::set<float> values;
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    while (query->next()) {
        values.insert(query->value(0).toFloat());
    }
    speed_values.insert(1, values);

    query->Prepare("SELECT value FROM speed_in_kmph");
    values.clear();
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    while (query->next()) {
        values.insert(query->value(0).toFloat());
    }
    speed_values.insert(0, values);

    query->Prepare("SELECT value FROM speed_in_knot");
    values.clear();
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    while (query->next()) {
        values.insert(query->value(0).toFloat());
    }
    speed_values.insert(2, values);
}

void Dao::loadVerticalSpeedValues(QHash<int, std::set<int> > &speed_values)
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare("SELECT value FROM vspeed_in_meter");
    std::set<int> values;
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    while (query->next()) {
        values.insert(query->value(0).toFloat());
    }
    speed_values.insert(0, values);

    query->Prepare("SELECT value FROM vspeed_in_feet");
    values.clear();
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    while (query->next()) {
        values.insert(query->value(0).toFloat());
    }
    speed_values.insert(1, values);
}

void Dao::LoadUserRestrictions(Scene &scene) const
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare("SELECT id, name, type, h_min, h_max, time_start, time_end, guid, actual FROM restriction_user WHERE zone_id = :1");
    query->BindValue(":1", scene.GetID());
    QSharedPointer<db::CDBQuery> queryPoints = db::CreateQuery();
    queryPoints->Prepare(
            "SELECT id, sn, type, latitude, longitude, arc_latitude, arc_longitude, arc_distance "
            "FROM restriction_point_user WHERE restriction_id = :1 "
            "ORDER BY sn");

    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    while (query->next()) {
        QSharedPointer<CRestrictionUser> restriction(scene.CreateUserRestriction());
        restriction->SetID(query->value(0).toUInt());
        restriction->SetName(query->value(1).toString());
        restriction->SetType((CRestrictionUser::Type) query->value(2).toUInt());
        restriction->SetHMin(query->value(3).toDouble());
        restriction->SetHMax(query->value(4).toDouble());
        restriction->SetTimeBegin(query->value(5).toDateTime());
        restriction->SetTimeEnd(query->value(6).toDateTime());
        restriction->SetGUID(QUuid(query->value(7).toString()));
        restriction->setActual(static_cast<Qt::CheckState>(query->value(8).toUInt()));

        restriction->SetZoneID(scene.GetID());

        queryPoints->BindValue(":1", restriction->GetID());
        TVA_CHECK(
                queryPoints->exec(),
                tr("Execution error. %1").arg(queryPoints->lastError().text()),
                E_FAIL);
        while (queryPoints->next()) {
            QSharedPointer<CRestrictPoint> restr_point(new CRestrictPoint());
            restr_point->SetID(queryPoints->value(0).toUInt());
            restr_point->SetSN(queryPoints->value(1).toInt());
            restr_point->SetType((CRestrictPoint::Type) queryPoints->value(2).toInt());
            restr_point->SetCoord(CCoord(queryPoints->value(4).toDouble(),
                                         queryPoints->value(3).toDouble()));
            restr_point->SetArcCoord(CCoord(queryPoints->value(6).toDouble(),
                                            queryPoints->value(5).toDouble()));
            restr_point->SetArcDistance(queryPoints->value(7).toDouble());
            restr_point->SetRestrictID(restriction->GetID());
            restriction->AddPoint(restr_point);
        }
        scene.AddUserRestriction(restriction);
    }
}

void Dao::LoadUserRestriction(Scene& scene, quint32 id) const
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare(
            "SELECT id, name, type, h_min, h_max, time_start, time_end, guid, actual FROM restriction_user WHERE zone_id = :1 and id = :2");
    query->BindValue(":1", scene.GetID());
    query->BindValue(":2", id);
    QSharedPointer<db::CDBQuery> queryPoints = db::CreateQuery();
    queryPoints->Prepare(
            "SELECT id, sn, type, latitude, longitude, arc_latitude, arc_longitude, arc_distance "
            "FROM restriction_point_user WHERE restriction_id = :1 "
            "ORDER BY sn");

    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    if ( query->next() ) {
        QSharedPointer < CRestrictionUser > pRestriction(scene.CreateUserRestriction());
        pRestriction->SetID(query->value(0).toUInt());
        pRestriction->SetName(query->value(1).toString());
        pRestriction->SetType((CRestrictionUser::Type) query->value(2).toInt());
        pRestriction->SetHMin(query->value(3).toDouble());
        pRestriction->SetHMax(query->value(4).toDouble());
        pRestriction->SetTimeBegin(query->value(5).toDateTime());
        pRestriction->SetTimeEnd(query->value(6).toDateTime());
        pRestriction->SetGUID(QUuid(query->value(7).toString()));
        pRestriction->setActual(static_cast<Qt::CheckState>(query->value(8).toUInt()));

        pRestriction->SetZoneID(scene.GetID());

        queryPoints->BindValue(":1", pRestriction->GetID());
        TVA_CHECK(
                queryPoints->exec(),
                tr("Execution error. %1").arg(queryPoints->lastError().text()),
                E_FAIL);
        while (queryPoints->next()) {
            QSharedPointer < CRestrictPoint > pRestrPoint(new CRestrictPoint());
            pRestrPoint->SetID(queryPoints->value(0).toUInt());
            pRestrPoint->SetSN(queryPoints->value(1).toInt());
            pRestrPoint->SetType((CRestrictPoint::Type) queryPoints->value(2).toInt());
            pRestrPoint->SetCoord(CCoord(queryPoints->value(4).toDouble(),
                                         queryPoints->value(3).toDouble()));
            pRestrPoint->SetArcCoord(CCoord(queryPoints->value(6).toDouble(),
                                            queryPoints->value(5).toDouble()));
            pRestrPoint->SetArcDistance(queryPoints->value(7).toDouble());
            pRestrPoint->SetRestrictID(pRestriction->GetID());
            pRestriction->AddPoint(pRestrPoint);
        }
        scene.AddUserRestriction(pRestriction);
    }
}

void Dao::loadUserRestriction(Scene &scene, const QUuid &uid) const
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare(
            "SELECT id, name, type, h_min, h_max, time_start, time_end, actual FROM restriction_user WHERE zone_id = :1 and guid = :2");
    query->BindValue(":1", scene.GetID());
    query->BindValue(":2", uid.toString());
    QSharedPointer<db::CDBQuery> queryPoints = db::CreateQuery();
    queryPoints->Prepare(
            "SELECT id, sn, type, latitude, longitude, arc_latitude, arc_longitude, arc_distance "
            "FROM restriction_point_user WHERE restriction_id = :1 "
            "ORDER BY sn");

    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    if ( query->next() ) {
        QSharedPointer<CRestrictionUser> pRestriction(scene.CreateUserRestriction());
        pRestriction->SetID(query->value(0).toUInt());
        pRestriction->SetName(query->value(1).toString());
        pRestriction->SetType((CRestrictionUser::Type) query->value(2).toInt());
        pRestriction->SetHMin(query->value(3).toDouble());
        pRestriction->SetHMax(query->value(4).toDouble());
        pRestriction->SetTimeBegin(query->value(5).toDateTime());
        pRestriction->SetTimeEnd(query->value(6).toDateTime());
        pRestriction->SetGUID(uid);
        pRestriction->setActual(static_cast<Qt::CheckState>(query->value(7).toUInt()));

        pRestriction->SetZoneID(scene.GetID());

        queryPoints->BindValue(":1", pRestriction->GetID());
        TVA_CHECK(
                queryPoints->exec(),
                tr("Execution error. %1").arg(queryPoints->lastError().text()),
                E_FAIL);
        while (queryPoints->next()) {
            QSharedPointer<CRestrictPoint> pRestrPoint(new CRestrictPoint());
            pRestrPoint->SetID(queryPoints->value(0).toUInt());
            pRestrPoint->SetSN(queryPoints->value(1).toInt());
            pRestrPoint->SetType((CRestrictPoint::Type) queryPoints->value(2).toInt());
            pRestrPoint->SetCoord(CCoord(queryPoints->value(4).toDouble(),
                                         queryPoints->value(3).toDouble()));
            pRestrPoint->SetArcCoord(CCoord(queryPoints->value(6).toDouble(),
                                            queryPoints->value(5).toDouble()));
            pRestrPoint->SetArcDistance(queryPoints->value(7).toDouble());
            pRestrPoint->SetRestrictID(pRestriction->GetID());
            pRestriction->AddPoint(pRestrPoint);
        }
        scene.AddUserRestriction(pRestriction);
    }
}

quint32 Dao::SaveUserRestriction(quint32 zoneId, CRestrictionUser &r, bool bDelPoints)
{
    quint32 restId = 0;

    if(!r.GetGUID().isNull())
    {
        restId = GetUserRestrictionIDbyGUID(r.GetGUID());
        if(restId)
        {
            r.SetID(restId);
            for ( CRestrictionUser::TPointList::iterator itc = r.GetPoints().begin();
                  itc != r.GetPoints().end(); ++itc ) (*itc)->SetRestrictID(restId);
            UpdateUserRestriction(r, bDelPoints);
            return restId;
        }
    }
    else
    {
        r.SetGUID(QUuid::createUuid());
    }

    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return restId;
    QtAutoTransaction transaction(QSqlDatabase::database());

    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    // Add new record into restriction_user
    query->Prepare(
            "INSERT INTO restriction_user (name, h_min, h_max, time_start, time_end, zone_id, type, guid, actual) VALUES(:1, :2, :3, :4, :5, :6, :7, :8, :9)",
            "id");
    query->BindValue(":1", r.GetName());
    query->BindValue(":2", r.GetHMin());
    query->BindValue(":3", r.GetHMax());
    query->BindValue(":4", r.GetTimeBegin());
    query->BindValue(":5", r.GetTimeEnd());
    query->BindValue(":6", zoneId);
    query->BindValue(":7", r.GetType());
    query->BindValue(":8", r.GetGUID().toString());
    query->BindValue(":9", r.actual());
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);

    restId = query->GetLastInsertID().toUInt();
    r.SetID(restId);

    QSharedPointer<db::CDBQuery> queryPoints = db::CreateQuery();
    // Add new records into restriction_point_user
    if(bDelPoints)
    {
        queryPoints->Prepare(
                "DELETE FROM restriction_point_user WHERE restriction_id = :1");
        queryPoints->BindValue(":1", restId);
        TVA_CHECK(queryPoints->exec(), tr("Execution error. %1").arg(queryPoints->lastError().text()), E_FAIL);
    }

    queryPoints->Prepare(
            "INSERT INTO restriction_point_user (sn, type, latitude, longitude, arc_latitude, arc_longitude, arc_distance, restriction_id) VALUES(:1, :2, :3, :4, :5, :6, :7, :8)",
            "id");

    for ( CRestrictionUser::TPointList::iterator itc = r.GetPoints().begin();
          itc != r.GetPoints().end(); ++itc )
    {
        CRestrictionUser::TPoint &point = *itc;

        queryPoints->BindValue(":1", point->GetSN());
        queryPoints->BindValue(":2", (int)point->GetType());
        queryPoints->BindValue(":3", point->GetCoord().GetLatitude().GetAngle());
        queryPoints->BindValue(":4", point->GetCoord().GetLongitude().GetAngle());
        queryPoints->BindValue(":5", point->GetArcCoord().GetLatitude().GetAngle());
        queryPoints->BindValue(":6", point->GetArcCoord().GetLongitude().GetAngle());
        queryPoints->BindValue(":7", point->GetArcDistance());
        queryPoints->BindValue(":8", restId);
        TVA_CHECK(queryPoints->exec(), tr("Execution error. %1").arg(queryPoints->lastError().text()), E_FAIL);

        point->SetRestrictID(restId);
        point->SetID(query->GetLastInsertID().toUInt());
    }

    transaction.commit();

    return restId;
}

void Dao::UpdateUserRestriction(CRestrictionUser &r, bool bDelPoints)
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QtAutoTransaction transaction(QSqlDatabase::database());

    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare("UPDATE restriction_user SET "
                  "name = :2, type = :3, h_min = :4, h_max = :5, "
                  "time_start = :6, time_end = :7, actual = :8 "
                  "WHERE id = :1");
    query->BindValue(":1", r.GetID());
    query->BindValue(":2", r.GetName());
    query->BindValue(":3", r.GetType());
    query->BindValue(":4", r.GetHMin());
    query->BindValue(":5", r.GetHMax());
    query->BindValue(":6", r.GetTimeBegin());
    query->BindValue(":7", r.GetTimeEnd());
    query->BindValue(":8", r.actual());
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);

    QSharedPointer<db::CDBQuery> queryPoints = db::CreateQuery();
    // Add new records into restriction_point_user
    if (bDelPoints)
    {
        queryPoints->Prepare(
                "DELETE FROM restriction_point_user WHERE restriction_id = :1");
        queryPoints->BindValue(":1", r.GetID());
        TVA_CHECK(queryPoints->exec(), tr("Execution error. %1").arg(queryPoints->lastError().text()), E_FAIL);
    }

    queryPoints->Prepare(
            "INSERT INTO restriction_point_user (sn, type, latitude, longitude, arc_latitude, arc_longitude, arc_distance, restriction_id) VALUES(:1, :2, :3, :4, :5, :6, :7, :8)",
            "id");

    for (CRestrictionUser::TPointList::iterator itc = r.GetPoints().begin(); itc != r.GetPoints().end(); ++itc)
    {
        CRestrictionUser::TPoint &point = *itc;

        queryPoints->BindValue(":1", point->GetSN());
        queryPoints->BindValue(":2", (int)point->GetType());
        queryPoints->BindValue(":3", point->GetCoord().GetLatitude().GetAngle());
        queryPoints->BindValue(":4", point->GetCoord().GetLongitude().GetAngle());
        queryPoints->BindValue(":5", point->GetArcCoord().GetLatitude().GetAngle());
        queryPoints->BindValue(":6", point->GetArcCoord().GetLongitude().GetAngle());
        queryPoints->BindValue(":7", point->GetArcDistance());
        queryPoints->BindValue(":8", r.GetID());
        TVA_CHECK(queryPoints->exec(), tr("Execution error. %1").arg(queryPoints->lastError().text()), E_FAIL);

        point->SetRestrictID(r.GetID());
        point->SetID(query->GetLastInsertID().toUInt());
    }

    transaction.commit();
}

void Dao::DeleteUserRestriction(const QUuid &uid)
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QtAutoTransaction transaction(QSqlDatabase::database());
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare("DELETE FROM restriction_user WHERE guid = :1");
    query->BindValue(":1", uid.toString());
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    transaction.commit();
}

quint32 Dao::GetUserRestrictionIDbyGUID(const QUuid& uid) const
{
    quint32 qID(0);
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return qID;
    if(!uid.isNull())
    {
        QSharedPointer<db::CDBQuery> query = db::CreateQuery();
        query->Prepare("SELECT id FROM restriction_user WHERE guid = :1");
        query->BindValue(":1", uid.toString());

        if (query->exec() && query->next())
        {
            qID = query->value(0).toUInt();
        }
    }
    return qID;
}

void Dao::LoadHoldings(Scene& scene) const
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare(
            "SELECT holding.id, point.name, point.type, point.icao, point.airport, holding.type, in_course, turn_direction, leg_length, leg_time, speed "
            "FROM holding, point WHERE point.id = holding.point_id AND holding.zone_id = :1");
    query->BindValue(":1", scene.GetID());

    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    while (query->next()) {
        QSharedPointer < CHolding > pHolding(scene.CreateHolding());
        pHolding->SetID(query->value(0).toUInt());
        QString sPointName(query->value(1).toString());
        CPoint::Type PointType((CPoint::Type) query->value(2).toInt());
        QString sPointICAO(query->value(3).toString());
        QString sPointAir(query->value(4).toString());
        Scene::TPointsMap::iterator itPointFind = scene.GetPoints().find(CPointKey(
                sPointName, PointType, sPointICAO, sPointAir));
        if (itPointFind != scene.GetPoints().end()) {
            pHolding->SetPoint(*itPointFind);
            pHolding->SetType((CHolding::Type) query->value(5).toInt());
            pHolding->SetInCourse(query->value(6).toDouble());
            pHolding->SetTurn((CHolding::Turn) query->value(7).toInt());
            pHolding->SetLegLength(query->value(8).toDouble());
            pHolding->SetLegTime(query->value(9).toInt());
            pHolding->SetSpeed(query->value(10).toDouble());
            pHolding->SetZoneID(scene.GetID());
            scene.AddHolding(pHolding);
        }
    }
}

void Dao::LoadMapNotes(Scene &scene) const
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> dbQuery = db::CreateQuery();
    dbQuery->Prepare("select LATITUDE, LONGITUDE, DESCRIPTION "
                          "from ZONE_MAP_NOTE "
                          "where zone_id = :1");
    dbQuery->BindValue(":1", scene.GetID());

    TVA_CHECK(dbQuery->exec(), (tr("Execution error") + QString(". %1")).arg(dbQuery->lastError().text()), E_FAIL);
    while(dbQuery->next())
    {
        QSharedPointer<MapNote> pNote(scene.CreateNote());
        CCoord coord(dbQuery->value(1).toDouble(), dbQuery->value(0).toDouble());
        pNote->setCoord(coord);
        pNote->setText(dbQuery->value(2).toString());
        scene.AddNote(pNote);
    }
}

void Dao::SaveMapNotes(const Scene &scene) const
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> dbQuery = db::CreateQuery();
    // Remove old entries
    dbQuery->Prepare("delete from zone_map_note "
                          "where zone_id = :1");

    dbQuery->BindValue(":1", scene.GetID());

    TVA_CHECK(dbQuery->exec(), (tr("Execution error") + QString(". %1")).arg(dbQuery->lastError().text()), E_FAIL);

    // populate new data
    dbQuery->Prepare(
            "INSERT INTO zone_map_note(zone_id, latitude, longitude, description) "
                 "VALUES(:1, :2, :3, :4)");
    dbQuery->BindValue(":1", scene.GetID());
    Scene::TNotesMap::const_iterator itScanNote = scene.GetNotes().begin();
    for(; itScanNote != scene.GetNotes().end(); ++itScanNote)
    {
        // Create point
        dbQuery->BindValue(":2", (*itScanNote)->coord().GetLatitude().GetAngle());
        dbQuery->BindValue(":3", (*itScanNote)->coord().GetLongitude().GetAngle());
        dbQuery->BindValue(":4", (*itScanNote)->text());
        TVA_CHECK(dbQuery->exec(), (tr("Execution error") + QString(". %1")).arg(dbQuery->lastError().text()), E_FAIL);
    }
}

void Dao::LoadUserPics(Scene& scene) const
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare(
            "SELECT id, name FROM user_pics WHERE zone_id = :1");
    query->BindValue(":1", scene.GetID());
    QSharedPointer<db::CDBQuery> queryPoints = db::CreateQuery();
    queryPoints->Prepare(
            "SELECT id, sn, type, latitude, longitude, arc_latitude, arc_longitude, arc_distance "
            "FROM user_pics_point WHERE user_pics_id = :1 "
            "ORDER BY sn");

    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    while (query->next()) {
        QSharedPointer < CUserPics > pUserPics(scene.CreateUserPics());
        pUserPics->SetID(query->value(0).toUInt());
        pUserPics->SetName(query->value(1).toString());

        pUserPics->SetZoneID(scene.GetID());

        queryPoints->BindValue(":1", pUserPics->GetID());
        TVA_CHECK(
                queryPoints->exec(),
                tr("Execution error. %1").arg(queryPoints->lastError().text()),
                E_FAIL);
        while (queryPoints->next()) {
            QSharedPointer < CUserPicsPoint > pUserPicsPoint(new CUserPicsPoint());
            pUserPicsPoint->SetID(queryPoints->value(0).toUInt());
            pUserPicsPoint->SetSN(queryPoints->value(1).toInt());
            pUserPicsPoint->SetType((CUserPicsPoint::Type) queryPoints->value(2).toInt());
            pUserPicsPoint->SetCoord(CCoord(queryPoints->value(4).toDouble(),
                                         queryPoints->value(3).toDouble()));
            pUserPicsPoint->SetArcCoord(CCoord(queryPoints->value(6).toDouble(),
                                            queryPoints->value(5).toDouble()));
            pUserPicsPoint->SetArcDistance(queryPoints->value(7).toDouble());
            pUserPicsPoint->SetUserPicsID(pUserPics->GetID());
            pUserPics->AddPoint(pUserPicsPoint);
        }
        scene.AddUserPics(pUserPics);
    }
}

void Dao::LoadZone(quint32 uiID, Scene &scene) const
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare("SELECT name, type, id_name, parent_id, low_limit, up_limit, "
                  "short_name, label_name, frequency "
                  "FROM zone WHERE id = :1");
    query->BindValue(":1", uiID);
    TVA_CHECK(query->exec() && query->next(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    scene.SetID(uiID);
    scene.SetName(query->value(0).toString());
    scene.SetType(query->value(1).toInt());
    scene.SetIDName(query->value(2).toString());
    scene.SetParentID(query->value(3).toUInt());
    scene.SetLowLimit(query->value(4).toUInt());
    scene.SetUpLimit(query->value(5).toUInt());
    scene.SetShortName(query->value(6).toString());
    scene.SetLabelName(query->value(7).toString());
    scene.SetFrequency(query->value(8).toDouble());

    if(!scene.GetCentralPoint() || scene.GetCentralPoint()->GetCoord().IsNull())
    {
        CCoord coord;
        if(GetCentralPointForAllFIRs(coord))
        {
            QSharedPointer<CPoint> pCentralPoint(new CPoint);
            pCentralPoint->SetName("Central point");
            pCentralPoint->SetCoord(coord);
            scene.SetCentralPoint(pCentralPoint);
        }
    }

    _LoadBoundaryPoints(scene, scene);

    query->Prepare(
            "SELECT id, name, type, id_name, parent_id, low_limit, up_limit, "
            "short_name, label_name, frequency "
            "FROM zone WHERE parent_id = :1");
    query->BindValue(":1", scene.GetID());
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    scene.GetSectorList().clear();
    while (query->next()) {
        QSharedPointer<Scene> sector(scene.CreateSector());
        sector->SetCentralPoint(scene.GetCentralPoint());
        sector->SetID(query->value(0).toUInt());
        sector->SetName(query->value(1).toString());
        sector->SetType(query->value(2).toInt());
        sector->SetIDName(query->value(3).toString());
        sector->SetParentID(query->value(4).toUInt());
        sector->SetLowLimit(query->value(5).toUInt());
        sector->SetUpLimit(query->value(6).toUInt());
        sector->SetShortName(query->value(7).toString());
        sector->SetLabelName(query->value(8).toString());
        sector->SetFrequency(query->value(9).toDouble());
        _LoadBoundaryPoints(scene, *sector);
        scene.AddSector(sector);
    }
}

void Dao::LoadFir(const QString& sFIR, Scene& Scene) const
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare("SELECT id FROM zone WHERE id_name = :1 AND type = 1");
    query->BindValue(":1", sFIR);
    TVA_CHECK(
            query->exec() && query->next(),
            tr("Execution error. %1").arg(query->lastError().text()),
            E_FAIL);
    LoadZone(query->value(0).toUInt(), Scene);
}

void Dao::LoadZones(QList<Zone>& zones, QString sWhere) const
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QString sSelect = "SELECT id, name, type, id_name, parent_id, low_limit, up_limit, "
                    "short_name, label_name, frequency "
                    "FROM zone";
    if(!sWhere.isEmpty())
        sSelect += " WHERE " + sWhere;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare(sSelect);
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    while (query->next()) {
        Zone zone;
        zone.SetID(query->value(0).toUInt());
        zone.SetName(query->value(1).toString());
        zone.SetType(query->value(2).toInt());
        zone.SetIDName(query->value(3).toString());
        zone.SetParentID(query->value(4).toUInt());
        zone.SetLowLimit(query->value(5).toUInt());
        zone.SetUpLimit(query->value(6).toUInt());
        zone.SetShortName(query->value(7).toString());
        zone.SetLabelName(query->value(8).toString());
        zone.SetFrequency(query->value(9).toDouble());
        zones.push_back(zone);
    }
}

void Dao::LoadFIRs(TSceneMap& SceneList) const
{
    QList < Zone > Zones;
    LoadZones( Zones);
    QList<Zone>::const_iterator itScan = Zones.begin();
    for (; itScan != Zones.end(); ++itScan)
        if (itScan->GetType() == Zone::enFIR)
            SceneList[itScan->GetID()] = QSharedPointer<Scene> (new Scene(*itScan));
}

QSharedPointer<CPoint> Dao::LoadPoint(uint uiID) const
{
    QSharedPointer < CPoint > pPoint(new CPoint);
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return pPoint;
    // Point
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare(
            "SELECT point.id, name, longitude, latitude, icao, type, airport "
            "FROM point "
            "WHERE id = :1");
    // Boundary points
    query->BindValue(":1", uiID);
    TVA_CHECK(query->exec() && query->next(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);

    pPoint->SetID(query->value(0).toUInt());
    pPoint->SetName(query->value(1).toString());
    pPoint->SetCoord(CCoord(query->value(2).toDouble(), query->value(3).toDouble()));
    pPoint->SetICAO(query->value(4).toString());
    pPoint->SetType((CPoint::Type) query->value(5).toInt());
    pPoint->SetAirportName(query->value(6).toString());
    return pPoint;
}

QSharedPointer<CHolding> Dao::LoadHolding(quint32 uiHoldID) const
{
    QSharedPointer < CHolding > pHolding(new CHolding);
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return pHolding;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare(
            "SELECT point.id, point.name, point.longitude, point.latitude, point.icao, point.type, "
            "holding.id, holding.type, holding.in_course, holding.turn_direction, holding.leg_length, holding.leg_time, holding.speed, holding.zone_id "
            "FROM holding, point WHERE point.id = holding.point_id AND holding.id = :1");

    query->BindValue(":1", uiHoldID);
    TVA_CHECK(query->exec() && query->next(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);

    QSharedPointer < CPoint > pBearPoint(new CPoint());
    pBearPoint->SetID(query->value(0).toUInt());
    pBearPoint->SetName(query->value(1).toString());
    pBearPoint->SetCoord(CCoord(query->value(2).toDouble(),
                                query->value(3).toDouble()));
    pBearPoint->SetICAO(query->value(4).toString());
    pBearPoint->SetType((CPoint::Type) query->value(5).toInt());

    pHolding->SetID(query->value(6).toUInt());
    pHolding->SetPoint(pBearPoint);
    pHolding->SetType((CHolding::Type) query->value(7).toInt());
    pHolding->SetInCourse(query->value(8).toDouble());
    pHolding->SetTurn((CHolding::Turn) query->value(9).toInt());
    pHolding->SetLegLength(query->value(10).toDouble());
    pHolding->SetLegTime(query->value(11).toInt());
    pHolding->SetSpeed(query->value(12).toDouble());
    pHolding->SetZoneID(query->value(13).toUInt());

    query->Prepare("SELECT longitude, latitude, sn "
                  "FROM holding_point_master WHERE holding_id = :1");

    query->BindValue(":1", uiHoldID);
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    CHolding::TPointList AlbanianList;
    while (query->next()) {
        QSharedPointer < CPoint > pAlbPoint(new CPoint);
        pAlbPoint->SetCoord(CCoord(query->value(0).toDouble(),
                                   query->value(1).toDouble()));
        AlbanianList[query->value(2).toInt()] = pAlbPoint;
    }
    pHolding->SetMasterPoints(AlbanianList);
    return pHolding;
}

void Dao::LoadHoldings(QList<QSharedPointer<CHolding> >& listHolding,
                       const QString& sFIR) const
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare(
            "SELECT point.id, point.name, point.longitude, point.latitude, point.icao, point.type, "
            "holding.id, holding.type, holding.in_course, holding.turn_direction, holding.leg_length, holding.leg_time, holding.speed, holding.zone_id "
            "FROM holding, point WHERE point.id = holding.point_id AND holding.zone_id IN (SELECT id FROM zone WHERE id_name = :1 AND type = 1)"
            "ORDER BY point.name");

    query->BindValue(":1", sFIR);
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);

    while (query->next()) {
        QSharedPointer < CPoint > pBearPoint(new CPoint());
        pBearPoint->SetID(query->value(0).toUInt());
        pBearPoint->SetName(query->value(1).toString());
        pBearPoint->SetCoord(CCoord(query->value(2).toDouble(),
                                    query->value(3).toDouble()));
        pBearPoint->SetICAO(query->value(4).toString());
        pBearPoint->SetType((CPoint::Type) query->value(5).toInt());

        QSharedPointer < CHolding > pHolding(new CHolding);
        pHolding->SetID(query->value(6).toUInt());
        pHolding->SetPoint(pBearPoint);
        pHolding->SetType((CHolding::Type) query->value(7).toInt());
        pHolding->SetInCourse(query->value(8).toDouble());
        pHolding->SetTurn((CHolding::Turn) query->value(9).toInt());
        pHolding->SetLegLength(query->value(10).toDouble());
        pHolding->SetLegTime(query->value(11).toInt());
        pHolding->SetSpeed(query->value(12).toDouble());
        pHolding->SetZoneID(query->value(13).toUInt());
        listHolding.append(pHolding);
    }
}

void Dao::LoadAirports(uint uiZoneID, QList<CAirport>& AirList) const
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare("SELECT airport.id, name, longitude, latitude, icao, altitude, declination "
                  "FROM airport, zone_airport "
                  "WHERE zone_airport.airport_id = airport.id AND zone_id = :1");
    query->BindValue(":1", uiZoneID);
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    AirList.clear();
    while (query->next()) {
        CAirport Air;
        Air.SetID(query->value(0).toUInt());
        Air.SetName(query->value(1).toString());
        Air.SetCoord(CCoord(query->value(2).toDouble(), query->value(3).toDouble()));
        Air.SetICAO(query->value(4).toString());
        Air.SetAlt(query->value(5).toDouble());
        Air.SetDeclination(query->value(6).toString());
        AirList.append(Air);
    }
}

void Dao::BoundaryToPolygon(const QSharedPointer<CPoint>& pCentral,
                            const QList<QSharedPointer<CBoundaryPoint> >& BoundaryList,
                            QList<CCoord>& CoordList) const
{
    typedef const QList<QSharedPointer<CBoundaryPoint> > TBoundaryList;
    const qint64 qllXMin = -617546;
    const qint64 qllYMax = 422804;
    const double dScale = 0.00106779;

    QPainterPath painterPath;
    TBoundaryList::const_iterator itScan = BoundaryList.begin();
    for (; itScan != BoundaryList.end(); ++itScan) {
        CCoord coord1((*itScan)->GetCoord());
        coord1.Center(pCentral->GetCoord());
        QPointF point((coord1.GetX() - qllXMin) * dScale, (qllYMax - coord1.GetY())
                      * dScale);
        CCoord coord2((*itScan)->GetArcCoord());
        coord2.Center(pCentral->GetCoord());
        QPointF pointArc((coord2.GetX() - qllXMin) * dScale, (qllYMax
                                                              - coord2.GetY()) * dScale);

        TBoundaryList::const_iterator itScanNext = itScan + 1;
        if (itScanNext == BoundaryList.end())
            itScanNext = BoundaryList.begin();

        CCoord coord3((*itScanNext)->GetCoord());
        coord3.Center(pCentral->GetCoord());
        QPointF pointNext((coord3.GetX() - qllXMin) * dScale, (qllYMax
                                                               - coord3.GetY()) * dScale);

        qreal qRadius = (*itScan)->GetArcDistance() * dScale;

        if (itScan == BoundaryList.begin())
            painterPath.moveTo(point);

        switch ((*itScan)->GetType()) {
        case CRestrictPoint::enLine:
            painterPath.lineTo(pointNext);
            break;
        case CRestrictPoint::enCircle:
            painterPath.addEllipse(pointArc, qRadius, qRadius);
            break;
        case CRestrictPoint::enLeftArc:
        case CRestrictPoint::enRightArc: {
                QRectF rect(QPointF(pointArc.x() - qRadius, pointArc.y() - qRadius),
                            QSizeF(2 * qRadius, 2 * qRadius));
                QLineF line1(pointArc, point);
                QLineF line2(pointArc, pointNext);

                if ((*itScan)->GetType() == CRestrictPoint::enLeftArc)
                    painterPath.arcTo(rect, line1.angle(), line1.angleTo(line2));
                else
                    painterPath.arcTo(rect, line1.angle(), line1.angleTo(line2) - 360);
            }
            break;
        default:
            qWarning("unknow CRestrictPoint::Type");
        }
    }
    CoordList.clear();
    if (!painterPath.isEmpty()) {
        QPolygonF polygon = painterPath.toFillPolygon();
        QPolygonF::const_iterator itPolygon = polygon.begin();
        for (; itPolygon != polygon.end(); ++itPolygon) {
            QPointF point = *itPolygon;
            CCoord coord;
            coord.SetX(point.x() / dScale + qllXMin);
            coord.SetY(qllYMax - point.y() / dScale);
            coord.DeCenter(pCentral->GetCoord());
            CoordList.append(coord);
        }
    }
}

void Dao::_LoadBoundaryPoints(Scene& scene, Zone& zone) const
{
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare(
            "SELECT id, type, sn, longitude, latitude, arc_longitude, arc_latitude, arc_distance  "
            "FROM zone_boundary "
            "WHERE zone_id = :1 ORDER BY sn");
    // Boundary points
    query->BindValue(":1", zone.GetID());
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    zone.GetBoundary().clear();
    while (query->next()) {
        QSharedPointer < CBoundaryPoint > pPoint(scene.CreateBoundaryPoint());
        pPoint->SetID(query->value(0).toUInt());
        pPoint->SetType((CBoundaryPoint::Type) query->value(1).toInt());
        pPoint->SetSN(query->value(2).toInt());
        pPoint->SetCoord(CCoord(query->value(3).toDouble(),
                                query->value(4).toDouble()));
        pPoint->SetArcCoord(CCoord(query->value(5).toDouble(),
                                   query->value(6).toDouble()));
        pPoint->SetArcDistance(query->value(7).toDouble());

        zone.AddBoundaryPoint(pPoint);
    }
    BoundaryToPolygon(scene.GetCentralPoint(), zone.GetBoundary(),
                      zone.GetPolygonCoord());
}

void Dao::InitOLDI(const QStringList& listSectors, const bool bClearFPL, const bool bOnlySFPL) const
{
#if defined(REDIS)
#else
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QtAutoTransaction transaction(QSqlDatabase::database());
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    if(!bOnlySFPL)
    {
        query->Prepare("DELETE FROM oldi_clients");
        TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
        query->Prepare("DELETE FROM inputmessages");
        TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
        query->Prepare("DELETE FROM outputmessages");
        TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    }
    query->Prepare("DELETE FROM "+MASTER_SFPL_TABLE);
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    query->Prepare("DELETE FROM "+MASTER_FPL_TABLE);
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    if(bClearFPL)
    {
        query->Prepare("DELETE FROM fpl");
        TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    }
    transaction.commit();
#endif
}

quint32 Dao::GetFPLIdByKey(QString sACID, QString sADEP, QString sADES, int iTable) const
{
#if defined(REDIS)
    return 0;
#else
    QString sTable;
    switch(iTable)
    {
    case 0:
        sTable = "fpl";
        break;
    case 1:
        sTable = MASTER_FPL_TABLE;
        break;
    case 2:
        sTable = MASTER_SFPL_TABLE;
        break;
    default:
        sTable = "fpl";
        break;
    }

    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    quint32 idFPL = 0;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare(
            "SELECT id "
            " FROM " + sTable +
            " WHERE aircraft_id = :1 AND adep = :2 AND ades = :3");
    query->BindValue(":1", sACID);
    query->BindValue(":2", sADEP);
    query->BindValue(":3", sADES);
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    if(query->next())
        idFPL = query->value(0).toUInt();
    return idFPL;
#endif
}

void Dao::UpdateFPLTable(const QSharedPointer<CSFPL>& pSFPL)
{
#if defined(REDIS)
    if(master() && master()->getRedisDB())
    {
        if (master()->getRedisDB()->getConnectionStatus() != 1)
            return;
        master()->getRedisDB()->saveSFPL(pSFPL);
    }
#else
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QtAutoTransaction transaction(QSqlDatabase::database());
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    quint32 idFPL = 0;
    idFPL = GetFPLIdByKey(pSFPL->GetACID(), pSFPL->GetDepAerodrome(), pSFPL->GetDestAerodrome(), 0);
    if(!idFPL)
    {
        // insert in FPL
        query->Prepare(
                "INSERT INTO fpl "
                "(fir_name, aircraft_id, taircraft, frules, tflight, num, wtc, equipment, adep, time, speed, flevel, route, ades, eet, alt1, alt2, other, trtime) "
                "VALUES(:0, :1, :2, :3, :4, :5, :6, :7, :8, :9, :10, :11, :12, :13, :14, :15, :16, :17, " + query->GetSystimestampStr() + ")");
        _BindSFPLValues(pSFPL, query);
        TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    }
    else
    {
        // update FPL
        pSFPL->SetID(idFPL);
        query->Prepare(
                "UPDATE fpl SET "
                "fir_name = :0, aircraft_id = :1, taircraft = :2, frules = :3, tflight = :4, num = :5, "
                "wtc = :6, equipment = :7, adep = :8, time = :9, speed = :10, flevel = :11, route = :12, "
                "ades = :13, eet = :14, alt1 = :15, alt2 = :16, other = :17 "
                "WHERE id = :18");
        _BindSFPLValues(pSFPL, query);
        TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    }
    idFPL = GetFPLIdByKey(pSFPL->GetACID(), pSFPL->GetDepAerodrome(), pSFPL->GetDestAerodrome(), 0);
    pSFPL->SetID(idFPL);
    if(idFPL)
    {
        quint32 idMasterFPL = GetFPLIdByKey(pSFPL->GetACID(), pSFPL->GetDepAerodrome(), pSFPL->GetDestAerodrome(), 1);
        while(idMasterFPL)
        {
            // delete FPL from MASTER_FPL_TABLE
            query->Prepare("DELETE FROM "+MASTER_FPL_TABLE+" WHERE id = :1");
            query->BindValue(":1", idMasterFPL);
            TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
            idMasterFPL = GetFPLIdByKey(pSFPL->GetACID(), pSFPL->GetDepAerodrome(), pSFPL->GetDestAerodrome(), 1);
        }
        // insert in MASTER_FPL_TABLE
        query->Prepare(
                "INSERT INTO " +MASTER_FPL_TABLE+
                " SELECT * FROM fpl "
                " WHERE fpl.id = :1");
        query->BindValue(":1", idFPL);
        TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);

        query->Prepare("UPDATE "+MASTER_FPL_TABLE+" SET visibility = 1 "
                      "WHERE id = :1");
        query->BindValue(":1", idFPL);
        TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    }
    else
    {
        TVA_CHECK(false, tr("FPL doesn't exist."), E_FAIL);
    }
    transaction.commit();
#endif
}

void Dao::InsertFPLinFPL_MASTER(quint32 idFPL)
{
#if defined(REDIS)
#else
    if(idFPL)
    {
        if (DBConnectionsManager::instance().getConnectionStatus() != 1)
            return;
        QtAutoTransaction transaction(QSqlDatabase::database());
        QSharedPointer<db::CDBQuery> query = db::CreateQuery();
        // insert in MASTER_FPL_TABLE
        query->Prepare(
                "INSERT INTO " +MASTER_FPL_TABLE+
                " SELECT * FROM fpl "
                " WHERE fpl.id = :1");
        query->BindValue(":1", idFPL);
        TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);

        query->Prepare("UPDATE "+MASTER_FPL_TABLE+" SET visibility = 1 "
                      "WHERE id = :1");
        query->BindValue(":1", idFPL);
        TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
        transaction.commit();
    }
#endif
}

void Dao::ClearFPLByKey(QString sACID, QString sADEP, QString sADES)
{
#if defined(REDIS)
#else
    if (DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QtAutoTransaction transaction(QSqlDatabase::database());
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();

    query->Prepare("DELETE FROM "+MASTER_SFPL_TABLE+" WHERE aircraft_id = :1 AND adep = :2 AND ades = :3");
    query->BindValue(":1", sACID);
    query->BindValue(":2", sADEP);
    query->BindValue(":3", sADES);
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);

    query->Prepare("DELETE FROM "+MASTER_FPL_TABLE+" WHERE aircraft_id = :1 AND adep = :2 AND ades = :3");
    query->BindValue(":1", sACID);
    query->BindValue(":2", sADEP);
    query->BindValue(":3", sADES);
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);

    query->Prepare("DELETE FROM fpl WHERE aircraft_id = :1 AND adep = :2 AND ades = :3");
    query->BindValue(":1", sACID);
    query->BindValue(":2", sADEP);
    query->BindValue(":3", sADES);
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);

    transaction.commit();
#endif
}

void Dao::_BindSFPLValues(const QSharedPointer<CSFPL>& pSFPL, db::CDBQuery *query) const
{
    query->BindValue(":0", pSFPL->GetFirName());
    query->BindValue(":1", pSFPL->GetACID());
    query->BindValue(":2", pSFPL->GetAirplanePtr() ? pSFPL->GetAirplanePtr()->GetName() : "");
    query->BindValue(":3", pSFPL->GetFRules());
    query->BindValue(":4", pSFPL->GetTypeFlight());
    query->BindValue(":5", pSFPL->GetNum());
    query->BindValue(":6", pSFPL->GetTurbulence());
    query->BindValue(":7", pSFPL->GetEquipment());
    query->BindValue(":8", pSFPL->GetDepAerodrome());
    query->BindValue(":9", pSFPL->GetDepTime());
    query->BindValue(":10", pSFPL->GetSpeedS());
    query->BindValue(":11", pSFPL->GetLevelS());
    query->BindValue(":12", pSFPL->GetRoute());
    query->BindValue(":13", pSFPL->GetDestAerodrome());
    query->BindValue(":14", pSFPL->GetDestTime());
    query->BindValue(":15", pSFPL->GetAlt1Aerodrome());
    query->BindValue(":16", pSFPL->GetAlt2Aerodrome());
    query->BindValue(":17", pSFPL->GetOther());
    query->BindValue(":18", pSFPL->GetID());

    query->BindValue(":19", pSFPL->GetCODE());
    query->BindValue(":20", pSFPL->GetAllSectors());
    query->BindValue(":21", pSFPL->GetInSector());
    query->BindValue(":22", pSFPL->GetCurSector());
    query->BindValue(":23", pSFPL->GetNextSector());
    query->BindValue(":24", pSFPL->GetCOPin());
    query->BindValue(":25", pSFPL->GetETOin().toString("hh:mm"));
    query->BindValue(":26", pSFPL->GetCOPinFL().GetF());
}

QString Dao::GetCWPbySector(const QString &sSector) const
{
    QString sCWP;
    if(DBConnectionsManager::instance().getConnectionStatus() != 1)
        return sCWP;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare(
            "SELECT client FROM oldi_clients "
            "WHERE sector = :1");
    query->BindValue(":1", sSector);
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    if(query->next())
        sCWP = query->value(0).toString();
    TVA_CHECK(!query->next(), tr("Execution error. %1").arg(tr("Multiselection CWP")), E_FAIL);

    return sCWP;
}

QString Dao::GetSectorByCWP(const QString &sCWP) const
{
    QString sSector;
    if(DBConnectionsManager::instance().getConnectionStatus() != 1)
        return sSector;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare(
            "SELECT sector FROM oldi_clients "
            "WHERE status = 1 AND client = :1");
    query->BindValue(":1", sCWP);
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    if(query->next())
        sSector = query->value(0).toString();
    TVA_CHECK(!query->next(), tr("Execution error. %1").arg(tr("Multiselection CWP")), E_FAIL);

    return sSector;
}

QString Dao::GetSectorBySectorShortName(const QString &sSector) const
{
    QString sCWP = GetCWPbySector(sSector);
    if(!sCWP.isEmpty())
        return GetSectorByCWP(sCWP);
    return sCWP;
}

void Dao::LoadCWPSectors(OLDIClients::TClientMap& mapCWP, QString sWhere)
{
    if(DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    QString sSelect = "SELECT client, status, sector FROM oldi_clients ";
    if (!sWhere.isEmpty())
        sSelect += "WHERE " + sWhere;
    //sSelect += " GROUP BY client, status, sector";
    query->Prepare(sSelect);
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    while (query->next())
    {
        OLDIClients clData;
        clData.SetClient(query->value(0).toString());
        clData.SetStatus(query->value(1).toInt());
        clData.SetSector(query->value(2).toString());

        if (mapCWP[clData.GetClient()].size()) {
            if (mapCWP[clData.GetClient()].first().GetStatus() > clData.GetStatus())
                mapCWP[clData.GetClient()].prepend(clData);
            else
                mapCWP[clData.GetClient()].append(clData);
        } else {
            mapCWP[clData.GetClient()].append(clData);
        }
    }
}

void Dao::LoadSectorAirports(QHash<quint32, QStringList> &data)
{
    if(DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    QString sSelect =
            "select air.sector_id, air.airports "
            "from sectoring_airports air, sectoring sec "
            "where sec.id = air.sector_id";

    query->Prepare(sSelect);

    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);

    quint32 id;
    QStringList list;
    while ( query->next() )
    {
        id = query->value(0).toUInt();
        list = query->value(1).toString().split(';', QString::SkipEmptyParts);

        data.insert(id, list);
    }
}

void Dao::SaveSectorAirports(const QHash<quint32, QStringList> &data)
{
    if(DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QtAutoTransaction transaction(QSqlDatabase::database());
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();

    query->Prepare("DELETE FROM sectoring_airports");
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);

    foreach(quint32 id, data.keys())
    {
        query->Prepare(
                "INSERT INTO sectoring_airports "
                "(sector_id, airports) "
                "VALUES(:0, :1)");
        query->BindValue(":0", id);
        query->BindValue(":1", data[id].join(";"));
        TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    }
    transaction.commit();
}

void Dao::LoadAirportRunwaysByName(const QString &airportName, QStringList *runways)
{
    if(DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    QString sSelect =
            "select rw.WIDGET "
            "from runway rw, airport air "
            "where "
            "air.name = :0 and "
            "air.id = rw.airport_id and "
            "rw.WIDGET is not null";

    query->Prepare(sSelect);
    query->BindValue(":0", airportName);

    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);

    while ( query->next() )
    {
        if(!query->value(0).toString().isEmpty())
            runways->append(query->value(0).toString());
    }

}

void Dao::LoadMeteoRunway(const QString &airportName, QList<MeteoRunway> &meteos)
{
    if ( airportName.isEmpty() )
        return;

    if(DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    QString sSelect =
            "SELECT r.NAME, m.CONTROL_TIME, m.MAGNETIC_WIND, m.AVERAGE_SPEED, m.MAX_SPEED, m.SIDE_WIND, m.LONGITUDIAL_WIND, "
            "m.SIGHT_DIST, m.SDIST_START_OVI, m.SDIST_MIDDLE_OVI, m.SDIST_END_OVI, m.CLOUD_LOWER, m.PRESSURE_THRESHOULD, "
            "m.PRESSURE_SEA, m.TEMPERATURE, m.CONDENSATION_POINT "
            "FROM (SELECT r.name, r.id FROM runway r, airport air WHERE air.id = r.airport_id and air.name = :name) r "
            "LEFT JOIN meteo_runway m on m.runway_id = r.id";

    query->Prepare(sSelect);
    query->BindValue(":name", airportName);

    // The order of identifiers should be same as order of fields in the query.
    static int meteoIds[] = { 0,
                              MeteoRunway::ControlTime,
                              MeteoRunway::MagneticWind,
                              MeteoRunway::AverageSpeed,
                              MeteoRunway::MaxSpeed,
                              MeteoRunway::SideWind,
                              MeteoRunway::LongitudialWind,
                              MeteoRunway::SightDistance,
                              MeteoRunway::SDistanceStart,
                              MeteoRunway::SDistanceMiddle,
                              MeteoRunway::SDistanceEnd,
                              MeteoRunway::LowerCloudiness,
                              MeteoRunway::Pressure,
                              MeteoRunway::PressureSea,
                              MeteoRunway::Temperature,
                              MeteoRunway::CondensationPoint
                            };

    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);

    while(query->next())
    {
        MeteoRunway mr;
        mr.SetRunwayName( query->value(0).toString() );
        mr.SetAirportName(airportName);

        for ( int i = 1; i < 16; i++ )
        {
            QVariant v = query->value(i);
            if ( !v.isNull() )
                mr.insert( meteoIds[i], v );
        }

        meteos.append( mr );
    }
}

void Dao::LoadMeteoAirportSettings(const QString &airportName, QDateTime &lastEditTime,
                                   QString &currentRunway)
{
    if ( airportName.isEmpty() )
        return;

    if(DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    QString sSelect =
            "SELECT r.name, mas.last_edit "
            "FROM airport air, meteo_airport_stg mas, runway r "
            "WHERE air.id = mas.airport_id and r.id = mas.runway_id and air.name = :name";

    query->Prepare(sSelect);
    query->BindValue(":name", airportName);

    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);

    if ( query->next() )
    {
        currentRunway = query->value(0).toString();
        lastEditTime = query->value(1).toDateTime();
    }
}

void Dao::SaveMeteoRunway(const QList<MeteoRunway> &meteos)
{
    typedef QMap<uint, QString>     TRunwayMap;
    TRunwayMap updateRunways;

    // Make list of runwai id for update.
    {
        if(DBConnectionsManager::instance().getConnectionStatus() != 1)
            return;
        QSharedPointer<db::CDBQuery> query = db::CreateQuery();
        QString sSelect =
                "SELECT r.id, r.name "
                "FROM meteo_runway m, runway r "
                "WHERE m.runway_id = r.id";

        query->Prepare(sSelect);

        TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);

        while ( query->next() )
        {
            updateRunways.insert(
                        query->value(0).toUInt(),
                        query->value(1).toString());
        }
    }

    QtAutoTransaction transaction(QSqlDatabase::database());

    // first - MeteoRunway id, second - meteo_runway field name
    typedef QPair<int, QString>     TMeteoPair;
    // The order of identifiers should be same as order of fields in the query.
    const int countMeteoId = 15;
    static TMeteoPair meteoIds[countMeteoId] = {
        TMeteoPair(MeteoRunway::ControlTime, "control_time"),
        TMeteoPair(MeteoRunway::MagneticWind, "magnetic_wind"),
        TMeteoPair(MeteoRunway::AverageSpeed, "average_speed"),
        TMeteoPair(MeteoRunway::MaxSpeed, "max_speed"),
        TMeteoPair(MeteoRunway::SideWind, "side_wind"),
        TMeteoPair(MeteoRunway::LongitudialWind, "longitudial_wind"),
        TMeteoPair(MeteoRunway::SightDistance, "sight_dist"),
        TMeteoPair(MeteoRunway::SDistanceStart, "sdist_start_ovi"),
        TMeteoPair(MeteoRunway::SDistanceMiddle, "sdist_middle_ovi"),
        TMeteoPair(MeteoRunway::SDistanceEnd, "sdist_end_ovi"),
        TMeteoPair(MeteoRunway::LowerCloudiness, "cloud_lower"),
        TMeteoPair(MeteoRunway::Pressure, "pressure_threshould"),
        TMeteoPair(MeteoRunway::PressureSea, "pressure_sea"),
        TMeteoPair(MeteoRunway::Temperature, "temperature"),
        TMeteoPair(MeteoRunway::CondensationPoint, "condensation_point")
    };

    foreach ( const MeteoRunway &m, meteos )
    {
        QList<uint> keys = updateRunways.keys(m.GetRunwayName());

        QSharedPointer<db::CDBQuery> query = db::CreateQuery();

        // Insert of a new field should be done.
        if (  keys.empty() )
        {
            if ( !m.empty() )
            {
                QString sInsert =
                        "INSERT INTO meteo_runway "
                        "(runway_id, ";
                // making query string
                for ( int i = 0; i < countMeteoId; i++ )
                {
                    const TMeteoPair &meteoPair = meteoIds[i];
                    if ( m.contains(meteoPair.first) )
                        sInsert += QString("%1, ").arg(meteoPair.second);
                }
                // remove trailing comma and space.
                sInsert.remove(sInsert.length()-2, 2);
                sInsert += ") ";
                sInsert += "VALUES((SELECT r.id FROM airport air, runway r WHERE air.id = r.airport_id and air.name = :airName and r.name = :runwayName), ";
                // making query string
                for ( int i = 0; i < countMeteoId; i++ )
                {
                    const TMeteoPair &meteoPair = meteoIds[i];
                    if ( m.contains(meteoPair.first) )
                        sInsert += QString(":%1, ").arg(meteoPair.second);
                }
                // remove trailing comma and space.
                sInsert.remove(sInsert.length()-2, 2);
                sInsert += ")";

                qDebug() << sInsert;

                query->Prepare(sInsert);
                query->BindValue(":airName", m.GetAirportName());
                query->BindValue(":runwayName", m.GetRunwayName());
                // Binding values.
                for ( int i = 0; i < countMeteoId; i++ )
                {
                    const TMeteoPair &meteoPair = meteoIds[i];
                    if ( m.contains(meteoPair.first) )
                        query->BindValue(QString(":%1").arg(meteoPair.second), m.value(meteoPair.first));
                }

                TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
            }
        }
        else
        {
            uint runwayId = keys[0];

            QString sUpdate =
                    "UPDATE meteo_runway SET ";
            // making query string
            for ( int i = 0; i < countMeteoId; i++ )
            {
                const TMeteoPair &meteoPair = meteoIds[i];
//                if ( m.contains(meteoPair.first) )
                    sUpdate += QString("%1 = :%1, ").arg(meteoPair.second);
            }
            sUpdate.remove(sUpdate.length()-2, 2);
            sUpdate += " WHERE runway_id = :runwayId";

            qDebug() << sUpdate;

            query->Prepare(sUpdate);

            // Binding values.
            for ( int i = 0; i < countMeteoId; i++ )
            {
                const TMeteoPair &meteoPair = meteoIds[i];
                if ( m.contains(meteoPair.first) )
                    query->BindValue(QString(":%1").arg(meteoPair.second), m.value(meteoPair.first));
                else
                    query->BindValue(QString(":%1").arg(meteoPair.second), QVariant());
            }
            query->BindValue(":runwayId", runwayId);

            TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
        }
    }

    transaction.commit();
}

void Dao::SaveMeteoAirportSettings(const QString &airportName, const QDateTime &lastEditTime,
                                   const QString &currentRunway)
{
    if(DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    bool isInsertRequired = true;
    {
        QSharedPointer<db::CDBQuery> query = db::CreateQuery();
        QString sSelect =
                "SELECT airport_id "
                "FROM meteo_airport_stg mas, airport air "
                "WHERE air.id = mas.airport_id and air.name = :name";

        query->Prepare(sSelect);
        query->BindValue(":name", airportName);

        TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);

        if ( query->next() )
            isInsertRequired = false;
    }

    QtAutoTransaction transaction(QSqlDatabase::database());
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();

    // Make insert query
    if ( isInsertRequired )
    {
        QString sInsert =
                "INSERT INTO meteo_airport_stg "
                "(airport_id, runway_id, last_edit) "
                "VALUES((SELECT id FROM airport WHERE name = :airName1), "
                "(SELECT r.id FROM airport air, runway r WHERE r.name = :runwayName and air.id = r.airport_id and air.name = :airName2), "
                ":lastEdit)";

        query->Prepare(sInsert);
        query->BindValue(":airName1", airportName);
        query->BindValue(":airName2", airportName);
        query->BindValue(":runwayName", currentRunway);
        query->BindValue(":lastEdit", lastEditTime);
    }
    // Make update query
    else
    {
        QString sUpdate =
                "UPDATE meteo_airport_stg SET "
                "runway_id = (SELECT r.id FROM airport air, runway r WHERE r.name = :runwayName and air.id = r.airport_id and air.name = :airName1), "
                "last_edit = :lastEdit "
                "WHERE airport_id = (SELECT id FROM airport WHERE name = :airName2)";

        query->Prepare(sUpdate);
        query->BindValue(":airName1", airportName);
        query->BindValue(":airName2", airportName);
        query->BindValue(":runwayName", currentRunway);
        query->BindValue(":lastEdit", lastEditTime);
    }

    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);

    transaction.commit();
}

bool Dao::GetCentralPointForAllFIRs(CCoord& coord) const
{
    bool bRes = false;
    if(DBConnectionsManager::instance().getConnectionStatus() != 1)
        return bRes;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare("SELECT latitude, longitude FROM centralpointforallfirs");
    if(query->exec())
    {
        if(query->next())
        {
            coord.SetLatitude(CAngle(query->value(0).toDouble()));
            coord.SetLongitude(CAngle(query->value(1).toDouble()));
            if(!coord.IsNull())
                bRes = true;
        }
    }
    else
    {
        qDebug() << tr("Execution error. %1").arg(query->lastError().text());
    }
    return bRes;
}

quint32 Dao::GetIdAirport(const QString nameAirport)
{
    quint32 idAirport;
    if(DBConnectionsManager::instance().getConnectionStatus() != 1)
        return idAirport;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare(
            "SELECT id FROM airport "
            "WHERE name = :1");
    query->BindValue(":1", nameAirport);
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    if(query->next())
        idAirport = query->value(0).toInt();
    return idAirport;
}

QString Dao::GetNameAirport(const quint32 idAirport)
{
    QString nameAirport;
    if(DBConnectionsManager::instance().getConnectionStatus() != 1)
        return nameAirport;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare(
            "SELECT name FROM airport "
            "WHERE id = :1");
    query->BindValue(":1", idAirport);
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    if(query->next())
        nameAirport = query->value(0).toString();
    return nameAirport;
}

void Dao::GetOrcamList(const QString sector_name, QStringList &orcamList) const
{
    if(DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();

    if(sector_name == "Empty" || sector_name == "Custom")
        return;
    if(DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    query->Prepare(
                "SELECT orc.orcam_min_code, orc.orcam_max_code FROM orcam_range as orc "
                "WHERE orc.sectortype_id IS NULL or orc.sectortype_id = (SELECT type_id FROM sectoring WHERE name = :1)");
    query->BindValue(":1", sector_name);
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    while(query->next()) {
        QString minOrcam = query->value(0).toString();
        QString maxOrcam = query->value(1).toString();

        bool validCode;
        uint mMinCode = minOrcam.toInt(&validCode, 8);
        if (!validCode)
            mMinCode = 0;
        uint mMaxCode = maxOrcam.toInt(&validCode, 8);
        if (!validCode)
            mMaxCode = 4095; // code 7777

        if (mMinCode > mMaxCode)
            qSwap(mMinCode, mMaxCode);

        for(int i = mMinCode; i <= mMaxCode; i++){
            orcamList <<  QString::number(i, 8).rightJustified(4, '0');
        }
    }
}

void Dao::GetBearingList(QStringList &bearingList) const
{
    if(DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare(
                "SELECT a.name FROM airport as a "
                "WHERE a.id IN (SELECT bp.airport_id FROM bearing_point as bp)");

    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    while(query->next()) {
        bearingList << query->value(0).toString();
    }
}

void Dao::GetFrequencyList(const QString airport_name, QStringList &frequencyList) const
{
    if(DBConnectionsManager::instance().getConnectionStatus() != 1)
        return;
    QSharedPointer<db::CDBQuery> query = db::CreateQuery();
    query->Prepare(
                "SELECT bp.name FROM bearing_point as bp "
                "WHERE bp.airport_id = (SELECT a.id FROM airport as a WHERE a.name = :1)");

    query->BindValue(":1", airport_name);
    TVA_CHECK(query->exec(), tr("Execution error. %1").arg(query->lastError().text()), E_FAIL);
    while(query->next()) {
        frequencyList << query->value(0).toString();
    }
}


