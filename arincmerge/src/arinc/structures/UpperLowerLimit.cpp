#include "UpperLowerLimit.h"

UpperLowerLimit UpperLowerLimit::fromArray(const QByteArray& arr)
{
    UpperLowerLimit result;
    bool bOk(false);
    if ("FL" == arr.left(2))
    {
        result.value = arr.mid(2).toUInt(&bOk);
        if (!bOk)
            return UpperLowerLimit();
        result.value *= 30.48;
        result.type = UpperLowerLimit::AlphaNumeric;
    }
    else if ("NOTSP" == arr.left(5))
    {
        result.type = UpperLowerLimit::NotSpecified;
    }
    else if ("UNLTD" == arr.left(5))
    {
        result.type = UpperLowerLimit::Unlimited;
    }
    else if ("GND" == arr.left(3))
    {
        result.type = UpperLowerLimit::Ground;
    }
    else if ("MSL" == arr.left(3))
    {
        result.type = UpperLowerLimit::MeanSeaLevel;
    }
    else if ("NOTAM" == arr.left(5))
    {
        result.type = UpperLowerLimit::Notam;
    }
    else
    {
        result.value = arr.toUInt(&bOk);
        if(!bOk)
            return UpperLowerLimit();
        result.value *= 0.3048;
        result.type = UpperLowerLimit::AllNumeric;
    }
    return result;
}
