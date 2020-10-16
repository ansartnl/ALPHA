#include "WarnNotifier.h"
#include "helpers/DbReadThread.h"
#include "helpers/WorkerThread.h"
#include "helpers/NFZchecker.h"

#include "cs/CoordSystemStereo.h"
#include "databasesubscriber.h"

#include "QtCore/QDebug"
#include <QtCore/QCoreApplication>
#include <QtCore/QTimer>


namespace rewarn {

typedef CCoordSystemStereo  CurrentCoordSystem;
typedef QList<quint32>      TIdList;

class WarnNotifierPrivate
{
public:
    WarnNotifierPrivate();

    DbReaderThread          mDbReader;
    DbReaderSyncThread      mDbSyncReader;
    DbReaderMeteoSyncThread mDbMeteoSyncReader;

    WorkerThread    mWorker;
    /** Checker of "No Conflict" areas */
    NFZchecker      mNFZchecker;

    TCoordSystem    mCoordSys;

    QTimer          *mSyncTimer;

    /** Identifiers of areas that should be removed from latest FIR map */
    TIdList         mSyncIdAreasForDelete;
    /** New FIRs with areas that were updated - for synchronization */
    TFirMap         mSyncFirs;
    TFirMap         mMeteoSyncFirs;
    /** FIRs with the latest restriction areas */
    TFirMap         mFirs;
    /** FIRs with areas where "No Conflict" zones only */
    TFirMap         mNFZFirs;
};

WarnNotifierPrivate::WarnNotifierPrivate()
    : mDbReader(mFirs), mDbSyncReader(mSyncFirs, mSyncIdAreasForDelete),
      mDbMeteoSyncReader(mMeteoSyncFirs), mWorker(mFirs)
{
    QObject::connect(&mDbReader, SIGNAL(finished()), &mWorker, SLOT(start()));
}


///////////////////////////////////////////////////////////////////////////////////////
// WarnNotifier implementation.

WarnNotifier::WarnNotifier(QObject *parent)
    : QObject(parent), data(0)
{
    data = new WarnNotifierPrivate();

    Q_D(WarnNotifier);
    d->mCoordSys = TCoordSystem(new CurrentCoordSystem());
    d->mWorker.setCoordSys( d->mCoordSys );

    d->mNFZchecker.setCoordSys( d->mCoordSys );

    d->mSyncTimer = new QTimer(this);
    // Default interval -> once in a minute.
    d->mSyncTimer->setInterval( 60000 );

    // Signals retranslation.
    connect(&d->mWorker, SIGNAL(signalInsideZone(quint16, int, quint32)),
            SIGNAL(signalInsideZone(quint16, int, quint32)));
    connect(&d->mWorker, SIGNAL(signalZoneOnCourse(quint16, int, quint32)),
            SIGNAL(signalZoneOnCourse(quint16, int, quint32)));
    connect(&d->mWorker, SIGNAL(signalInsideMeteoZone(quint16, int, quint32)),
            SIGNAL(signalInsideMeteoZone(quint16, int, quint32)));
    connect(&d->mWorker, SIGNAL(signalMeteoZoneOnCourse(quint16, int, quint32)),
            SIGNAL(signalMeteoZoneOnCourse(quint16, int, quint32)));
    connect(&d->mWorker, SIGNAL(signalAreasReady()), SLOT(on_areasReady()));

    // Start synchronization timer after database finished to read.
    connect(d->mSyncTimer, SIGNAL(timeout()), SLOT(on_syncDbAreasUpdate()));
    connect(&d->mDbReader, SIGNAL(finished()), d->mSyncTimer, SLOT(start()));
    connect(&d->mDbReader, SIGNAL(finished()), SLOT(on_finishedDbReader()));

    connect(&d->mDbSyncReader, SIGNAL(finished()), SLOT(on_syncThreadFinished()));
    connect(&d->mDbMeteoSyncReader, SIGNAL(finished()), SLOT(on_syncMeteoThreadFinished()));

    connect(DatabaseSubscriber::instance("meteo").subscribe("meteo"),
            SIGNAL(notification()), SLOT(on_syncDbMeteoAreasUpdate()));
    connect(qApp, SIGNAL(aboutToQuit()), SLOT(on_aboutToQuit()));
}

WarnNotifier::~WarnNotifier()
{
    delete data;
}

void WarnNotifier::addAirplane(const AirplaneInfo& ai)
{
    Q_D(WarnNotifier);

    AirplaneInfo airPlaneInfo = ai;

    CCoord coord(ai.y, ai.x, ai.height);
    d->mCoordSys->toModel(coord);
    airPlaneInfo.x = coord.GetX();
    airPlaneInfo.y = coord.GetY();

    d->mWorker.addAirplane(airPlaneInfo);
}

void WarnNotifier::removeAirplanes()
{
    d_func()->mWorker.removeAirplanes();
}

void WarnNotifier::setParams(qreal dt, bool isNotify, const TReTypeSet filterReTypes)
{
    d_func()->mWorker.setParams(dt, isNotify, filterReTypes);
}

void WarnNotifier::setDatabaseParams(const QString &driver, const QString &host, int port,
                                     const QString &dbName, const QString &user, const QString &password)
{
    Q_D(WarnNotifier);
    d->mDbReader.setDatabaseParams(driver, host, port, dbName, user, password);
    d->mDbSyncReader.setDatabaseParams(driver, host, port, dbName, user, password);
}

void WarnNotifier::defineCSCenterPoint(qreal latitude, qreal longitude)
{
    Q_D(WarnNotifier);
    d->mWorker.setMainFir("");
    d->mWorker.removeAirplanes();

    CurrentCoordSystem *cs = static_cast<CurrentCoordSystem *>(d_func()->mCoordSys.data());
    Q_ASSERT(cs);
    cs->defineCenterPoint(latitude, longitude);

    // Restart worker process in real time if running.
    if ( d->mWorker.isRunning() )
    {
        d->mWorker.stop();
        d->mWorker.wait();

        d->mWorker.start();
    }
}

void WarnNotifier::defineMainFir(const QString &nameId)
{
    Q_D(WarnNotifier);
    d->mWorker.setMainFir(nameId);
    d->mWorker.removeAirplanes();

    // Restart worker process in real time if running.
    if ( d->mWorker.isRunning() )
    {
        d->mWorker.stop();
        d->mWorker.wait();

        d->mWorker.start();
    }
}

void WarnNotifier::setSyncInterval(int msecs)
{
    Q_D(WarnNotifier);
    bool isStarted = d->mSyncTimer->isActive();
    d->mSyncTimer->stop();
    d->mSyncTimer->setInterval( msecs );

    if ( isStarted )
        d->mSyncTimer->start();
}

void WarnNotifier::setSyncCleanInterval(int hours)
{
    Q_D(WarnNotifier);
    d->mDbSyncReader.setCleanerInterval(hours);
}

void WarnNotifier::start()
{
    qDebug() << tr("WarnNotifier process scheduled to start ...");

#ifdef REWARN_TEST
    d_func()->mWorker.start();
#else
    d_func()->mDbReader.start();
#endif
}

void WarnNotifier::stop()
{
    qDebug() << tr("WarnNotifier process scheduled to stop ...");

    Q_D(WarnNotifier);
    d->mSyncTimer->stop();
    if ( d->mDbReader.isRunning() )
    {
        d->mDbReader.stop();
        // Wait until thread terminates.
        d->mDbReader.wait();
    }
    if ( d->mDbSyncReader.isRunning() )
    {
        d->mDbSyncReader.stop();
        // Wait until thread terminates.
        d->mDbSyncReader.wait();
    }
    if ( d->mDbMeteoSyncReader.isRunning() )
    {
        d->mDbMeteoSyncReader.stop();
        // Wait until thread terminates.
        d->mDbMeteoSyncReader.wait();
    }
    if ( d->mWorker.isRunning() )
    {
        d->mWorker.stop();
        // Wait max 10 seconds or terminate thread instead.
        if ( !d->mDbReader.wait(10000) )
            d->mDbReader.terminate();
    }
}

void WarnNotifier::on_aboutToQuit()
{
    QSqlDatabase::removeDatabase( "WarnDbReader" );
    stop();
}

void WarnNotifier::on_finishedDbReader()
{
    Q_D(WarnNotifier);
    d->mDbSyncReader.setSystime( d->mDbReader.systime() );
}

void WarnNotifier::on_syncThreadFinished()
{
    Q_D(WarnNotifier);
    d->mWorker.setNewAreas( d->mSyncFirs, d->mSyncIdAreasForDelete, TFirMap() );
}

void WarnNotifier::on_syncMeteoThreadFinished()
{
    Q_D(WarnNotifier);
    d->mWorker.setNewAreas( TFirMap(), TIdList(), d->mMeteoSyncFirs );
}

void WarnNotifier::on_syncDbAreasUpdate()
{
    Q_D(WarnNotifier);
    if ( !d->mDbReader.isRunning() && !d->mDbSyncReader.isRunning() )
        d->mDbSyncReader.start();
}

void WarnNotifier::on_syncDbMeteoAreasUpdate()
{
    Q_D(WarnNotifier);
    if ( !d->mDbReader.isRunning() && !d->mDbMeteoSyncReader.isRunning() )
        d->mDbMeteoSyncReader.start();
}

void WarnNotifier::on_areasReady()
{
    Q_D(WarnNotifier);
    d->mNFZchecker.setNewAreas( d->mFirs );
}

void WarnNotifier::on_databaseConnected()
{
    QSqlDatabase::removeDatabase("meteo");
    QSqlDatabase::cloneDatabase(QSqlDatabase::database(QSqlDatabase::defaultConnection, false), "meteo");

    DatabaseSubscriber::instance("meteo").onDatabaseConnected();
}

bool WarnNotifier::isInsideNFZone(const AirplaneInfo& ai)
{
    return d_func()->mNFZchecker.isInside(ai);
}

// For test purpose only
#ifdef REWARN_TEST

void WarnNotifier::addFir(const QString &nameId, const ReFir &fir)
{
    Q_D(WarnNotifier);
    d->mFirs.clear();

    d->mFirs[nameId] = fir;
}

#endif // REWARN_TEST

}   // namespace rewarn {
