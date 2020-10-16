#ifndef RUNWAY_H
#define RUNWAY_H

#include "Airport.h"
#include "GeoPoint.h"
#include <QtCore/QString>

struct Runway : public Airport
{
    static const char SubsectionCode;
    inline virtual char subsectionCode() const { return SubsectionCode; }

    QString areaCode;
    QString airportIcaoId;
    QString icaoCode;
    QString id;
    int length;
    double magneticBearing;
    GeoPoint coord;
    QByteArray gradient;
    QByteArray landingThresholdElevation;
    QByteArray displacedThresholdDistance;
    QByteArray thresholdCrossingHeight;
    int width;
    QByteArray pathId;
    char category;
    QByteArray stopWay;
    QByteArray pathId2;
    char category2;
    QString description;
};

#endif // RUNWAY_H
