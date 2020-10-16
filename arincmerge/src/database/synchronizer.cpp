#include "synchronizer.h"

#include "qtautotransaction.h"

#include <QtCore/QDateTime>
#include <QtCore/QStringList>
#include <QtCore/QUuid>
#include <QtCore/QVariant>

#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>

#include <QtDebug>

const QString randName()
{
    return QUuid::createUuid().toString();
}

const QString Synchronizer::kConnectionName = "synchronization_connection";

Synchronizer::Synchronizer(QObject *parent, const QSqlDatabase &db_) :
    QObject(0), db(QSqlDatabase::cloneDatabase(db_, kConnectionName))
{
    if (parent)
        connect(parent, SIGNAL(destroyed()), SLOT(deleteLater()));

    db.open();

    qRegisterMetaTypeStreamOperators < ServerInfo >();
}

Synchronizer::~Synchronizer()
{
    db.close();
    db = QSqlDatabase();
    QSqlDatabase::removeDatabase(kConnectionName);
    foreach (const ServerInfo &info, mServers) {
        mServers[info.identifier()].db.close();
        mServers[info.identifier()].db = QSqlDatabase();
    }
    foreach (const ServerInfo &info, mServers) {
        QSqlDatabase::removeDatabase(info.connectionName);
    }
}

void Synchronizer::addServer(const ServerInfo &info)
{
    mServers.insert(info.identifier(), info);

    QSqlDatabase &sdb = mServers[info.identifier()].db;
    sdb = QSqlDatabase::addDatabase(info.driver, info.connectionName);
    sdb.setHostName(info.host);
    sdb.setPort(info.port);
    sdb.setDatabaseName(info.databaseName);
    sdb.setUserName(info.user);
    sdb.setPassword(info.password);
    if (info.driver == "QPSQL")
        sdb.setConnectOptions("connect_timeout=5");

    if (!sdb.open()) {
        onError(tr("Error connecting to '%1': '%2'").arg(info.humanReadable()).arg(sdb.lastError().text()));
        emit changeLastSynchronization(info.identifier(), tr("Could not connect"));
        return;
    }

    QSqlQuery query(sdb);
    query.prepare("SELECT \"time\" FROM last_sync");
    if (!query.exec()) {
        onError(query);
        emit changeLastSynchronization(info.identifier(), tr("SQL error"));
        return;
    }

    if (!query.next()) {
        emit changeLastSynchronization(info.identifier(), tr("Never synchronized"));
        return;
    }

    emit changeLastSynchronization(info.identifier(), query.value(0).toDateTime()
                                   .toString("yyyy.MM.dd hh:mm:ss")
                                   );
}

void Synchronizer::synchronize(const QString &id)
{
    Q_ASSERT(mServers.contains(id));

    emit changeStatus(id, tr("Synchronizing..."));

    QSqlDatabase sdb = mServers[id].db;
    if (!sdb.isOpen() && !sdb.open()) {
        onError(tr("Error connecting to '%1': '%2'")
                .arg(mServers[id].humanReadable())
                .arg(sdb.lastError().text()));
        emit changeStatus(id, tr("Could not connect"));
        return;
    }

    if (sdb.hostName() == db.hostName()
            && sdb.port() == db.port()
            && sdb.databaseName() == db.databaseName()
            //&& sdb.userName() == db.userName()
            ) {
        emit changeStatus(id, tr("Trying to synchronize with myself."));
        return;
    }

    QtAutoTransaction autoTransaction(sdb);

    const QStringList tables = allTables(db);

    int counter = 0;
    foreach (const QString &tableName, tables) {
        const QSqlRecord record = db.record(tableName);
        const QSqlRecord srecord = sdb.record(tableName);
        QStringList fields;
        for (int i = 0; i < record.count(); ++i) {
            const QString fieldName = record.fieldName(i);
            if (!srecord.contains(fieldName)) {
                onError(tr("Table '%1' of database '%2' does not contain field '%3'. "
                           "Cancelling synchronization.")
                        .arg(tableName)
                        .arg(mServers[id].humanReadable())
                        .arg(fieldName)
                        );
                emitError(id, counter, tables.count());
                return;
            }
            fields << fieldName;
        }
        if (!disableTriggers(sdb, tableName)) {
            emitError(id, counter, tables.count());
            return;
        }
        if (!clearTable(sdb, tableName)) {
            emitError(id, counter, tables.count());
            return;
        }
        if (!copyTable(sdb, tableName, fields)) {
            emitError(id, counter, tables.count());
            return;
        }
        if (!enableTriggers(sdb, tableName)) {
            emitError(id, counter, tables.count());
            return;
        }
        ++counter;
        emit changeStatus(id, tr("Synchronizing... %1/%2").arg(counter).arg(tables.count()));
    }

    const QDateTime time = QDateTime::currentDateTimeUtc();
    if (!updateLastSynchronization(sdb, time)) {
        emitError(id);
        return;
    }

    if (!autoTransaction.commit()) {
        onError(tr("Could not commit transaction. SQL error '%1'").arg(sdb.lastError().text()));
        emitError(id);
        return;
    }

    emit changeStatus(id, tr("Finished."));
    emit changeLastSynchronization(id, time.toString("yyyy.MM.dd hh:mm:ss"));
}

const QStringList Synchronizer::allTables(const QSqlDatabase &sdb)
{
    QStringList ret;

    const QStringList tables = sdb.tables(QSql::Tables);
    foreach (const QString &table, tables) {
        ret << table.mid(table.lastIndexOf('.') + 1);
    }

    return ret;
}

bool Synchronizer::disableTriggers(const QSqlDatabase &sdb, const QString &tableName)
{
    return execute(sdb, QString("ALTER TABLE \"%1\".\"%2\" DISABLE TRIGGER ALL")
                   .arg(sdb.userName()).arg(tableName));
}

bool Synchronizer::enableTriggers(const QSqlDatabase &sdb, const QString &tableName)
{
    return execute(sdb, QString("ALTER TABLE \"%1\".\"%2\" ENABLE TRIGGER ALL")
                   .arg(sdb.userName()).arg(tableName));
}

bool Synchronizer::clearTable(const QSqlDatabase &sdb, const QString &tableName)
{
    return execute(sdb, QString("DELETE FROM \"%1\"").arg(tableName));
}

bool Synchronizer::copyTable(const QSqlDatabase &sdb, const QString &tableName, const QStringList &fields)
{
    QSqlQuery select(db);
    select.prepare(QString("SELECT \"%1\" FROM \"%2\"").arg(fields.join("\", \"")).arg(tableName));
    if (!select.exec()) {
        onError(select);
        return false;
    }

    QSqlQuery insert(sdb);
    insert.prepare(QString("INSERT INTO \"%1\" (\"%2\") VALUES (%3)")
                   .arg(tableName)
                   .arg(fields.join("\", \""))
                   .arg(QString(fields.count(), '?').split("", QString::SkipEmptyParts).join(", ")));

    while (select.next()) {
        for (int i = 0; i < fields.count(); ++i)
            insert.addBindValue(select.value(i));
        if (!insert.exec()) {
            onError(insert);
            return false;
        }
    }
    return true;
}

bool Synchronizer::updateLastSynchronization(const QSqlDatabase &sdb, const QDateTime &time)
{
    QSqlQuery query(sdb);
    if (!query.exec("SELECT * FROM last_sync WHERE id = 1")) {
        onError(query);
        return false;
    }

    if (query.next()) {
        query.prepare("UPDATE last_sync SET \"time\" = :time WHERE id = 1");
    } else {
        query.prepare("INSERT INTO last_sync (id, \"time\") VALUES (1, :time)");
    }
    query.bindValue(":time", time);
    if (!query.exec()) {
        onError(query);
        return false;
    }

    return true;
}

bool Synchronizer::execute(const QSqlDatabase &sdb, const QString &queryString)
{
    QSqlQuery query(sdb);
    if (!query.exec(queryString)) {
        onError(query);
        return false;
    }
    return true;
}

void Synchronizer::emitError(const QString &id, int counter, int total)
{
    emit changeStatus(id, tr("Error occurred after copying %1/%2 tables. See log for more info.")
                      .arg(counter).arg(total));
}

void Synchronizer::emitError(const QString &id)
{
    emit changeStatus(id, tr("Error occurred. See log for more info."));
}

void Synchronizer::onError(const QSqlQuery &query)
{
    onError(tr("Error '%1' while executing '%2'").arg(query.lastError().text()).arg(query.lastQuery()));
}

void Synchronizer::onError(const QString &text)
{
    qWarning() << text;
}
