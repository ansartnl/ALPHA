#include "qtdatabaseconnector.h"
#include <QLibrary>

QtDatabaseConnector::QtDatabaseConnector(const QString &connectionName, QObject *parent) : QThread(parent)
{
    dbConnectionName = objectName() + "_trying_" + connectionName;
}

QtDatabaseConnector::~QtDatabaseConnector()
{
    clear();
}

void QtDatabaseConnector::clear()
{
    if (isRunning())
        wait();
    db = QSqlDatabase();
    QSqlDatabase::removeDatabase(dbConnectionName);
}

void QtDatabaseConnector::run()
{
    db = QSqlDatabase();
    QSqlDatabase::removeDatabase(dbConnectionName);
#ifdef QT_DEBUG
    if(driverName == "QOCI") {
        // fix Oracle bug for heap error
        QLibrary lib("oci");
        //lib.load();
    }
#endif
    db = QSqlDatabase::addDatabase(driverName, dbConnectionName);
    db.setDatabaseName(databaseName);
    db.setHostName(host);
    db.setPort(port);
    db.setUserName(login);
    db.setPassword(password);

    if (driverName == "QPSQL") {
        db.setConnectOptions("connect_timeout=5");
    }

    db.open();
}
