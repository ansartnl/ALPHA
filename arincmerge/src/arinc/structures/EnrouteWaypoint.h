#ifndef ENROUTEWAYPOINT_H
#define ENROUTEWAYPOINT_H

#include "Enroute.h"
#include "Waypoint.h"

struct EnrouteWaypoint : public Waypoint, public Enroute
{
    static const char SubsectionCode;
    inline virtual char subsectionCode() const { return SubsectionCode; }
};

#endif // ENROUTEWAYPOINT_H
