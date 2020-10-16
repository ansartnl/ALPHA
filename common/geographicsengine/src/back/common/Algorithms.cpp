#include "Algorithms.h"

#include <math.h>

#include <QRectF>
#include <QPointF>
#include <QLineF>

namespace algo
{

    QPointF closestPoint(const QRectF& r, const QPointF& p)
    {
        QPointF pc;
        pc.rx() = qAbs(p.x()-r.left()) < qAbs(p.x()-r.right()) ? r.left() : r.right();
        pc.ry() = qAbs(p.y()-r.top()) < qAbs(p.y()-r.bottom()) ? r.top() : r.bottom();
        return pc;
    }

    QRectF extendRect(const QRectF& r, const QPointF& p)
    {
        qreal x = p.x(), y = p.y();
        if ( r.left() < x )
            x = r.left();
        if ( r.top() < y )
            y = r.top();

        QRectF rp(x, y, qAbs(p.x()-r.left()), qAbs(p.y()-r.top()));
        return rp.united(r);
    }

    Direction classify(const QLineF &vec1, const QLineF &vec2)
    {
        qreal dotProduct = dot(vec1, vec2);
        return ( dotProduct == 0.0 ? Perpendicular : (dotProduct < 0.0 ? Opposite : Direct) );
    }

    qreal dot(const QLineF &vec1, const QLineF &vec2)
    {
        qreal dx1 = vec1.dx(),
            dy1 = vec1.dy();
        qreal dx2 = vec2.dx(),
            dy2 = vec2.dy();
        return ( dx1*dx2 + dy1*dy2 );
    }

    QLineF unitVector(const QLineF &line)
    {
        qreal lengthLine = line.length();
        return QLineF(QPointF(0., 0.), QPointF(line.dx()/lengthLine, line.dy()/lengthLine));
    }

    QLineF oppositeVector(const QLineF &line)
    {
        QPointF p1 = line.p1();
        return QLineF(p1, QPointF(p1.x()-line.dx(), p1.y()-line.dy()));
    }

    bool isRightSide(const QLineF& Line, const QPointF& Point)
    {
        QLineF LineTemp(Line.p1(), Point);
        double dAngle = Line.angleTo(LineTemp);
        if (dAngle > 0 && dAngle < 180)
            return false;
        return true;
    }

    double azimuth(double dAzim)
    {
        dAzim = fmod(dAzim, 2*M_PI);
        if (dAzim < 0)
            dAzim = 2*M_PI + dAzim;

        return dAzim;
    }

    double azimuthToAngle(double dAzim)
    {
        dAzim = azimuth(2 * M_PI - dAzim + M_PI_2);
        if (dAzim > M_PI)
            dAzim = dAzim - 2 * M_PI;
        return dAzim;
    }

    double angleToAzimuth(double dAngle)
    {
        if (dAngle < 0)
            dAngle = dAngle + 2 * M_PI;
        dAngle = azimuth(2 * M_PI - dAngle + M_PI_2);
        return dAngle;
    }

    int getTurningOrientation(double dFirst, double dNext)
    {
        double dDelta = dNext - dFirst;
        if ((dDelta >= 0 && dDelta <= M_PI) ||
            (dDelta >= (-2 * M_PI) && dDelta <= -M_PI))
            return 1;
        return -1;
    }

}   // namespace algo
