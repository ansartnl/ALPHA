#include "redisdb.h"
//#include "main.h"

#include "QJson/parser.h"
#include "QJson/serializer.h"

#include <QCoreApplication>
#include <QThread>
#include <QTimer>
#include <QTimerEvent>
#include <QSharedPointer>
#include <QUuid>
#include <QDebug>

RedisDB::RedisDB(QObject *parent)
    : QObject(parent),
      mRedisSubscribe(0), mChannel("Unknown"), mCurrentRedis(0), isFirstReconnect(true), mHeartbeatTimer(0)
    , mReconnectTimer(0), mReconnectTimeout(5000), mHeartbeatInitiated(false), mReconnectInitiated(false), mFirstInit(false), mNotifExpire(false)
{
    mRedisPublishPool.append(QPair<bool, QtRedis*>(false, 0));

    /*if (parent) {
        if (parent->thread()->isRunning())
            mHeartbeatTimer = startTimer(mReconnectTimeout);
    } else if (qApp && qApp->thread()->isRunning()) {
        mHeartbeatTimer = startTimer(mReconnectTimeout);
    }*/

    mThread = new QThread();
    mWorker = new RedisConnectionsWorker();
    mConnectionStatus = 0; //Connect false

    mThread->setObjectName("RedisConnectionsManager");
}

RedisDB::~RedisDB()
{
    killTimer(mReconnectTimer);
    killTimer(mHeartbeatTimer);
    Close();
}

QPair<bool, QtRedis*>& RedisDB::GetPublisher()
{
    for(int i = 1; i < mRedisPublishPool.size() - 1; ++i)   // reserve last connection for heartbeat
    {
        QPair<bool, QtRedis*>& redisPublish = mRedisPublishPool[i];
        if(!redisPublish.first)
        {
            return redisPublish;
        }
    }
    if(mRedisPublishPool.size() > 1)
        qWarning() << "POOL is FULL";
    return mRedisPublishPool.first();
}

bool RedisDB::Init()
{
    Close();

    if(mParams.isEmpty())
        return false;

    if(mCurrentRedis < 0 || mCurrentRedis >= mParams.size())
        mCurrentRedis = 0;

    mRedisIp = mParams[mCurrentRedis].ip;
    int redisPort = mParams[mCurrentRedis].port;

    mRedisSubscribe = new QtRedis(mRedisIp, redisPort, this);
    connect(mRedisSubscribe, SIGNAL(returnData(QtRedis::Reply)), this, SLOT(onRedisData(QtRedis::Reply)));
    connect(mRedisSubscribe, SIGNAL(disconnected()), this, SLOT(onRedisSubscribeDisconnected()));
    bool bRes = mRedisSubscribe->openConnection();
    qDebug() << "Redis subscribe connected" << bRes;
    emit changedConnection(mRedisIp, bRes);

    mFirstInit = true;

    if(!bRes)
        return false;

    if(mRedisPublishPool.size() != 1)
    {
        qWarning() << "INITIAL PUBLISH POOL > 1, ERROR";
    }
    for(int i = 1; i <= PUBLISH_POOL_SIZE; ++i)
    {
        QtRedis* redisPublish = new QtRedis(mRedisIp, redisPort, this);
        connect(redisPublish, SIGNAL(disconnected()), this, SLOT(onRedisPublishDisconnected()));
        bool bRes = redisPublish->openConnection();
        qDebug() << i << "Redis publish connected" << bRes;
        mRedisPublishPool.append(QPair<bool, QtRedis*>(false, redisPublish));
    }

    if(!mChannel.isEmpty())
    {
        if(mRedisSubscribe && mRedisSubscribe->isConnected())
        {
            if(!mNotifExpire)
                mRedisSubscribe->subscribe(mChannel);
            else
                mRedisSubscribe->psubscribe("__key*__:*");
        }

        QPair<bool, QtRedis*>& redisPublish = GetPublisher();
        redisPublish.first = true;
        if(redisPublish.second && redisPublish.second->isConnected())
        {
            bRes = redisPublish.second->sadd(mTableChannel, mChannel);
            if(!bRes)
            {
//#ifdef QT_DEBUG
                QStringList list = redisPublish.second->smembers(mTableChannel);
                qDebug() << mTableChannel << "duplicate" << mChannel << list;
//#endif
            }
        }
        redisPublish.first = false;
    }
    else
    {
        return false;
    }

    mConnectionId == QUuid::createUuid();
    return true;
}

void RedisDB::Close()
{
    if(mRedisSubscribe)
    {
        disconnect(mRedisSubscribe, SIGNAL(returnData(QtRedis::Reply)), this, SLOT(onRedisData(QtRedis::Reply)));
        disconnect(mRedisSubscribe, SIGNAL(disconnected()), this, SLOT(onRedisSubscribeDisconnected()));
        if(mRedisSubscribe->isConnected())
        {
            mRedisSubscribe->unsubscribe(mChannel);
        }
        mRedisSubscribe->closeConnection();
        mRedisSubscribe->deleteLater();
        // Не удаляем из-за падений программы, если хост недоступен
//        mRedisSubscribe->deleteLater();
//        delete mRedisSubscribe;
        mRedisSubscribe = 0;
    }

    if(mRedisPublishPool.size() > 1)
    {
        QPair<bool, QtRedis*>& redisPublish = GetPublisher();
        redisPublish.first = true;
        if(redisPublish.second && redisPublish.second->isConnected())
        {
            redisPublish.second->srem(mTableChannel, mChannel);
        }
        redisPublish.first = false;
    }
    while(mRedisPublishPool.size() > 1)
    {
        QPair<bool, QtRedis*> redisPublish = mRedisPublishPool.takeLast();
        disconnect(redisPublish.second, SIGNAL(disconnected()), this, SLOT(onRedisPublishDisconnected()));
        redisPublish.second->closeConnection();
        redisPublish.second->deleteLater();
        // Не удаляем из-за падений программы, если хост недоступен
//        redisPublish.second->deleteLater();
//        delete redisPublish.second;
    }
}

void RedisDB::onRedisSubscribeDisconnected()
{
    qWarning() << "Redis subscribe disconnected";

    emit changedConnectionState(false);

    if(!mReconnectInitiated)
    {
        mReconnectInitiated = true;
        //mReconnectTimer = startTimer(mReconnectTimeout);
    }
}

void RedisDB::onRedisPublishDisconnected()
{
    qWarning() << "Redis publish disconnected";

    if(!mReconnectInitiated)
    {
        mReconnectInitiated = true;
        //mReconnectTimer = startTimer(mReconnectTimeout);
    }
}

void RedisDB::onRedisData(QtRedis::Reply data)
{
    QString sData = data.value.toString();

    if(data.channel == mChannel)
    {
        int ind = sData.indexOf(" ");
        if(ind != -1)
        {
            QString dtType = sData.left(ind);
            sData.remove(0, ind+1);

//            if(checkDT(dtType))
//            {
//                ind = sData.indexOf(" ");
//                if(ind != -1)
//                {
//                    QString otType = sData.left(ind);
//                    sData.remove(0, ind+1);

//                    if(checkOT(otType))
//                    {
//                        if(dtType == DT_SFPL)
//                        {
//                            QUuid uid(sData);
//                            if(!uid.isNull())
//                            {
//                                QSharedPointer<CSFPL> sfpl(new CSFPL);
//                                sfpl->SetUid(uid);
//                                if(otType != OT_DELETE)
//                                {
//                                    if(!loadSFPL(sfpl, uid))
//                                        return;
//                                }
//                                emit changedSFPL(sfpl, otType);
//                            }
//                        }
//                        else if(dtType == DT_RWF)
//                        {
//                            QString idFIR(sData);
//                            RunwayWidgetDataFIR rwf(idFIR);
//                            if(!loadRWF(idFIR, rwf))
//                                return;
//                            emit changedRWF(idFIR, rwf, otType);
//                        } else if (dtType == DT_RESTRICTION) {
//                            QUuid uid(sData);
//                            if (!uid.isNull()) {
//                                emit changedRestriction(uid, otType);
//                            }
//                        } else if(dtType == DT_SECTOR) {
//                            SectorRedis sr;
//                            if(!loadSectors(sr))
//                                return;
//                            emit changedSectors(sr, otType);
//                        }
//                    }
//                }
//            }
        }
    }
}

QStringList RedisDB::getSubscribers()
{
    QStringList list;
    QPair<bool, QtRedis*>& redisPublish = GetPublisher();
    redisPublish.first = true;
    if(redisPublish.second && redisPublish.second->isConnected())
    {
        QUuid curId = connectionId();

        list = redisPublish.second->smembers(mTableChannel);

        if (!isConnectionValid(curId))
            return list;
    }
    redisPublish.first = false;
    return list;
}

void RedisDB::onPublishToOther(const QString& sID, const QString& dtType, const QString& otType)
{
    //if(checkDT(dtType) && checkOT(otType))
    {
        QStringList list = getSubscribers();
        if(list.size())
        {
            QString mes = "\"" + dtType + " " + otType + " " + sID + "\"";
            foreach(const QString& subscriber, list)
            {
                QPair<bool, QtRedis*>& redisPublish = GetPublisher();
                redisPublish.first = true;
                if(redisPublish.second && redisPublish.second->isConnected())
                {
                    QUuid curId = connectionId();

                    redisPublish.second->publish(subscriber, mes);

                    if (!isConnectionValid(curId))
                        return;
                }
                redisPublish.first = false;
            }
        }
    }
}

void RedisDB::onTimer()
{
    killTimer(mReconnectTimer);

    qDebug() << "Redis reconnect attempt with First reconnect (" << isFirstReconnect << ") and Current Redis (" << mCurrentRedis << ")";
    if(isFirstReconnect)
    {
        isFirstReconnect = false;
    }
    else
    {
        ++mCurrentRedis;
        isFirstReconnect = true;
    }

    if(Init())
    {
        mReconnectInitiated = false;
        isFirstReconnect = true;
        qDebug() << "Redis reconnected to " << mParams[mCurrentRedis].ip << ":" << mParams[mCurrentRedis].port;
    }
    else
    {
        mReconnectTimer = startTimer(mReconnectTimeout);
    }
}

void RedisDB::heartbeat()
{
    if(mReconnectInitiated)
        return;

    if (mHeartbeatInitiated)
        return;

    mHeartbeatInitiated = true;

    QString mes = "ROLE";
    QtRedis::Reply data;

    QPair<bool, QtRedis*>& redisPublish = mRedisPublishPool[mRedisPublishPool.size() - 1];

    redisPublish.first = true;
    if(redisPublish.second && redisPublish.second->isConnected())
    {
        QUuid curId = connectionId();
        data = redisPublish.second->command(mes);

        if (!isConnectionValid(curId)) {
            mHeartbeatInitiated = false;
            return;
        }
    }
    redisPublish.first = false;

    QList<QVariant> values = data.value.toList();

    if(values.isEmpty() || values.first().toString() != "master")
    {
        if(!mReconnectInitiated)
        {
            mReconnectInitiated = true;
            mReconnectTimer = startTimer(mReconnectTimeout);
            isFirstReconnect = false;

            emit changedConnectionState(false);
        }
    }
    mHeartbeatInitiated = false;
}

void RedisDB::start()
{
    mWorker->moveToThread(mThread);

    connect(mThread, SIGNAL (started()), mWorker, SLOT (start()));
    connect(mWorker, SIGNAL (finished()), mThread, SLOT (quit()));
    connect(mWorker, SIGNAL (finished()), mWorker, SLOT (deleteLater()));
    connect(mThread, SIGNAL (finished()), mThread, SLOT (deleteLater()));

    connect(mWorker, SIGNAL(connectFailed(RedisParams)), SIGNAL(connectFailed(RedisParams)));
    connect(mWorker, SIGNAL(connectedParam(QString, int)), SLOT(onConnected(QString, int)));
    connect(mWorker, SIGNAL(connected()), SIGNAL(connected()));
    connect(mWorker, SIGNAL(disconnected()), SIGNAL(disconnected()));
    connect(mWorker, SIGNAL(error(QString)), SIGNAL(error(QString)));
    connect(mWorker, SIGNAL(chekConnectionStatus(QString, int)), SLOT(onChekConnectionStatus(QString, int)));
    connect(mWorker, SIGNAL(changeCurrentIndex(int)), SLOT(onChangeCurrentIndex(int)));

    mThread->start();
}

void RedisDB::onConnected(QString ip, int port)
{
    RedisParams p;
    p.ip = ip;
    p.port = port;

    if(Init())
    {
        mReconnectInitiated = false;
        qDebug() << "Redis reconnected to " << mParams[mCurrentRedis].ip << ":" << mParams[mCurrentRedis].port;
        emit connected();
        emit connected(p);
    }
}

void RedisDB::onChekConnectionStatus(QString ip, int status)
{
    if(getConnectionStatus() == status && mRedisIp == ip )
        return;
    setConnectionStatus(status);
    if(status == 1)
    {
        emit changedConnectionState(true);
        emit changedConnection(ip, true);
    }
    else
    {
        emit changedConnectionState(false);
        emit changedConnection(ip, false);
    }
    mRedisIp = ip;
}

void RedisDB::onChangeCurrentIndex(int index)
{
    mCurrentRedis = index;
}

void RedisDB::timerEvent(QTimerEvent *event)
{
    /*if(!mFirstInit) return;

    if(event->timerId() == mReconnectTimer)
        QMetaObject::invokeMethod(this, "onTimer", Qt::QueuedConnection);//onTimer();
    else if(event->timerId() == mHeartbeatTimer)
        QMetaObject::invokeMethod(this, "heartbeat", Qt::QueuedConnection);//heartbeat();*/
}

void RedisDB::publishToOther(const QString& sID, const QString& dtType, const QString& otType)
{
    //if(checkDT(dtType) && checkOT(otType))
    {
        QMetaObject::invokeMethod(this, "onPublishToOther", Qt::QueuedConnection, Q_ARG(const QString&, sID), Q_ARG(const QString&, dtType), Q_ARG(const QString&, otType));
    }
}

void RedisDB::publishToOther(const QUuid& uid, const QString& dtType, const QString& otType)
{
    if(!uid.isNull()/* && checkDT(dtType) && checkOT(otType)*/)
    {
        publishToOther(uid.toString(), dtType, otType);
    }
}

void RedisDB::addRedis(const QString &host, int port)
{
    addRedis(RedisParams(host, port));
}

void RedisDB::addRedis(const RedisParams &params)
{
    mParams.append(params);
    if(mCurrentRedis == -1)
        mCurrentRedis = 0;
}

void RedisDB::addDbParams(const RedisParams &p)
{
    mWorker->addDbParams(p);
}

void RedisDB::setDbAlias(const QString &alias)
{
    mWorker->setDbAlias(alias);
}

void RedisDB::setInteraval(int msec)
{
    mWorker->setInterval(msec);
    /*mReconnectTimeout = msec;
    killTimer(mHeartbeatTimer);
    mHeartbeatTimer = startTimer(msec);*/
}

int RedisDB::getConnectionStatus()
{
    return mConnectionStatus;
}

void RedisDB::setConnectionStatus(int status)
{
    mConnectionStatus = status;
}

void RedisDB::setChannel(const QString &channelName)
{
    mChannel = channelName;
}

const QString &RedisDB::channel() const
{
    return mChannel;
}

void RedisDB::setTableChannel(const QString &tableName)
{
    mTableChannel = tableName;
}

QString RedisDB::currentConnection() const
{
    return mRedisIp;
}

QUuid RedisDB::connectionId() const
{
    return mConnectionId;
}

bool RedisDB::isConnectionValid(const QUuid &uid) const
{
    return !mReconnectInitiated && uid == connectionId();
}

RedisConnectionsWorker::RedisConnectionsWorker(QObject *parent):
    mCurrentIndex(-1), isActive(false), isFirstReconnect(true)
{
    setObjectName("RedisConnectionsManager");

    db = new QtRedis(QString(), 0);
    timer = new QTimer(this);
    timer->setInterval(5000);
    connect(timer, SIGNAL(timeout()), SLOT(onTimer()));

    connect(this, SIGNAL(connected()), SLOT(onConnected()));

}

RedisConnectionsWorker::~RedisConnectionsWorker()
{
    timer->stop();
    removeDb();
}

void RedisConnectionsWorker::addDbParams(const RedisParams &p)
{
    mDbParams.append(p);
    if (mCurrentIndex == -1)
        mCurrentIndex = 0;
}

void RedisConnectionsWorker::setDbAlias(const QString &alias)
{
    mAlias = alias;
}

void RedisConnectionsWorker::setInterval(int msec)
{
    timer->setInterval(msec);
    //setConnectTimeout(db, msec / 1000);
}

bool RedisConnectionsWorker::isConnected() const
{
    QMutexLocker locker(&mutex);
    return isActive;
}

void RedisConnectionsWorker::start()
{
    if (mAlias.isEmpty())
        mAlias = QLatin1String(QUuid::createUuid().toByteArray());

    if (!fillDb())
        return;

    timer->start();
    onTimer();
}

void RedisConnectionsWorker::stop()
{
    timer->stop();
    setActive(false);
    emit finished();
}

void RedisConnectionsWorker::reconnect()
{
    if (isConnected())
    {
        setActive(false);
        emit disconnected();
    }
    if (!timer->isActive())
        start();
}

bool RedisConnectionsWorker::fillDb()
{
    removeDb();

    if (mDbParams.isEmpty())
    {
        emit error(tr("No db params"));
        return false;
    }

    if ((mCurrentIndex < 0) || (mCurrentIndex >= mDbParams.count()))
        mCurrentIndex = 0;

    changeCurrentIndex(mCurrentIndex);

    RedisParams p = mDbParams[mCurrentIndex];
    if(db)
        delete db;
    db = new QtRedis(p.ip, p.port);

    return true;
}

void RedisConnectionsWorker::removeDb()
{
    if (db->isConnected())
    {
        db->closeConnection();
        delete db;
    }
}

void RedisConnectionsWorker::setActive(bool active)
{
    QMutexLocker locker(&mutex);
    isActive = active;
}

bool RedisConnectionsWorker::checkRole()
{
    QString mes = "ROLE";
    QtRedis::Reply data;
    data = db->command(mes);
    QList<QVariant> values = data.value.toList();

    if(values.isEmpty() || values.first().toString() != "master")
    {
        return false;
    }
    return true;
}

void RedisConnectionsWorker::onTimer()
{
    bool openConnection = true;
    if(!db->isConnected())
        openConnection = db->openConnection();

    if (openConnection && checkRole())
    {
        emit chekConnectionStatus(mDbParams[mCurrentIndex].ip, 1);
        if (!isConnected())
        {
            setActive(true);
            emit connected();
        }

        isFirstReconnect = true;
    }
    else
    {
        db->disconnectHost();
        db->closeConnection();
        emit connectFailed(mDbParams[mCurrentIndex]);
        emit chekConnectionStatus(mDbParams[mCurrentIndex].ip, 0);

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

void RedisConnectionsWorker::onConnected()
{
    emit connectedParam(mDbParams[mCurrentIndex].ip, mDbParams[mCurrentIndex].port);
    if(isActive)
        emit chekConnectionStatus(mDbParams[mCurrentIndex].ip, 1);
}
