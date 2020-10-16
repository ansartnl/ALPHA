#ifndef CONTROLLEDAIRSPACE_H
#define CONTROLLEDAIRSPACE_H

#include "Airspace.h"
#include "BoundaryPoint.h"
#include "UpperLowerLimit.h"
#include "ArincGlobal.h"
#include <QtCore/QString>
#include <QtCore/QList>

struct ControlledAirspace : public Airspace
{
    static const char SubsectionCode;
    inline virtual char subsectionCode() const { return SubsectionCode; }

    QString areaCode;
    QString icaoCode;
    enum Type
    {
        ClassC = 'A',
        ControlArea = 'C',
        TerminalControlArea = 'M',
        RadarArea = 'R',
        ClassB = 'T',
        ClassD = 'Z'
    } type;
    QString center;
    char centerSectionCode;
    char centerSubsectionCode;
    char classification;
    char multipleCode;
    ArincGlobal::Level level;
    char timeCode;
    char notam;
    QByteArray rnp;
    UpperLowerLimit lowerLimit;
    ArincGlobal::UnitIndicator lowerUi;
    UpperLowerLimit upperLimit;
    ArincGlobal::UnitIndicator upperUi;
    QString name;
    QList<BoundaryPoint> boundary;

    inline virtual QString toString() const { return QString("ControlledAirspace: %1; Icao: %2; %3").arg(name).arg(icaoCode).arg(ArincBaseStruct::toString()); }
};

#endif // CONTROLLEDAIRSPACE_H
