#include "XtVehicle.h"

Xt::Vehicle::Vehicle()
    : mId(0), mName(), mLatitude(0), mLongitude(0), mVelocity(0), mDelete(false), mHidden(false)
{
}

void Xt::Vehicle::Serialize(QDataStream &out) const
{
    CObject::Serialize(out);
    out << mId << mName << mLatitude << mLongitude
        << mVelocity << mDelete << mHidden;
}

void Xt::Vehicle::Deserialize(QDataStream &in)
{
    CObject::Deserialize(in);
    in >> mId >> mName >> mLatitude >> mLongitude
       >> mVelocity >> mDelete >> mHidden;
}

QDataStream &operator<<(QDataStream &out, const Xt::Vehicle &vehicle)
{
    vehicle.Serialize(out);
    return out;
}

QByteArray Xt::Vehicle::toArray() const
{
    QByteArray result;
    QDataStream stream(&result, QIODevice::WriteOnly);
    stream << (*this);
    return result;
}

bool Xt::Vehicle::operator ==(const Xt::Vehicle &obj) const
{
    return ( mId == obj.mId &&
             mName == obj.mName &&
             mLatitude == obj.mLatitude &&
             mLongitude == obj.mLongitude &&
             mVelocity == obj.mVelocity &&
             mDelete == obj.mDelete &&
             mHidden == obj.mHidden);
}
