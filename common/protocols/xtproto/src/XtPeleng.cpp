#include "XtPeleng.h"

QDataStream &operator<<(QDataStream &out, const Xt::Peleng &peleng)
{
    peleng.Serialize(out);
    return out;
}

QByteArray Xt::Peleng::toArray() const
{
    QByteArray result;
    QDataStream stream(&result, QIODevice::WriteOnly);
    stream << (*this);
    return result;
}
