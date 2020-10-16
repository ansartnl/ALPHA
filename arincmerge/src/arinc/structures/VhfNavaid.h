#ifndef VHFNAVAID_H
#define VHFNAVAID_H

#include "Navaid.h"
#include "GeoPoint.h"
#include <QtCore/QStringList>

struct VhfNavaid : public Navaid
{
    static const char SubsectionCode;
    inline virtual char subsectionCode() const { return SubsectionCode; }

    QString areaCode;
    QString airportIcaoId;
    QString icaoCode1;
    QString vorId;
    QString icaoCode2;
    int vorFrequency;
    QByteArray navaidClass;
    GeoPoint vorCoord;
    QString dmeId;
    GeoPoint dmeCoord;
    QByteArray stationDeclination;
    int dmeElevation;
    enum FigureOfMerit
    {
        TerminalUse = '0',
        LowAltitudeUse = '1',
        HighAltitudeUse = '2',
        ExtendedHighAltitudeUse = '3',
        OutOfService = '9'
    } figureOfMerit;
    int bias;
    int frequencyProtection;
    QString datumCode;
    QString name;
};

#endif // VHFNAVAID_H
