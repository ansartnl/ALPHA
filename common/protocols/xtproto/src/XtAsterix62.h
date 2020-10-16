#ifndef XT_ASTERIX62_DATA_H
#define XT_ASTERIX62_DATA_H

#include "xtproto_global.h"
#include "XtDefines.h"
#include "XtObject.h"
#include "XtCommon.h"
#include <QtGlobal>

XT_BEGIN_NAMESPACE

//! Asterix 62 data storage
struct XTPROTO_EXPORT CAsterix62 : public CObject
{
    virtual void Serialize(QDataStream& out) const;
    virtual void Deserialize(QDataStream& in)
    {
        CObject::Deserialize(in);
        in >> mLocation >> mTrackNumber >> mTrackMode3ACode
                >> mVelocity >> mRateOfClimbDescent;
    }

    QByteArray toArray() const;

    bool operator== (const CAsterix62& obj) const
    {
        return ( mLocation == obj.mLocation && mTrackNumber == obj.mTrackNumber &&
                 mTrackMode3ACode == obj.mTrackMode3ACode && mVelocity == obj.mVelocity &&
                 mRateOfClimbDescent == obj.mRateOfClimbDescent );
    }

    bool operator != (const CAsterix62& obj) const
    { return !operator ==(obj); }

    XT_OBJECT_TYPE_DECL

public:
    /** Location point */
    GeoPoint    mLocation;

    /** Identifier of airplane */
    quint16     mTrackNumber;
    float       mTrackMode3ACode;

    /** Velocity in meter/sec */
    Velocity    mVelocity;

    qreal       mRateOfClimbDescent;
};

XT_END_NAMESPACE

QDataStream &operator<<(QDataStream &out, const Xt::CAsterix62 &asterix);

#endif // XT_ASTERIX62_DATA_H
