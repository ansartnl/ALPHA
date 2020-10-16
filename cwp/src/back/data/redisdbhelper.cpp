#include "redisdbhelper.h"
#include "main.h"

#include "QJson/parser.h"
#include "QJson/serializer.h"

#include <QTimer>
#include <QTimerEvent>
#include <QSharedPointer>
#include <QUuid>
#include <QDebug>

const QString RedisDBHelper::TABLE_CHANNEL = "cwp_channel";
const QString RedisDBHelper::TABLE_SECTOR = "cwp_sector";
const QString RedisDBHelper::TABLE_SFPL = "cwp_sfpl";
//const QString RedisDBHelper::TABLE_RWF = "cwp_rwf";
const QString RedisDBHelper::TABLE_ORCAM = "code_orcam";

const QString RedisDBHelper::DT_SFPL = "SFPL";
//const QString RedisDBHelper::DT_RWF = "RWF";
const QString RedisDBHelper::DT_SECTOR = "SECTOR";
const QString RedisDBHelper::DT_RESTRICTION = "RESTRICTION";
const QString RedisDBHelper::DT_ORCAM = "ORCAM";

const QString RedisDBHelper::OT_INSERT = "insert";
const QString RedisDBHelper::OT_UPDATE = "update";
const QString RedisDBHelper::OT_DELETE = "delete";

RedisDBHelper::RedisDBHelper(const QString &client, QObject *parent)
    : RedisDB(parent), mClient(client), mExpireTimer(new QTimer(this))
{
    //mExpireTimer->setInterval(10 * 1000);
    //connect(mExpireTimer, SIGNAL(timeout()), SLOT(updateExpire()));
    //mExpireTimer->start();
}

RedisDBHelper::~RedisDBHelper()
{

}


bool RedisDBHelper::saveSFPL(const QSharedPointer<CSFPL>& sfpl)
{
    bool bOk = false;
    if(sfpl)
    {
        QUuid uid = sfpl->GetUid();
        QString otType = OT_UPDATE;
        if(uid.isNull())
        {
            otType = OT_INSERT;
            uid = QUuid::createUuid();
        }
        sfpl->SetUid(uid);

        QVariantMap map = sfpl->toVariantMap();
        QJson::Serializer serializer;
        QByteArray json = serializer.serialize(map, &bOk);
        if(bOk)
        {
            QPair<bool, QtRedis*>& redisPublish = GetPublisher();
            redisPublish.first = true;
            if(redisPublish.second && redisPublish.second->isConnected())
            {
                QUuid curId = connectionId();

                QString sKey = TABLE_SFPL + ":" + uid.toString();
                int ttl = redisPublish.second->ttl(sKey);
                if (!isConnectionValid(curId))
                    return false;

                if (ttl < 0)
                    ttl = 3*60*60;
                QString sValue = json;
                sValue.replace("\"", "\\\"");
                sValue = "\"" + sValue + "\"";
                bOk = redisPublish.second->set(sKey, sValue);
                if (!isConnectionValid(curId))
                    return false;

                if (bOk)
                {
                    if (otType == OT_INSERT)
                        redisPublish.second->expire(sKey, 3*60*60); // sfpl lifetime 3 hours
                    else
                        redisPublish.second->expire(sKey, ttl);
                }
                if (!isConnectionValid(curId))
                    return false;
            }
            else
            {
                redisPublish.first = false;
                if(otType == OT_INSERT)
                {
                    sfpl->SetUid(QUuid());
                }
                return false;
            }
            redisPublish.first = false;
        }

        if(bOk)
        {
            publishToOther(uid, DT_SFPL, otType);
        }
        else
        {
            qWarning() << "error save SFPL" << uid.toString();
            if(otType == OT_INSERT)
            {
                sfpl->SetUid(QUuid());
            }
        }
    }
    return bOk;
}

bool RedisDBHelper::deleteSFPL(const QSharedPointer<CSFPL>& sfpl)
{
    bool bRes = false;
    QPair<bool, QtRedis*>& redisPublish = GetPublisher();
    redisPublish.first = true;
    if(sfpl && !sfpl->GetUid().isNull() && redisPublish.second && redisPublish.second->isConnected())
    {
        QUuid curId = connectionId();

        QString sKey = TABLE_SFPL + ":" + sfpl->GetUid().toString();
        redisPublish.second->del(sKey);

        if (!isConnectionValid(curId))
            return false;

        ////////if( redisPublish.second->isValidReply(reply) )
            bRes = true;
    }
    redisPublish.first = false;
    if(bRes)
    {
        publishToOther(sfpl->GetUid().toString(), DT_SFPL, OT_DELETE);
    }
    else
    {
        if(sfpl && !sfpl->GetUid().isNull())
        {
            qWarning() << "error delete SFPL" << sfpl->GetUid().toString();
        }
    }
    return bRes;
}

bool RedisDBHelper::loadSFPL(QSharedPointer<CSFPL>& sfpl, const QUuid& uid)
{
    bool bRes = false;
    QPair<bool, QtRedis*>& redisPublish = GetPublisher();
    redisPublish.first = true;
    if(sfpl && !uid.isNull() && redisPublish.second && redisPublish.second->isConnected())
    {
        QUuid curId = connectionId();

        QString sKey = TABLE_SFPL + ":" + uid.toString();
        QString sValue = redisPublish.second->get(sKey);

        if (!isConnectionValid(curId))
            return false;

        if(!sValue.isEmpty())
        {
            QJson::Parser parser;
            bool bOk = false;
            QByteArray arr;
            arr.append(sValue);
            QVariant var = parser.parse(arr, &bOk);
            if(bOk)
            {
                QVariantMap map = var.value<QVariantMap>();
                if(map.size())
                {
                    sfpl->fromVariantMap(map);
                    bRes = true;
                }
            }
        }
    }
    redisPublish.first = false;
    return bRes;
}

bool RedisDBHelper::loadSFPLs(QList<QSharedPointer<CSFPL> >& list)
{
    bool bRes = true;
    QStringList listKeys;
    QPair<bool, QtRedis*>& redisPublish = GetPublisher();
    redisPublish.first = true;
    if(redisPublish.second && redisPublish.second->isConnected())
    {
        QUuid curId = connectionId();

        QStringList listKeysTmp = redisPublish.second->keys(TABLE_SFPL + ":*");

        if (!isConnectionValid(curId))
            return false;

        foreach(const QString& sKeyTmp, listKeysTmp)
        {
            QString sKey(sKeyTmp);
            sKey.replace(QString(TABLE_SFPL + ":"), QString(""));
            listKeys.append(sKey);
        }
    }
    else
    {
        bRes = false;
    }
    redisPublish.first = false;
    foreach(const QString& sKey, listKeys)
    {
        QSharedPointer<CSFPL> sfpl(new CSFPL);
        bool bOk = loadSFPL(sfpl, QUuid(sKey));
        if(bOk)
        {
            list.append(sfpl);
        }
        else
        {
            qWarning() << "error load SFPL" << sKey;
        }
    }
    return bRes;
}

void RedisDBHelper::onRedisData(QtRedis::Reply data)
{
    QString sData = data.value.toString();

    if(data.channel == channel())
    {
        int ind = sData.indexOf(" ");
        if(ind != -1)
        {
            QString dtType = sData.left(ind);
            sData.remove(0, ind+1);
            if(checkDT(dtType))
            {
                ind = sData.indexOf(" ");
                if(ind != -1)
                {
                    QString otType = sData.left(ind);
                    sData.remove(0, ind+1);
                    if(checkOT(otType))
                    {
                        if(dtType == "SFPL" || dtType == "SFPL_rtor")
                        {
                            QUuid uid(sData);

                            if(!uid.isNull())
                            {
                                QSharedPointer<CSFPL> sfpl(new CSFPL);
                                sfpl->SetUid(uid);
                                if(otType != OT_DELETE)
                                {
                                    if(!loadSFPL(sfpl, uid))
                                        return;
                                }
                                emit changedSFPL(sfpl, otType);
                            }
                        }

                        else if(dtType == "RWF1" || dtType == "RWF2" || dtType == "RWF1_rtor" || dtType == "RWF2_rtor")
                        {
                            QString idFIR(sData);
                            RunwayWidgetDataFIR rwf(idFIR);

                            if(dtType == "RWF1" || dtType == "RWF1_rtor") {
                                if(!loadRWF1(idFIR, rwf))
                                    return;
                                emit changedRWF1(idFIR, rwf, otType);
                            }
                            if(dtType == "RWF2" || dtType == "RWF2_rtor") {
                                if(!loadRWF2(idFIR, rwf))
                                    return;
                                emit changedRWF2(idFIR, rwf, otType);
                            }
                         }else if(dtType == "RWF" || dtType == "RWF_rtor") {

                                QString idFIR(sData);
                                RunwayWidgetDataVPP rwf(idFIR);
                                if(!loadRWFVPP(idFIR, rwf))
                                    return;
                                emit changedRWFVPP(idFIR, rwf, otType);

                        } else if (dtType == DT_RESTRICTION) {
                            QUuid uid(sData);
                            if (!uid.isNull()) {
                                emit changedRestriction(uid, otType);
                            }
                        } else if(dtType == DT_SECTOR) {
                            SectorRedis sr;
                            if(!loadSectors2(sr))
                                return;
                            emit changedSectors(sr, otType);
                        } else if(dtType == DT_ORCAM) {
                            QString code, assigned_time;
                            code = sData;
                            if(!loadOrcam(code, assigned_time))
                                return;
                            emit changedOrcam(code, assigned_time, otType);
                        }
                    }
                }
            }
        }
    }
    else{
        int ind = sData.indexOf(":");
        if (ind != -1){
            QString code, assigned_time;
            code = sData.remove(0, ind+1);
            emit changedOrcam(code, assigned_time, "delete");
        }
    }
}

bool RedisDBHelper::checkDT(const QString& dtType) const
{
    //if(dtType != DT_SFPL)
    if(dtType != "SFPL" && dtType != "SFPL_rtor" )
        if(dtType != "RWF1" && dtType != "RWF2"
                && dtType != "RWF1_rtor" && dtType != "RWF2_rtor"
                && dtType != "RWF" && dtType != "RWF_rtor")
            if(dtType != DT_SECTOR)
                if (dtType != DT_RESTRICTION)
                    if (dtType != DT_ORCAM)
                    return false;

    return true;
}

bool RedisDBHelper::checkOT(const QString& otType) const
{
    if(otType != OT_INSERT)
        if(otType != OT_UPDATE)
            if(otType != OT_DELETE)
                return false;

    return true;
}

bool RedisDBHelper::saveRWF1(const QString& idFIR, const RunwayWidgetDataFIR& rwf)
{
    TABLE_RWF = "cwp_rwf_left";
    DT_RWF = "RWF1";
    return saveRWF(idFIR,rwf);
}

bool RedisDBHelper::saveRWF2(const QString& idFIR, const RunwayWidgetDataFIR& rwf)
{
    TABLE_RWF = "cwp_rwf_right";
     DT_RWF = "RWF2";
    return saveRWF(idFIR,rwf);
}

bool RedisDBHelper::saveRWFVPP(const QString& idFIR, const RunwayWidgetDataVPP& rwf)
{
    TABLE_RWF = "cwp_rwf";
     DT_RWF = "RWF";
     QVariantMap map = rwf.toVariantMap();
     QJson::Serializer serializer;
     bool bOk = false;
     QByteArray json = serializer.serialize(map, &bOk);
     if(bOk)
     {
         QPair<bool, QtRedis*>& redisPublish = GetPublisher();
         redisPublish.first = true;
         if(redisPublish.second && redisPublish.second->isConnected())
         {
             QString sKey = TABLE_RWF + ":" + idFIR;
             QString sValue = json;
             sValue.replace("\"", "\\\"");
             sValue = "\"" + sValue + "\"";

             QUuid curId = connectionId();
             bOk = redisPublish.second->set(sKey, sValue);

             if (!isConnectionValid(curId))
                 return false;
         }
         else
         {
             redisPublish.first = false;
             return false;
         }
         redisPublish.first = false;
     }
     if(bOk)
     {
         publishToOther(idFIR, DT_RWF, OT_UPDATE);
     }
     else
     {
         qWarning() << "error save RWF" << idFIR;
     }
     return bOk;
}

bool RedisDBHelper::saveRWF(const QString& idFIR, const RunwayWidgetDataFIR& rwf)
{
    QVariantMap map = rwf.toVariantMap();
    QJson::Serializer serializer;
    bool bOk = false;
    QByteArray json = serializer.serialize(map, &bOk);
    if(bOk)
    {
        QPair<bool, QtRedis*>& redisPublish = GetPublisher();
        redisPublish.first = true;
        if(redisPublish.second && redisPublish.second->isConnected())
        {
            QString sKey = TABLE_RWF + ":" + idFIR;
            QString sValue = json;
            sValue.replace("\"", "\\\"");
            sValue = "\"" + sValue + "\"";

            QUuid curId = connectionId();

            bOk = redisPublish.second->set(sKey, sValue);

            if (!isConnectionValid(curId))
                return false;
        }
        else
        {
            redisPublish.first = false;
            return false;
        }
        redisPublish.first = false;
    }
    if(bOk)
    {
        publishToOther(idFIR, DT_RWF, OT_UPDATE);
    }
    else
    {
        qWarning() << "error save RWF" << idFIR;
    }
    return bOk;
}


bool RedisDBHelper::loadRWF1(const QString& idFIR, RunwayWidgetDataFIR& rwf)
{
    TABLE_RWF = "cwp_rwf_left";
    return loadRWF(idFIR,rwf);
}
bool RedisDBHelper::loadRWF2(const QString& idFIR, RunwayWidgetDataFIR& rwf)
{
    TABLE_RWF = "cwp_rwf_right";
    return loadRWF(idFIR,rwf);
}

bool RedisDBHelper::loadRWFVPP(const QString& idFIR, RunwayWidgetDataVPP& rwf)
{
    TABLE_RWF = "cwp_rwf";
    bool bOk = false;
    QPair<bool, QtRedis*>& redisPublish = GetPublisher();
    redisPublish.first = true;
    if(redisPublish.second && redisPublish.second->isConnected())
    {
        QUuid curId = connectionId();

        QString sKey = TABLE_RWF + ":" + idFIR;
        QString sValue = redisPublish.second->get(sKey);
        if (!isConnectionValid(curId))
            return false;

        if(!sValue.isEmpty())
        {
            QJson::Parser parser;
            QByteArray arr;
            arr.append(sValue);
            QVariant var = parser.parse(arr, &bOk);
            if(bOk)
            {
                QVariantMap map = var.value<QVariantMap>();
                if(map.size())
                {
                    rwf.fromVariantMap(map);
                }
            }
        }
    }
    redisPublish.first = false;
    return bOk;
}

bool RedisDBHelper::loadRWF(const QString& idFIR, RunwayWidgetDataFIR& rwf)
{
    bool bOk = false;
    QPair<bool, QtRedis*>& redisPublish = GetPublisher();
    redisPublish.first = true;
    if(redisPublish.second && redisPublish.second->isConnected())
    {
        QUuid curId = connectionId();

        QString sKey = TABLE_RWF + ":" + idFIR;
        QString sValue = redisPublish.second->get(sKey);

        if (!isConnectionValid(curId))
            return false;

        if(!sValue.isEmpty())
        {
            QJson::Parser parser;
            QByteArray arr;
            arr.append(sValue);
            QVariant var = parser.parse(arr, &bOk);
            if(bOk)
            {
                QVariantMap map = var.value<QVariantMap>();
                if(map.size())
                {
                    rwf.fromVariantMap(map);
                }
            }
        }
    }
    redisPublish.first = false;
    return bOk;
}

bool RedisDBHelper::saveSectors(const SectorRedis& sr)
{
    QVariantMap map = sr.toVariantMap();
    QJson::Serializer serializer;
    bool bOk = false;
    QByteArray json = serializer.serialize(map, &bOk);
    if(bOk)
    {
        QPair<bool, QtRedis*>& redisPublish = GetPublisher();
        redisPublish.first = true;
        if(redisPublish.second && redisPublish.second->isConnected())
        {
            QString sKey = TABLE_SECTOR;
            QString sValue = json;
            sValue.replace("\"", "\\\"");
            sValue = "\"" + sValue + "\"";

            QUuid curId = connectionId();

            bOk = redisPublish.second->set(sKey, sValue);

            if (!isConnectionValid(curId))
                return false;

        }
        else
        {
            redisPublish.first = false;
            return false;
        }
        redisPublish.first = false;
    }
    if(bOk)
    {
        publishToOther(QString::null, DT_SECTOR, OT_UPDATE);
    }
    else
    {
        qWarning() << "error save sectors";
    }
    return bOk;
}

bool RedisDBHelper::loadSectors(SectorRedis& sr)
{
    bool bOk = false;
    QPair<bool, QtRedis*>& redisPublish = GetPublisher();
    redisPublish.first = true;
    if(redisPublish.second && redisPublish.second->isConnected())
    {
        QUuid curId = connectionId();

        QString sKey = TABLE_SECTOR;
        QString sValue = redisPublish.second->get(sKey);

        if (!isConnectionValid(curId))
            return false;

        if(!sValue.isEmpty())
        {
            QJson::Parser parser;
            QByteArray arr;
            arr.append(sValue);
            QVariant var = parser.parse(arr, &bOk);
            if(bOk)
            {
                QVariantMap map = var.value<QVariantMap>();
                if(map.size())
                {
                    sr.fromVariantMap(map);
                }
            }
        }
    }
    redisPublish.first = false;
    return bOk;
}

bool RedisDBHelper::saveSectors2(const SectorRedis &sr)
{
    // Updating expire to everyone. Do not call this on timer!

    QVariantMap map = sr.toVariantMap2();
    QJson::Serializer serializer;
    bool bOk = false;

    QPair<bool, QtRedis*>& redisPublish = GetPublisher();
    redisPublish.first = true;
    if(redisPublish.second && redisPublish.second->isConnected())
    {
        QUuid curId = connectionId();

        bOk = true;
        // removing old keys
        const QString sPattern = TABLE_SECTOR + ":*";
        const QStringList sKeys = redisPublish.second->keys(sPattern);
        if (!isConnectionValid(curId))
            return false;

        foreach (const QString &sKey, sKeys) {
            logOperation(tr("Func 'saveSectors2' Delete sector '%1'").arg(sKey));
            redisPublish.second->del(sKey);
            if (!isConnectionValid(curId))
                return false;
        }

        // inserting new keys
        const QStringList clients = map.keys();
        foreach (const QString &client, clients) {
            QString sKey = TABLE_SECTOR + ":" + client;
            QString sValue = serializer.serialize(map[client].toMap(), &bOk);
            if (!bOk)
                break;
            sValue.replace("\"", "\\\"");
            sValue = "\"" + sValue + "\"";
            bOk = redisPublish.second->set(sKey, sValue);
            if (!isConnectionValid(curId))
                return false;

            if (!bOk)
                break;
            /*bOk = redisPublish.second->expire(sKey, 30);
            if (!isConnectionValid(curId))
                return false;

            if (!bOk)
                break;*/
        }
    }
    else
    {
        redisPublish.first = false;
        return false;
    }
    redisPublish.first = false;

    if(bOk)
    {
        publishToOther(QString::null, DT_SECTOR, OT_UPDATE);

        QEventLoop loop;
        QTimer::singleShot(500, &loop, SLOT(quit()));
        loop.exec();
    }
    else
    {
        qWarning() << "error save sectors";
    }
    return bOk;
}

bool RedisDBHelper::loadSectors2(SectorRedis &sr)
{
    bool bOk = false;
    QPair<bool, QtRedis*>& redisPublish = GetPublisher();
    redisPublish.first = true;
    if(redisPublish.second && redisPublish.second->isConnected())
    {
        QUuid curId = connectionId();

        const QString sPattern = TABLE_SECTOR + ":*";
        const QStringList sKeys = redisPublish.second->keys(sPattern);
        if (!isConnectionValid(curId))
            return false;

        QVariantMap map;
        foreach (const QString &sKey, sKeys) {
            const QString sValue = redisPublish.second->get(sKey);
            if (!isConnectionValid(curId))
                return false;

            if (sValue.isEmpty())
                continue;
            QJson::Parser parser;
            QByteArray arr = sValue.toUtf8();
            const QVariant var = parser.parse(arr, &bOk);
            if (!bOk)
                return false;
            const QVariantMap sectors = var.toMap();
            map.insert(sKey.mid(sKey.indexOf(":") + 1), sectors);
        }
        sr.fromVariantMap2(map);
    }
    redisPublish.first = false;
    return true;;
}

void RedisDBHelper::sendRestriction(const QUuid &uid, const QString &operation)
{
    publishToOther(uid, DT_RESTRICTION, operation);
}


bool RedisDBHelper::updateExpire()
{
    if (mClient.isEmpty())
        return false;

    bool bOk = false;
    QPair<bool, QtRedis*>& redisPublish = GetPublisher();
    redisPublish.first = true;
    if(redisPublish.second && redisPublish.second->isConnected())
    {
        QUuid curId = connectionId();

        const QString sKey = TABLE_SECTOR + ":" + mClient;
        const QString sValue = redisPublish.second->get(sKey);
        if (!isConnectionValid(curId))
            return false;

        if (sValue == "NULL") {
            bOk = true;
        } else {
            bOk = redisPublish.second->expire(sKey, 30);
            if (!isConnectionValid(curId))
                return false;
        }
    }
    else
    {
        redisPublish.first = false;
        return false;
    }
    redisPublish.first = false;

    if(bOk)
    {
        publishToOther(QString::null, DT_SECTOR, OT_UPDATE);
    }
    else
    {
        qWarning() << "error update sector expire time";
    }
    return bOk;
}

bool RedisDBHelper::removeClient(const SectorR &sr)
{
    bool bOk = false;

    QPair<bool, QtRedis*>& redisPublish = GetPublisher();
    redisPublish.first = true;
    if(redisPublish.second && redisPublish.second->isConnected())
    {
        QUuid curId = connectionId();
        bOk = true;

        const QString client = sr.GetClient();
        const QString sPattern = TABLE_SECTOR + ":" + client;

        if(!redisPublish.second->keys(sPattern).isEmpty())
        {
            const QStringList sKeys = redisPublish.second->keys(sPattern);
            if (!isConnectionValid(curId))
            return false;

            foreach (const QString &sKey, sKeys) {
            logOperation(tr("Remove Client '%1'").arg(sKey));
            redisPublish.second->del(sKey);
            if (!isConnectionValid(curId))
                return false;
            }
        }
    }
    else
    {
        redisPublish.first = false;
        return false;
    }
    redisPublish.first = false;

    if(bOk)
    {
        publishToOther(QString::null, DT_SECTOR, OT_UPDATE);
    }
    else
    {
        qWarning() << "error remove client";
    }
    return bOk;
}

bool RedisDBHelper::addOrcam(QString code, QDateTime assigned_time, int minutes)
{
    QVariantMap map;
    map.insert("TIME", assigned_time.toString());
    QJson::Serializer serializer;
    bool bOk = false;
    QByteArray json = serializer.serialize(map, &bOk);
    if(bOk)
    {
        QPair<bool, QtRedis*>& redisPublish = GetPublisher();
        redisPublish.first = true;
        if(redisPublish.second && redisPublish.second->isConnected())
        {
            QString sKey = TABLE_ORCAM + ":" + code;
            QString sValue = json;

            sValue.replace("\"", "\\\"");
            sValue = "\"" + sValue + "\"";

            QUuid curId = connectionId();

            bOk = redisPublish.second->set(sKey, sValue);

            if (!isConnectionValid(curId))
                return false;

            if (bOk)
                redisPublish.second->expire(sKey, minutes*60); // orcam lifetime minutes min

            if (!isConnectionValid(curId))
                return false;

        }
        else
        {
            redisPublish.first = false;
            return false;
        }
        redisPublish.first = false;
    }
    if(bOk)
    {
        publishToOther(code, DT_ORCAM, OT_INSERT);
    }
    else
    {
        qWarning() << "error save ORCAM";
    }
    return bOk;
}

bool RedisDBHelper::loadOrcam(const QString &code, QString &assigned_time)
{
    bool bRes = false;
    QPair<bool, QtRedis*>& redisPublish = GetPublisher();
    redisPublish.first = true;

    if(!code.isNull() && redisPublish.second && redisPublish.second->isConnected())
    {
        QUuid curId = connectionId();

        QString sKey = TABLE_ORCAM + ":" + code;
        QString sValue = redisPublish.second->get(sKey);

        if (!isConnectionValid(curId))
            return false;

        if(!sValue.isEmpty())
        {
            QJson::Parser parser;
            bool bOk = false;
            QByteArray arr;
            arr.append(sValue);
            QVariant var = parser.parse(arr, &bOk);
            if(bOk)
            {
                QVariantMap map = var.value<QVariantMap>();
                if(map.size())
                {
                    assigned_time = map.value("TIME", assigned_time).toString();
                    bRes = true;
                }
            }
        }
    }
    redisPublish.first = false;
    return bRes;
}

bool RedisDBHelper::loadOrcams(QMultiMap<QString, QString>& map)
{
    bool bRes = true;
    QStringList listKeys;
    QPair<bool, QtRedis*>& redisPublish = GetPublisher();
    redisPublish.first = true;
    if(redisPublish.second && redisPublish.second->isConnected())
    {
        QUuid curId = connectionId();

        QStringList listKeysTmp = redisPublish.second->keys(TABLE_ORCAM + ":*");

        if (!isConnectionValid(curId))
            return false;

        foreach(const QString& sKeyTmp, listKeysTmp)
        {
            QString sKey(sKeyTmp);
            sKey.replace(QString(TABLE_ORCAM + ":"), QString(""));
            listKeys.append(sKey);
        }
    }
    else
    {
        bRes = false;
    }
    redisPublish.first = false;
    foreach(const QString& sKey, listKeys)
    {
        QString assigned_time;
        bool bOk = loadOrcam(sKey, assigned_time);
        if(bOk)
            map.insert(sKey, assigned_time);        
        else
            qWarning() << "error load ORCAM" << sKey;

    }
    return bRes;
}

bool RedisDBHelper::deleteOrcam(const QString &code)
{
    bool bRes = false;
    QPair<bool, QtRedis*>& redisPublish = GetPublisher();
    redisPublish.first = true;
    if(redisPublish.second && redisPublish.second->isConnected())
    {
        QUuid curId = connectionId();

        QString sKey = TABLE_ORCAM + ":" + code;
        logOperation(tr("Delete Orcam '%1'").arg(sKey));
        redisPublish.second->del(sKey);
        //QStringList listKeysTmp = redisPublish.second->keys(TABLE_ORCAM + ":*");

        if (!isConnectionValid(curId))
            return false;

        /*foreach(const QString& sKeyTmp, listKeysTmp)
        {
            QString sKey(sKeyTmp);
            sKey.replace(QString(TABLE_ORCAM + ":"), QString(code));
            redisPublish.second->del(sKey);
        }*/


        bRes = true;

    }
    else
    {
        bRes = false;
    }
    redisPublish.first = false;
    if(bRes)
    {
        publishToOther(code, DT_ORCAM, OT_DELETE);
    }
    else
    {
        if(!code.isEmpty())
        {
            qWarning() << "error delete Orcam"<< code;
        }
    }

    return bRes;
}



