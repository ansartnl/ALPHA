#ifndef _REWARN_WARN_NOTIFIER_H__
#define _REWARN_WARN_NOTIFIER_H__

#include <QtCore/QObject>
#include <QtCore/QSet>

#include "rewarn_global.h"
#include "AirplaneInfo.h"
#include "orm/ReTypes.h"

#ifdef REWARN_TEST
#   include "orm/ReFir.h"
#endif

namespace rewarn {

class WarnNotifierPrivate;

//! Main class for managing notifier process.
class REWARN_EXPORT WarnNotifier : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE_D(data, WarnNotifier)

public:
    typedef QSet<ReType>    TReTypeSet;

    explicit WarnNotifier(QObject *parent = 0);
    ~WarnNotifier();

    //! Add new airplane to process for searching intersections or
    //! notification about flying into restriction zone.
    void addAirplane(const AirplaneInfo& ai);
    //! Clear airplane stack.
    void removeAirplanes();

    //! Returns true if airplane is inside "No Conflict" restriction zone.
    bool isInsideNFZone(const AirplaneInfo& ai);

    //! Set process parameters.
    /*!
        \param dt Minimum time for notification about entering into restriction zone.
        \param filterReTypes Filter of restriction zone types that will be processed.
                             If set is empty - all restriction zones are processed.
        \param isNotify Notify about possible entering into restriction zone.
    */
    void setParams(qreal dt, bool isNotify, const TReTypeSet filterReTypes = TReTypeSet());

    //! Set connection parameters to database.
    void setDatabaseParams(const QString &driver, const QString &host, int port,
                           const QString &dbName, const QString &user, const QString &password);

    //! Define center point which defines center zone and point for conversion.
    void defineCSCenterPoint(qreal latitude, qreal longitude);
    //! Define center point which defines center FIR for conversion.
    void defineMainFir(const QString &nameId);

    //! Set synchronization interval for updating areas from database.
    void setSyncInterval(int msecs);
    //! Set synchronization interval for cleaning the sync table in the past.
    void setSyncCleanInterval(int hours);

    //! Run processes.
    void start();
    //! Stop notifier process.
    void stop();

#ifdef REWARN_TEST
    void addFir(const QString &nameId, const ReFir &fir);
#endif

public slots:
    void on_databaseConnected();
    void on_syncDbAreasUpdate();
    void on_syncDbMeteoAreasUpdate();

signals:
    void signalInsideZone(quint16 boardNumber, int typeZone, quint32 idZone);
    void signalZoneOnCourse(quint16 boardNumber, int typeZone, quint32 idZone);

    void signalInsideMeteoZone(quint16 boardNumber, int typeZone, quint32 idZone);
    void signalMeteoZoneOnCourse(quint16 boardNumber, int typeZone, quint32 idZone);

private slots:
    void on_aboutToQuit();
    void on_finishedDbReader();
    void on_syncThreadFinished();
    void on_syncMeteoThreadFinished();
    void on_areasReady();

private:
    WarnNotifierPrivate     *data;
};

}   // namespace rewarn {

#endif // _REWARN_WARN_NOTIFIER_H__
