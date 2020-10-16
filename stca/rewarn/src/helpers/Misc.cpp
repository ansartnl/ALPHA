#include "Misc.h"

#include <math.h>

namespace rewarn {

void projectFirCoords(const TCoordSystem &cs, TFirMap &firs)
{
    for (auto itFir = firs.begin(); itFir != firs.end(); ++itFir)
    {
        ReFir &fir = *itFir;
        for (auto itArea = fir.mAreas.begin(); itArea != fir.mAreas.end(); ++itArea)
        {
            projectAreaCoords(cs, *itArea);
        }
        for (auto itArea = fir.mMeteoAreas.begin(); itArea != fir.mMeteoAreas.end(); ++itArea)
        {
            projectAreaCoords(cs, *itArea);
        }
    }
}

void projectAreaCoords(const TCoordSystem &cs, Area &area)
{
    for (auto itPt = area.mPoints.begin(); itPt != area.mPoints.end(); ++itPt)
    {
        RePoint &point = *itPt;
        if (point.mType == RePoint::enLine)
            cs->toModel(point.mCoord);
        else
            cs->toModel(point.mArcCoord);
    }
}

void prepareFirForCalculations(TFirMap &firs)
{
    for (auto itFir = firs.begin(); itFir != firs.end(); ++itFir)
    {
        ReFir &fir = *itFir;
        for (auto itArea = fir.mAreas.begin(); itArea != fir.mAreas.end(); ++itArea)
        {
            (*itArea).prepare();
        }
        for (auto itArea = fir.mMeteoAreas.begin(); itArea != fir.mMeteoAreas.end(); ++itArea)
        {
            (*itArea).prepare();
        }
        fir.prepare();
    }
}

bool isLineIntersectCircle(const QLineF &line, const QPointF &circleCenter, double radius)
{
    QPointF A = line.p1(), B = line.p2();
    const QPointF &C = circleCenter;

    //Окнужность пересекает прямую AB, если в треугольнике ABC, где C - центр окружности,
    //длина h высоты , опущенной на сторону AB, не больше радиуса данной окружности.
    //h найдем, разделив удвоенную площадь треугольника ABC на длину отрезка AB.
    //Удвоенную площадь треугольника ABC найдем как модуль определителя векторов AB и AC.
    //Итак, пересекает, если h <= R, т.е.
    //2 * S / abs(AB) <= R

    // Если заданы декартовы координаты точек на плоскости A(x1, y1), B(x2, y2) и C(x3, y3),
    // то площадь S можно найти по следующей формуле (через определитель второго порядка для
    // матрицы разниц координат):
    //
    // S = |(x1 – x3)·(y2 – y3) – (x2 – x3)·(y1 – y3)|/2,
    //
    // где |...| — обозначение модуля. Эта формула получена из выражения для векторного
    // произведения двух векторов на плоскости, которое по абсолютной величине равно значению
    // определителя, составленного из их координат.

    double h = qAbs((A.x()-C.x())*(B.y()-C.y()) - (B.x()-C.x())*(A.y()-C.y())) / line.length();
    return h <= radius;
}

AirplaneInfo moveAirplane(const AirplaneInfo &airPlane, double dt)
{
    AirplaneInfo airPlaneNew = airPlane;
    airPlaneNew.x += airPlane.dx*dt;
    airPlaneNew.y += airPlane.dy*dt;
    airPlaneNew.height += airPlane.dz*dt;
    return airPlaneNew;
}

bool isInside(const AirplaneInfo &airPlane, const Area &area)
{
    QPointF pt(airPlane.x, airPlane.y);

    // Check hitting inside polygon.
    if ( !area.mPolygon.isEmpty() )
        return area.mPolygon.containsPoint(pt, Qt::OddEvenFill);
    else
    {
        // Check hit inside the circle.
        foreach ( const RePoint &point, area.mPoints )
        {
            if ( point.mType == RePoint::enCircle )
            {
                if ( QLineF(pt, QPointF(point.mArcCoord.GetX(), point.mArcCoord.GetY())).length() < point.mArcDistance )
                    return true;
            }
        }
    }

    return false;
}

// Intersect line with a polygon.
QVector<QPointF> intersect(const QLineF &line, const QPolygonF &polygon)
{
    QVector<QPointF> inters;

    if ( polygon.size() < 2 )
        return inters;

    QPointF intersPoint;
    for ( int i = 1; i < polygon.size(); i++ )
    {
        QLineF linePoly(polygon[i-1], polygon[i]);
        if ( line.intersect(linePoly, &intersPoint) == QLineF::BoundedIntersection )
            inters << intersPoint;
    }
    // Test join line.
    QLineF linePoly(polygon.back(), polygon.first());
    if ( line.intersect(linePoly, &intersPoint) == QLineF::BoundedIntersection )
        inters << intersPoint;

    return inters;
}

static inline double operator*(const QPointF &p1, const QPointF &p2)
{
    return p1.x()*p2.x() + p1.y()*p2.y();
}

// Intersect line with a circle.
QVector<QPointF> intersect(const QLineF &line, const QPointF &circleCenter, double radius)
{
    QVector<QPointF> inters;

    QPointF U = line.p1(),
            V = line.p2() - line.p1();

    // Алгоритм взят с ресурса:
    // http://forum.codenet.ru/threads/59244-%D0%9F%D0%B5%D1%80%D0%B5%D1%81%D0%B5%D1%87%D0%B5%D0%BD%D0%B8%D0%B5-%D0%BE%D1%82%D1%80%D0%B5%D0%B7%D0%BA%D0%B0-%D0%B8-%D0%BE%D0%BA%D1%80%D1%83%D0%B6%D0%BD%D0%BE%D1%81%D1%82%D0%B8
    //
    // Информация для размышлени:
    // Glassner A. (ed.) Graphics gems, vol.1 (AP, 1995)(T)(666s).djvu

    QPointF g = U - circleCenter;
    double a = V * V;
    double b = 2. * (V * g);
    double c = g*g - radius*radius;
    double d = b*b - 4.*a*c;

    if ( d < 0 )
        return inters;

    double sqrt_from_d = sqrt(d);
    double double_a = 2.*a;

    double first = (-b - sqrt_from_d)/double_a;
    double second = (-b + sqrt_from_d)/double_a;

    // Не пересекает в двух случаях:
    // 1. first < 0 && second < 0;
    // 2. first > 1 && second > 1.
    if ( (first < 0. && second < 0.) || (first > 1. && second > 1.) )
        return inters;

    // Leave first point closer to start point of a line.
    if ( second < first )
        qSwap(first, second);

    inters.push_back( U + V*first );
    inters.push_back( U + V*second );
    return inters;
}

double lengthSquared(const QPointF &p1, const QPointF &p2)
{
    return (p2.x()-p1.x())*(p2.x()-p1.x()) + (p2.y()-p1.y())*(p2.y()-p1.y());
}

bool isTouches(const AirplaneInfo &airPlane, const AirplaneInfo &airPlaneMoved,
               const Area &area, double *distToArea)
{
    QPointF ptAirPlane(airPlane.x, airPlane.y);
    QPointF ptAirPlaneMoved(airPlaneMoved.x, airPlaneMoved.y);
    QLineF line(ptAirPlane, ptAirPlaneMoved);
    double lengthAir = line.length();

    QVector<QPointF> inters;

    // Intersection with the polygon.
    if ( !area.mPolygon.isEmpty() )
        inters = intersect(line, area.mPolygon);

    else
    {
        // Check hit inside the circle.
        foreach ( const RePoint &point, area.mPoints )
        {
            // think about circles only here.
            if ( point.mType != RePoint::enCircle )
                continue;

            QPointF circleCenter(point.mArcCoord.GetX(), point.mArcCoord.GetY());

            double lenToCircleCenter = QLineF(ptAirPlane, circleCenter).length();
            double lenToTouch = QLineF(ptAirPlane, ptAirPlaneMoved).length() + point.mArcDistance;
            if ( lenToCircleCenter <= lenToTouch
                 && isLineIntersectCircle(line, circleCenter, point.mArcDistance) )
            {
                inters = intersect(line, circleCenter, point.mArcDistance);
                if ( !inters.empty() )
                    break;
            }
        }
    }

    foreach ( const QPointF &pt, inters )
    {
        double distToPoint = QLineF(ptAirPlane, pt).length();
        double coef = distToPoint / lengthAir;
        double heightPt = airPlane.height +
                (airPlaneMoved.height-airPlane.height)*coef;

        if ( area.mHeightMin <= heightPt && heightPt <= area.mHeightMax )
        {
            if ( distToArea )
                *distToArea = distToPoint;
            return true;
        }
    }

    return false;
}

}   // namespace rewarn {
