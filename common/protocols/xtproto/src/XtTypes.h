#ifndef XT_TYPES_H
#define XT_TYPES_H

#include "XtDefines.h"

XT_BEGIN_NAMESPACE

//! Message type
/*!
  \sa CMessage
*/
enum MessageType
{
    EData        = 1,
    ERequest,
    ESubscribe
};

enum MessageDataType
{
    EDataAsterix62      = 1,
    EDataMasterServer,
    EDataPlot,
    EDataTrack,
    EDataPeleng,
    EDataRadar,
    EDataAirplaneInfo,
    EDataMeteoVector,
    EDataMeteoContour,
    EDataMeteoInfo,
    EDataSimCmd,
    EDataStand,
    EDataVehicle
};

enum MessageRequestType
{
    ERequestAll          = 1,
    ERequestByType,
    ERequestMasterServer
};

enum MessageSubscribeType
{
    ESubscribeByType     = 1
};

XT_END_NAMESPACE

#endif // XT_GLOBAL_H
