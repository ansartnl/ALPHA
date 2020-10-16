#ifndef XTVEHICLE_H
#define XTVEHICLE_H

#include "xtproto_global.h"
#include "XtDefines.h"
#include "XtObject.h"
#include "XtCommon.h"
#include <QtGlobal>

XT_BEGIN_NAMESPACE

struct XTPROTO_EXPORT Vehicle : public CObject
{
    Vehicle();

    virtual void Serialize(QDataStream& out) const;
    virtual void Deserialize(QDataStream& in);

    QByteArray toArray() const;

    bool operator == (const Vehicle& obj) const;
    bool operator != (const Vehicle& obj) const
    { return !operator ==(obj); }

    XT_OBJECT_TYPE_DECL

public:
    /** Идентификатор */
    qint16  mId;
    /** Имя */
    QString mName;
    /** Широта */
    double  mLatitude;
    /** Долгота */
    double  mLongitude;
    /** Скорость */
    double  mVelocity;
    /** Флаг удаления */
    bool mDelete;
    /** Флаг скрытия */
    bool mHidden;
};

XT_END_NAMESPACE

QDataStream &operator<<(QDataStream &out, const Xt::Vehicle &vehicle);

#endif // XTVEHICLE_H
