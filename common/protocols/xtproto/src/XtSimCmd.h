#ifndef XT_SIM_CMD_DATA_H
#define XT_SIM_CMD_DATA_H

#include "xtproto_global.h"
#include "XtDefines.h"
#include "XtObject.h"
#include "XtCommon.h"
#include <QtGlobal>

XT_BEGIN_NAMESPACE

//! Simulator commands
struct XTPROTO_EXPORT SimCmd : public CObject
{
    enum SimCmdType
    {
        StartExercise, StopExercise, BeginMeteo, EndMeteo,
        BeginStand, EndStand
    };

    SimCmd()
        : mType(0), mDBID(0) {}

    virtual void Serialize(QDataStream& out) const
    {
        CObject::Serialize(out);
        out << mType << mIdFir << mDBID;
    }
    virtual void Deserialize(QDataStream& in)
    {
        CObject::Deserialize(in);
        in >> mType >> mIdFir >> mDBID;
    }

    QByteArray toArray() const;

    bool operator == (const SimCmd& obj) const
    {
        return ( mType == obj.mType && mIdFir == obj.mIdFir &&
                 mDBID == obj.mDBID );
    }

    bool operator != (const SimCmd& obj) const
    { return !operator ==(obj); }

    XT_OBJECT_TYPE_DECL

public:
    /** Тип команды SimCmdType */
    quint32     mType;
    /** Идентификатор FIR */
    QString     mIdFir;
    /** Database identifier */
    quint32     mDBID;
};

XT_END_NAMESPACE

QDataStream &operator<<(QDataStream &out, const Xt::SimCmd &simcmd);

#endif // XT_SIM_CMD_DATA_H
