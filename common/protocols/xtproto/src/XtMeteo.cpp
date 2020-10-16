#include "XtMeteo.h"


///////////////////////////////////////////////////////////////////////////////
// MeteoVector implementation.

QDataStream &operator<<(QDataStream &out, const Xt::MeteoVector &meteoVector)
{
    meteoVector.Serialize(out);
    return out;
}

QByteArray Xt::MeteoVector::toArray() const
{
    QByteArray result;
    QDataStream stream(&result, QIODevice::WriteOnly);
    stream << (*this);
    return result;
}

///////////////////////////////////////////////////////////////////////////////
// MeteoContour implementation.

QDataStream &operator<<(QDataStream &out, const Xt::MeteoContour &meteoContour)
{
    meteoContour.Serialize(out);
    return out;
}

QByteArray Xt::MeteoContour::toArray() const
{
    QByteArray result;
    QDataStream stream(&result, QIODevice::WriteOnly);
    stream << (*this);
    return result;
}

///////////////////////////////////////////////////////////////////////////////
// MeteoInfo implementation.

XT_BEGIN_NAMESPACE

const quint8 MeteoInfo::WindCourse_ID = 1;
const quint8 MeteoInfo::WindAverage_ID = 2;
const quint8 MeteoInfo::WindMax_ID = 3;
const quint8 MeteoInfo::WindSide_ID = 4;
const quint8 MeteoInfo::WindLong_ID = 5;
const quint8 MeteoInfo::Temperature_ID = 6;
const quint8 MeteoInfo::DewPoint_ID = 7;
const quint8 MeteoInfo::Vis_ID = 8;
const quint8 MeteoInfo::DistVisBegin_ID = 9;
const quint8 MeteoInfo::DistVisMiddle_ID = 10;
const quint8 MeteoInfo::DistVisEnd_ID = 11;
const quint8 MeteoInfo::HeightCloud_ID = 12;
const quint8 MeteoInfo::QFE_ID = 13;
const quint8 MeteoInfo::QFEh_ID = 14;
const quint8 MeteoInfo::QNHh_ID = 15;

XT_END_NAMESPACE

QDataStream &operator<<(QDataStream &out, const Xt::MeteoInfo &meteoInfo)
{
    meteoInfo.Serialize(out);
    return out;
}

QByteArray Xt::MeteoInfo::toArray() const
{
    QByteArray result;
    QDataStream stream(&result, QIODevice::WriteOnly);
    stream << (*this);
    return result;
}
