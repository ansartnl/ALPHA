#ifndef _ALGORITHMS_H__
#define _ALGORITHMS_H__

#include <QtCore/QtGlobal>
class QPointF;
class QRectF;
class QLineF;

//! Namespace contains common calculation functions
namespace algo
{
    // Direction of 2 vectors.
    enum Direction
    {
        Direct,         /*!< Vectors have the same direction.*/
        Opposite,       /*!< Vectors point to opposite directions.*/
        Perpendicular   /*!< Vectors are perpendicular.*/
    };

    //! Finds closest point to the rectangle
    /*!
        \param r Source rectangle
        \param p Point which is base for seaching the closest point
    */
    QPointF closestPoint(const QRectF& r, const QPointF& p);

    //! Extend rectangle by a point.
    QRectF extendRect(const QRectF& r, const QPointF& p);

    //! Find direction of vectors.
    Direction classify(const QLineF &vec1, const QLineF &vec2);

    //! Dot product of 2 vectors.
    qreal dot(const QLineF &vec1, const QLineF &vec2);

    //! Unit vector in the position (0.0, 0.0).
    QLineF unitVector(const QLineF &line);

    //! Make vector with opposite direction with the same origin.
    QLineF oppositeVector(const QLineF &line);

    //! Return true if point lays on the right side of the line.
    bool isRightSide(const QLineF& Line, const QPointF& Point);

    //! Make positive azimuth less than 2*M_PI.
    double azimuth(double dAzim);

    //! Convert azimuth to Qt angle.
    double azimuthToAngle(double dAzim);

    //! Convert Qt angle to azimuth.
    double angleToAzimuth(double dAngle);

    //! Gets turning orientation
    int getTurningOrientation(double dFirst, double dNext);
}

#endif // _ALGORITHMS_H__
