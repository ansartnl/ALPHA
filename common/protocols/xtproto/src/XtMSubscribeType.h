#ifndef XT_SUBSCRIBE_MESSAGE_TYPE_H
#define XT_SUBSCRIBE_MESSAGE_TYPE_H

#include "xtproto_global.h"
#include "XtDefines.h"
#include "XtSubscribeMessage.h"
#include <QSet>

XT_BEGIN_NAMESPACE

//! Message to subscribe on specified data types
class XTPROTO_EXPORT CMSubscribeType : public CSubscribeMessage
{
public:
    typedef QSet<quint32>   TDataTypeList;

    //! Constructs null data message.
    CMSubscribeType()
        : CSubscribeMessage((int)ESubscribeByType)
    {}

    //! Serialization handler.
    /*!
        \param out Output data stream
    */
    virtual void Serialize(QDataStream& out)
    {
        CSubscribeMessage::Serialize(out);
        out << m_DataTypeList;
    }
    //! Deserialization handler.
    /*!
        \param in Input data stream
        \param isPartial True if only current class types deserialization is required
    */
    virtual void Deserialize(QDataStream& in, bool isPartial)
    {
        if ( !isPartial )
            CSubscribeMessage::Deserialize(in, isPartial);
        in >> m_DataTypeList;
    }

    //! Add data type to the list of subscription.
    void AddDataType(quint32 typeData)
    { m_DataTypeList << typeData; }

    //! List of subscribing data types
    PROPERTY_DECL(TDataTypeList, DataTypeList);

    XT_MESSAGE_TYPE_DECL
};

XT_END_NAMESPACE

#endif // XT_SUBSCRIBE_MESSAGE_TYPE_H
