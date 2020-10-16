#include "WorkerThread.h"
#include "Misc.h"
#include "cs/CoordSystemStereo.h"

#include <QtCore/QMutexLocker>
#include <QtCore/QDebug>
#include <QtCore/QMap>

namespace rewarn {

WorkerThread::WorkerThread(TFirMap &firs, QObject *parent)
    : QThread(parent), mFirs(firs), mUseNotification(true), mIsStopped(false)
{
    if (objectName().isEmpty())
        setObjectName("restriction_worker");
}

void WorkerThread::run()
{
    mIsStopped = false;

    // Database reader process was stopped or database is empty -> no need to do processing
    // as nothing to process.
    if ( mFirs.empty() )
        return;

    qDebug() << tr("Worker thread started successfully ...");

    // Define center point of coordinate system if main fir name is defined.
    if ( !mMainFirId.isEmpty() && mFirs.contains(mMainFirId) )
    {
        const ReFir &mainFir = mFirs[mMainFirId];
        CCoordSystemStereo *csStereo = dynamic_cast<CCoordSystemStereo *>(mCoordSys.data());
        if ( csStereo )
            csStereo->defineCenterPoint(mainFir.mCenterLatitude, mainFir.mCenterLongitude);
    }

    // Prepare database objects ready to do fast calculations.
    projectFirCoords(mCoordSys, mFirs);
    prepareFirForCalculations(mFirs);

    qDebug() << tr("Restriction zones prepared for calculations ...");

    emit signalAreasReady();

    AirplaneInfo airPlane, airPlaneMoved;

    forever
    {
        if ( mIsStopped )
            break;

        updateAreas();

        if ( mIsStopped )
            break;

        QMutexLocker mutexForeverLocker(&mForeverMutex);
        if ( mAirplanes.empty() )
            mAirplaneWait.wait(&mForeverMutex);

        // Start new loop to update zones before process airplanes.
        if ( !mFirsNew.empty() || !mAreaIdForDelete.empty() || !mMeteoFirsNew.empty() )
            continue;

        if ( mIsStopped )
            break;

        {
            // Retrieve one airplane from the stack.
            QMutexLocker locker(&mUpdateMutex);

            if ( mAirplanes.size() > 10000 )
            {
                qWarning() << tr("Processing stack of airplanes '%1' is too big. Optimization is required here.");
                // Cut stack as number of airplanes too big.
                mAirplanes.remove(1000, mAirplanes.size()-1000);
                qWarning() << tr("Processing stack is cut to 1000 airplanes number.");
            }

            airPlane = mAirplanes.pop();
            airPlaneMoved = moveAirplane(airPlane, mMinEntryTime);
        }

        // Predict intersection.

        QPointF ptAirPlane(airPlane.x, airPlane.y),
                ptAirPlaneMoved(airPlaneMoved.x, airPlaneMoved.y);
        QRectF rectAir(ptAirPlane, ptAirPlaneMoved);
        rectAir = rectAir.normalized();

        double heightAirMin = qMin(airPlane.height, airPlaneMoved.height),
               heightAirMax = qMax(airPlane.height, airPlaneMoved.height);

        // Test what fir airplane on.
        for ( TFirMap::const_iterator itcFir = mFirs.constBegin(); itcFir != mFirs.constEnd(); ++itcFir )
        {
            const ReFir &fir = *itcFir;

            // Check that airplane is above current FIR.
            if ( !fir.mBoundingBox.contains(airPlane.x, airPlane.y) )
            {
                // Check that airplane is in the vicinity of notify interval to the FIR.
                QRectF rectFirVicinity = fir.mBoundingBox;
                double dx = qAbs(airPlane.dx*mMinEntryTime),
                       dy = qAbs(airPlane.dy*mMinEntryTime);
                rectFirVicinity.adjust(-dx, -dy, dx, dy);
                if ( !rectFirVicinity.contains(airPlane.x, airPlane.y) )
                    continue;
            }

            foreach ( const ReArea &area, fir.mAreas )
            {
                // Restriction area type check.
                if ( !(mFilterReTypes.empty() || mFilterReTypes.contains(area.mType)) )
                    continue;

                // Check if restriction zone is working (valid).
                if ( area.isUser() && !area.isValid(QDateTime::currentDateTime()) )
                    continue;

                // Check hit inside polygon.
                if ( area.mBoundingBox.contains(airPlane.x, airPlane.y) &&
                     area.mHeightMin <= airPlane.height && airPlane.height <= area.mHeightMax )
                {
                    if ( isInside(airPlane, area) ) // precise check
                    {
                        emit signalInsideZone(airPlane.boardNumber, (int)area.mType, area.mID);
                        continue;
                    }
                }

                // Check prediction to fly into the restriction zone.
                if ( !mUseNotification )
                    continue;

                if ( area.mBoundingBox.intersects(rectAir) &&
                     area.mHeightMin <= heightAirMax && heightAirMin <= area.mHeightMax )
                {
                    if ( isTouches(airPlane, airPlaneMoved, area) ) // precise check
                    {
                        emit signalZoneOnCourse(airPlane.boardNumber, (int)area.mType, area.mID);
                        continue;
                    }
                }
            }

            foreach (const MeteoArea &area, fir.mMeteoAreas)
            {
                // Check hit inside polygon.
                if ( area.mBoundingBox.contains(airPlane.x, airPlane.y) &&
                     area.mHeightMin <= airPlane.height && airPlane.height <= area.mHeightMax )
                {
                    if ( isInside(airPlane, area) ) // precise check
                    {
                        emit signalInsideMeteoZone(airPlane.boardNumber, (int)area.mType, area.mID);
                        continue;
                    }
                }

                // Check prediction to fly into the restriction zone.
                if ( !mUseNotification )
                    continue;

                if ( area.mBoundingBox.intersects(rectAir) &&
                     area.mHeightMin <= heightAirMax && heightAirMin <= area.mHeightMax )
                {
                    if ( isTouches(airPlane, airPlaneMoved, area) ) // precise check
                    {
                        emit signalMeteoZoneOnCourse(airPlane.boardNumber, (int)area.mType, area.mID);
                        continue;
                    }
                }
            }
        }
    }

    qDebug() << tr("Worker thread was stopped ...");
}

void WorkerThread::updateAreas()
{
    // Nothing to update.
    if ( mFirsNew.empty() && mAreaIdForDelete.empty() && mMeteoFirsNew.empty() )
        return;

    QMutexLocker locker(&mUpdateMutex);

    qDebug() << tr("Restriction/Meteo zones were changed. Update started ...");

    // Update each new user restriction area.
    for ( TFirMap::iterator itFir = mFirsNew.begin(); itFir != mFirsNew.end(); ++itFir )
    {
        ReFir &firNew = *itFir;

        if ( !mFirs.contains(itFir.key()) )
            continue;

        ReFir &firCurrent = mFirs[itFir.key()];

        for ( TAreaList::iterator itArea = firNew.mAreas.begin();
              itArea != firNew.mAreas.end(); ++itArea )
        {
            ReArea &areaNew = *itArea;

            ReArea *areaCurrent = findArea(firCurrent, areaNew.mID);
            // Update user restriction area.
            if ( areaCurrent )
            {
                *areaCurrent = areaNew;
                projectAreaCoords(mCoordSys, *areaCurrent);
            }
            // Create new user restricion area.
            else
            {
                firCurrent.mAreas.append( areaNew );
                projectAreaCoords(mCoordSys, firCurrent.mAreas.back());
            }
        }
    }

    // Remove restriction zones from main FIR map.
    foreach ( quint32 id, mAreaIdForDelete )
    {
        for ( TFirMap::iterator itFir = mFirs.begin(); itFir != mFirs.end(); ++itFir )
        {
            ReFir &fir = *itFir;

            TAreaList::iterator itArea = fir.mAreas.begin();
            while ( itArea != fir.mAreas.end() )
            {
                ReArea &area = *itArea;
                if ( area.mID == id )
                {
                    // Safe remove of area from the list.
                    TAreaList::iterator itAreaPrev = itArea;
                    if ( itArea != fir.mAreas.begin() )
                        --itAreaPrev;
                    fir.mAreas.erase(itArea);
                    if ( itArea == itAreaPrev )
                    {
                        itArea = fir.mAreas.begin();
                        continue;
                    }
                    itArea = itAreaPrev;
                }

                ++itArea;
            }
        }
    }

    // update meteo zones
    for (auto itFir = mMeteoFirsNew.begin(); itFir != mMeteoFirsNew.end(); ++itFir)
    {
        if (mFirs.contains(itFir.key()))
        {
            ReFir &fir = mFirs[itFir.key()];               
            fir.mMeteoAreas = itFir.value().mMeteoAreas;
            for (auto itArea = fir.mMeteoAreas.begin(); itArea != fir.mMeteoAreas.end(); ++itArea)
            {
                projectAreaCoords(mCoordSys, *itArea);
            }
        }
    }

    prepareFirForCalculations(mFirs);

    mFirsNew.clear();
    mAreaIdForDelete.clear();
    mMeteoFirsNew.clear();

    emit signalAreasReady();

    qDebug() << tr("Restriction/Meteo zones were changed. Update finished ...");
}

ReArea* WorkerThread::findArea(ReFir &fir, quint32 idArea) const
{
    for ( TAreaList::iterator itArea = fir.mAreas.begin();
          itArea != fir.mAreas.end(); ++itArea )
    {
        ReArea &area = *itArea;

        if ( area.mID == idArea )
            return &area;
    }

    return 0;
}

void WorkerThread::setNewAreas(const TFirMap &firs, const TIdList &areaIdListForDelete, const TFirMap &meteoFirs)
{
    if ( firs.empty() && areaIdListForDelete.empty() && meteoFirs.empty() )
        return;

    QMutexLocker locker(&mUpdateMutex);

    mFirsNew = firs;
    mAreaIdForDelete = areaIdListForDelete;
    mMeteoFirsNew = meteoFirs;

    mAirplaneWait.wakeAll();
}

void WorkerThread::setParams(qreal dt, bool isNotify, const TFilterSet filterReTypes)
{
    QMutexLocker locker(&mUpdateMutex);

    mMinEntryTime = dt;
    mFilterReTypes = filterReTypes;
    mUseNotification = isNotify;
}

void WorkerThread::addAirplane(const AirplaneInfo& ai)
{
    QMutexLocker locker(&mUpdateMutex);
    mAirplanes.push(ai);
    mAirplaneWait.wakeAll();
}

void WorkerThread::removeAirplanes()
{
    QMutexLocker locker(&mUpdateMutex);
    mAirplanes.clear();
}

void WorkerThread::stop()
{
    mIsStopped = true;
    mAirplaneWait.wakeAll();
}

}   // namespace rewarn {
