#include "qtdbautoconnector.h"

#include <QtCore/QTimer>

#include <QtSql/QSqlError>

QtDbAutoConnector::QtDbAutoConnector(QObject *parent) :
    QThread(0), mCurrentIndex(-1), isActive(false)
{
    setObjectName("QtDbAutoConnector");

    moveToThread(this);

    timer = new QTimer(parent);
    timer->setInterval(5000);
    connect(timer, SIGNAL(timeout()), SLOT(onTimer()));

    connect(this, SIGNAL(connected()), SLOT(onConnected()));

    if (parent)
        connect(parent, SIGNAL(destroyed()), SLOT(deleteLater()));
}

QtDbAutoConnector::~QtDbAutoConnector()
{
    timer->stop();
    removeDb();
}

void QtDbAutoConnector::addDbParams(const DbParams &p)
{
    mDbParams.append(p);
    if (mCurrentIndex == -1)
        mCurrentIndex = 0;
}

void QtDbAutoConnector::clearDbParams()
{
    mDbParams.clear();
    mCurrentIndex = -1;
}

const DbParamsList QtDbAutoConnector::dbParams() const
{
    return mDbParams;
}

void QtDbAutoConnector::setDbAlias(const QString &alias)
{
    mAlias = alias;
}

void QtDbAutoConnector::setInteraval(int msec)
{
    timer->setInterval(msec);
}

bool QtDbAutoConnector::isConnected() const
{
    QMutexLocker locker(&mutex);
    return isActive;
}

const DbParams QtDbAutoConnector::currentParams() const
{
    if ((mCurrentIndex < 0) || (mCurrentIndex >= mDbParams.size()))
        return DbParams();
    return mDbParams[mCurrentIndex];
}

void QtDbAutoConnector::start()
{
    if (mAlias.isEmpty())
        mAlias = QLatin1String(QSqlDatabase::defaultConnection);
    if (!fillDb())
        return;
    QThread::start();
    timer->start();
    onTimer();
}

void QtDbAutoConnector::start(const QString &dbAlias)
{
    mAlias = dbAlias;
    start();
}

void QtDbAutoConnector::reconnect()
{
    if (isConnected())
    {
        setActive(false);
        emit disconnected();
    }
    if (!timer->isActive())
        start();
}

void QtDbAutoConnector::stop()
{
    timer->stop();
    setActive(false);
}

bool QtDbAutoConnector::fillDb()
{
    QMutexLocker locker(&mutex);

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

    return true;
}

void QtDbAutoConnector::removeDb()
{
    if (db.isValid())
    {
        db = QSqlDatabase();
        QSqlDatabase::removeDatabase(mAlias);
    }
}

void QtDbAutoConnector::setActive(bool active)
{
    QMutexLocker locker(&mutex);
    isActive = active;
}

void QtDbAutoConnector::onTimer()
{
    if (db.open())
    {
        db.close();
        if (!isConnected())
        {
            setActive(true);
            emit connected();
        }
    } else
    {
        emit error(db.lastError().text());
        if (isConnected())
            reconnect();
        ++mCurrentIndex;
        fillDb();
    }

}

void QtDbAutoConnector::onConnected()
{
    emit connected(mCurrentIndex);
    emit connected(mDbParams[mCurrentIndex]);
}
