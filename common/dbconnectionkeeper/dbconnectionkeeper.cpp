#include "dbconnectionkeeper.h"

#include <QtCore/QTimerEvent>
#include <QtCore/QMutexLocker>


DbConnectionKeeper::DbConnectionKeeper(const QSqlDatabase &db, quint32 inverval, QObject *parent)
    : QObject(parent), mDB(db), mIsActive(false), mReInverval(inverval), mTimerId(-1)
{
}

void DbConnectionKeeper::setDatabase(const QSqlDatabase &db)
{
    mDB = db;
}

void DbConnectionKeeper::setReconnectionInterval(quint32 inverval)
{
    if ( mTimerId != -1 )
        killTimer(mTimerId);
    mTimerId = startTimer(inverval);
    mReInverval = inverval;
}

void DbConnectionKeeper::start()
{
    setReconnectionInterval(mReInverval);
}

void DbConnectionKeeper::stop()
{
    if ( mTimerId != -1 )
        killTimer(mTimerId);
    mIsActive = false;
}

void DbConnectionKeeper::timerEvent(QTimerEvent* event)
{
    if ( event->timerId() == mTimerId )
    {
        if ( mMutex )
            mMutex->lock();

        // Do workaround for this - making transaction to check if there is a connection
        // to database.
        if ( mDB.transaction() )
        {
            mDB.rollback();
            if ( !mIsActive )
                emit connected();
            mIsActive = true;
        }
        else
        {
            if ( mIsActive )
            {
                emit disconnected();
                mIsActive = false;
            }
            else
            {
                mIsActive = mDB.open();  // try to restore the connection
                if ( mIsActive )
                    emit connected();
            }
        }

        if ( mMutex )
            mMutex->unlock();
    }

    QObject::timerEvent(event);
}
