#ifndef FIRUIR_H
#define FIRUIR_H

#include "Airspace.h"
#include "BoundaryPointFirUir.h"
#include "UpperLowerLimit.h"

struct FirUir : public Airspace
{
    static const char SubsectionCode;
    inline virtual char subsectionCode() const { return SubsectionCode; }

    QString areaCode;
    QString id;
    QString address;

    enum Type
    {
        Fir = 'F',
        Uir = 'U',
        Both = 'B'
    } type;

    enum ReportingUnitsSpeed
    {
        TasNotSpecified = '0',
        TasInKnots = '1',
        TasInMach = '2',
        TasInKilometersPerHr = '3'
    } rus;

    enum ReportingUnitsAltitude
    {
        AltNotSpecified = '0',
        AltInFlightLevel = '1',
        AltInMeters = '2',
        AltInFeet = '3'
    } rua;

    enum EntryReport
    {
        Required = 'Y',
        NoRequired = 'N'
    } er;

    UpperLowerLimit firUpperLimit;
    UpperLowerLimit uirLowerLimit;
    UpperLowerLimit uirUpperLimit;

    QByteArray cruiseTableIdentifier;

    QString name;

    QList<BoundaryPointFirUir> boundary;

    inline virtual QString toString() const { return QString("Fir/Uir: %1-%2; %3").arg(id).arg(name).arg(ArincBaseStruct::toString()); }
};

#endif // FIRUIR_H
