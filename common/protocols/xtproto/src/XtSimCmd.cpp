#include "XtSimCmd.h"

QDataStream &operator<<(QDataStream &out, const Xt::SimCmd &simcmd)
{
    simcmd.Serialize(out);
    return out;
}

QByteArray Xt::SimCmd::toArray() const
{
    QByteArray result;
    QDataStream stream(&result, QIODevice::WriteOnly);
    stream << (*this);
    return result;
}
