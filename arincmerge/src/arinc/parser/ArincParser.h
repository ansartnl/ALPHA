#ifndef ARINCPARSER_H
#define ARINCPARSER_H

#include "ArincEmptyStorage.h"
#include "ArincFilePrimitiveIterator.h"

class ArincParser
{
    static const bool bSofterParser = true;

    ArincParser();
    static double toLatitude(const QByteArray& arr);
    static double toLongitude(const QByteArray& arr);
    static double toAzimuth(const QByteArray& arr);
    static double toAltitude(const QByteArray &arr);
    static int toInt(const QByteArray &arr, bool* bOk = 0);
    static uint toUint(const QByteArray &arr, bool* bOk = 0);
    static double toDouble(const QByteArray &arr, bool* bOk = 0);
    static FirUir parseFirUir(ArincFilePrimitiveIterator &it);
    static ControlledAirspace parseControlledAirspace(ArincFilePrimitiveIterator &it);
    static RestrictedAirspace parseRestrictedAirspace(ArincFilePrimitiveIterator &it);
    static VhfNavaid parseVhfNavaid(ArincFilePrimitiveIterator &it);
    static void parseNdb(Ndb &r, ArincFilePrimitiveIterator &it);
    static NdbNavaid parseNdbNavaid(ArincFilePrimitiveIterator &it);
    static void parseWaypoint(Waypoint &r, ArincFilePrimitiveIterator &it);
    static EnrouteWaypoint parseEnrouteWaypoint(ArincFilePrimitiveIterator &it);
    static TerminalWaypoint parseTerminalWaypoint(ArincFilePrimitiveIterator &it);
    static Airway parseAirway(ArincFilePrimitiveIterator &it);
    static Holding parseHolding(ArincFilePrimitiveIterator &it);
    static AirportRefPoint parseAirportRefPoint(ArincFilePrimitiveIterator &it);
    static void parseSSA(SSA &r, ArincFilePrimitiveIterator &it);
    static Sid parseSid(ArincFilePrimitiveIterator &it);
    static Star parseStar(ArincFilePrimitiveIterator &it);
    static Approach parseApproach(ArincFilePrimitiveIterator &it);
    static Runway parseRunway(ArincFilePrimitiveIterator &it);
    static NdbTerminal parseNdbTerminal(ArincFilePrimitiveIterator &it);
public:
    static bool parse(ArincFilePrimitiveIterator &it, ArincEmptyStorage &out);
};

#endif // ARINCPARSER_H
