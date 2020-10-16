#ifndef AIRPORTREFPOINT_H
#define AIRPORTREFPOINT_H

#include "Airport.h"
#include "GeoPoint.h"
#include <QtCore/QString>

struct AirportRefPoint : public Airport
{
    static const char SubsectionCode;
    inline virtual char subsectionCode() const { return SubsectionCode; }
    QString areaCode;
    QString airportIcaoId;
    QString icaoCode;
    QByteArray ataIataDesignator; //?
    QByteArray speedLimitAltitude; //?
    QByteArray longestRunway; //?
    char ifrCapability; //?
    char longestRunwaySurfaceCode; //?
    GeoPoint coord;
    QByteArray magneticVariation; //?
    double elevation;
    QByteArray speedLimit; //?
    QByteArray recomendedNavaid; //?
    QString icaoCode2; //?
    QByteArray transitionAltitude; //?
    QByteArray transitionLevel; //?
    enum Accessibility
    {
        Public = 'C',
        Military = 'M',
        Private = 'P'
    } acces;
    QByteArray timeZone;
    char daylight;
    enum NorthType
    {
        MagneticNorth = 'M',
        TrueNorth = 'T'
    } north;
    QByteArray datumCode;
    QString name;
};

#endif // AIRPORTREFPOINT_H
