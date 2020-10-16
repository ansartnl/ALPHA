#ifndef RCGRID_VALUES_H
#define RCGRID_VALUES_H

#include <QtCore/QDataStream>
#include "Coordinate.h"

struct RcGridValues
{
    RcGridValues();

    void initDefault();

    CCoord  mCenter;
    int     mRadius;
    int     mStep;
    int     mSector;
};

QDataStream &operator<<(QDataStream &out, const RcGridValues &value);
QDataStream &operator>>(QDataStream &in, RcGridValues &value);

#endif // RCGRID_VALUES_H
