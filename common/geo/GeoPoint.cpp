#include "GeoPoint.h"
#include <qmath.h>

static void angleToComponents(double angle, int &degrees, int &minutes, int &seconds)
{
    double temp;
    degrees = (int)angle;
    temp = (angle - degrees) * 60;
    minutes = (int)temp;
    seconds = (temp - minutes) * 60;
    minutes = qAbs(minutes);
    seconds = qAbs(seconds);
}

GeoPoint::GeoPoint() : mLongitude(0), mLatitude(0)
{}

GeoPoint::GeoPoint(double longitude, double latitude)
{
    set(longitude, latitude);
}

double GeoPoint::latitude() const
{
    return mLatitude;
}

double GeoPoint::longitude() const
{
    return mLongitude;
}

double GeoPoint::latitudeRad() const
{
    return mLatitude * M_PI / 180;
}

double GeoPoint::longitudeRad() const
{
    return mLongitude * M_PI / 180;
}

void GeoPoint::setLatitude(double latitude)
{
    if (latitude > 90.0)
    {
        qWarning() << "GeoPoint: latitude is greater than 90 degree";
        latitude = fmod(latitude + 90.0, 180.0) - 90.0;
    }
    else if (latitude < -90.0)
    {
        qWarning() << "GeoPoint: latitude is less than -90 degree";
        latitude = fmod(latitude - 90.0, 180.0) + 90.0;
    }

    mLatitude = latitude;
}

void GeoPoint::setLongitude(double longitude)
{
    if (longitude > 180.0)
    {
        qWarning() << "GeoPoint: longitude is greater than 180 degree";
        longitude = fmod(longitude + 180.0, 360.0) - 180.0;
    }
    else if (longitude < -180.0)
    {
        qWarning() << "GeoPoint: longitude is less than -180 degree";
        longitude = fmod(longitude - 180.0, 360.0) + 180.0;
    }

    mLongitude = longitude;
}

void GeoPoint::set(double longitude, double latitude)
{
    setLongitude(longitude);
    setLatitude(latitude);
}

bool GeoPoint::operator==(const GeoPoint &other) const
{
    return qAbs(mLongitude - other.mLongitude) < 0.0000003
            && qAbs(mLatitude - other.mLatitude) < 0.0000003; // 1/1000 of a second
}

QString GeoPoint::toString() const
{
    int deg, min, sec;
    angleToComponents(mLatitude, deg, min, sec);
    QString result;
    result += mLatitude >= 0 ? 'N' : 'S';
    result += QString::number(qAbs(deg)).rightJustified(2, '0');
    result += ' ';
    result += QString::number(min).rightJustified(2, '0');
    result += ' ';
    result += QString::number(sec).rightJustified(2, '0');
    result += ' ';
    angleToComponents(mLongitude, deg, min, sec);
    result += mLongitude >= 0 ? 'E' : 'W';
    result += QString::number(qAbs(deg)).rightJustified(3, '0');
    result += ' ';
    result += QString::number(min).rightJustified(2, '0');
    result += ' ';
    result += QString::number(sec).rightJustified(2, '0');
    return result;
}

QDebug operator<<(QDebug dbg, const GeoPoint &p)
{
    //dbg << qPrintable(QString("GeoPoint(%1,%2)").arg(p.longitude(), 0, 'f', 3).arg(p.latitude(), 0, 'f', 3));
    dbg << qPrintable(QString("GeoPoint(%1)").arg(p.toString()));
    return dbg;
}
