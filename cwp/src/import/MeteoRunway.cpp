#include "MeteoRunway.h"


// Identifier initialization
const int MeteoRunway::ControlTime = 221;
const int MeteoRunway::MagneticWind = 222;
const int MeteoRunway::AverageSpeed = 223;
const int MeteoRunway::MaxSpeed = 224;
const int MeteoRunway::SideWind = 225;
const int MeteoRunway::LongitudialWind = 226;
const int MeteoRunway::SightDistance = 227;
const int MeteoRunway::SDistanceStart = 228;
const int MeteoRunway::SDistanceMiddle = 229;
const int MeteoRunway::SDistanceEnd = 230;
const int MeteoRunway::LowerCloudiness = 231;
const int MeteoRunway::Pressure = 232;
const int MeteoRunway::PressureSea = 233;
const int MeteoRunway::Temperature = 234;
const int MeteoRunway::CondensationPoint = 235;


bool MeteoRunway::operator == (const MeteoRunway &v) const
{
    if ( mRunwayName != v.mRunwayName || mAirportName != v.mAirportName || mValues.size() != v.mValues.size() )
        return false;

    bool isEqual = true;

    // Test map values.
    for ( TValueMap::const_iterator itv = mValues.constBegin(); itv != mValues.constEnd(); ++itv )
    {
        int key = itv.key();
        QVariant valueThis = *itv;
        QVariant valueTest = v.value(key);

        if ( valueThis.canConvert<double>() )
            isEqual = qFuzzyCompare(valueThis.toDouble(), valueTest.toDouble());
        else
            isEqual = valueThis == valueTest;

        if ( !isEqual )
            break;
    }

    return isEqual;
}

bool MeteoRunway::operator != (const MeteoRunway &v) const
{
    return !operator == (v);
}

bool MeteoRunway::contains(int key) const
{
    return mValues.contains(key);
}

QVariant MeteoRunway::value(int key, const QVariant &defValue) const
{
    if ( mValues.contains(key) )
        return mValues[key];
    return defValue;
}

void MeteoRunway::insert(int key, const QVariant &v)
{
    mValues[key] = v;
}

void MeteoRunway::remove(int key)
{
    mValues.remove(key);
}

QDataStream &operator<<(QDataStream &out, const MeteoRunway &value)
{
    out << value.GetAirportName();
    out << value.GetRunwayName();
    out << value.GetValues();
    return out;
}

QDataStream &operator>>(QDataStream &in, MeteoRunway &value)
{
    QString sVal;
    in >> sVal;
    value.SetAirportName(sVal);
    in >> sVal;
    value.SetRunwayName(sVal);
    MeteoRunway::TValueMap mVal;
    in >> mVal;
    value.SetValues(mVal);
    return in;
}
