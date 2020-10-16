#ifndef BOUNDARYPOINT_H
#define BOUNDARYPOINT_H

#include "GeoPoint.h"

struct BoundaryPoint
{
    BoundaryPoint();

    enum Type
    {
        ArcByEdge = 'A',
        Circle = 'C',
        GreatCircle = 'G',
        RhumbLine = 'H',
        CounterClockwiseArc = 'L',
        ClockwiseArc = 'R'
    } type;

    int sn;
    GeoPoint coord;
    GeoPoint arcCoord;
    double arcDistance;
    double arcBearing;
};

#endif // BOUNDARYPOINT_H
