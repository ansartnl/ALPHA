#ifndef SSA_H
#define SSA_H

#include "Airport.h"
#include "ArincGlobal.h"
#include <QtCore/QList>
#include <QtCore/QString>

struct SSA : public Airport
{
    QString areaCode;
    QString airportId;
    QString icaoCode;
    QString id;
    char routeType;
    QString transitionId;

    struct Point
    {
        int sn;
        QString id;
        QString icaoCode;
        char sectionCode;
        char subsrectionCode;
        QByteArray descriptionCode;
        ArincGlobal::TurnDirection turn;
        QByteArray rnp;
        QByteArray path;
        char turnDirectionValid;
        QByteArray recomendedNavaid;
        QString icaoCode2;
        QByteArray arcRadius;
        QByteArray thera;
        QByteArray rho;
        double course;
        QByteArray distance;
        char recdNavSectOn;
        char rectNavSubsection;
        char altitudeDescription;
        char atcIndicator;
        double altitude1;
        double altitude2;
        double transitionAltitude;
        QByteArray speedLimit;
        QByteArray verticalAngle;
        QByteArray centerPointId;
        char centerPointMultipleCode;
        QString centerPointIcaoCode;
        char centerPointSectionCode;
        char centerPointSubsectionCode;
        char gpsFmsIndicator;
        char apchRouteQualifier1;
        char apchRouteQualifier2;
    };

    QList<Point> points;
};

struct Sid : public SSA
{
    static const char SubsectionCode;
    inline virtual char subsectionCode() const { return SubsectionCode; }
};

struct Star : public SSA
{
    static const char SubsectionCode;
    inline virtual char subsectionCode() const { return SubsectionCode; }
};

struct Approach : public SSA
{
    static const char SubsectionCode;
    inline virtual char subsectionCode() const { return SubsectionCode; }
};

#endif // SSA_H
