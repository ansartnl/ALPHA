#ifndef NDB_H
#define NDB_H

#include "GeoPoint.h"

struct Ndb
{
    QString areaCode;
    QString airportIcaoId;
    QString icaoCode1;
    QString ndbId;
    QString icaoCode2;
    int ndbFrequency;
    QByteArray ndbClass;
    GeoPoint ndbCoord;
    QByteArray magneticVariation;
    QString datumCode;
    QString name;
};

#endif // NDB_H
