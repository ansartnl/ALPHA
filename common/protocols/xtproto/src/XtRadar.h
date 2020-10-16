#ifndef XT_RADAR_DATA_H
#define XT_RADAR_DATA_H

#include "xtproto_global.h"
#include "XtDefines.h"
#include "XtObject.h"
#include <QtGlobal>

XT_BEGIN_NAMESPACE

//! Radar data storage
struct XTPROTO_EXPORT Radar : public CObject
{
    enum ScanType { North = 1, SectorCrossing, South };

    virtual void Serialize(QDataStream& out) const
    {
        CObject::Serialize(out);
        out << mId << int(mScanType) << mAngle;
    }
    virtual void Deserialize(QDataStream& in)
    {
        CObject::Deserialize(in);
        int scanType;
        in >> mId >> scanType >> mAngle;
        mScanType = ScanType(scanType);
    }

    QByteArray toArray() const;

    bool operator == (const Radar& obj) const
    { return ( mId == obj.mId && qFuzzyCompare(mAngle+1.0, obj.mAngle+1.0) ); }

    bool operator != (const Radar& obj) const
    { return !operator ==(obj); }

    XT_OBJECT_TYPE_DECL

public:
    /** Идентификатор */
    quint16     mId;
    /** Scanning type */
    ScanType    mScanType;
    /** Угол развертки относительно направления на север */
    qreal       mAngle;
};

XT_END_NAMESPACE

QDataStream &operator<<(QDataStream &out, const Xt::Radar &radar);

#endif // XT_RADAR_DATA_H
