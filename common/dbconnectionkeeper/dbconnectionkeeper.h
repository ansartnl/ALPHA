#ifndef _DB_CONNECTION_KEEPER_H__
#define _DB_CONNECTION_KEEPER_H__

#include <QtCore/QObject>
#include <QtCore/QMutex>
#include <QtCore/QSharedPointer>
#include <QtSql/QSqlDatabase>
class QTimerEvent;

typedef QSharedPointer<QMutex>  TMutexPointer;


class DbConnectionKeeper : public QObject
{
    Q_OBJECT

public:
    explicit DbConnectionKeeper(const QSqlDatabase &db, quint32 inverval, QObject *parent = 0);

    //! Set global sync object.
    void setSharedMutex(const TMutexPointer &mutex) { mMutex = mutex; }

    //! Reconnection interval, secs.
    void setReconnectionInterval(quint32 inverval);

    void setDatabase(const QSqlDatabase &db);

    void setActive(bool active) { mIsActive = active; }
    bool isActive() const { return mIsActive; }

    QSqlDatabase database() const { return mDB; }

public slots:
    void start();
    void stop();

signals:
    void connected();
    void disconnected();

protected:
    void timerEvent(QTimerEvent* event);

private:
    /** Reference to the watched database connection */
    QSqlDatabase    mDB;
    /** When true - connection is open and active */
    bool            mIsActive;
    /** Reconnection interval, secs */
    quint32         mReInverval;
    /** Identificator of reconnection timer */
    int             mTimerId;
    /** Global synchronization object */
    TMutexPointer   mMutex;
};

#endif // _DB_CONNECTION_KEEPER_H__
