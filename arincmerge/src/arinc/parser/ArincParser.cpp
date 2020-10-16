#include "ArincParser.h"
#include "ContinuantionNumber.h"

ArincParser::ArincParser()
{}

double ArincParser::toLatitude(const QByteArray &arr)
{
    if (bSofterParser && arr.trimmed().isEmpty())
        return 0.0;

    if (9 != arr.size())
        throw QString("Invalid latitude field size");

    bool bOk(true);
    double result = arr.mid(1, 2).toInt(&bOk, 10);
    if (!bOk)
        throw QString("Invalid latitude degree");
    result += arr.mid(3, 2).toInt(&bOk, 10) / 60.;
    if (!bOk)
        throw QString("Invalid latitude minutes");
    result += arr.mid(5, 4).toInt(&bOk, 10) / 360000.;
    if (!bOk)
        throw QString("Invalid latitude seconds");

    switch (arr[0])
    {
    case 'N':
        break;
    case 'S':
        result *= -1;
        break;
    default:
        throw QString("Invalid latitude alpha character");
    }

    return result;
}

double ArincParser::toLongitude(const QByteArray &arr)
{
    if (bSofterParser && arr.trimmed().isEmpty())
        return 0.0;

    if (10 != arr.size())
        throw QString("Invalid longitude field size");

    bool bOk(true);
    double result = arr.mid(1, 3).toInt(&bOk, 10);
    if (!bOk)
        throw QString("Invalid longitude degree");
    result += arr.mid(4, 2).toInt(&bOk, 10) / 60.;
    if (!bOk)
        throw QString("Invalid longitude minutes.");
    result += arr.mid(6, 4).toInt(&bOk, 10) / 360000.;
    if (!bOk)
        throw QString("Invalid longitude seconds");

    switch (arr[0])
    {
    case 'E':
        break;
    case 'W':
        result *= -1;
        break;
    default:
        throw QString("Invalid longitude alpha character");
    }

    return result;
}

double ArincParser::toAzimuth(const QByteArray &arr)
{
    if (bSofterParser && arr.trimmed().isEmpty())
        return 0.0;

    if (4 != arr.size())
        throw QString("invalid azimuth field size");

    double result = 0;
    bool bOk(true);
    if ('T' == arr[3])
        result = arr.left(3).toInt(&bOk);
    else
        result = arr.toInt(&bOk) / 10.0;
    if (!bOk)
        throw QString("invalid azimuth value");
    return result;
}

double ArincParser::toAltitude(const QByteArray &arr)
{
    if (bSofterParser && arr.trimmed().isEmpty())
        return 0.0;

    if (5 != arr.size())
        throw QString("invalid altitude field size");

    double result;
    bool bOk;
    if (arr.left(2) == "FL")
        result = arr.right(3).toUInt(&bOk) * 30.48;
    else
        result = arr.toUInt(&bOk) * 0.3048;
    if (!bOk)
        throw QString("invalid altitude value");
    return result;
}

int ArincParser::toInt(const QByteArray &arr, bool *bOk)
{
    if(bOk)
        *bOk = true;
    if (bSofterParser && arr.trimmed().isEmpty())
        return 0;
    int iVal = arr.toInt(bOk);
    return iVal;
}

uint ArincParser::toUint(const QByteArray &arr, bool *bOk)
{
    if(bOk)
        *bOk = true;
    if (bSofterParser && arr.trimmed().isEmpty())
        return 0;
    uint uiVal = arr.toUInt(bOk);
    return uiVal;
}

double ArincParser::toDouble(const QByteArray &arr, bool *bOk)
{
    if(bOk)
        *bOk = true;
    if (bSofterParser && arr.trimmed().isEmpty())
        return 0;
    double dVal = arr.toDouble(bOk);
    return dVal;
}

FirUir ArincParser::parseFirUir(ArincFilePrimitiveIterator &it)
{
    FirUir r;
    r.file = it.fullFileName();
    r.line = it.line();
    r.areaCode = it.str(2, 3);
    r.id = it.str(7, 4);
    r.address = it.str(11, 4);
    r.type = FirUir::Type(it[15]);
    r.rus = FirUir::ReportingUnitsSpeed(it[29]);
    r.rua = FirUir::ReportingUnitsAltitude(it[30]);
    r.er = FirUir::EntryReport(it[31]);
    if (FirUir::Fir == r.type || FirUir::Both == r.type)
        r.firUpperLimit = UpperLowerLimit::fromArray(it.arr(81, 5));
    if (FirUir::Uir == r.type || FirUir::Both == r.type)
    {
        r.uirLowerLimit = UpperLowerLimit::fromArray(it.arr(86, 5));
        r.uirUpperLimit = UpperLowerLimit::fromArray(it.arr(91, 5));
    }

    r.cruiseTableIdentifier = it.arr(96, 2);
    r.name = it.str(99, 25);

    const QByteArray mainMarker = it.arr(1, 15);

    bool bOk;
    Q_FOREVER
    {
        if (it.arr(1, 15) != mainMarker)
            break;

        try
        {

            BoundaryPointFirUir point;
            point.adjacentFir = it.str(21, 4);
            point.adjacentUir = it.str(25, 4);
            point.sn = toUint(it.arr(16, 4), &bOk);
            if (!bOk)
                throw QString("invalid sequence number");
            point.type = BoundaryPoint::Type(it[33]);
            if (BoundaryPoint::ArcByEdge == point.type // A
                || BoundaryPoint::GreatCircle == point.type // G
                || BoundaryPoint::RhumbLine == point.type // H
                || BoundaryPoint::CounterClockwiseArc == point.type // L
                || BoundaryPoint::ClockwiseArc == point.type) // R
            {
                point.coord.set(toLongitude(it.arr(44, 10)), toLatitude(it.arr(35, 9)));
            }
            if (BoundaryPoint::ArcByEdge == point.type // A
                || BoundaryPoint::Circle == point.type // C
                || BoundaryPoint::CounterClockwiseArc == point.type // L
                || BoundaryPoint::ClockwiseArc == point.type) // R
            {
                point.arcCoord.set(toLongitude(it.arr(63, 10)), toLatitude(it.arr(54, 9)));
                point.arcDistance = toDouble(it.arr(73, 4), &bOk) * 185.2;
                if (!bOk)
                    point.arcDistance = 0.0;
                point.arcBearing = toDouble(it.arr(77, 4), &bOk);
                if (!bOk)
                    point.arcBearing = 0.0;
            }
            r.boundary.push_back(point);
        }
        catch(const QString &err)
        {
            qWarning() << "Unable to load FIR/UIR boundary point at" << it.desctiption() << ": " << err;
        }

        bool bEnd = 'E' == it[34];
        if (!it.next() || bEnd)
            break;
    }
    return r;
}

ControlledAirspace ArincParser::parseControlledAirspace(ArincFilePrimitiveIterator &it)
{
    ControlledAirspace r;
    r.file = it.fullFileName();
    r.line = it.line();
    r.areaCode = it.str(2, 3);
    r.icaoCode = it.str(7, 2);
    r.type = ControlledAirspace::Type(it[9]);
    r.center = it.str(10, 5);
    r.centerSectionCode = it[15];
    r.centerSubsectionCode = it[16];
    r.classification = it[17];
    r.multipleCode = it[20];
    r.level = ArincGlobal::Level(it[26]);
    r.timeCode = it[27];
    r.notam = it[28];
    r.rnp = it.arr(79, 3);
    r.lowerLimit = UpperLowerLimit::fromArray(it.arr(82, 5));
    r.lowerUi = ArincGlobal::UnitIndicator(it[87]);
    r.upperLimit = UpperLowerLimit::fromArray(it.arr(88, 5));
    r.upperUi = ArincGlobal::UnitIndicator(it[93]);
    r.name = it.str(94, 30);

    const QByteArray mainMarker = it.arr(1, 20);

    bool bOk;
    Q_FOREVER
    {
        if (it.arr(1, 20) != mainMarker)
            return r;

        try
        {
            BoundaryPoint point;
            point.sn = toUint(it.arr(21, 4), &bOk);
            if (!bOk)
                throw QString("invalid sequence number");
            point.type = BoundaryPoint::Type(it[31]);
            if (BoundaryPoint::ArcByEdge == point.type // A
                || BoundaryPoint::GreatCircle == point.type // G
                || BoundaryPoint::RhumbLine == point.type // H
                || BoundaryPoint::CounterClockwiseArc == point.type // L
                || BoundaryPoint::ClockwiseArc == point.type) // R
            {
                point.coord.set(toLongitude(it.arr(42, 10)), toLatitude(it.arr(33, 9)));
            }
            if (BoundaryPoint::ArcByEdge == point.type // A
                || BoundaryPoint::Circle == point.type // C
                || BoundaryPoint::CounterClockwiseArc == point.type // L
                || BoundaryPoint::ClockwiseArc == point.type) // R
            {
                point.arcCoord.set(toLongitude(it.arr(61, 10)), toLatitude(it.arr(52, 9)));
                point.arcDistance = toDouble(it.arr(71, 4), &bOk) * 185.2;
                if (!bOk)
                    point.arcDistance = 0.0;
                point.arcBearing = toDouble(it.arr(75, 4), &bOk);
                if (!bOk)
                    point.arcBearing = 0.0;
            }
            r.boundary.push_back(point);
        }
        catch(const QString &err)
        {
            qWarning() << "Unable to load controlled airspace boundary point at" << it.desctiption() << ": " << err;
        }

        bool leave = 'E' == it[32];

        const QByteArray continuationMarker = it.arr(1, 24);
        ContinuantionNumber cn(it[25]);
        Q_FOREVER // processint continuation records
        {
            if (!it.next())
                return r;
            if (it.arr(1, 24) != continuationMarker || it[25] != cn.next())
                break;
        }

        if (leave)
            break;
    }
    return r;
}

RestrictedAirspace ArincParser::parseRestrictedAirspace(ArincFilePrimitiveIterator &it)
{
    RestrictedAirspace r;
    r.file = it.fullFileName();
    r.line = it.line();
    r.areaCode = it.str(2, 3);
    r.icaoCode = it.str(7, 2);
    r.type = RestrictedAirspace::Type(it[9]);
    r.designation = it.str(10, 10);
    r.multipleCode = it[20];
    r.level = ArincGlobal::Level(it[26]);
    r.timeCode = it[27];
    r.notam = it[28];
    r.lowerLimit = UpperLowerLimit::fromArray(it.arr(82, 5));
    r.lowerUi = ArincGlobal::UnitIndicator(it[87]);
    r.upperLimit = UpperLowerLimit::fromArray(it.arr(88, 5));
    r.upperUi = ArincGlobal::UnitIndicator(it[93]);
    r.name = it.str(94, 30);

    const QByteArray mainMarker = it.arr(1, 15);

    bool bOk;
    Q_FOREVER
    {
        if (it.arr(1, 15) != mainMarker)
            return r;

        const QByteArray continuationMarker = it.arr(1, 24);
        ContinuantionNumber cn(it[25]);

        try
        {
            BoundaryPoint point;
            point.sn = toUint(it.arr(21, 4), &bOk);
            if (!bOk)
                throw QString("invalid sequence number");
            point.type = BoundaryPoint::Type(it[31]);
            if (BoundaryPoint::ArcByEdge == point.type // A
                || BoundaryPoint::GreatCircle == point.type // G
                || BoundaryPoint::RhumbLine == point.type // H
                || BoundaryPoint::CounterClockwiseArc == point.type // L
                || BoundaryPoint::ClockwiseArc == point.type) // R
            {
                point.coord.set(toLongitude(it.arr(42, 10)), toLatitude(it.arr(33, 9)));
            }
            if (BoundaryPoint::ArcByEdge == point.type // A
                || BoundaryPoint::Circle == point.type // C
                || BoundaryPoint::CounterClockwiseArc == point.type // L
                || BoundaryPoint::ClockwiseArc == point.type) // R
            {
                point.arcCoord.set(toLongitude(it.arr(61, 10)), toLatitude(it.arr(52, 9)));
                point.arcDistance = it.arr(71, 4).toDouble(&bOk) * 185.2;
                if (!bOk)
                    point.arcDistance = 0.0;
                point.arcBearing = toDouble(it.arr(75, 4), &bOk);
                if (!bOk)
                    point.arcBearing = 0.0;
            }
            r.boundary.push_back(point);
        }
        catch(const QString &err)
        {
            qWarning() << "Unable to load restricted airspace boundary point at" << it.desctiption() << ": " << err;
        }

        bool leave = 'E' == it[32];

        Q_FOREVER // processint continuation records
        {
            if (!it.next())
                return r;
            if (it.arr(1, 24) != continuationMarker || it[25] != cn.next())
                break;
        }

        if (leave)
            break;
    }
    return r;
}


VhfNavaid ArincParser::parseVhfNavaid(ArincFilePrimitiveIterator &it)
{
    VhfNavaid r;
    r.file = it.fullFileName();
    r.line = it.line();
    r.areaCode = it.str(2, 3);
    r.airportIcaoId = it.str(7, 4);
    r.icaoCode1 = it.str(11, 2);
    r.vorId = it.str(14, 4);
    r.icaoCode2 = it.str(20, 2);
    r.vorFrequency = toInt(it.arr(23, 5));
    r.navaidClass = it.arr(28, 5);
    try { r.vorCoord.set(toLongitude(it.arr(42, 10)), toLatitude(it.arr(33, 9))); }
    catch(const QString &) {}
    r.dmeId = it.str(52, 4);
    try { r.dmeCoord.set(toLongitude(it.arr(65, 10)), toLatitude(it.arr(56, 9))); }
    catch(const QString &) {}
    r.stationDeclination = it.arr(75, 7);
    r.dmeElevation = toInt(it.arr(80, 5));
    r.figureOfMerit = VhfNavaid::FigureOfMerit(it[85]);
    r.bias = toInt(it.arr(86, 2));
    r.frequencyProtection = toInt(it.arr(88, 3));
    r.datumCode = it.str(91, 3);
    r.name = it.str(94, 30);

    const QByteArray continuationMarker = it.arr(1, 21);
    ContinuantionNumber cn(it[22]);

    Q_FOREVER // processint continuation records
    {
        if (!it.next() || it.arr(1, 21) != continuationMarker || it[22] != cn.next())
            break;
    }

    return r;
}

void ArincParser::parseNdb(Ndb &r, ArincFilePrimitiveIterator &it)
{
    r.areaCode = it.str(2, 3);
    r.airportIcaoId = it.str(7, 4);
    r.icaoCode1 = it.str(11, 2);
    r.ndbId = it.str(14, 4);
    r.icaoCode2 = it.str(20, 2);
    r.ndbFrequency = toInt(it.arr(23, 5));
    r.ndbClass = it.arr(28, 5);
    try { r.ndbCoord.set(toLongitude(it.arr(42, 10)), toLatitude(it.arr(33, 9))); }
    catch(const QString &) {}
    r.magneticVariation = it.arr(75, 5);
    r.datumCode = it.str(91, 3);
    r.name = it.str(94, 30);

    const QByteArray continuationMarker = it.arr(1, 21);
    ContinuantionNumber cn(it[22]);

    Q_FOREVER // processint continuation records
    {
        if (!it.next() || it.arr(1, 21) != continuationMarker || it[22] != cn.next())
            break;
    }
}

NdbNavaid ArincParser::parseNdbNavaid(ArincFilePrimitiveIterator &it)
{
    NdbNavaid r;
    r.file = it.fullFileName();
    r.line = it.line();
    parseNdb(r, it);
    return r;
}

NdbTerminal ArincParser::parseNdbTerminal(ArincFilePrimitiveIterator &it)
{
    NdbTerminal r;
    r.file = it.fullFileName();
    r.line = it.line();
    parseNdb(r, it);
    return r;
}

void ArincParser::parseWaypoint(Waypoint &r, ArincFilePrimitiveIterator &it)
{
    QString errorString;
    try
    {
        r.areaCode = it.str(2, 3);
        r.regionCode = it.str(7, 4);
        r.icaoCode1 = it.str(11, 2);
        r.id = it.str(14, 5);
        r.icaoCode2 = it.str(20, 2);
        r.type = it.arr(27, 3);
        r.usage = it.arr(30, 2);
        r.coord.set(toLongitude(it.arr(42, 10)), toLatitude(it.arr(33, 9)));
        r.magneticVariation = it.arr(75, 5);
        r.datumCode = it.str(85, 3);
        r.nameFormat = it.arr(96, 3);
        r.name = it.str(99, 25);
    }
    catch(const QString &err)
    {
        errorString = QString("Unable to load waypoint %1 at %2").arg(err).arg(it.desctiption());
    }

    const QByteArray continuationMarker = it.arr(1, 21);
    ContinuantionNumber cn(it[22]);

    Q_FOREVER // processint continuation records
    {
        if (!it.next() || it.arr(1, 21) != continuationMarker || it[22] != cn.next())
            break;
    }

    if (!errorString.isEmpty())
        throw errorString;
}

EnrouteWaypoint ArincParser::parseEnrouteWaypoint(ArincFilePrimitiveIterator &it)
{
    EnrouteWaypoint r;
    r.file = it.fullFileName();
    r.line = it.line();
    parseWaypoint(r, it);
    return r;
}

TerminalWaypoint ArincParser::parseTerminalWaypoint(ArincFilePrimitiveIterator &it)
{
    TerminalWaypoint r;
    r.file = it.fullFileName();
    r.line = it.line();
    parseWaypoint(r, it);
    return r;
}

Airway ArincParser::parseAirway(ArincFilePrimitiveIterator &it)
{
    Airway r;
    r.file = it.fullFileName();
    r.line = it.line();
    r.areaCode = it.str(2, 3);
    r.id = it.str(14, 5);

    const QByteArray mainMarker = it.arr(1, 25);

    Q_FOREVER
    {
        const QByteArray continuationMarker = it.arr(1, 38);
        ContinuantionNumber cn(it[39]);

        try
        {
            Airway::Point p;
            bool bOk;
            p.sn = toInt(it.arr(26, 4), &bOk);
            if (!bOk)
                throw QString("invalid sequence number");
            p.id = it.str(30, 5);
            p.icaoCode1 = it.str(35, 2);
            p.sectionCode = it[37];
            p.subsectionCode = it[38];
            p.descriptionCode = it.arr(40, 4);
            p.boundaryCode = it[44];
            p.routeType = it[45];
            p.level = it[46];
            p.directionRestriction = it[47];
            p.cruiseTableIndicator = it.arr(48, 2);
            p.euIndicator = it[50];
            p.recomendedNavaid = it.arr(51, 4);
            p.icaoCode2 = it.str(55, 2);
            p.rnp = it.arr(57, 3);
            p.theta = it.arr(63, 4);
            p.rho = it.arr(67, 4);
            p.outboundMagneticCourse = it.arr(71, 4);
            p.routeDistanceFrom = it.arr(75, 4);
            p.inboundMagneticCourse = it.arr(79, 4);
            p.minAltitude1 = it.arr(84, 5);
            p.minAltitude2 = it.arr(89, 5);
            p.maxAltitude = it.arr(94, 5);
            r.points.push_back(p);
        }
        catch (const QString &err)
        {
            qWarning() << "Unable to load Airway point at" << it.desctiption() << ": " << err;
        }

        Q_FOREVER // processint continuation records
        {
            if (!it.next())
                return r;

            if (it.arr(1, 38) != continuationMarker || it[39] != cn.next())
                break;
        }

        if (mainMarker != it.arr(1, 25))
            break;
    }
    return r;
}

Holding ArincParser::parseHolding(ArincFilePrimitiveIterator &it)
{
    Holding r;
    r.file = it.fullFileName();
    r.line = it.line();
    QString errorString;
    try
    {
        bool bOk;
        r.areaCode = it.str(2, 3);
        r.regionCode = it.str(7, 4);
        r.icaoCode = it.str(11, 2);
        r.type = Holding::Type(it[28]);
        r.duplicate = it[29];
        r.pointId = it.str(30, 5);
        r.pointIcaoCode = it.str(35, 2);
        r.pointSectionCode = it[37];
        r.pointSubsectionCode = it[38];
        r.inboundHoldingCourse = toAzimuth(it.arr(40, 4));
        r.turn = ArincGlobal::TurnDirection(it[44]);
        r.legLength = toInt(it.arr(45, 3), &bOk) * 185.2;
        if (!bOk)
            r.legLength = 0.0;
        r.legTime = toInt(it.arr(48, 2), &bOk) * 6;
        if (!bOk)
            r.legTime = 0;
        r.minAltitude = it.arr(50, 5);
        r.maxAltitude = it.arr(55, 5);
        r.speed = toInt(it.arr(60, 3), &bOk) * 1852. / 3600.;
        if (!bOk)
            r.speed = 0.0;
        r.note = it.str(99, 25);
    }
    catch (const QString &err)
    {
        errorString = QString("Unbale to load holdind %1 at %2").arg(err).arg(it.desctiption());
    }

    const QByteArray continuationMarker = it.arr(1, 38);
    ContinuantionNumber cn(it[39]);

    Q_FOREVER // processint continuation records
    {
        if (!it.next() || it.arr(1, 38) != continuationMarker || it[39] != cn.next())
            break;
    }

    if (!errorString.isEmpty())
        throw errorString;

    return r;
}

AirportRefPoint ArincParser::parseAirportRefPoint(ArincFilePrimitiveIterator &it)
{
    AirportRefPoint r;
    r.file = it.fullFileName();
    r.line = it.line();
    QString errorString;
    try
    {
        bool bOk;
        r.areaCode = it.str(2, 3);
        r.airportIcaoId = it.str(7, 4);
        r.icaoCode = it.str(11, 2);
        r.ataIataDesignator = it.arr(14, 3);
        r.speedLimitAltitude = it.arr(23, 5);
        r.longestRunway = it.arr(28, 3);
        r.ifrCapability = it[31];
        r.longestRunwaySurfaceCode = it[32];
        r.coord.set(toLongitude(it.arr(42, 10)), toLatitude(it.arr(33, 9)));
        r.magneticVariation = it.arr(52, 5);
        r.elevation = toDouble(it.arr(57, 5), &bOk) * 0.3048;
        if (!bOk)
            r.elevation = 0;
        r.speedLimit = it.arr(62, 3);
        r.recomendedNavaid = it.arr(65, 4);
        r.icaoCode2 = it.str(69, 2);
        r.transitionAltitude = it.arr(71, 5);
        r.transitionLevel = it.arr(76, 5);
        r.acces = AirportRefPoint::Accessibility(it[81]);
        r.timeZone = it.arr(82, 3);
        r.daylight = it[85];
        r.north = AirportRefPoint::NorthType(it[86]);
        r.datumCode = it.arr(87, 3);
        r.name = it.str(94, 30);
    }
    catch (const QString &err)
    {
        errorString = QString("Unbale to load AirportRefPoint %1 at %2").arg(err).arg(it.desctiption());
    }

    const QByteArray continuationMarker = it.arr(1, 21);
    ContinuantionNumber cn(it[22]);

    Q_FOREVER // processint continuation records
    {
        if (!it.next() || it.arr(1, 21) != continuationMarker || it[22] != cn.next())
            break;
    }

    if (!errorString.isEmpty())
        throw errorString;

    return r;
}

void ArincParser::parseSSA(SSA &r, ArincFilePrimitiveIterator &it)
{
    r.file = it.fullFileName();
    r.line = it.line();
    r.areaCode = it.str(2, 3);
    r.airportId = it.str(7, 4);
    r.icaoCode = it.str(11, 2);
    r.id = it.str(14, 6);
    r.routeType = it[20];
    r.transitionId = it.str(21, 5);

    const QByteArray mainMarker = it.arr(1, 26);

    Q_FOREVER
    {
        if (mainMarker != it.arr(1, 26))
            break;

        try
        {
            SSA::Point p;
            bool bOk;
            p.sn = toInt(it.arr(27, 3), &bOk);
            if (!bOk)
                throw QString("invalid sequence number field");
            p.id = it.str(30, 5);
            p.icaoCode = it.str(35, 2);
            p.sectionCode = it[37];
            p.subsrectionCode = it[38];
            p.descriptionCode = it.arr(40, 4);
            p.turn = ArincGlobal::TurnDirection(it[44]);
            p.rnp = it.arr(45, 3);
            p.path = it.arr(48, 2);
            p.turnDirectionValid = it[50];
            p.recomendedNavaid  = it.arr(51, 4);
            p.icaoCode2 = it.str(55, 2);
            p.arcRadius = it.arr(57, 6);
            p.thera = it.arr(63, 4);
            p.rho = it.arr(67, 4);
            p.course = toAzimuth(it.arr(71, 4));
            p.distance = it.arr(75, 4);
            p.recdNavSectOn = it[79];
            p.rectNavSubsection = it[80];
            p.altitudeDescription = it[83];
            p.atcIndicator = it[84];
            p.altitude1 = toAltitude(it.arr(85, 5));
            p.altitude2 = toAltitude(it.arr(90, 5));
            p.transitionAltitude = toAltitude(it.arr(95, 5));
            p.speedLimit = it.arr(100, 3);
            p.verticalAngle = it.arr(103, 4);
            p.centerPointId = it.arr(107, 5);
            p.centerPointMultipleCode = it[112];
            p.centerPointIcaoCode = it.str(113, 2);
            p.centerPointSectionCode = it[115];
            p.centerPointSubsectionCode = it[116];
            p.gpsFmsIndicator = it[117];
            p.apchRouteQualifier1 = it[119];
            p.apchRouteQualifier2 = it[120];
            r.points.push_back(p);
        }
        catch (const QString &err)
        {
            qWarning() << QString("Unable to load SSA point %1 at %2").arg(err).arg(it.desctiption());
        }

        const QByteArray continuationMarker = it.arr(1, 38);
        ContinuantionNumber cn(it[39]);

        Q_FOREVER // processint continuation records
        {
            if (!it.next() || it.arr(1, 38) != continuationMarker || it[39] != cn.next())
                break;
        }
    }
}

Sid ArincParser::parseSid(ArincFilePrimitiveIterator &it)
{
    Sid r;
    parseSSA(r, it);
    return r;
}

Star ArincParser::parseStar(ArincFilePrimitiveIterator &it)
{
    Star r;
    parseSSA(r, it);
    return r;
}

Approach ArincParser::parseApproach(ArincFilePrimitiveIterator &it)
{
    Approach r;
    parseSSA(r, it);
    return r;
}

Runway ArincParser::parseRunway(ArincFilePrimitiveIterator &it)
{
    QString errorString;
    Runway r;
    r.file = it.fullFileName();
    r.line = it.line();
    try
    {
        bool bOk;
        r.areaCode = it.str(2, 3);
        r.airportIcaoId = it.str(7, 4);
        r.icaoCode = it.str(11, 2);
        r.id = it.str(14, 5);
        r.length = toInt(it.arr(23, 5), &bOk) * 0.3048;
        if (!bOk)
            throw QString("invalid length field");
        r.magneticBearing = toAzimuth(it.arr(28, 4));
        r.coord.set(toLongitude(it.arr(42, 10)), toLatitude(it.arr(33, 9)));
        r.gradient = it.arr(52, 5);
        r.landingThresholdElevation = it.arr(67, 5);
        r.displacedThresholdDistance = it.arr(72, 4);
        r.thresholdCrossingHeight = it.arr(76, 2);
        r.width = toInt(it.arr(78, 3), &bOk) * 0.3048;
        if (!bOk)
            throw QString("invalid width field");
        r.pathId = it.arr(82, 4);
        r.category = it[86];
        r.stopWay = it.arr(87, 4);
        r.pathId2 = it.arr(91, 4);
        r.category2 = it[95];
        r.description = it.str(102, 22);
    }
    catch (const QString &err)
    {
        errorString = QString("Unable to load Runway %1 at %2").arg(err).arg(it.desctiption());
    }

    const QByteArray continuationMarker = it.arr(1, 21);
    ContinuantionNumber cn(it[22]);

    Q_FOREVER // processint continuation records
    {
        if (!it.next() || it.arr(1, 21) != continuationMarker || it[22] != cn.next())
            break;
    }

    if (!errorString.isEmpty())
        throw errorString;

    return r;
}

bool ArincParser::parse(ArincFilePrimitiveIterator &it, ArincEmptyStorage &out)
{
    if (it.isEnd())
        return false;

    try
    {
        const char recordType = it[1];
        if (recordType == 'S') // Standart
        {
            const char sectionCode = it[5];
            if (sectionCode == Airspace::SectionCode) // Airspace
            {
                const char subsectionCode = it[6];
                if (subsectionCode == FirUir::SubsectionCode) //Fir/Uir
                    out.add(parseFirUir(it));
                else if (subsectionCode == ControlledAirspace::SubsectionCode) // Controlled Airspace
                    out.add(parseControlledAirspace(it));
                else if (subsectionCode == RestrictedAirspace::SubsectionCode) //Restricted Airspace
                    out.add(parseRestrictedAirspace(it));
                else
                    it.next();
            }
            else if (sectionCode == Navaid::SectionCode) // Navaid
            {
                const char subsectionCode = it[6];
                if (subsectionCode == VhfNavaid::SubsectionCode) // VHF Navaid
                    out.add(parseVhfNavaid(it));
                else if (subsectionCode == NdbNavaid::SubsectionCode) // NDB Navaid
                    out.add(parseNdbNavaid(it));
                else
                    it.next();
            }
            else if (sectionCode == Enroute::SectionCode) // Enroute
            {
                const char subsectionCode = it[6];
                if (subsectionCode == EnrouteWaypoint::SubsectionCode) // Waypoint
                    out.add(parseEnrouteWaypoint(it));
                else if (subsectionCode == Airway::SubsectionCode) // Airway and Routes
                    out.add(parseAirway(it));
                else if (subsectionCode == 'M') // Airway Marker
                    it.next();
                else if (subsectionCode == Holding::SubsectionCode) // Holding Pattern
                    out.add(parseHolding(it));
                else if (subsectionCode == 'T') // Referred Routes
                    it.next();
                else if (subsectionCode == 'U') // Airway Restrictions
                    it.next();
                else if (subsectionCode == 'V') // Communications
                    it.next();
                else
                    it.next();
            }
            else if (sectionCode == 'A') // MORA
                it.next();
            else if (sectionCode == 'H') // Heliport
                it.next();
            else if (sectionCode == Airport::SectionCode) // Airport
            {
                const char subsectionCode = it[13];
                if (subsectionCode == AirportRefPoint::SubsectionCode) // Airport Reference Point
                    out.add(parseAirportRefPoint(it));
                else if (subsectionCode == 'B') // Gates
                    it.next();
                else if (subsectionCode == TerminalWaypoint::SubsectionCode) // Terminal Waypoints
                    out.add(parseTerminalWaypoint(it));
                else if (subsectionCode == Sid::SubsectionCode) // SID
                    out.add(parseSid(it));
                else if (subsectionCode == Star::SubsectionCode) // STAR
                    out.add(parseStar(it));
                else if (subsectionCode == Approach::SubsectionCode) // Approach
                    out.add(parseApproach(it));
                else if (subsectionCode == Runway::SubsectionCode) // Runway
                    out.add(parseRunway(it));
                else if (subsectionCode == 'I') // Localizer/Glide Slope
                    it.next();
                else if (subsectionCode == 'L') // MLS
                    it.next();
                else if (subsectionCode == 'M') // Localizer Marker
                    it.next();
                else if (NdbTerminal::SubsectionCode == it[6]) // Terminal NDB
                    out.add(parseNdbTerminal(it));
                else if (subsectionCode == 'P') // Pathpoint
                    it.next();
                else if (subsectionCode == 'R') // Flt Planning ARR/DEP
                    it.next();
                else if (subsectionCode == 'S') // MSA
                    it.next();
                else if (subsectionCode == 'T') // GLS Station
                    it.next();
                else if (subsectionCode == 'V') // Communications
                    it.next();
                else
                    it.next();
            }
            else if (sectionCode == 'R') // Company Routes
                it.next();
            else if (sectionCode == 'T') // Tables
                it.next();
            else
                it.next();
        }
        else if (recordType == 'T') // Tailored
            it.next();
        else
            it.next();
    }
    catch (const QString &err)
    {
        qWarning() << err;
        return false;
    }
    return true;
}
