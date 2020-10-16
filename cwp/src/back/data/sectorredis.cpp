#include "sectorredis.h"

int metatype_id_Sector_Redis = qRegisterMetaType<SectorRedis>("SectorRedis");

QVariantMap SectorR::toVariantMap() const
{
    QVariantMap map;
    map.insert("CLIENT", mClient);
    map.insert("STATUS", mStatus);
    return map;
}

void SectorR::fromVariantMap(const QVariantMap& map)
{
    mClient = map.value("CLIENT", mClient).toString();
    mStatus = map.value("STATUS", mStatus).toInt();
}



SectorRedis::SectorRedis()
{

}

QVariantMap SectorRedis::toVariantMap() const
{
    QVariantMap map;
    for(TSectorMap::const_iterator itSector = mSectorMap.constBegin(); itSector != mSectorMap.constEnd(); ++itSector)
        map.insert(itSector.key(), itSector->toVariantMap());
    return map;
}

void SectorRedis::fromVariantMap(const QVariantMap& map)
{
    mSectorMap.clear();
    for(QVariantMap::const_iterator itMap = map.constBegin(); itMap != map.constEnd(); ++itMap)
    {
        QVariantMap srMap = itMap->toMap();
        SectorR sr;
        sr.fromVariantMap(srMap);
        mSectorMap.insert(itMap.key(), sr);
    }
}

bool SectorRedis::isOccupiedClientSectors(const QString& sClient, const QStringList& sSectors) const
{
    bool bRes = false;
    for(QStringList::const_iterator itSector = sSectors.constBegin(); itSector != sSectors.constEnd(); ++itSector)
    {
        TSectorMap::const_iterator itFind = mSectorMap.constFind(*itSector);
        if(itFind != mSectorMap.constEnd())
        {
            if(itFind->GetClient() != sClient)
            {
                bRes = true;
                break;
            }
        }
    }
    return bRes;
}

bool SectorRedis::isEqualClientSectors(const QString& sClient, const QStringList& sSectors) const
{
    QStringList sl2 = getClientSectors(sClient);
    if(sSectors.size() != sl2.size())
        return false;
    if(sSectors.size() && sl2.size())
        if(sSectors.first() != sl2.first())
            return false;
    for(QStringList::const_iterator itSector = sSectors.constBegin(); itSector != sSectors.constEnd(); ++itSector)
    {
        if(!sl2.contains(*itSector))
            return false;
    }
    for(QStringList::const_iterator itSector = sl2.constBegin(); itSector != sl2.constEnd(); ++itSector)
    {
        if(!sSectors.contains(*itSector))
            return false;
    }
    return true;
}

void SectorRedis::setClientSectors(const QString& sClient, const QStringList& sSectors)
{
    QStringList grabClients;
    for(QStringList::const_iterator itSector = sSectors.constBegin(); itSector != sSectors.constEnd(); ++itSector)
    {
        TSectorMap::const_iterator itFind = mSectorMap.constFind(*itSector);
        if(itFind != mSectorMap.constEnd())
        {
            if(itFind->GetClient() != sClient)
            {
                if(itFind->GetStatus() == 1)
                {
                    grabClients.prepend(itFind->GetClient());
                }
            }
        }
    }
    QStringList sSectorsNew(sSectors);
    for(QStringList::const_iterator itClient = grabClients.constBegin(); itClient != grabClients.constEnd(); ++itClient)
    {
        QStringList sl = getClientSectors(*itClient);
        if(sl.size())
            sSectorsNew.append(sl);
    }
    sSectorsNew.removeDuplicates();
    for(TSectorMap::iterator itSector = mSectorMap.begin(); itSector != mSectorMap.end(); )
    {
        if(itSector->GetClient() == sClient)
            itSector = mSectorMap.erase(itSector);
        else
            ++itSector;
    }
    for(QStringList::const_iterator itSector = sSectorsNew.constBegin(); itSector != sSectorsNew.constEnd(); ++itSector)
    {
        SectorR sr;
        sr.SetClient(sClient);
        sr.SetStatus(itSector == sSectorsNew.constBegin() ? 1 : 2);
        mSectorMap.insert(*itSector, sr);
    }
}

QStringList SectorRedis::getClientSectors(const QString& sClient) const
{
    QStringList sl;
    for(TSectorMap::const_iterator itSector = mSectorMap.constBegin(); itSector != mSectorMap.constEnd(); ++itSector)
    {
        if(itSector->GetClient() == sClient)
        {
            if(itSector->GetStatus() == 1)
                sl.prepend(itSector.key());
            else
                sl.append(itSector.key());
        }
    }
    return sl;
}
