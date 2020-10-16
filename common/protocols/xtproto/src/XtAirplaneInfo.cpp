#include "XtAirplaneInfo.h"
#include <QtCore/QDataStream>

XT_BEGIN_NAMESPACE

QByteArray AirplaneInfo::toArray() const
{
    QByteArray result;
    QDataStream stream(&result, QIODevice::WriteOnly);
    Serialize(stream);
    return result;
}

void AirplaneInfo::Serialize(QDataStream& out) const
{
    CObject::Serialize(out);
    out << mNumber << mId << mFlags;
    if ( mFlags & ICAO )
        out << mICAO;
    if ( mFlags & CallSign )
        out << mCallSign;
    if ( mFlags & StateName )
        out << mStateName;
    if ( mFlags & RemainingFuel )
        out << mRemainingFuel;
    if ( mFlags & Alarm )
        out << (quint8)mAlarmFlag;
}

void AirplaneInfo::Deserialize(QDataStream& in)
{
    CObject::Deserialize(in);
    in >> mNumber >> mId >> mFlags;
    if ( mFlags & ICAO )
        in >> mICAO;
    if ( mFlags & CallSign )
        in >> mCallSign;
    if ( mFlags & StateName )
        in >> mStateName;
    if ( mFlags & RemainingFuel )
        in >> mRemainingFuel;
    if ( mFlags & Alarm )
    {
        quint8 alarmFlag;
        in >> alarmFlag;
        mAlarmFlag = AlarmFlag(alarmFlag);
    }
}

XT_END_NAMESPACE
