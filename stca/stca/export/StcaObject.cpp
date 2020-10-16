#include "StcaObject.h"


QDataStream &operator<<(QDataStream & stream, const StcaObject &o)
{
    return stream << o.identifier;
}

QDataStream &operator>>(QDataStream & stream, StcaObject &o)
{
    stream >> o.identifier;
    return stream;
}
