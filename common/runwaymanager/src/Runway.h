#ifndef RUNWAY_H
#define RUNWAY_H

#include <QVariant>
#include <QDataStream>

struct Runway{
    Runway():numPeople(0), numAuto(0), numOther(0)
    {}

    int numPeople;
    int numAuto;
    int numOther;

    inline bool isDirty()
    {return numPeople != 0 || numAuto !=0 || numOther != 0;}
};

Q_DECLARE_METATYPE(Runway)

QDataStream &operator<<(QDataStream &out, const Runway &runway);
QDataStream &operator>>(QDataStream &in, Runway &runway);

#endif // RUNWAY_H
