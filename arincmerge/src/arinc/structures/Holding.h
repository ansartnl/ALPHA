#ifndef HOLDING_H
#define HOLDING_H

#include "Enroute.h"
#include "ArincGlobal.h"
#include <QtCore/QList>
#include <QtCore/QString>

struct Holding : public Enroute
{
    static const char SubsectionCode;
    inline virtual char subsectionCode() const { return SubsectionCode; }

    QString areaCode;
    QString regionCode;
    QString icaoCode;
    enum Type
    {
        Undefined = '0',
        HighAltitude = '1',
        LowAltitude = '2',
        SID = '3',
        STAR = '4',
        Approach = '5',
        MissedApproach = '6',
        Other = '7'
    } type;
    char duplicate;
    QString pointId;
    QString pointIcaoCode;
    char pointSectionCode;
    char pointSubsectionCode;
    double inboundHoldingCourse;
    ArincGlobal::TurnDirection turn;
    double legLength;
    int legTime;
    QByteArray minAltitude;
    QByteArray maxAltitude;
    double speed;
    QString note;
};

#endif // HOLDING_H
