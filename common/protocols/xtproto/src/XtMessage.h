#ifndef XT_MESSAGE_H
#define XT_MESSAGE_H

#include "xtproto_global.h"
#include "XtDefines.h"
#include <QDataStream>
#include <QSharedPointer>


XT_BEGIN_NAMESPACE


//! Base message class
class XTPROTO_EXPORT CMessage
{
public:
    //! Constructs null message
    CMessage()
        : m_Type(0)
    {}
    virtual ~CMessage()
    {}

    //! Serialization handler.
    /*!
        \param out Output data stream
    */
    virtual void Serialize(QDataStream& out)
    { out << m_Type; }
    //! Deserialization handler.
    /*!
        \param in Input data stream
        \param isPartial True if only current class types deserialization is required
    */
    virtual void Deserialize(QDataStream& in, bool isPartial = false)
    { in >> m_Type; }

    //! Retrieve message type
    int GetType() const
    { return m_Type; }

    //! Returns true if message is not valid.
    bool isNull() const
    { return ( m_Type < 1 ); }

protected:
    //! Constructor
    CMessage(quint32 type)
        : m_Type(type)
    {}

    /** Message type */
    quint32     m_Type;
};

typedef QSharedPointer<CMessage>    TMessage;


#define XT_MESSAGE_TYPE_DECL \
        public: \
        static int GetClassType();

#define XT_MESSAGE_TYPE_IMPL(BaseClass, Value) \
        int BaseClass::GetClassType() { return int(Value); }


XT_END_NAMESPACE

#endif // XT_MESSAGE_H
