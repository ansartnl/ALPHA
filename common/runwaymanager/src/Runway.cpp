#include "Runway.h"

static int i0 = qRegisterMetaTypeStreamOperators < Runway >();

QDataStream &operator<<(QDataStream &out, const Runway &runway)
{
    out << runway.numPeople << runway.numAuto << runway.numOther;
    return out;
}

QDataStream &operator>>(QDataStream &in, Runway &runway)
{
    in >> runway.numPeople >> runway.numAuto >> runway.numOther;
    return in;
}
