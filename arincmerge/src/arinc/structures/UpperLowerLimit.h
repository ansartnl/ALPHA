#ifndef UPPERLOWERLIMIT_H
#define UPPERLOWERLIMIT_H

#include <QtCore/QByteArray>

struct UpperLowerLimit
{
    UpperLowerLimit() : type(Invalid), value(0.0) {}
    enum Type
    {
        Invalid,
        AllNumeric,
        AlphaNumeric,
        NotSpecified,
        Unlimited,
        Ground,
        MeanSeaLevel,
        Notam
    } type;
    double value;
    bool isValid() const { return type != Invalid; }
    static UpperLowerLimit fromArray(const QByteArray& arr);
};

#endif // UPPERLOWERLIMIT_H
