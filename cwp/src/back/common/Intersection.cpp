#include "Intersection.h"
#include <qmath.h>

Intersection::Intersection()
{
}

CPoint Intersection::checkIntersection(CCoord p1, CCoord p2, CCoord border_Coord1, CCoord border_Coord2, CCoord border_Coord3, double radius, CBoundaryPoint::Type type)
{
    r_p1 = p1;
    r_p2 = p2;
    switch(type){
    case CBoundaryPoint::enLine:
        return checkLine(border_Coord1, border_Coord2);
        break;
    case CBoundaryPoint::enCircle:
        return checkCircle(border_Coord1, radius);
        break;
    case CBoundaryPoint::enLeftArc:
    case CBoundaryPoint::enRightArc:
        return checkArc(border_Coord1, border_Coord2, border_Coord3, radius, type == CBoundaryPoint::enRightArc);
        break;
    default:
        return CPoint();
        break;
    }
}

CPoint Intersection::checkLine(CCoord p1, CCoord p2)
{
    double v1=(p2.GetX() - p1.GetX()) * (r_p1.GetY() - p1.GetY())-
              (p2.GetY() - p1.GetY()) * (r_p1.GetX() - p1.GetX());

    double v2=(p2.GetX() - p1.GetX()) * (r_p2.GetY() - p1.GetY())-
              (p2.GetY() - p1.GetY()) * (r_p2.GetX() - p1.GetX());

    double v3=(r_p2.GetX() - r_p1.GetX()) * (p1.GetY() - r_p1.GetY())-
              (r_p2.GetY() - r_p1.GetY()) * (p1.GetX() - r_p1.GetX());

    double v4=(r_p2.GetX() - r_p1.GetX()) * (p2.GetY() - r_p1.GetY())-
              (r_p2.GetY() - r_p1.GetY()) * (p2.GetX() - r_p1.GetX());

    if((v1*v2<=0) && (v3*v4<=0))
    {// расчет координат точки пересечения
        CPoint p;
        double k1 = (r_p2.GetY() - r_p1.GetY()) / (r_p2.GetX() - r_p1.GetX());
        double k2 = (p2.GetY() - p1.GetY()) / (p2.GetX() - p1.GetX());
        double b1 = (r_p1.GetY()*r_p2.GetX() - r_p1.GetX()*r_p2.GetY()) /
                (r_p2.GetX() - r_p1.GetX());
        double b2 = (p1.GetY()*p2.GetX() - p1.GetX()*p2.GetY()) /
                (p2.GetX() - p1.GetX());
        double x = (b2 - b1)/(k1 - k2);
        double y = k1 * (b2 - b1) / (k1 - k2) + b1;
        p.SetCoord(CCoord(x, y));
        return p;
    }
    else
        return CPoint();
}

CPoint Intersection::checkCircle(CCoord center, double radius)
{
    double k = (r_p2.GetY() - r_p1.GetY()) / (r_p2.GetX() - r_p1.GetX());
    double b = (r_p1.GetY()*r_p2.GetX() - r_p1.GetX()*r_p2.GetY()) /
            (r_p2.GetX() - r_p1.GetX());
    //находим дискременант квадратного уравнения
    double d=(pow((2*k*b-2*center.GetX()-2*center.GetY()*k),2)-
              (4+4*k*k)*
              (b*b-radius*radius+center.GetX()*center.GetX()+center.GetY()*center.GetY()-2*center.GetY()*b));

    //если он равен 0, уравнение не имеет решения
    if(d<0)
        return CPoint();

    //иначе находим корни квадратного уравнения
    double x1=((-(2*k*b-2*center.GetX()-2*center.GetY()*k)-sqrt(d))/(2+2*k*k));
    double y1=k*x1+b;
    CPoint p;
    p.SetCoord(CCoord(x1, y1));
    return p;
}

CPoint Intersection::checkArc(CCoord center, CCoord point, CCoord prev, double radius, bool isRight)
{
    CPoint inter = checkCircle(center, radius);
    if(inter.GetCoord() != CCoord()){
        double k = (prev.GetY() - point.GetY()) / (prev.GetX() - point.GetX());
        double b = (point.GetY()*prev.GetX() - point.GetX()*prev.GetY()) /
                (prev.GetX() - point.GetX());
        double y = k * inter.GetCoord().GetX() + b;
        if((inter.GetCoord().GetY() > y && !isRight) ||
           (inter.GetCoord().GetY() < y && isRight))
            return inter;
        else
            return CPoint();
    }
    return CPoint();
}
