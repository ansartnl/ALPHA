#include "XtDefines.h"
#include "XtTypes.h"

#include "XtAsterix62.h"
#include "XtMasterServer.h"
#include "XtPlotTrack.h"
#include "XtAirplaneInfo.h"
#include "XtMeteo.h"
#include "XtPeleng.h"
#include "XtRadar.h"
#include "XtSimCmd.h"
#include "XtStand.h"
#include "XtVehicle.h"

XT_BEGIN_NAMESPACE

//! Object version
static const int g_objectVersion = 0x0001000B;

CObject::CObject()
    : mVersion(g_objectVersion)
{
}

int CObject::GetClassVersion()
{
    return g_objectVersion;
}

XT_OBJECT_TYPE_IMPL(CAsterix62, EDataAsterix62)
XT_OBJECT_TYPE_IMPL(CMasterServer, EDataMasterServer)
XT_OBJECT_TYPE_IMPL(Plot, EDataPlot)
XT_OBJECT_TYPE_IMPL(Track, EDataTrack)
XT_OBJECT_TYPE_IMPL(AirplaneInfo, EDataAirplaneInfo)
XT_OBJECT_TYPE_IMPL(MeteoVector, EDataMeteoVector)
XT_OBJECT_TYPE_IMPL(MeteoContour, EDataMeteoContour)
XT_OBJECT_TYPE_IMPL(MeteoInfo, EDataMeteoInfo)
XT_OBJECT_TYPE_IMPL(Peleng, EDataPeleng)
XT_OBJECT_TYPE_IMPL(Radar, EDataRadar)
XT_OBJECT_TYPE_IMPL(SimCmd, EDataSimCmd)
XT_OBJECT_TYPE_IMPL(Stand, EDataStand)
XT_OBJECT_TYPE_IMPL(Vehicle, EDataVehicle)

XT_END_NAMESPACE
