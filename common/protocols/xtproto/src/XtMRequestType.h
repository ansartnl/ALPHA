#ifndef XT_REQUEST_MESSAGE_BY_TYPE_H
#define XT_REQUEST_MESSAGE_BY_TYPE_H

#include "xtproto_global.h"
#include "XtDefines.h"
#include "XtRequestMessage.h"
#include <QSet>

XT_BEGIN_NAMESPACE

//! Message to request data by specified types
class XTPROTO_EXPORT CMRequestType : public CRequestMessage
{
public:
    typedef QSet<int>   TDataTypeList;

    //! Constructs null data message.
    CMRequestType()
        : CRequestMessage((int)ERequestByType)
    {}

    //! Serialization handler.
    /*!
        \param out Output data stream
    */
    virtual void Serialize(QDataStream& out)
    {
        CRequestMessage::Serialize(out);
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
            CRequestMessage::Deserialize(in, isPartial);
        in >> m_DataTypeList;
    }

    //! Add data type to the list of request.
    void AddDataType(int typeData)
    { m_DataTypeList << typeData; }

    //! List of requesting data types
    PROPERTY_DECL(TDataTypeList, DataTypeList);

    XT_MESSAGE_TYPE_DECL
};

XT_END_NAMESPACE

#endif // XT_REQUEST_MESSAGE_BY_TYPE_H
