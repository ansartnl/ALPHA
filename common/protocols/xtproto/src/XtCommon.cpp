#include "XtCommon.h"

XT_BEGIN_NAMESPACE

bool Coord::operator == (const Coord& crd) const
{
    return ( mType == crd.mType
             && qFuzzyCompare(data.mGeo.latitude+1.0, crd.data.mGeo.latitude+1.0)
             && qFuzzyCompare(data.mGeo.longitude+1.0, crd.data.mGeo.longitude+1.0)
             && qFuzzyCompare(mHeight+1.0, crd.mHeight+1.0) );
}

bool GeoPoint::operator == (const GeoPoint &point) const
{
    return ( qFuzzyCompare(mLatitude+1.0, point.mLatitude+1.0) &&
             qFuzzyCompare(mLongitude+1.0, point.mLongitude+1.0) &&
             qFuzzyCompare(mHeight+1.0, point.mHeight+1.0) );
}

bool Velocity::operator == (const Velocity &vel) const
{
    return ( qFuzzyCompare(mX+1.0, vel.mX+1.0) &&
             qFuzzyCompare(mY+1.0, vel.mY+1.0) &&
             qFuzzyCompare(mZ+1.0, vel.mZ+1.0) );
}

QDataStream &operator<<(QDataStream &in, const Coord &crd)
{
    in << quint8(crd.mType) << crd.data.mGeo.latitude << crd.data.mGeo.longitude
       << crd.mHeight;
    return in;
}

QDataStream &operator>>(QDataStream &out, Coord &crd)
{
    quint8 type;
    out >> type >> crd.data.mGeo.latitude >> crd.data.mGeo.longitude
        >> crd.mHeight;
    crd.mType = Coord::Type(type);
    return out;
}

QDataStream &operator<<(QDataStream &in, const GeoPoint &point)
{
    in << point.mLatitude << point.mLongitude << point.mHeight;
    return in;
}

QDataStream &operator>>(QDataStream &out, GeoPoint &point)
{
    out >> point.mLatitude >> point.mLongitude >> point.mHeight;
    return out;
}

QDataStream &operator<<(QDataStream &in, const Velocity &vel)
{
    in << vel.mX << vel.mY << vel.mZ;
    return in;
}

QDataStream &operator>>(QDataStream &out, Velocity &vel)
{
    out >> vel.mX >> vel.mY >> vel.mZ;
    return out;
}

XT_END_NAMESPACE
