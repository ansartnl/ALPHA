#include "XtAsterix62.h"

QDataStream &operator<<(QDataStream &out, const Xt::CAsterix62 &asterix)
{
    asterix.Serialize(out);
    return out;
}

QByteArray Xt::CAsterix62::toArray() const
{
    QByteArray result;
    QDataStream stream(&result, QIODevice::WriteOnly);
    stream << (*this);
    return result;
}

void Xt::CAsterix62::Serialize(QDataStream& out) const
{
    CObject::Serialize(out);
    out << mLocation << mTrackNumber << mTrackMode3ACode
            << mVelocity << mRateOfClimbDescent;
}
