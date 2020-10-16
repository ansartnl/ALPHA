#ifndef _Z_ORDER_VALUE_H__
#define _Z_ORDER_VALUE_H__

#include <QtCore/QtGlobal>

//! Geodesy graphics engine
namespace geoGE {

//! Z order item for GUI graphics items.
enum ZOrderItem
{
    ZOrderFIR           = 10,
    ZOrderCorridor      = 20,
    ZOrderSector        = 30,
    ZOrderFIRBorder     = 40,
    ZOrderRestriction   = 50,
    ZOrderRoute         = 60,
    ZOrderPoint         = 70,
    ZOrderHolding       = 80,
    ZOrderAirport       = 90,
    ZOrderSSA           = 100,
	
    ZOrderMeteoContour  = 110,
    ZOrderMeteoVector   = 120,
    ZOrderPeleng        = 130,

    ZOrderGrid          = 140
};

//! Retrieve Z order value for GUI graphics item.
qreal getZOrderValue(ZOrderItem item);

}   // namespace geoGE

#endif // _Z_ORDER_VALUE_H__
