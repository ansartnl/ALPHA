#include "WaypointManager.h"

#include "front/ground/GUIGroundScene.h"

WaypointManager::WaypointManager(GUIGroundScene *scene /* = 0 */)
    : mScene(scene), mEditable(false)
{
}

int WaypointManager::addNew(const QPoint &point)
{
    if(mScene)
    {
        return addNew(mScene->GetCoordAtPoint(mScene->mapToScene(point)));
    }
    else
        return -1;
}

int WaypointManager::addNew(const CCoord &coord)
{
    int SN = -1;

    if(mWaypoints.size())
        SN = mWaypoints.keys().last() + 1;
    else
        SN = 0;

    mWaypoints.insert(SN, coord);
    return SN;
}

void WaypointManager::move(int SN, const CCoord &coord)
{
    if(mWaypoints.contains(SN))
    {
        mWaypoints[SN] = coord;
    }
}

void WaypointManager::remove(int SN)
{
    if(mWaypoints.contains(SN))
        mWaypoints.remove(SN);
}
