#ifndef VINCENTY_H
#define VINCENTY_H

#include "GeoPoint.h"
#include <QtCore/QPointF>

namespace Vincenty
{
    QPointF toCartesian(const GeoPoint &point, const GeoPoint &center, const QPointF &centerPoint = QPointF(0.0, 0.0)); // mistake is about 0.5%
    GeoPoint toGeographic(const QPointF &point, const GeoPoint &center, const QPointF &centerPoint = QPointF(0.0, 0.0)); // mistake is about 0.5%
    double distance(const GeoPoint &a, const GeoPoint &b); // distance in meters
    double fastDistance(const GeoPoint &a, const GeoPoint &b);
}

#endif // VINCENTY_H
