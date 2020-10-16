#ifndef AIRPLANE_H
#define AIRPLANE_H

#include "GeoPoint.h"
#include <QtCore/QTime>

struct Airplane
{
    Airplane();
    int trackNumber;
    GeoPoint position;
    double z;
    double vx;
    double vy;
    double vz;
    QTime recived;
    QByteArray buffer;  //!< Asterix buffer
};

struct Location
{
    Location() {}
    Location(const GeoPoint &bl, double h) : position(bl), z(h) {}

    GeoPoint position;
    double z;
};

#endif // AIRPLANE_H
