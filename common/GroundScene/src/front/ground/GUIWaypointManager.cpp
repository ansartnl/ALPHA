#include "GUIWaypointManager.h"

#include <QPainter>
#include "back/ground/GroundMain.h"
#include "back/ground/GroundSceneLogic.h"
#include "front/ground/GUIGroundScene.h"
#include "import/Point.h"

GUIWaypointManager::GUIWaypointManager(QGraphicsItem *parent) :
    QGraphicsItem(parent)
{
    setZValue(gui::ZwaypointMarker);
}

GUIWaypointManager::GUIWaypointManager(GUIGroundScene *scene) :
      WaypointManager(scene),
      QGraphicsItem()
{
    setZValue(gui::ZwaypointMarker);
}

GUIWaypointManager::~GUIWaypointManager()
{
}

QRectF GUIWaypointManager::boundingRect() const
{
    return mPath.boundingRect();
}

QPainterPath GUIWaypointManager::shape() const
{
    return mPath;
}

void GUIWaypointManager::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    QPen pen(Qt::yellow);
    pen.setWidth(2);
    painter->setPen(pen);
    painter->drawPath(mPath);

    painter->restore();
}

void GUIWaypointManager::recalculate()
{
#ifndef GROUND_CWP
    mPathCoords.clear();
    QVector<QSharedPointer<atd::CRoutePoint> > pts = getScene().logic()->computeVehiclePath(waypoints().toVector());
    foreach(QSharedPointer<atd::CRoutePoint> pnt, pts)
        mPathCoords.append(pnt->GetPoint()->GetCoord());

    getScene().Draw();
#endif
}

void GUIWaypointManager::SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord &centralCoord)
{
    prepareGeometryChange();
    mPath = QPainterPath();

    CCoord prevCoord;
    CCoord coord;
    for(int i = 0; i < mPathCoords.size(); ++i)
    {
        if(i == 0)
        {
            prevCoord = mPathCoords.first();
            prevCoord.Center(centralCoord);
        }
        coord = mPathCoords[i];
        coord.Center(centralCoord);

        QPointF point1(
            (prevCoord.GetX() - qllXMin) * dScale,
            (qllYMax - prevCoord.GetY()) * dScale);

        QPointF point2(
            (coord.GetX() - qllXMin) * dScale,
            (qllYMax - coord.GetY()) * dScale);

        point1 = mapFromScene(point1);
        point2 = mapFromScene(point2);

        mPath.moveTo(point1);
        mPath.lineTo(point2);
        prevCoord = coord;
    }
    update();
}

void GUIWaypointManager::onWaypointMoved(int SN, QPointF where)
{
    CCoord coord = getScene().GetCoordAtPoint(where.toPoint());
    move(SN, coord);    

    if(getScene().GetWaypointMarkers().contains(SN))
    {
        CCoord coord = getScene().GetCoordAtPoint(where.toPoint());
        getScene().GetWaypointMarkers()[SN]->setCoord(coord);
    }

    recalculate();
}

void GUIWaypointManager::onWaypointDeleted(int SN)
{
    remove(SN);    

    if(getScene().GetWaypointMarkers().contains(SN))
    {
        getScene().setDeletingWaypoint(true);
        getScene().DelWaypointMarker(getScene().GetWaypointMarkers()[SN]);
    }

    recalculate();
}
