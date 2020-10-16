#include "AirplaneCollision.h"

static int iAirplaneCollision = qRegisterMetaTypeStreamOperators<AirplaneCollision>();
static int type = qRegisterMetaType<AirplaneCollision::Type>();

AirplaneCollision::AirplaneCollision(int id1, int id2, int RVSM, Type type) : id1(id1), id2(id2), RVSM(RVSM), type(type)
{}

QDataStream &operator<<(QDataStream & stream, const AirplaneCollision &collision)
{
    stream << (const StcaObject&)collision;
    stream << collision.id1 << collision.id2 << collision.RVSM << (qint32)collision.type;
    return stream;
}

QDataStream &operator>>(QDataStream & stream, AirplaneCollision &collision)
{
    stream >> (StcaObject&)collision;
    qint32 type;
    stream >> collision.id1 >> collision.id2 >> collision.RVSM >> type;
    collision.type = AirplaneCollision::Type(type);
    return stream;
}
