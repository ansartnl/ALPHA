#include "RestrictionWarning.h"

static int iRestrictionWarning = qRegisterMetaTypeStreamOperators<RestrictionWarning>();

RestrictionWarning::RestrictionWarning(int bn, ZoneType t, quint32 ID, Position pos)
    : boardNumber(bn), type(t), id(ID), position(pos)
{}

QDataStream &operator<<(QDataStream & stream, const RestrictionWarning &rw)
{
    stream << (const StcaObject &)rw;
    stream << rw.boardNumber << (int)rw.type << rw.id << (int)rw.position;
    return stream;
}

QDataStream &operator>>(QDataStream & stream, RestrictionWarning &rw)
{
    int type, pos;
    stream >> (StcaObject &)rw;
    stream >> rw.boardNumber >> type >> rw.id >> pos;
    rw.type = RestrictionWarning::ZoneType(type);
    rw.position = RestrictionWarning::Position(pos);
    return stream;
}
