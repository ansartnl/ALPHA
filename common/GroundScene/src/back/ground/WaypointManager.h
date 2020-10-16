#ifndef WAYPOINTMANAGER_H
#define WAYPOINTMANAGER_H

#include "GroundCoordinate.h"

class GUIGroundScene;

class WaypointManager
{
public:
    WaypointManager(GUIGroundScene* scene = 0);

    inline GUIGroundScene& getScene() const
    {return *mScene;}

    void setWaypoints(const QList<CCoord>& waypoints);

    void clearWaypoints()
    {mWaypoints.clear();}

    inline QList<CCoord> waypoints() const
    {return mWaypoints.values();}

    inline void allowEdit(bool allow)
    {mEditable = allow;}

    int addNew(const QPoint& point);

    virtual int addNew(const CCoord& coord);

    void move(int SN, const CCoord& coord);
    void remove(int SN);

private:
    GUIGroundScene* mScene;
    QMap<int, CCoord>   mWaypoints;
    bool            mEditable;
};

#endif // WAYPOINTMANAGER_H
