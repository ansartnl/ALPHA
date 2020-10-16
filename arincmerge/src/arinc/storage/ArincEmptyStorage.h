#ifndef ARINCEMPTYSTORAGE_H
#define ARINCEMPTYSTORAGE_H

#include "FirUir.h"
#include "ControlledAirspace.h"
#include "RestrictedAirspace.h"
#include "VhfNavaid.h"
#include "NdbNavaid.h"
#include "EnrouteWaypoint.h"
#include "Airway.h"
#include "Holding.h"
#include "AirportRefPoint.h"
#include "TerminalWaypoint.h"
#include "SSA.h"
#include "Runway.h"
#include "NdbNavaid.h"
#include "NdbTerminal.h"

class ArincEmptyStorage
{
public:
    ArincEmptyStorage() {}
    inline virtual ~ArincEmptyStorage() {}
    inline virtual void add(const FirUir &) {}
    inline virtual void add(const ControlledAirspace &) {}
    inline virtual void add(const RestrictedAirspace &) {}
    inline virtual void add(const VhfNavaid &) {}
    inline virtual void add(const NdbNavaid &) {}
    inline virtual void add(const EnrouteWaypoint &) {}
    inline virtual void add(const Airway &) {}
    inline virtual void add(const Holding &) {}
    inline virtual void add(const AirportRefPoint &) {}
    inline virtual void add(const TerminalWaypoint &) {}
    inline virtual void add(const Sid &) {}
    inline virtual void add(const Star &) {}
    inline virtual void add(const Approach &) {}
    inline virtual void add(const Runway &) {}
    inline virtual void add(const NdbTerminal &) {}
};

#endif // ARINCEMPTYSTORAGE_H
