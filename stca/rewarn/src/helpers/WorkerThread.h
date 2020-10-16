#ifndef _WORKER_THREAD_H__
#define _WORKER_THREAD_H__

#include <QtCore/QThread>
#include <QtCore/QMutex>
#include <QtCore/QWaitCondition>
#include <QtCore/QSet>
#include <QtCore/QStack>

#include "AirplaneInfo.h"
#include "orm/ReTypes.h"
#include "cs/CoordSystem.h"
#include "orm/ReFir.h"

namespace rewarn {

//! Thread to do searching for intersection and notification about
//! fly to the restriction zone.
class WorkerThread : public QThread
{
    Q_OBJECT

public:
    typedef QSet<ReType>            TFilterSet;
    typedef QStack<AirplaneInfo>    TAirplanes;
    typedef QList<quint32>          TIdList;

    explicit WorkerThread(TFirMap &firs, QObject *parent = 0);

    void setCoordSys(const TCoordSystem &cs) { mCoordSys = cs; }

    void run();

    void stop();
    bool isStopped() { return mIsStopped; }

    //! Set process parameters.
    /*!
        \param dt Minimum time for notification about entering into restriction zone.
        \param filterReTypes Filter of restriction zone types that will be processed.
                             If set is empty - all restriction zones are processed.
        \param isNotify Notify about possible entering into restriction zone.
    */
    void setParams(qreal dt, bool isNotify, const TFilterSet filterReTypes = TFilterSet());

    //! Set main fir name identifier.
    void setMainFir(const QString &nameId) { mMainFirId = nameId; }

    //! Add new airplane for processing.
    void addAirplane(const AirplaneInfo& ai);
    //! Clear airplane stack.
    void removeAirplanes();

    //! Set new restriction areas to update main areas.
    /*!
        firs New or updated restriction areas geometry
        removedIdList List of removed user restriction areas
    */
    void setNewAreas(const TFirMap &firs, const TIdList &areaIdListForDelete, const TFirMap &meteoFirs);

    //! Update main restriction areas.
    void updateAreas();

signals:
    void signalInsideZone(quint16 boardNumber, int typeZone, quint32 idZone);
    void signalZoneOnCourse(quint16 boardNumber, int typeZone, quint32 idZone);

    void signalInsideMeteoZone(quint16 boardNumber, int typeZone, quint32 idZone);
    void signalMeteoZoneOnCourse(quint16 boardNumber, int typeZone, quint32 idZone);

    /** When all zones are prepared for calculations - signal is emitted */
    void signalAreasReady();

private:
    //! Find user area for specified fir.
    ReArea* findArea(ReFir &fir, quint32 idArea) const;

private:
    /*! Minimum time for entering into restriction zone */
    qreal       mMinEntryTime;
    /*! Filter of restriction zone for processing */
    TFilterSet  mFilterReTypes;

    /*! Set of airplanes */
    TAirplanes  mAirplanes;

    /*! Main FIR Name identifier (center FIR). */
    QString     mMainFirId;

    /*! Reference to FIR map */
    TFirMap     &mFirs;

    /*! FIR map of new restriction zones */
    TFirMap     mFirsNew;
    TFirMap     mMeteoFirsNew;
    /*! List of area identifiers need to be removed */
    TIdList     mAreaIdForDelete;

    /*! Reference to coordinate system */
    TCoordSystem mCoordSys;

    /*! Update data mutex */
    QMutex      mUpdateMutex;
    /*! Forever loop mutex */
    QMutex      mForeverMutex;
    /*! Airplane wait condition */
    QWaitCondition mAirplaneWait;

    bool        mUseNotification;

    /*! Cancel state */
    bool        mIsStopped;
};

}   // namespace rewarn {

#endif // _WORKER_THREAD_H__
