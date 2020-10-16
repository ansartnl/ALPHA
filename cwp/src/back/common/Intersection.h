#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "Point.h"
#include "BoundaryPoint.h"

class Intersection
{
public:
    Intersection();

    //! Checks if FPL route intersects sector line, depending on line's type
    /*!
      \param p1,p2 - FPL route points (start and end)
      \param border_Coord1 - arc center or 1st line point (4 all types)
      \param border_Coord2 - 2nd line point or Arc starting point (4 line or Arcs)
      \param border_coord3 - Arc ending point (4 arcs)
      \param radius - Arc and Circle radius (4 arcs and circle)
      \param type - line type (line, circle, left arc, right arc
      \return intersection point
    */
    CPoint checkIntersection(CCoord p1, CCoord p2, CCoord border_Coord1, CCoord border_Coord2,
                             CCoord border_Coord3 = CCoord(), double radius = 0,
                             CBoundaryPoint::Type type = CBoundaryPoint::enLine);

private:

    CCoord r_p1;
    CCoord r_p2;

    CPoint checkLine(CCoord p1, CCoord p2);
    CPoint checkCircle(CCoord center, double radius);
    CPoint checkArc(CCoord center, CCoord point, CCoord prev, double radius, bool isRight);
};

#endif // INTERSECTION_H
