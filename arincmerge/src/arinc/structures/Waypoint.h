#ifndef WAYPOINT_H
#define WAYPOINT_H

#include "GeoPoint.h"
#include <QtCore/QString>

struct Waypoint
{
    QString areaCode;
    QString regionCode;
    QString icaoCode1;
    QString id;
    QString icaoCode2;
    QByteArray type;
    QByteArray usage;
    GeoPoint coord;
    QByteArray magneticVariation;
    QString datumCode;
    QByteArray nameFormat;
    QString name;
};

#endif // WAYPOINT_H
