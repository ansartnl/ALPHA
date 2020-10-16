#include "XtStand.h"

Xt::Stand::Stand()
    : mId(0), mStandId(0), mAirportId(0), mFplId(0), mStateId(0)
{

}

QDataStream &operator<<(QDataStream &out, const Xt::Stand &stand)
{
    stand.Serialize(out);
    return out;
}

QByteArray Xt::Stand::toArray() const
{
    QByteArray result;
    QDataStream stream(&result, QIODevice::WriteOnly);
    stream << (*this);
    return result;
}
