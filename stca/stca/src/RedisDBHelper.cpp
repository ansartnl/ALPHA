#include "RedisDBHelper.h"

const QString RedisDBHelper::DT_RESTRICTION = "RESTRICTION";
const QString RedisDBHelper::OT_INSERT = "insert";
const QString RedisDBHelper::OT_UPDATE = "update";
const QString RedisDBHelper::OT_DELETE = "delete";

RedisDBHelper::RedisDBHelper(QObject *parent) :
    RedisDB(parent)
{
}

RedisDBHelper::~RedisDBHelper()
{

}

void RedisDBHelper::onRedisData(QtRedis::Reply data)
{
    QString sData = data.value.toString();
    qDebug() << "RedisDB::onRedisData" << sData;
    if(data.channel == channel())
    {
        int ind = sData.indexOf(" ");
        if(ind != -1)
        {
            QString dtType = sData.left(ind);
            sData.remove(0, ind+1);

            ind = sData.indexOf(" ");
            if(ind != -1)
            {
                QString otType = sData.left(ind);
                sData.remove(0, ind+1);

                if(checkOT(otType))
                {
                    if (dtType == DT_RESTRICTION) {
                        QUuid uid(sData);
                        if (!uid.isNull()) {
                            emit restrictionsChanged();
                        }
                    }
                }
            }
        }
    }
}

bool RedisDBHelper::checkOT(const QString &otType) const
{
    if(otType != OT_INSERT)
            if(otType != OT_UPDATE)
                if(otType != OT_DELETE)
                    return false;

        return true;
}
