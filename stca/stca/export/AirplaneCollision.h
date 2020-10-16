#ifndef AIRPLANECOLLISION_H
#define AIRPLANECOLLISION_H

#include "StcaObject.h"

#include <QtCore/QDataStream>
#include <QtCore/QVariant>

struct AirplaneCollision : public StcaObject
{
    int id1;
    int id2;
    int RVSM;
    enum Type
    {
        Crash,
        FollowingWarning,
        PerpendicularWarning,
        CounterWarning,
        NoCollision = USHRT_MAX
    } type;
    AirplaneCollision(int id1 = 0, int id2 = 0, int RVSM = 0, Type type = Crash);
};

QDataStream &operator<<(QDataStream &, const AirplaneCollision &);
QDataStream &operator>>(QDataStream &, AirplaneCollision &);

Q_DECLARE_METATYPE(AirplaneCollision)
Q_DECLARE_METATYPE(AirplaneCollision::Type)

#endif // AIRPLANECOLLISION_H
