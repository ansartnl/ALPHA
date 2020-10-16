#include "XtDefines.h"
#include "XtTypes.h"

#include "XtDataMessage.h"
#include "XtSubscribeMessage.h"
#include "XtRequestMessage.h"

#include "XtMRequestType.h"
#include "XtMRequestAll.h"
#include "XtMRequestMaster.h"

#include "XtMSubscribeType.h"

XT_BEGIN_NAMESPACE

// Message types impl
XT_MESSAGE_TYPE_IMPL(CDataMessage, EData)
XT_MESSAGE_TYPE_IMPL(CSubscribeMessage, ESubscribe)
XT_MESSAGE_TYPE_IMPL(CRequestMessage, ERequest)

// Request messages impl
XT_MESSAGE_TYPE_IMPL(CMRequestAll, ERequestAll)
XT_MESSAGE_TYPE_IMPL(CMRequestType, ERequestByType)
XT_MESSAGE_TYPE_IMPL(CMRequestMasterServer, ERequestMasterServer)

// Subscribe messages impl
XT_MESSAGE_TYPE_IMPL(CMSubscribeType, ESubscribeByType)

XT_END_NAMESPACE
