#include "DbReadThread.h"
#include "qtautotransaction.h"

#include <QtCore/QDebug>
#include <QtCore/QVariant>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include <math.h>

#include "Database.h"

namespace rewarn {

static const QString g_connectionDbName = "WarnDbReader";

// Convert database user restriction type into ReType
static ReType toUserReType(int type)
{
    if ( type == int('O') )
        return enUserObstacle;
    else if ( type == int('Z') )
        return enUserNFZ;
    return enUser;
}


DbReaderThread::DbReaderThread(TFirMap &firs, QObject *parent)
    : QThread(parent), mFirs(firs), mIsStopped(false), mPort(0)
{
    setObjectName("db_reader");
}

void DbReaderThread::setDatabaseParams(const QString &driver, const QString &host, int port,
                                       const QString &dbName, const QString &user, const QString &password)
{
    mDriver = driver;
    mHost = host;
    mDbName = dbName;
    mUser = user;
    mPassword = password;
    mPort = port;
}

void DbReaderThread::run()
{
    mIsStopped = false;

    {
        QSqlDatabase db = cloneDatabase(objectName());

        if ( !db.open() )
        {
            qDebug() << tr("Failed to open database. Error: %1").arg(db.lastError().text());
            QSqlDatabase::removeDatabase(objectName());
            return;
        }

        if ( mIsStopped )
            return;

        qDebug() << tr("Database reading started ...");

        try
        {
            // Need this to do synchronization by database system time timestamp.
            mSystime = loadSystime(db);

            load(db);
        }
        catch( const QString &ex )
        {
            qDebug() << tr("Exception DB reader: ") << ex;
        }
    }
    QSqlDatabase::removeDatabase(objectName());
    qDebug() << tr("Database reading finished ...");

    if ( !mIsStopped && mFirs.empty() )
        qDebug() << tr("Database is empty - nothing to do ...");

    // If process was stopped - clear what we read.
    if ( mIsStopped )
        mFirs.clear();
}

QDateTime DbReaderThread::loadSystime(QSqlDatabase &db)
{
    QSharedPointer<db::CDBQuery> query = db::CreateQuery(db);
    QString sSelect = "SELECT " + query->GetSystimestampStr() + " " + query->GetFromDualStr();
    query->Prepare(sSelect);
    if ( !query->exec() )
        throw tr("%1. Query: %2").arg(query->lastError().text()).arg(sSelect);

    if ( query->next() )
        return query->value(0).toDateTime();
    return QDateTime();
}

void DbReaderThread::load(QSqlDatabase &db)
{
    QString sSelect = "SELECT id, id_name FROM zone WHERE type = 1";
    QSharedPointer<db::CDBQuery> query = db::CreateQuery(db);
    query->Prepare(sSelect);
    if ( !query->exec() )
        throw tr("%1. Query: %2").arg(query->lastError().text()).arg(sSelect);

    while ( query->next() )
    {
        if ( mIsStopped )
            return;

        int id = query->value(0).toUInt();
        QString name = query->value(1).toString();

        mFirs[name] = ReFir();
        ReFir &fir = mFirs[name];

        load(db, id, fir);
        loadUser(db, id, fir);
        loadMeteo(db, id, fir);

        loadBoundaryCenter(db, id, fir);
    }
}

void DbReaderThread::load(QSqlDatabase &db, int id, ReFir &fir)
{

    QString sSelect = "SELECT id, name, type, designation, multi_code, h_min, h_max "
                      "FROM restriction WHERE zone_id = :1";

    QSharedPointer<db::CDBQuery> query = db::CreateQuery(db);
    query->Prepare(sSelect);

    query->BindValue(":1", id);
    if ( !query->exec() )
        throw tr("%1. Query: %2").arg(query->lastError().text()).arg(sSelect);

    while ( query->next() )
    {
        if ( mIsStopped )
            return;

        fir.mAreas.append(ReArea());
        ReArea &area = fir.mAreas.back();

        area.mID = query->value(0).toUInt();
        area.mName = query->value(1).toString();
        area.mType = (ReType)query->value(2).toInt();
        area.mDesignation = query->value(3).toString();
        area.mZoneID = id;
        area.mMultiCode = query->value(4).toString();
        QVariant hMin = query->value(5);
        QVariant hMax = query->value(6);

        area.mHeightMin = hMin.isNull() ? -1e37 : hMin.toDouble();
        area.mHeightMax = hMax.isNull() ? 1e37 : hMax.toDouble();

        load(db, area.mID, area, "restriction_point");
    }
}

void DbReaderThread::loadUser(QSqlDatabase &db, int id, ReFir &fir)
{

    QString sSelect = "SELECT id, name, type, h_min, h_max, time_start, time_end, actual "
                      "FROM restriction_user WHERE zone_id = :1";


    QSharedPointer<db::CDBQuery> query = db::CreateQuery(db);
    query->Prepare(sSelect);

    query->BindValue(":1", id);
    if ( !query->exec() )
        throw tr("%1. Query: %2").arg(query->lastError().text()).arg(sSelect);

    while ( query->next() )
    {
        if ( mIsStopped )
            return;

        fir.mAreas.append(ReArea());
        ReArea &area = fir.mAreas.back();

        area.mID = query->value(0).toUInt();
        area.mName = query->value(1).toString();
        area.mType = toUserReType( query->value(2).toInt() );
        QVariant hMin = query->value(3);
        QVariant hMax = query->value(4);
        area.mTimeBegin = query->value(5).toDateTime();
        area.mTimeEnd = query->value(6).toDateTime();

        area.mHeightMin = hMin.isNull() ? -1e37 : hMin.toDouble();
        area.mHeightMax = hMax.isNull() ? 1e37 : hMax.toDouble();

        switch ( query->value(7).toInt() )
        {
            case 0: area.mState = ReArea::Disabled; break;
            case 1: area.mState = ReArea::Enabled; break;
            case 2: area.mState = ReArea::AlwaysEnabled; break;
        }

        load(db, area.mID, area, "restriction_point_user");
    }
}

void DbReaderThread::loadMeteo(QSqlDatabase &db, int id, ReFir &fir)
{
    QString sSelect = "SELECT id, name, type, h_min, h_max FROM meteo WHERE zone_id = :1";

    QSharedPointer<db::CDBQuery> query = db::CreateQuery(db);
    query->Prepare(sSelect);

    query->BindValue(":1", id);
    if (!query->exec())
        throw tr("%1. Query: %2").arg(query->lastError().text()).arg(sSelect);

    while (query->next())
    {
        if (mIsStopped)
            return;

        MeteoArea area;

        area.mID = query->value(0).toUInt();
        area.mName = query->value(1).toString();
        area.mType = (MeteoType)query->value(2).toInt();
        area.mZoneID = id;

        QVariant hMin = query->value(3);
        if (!hMin.isNull())
            area.mHeightMin = hMin.toDouble();

        QVariant hMax = query->value(4);
        if (!hMax.isNull())
            area.mHeightMax = hMax.toDouble();

        loadMeteoPoints(db, area.mID, area);

        fir.mMeteoAreas.append(area);
    }
}

void DbReaderThread::loadBoundaryCenter(QSqlDatabase &db, int id, ReFir &fir)
{

    QString sSelect = "SELECT type, longitude, latitude, arc_longitude, arc_latitude "
                      "FROM zone_boundary "
                      "WHERE zone_id = :1 ORDER BY sn";

    QSharedPointer<db::CDBQuery> query = db::CreateQuery(db);
    query->Prepare(sSelect);

    query->BindValue(":1", id);
    if ( !query->exec() )
        throw tr("%1. Query: %2").arg(query->lastError().text()).arg(sSelect);

    double lat = 0., lon = 0;
    int countPoints = 0;
    while ( query->next() )
    {
        if ( mIsStopped )
            return;

        RePoint::Type type = (RePoint::Type) query->value(0).toUInt();
        if ( type == RePoint::enLine )
        {
            lon += query->value(1).toDouble();
            lat += query->value(2).toDouble();
            countPoints++;
        }
        else if ( type == RePoint::enLeftArc || type == RePoint::enRightArc )
        {
            lon += query->value(3).toDouble();
            lat += query->value(4).toDouble();
            countPoints++;
        }
    }

    fir.mCenterLatitude = lat / countPoints;
    fir.mCenterLongitude = lon / countPoints;
}

void DbReaderThread::load(QSqlDatabase &db, int id, ReArea &area, const QString &tableName)
{

    QString queryStr = QString(
                "SELECT sn, type, latitude, longitude, arc_latitude, arc_longitude, arc_distance "
                "FROM %1 WHERE restriction_id = :1 "
                "ORDER BY sn").arg(tableName);

    QSharedPointer<db::CDBQuery> query = db::CreateQuery(db);
    query->Prepare(queryStr);

    query->BindValue(":1", id);
    if ( !query->exec() )
        throw tr("%1. Query: %2").arg(query->lastError().text()).arg(queryStr);

    while ( query->next() )
    {
        if ( mIsStopped )
            return;

        RePoint point;

        int sn = query->value(0).toInt();
        point.mType = (RePoint::Type) query->value(1).toInt();
        double lat = query->value(2).toDouble();
        double lon = query->value(3).toDouble();
        double latArc = query->value(4).toDouble();
        double lonArc = query->value(5).toDouble();
        point.mArcDistance = query->value(6).toDouble();

        point.mCoord = CCoord(lon, lat);
        point.mArcCoord = CCoord(lonArc, latArc);

        area.mPoints[sn] = point;
    }
}

void DbReaderThread::loadMeteoPoints(QSqlDatabase &db, int id, MeteoArea &area)
{
    QString queryStr = "SELECT sn, latitude, longitude FROM meteo_point WHERE meteo_id = :1 ORDER BY sn";

    QSharedPointer<db::CDBQuery> query = db::CreateQuery(db);
    query->Prepare(queryStr);

    query->BindValue(":1", id);
    if (!query->exec())
        throw tr("%1. Query: %2").arg(query->lastError().text()).arg(queryStr);

    while (query->next())
    {
        if (mIsStopped)
            return;

        RePoint point;

        int sn = query->value(0).toInt();
        point.mType = RePoint::enLine;
        point.mCoord = CCoord(query->value(2).toDouble(), query->value(1).toDouble());

        area.mPoints[sn] = point;
    }
}

void DbReaderThread::clearSyncTable(QSqlDatabase &db, int interval)
{
    QSharedPointer<db::CDBQuery> query = db::CreateQuery(db);
    QString queryStr = QString("DELETE FROM restriction_user_sync "
                               "WHERE lasttouch < " + query->GetSystimestampStr() + " - interval '%1' hour")
                                .arg(interval);

    query->Prepare(queryStr);

    if ( !query->exec() )
        throw tr("%1. Query: %2").arg(query->lastError().text()).arg(queryStr);
}

QSqlDatabase DbReaderThread::cloneDatabase(const QString &connectionName)
{
    return QSqlDatabase::cloneDatabase(QSqlDatabase::database(QSqlDatabase::defaultConnection, false),
                                       connectionName);
}

//////////////////////////////////////////////////////////////////////////
// DbReaderSyncThread implementation

DbReaderSyncThread::DbReaderSyncThread(TFirMap &firs, TIdList &idAreasForDelete, QObject *parent)
    : DbReaderThread(firs, parent), mIdAreasForDelete(idAreasForDelete), mCleanerInterval(0)
{
    setObjectName("db_reader_sync");
}

void DbReaderSyncThread::run()
{
    mIsStopped = false;

    mFirs.clear();
    mIdAreasForDelete.clear();
    {
        QSqlDatabase db = cloneDatabase(objectName());

        if ( !db.open() )
        {
            qDebug() << tr("Failed to open database. Error: %1").arg(db.lastError().text());
            QSqlDatabase::removeDatabase(objectName());
            return;
        }

        if ( mIsStopped )
            return;

#ifndef QT_NO_DEBUG
        qDebug() << tr("Database reading of modifications started ...");
#endif

        try
        {
            load(db);
        }
        catch( const QString &ex )
        {
            qDebug() << tr("Exception DB reader: ") << ex;
        }
    }
    QSqlDatabase::removeDatabase(objectName());

#ifndef QT_NO_DEBUG
    qDebug() << tr("Database reading of modifications finished ...");
#endif

    // If process was stopped - clear what we read.
    if ( mIsStopped )
    {
        mFirs.clear();
        mIdAreasForDelete.clear();
    }
}

void DbReaderSyncThread::load(QSqlDatabase &db)
{
    QtAutoTransaction transaction(db);

    QDateTime systimeNew = loadSystime(db);

    // Load identifiers of areas that were modified.
    TIdList idListForModify = loadSyncIds(db, 1);
    {
        TFirAreaMap firAreaMap = loadFirs(db, idListForModify);
        if ( !firAreaMap.isEmpty() )
        {
            foreach ( const QString nameFir, firAreaMap.uniqueKeys() )
            {
                if ( mIsStopped )
                    return;

                mFirs[nameFir] = ReFir();
                ReFir &fir = mFirs[nameFir];

                load(db, fir, firAreaMap.values(nameFir));
            }
        }
    }

    if ( mIsStopped )
        return;

    // Load identifiers of areas that were deleted.
    mIdAreasForDelete = loadSyncIds(db, 0);

    if ( mIsStopped )
        return;

    // Clearing table does not needed here as "restriction_user_sync" table is cleaned by the trigger in
    // database: RESTRICTION_USER.RESTRICTION_USER_CLEAN_TRG (Trigger deletes all modifications older
    // than 6 hours from now on).

    // Clear sync table.
//    clearSyncTable(db, mCleanerInterval);

//    if ( mIsStopped )
//        return;

    // Need to commit as table was cleared.
    if ( !mFirs.empty() || !mIdAreasForDelete.empty() )
    {
        transaction.commit();

        // Set new systime in order to not grab old updates from sync table.
        mSystime = systimeNew;
    }
}

void DbReaderSyncThread::load(QSqlDatabase &db, ReFir &fir, const TIdList &idAreaList)
{

    // Prepare where "in" clause.
    QString whereInStr = prepareWhereInClause(idAreaList);

    QString queryStr = QString(
                "SELECT ru.id, ru.name, ru.type, ru.h_min, ru.h_max, ru.time_start, ru.time_end, ru.actual "
                "FROM zone z, restriction_user ru "
                "WHERE z.id = ru.zone_id AND ru.id in (%1)").arg(whereInStr);

    QSharedPointer<db::CDBQuery> query = db::CreateQuery(db);
    query->Prepare(queryStr);

    if ( !query->exec() )
        throw tr("%1. Query: %2").arg(query->lastError().text()).arg(queryStr);

    while ( query->next() )
    {
        if ( mIsStopped )
            return;

        fir.mAreas.append(ReArea());
        ReArea &area = fir.mAreas.back();

        area.mID = query->value(0).toUInt();
        area.mName = query->value(1).toString();
        area.mType = toUserReType( query->value(2).toInt() );
        QVariant hMin = query->value(3);
        QVariant hMax = query->value(4);
        area.mTimeBegin = query->value(5).toDateTime();
        area.mTimeEnd = query->value(6).toDateTime();

        area.mHeightMin = hMin.isNull() ? -1e37 : hMin.toDouble();
        area.mHeightMax = hMax.isNull() ? 1e37 : hMax.toDouble();

        switch ( query->value(7).toInt() )
        {
            case 0: area.mState = ReArea::Disabled; break;
            case 1: area.mState = ReArea::Enabled; break;
            case 2: area.mState = ReArea::AlwaysEnabled; break;
        }

        DbReaderThread::load(db, area.mID, area, "restriction_point_user");
    }
}

DbReaderSyncThread::TFirAreaMap
DbReaderSyncThread::loadFirs(QSqlDatabase &db, const TIdList &idAreaList)
{
    TFirAreaMap firAreaMap;
    if ( idAreaList.empty() )
        return firAreaMap;

    // Prepare where "in" clause.
    QString whereInStr = prepareWhereInClause(idAreaList);

    QString queryStr = QString(
                "SELECT z.id_name, ru.id "
                "FROM zone z, restriction_user ru "
                "WHERE ru.zone_id = z.id and ru.id in (%1)").arg(whereInStr);

    QSharedPointer<db::CDBQuery> query = db::CreateQuery(db);
    query->Prepare(queryStr);

    if ( !query->exec() )
        throw tr("%1. Query: %2").arg(query->lastError().text()).arg(queryStr);

    while ( query->next() )
    {
        if ( mIsStopped )
            return TFirAreaMap();

        QString nameFir = query->value(0).toString();
        quint32 id = query->value(1).toInt();

        firAreaMap.insertMulti(nameFir, id);
    }

    return firAreaMap;
}

DbReaderSyncThread::TIdList
DbReaderSyncThread::loadSyncIds(QSqlDatabase &db, int type)
{
    // Nothing to do as select can not be correctly configured.
    if ( mSystime.isNull() )
        return TIdList();

    QString queryStr = QString(
                "SELECT id "
                "FROM restriction_user_sync WHERE %1 and lasttouch >= ?")
            .arg(type == 0 ? "type = 0" : "type != 0");

    QSharedPointer<db::CDBQuery> query = db::CreateQuery(db);
    query->Prepare(queryStr);

    query->AddBindValue(mSystime);

    if ( !query->exec() )
        throw tr("%1. Query: %2").arg(query->lastError().text()).arg(queryStr);

    TIdList ids;
    while ( query->next() )
    {
        if ( mIsStopped )
            return TIdList();

        quint32 id = query->value(0).toInt();
        ids << id;
    }
    return ids;
}

QString DbReaderSyncThread::prepareWhereInClause(const TIdList &idList) const
{
    QString whereInStr;
    // Prepare where "in" clause.
    foreach ( quint32 id, idList )
        whereInStr += QString::number(id) + ", ";
    if ( !whereInStr.isEmpty() )
        whereInStr.remove(whereInStr.length()-2, 2);
    return whereInStr;
}

//////////////////////////////////////////////////////////////////////////
// DbReaderMeteoSyncThread implementation

DbReaderMeteoSyncThread::DbReaderMeteoSyncThread(TFirMap &firs, QObject *parent)
    : DbReaderThread(firs, parent)
{
    setObjectName("db_reader_meteo_sync");
}

void DbReaderMeteoSyncThread::run()
{
    mIsStopped = false;

    mFirs.clear();
    {
        QSqlDatabase db = cloneDatabase(objectName());

        if ( !db.open() )
        {
            qDebug() << tr("Failed to open database. Error: %1").arg(db.lastError().text());
            QSqlDatabase::removeDatabase(objectName());
            return;
        }

        if ( mIsStopped )
            return;

#ifndef QT_NO_DEBUG
        qDebug() << tr("Database reading of meteo started ...");
#endif

        try
        {
            load(db);
        }
        catch( const QString &ex )
        {
            qDebug() << tr("Exception DB reader: ") << ex;
        }
    }
    QSqlDatabase::removeDatabase(objectName());

#ifndef QT_NO_DEBUG
    qDebug() << tr("Database reading of meteo finished ...");
#endif

    // If process was stopped - clear what we read.
    if ( mIsStopped )
    {
        mFirs.clear();
    }
}

void DbReaderMeteoSyncThread::load(QSqlDatabase &db)
{
    QString sSelect = "SELECT id, id_name FROM zone WHERE type = 1";
    QSharedPointer<db::CDBQuery> query = db::CreateQuery(db);
    query->Prepare(sSelect);
    if ( !query->exec() )
        throw tr("%1. Query: %2").arg(query->lastError().text()).arg(sSelect);

    while ( query->next() )
    {
        if ( mIsStopped )
            return;

        int id = query->value(0).toUInt();
        QString name = query->value(1).toString();

        mFirs[name] = ReFir();
        ReFir &fir = mFirs[name];

        loadMeteo(db, id, fir);
    }
}

}   // namespace rewarn {
