#include "ZOrderValue.h"

//! Geodesy graphics engine
namespace geoGE {

# define ZORDER_VALUE_TEST(itemTest, item) \
            if ( itemTest == item ) return qreal(item); \


qreal getZOrderValue(ZOrderItem item)
{
    // List of items in the order from bottom to top ...

    ZORDER_VALUE_TEST(item, ZOrderFIR);
    ZORDER_VALUE_TEST(item, ZOrderCorridor);
    ZORDER_VALUE_TEST(item, ZOrderSector);
    ZORDER_VALUE_TEST(item, ZOrderFIRBorder);
    ZORDER_VALUE_TEST(item, ZOrderRestriction);
    ZORDER_VALUE_TEST(item, ZOrderRoute);
    ZORDER_VALUE_TEST(item, ZOrderPoint);
    ZORDER_VALUE_TEST(item, ZOrderHolding);
    ZORDER_VALUE_TEST(item, ZOrderAirport);
    ZORDER_VALUE_TEST(item, ZOrderSSA);
    ZORDER_VALUE_TEST(item, ZOrderMeteoContour);
    ZORDER_VALUE_TEST(item, ZOrderMeteoVector);
    ZORDER_VALUE_TEST(item, ZOrderPeleng);
    ZORDER_VALUE_TEST(item, ZOrderGrid);

    return 0.0;
}

}   // namespace geoGE
