#ifndef TERMINALWAYPOINT_H
#define TERMINALWAYPOINT_H

#include "Waypoint.h"
#include "Airport.h"

struct TerminalWaypoint : public Waypoint, public Airport
{
    static const char SubsectionCode;
    inline virtual char subsectionCode() const { return SubsectionCode; }
};

#endif // TERMINALWAYPOINT_H
