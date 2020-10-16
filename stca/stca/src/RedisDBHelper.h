#ifndef REDISDBHELPER_H
#define REDISDBHELPER_H

#include "qtredis.h"
#include "redisdb.h"

class RedisDBHelper : public RedisDB
{
    Q_OBJECT
public:

    static const QString DT_RESTRICTION;
    //! Operation type constants
    static const QString OT_INSERT;
    static const QString OT_UPDATE;
    static const QString OT_DELETE;

    RedisDBHelper(QObject * parent = 0);
    virtual ~RedisDBHelper();

protected slots:
    void onRedisData(QtRedis::Reply data);

protected:
    bool checkOT(const QString& otType) const;

signals:
    void restrictionsChanged();
};

#endif // REDISDBHELPER_H
