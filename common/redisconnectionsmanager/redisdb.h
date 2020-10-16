#ifndef REDIS_DATABASE_H
#define REDIS_DATABASE_H

#include "qtredis.h"
#include "sectorredis.h"

#include <QUuid>
#include <QSharedPointer>

#include <QtCore/QMutex>
#include <QtCore/QThread>

class QTimer;

struct RedisParams
{
    QString ip;
    int port;

    RedisParams()
        : port(0)
    {}

    RedisParams(const QString& ip, int port)
        : ip(ip), port(port)
    {}
};

typedef QList < RedisParams > RedisParamsList;

class RedisConnectionsWorker : public QObject
{
    Q_OBJECT
public:
    explicit RedisConnectionsWorker(QObject *parent = 0);
    virtual ~RedisConnectionsWorker();

    void addDbParams(const RedisParams &p);

    void setDbAlias(const QString &alias);
    void setInterval(int msec);

    bool isConnected() const;
    bool checkRole();

public slots:
    void start();
    void stop();

signals:
    void connected();
    void connectedParam(QString ip, int port);
    void connectFailed(const RedisParams &p);
    void disconnected();
    void error(const QString &err);
    void finished();
    void chekConnectionStatus(QString ip, int status);
    void changeCurrentIndex(int index);

protected:
    void reconnect();
    bool fillDb();
    void removeDb();

    void setActive(bool active);
    void setConnectTimeout(QtRedis *db, int secs);

protected slots:
    void onTimer();
    void onConnected();

protected:
    QTimer *timer;

    RedisParamsList mDbParams;
    int mCurrentIndex;

    QString mAlias;
    QtRedis *db;

    bool isActive;
    mutable QMutex mutex;

    bool isFirstReconnect;
};

//! Main class to work with redis database
class RedisDB: public QObject
{
    Q_OBJECT
public:

    //! Constructor
    RedisDB(QObject *parent=0);
    virtual ~RedisDB();

    bool Init();
    void Close();

    void addRedis(const QString& host, int port);
    void addRedis(const RedisParams& params);

    void addDbParams(const RedisParams &p);
    void setDbAlias(const QString &alias);
    void setInteraval(int msec);
    int getConnectionStatus();
    void setConnectionStatus(int status);

    void setChannel(const QString& channelName);
    const QString& channel() const;
    void setTableChannel(const QString& tableName);
    QString currentConnection() const;
    void setNotifExpire(){mNotifExpire = true;}

signals:
    void changedConnection(const QString &ip, bool connected);
    void changedConnectionState(bool connected);

    void connectFailed(const RedisParams &p);
    void disconnected();
    void error(const QString &err);
    void connected();
    void connected(const RedisParams &p);

protected slots:
    virtual void onRedisData(QtRedis::Reply data);

    void onRedisSubscribeDisconnected();
    void onRedisPublishDisconnected();

    void onPublishToOther(const QString& sID, const QString& dtType, const QString& otType);

    void onTimer();
    void heartbeat();

public slots:
    void start();
    void onConnected(QString ip, int port);
    void onChekConnectionStatus(QString ip, int status );
    void onChangeCurrentIndex(int index);

protected:
    void timerEvent(QTimerEvent *event);

    QStringList getSubscribers();
    void publishToOther(const QUuid& uid, const QString& dtType, const QString& otType);
    void publishToOther(const QString& sID, const QString& dtType, const QString& otType);

    QPair<bool, QtRedis*>& GetPublisher();

    QUuid connectionId() const;
    bool isConnectionValid(const QUuid& uid) const;

private:
    static const int PUBLISH_POOL_SIZE = 7;
    QList<QPair<bool, QtRedis*> > mRedisPublishPool;
    QtRedis* mRedisSubscribe;
    QString  mChannel;
    QString  mTableChannel;
    QString  mRedisIp;

    RedisParamsList mParams;
    int mCurrentRedis;
    bool isFirstReconnect;
    int mHeartbeatTimer;
    int mReconnectTimer;
    int mReconnectTimeout;
    bool mHeartbeatInitiated;
    bool mReconnectInitiated;
    bool mFirstInit;
    bool mNotifExpire;

    QUuid mConnectionId;

    QThread *mThread;
    RedisConnectionsWorker *mWorker;
    int mConnectionStatus;
};

#endif // REDIS_DATABASE_H
