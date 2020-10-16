#ifndef AREA_H
#define AREA_H

#include <QtCore/QMap>
#include <QtCore/QRectF>
#include <QtGui/QPolygonF>

#include "RePoint.h"

namespace rewarn {

struct Area
{
    //! Map of area points.Key - serial number
    typedef QMap<int, RePoint> TPointList;

    //! Constructor
    Area() : mID(0), mZoneID(0), mHeightMin(-1e37), mHeightMax(1e37) {}

    //! Prepare area for calculations.
    void prepare();

    quint32 mID;
    QString mName;
    quint32 mZoneID;
    double mHeightMin;
    double mHeightMax;
    TPointList mPoints;

    /*! Cartesian polygon */
    QPolygonF   mPolygon;
    /*! Cartesian bounding box. */
    QRectF      mBoundingBox;
};

}

#endif // AREA_H
