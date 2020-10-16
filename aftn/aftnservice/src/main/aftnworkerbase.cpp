#include "aftnworkerbase.h"

#include "qtxmlsettings.h"

#include <QtDebug>

#include <QtCore/QSettings>
#include <QtCore/QDir>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include <QLibrary>

struct Sleep : public QThread { using QThread::msleep;
    using QThread::sleep;};

AFTNWorkerBase::AFTNWorkerBase(QObject* parent)
    : QThread(parent)
{
}

void WorkerBase::init()
{
    // ----------------------------------------------------------------------
    // Reserving sql connection
    mConnectionName = QString("%1_n_%2").arg(metaObject()->className()).arg(
            qrand());
    while (QSqlDatabase::contains(mConnectionName))
        mConnectionName = QString("%1_n_%2").arg(metaObject()->className()).arg(
                qrand());

    connectToDatabase();
    // ----------------------------------------------------------------------

    dbTimerId = startTimer(5000);
}

WorkerBase::WorkerBase(QObject* parent)
    :QObject(parent)
    , mSettings(new QtXmlSettings(QDir(QCoreApplication::applicationDirPath()).filePath(qApp->applicationName() + ".xml"), this))
{
}

WorkerBase::~WorkerBase()
{
    if (QSqlDatabase::database(connectionName()).isValid())
        QSqlDatabase::removeDatabase(connectionName());
}

void WorkerBase::connectToDatabase()
{
    qloginfo(tr("Connecting to database..."));

    mSettings->beginGroup("db");
    QString driver = mSettings->value("driver").toString();
    QString host = mSettings->value("host").toString();
    QString name = mSettings->value("name").toString();
    int port = mSettings->value("port").toInt();
    QString user = mSettings->value("user").toString();
    QString password = mSettings->value("password").toString();
    int attempts = mSettings->value("connect_timeout", 5).toInt();
    mSettings->endGroup();

    QSqlDatabase mDatabase;
    if (QSqlDatabase::contains(connectionName()))
        mDatabase = QSqlDatabase::database(connectionName());
    else
    {
        if(driver == "QOCI")
        {
            // fix Oracle bug for heap error
            QLibrary lib("oci");
            //lib.load();
        }
        mDatabase = QSqlDatabase::addDatabase(driver, connectionName());
    }
    mDatabase.setHostName(host);
    mDatabase.setDatabaseName(name);
    mDatabase.setPort(port);
    mDatabase.setUserName(user);
    mDatabase.setPassword(password);


    int wait = 3;
    attempts = (attempts*60)/3;
    bool success = false;
    while (attempts) {
        if (mDatabase.open()) {
            success = true;
            break;
        }
        Sleep::msleep(wait * 1000);
        --attempts;
    }
    if (!success) {
        qlogerror(tr("Could not connect to database\nError: %1").arg(mDatabase.lastError().text()));
        return;
    }

    QSqlQuery query(mDatabase);
    query.prepare("INSERT INTO LOGIN(USER_ID, USER_NAME) VALUES(?, ?)");
    query.addBindValue(0);
    query.addBindValue("aftnservice");
    // eat
    if (!query.exec()) {
        qlogerror(tr("Query execution error.\nExecuted query: %1\nError: %2")
                  .arg(query.lastQuery(), query.lastError().text()));
    }
    if (!query.numRowsAffected()) {
        qlogwarn(tr("Authorization failed."));
    }

    qloginfo(tr("Database connected"));
}

void WorkerBase::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == dbTimerId) {
        if (!QSqlDatabase::contains(connectionName())) {
            qlogwarn(tr("Database connection lost"));
            connectToDatabase();
        } else if (!QSqlDatabase::database(connectionName()).isOpen()) {
            // Restore database connection
            qlogwarn(tr("Database connection lost"));
            connectToDatabase();
        }
    }
    QObject::timerEvent(event);
}
