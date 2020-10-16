#ifndef _DB_READ_THREAD_H__
#define _DB_READ_THREAD_H__

#include <QtCore/QThread>
#include <QtCore/QMap>
#include <QtSql/QSqlDatabase>

#include "orm/ReFir.h"

namespace rewarn {

//! Read all firs and ares (all types + user restrictions) thread.
class DbReaderThread : public QThread
{
    Q_OBJECT
public:
    explicit DbReaderThread(TFirMap &firs, QObject *parent = 0);

    void stop() { mIsStopped = true; }
    bool isStopped() { return mIsStopped; }

    void setDatabaseParams(const QString &driver, const QString &host, int port,
                           const QString &dbName, const QString &user, const QString &password);

    QDateTime systime() const { return mSystime; }

protected:
    virtual void run();

    void load(QSqlDatabase &db);
    void load(QSqlDatabase &db, int id, ReFir &fir);
    void loadUser(QSqlDatabase &db, int id, ReFir &fir);
    void loadMeteo(QSqlDatabase &db, int id, ReFir &fir);
    void loadBoundaryCenter(QSqlDatabase &db, int id, ReFir &fir);
    void load(QSqlDatabase &db, int id, ReArea &area, const QString &tableName);
    void loadMeteoPoints(QSqlDatabase &db, int id, MeteoArea &area);
    QDateTime loadSystime(QSqlDatabase &db);

    //! Remove all rows from synchronization table.
    /*!
        \param interval Interval in the past in hours
    */
    void clearSyncTable(QSqlDatabase &db, int interval = 0);

    QSqlDatabase cloneDatabase(const QString &connectionName);
protected:
    /*! Reference to FIR map */
    TFirMap     &mFirs;
    /*! Cancel state */
    bool        mIsStopped;
    /*! Current database system time */
    QDateTime   mSystime;
    // Database parameters.
    QString     mDriver, mHost, mDbName, mUser, mPassword;
    int         mPort;
};


//! Read only areas that were modified (synchronization thread).
class DbReaderSyncThread : public DbReaderThread
{
    Q_OBJECT
public:
    typedef QList<quint32>          TIdList;
    typedef QMap<QString, quint32>  TFirAreaMap;

    explicit DbReaderSyncThread(TFirMap &firs, TIdList &idAreasForDelete, QObject *parent = 0);

    //! Sets interval to delete records from sync table in the past.
    void setCleanerInterval(int hours) { mCleanerInterval = hours; }

    //! Set last systime used to sync with the user restriction zones.
    void setSystime(const QDateTime &dt) { mSystime = dt; }

private:
    virtual void run();

    void load(QSqlDatabase &db);
    void load(QSqlDatabase &db, ReFir &fir, const TIdList &idAreaList);

    //! Load map of pairs <fir name - area id>.
    TFirAreaMap loadFirs(QSqlDatabase &db, const TIdList &idAreaList);

    //! Load synchronization identifiers based on type.
    TIdList loadSyncIds(QSqlDatabase &db, int type);

    //! Prepare where in clause
    QString prepareWhereInClause(const TIdList &idList) const;

protected:
    /*! List of id areas needed to delete */
    TIdList     &mIdAreasForDelete;
    /** Interval of cleaning sync records in the past */
    int         mCleanerInterval;
};

class DbReaderMeteoSyncThread : public DbReaderThread
{
    Q_OBJECT
public:
    explicit DbReaderMeteoSyncThread(TFirMap &firs, QObject *parent = 0);

private:
    virtual void run();

    void load(QSqlDatabase &db);
};

}   // namespace rewarn {

#endif // _DB_READ_THREAD_H__
