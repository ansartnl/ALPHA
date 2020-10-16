#include "Area.h"

namespace rewarn {

void Area::prepare()
{
    // Make cartesian polygon.
    mPolygon = QPolygonF();
    if (mPoints.size() > 2)
    {
        QMapIterator<int, RePoint> it(mPoints);
        while ( it.hasNext() )
        {
            it.next();
            const RePoint &point = it.value();

            if ( point.mType == RePoint::enLine )
                mPolygon << QPointF(point.mCoord.GetX(), point.mCoord.GetY());
            else if ( point.mType == RePoint::enLeftArc || point.mType == RePoint::enRightArc )
                mPolygon << QPointF(point.mArcCoord.GetX(), point.mArcCoord.GetY());
        }
    }

    const double dbl_max = 1e+37;
    QPointF minPt(dbl_max, dbl_max), maxPt(-dbl_max, -dbl_max);

    // Find topLeft and bottomRight rect points.
    foreach ( const RePoint &point, mPoints )
    {
        if ( point.mType == RePoint::enLine )
        {
            minPt.rx() = qMin(minPt.x(), (qreal)point.mCoord.GetX());
            minPt.ry() = qMin(minPt.y(), (qreal)point.mCoord.GetY());
            maxPt.rx() = qMax(maxPt.x(), (qreal)point.mCoord.GetX());
            maxPt.ry() = qMax(maxPt.y(), (qreal)point.mCoord.GetY());
        }
        else if ( point.mType == RePoint::enLeftArc || point.mType == RePoint::enRightArc )
        {
            minPt.rx() = qMin(minPt.x(), (qreal)point.mArcCoord.GetX());
            minPt.ry() = qMin(minPt.y(), (qreal)point.mArcCoord.GetY());
            maxPt.rx() = qMax(maxPt.x(), (qreal)point.mArcCoord.GetX());
            maxPt.ry() = qMax(maxPt.y(), (qreal)point.mArcCoord.GetY());
        }
        else
        {
            minPt = QPointF((qreal)point.mArcCoord.GetX()-point.mArcDistance,
                            (qreal)point.mArcCoord.GetY()-point.mArcDistance);
            maxPt = QPointF((qreal)point.mArcCoord.GetX()+point.mArcDistance,
                            (qreal)point.mArcCoord.GetY()+point.mArcDistance);
        }
    }

    mBoundingBox = QRectF(minPt, maxPt);
}

}
