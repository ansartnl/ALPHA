#include "XtPlotTrack.h"

QDataStream &operator<<(QDataStream &out, const Xt::Track &track)
{
    track.Serialize(out);
    return out;
}

void Xt::Track::Serialize(QDataStream& out) const
{
    CObject::Serialize(out);
    out << mNumber << mBoardNumber << mRlsId << mType << mLocation << mVelocity;
}

QByteArray Xt::Track::toArray() const
{
    QByteArray result;
    QDataStream stream(&result, QIODevice::WriteOnly);
    stream << (*this);
    return result;
}

QDataStream &operator<<(QDataStream &out, const Xt::Plot &plot)
{
    plot.Serialize(out);
    return out;
}

void Xt::Plot::Serialize(QDataStream& out) const
{
    CObject::Serialize(out);
    out << mNumber << mBoardNumber << mRlsId << mType << mLocation;
}

QByteArray Xt::Plot::toArray() const
{
    QByteArray result;
    QDataStream stream(&result, QIODevice::WriteOnly);
    stream << (*this);
    return result;
}
