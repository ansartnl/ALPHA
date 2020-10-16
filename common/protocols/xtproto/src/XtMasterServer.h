#ifndef XT_MASTER_SERVER_DATA_H
#define XT_MASTER_SERVER_DATA_H

#include "xtproto_global.h"
#include "XtDefines.h"
#include "XtObject.h"
#include <QtGlobal>

XT_BEGIN_NAMESPACE

//! Structure to store master server location
struct XTPROTO_EXPORT CMasterServer : public CObject
{
    virtual void Serialize(QDataStream& out)
    {
        out << m_host << m_port;
    }
    virtual void Deserialize(QDataStream& in)
    {
        in >> m_host >> m_port;
    }

    bool operator == (const CMasterServer& obj) const
    {
        return ( m_host == obj.m_host && m_port == obj.m_port );
    }

    bool operator != (const CMasterServer& obj) const
    { return !operator ==(obj); }

    XT_OBJECT_TYPE_DECL

public:
    QString     m_host;
    quint32     m_port;
};

XT_END_NAMESPACE

#endif // XT_MASTER_SERVER_DATA_H
