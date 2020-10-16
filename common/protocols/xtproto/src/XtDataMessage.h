#ifndef XT_DATA_MESSAGE_H
#define XT_DATA_MESSAGE_H

#include "xtproto_global.h"
#include "XtDefines.h"
#include "XtTypes.h"
#include "XtMessage.h"
#include "XtObject.h"
#include <QDataStream>


XT_BEGIN_NAMESPACE

//! Message class for storing data
class XTPROTO_EXPORT CDataMessage : public CMessage
{
public:
    //! Constructor.
    CDataMessage()
        : CMessage((int)EData), m_DataType(0)
    {}

    //! Serialization handler.
    /*!
        \param out Output data stream
    */
    virtual void Serialize(QDataStream& out)
    {
        CMessage::Serialize(out);
        out << m_DataType;
        if ( m_Object )
            m_Object->Serialize(out);
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
        in >> m_DataType;
        if ( !isPartial && m_Object && m_Object->GetType() == m_DataType )
            m_Object->Deserialize(in);
    }

    //! Pack object to the message.
    void SetObject(const TObject& obj)
    {
        m_Object = obj;
        // Set same type of data message as packed object type.
        if ( obj )
            m_DataType = obj->GetType();
    }
    //! Retrieve reference to packed data object.
    const TObject& GetObject() const
    { return m_Object; }

    //! Data type of currently packed object.
    PROPERTY_DECL(int, DataType);

    XT_MESSAGE_TYPE_DECL

private:
    /** Data object */
    TObject     m_Object;
};

XT_END_NAMESPACE

#endif // XT_DATA_MESSAGE_H
