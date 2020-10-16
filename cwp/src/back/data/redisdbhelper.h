#ifndef REDIS_DATABASE_HELPER_H
#define REDIS_DATABASE_HELPER_H

#include "qtredis.h"
#include "redisdb.h"

#include "SFPL.h"
#include "RunwayWidgetData.h"
#include "sectorredis.h"

//! Main class to work with redis database
class RedisDBHelper: public RedisDB
{
    Q_OBJECT

    static const QString TABLE_CHANNEL;
    static const QString TABLE_SECTOR;
    static const QString TABLE_SFPL;
    //static const QString TABLE_RWF;
    static const QString TABLE_ORCAM;

public:
    //! Data type constants
    static const QString DT_SFPL;
    //static const QString DT_RWF;
    static const QString DT_SECTOR;
    static const QString DT_RESTRICTION;
    static const QString DT_ORCAM;
    //! Operation type constants
    static const QString OT_INSERT;
    static const QString OT_UPDATE;
    static const QString OT_DELETE;

    //! Constructor
    RedisDBHelper(const QString &client, QObject *parent=0);
    virtual ~RedisDBHelper();

    //! Save FPL into redis database
    /*!
         \param sfpl pointer on SFPL
    */
    bool saveSFPL(const QSharedPointer<CSFPL>& sfpl);
    bool deleteSFPL(const QSharedPointer<CSFPL>& sfpl);

    bool loadSFPL(QSharedPointer<CSFPL>& sfpl, const QUuid& uid);
    bool loadSFPLs(QList<QSharedPointer<CSFPL> >& list);

    //! Save RunwayWidgetData for FIR into redis database
    /*!
         \param idFIR current FIR
    */
    bool saveRWF(const QString& idFIR, const RunwayWidgetDataFIR& rwf);
    bool saveRWF1(const QString& idFIR, const RunwayWidgetDataFIR& rwf);
    bool saveRWF2(const QString& idFIR, const RunwayWidgetDataFIR& rwf);
    bool saveRWFVPP(const QString& idFIR, const RunwayWidgetDataVPP& rwf);
    bool loadRWF(const QString& idFIR, RunwayWidgetDataFIR& rwf);
    bool loadRWF1(const QString& idFIR, RunwayWidgetDataFIR& rwf);
    bool loadRWF2(const QString& idFIR, RunwayWidgetDataFIR& rwf);
    bool loadRWFVPP(const QString& idFIR, RunwayWidgetDataVPP& rwf);


    //! Save sectors into redis database
    /*!
         \param sr sectors map
    */
    bool saveSectors(const SectorRedis& sr);
    bool loadSectors(SectorRedis& sr);

    bool saveSectors2(const SectorRedis& sr);
    bool loadSectors2(SectorRedis& sr);
    bool removeClient(const SectorR& sr);

    void sendRestriction(const QUuid &uid, const QString &operation);
    bool addOrcam(QString code, QDateTime assigned_time, int minutes);
    bool loadOrcam(const QString &code, QString &assigned_time);
    bool loadOrcams(QMultiMap<QString, QString>& map);
    bool deleteOrcam(const QString &code);

signals:
    void changedSFPL(QSharedPointer<CSFPL> sfpl, QString operType /*OT_* */);
    void changedRWF1(QString idFIR, RunwayWidgetDataFIR rwf, QString operType /*OT_* */);
    void changedRWF2(QString idFIR, RunwayWidgetDataFIR rwf, QString operType /*OT_* */);
    void changedRWFVPP(QString idFIR, RunwayWidgetDataVPP rwf, QString operType /*OT_* */);
    void changedSectors(SectorRedis sr, QString operType /*OT_* */);
    void changedRestriction(const QUuid &uid, const QString &operation);
    //void changedOrcam(QMultiMap<QString, QString> map, QString operType);
    void changedOrcam(QString code, QString assigned_time, QString operType);


protected slots:
    void onRedisData(QtRedis::Reply data);

protected:
    bool checkDT(const QString& dtType) const;
    bool checkOT(const QString& otType) const;

private slots:
    bool updateExpire();

private:
    QString mClient;
    QTimer *mExpireTimer;
    QString TABLE_RWF;
    QString DT_RWF;

};
#endif // REDIS_DATABASE_HELPER_H
