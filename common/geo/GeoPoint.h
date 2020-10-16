#ifndef GEOPOINT_H
#define GEOPOINT_H

#include <QtCore/QDebug>

class GeoPoint
{
public:
    GeoPoint();
    GeoPoint(double longitude, double latitude);

    double longitude() const;
    double latitude() const;

    double longitudeRad() const;
    double latitudeRad() const;

    void setLongitude(double longitude);
    void setLatitude(double latitude);
    void set(double longitude, double latitude);

    bool operator==(const GeoPoint &other) const;
    QString toString() const;

private:
    double mLongitude; // longitude in degrees from -180(west) to 180(east)
    double mLatitude; // latitude in degrees from -90(south) to 90(torth)
};

QDebug operator<<(QDebug dbg, const GeoPoint &p);

#endif // GEOPOINT_H
