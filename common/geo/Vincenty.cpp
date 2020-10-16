#include "Vincenty.h"
#include <qmath.h>

template<class U, class V>
static double distance(const U &a, const V &b)
{
    double x = a.x() - b.x();
    double y = a.y() - b.y();
    return qSqrt(x*x + y*y);
}

template<class U, class V>
static double azimuth(const U &point, const V &center)
{
    double result = qAtan2(center.x() - point.x(), center.y() - point.y());
    result = fmod(result, 2 * M_PI);
    if (result < 0)
        result = 2 * M_PI + result;
    return result;
}

//static int iround(double val)
//{
//    return (val >= 0) ? val + 0.5 : val - 0.5;
//}

static const double earthRadius = 6372795; // in meters

QPointF Vincenty::toCartesian(const GeoPoint &point, const GeoPoint &center, const QPointF &centerPoint)
{
    // http://gis-lab.info/qa/great-circles.html

    if (point == center)
        return centerPoint;

    // Inverse calculation
    // points in radians
    double lat1 = center.latitudeRad();
    double lat2 = point.latitudeRad();
    double lon1 = center.longitudeRad();
    double lon2 = point.longitudeRad();

    // cos & sin latitudes and delta longitude
    double lat1cos = qCos(lat1);
    double lat2cos = qCos(lat2);
    double lat1sin = qSin(lat1);
    double lat2sin = qSin(lat2);
    double lonDelta = lon2 - lon1;
    double lonDeltaCos = qCos(qFabs(lonDelta));
    double lonDeltaSin = qSin(lonDelta);

    // the distance of a big circle
    double p1 = lat2cos * lonDeltaSin;
    double p2 = (lat1cos * lat2sin) - (lat1sin * lat2cos * lonDeltaCos);
    double angleRad = atan2(qSqrt(p1 * p1 + p2 * p2), lat1sin * lat2sin + lat1cos * lat2cos * lonDeltaCos);
    double distance = angleRad * earthRadius;

    // azimuth
    double azimuth = qAtan2(lonDeltaSin * lat2cos, lat1cos * lat2sin - lat1sin * lat2cos * lonDeltaCos);
    double azimuthRad = fmod(azimuth, 2 * M_PI);

    return centerPoint + QPointF(distance * sin(azimuthRad), distance * cos(azimuthRad));
}

GeoPoint Vincenty::toGeographic(const QPointF &point, const GeoPoint &center, const QPointF &centerPoint)
{
    // http://en.wikipedia.org/wiki/Vincenty%27s_formulae
    // distance and azimuth
    if (point == centerPoint)
        return center;

    double dist = distance(point, centerPoint);
    double azim = M_PI - azimuth(point, centerPoint);

    // Direct calculation
    double angleRad = dist / earthRadius;
    double lat1 = center.latitudeRad();
    double lon1 = center.longitudeRad();

    // cos & sin
    double lat1sin = qSin(lat1);
    double angleSin  = qSin(angleRad);
    double azimSin = qSin(azim);
    double lat1cos = qCos(lat1);
    double angleCos = qCos(angleRad);
    double azimCos = qCos(azim);

    // general parts
    double p1 = angleSin * azimCos;
    double p2 = lat1sin * angleSin;
    double p3 = lat1cos * angleCos;

    // latitude
    double latU = lat1sin * angleCos + lat1cos * p1;
    double latD = pow(pow(lat1cos * azimSin, 2) + pow(lat1sin*angleSin - p3 * azimCos, 2), 0.5);
    double lat2 = atan2(latU, latD);

    // longitude
    double lonU = angleSin * azimSin;
    double lonD = p3 - p2 * azimCos;
    double lon2 = lon1 - atan2(lonU, lonD);

    return GeoPoint(lon2 * 180 / M_PI, lat2 * 180 / M_PI);
}

double Vincenty::distance(const GeoPoint &a, const GeoPoint &b)
{
    if (a == b)
        return 0;

    // Inverse calculation
    // points in radians
    double lat1 = a.latitudeRad();
    double lat2 = b.latitudeRad();
    double lon1 = a.longitudeRad();
    double lon2 = b.longitudeRad();

    // cos & sin latitudes and delta longitude
    double lat1cos = qCos(lat1);
    double lat2cos = qCos(lat2);
    double lat1sin = qSin(lat1);
    double lat2sin = qSin(lat2);
    double lonDelta = lon2 - lon1;
    double lonDeltaCos = qCos(qFabs(lonDelta));
    double lonDeltaSin = qSin(lonDelta);

    // the distance of a big circle
    double p1 = lat2cos * lonDeltaSin;
    double p2 = (lat1cos * lat2sin) - (lat1sin * lat2cos * lonDeltaCos);
    double angleRad = atan2(qSqrt(p1 * p1 + p2 * p2), lat1sin * lat2sin + lat1cos * lat2cos * lonDeltaCos);
    return angleRad * earthRadius;
}

double Vincenty::fastDistance(const GeoPoint &a, const GeoPoint &b)
{
    if (a == b)
        return 0;

    // Inverse calculation
    // points in radians
    double lat1 = a.latitudeRad();
    double lat2 = b.latitudeRad();
    double lon1 = a.longitudeRad();
    double lon2 = b.longitudeRad();

    // cos & sin latitudes and delta longitude
    double lat1cos = qFastCos(lat1);
    double lat2cos = qFastCos(lat2);
    double lat1sin = qFastSin(lat1);
    double lat2sin = qFastSin(lat2);
    double lonDelta = lon2 - lon1;
    double lonDeltaCos = qFastCos(qFabs(lonDelta));
    double lonDeltaSin = qFastSin(lonDelta);

    // the distance of a big circle
    double p1 = lat2cos * lonDeltaSin;
    double p2 = (lat1cos * lat2sin) - (lat1sin * lat2cos * lonDeltaCos);
    double angleRad = atan2(qSqrt(p1 * p1 + p2 * p2), lat1sin * lat2sin + lat1cos * lat2cos * lonDeltaCos);
    return angleRad * earthRadius;
}
