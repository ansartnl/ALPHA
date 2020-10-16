#ifndef _MISC_H__
#define _MISC_H__

#include "cs/CoordSystem.h"
#include "AirplaneInfo.h"
#include "orm/ReFir.h"

namespace rewarn {

//! Project restriction point coordinates using current coordinate system.
void projectFirCoords(const TCoordSystem &cs, TFirMap &firs);

//! Project restriction point coordinates using current coordinate system.
void projectAreaCoords(const TCoordSystem &cs, Area &area);

//! Prepare restriction zones for calculation.
void prepareFirForCalculations(TFirMap &firs);

//! Returns true if line intersects circle.
bool isLineIntersectCircle(const QLineF &line, const QPointF &circleCenter, double radius);

//! Find new airplane position (using delta time in seconds)
AirplaneInfo moveAirplane(const AirplaneInfo &airPlane, double dt);

//! Check if airplane is inside the area. Precise check.
bool isInside(const AirplaneInfo &airPlane, const Area &area);

//! Returns true if airplane flyes through the restriction area.
/*!
    \param distToArea Minimum distance to intersection point of this area.
*/
bool isTouches(const AirplaneInfo &airPlane, const AirplaneInfo &airPlaneMoved,
               const Area &area, double *distToArea = 0);

}   // namespace rewarn {

#endif // _MISC_H__
