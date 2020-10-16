#ifndef XT_PELENG_DATA_H
#define XT_PELENG_DATA_H

#include "xtproto_global.h"
#include "XtDefines.h"
#include "XtObject.h"
#include "XtCommon.h"
#include <QtGlobal>

XT_BEGIN_NAMESPACE

//! Peleng data storage
struct XTPROTO_EXPORT Peleng : public CObject
{
    virtual void Serialize(QDataStream& out) const
    {
        CObject::Serialize(out);
        out << mId << mLocation << mAngle << mChannelNumber;
    }
    virtual void Deserialize(QDataStream& in)
    {
        CObject::Deserialize(in);
        in >> mId >> mLocation >> mAngle >> mChannelNumber;
    }

    QByteArray toArray() const;

    bool operator == (const Peleng& obj) const
    {
        return ( mId == obj.mId && mLocation == obj.mLocation &&
                 qFuzzyCompare(mAngle+1.0, obj.mAngle+1.0) &&
                 mChannelNumber == obj.mChannelNumber );
    }

    bool operator != (const Peleng& obj) const
    { return !operator ==(obj); }

    XT_OBJECT_TYPE_DECL

public:
    /** Идентификатор */
    quint16     mId;
    /** Точка стояния пеленгатора */
    Coord       mLocation;
    /** Угол пеленгации относительно направления на север */
    qreal       mAngle;
    /** Номер канала */
    qint8       mChannelNumber;
};

XT_END_NAMESPACE

QDataStream &operator<<(QDataStream &out, const Xt::Peleng &peleng);

#endif // XT_PELENG_DATA_H
