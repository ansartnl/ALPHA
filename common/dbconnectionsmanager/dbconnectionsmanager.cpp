#include <QtCore/QMetaType>
#include <QtCore/QProcess>
#include <QtCore/QTimer>
#include <QtCore/QUuid>
#include <QtSql/QSqlError>

#include "dbconnectionsmanager.h"

int type = qRegisterMetaType<DbParams>("DbParams");

DBConnectionsWorker::DBConnectionsWorker(QObject *parent) :
    mCurrentIndex(-1), isActive(false), isFirstReconnect(true)
{
    setObjectName("DBConnectionsManager");

    timer = new QTimer(this);
    timer->setInterval(5000);
    connect(timer, SIGNAL(timeout()), SLOT(onTimer()));

    connect(this, SIGNAL(connected()), SLOT(onConnected()));
}

DBConnectionsWorker::~DBConnectionsWorker()
{
    timer->stop();
    removeDb();
}

void DBConnectionsWorker::addDbParams(const DbParams &p)
{
    mDbParams.append(p);
    if (mCurrentIndex == -1)
        mCurrentIndex = 0;
}

void DBConnectionsWorker::setDbAlias(const QString &alias)
{
    mAlias = alias;
}

void DBConnectionsWorker::setInterval(int msec)
{
    timer->setInterval(msec);
    setConnectTimeout(db, msec / 1000);
}

bool DBConnectionsWorker::isConnected() const
{
    QMutexLocker locker(&mutex);
    return isActive;
}

void DBConnectionsWorker::start()
{
    if (mAlias.isEmpty())
        mAlias = QLatin1String(QUuid::createUuid().toByteArray());

    if (!fillDb())
        return;

    timer->start();
    onTimer();
}

void DBConnectionsWorker::reconnect()
{
    if (isConnected())
    {
        setActive(false);
        emit disconnected();
        emit changeConnectionStatus(0);
    }
    if (!timer->isActive())
        start();
}

void DBConnectionsWorker::stop()
{
    timer->stop();
    setActive(false);
    emit changeConnectionStatus(0);
    emit finished();
}

bool DBConnectionsWorker::fillDb()
{
    removeDb();

    if (mDbParams.isEmpty())
    {
        emit error(tr("No db params"));
        return false;
    }

    if ((mCurrentIndex < 0) || (mCurrentIndex >= mDbParams.count()))
        mCurrentIndex = 0;

    DbParams p = mDbParams[mCurrentIndex];

    db = QSqlDatabase::addDatabase(p.driver, mAlias);
    db.setHostName(p.host);
    db.setPort(p.port);
    db.setDatabaseName(p.databaseName);
    db.setUserName(p.user);
    db.setPassword(p.password);
    setConnectTimeout(db, timer->interval() / 1000);

    return true;
}

void DBConnectionsWorker::removeDb()
{
    if (db.isValid())
    {
        db = QSqlDatabase();
        db.removeDatabase(mAlias);
        //QSqlDatabase::removeDatabase(mAlias);
    }
}

void DBConnectionsWorker::setActive(bool active)
{
    QMutexLocker locker(&mutex);
    isActive = active;
}

void DBConnectionsWorker::onTimer()
{
    if (db.open())
    {
        emit changeConnectionStatus(1);
        db.close();
        if (!isConnected())
        {
            setActive(true);
            emit connected();
        }

        isFirstReconnect = true;
    } else
    {
        db.close();
        emit error(db.lastError().text());
        emit connectFailed(mDbParams[mCurrentIndex]);
        emit changeConnectionStatus(0);

        if (isConnected())
            reconnect();

        if (isFirstReconnect)
        {
            isFirstReconnect = false;
        }
        else
        {
            ++mCurrentIndex;
            isFirstReconnect = true;
            fillDb();
        }
    }

}

void DBConnectionsWorker::onConnected()
{
    emit connected(mCurrentIndex);
    emit connected(mDbParams[mCurrentIndex]);
}

void DBConnectionsWorker::setConnectTimeout(QSqlDatabase &db, int secs)
{
    if (!db.isValid())
        return;

    if (db.driverName() == "QPSQL") {
        db.setConnectOptions("connect_timeout=" + QString::number(secs));
    }
}

DBConnectionsManager::DBConnectionsManager(const QString &type)
{
    mThread = new QThread();
    mWorker = new DBConnectionsWorker();
    setConnectionStatus(0);

    mThread->setObjectName("DBConnectionsManager");

    QSqlDatabase::addDatabase(type, QSqlDatabase::defaultConnection);
}

DBConnectionsManager::~DBConnectionsManager()
{
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

DBConnectionsManager &DBConnectionsManager::instance(const QString &type)
{
    static DBConnectionsManager object(type);
    return object;
}

void DBConnectionsManager::addDbParams(const DbParams &p)
{
    mWorker->addDbParams(p);
}

void DBConnectionsManager::setDbAlias(const QString &alias)
{
    mWorker->setDbAlias(alias);
}

void DBConnectionsManager::setInterval(int msec)
{
    mWorker->setInterval(msec);
}

bool DBConnectionsManager::isConnected()
{
    QSqlDatabase db = QSqlDatabase::database();

    if (!db.isOpen())
        return false;

    QProcess process;
    process.start("ping", QStringList() << "-c" << "1" << "-W" << "1" << db.hostName());
    process.waitForFinished();
    return (process.exitCode() == 0);
}

int DBConnectionsManager::getConnectionStatus()
{
    return mConnectionStatus;
}

void DBConnectionsManager::setConnectionStatus(int status)
{
    mConnectionStatus = status;
}

void DBConnectionsManager::start()
{
    mWorker->moveToThread(mThread);

    connect(mThread, SIGNAL (started()), mWorker, SLOT (start()));
    connect(mWorker, SIGNAL (finished()), mThread, SLOT (quit()));
    connect(mWorker, SIGNAL (finished()), mWorker, SLOT (deleteLater()));
    connect(mThread, SIGNAL (finished()), mThread, SLOT (deleteLater()));

    connect(mWorker, SIGNAL(connectFailed(DbParams)), SIGNAL(connectFailed(DbParams)));
    connect(mWorker, SIGNAL(connected(DbParams)), SLOT(onConnected(DbParams)));
    connect(mWorker, SIGNAL(disconnected()), SIGNAL(disconnected()));
    connect(mWorker, SIGNAL(error(QString)), SIGNAL(error(QString)));
    connect(mWorker, SIGNAL(changeConnectionStatus(int)), SLOT(onChangeConnectionStatus(int)));

    mThread->start();
}

void DBConnectionsManager::start(const QString &dbAlias)
{
    setDbAlias(dbAlias);
    start();
}

void DBConnectionsManager::quit()
{
    QMetaObject::invokeMethod(mWorker, "stop", Qt::QueuedConnection);
    mThread->wait(5000);

    mWorker = 0;
    mThread = 0;
}

void DBConnectionsManager::onConnected(const DbParams &p)
{
    QSqlDatabase db = QSqlDatabase::database(QSqlDatabase::defaultConnection, false);
    db.close();

    db.setHostName(p.host);
    db.setPort(p.port);
    db.setDatabaseName(p.databaseName);
    db.setUserName(p.user);
    db.setPassword(p.password);

    db.open();

    emit connected();
    emit connected(p);
}

void DBConnectionsManager::onChangeConnectionStatus(int status)
{
    setConnectionStatus(status);
}
