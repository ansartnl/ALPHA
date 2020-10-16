#ifndef METARSPECIPARSER_H
#define METARSPECIPARSER_H

#include "MetarSpeci.h"

#include <QtCore>

// parse the array of bytes to metar struct
class KRAMS_EXPORT MetarSpeciParser
{
public:
    MetarSpeci::Error lastError;
    int begError;
    int endError;

    MetarSpeci *doit(const QString &data);

private:
    void parseType();
    void parseCor();
    void parseAirport();
    void parseTime();
    void parseNil();
    void parseAuto();
    void parseWind();
    void parseCavok();
    void parseVisibility();
    void parseRunwayVisibility();
    void parseWeatherEvents();
    void parseCloudiness();
    void parseTemperature();
    void parsePressure();
    void parseRecentWeather();
    void parseWindShear();
    void parseSeaState();
    void parseRunwayState();
    void parseThend();
    void parseRmk();

    void setErrorPos();

    QStringList data;
    int curIndex;
    QScopedPointer<MetarSpeci> result;
};

#endif // METARSPECIPARSER_H
