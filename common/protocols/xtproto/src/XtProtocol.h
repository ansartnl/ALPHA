#ifndef XT_PROTOCOL_H
#define XT_PROTOCOL_H

#include "xtproto_global.h"
#include "XtDefines.h"
#include "XtMessage.h"

XT_BEGIN_NAMESPACE

//! Message protocol serializator
class XTPROTO_EXPORT CProtocol
{
public:
    //! Save message into buffer.
    /*!
        \return Filled buffer if message is valid.
    */
    static QByteArray Serialize(const TMessage& message);

    //! Make message from buffer.
    /*!
        \param bytesRead Number of bytes read
        \return Message object id buffer is valid otherwise null message
    */
    static TMessage Deserialize(const QByteArray& buffer, quint32 *bytesRead = 0);  // throw QString

    //! Current protocol version.
    /*!
        \return Current protoloc version.
    */
    static int Version();
};

XT_END_NAMESPACE

#endif // XT_PROTOCOL_H
