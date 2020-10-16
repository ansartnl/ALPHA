#ifndef ARINCNATIVESTORAGE_H
#define ARINCNATIVESTORAGE_H

#include "ArincEmptyStorage.h"
#include <QtCore/QList>

class ArincNativeStorage : public ArincEmptyStorage
{
public:
    ArincNativeStorage() {}
    inline virtual ~ArincNativeStorage() {}
    inline virtual void add(const FirUir &in) { firs.push_back(in); }
    inline virtual void add(const ControlledAirspace &in) { sectors.push_back(in); }
    inline virtual void add(const RestrictedAirspace &in) { restrictions.push_back(in); }
    inline virtual void add(const VhfNavaid &in) { vordme.push_back(in); }
    inline virtual void add(const NdbNavaid &in) { ndb.push_back(in); }
    inline virtual void add(const EnrouteWaypoint &in) { waypoints.push_back(in); }
    inline virtual void add(const Airway &in) { routes.push_back(in); }
    inline virtual void add(const Holding &in) { holdings.push_back(in); }
    inline virtual void add(const AirportRefPoint &in) { airports.push_back(in); }
    inline virtual void add(const TerminalWaypoint &in) { terminalWaypoints.push_back(in); }
    inline virtual void add(const Sid &in) { sids.push_back(in); }
    inline virtual void add(const Star &in) { stars.push_back(in); }
    inline virtual void add(const Approach &in) { approachs.push_back(in); }
    inline virtual void add(const Runway &in) { runways.push_back(in); }
    inline virtual void add(const NdbTerminal &in) { ndbterminals.push_back(in); }

    QList<FirUir> firs;
    QList<ControlledAirspace> sectors;
    QList<RestrictedAirspace> restrictions;
    QList<VhfNavaid> vordme;
    QList<NdbNavaid> ndb;
    QList<EnrouteWaypoint> waypoints;
    QList<Airway> routes;
    QList<Holding> holdings;
    QList<AirportRefPoint> airports;
    QList<TerminalWaypoint> terminalWaypoints;
    QList<Sid> sids;
    QList<Star> stars;
    QList<Approach> approachs;
    QList<Runway> runways;
    QList<NdbTerminal> ndbterminals;
};

#endif // ARINCNATIVESTORAGE_H
