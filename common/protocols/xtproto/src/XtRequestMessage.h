#ifndef XT_REQUEST_MESSAGE_H
#define XT_REQUEST_MESSAGE_H

#include "xtproto_global.h"
#include "XtDefines.h"
#include "XtTypes.h"
#include "XtMessage.h"
#include <QDataStream>


XT_BEGIN_NAMESPACE

//! Message class for storing request notification
class XTPROTO_EXPORT CRequestMessage : public CMessage
{
public:
    //! Constructs null data message.
    CRequestMessage()
        : m_RequType(0)
    {}

    //! Serialization handler.
    /*!
        \param out Output data stream
    */
    virtual void Serialize(QDataStream& out)
    {
        CMessage::Serialize(out);
        out << m_RequType;
    }
    //! Deserialization handler.
    /*!
        \param in Input data stream
        \param isPartial True if only current class types deserialization is required
    */
    virtual void Deserialize(QDataStream& in, bool isPartial = false)
    {
        if ( !isPartial )
            CMessage::Deserialize(in, isPartial);
        in >> m_RequType;
    }

    //! Type of request
    PROPERTY_DECL(int, RequType);

    XT_MESSAGE_TYPE_DECL

protected:
    //! Constructor for specific data message.
    CRequestMessage(int type)
        : CMessage((int)ERequest), m_RequType(type)
    {}
};

XT_END_NAMESPACE

#endif // XT_REQUEST_MESSAGE_H
