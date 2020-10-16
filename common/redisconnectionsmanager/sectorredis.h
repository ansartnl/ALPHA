#ifndef SECTOR_REDIS_H
#define SECTOR_REDIS_H

#include <QString>
#include <QMap>
#include <QVariantMap>
#include <QStringList>

class SectorR
{
public:
    //! Constructor
    SectorR() : mStatus(0) {}

    QString GetClient() const
    { return mClient; }
    void SetClient(QString sClient)
    { mClient = sClient; }

    int GetStatus() const
    { return mStatus; }
    void SetStatus(int iStatus)
    { mStatus = iStatus; }

    QVariantMap toVariantMap() const;
    void fromVariantMap(const QVariantMap& map);

private:
    QString mClient;
    int     mStatus;
};

//! Sectors in redis
class SectorRedis
{
public:
    typedef QMap<QString, SectorR>  TSectorMap;

    //! Constructor
    SectorRedis();

    QVariantMap toVariantMap() const;
    void fromVariantMap(const QVariantMap& map);

    QVariantMap toVariantMap2() const;
    void fromVariantMap2(const QVariantMap& map);

    bool isOccupiedClientSectors(const QString& sClient, const QStringList& sSectors) const;
    bool isEqualClientSectors(const QString& sClient, const QStringList& sSectors) const;
    void setClientSectors(const QString& sClient, const QStringList& sSectors);
    QStringList getClientSectors(const QString& sClient) const;
    QStringList sectors() const { return mSectorMap.keys(); }
    QStringList clients() const;
    QStringList getMasterSectors() const;

private:
    TSectorMap mSectorMap;
};

Q_DECLARE_METATYPE(SectorRedis)

#endif // SECTOR_REDIS_H
