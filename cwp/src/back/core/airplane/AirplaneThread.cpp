#include "AirplaneThread.h"

#include <QtCore/QMutexLocker>
#include <QtCore/QTime>
#include <QtCore/QDebug>


AirplaneThread::AirplaneThread(QObject *parent)
    : QThread(parent), mIsStopped(false)
{
}

void AirplaneThread::addIdList(const TListId &idList, SignalType type)
{
    QMutexLocker locker(&mDataMutex);

    if ( type == ForUpdate )
    {
        foreach ( quint16 id, idList )
        {
            if ( !mIds.contains(id) )
                mIds.insert(id, ForUpdate);
        }
    }
    else
    {
        foreach ( quint16 id, idList )
            mIds.insert(id, ForSet);
    }

    mWait.wakeAll();
}

void AirplaneThread::addId(quint16 id, SignalType type)
{
    QMutexLocker locker(&mDataMutex);

    if ( type == ForSet )
        mIds.insert(id, ForSet);
    else
    {
        if ( !mIds.contains(id) )
            mIds.insert(id, ForUpdate);
    }
    mWait.wakeAll();
}

void AirplaneThread::run()
{
    mIsStopped = false;

    qDebug() << tr("Airplane thread started successfully ...");

    static QMutex mutexAirplane;

    forever
    {
        if ( mIsStopped )
            break;

        QMutexLocker mutexAirplaneLocker(&mutexAirplane);
        if ( mIds.empty() )
            mWait.wait(&mutexAirplane);

        if ( mIsStopped )
            break;

        TListId updateIdList;
        TListId setIdList;
        {
            QMutexLocker locker(&mDataMutex);

            updateIdList = mIds.keys(ForUpdate);
            setIdList = mIds.keys(ForSet);
            mIds.clear();
        }

        if ( mIsStopped )
            break;

        qDebug() << QTime::currentTime().toString() <<
                    QString(" %1 of airplanes ready for update").arg(updateIdList.size());

        // Signal about UpdatePosition of airplane.
        foreach ( quint16 id, updateIdList )
        {
            emit signalUpdate(id);
        }
        // Signal about SetPosition of airplane.
        foreach ( quint16 id, setIdList )
        {
            emit signalSet(id);
        }

        qDebug() << QTime::currentTime().toString() << " All updates were successfully sent";
    }

    qDebug() << tr("Airplane thread is stopped ...");
}

void AirplaneThread::stop()
{
    mIsStopped = true;
    mWait.wakeAll();
}
