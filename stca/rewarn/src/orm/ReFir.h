#ifndef _RE_FIR_H__
#define _RE_FIR_H__

#include <QtCore/QRectF>
#include <QtCore/QMap>

#ifdef REWARN_TEST
#   include "rewarn_global.h"
#endif

#include "ReArea.h"
#include "MeteoArea.h"

namespace rewarn {

typedef QList<ReArea>    TAreaList;
typedef QList<MeteoArea> TMeteoAreaList;

//! Structure describes one FIR
#ifdef REWARN_TEST
    struct REWARN_EXPORT ReFir
#else
    struct ReFir
#endif
{
    //! Prepare FIR for calculations.
    void prepare();

    TAreaList      mAreas;
    TMeteoAreaList mMeteoAreas;
    double         mCenterLatitude;
    double         mCenterLongitude;

    /*! Cartesian bounding box. */
    QRectF      mBoundingBox;
};

typedef QMap<QString, ReFir>    TFirMap;

}   // namespace rewarn {

#endif // _RE_FIR_H__
