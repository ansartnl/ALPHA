#ifndef XT_STAND_DATA_H
#define XT_STAND_DATA_H

#include "xtproto_global.h"
#include "XtDefines.h"
#include "XtObject.h"
#include "XtCommon.h"
#include <QtGlobal>

XT_BEGIN_NAMESPACE

//! Stand data storage
struct XTPROTO_EXPORT Stand : public CObject
{
    Stand();

    virtual void Serialize(QDataStream& out) const
    {
        CObject::Serialize(out);
        out << mId << mStandId << mStandName
            << mAirportId << mAirportName
            << mFplId << mFplName
            << mStateId << mStateName;
    }
    virtual void Deserialize(QDataStream& in)
    {
        CObject::Deserialize(in);
        in >> mId >> mStandId >> mStandName
           >> mAirportId >> mAirportName
           >> mFplId >> mFplName
           >> mStateId >> mStateName;
    }

    QByteArray toArray() const;

    bool operator == (const Stand& obj) const
    {
        return ( mId == obj.mId &&
                 mStandId == obj.mStandId &&
                 mStandName == obj.mStandName &&
                 mAirportId == obj.mAirportId &&
                 mAirportName == obj.mAirportName &&
                 mFplId == obj.mFplId &&
                 mFplName == obj.mFplName &&
                 mStateId == obj.mStateId &&
                 mStateName == obj.mStateName );
    }

    bool operator != (const Stand& obj) const
    { return !operator ==(obj); }

    XT_OBJECT_TYPE_DECL

public:
    /** Идентификатор */
    quint32     mId;
    /** Идентификатор стоянки */
    quint32     mStandId;
    /** Наименование стоянки */
    QString     mStandName;
    /** Идентификатор аэропорта */
    quint32     mAirportId;
    /** Наименование аэропорта */
    QString     mAirportName;
    /** Идентификатор FPL (aircraft id) */
    quint32     mFplId;
    /** Наименование FPL (aircraft id) */
    QString     mFplName;
    /** Идентификатор состояния */
    quint16     mStateId;
    /** Наименование состояния */
    QString     mStateName;
};

XT_END_NAMESPACE

QDataStream &operator<<(QDataStream &out, const Xt::Stand &stand);

#endif // XT_STAND_DATA_H
