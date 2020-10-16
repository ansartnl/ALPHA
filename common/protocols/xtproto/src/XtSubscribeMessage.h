#ifndef XT_SUBSCRIBE_MESSAGE_H
#define XT_SUBSCRIBE_MESSAGE_H

#include "xtproto_global.h"
#include "XtDefines.h"
#include "XtTypes.h"
#include "XtMessage.h"
#include <QDataStream>


XT_BEGIN_NAMESPACE

//! Message class for storing subscribe notification
class XTPROTO_EXPORT CSubscribeMessage : public CMessage
{
public:
    //! Constructs null data message.
    CSubscribeMessage()
        : m_SubsType(0)
    {}

    //! Serialization handler.
    /*!
        \param out Output data stream
    */
    virtual void Serialize(QDataStream& out)
    {
        CMessage::Serialize(out);
        out << m_SubsType;
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
        in >> m_SubsType;
    }

    //! Type of subscription
    PROPERTY_DECL(int, SubsType);

    XT_MESSAGE_TYPE_DECL

protected:
    //! Constructor for specific data message.
    CSubscribeMessage(int type)
        : CMessage((int)ESubscribe), m_SubsType(type)
    {}
};

XT_END_NAMESPACE

#endif // XT_SUBSCRIBE_MESSAGE_H
