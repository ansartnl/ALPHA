#ifndef AIRWAY_H
#define AIRWAY_H

#include "Enroute.h"
#include <QtCore/QList>
#include <QtCore/QString>

struct Airway : public Enroute
{
    static const char SubsectionCode;
    inline virtual char subsectionCode() const { return SubsectionCode; }

    QString areaCode;
    QString id;

    struct Point
    {
        int sn;
        QString id;
        QString icaoCode1;
        char sectionCode;
        char subsectionCode;
        QByteArray descriptionCode;
        char boundaryCode;
        char routeType;
        char level;
        char directionRestriction;
        QByteArray cruiseTableIndicator;
        char euIndicator;
        QByteArray recomendedNavaid;
        QString icaoCode2;
        QByteArray rnp;
        QByteArray theta;
        QByteArray rho;
        QByteArray outboundMagneticCourse;
        QByteArray routeDistanceFrom;
        QByteArray inboundMagneticCourse;
        QByteArray minAltitude1;
        QByteArray minAltitude2;
        QByteArray maxAltitude;
    };

    QList<Point> points;
};

#endif // AIRWAY_H
