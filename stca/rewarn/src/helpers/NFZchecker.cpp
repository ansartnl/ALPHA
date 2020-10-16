#include "NFZchecker.h"
#include "Misc.h"

#include "QtCore/QDebug"


namespace rewarn {

NFZchecker::NFZchecker(QObject *parent)
    : QObject(parent)
{
}

void NFZchecker::setNewAreas(TFirMap firs)
{
    QWriteLocker locker(&mLock);

    mFirs.clear();
    for ( TFirMap::const_iterator itcFir = firs.constBegin(); itcFir != firs.constEnd(); ++itcFir )
    {
        const ReFir &fir = *itcFir;

        QString nameFir = itcFir.key();

        mFirs[nameFir] = ReFir();
        ReFir &firThis = mFirs[nameFir];

        foreach ( const ReArea &area, fir.mAreas )
        {
            if ( area.mType != enUserNFZ )
                continue;

            firThis.mAreas << area;
        }

        // Do not need FIR without areas.
        if ( firThis.mAreas.empty() )
            mFirs.remove(nameFir);
        else
        {
            firThis.mBoundingBox = fir.mBoundingBox;
            firThis.mCenterLatitude = fir.mCenterLatitude;
            firThis.mCenterLongitude = fir.mCenterLongitude;
        }
    }

    if ( !mFirs.empty() )
        qDebug() << tr("NFZchecker: 'No Conflict' zones were successfully updated");
}

bool NFZchecker::isInside(const AirplaneInfo &ai)
{
    QReadLocker locker(&mLock);

    AirplaneInfo airPlane = ai;

    CCoord coord(ai.y, ai.x, ai.height);
    mCoordSys->toModel(coord);
    airPlane.x = coord.GetX();
    airPlane.y = coord.GetY();

    // Test what fir airplane on.
    for ( TFirMap::const_iterator itcFir = mFirs.constBegin(); itcFir != mFirs.constEnd(); ++itcFir )
    {
        const ReFir &fir = *itcFir;

        // Check that airplane is above current FIR.
        if ( !fir.mBoundingBox.contains(airPlane.x, airPlane.y) )
            continue;

        foreach ( const ReArea &area, fir.mAreas )
        {
            // Check if restriction zone is working (valid).
            if ( !area.isValid(QDateTime::currentDateTime()) )
                continue;

            // Check hit inside polygon.
            if ( area.mBoundingBox.contains(airPlane.x, airPlane.y) &&
                 area.mHeightMin <= airPlane.height && airPlane.height <= area.mHeightMax )
            {
                if ( rewarn::isInside(airPlane, area) ) // precise check
                    return true;
            }
        }
    }
    return false;
}

}   // namespace rewarn {
