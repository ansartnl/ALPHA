#include "RcGridValues.h"

RcGridValues::RcGridValues()
    : mRadius(0), mStep(0), mSector(0)
{

}

void RcGridValues::initDefault()
{
    mCenter = CCoord();
    mRadius = 400000;
    mStep = 50000;
    mSector = 30;
}

QDataStream &operator<<(QDataStream &out, const RcGridValues &value)
{
    out << value.mCenter;
    out << value.mRadius;
    out << value.mStep;
    out << value.mSector;
    return out;
}

QDataStream &operator>>(QDataStream &in, RcGridValues &value)
{
    in >> value.mCenter;
    in >> value.mRadius;
    in >> value.mStep;
    in >> value.mSector;
    return in;
}
