#include "XtRadar.h"

QDataStream &operator<<(QDataStream &out, const Xt::Radar &radar)
{
    radar.Serialize(out);
    return out;
}

QByteArray Xt::Radar::toArray() const
{
    QByteArray result;
    QDataStream stream(&result, QIODevice::WriteOnly);
    stream << (*this);
    return result;
}
