#ifndef RESTRICTEDAIRSPACE_H
#define RESTRICTEDAIRSPACE_H

#include "Airspace.h"
#include "BoundaryPoint.h"
#include "UpperLowerLimit.h"
#include "ArincGlobal.h"
#include <QtCore/QString>
#include <QtCore/QList>

struct RestrictedAirspace : public Airspace
{
    static const char SubsectionCode;
    inline virtual char subsectionCode() const { return SubsectionCode; }

    QString areaCode;
    QString icaoCode;
    enum Type
    {
        UnspecifiedOrUnknown = 'U',
        Alert = 'A',
        Caution = 'C',
        Danger = 'D',
        MilitaryOperationsArea = 'M',
        Prohibited = 'P',
        Restricted = 'R',
        Training = 'T',
        Warning = 'W'
    } type;
    QString designation;
    char multipleCode;
    ArincGlobal::Level level;
    char timeCode;
    char notam;
    UpperLowerLimit lowerLimit;
    ArincGlobal::UnitIndicator lowerUi;
    UpperLowerLimit upperLimit;
    ArincGlobal::UnitIndicator upperUi;
    QString name;
    QList<BoundaryPoint> boundary;
};

#endif // RESTRICTEDAIRSPACE_H
