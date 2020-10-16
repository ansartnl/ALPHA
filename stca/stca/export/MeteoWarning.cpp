#include "MeteoWarning.h"

static int iMeteoWarning = qRegisterMetaTypeStreamOperators<MeteoWarning>();

MeteoWarning::MeteoWarning(int bn, ZoneType t, quint32 ID, Position pos)
    : boardNumber(bn), type(t), id(ID), position(pos)
{}

QDataStream &operator<<(QDataStream & stream, const MeteoWarning &mw)
{
    stream << (const StcaObject &)mw;
    stream << mw.boardNumber << (int)mw.type << mw.id << (int)mw.position;
    return stream;
}

QDataStream &operator>>(QDataStream & stream, MeteoWarning &mw)
{
    int type, pos;
    stream >> (StcaObject &)mw;
    stream >> mw.boardNumber >> type >> mw.id >> pos;
    mw.type = MeteoWarning::ZoneType(type);
    mw.position = MeteoWarning::Position(pos);
    return stream;
}
