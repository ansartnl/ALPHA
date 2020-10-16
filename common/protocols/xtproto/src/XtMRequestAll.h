#ifndef XT_REQUEST_ALL_MESSAGES_H
#define XT_REQUEST_ALL_MESSAGES_H

#include "xtproto_global.h"
#include "XtDefines.h"
#include "XtRequestMessage.h"

XT_BEGIN_NAMESPACE

//! Message to request all data server owns
class XTPROTO_EXPORT CMRequestAll : public CRequestMessage
{
public:
    //! Constructs null data message.
    CMRequestAll()
        : CRequestMessage((int)ERequestAll)
    {}

    XT_MESSAGE_TYPE_DECL
};

XT_END_NAMESPACE

#endif // XT_REQUEST_ALL_MESSAGES_H
