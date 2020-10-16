#include "GroundSceneLogic.h"

#ifndef GROUND_CWP
#include "utils/TVAMath.h"
#include "Simulator.h"

QString GroundSceneLogic::reverseDesignator(QString sDesignator)
{
    QString designator = "";
    QString course = reverseCourse(sDesignator.left(2));

    if(sDesignator.length() > 2)    //designator included
    {
        sDesignator.remove(0,2);
        if(sDesignator == QString("L"))
            designator = QString("R");
        else if(sDesignator == QString("R"))
            designator = QString("L");
        else if(sDesignator == QString("LL"))
            designator = QString("RR");
        else if(sDesignator == QString("RR"))
            designator = QString("LL");
        else
            designator = QString("C");
    }

    return QString("%1%2").arg(course).arg(designator);
}

GroundSceneLogic::TPointList GroundSceneLogic::getPointsByTaxiName(QString name)
{
    TPathList::const_iterator itPath = mScene->getPaths().begin();
    TPointList points;

    TPathList::const_iterator itEnd = mScene->getPaths().end();
    for(; itPath != itEnd; ++itPath)
    {
        if( (*itPath)->name() == name)
        {
            if(mScene->getPoints().contains((*itPath)->startPointIndex()))
                points.append( mScene->getPoints()[ (*itPath)->startPointIndex()] );
            if(mScene->getPoints().contains((*itPath)->endPointIndex()))
                points.append( mScene->getPoints()[ (*itPath)->endPointIndex()] );
        }
    }
    return points;
}

int GroundSceneLogic::findPoint(CCoord Location, bool bExactMatch, double dSearchRadius /* = 0 */)
{
    Location.Center(mScene->GetCentralPoint()->GetCoord());
    QLineF planeCheck;
    planeCheck.setP1( QPointF(Location.GetX(),
                              Location.GetY()));

    int idx = -1;
    double len = 0xffffff;
    int pointsCount = mScene->getPoints().count();
    for(int i = 0; i < pointsCount; i++)
    {
        GroundPoint* pnt = mScene->getPoints().values().at(i).data();
        if(bExactMatch)
        {
            if(Location == pnt->coord())
            {
                return pnt->index();
            }
        }
        else
        {
            planeCheck.setP2(QPointF(pnt->coord().GetX(),
                                     pnt->coord().GetY()));

            if(planeCheck.length() < len &&
                    (dSearchRadius == 0 || planeCheck.length() < dSearchRadius))
            {
                len = planeCheck.length();
                idx = pnt->index();
            }
        }
    }
    return idx;
}

QVector<QSharedPointer<atd::CRoutePoint> > GroundSceneLogic::parseTakeOffPath(QString sPath, quint16 planeID, QString sParking)
{
    TPointList lRoute;
    QVector<QSharedPointer<atd::CRoutePoint> > route;
    CSimulator::TPlaneList::iterator itFindPlane =
            mScene->getSimulator()->GetPlanes().find(planeID);
    if (itFindPlane != mScene->getSimulator()->GetPlanes().end())
    {
        (*itFindPlane)->SetCustomStunt(CustomStunt());

        if(mIndexCache.contains(TIndexCacheKey(planeID, sPath)))
        {
            lRoute = mIndexCache[TIndexCacheKey(planeID, sPath)];
        }
        else
        {
            lRoute = computeTakeOffPathPoints(sPath, planeID, sParking);
        }

        if(lRoute.count())
        {
            CCoord planeCoord = (*itFindPlane)->GetCoord();
            planeCoord.Center(mScene->GetCentralPoint()->GetCoord());
            (*itFindPlane)->ChangeImmedAzimuth(tva::GetAzimuth(planeCoord, lRoute.first()->coord()));

            createStunts(itFindPlane.value(), lRoute, false);
        }
        return toRoutePoints(lRoute);
    }
    return route;
}

GroundSceneLogic::TPointList GroundSceneLogic::computeTakeOffPathPoints(QString sPath, quint16 planeID, QString sParking)
{
    TPointList lRoute;
    QStringList taxis = sPath.split(QChar(' '), QString::SkipEmptyParts);

    if(taxis.isEmpty())
        return lRoute;

    CSimulator::TPlaneList::iterator itFindPlane =
            mScene->getSimulator()->GetPlanes().find(planeID);

    if (itFindPlane != mScene->getSimulator()->GetPlanes().end())
    {
        (*itFindPlane)->SetCustomStunt(CustomStunt());

        if(mIndexCache.contains(TIndexCacheKey(planeID, sPath)))
        {
            return mIndexCache[TIndexCacheKey(planeID, sPath)];
        }

        TPointList buf = getParkingApproachPoints(sParking);
        if(buf.isEmpty())
            return lRoute;

        //ACHTUNG!!! Temporary fix for selecting parking approach
        while(buf.size() > 1)
            buf.pop_front();

        lRoute += buf;  //get off parking spot

        QString currentTaxiway;
//        QString currentTaxiway = taxis.first();
//        buf = computePathToTaxiway(buf.last()->index() , taxis.first());
//        lRoute += buf;  //a way to enter taxiways through apron
//        taxis.pop_front();

        while(!taxis.isEmpty())
        {
            buf.clear();
            if(taxis.size() == 1)
            {// last point or runway
                int idx = findTaxiwayIntersection(taxis.first(), currentTaxiway)->index();
                if(idx != -1)
                {// crossroads
                    int idx = findTaxiwayIntersection(taxis.first(), currentTaxiway)->index();
                    buf = computePath(lRoute.last()->index() , idx, true);
                    lRoute += buf;
                }
                else
                {//runway
                    QString rwName = taxis.first().remove("RWY", Qt::CaseInsensitive);
                    TStartList::const_iterator itStart = mScene->getStarts().begin();
                    for(; itStart != mScene->getStarts().end(); itStart++)
                    {
                        if( (*itStart)->name() == rwName)
                        {
                            idx = (*itStart)->pointIndex();
                            if(idx != -1)
                            {
                                int last = lRoute.last()->index();
                                buf = computePathToTaxiway(idx , currentTaxiway, true); // reverse path from start to taxiway end
                                TPointList buf2 = computePath(last , buf.first()->index(), false);    //path from last point to the end of a taxiway
                                lRoute += buf2;
                                buf.pop_front();    // remove duplicate point

                                lRoute += buf;  // trailing path from HOLD_SHORT to starting point

                                lRoute.append(mScene->getPoints()[idx]);

                                QSharedPointer<GroundPoint> lastPoint = getTakeoffSupportPoint(mScene->getPoints()[idx]->coord(), (*itStart)->name());
                                lRoute.append(divideLine(mScene->getPoints()[idx]->coord(),
                                                         lastPoint->coord(), 0.75));

                                lRoute.append(lastPoint);
                            }                            
                        }
                    }
                }
            }
            else
            {                
                bool bOk(false);
                int nPnt = taxis.first().toInt(&bOk);
                if(bOk)
                {
                    buf.append(computePath(lRoute.last()->index() ,
                                           nPnt,
                                           false));
                    currentTaxiway = "";
                }
                else if (currentTaxiway.isEmpty())
                {
                    quint16 idx = lRoute.last()->index();
                    buf.append(computePathToTaxiway(idx , taxis.first()));
                    currentTaxiway = taxis.first();
                }
                else
                {
                    int idx = findTaxiwayIntersection(taxis.first(), currentTaxiway)->index();

                    if(idx == -1)   //there is a blank space between this taxiways
                    {
                        buf.append(computePathToTaxiway(lRoute.last()->index() , taxis.first()));
                    }
                    else
                    {
                        buf.append(computePath(lRoute.last()->index() , idx, true));
                    }
                    currentTaxiway = taxis.first();
                }

                lRoute += buf;
                buf.clear();
            }
            taxis.pop_front();
        }

        mIndexCache.insert(TIndexCacheKey(planeID, sPath), lRoute );
    }
    else
    {
        LOG_STR(CLog::enError, QString("No such plane"));
    }
    qDebug() << "Take off 2 result:";
    foreach(QSharedPointer<GroundPoint> pPoint, lRoute)
    {
        qDebug() << " --> " << pPoint->index();
    }

    return lRoute;
}

QStringList GroundSceneLogic::getTakeoffIndexes(const QString &runway)
{
    QStringList result;    
    bool bOk(false);
    QString rw;
    runway.toInt(&bOk);

    if(bOk)
        rw = runway;
    else
        rw = runway.mid(2,2);

    foreach(TaxiIndex index, mTakeoffIndexes.values( rw ))
        result.append(index.sName);
    return result;
}

QStringList GroundSceneLogic::getLandingIndexes(quint16 idPlane)
{
    QStringList result;
    QSharedPointer<GroundRunway> pRunway = airplaneOnRunway(idPlane);
    if(pRunway)
    {
        QString rw = pRunway->name();
        double azimDiff = tva::GetAzimuthDif(mScene->GetAirplanes()[idPlane]->GetAzimuth(), pRunway->azimuth());
        if(qAbs(azimDiff) > 90.0)
            rw = reverseDesignator(rw);

        foreach(TaxiIndex index, mLandingIndexes.values(rw))
            result.append(index.sName);
    }
    return result;
}

QString GroundSceneLogic::getLandingRoute(quint16 idPlane, QString sIndex)
{
    QSharedPointer<GroundRunway> pRunway = airplaneOnRunway(idPlane);
    if(pRunway)
    {
        foreach(TaxiIndex index, mLandingIndexes.values(pRunway->name()))
        {
            if(index.sName == sIndex)
                return index.sPath;
        }
    }
    return QString();
}

QString GroundSceneLogic::getTakeoffRoute(QString sIndex)
{
    foreach(TaxiIndex index, mTakeoffIndexes.values())
    {
        if(index.sName == sIndex)
            return index.sPath;
    }
    return QString();
}

void GroundSceneLogic::addIndex(const QString& sRunway, QString sName, QString sPath)
{
    TaxiIndex index;
    index.sName = sName;
    index.sRunway = sRunway;
    index.sPath = sPath;

    if(sRunway.toInt() != 0)
    {
        mTakeoffIndexes.insert(sRunway, index);
    }
    else
        mLandingIndexes.insert(sRunway, index);
}

void GroundSceneLogic::clearIndexCache()
{
    mIndexCache.clear();
}

void GroundSceneLogic::onPlaneLanding(QSharedPointer<CAirplane> airplane)
{
    handleLanding(airplane);
}

QSharedPointer<GroundPoint> GroundSceneLogic::divideLine(const CCoord &first, CCoord second, double ratio) const
{
    second.Center(mScene->GetCentralPoint()->GetCoord());
    QLineF line(first.GetX(),
                first.GetY(),
                second.GetX(),
                second.GetY());
    line.setLength(line.length() * ratio);
    CCoord coord;
    coord.Center(mScene->GetCentralPoint()->GetCoord());
    coord.SetX(line.x2());
    coord.SetY(line.y2());
    coord.DeCenter(mScene->GetCentralPoint()->GetCoord());

    QSharedPointer<GroundPoint> pnt = QSharedPointer<GroundPoint>(new GroundPoint);
    pnt->setCoord(coord);
    return pnt;
}

int GroundSceneLogic::getAirplaneNearestPoint(quint32 airID)
{
    int idxPlane = -1;
    CGUIAirplane* plane = dynamic_cast<CGUIAirplane*>(mScene->GetAirplanes()[airID].data());
    if(plane)
    {
        // if moving by a stunt
        if(plane->GetCustomStunt().isCustomStunt() && plane->GetCustomStunt().GetType() != CustomStunt::StuntPoint)
        {
            QSharedPointer<atd::CRoutePoint> pCurPoint = plane->GetCustomStunt().currentPoint();
            if(pCurPoint)
                idxPlane = findPoint(pCurPoint->GetPoint()->GetCoord(), true);
            else
                idxPlane = findPoint(plane->GetCustomStunt().GetPointList().last()->GetPoint()->GetCoord(), false, 30);
        }
        //else
        if(idxPlane == -1)
        {// check if standing on a taxiway
            foreach(QSharedPointer<GroundPath> pPath, mScene->getPaths())
            {
                GUIGroundPath* path = dynamic_cast<GUIGroundPath*>(pPath.data());

                if(path && path->contains(plane->getPlanePosition()) && path->GroundPath::type() != GroundPath::Parking)
                {// plane is on this taxiway
                    if(tva::GetAzimuthDif(plane->GetAzimuth(), tva::GetAzimuth(path->startPointCoord(), path->endPointCoord())) < 90)
                        idxPlane = path->endPointIndex();
                    else
                        idxPlane = path->startPointIndex();
                    break;
                }
            }
        }
    }
    if(idxPlane == -1)
    {
        CCoord coord(plane->GetCoord());
        coord.Center(mScene->GetCentralPoint()->GetCoord());

        idxPlane = findPoint(coord, false);
    }
    return idxPlane;
}

QString GroundSceneLogic::reverseCourse(const QString &course) const
{
    if(course.length() != 2)
        return QString("");

    int number = 0;
    number = course.left(2).toInt();

    return reverseCourse(number);
}

QString GroundSceneLogic::reverseCourse(int course) const
{
    course += 18;
    if(course >= 36)
        course -= 36;
    return QString("%1").arg(course, 2, 10, QLatin1Char('0'));
}

QSharedPointer<GroundPoint> GroundSceneLogic::getTakeoffSupportPoint(quint16 idPlane, QString sRunway)
{
    return getTakeoffSupportPoint(mScene->GetAirplanes()[idPlane]->GetCoord(), sRunway);
}

QSharedPointer<GroundPoint> GroundSceneLogic::getTakeoffSupportPoint(CCoord startCoord, QString sRunway)
{
    CCoord result;
    QLineF lnRunway;

    TStartList::const_iterator itStart = mScene->getStarts().begin();
    QString reverse = reverseDesignator(sRunway);
    CCoord cStart;
    CCoord cEnd;

    {
        TStartList::const_iterator itEnd = mScene->getStarts().end();
        for(; itStart != itEnd; itStart++)
        {
            if( (*itStart)->name() == sRunway)
            {
                if ( ((*itStart)->pointIndex() >= 0) && ((*itStart)->pointIndex() < mScene->getPoints().size()) )
                    cStart = mScene->getPoints()[(*itStart)->pointIndex()]->coord();
            }
            if( (*itStart)->name() == reverse)
            {
                if ( ((*itStart)->pointIndex() >= 0) && ((*itStart)->pointIndex() < mScene->getPoints().size()) )
                    cEnd = mScene->getPoints()[(*itStart)->pointIndex()]->coord();
            }
        }
    }

    cStart.Center(stuntCenter());
    cEnd.Center(stuntCenter());
    lnRunway.setP1(QPointF(cStart.GetX(),
                            cStart.GetY()));
    lnRunway.setP2(QPointF(cEnd.GetX(),
                           cEnd.GetY()));

    startCoord.Center(stuntCenter());
    QLineF parallelLine(startCoord.GetX(), startCoord.GetY(),
                        startCoord.GetX() - 100, startCoord.GetY() - 100);
    parallelLine.setAngle(lnRunway.angle());
    parallelLine.setLength(100);    // 100 meter place for turning
    parallelLine.setPoints(parallelLine.p2(), parallelLine.p1());

    QPointF intersect;
    if(lnRunway.intersect(parallelLine.normalVector(), &intersect) != QLineF::NoIntersection)
    {
        result.Center(stuntCenter());

        result.SetX(intersect.x());
        result.SetY(intersect.y());
        result.DeCenter(stuntCenter() );
    }
    QSharedPointer<GroundPoint> pSupPoint(new GroundPoint);
    pSupPoint->setCoord(result);
    return pSupPoint;
}

QSharedPointer<GroundRunway> GroundSceneLogic::airplaneOnRunway(quint16 idPlane)
{
    QSharedPointer<GroundRunway> pRunway;
    if(mScene->GetAirplanes().contains(idPlane))
    {
        CGUIAirplane* airplane = dynamic_cast<CGUIAirplane*>(mScene->GetAirplanes()[idPlane].data());
        foreach(QSharedPointer<GroundRunway> pRw, mScene->getRunways())
        {
            GUIGroundRunway* runway = dynamic_cast<GUIGroundRunway*>(pRw.data());
            if(runway && runway->contains(airplane->getPlanePosition()))
            {// plane is on this runway
                return pRw;
            }
        }
    }
    return pRunway;
}

void GroundSceneLogic::pointHoverEnter(IGroundItemGUI* item, int idx)
{
    int idxPlane = findPoint(item->coordinate(), false);

    if(dynamic_cast<CAirplane*>(item))
        idxPlane = getAirplaneNearestPoint(dynamic_cast<CAirplane*>(item)->GetID());
    TPointList route;
    route.append(mScene->getPoints()[idxPlane]);
    route.append(computePath(idxPlane, idx));
    item->setPathPoints( route );
    item->ShowPathPoints(true);
}

void GroundSceneLogic::pointHoverLeave(IGroundItemGUI* item, int idx)
{
    Q_UNUSED(idx)
    item->clearPathPoints();
    item->ShowPathPoints(false);
}

void GroundSceneLogic::standHoverEnter(IGroundItemGUI* item, QString name)
{
    int idxPlane = findPoint(item->coordinate(), false);
    if(dynamic_cast<CAirplane*>(item))
        idxPlane = getAirplaneNearestPoint(dynamic_cast<CAirplane*>(item)->GetID());
    item->setPathPoints(computePathToParking(idxPlane, name));
    item->ShowPathPoints(true);
}

void GroundSceneLogic::standHoverLeave(IGroundItemGUI* item, QString name)
{
    Q_UNUSED(name)
    item->clearPathPoints();
    item->ShowPathPoints(false);
}

QSharedPointer<GroundRunway> GroundSceneLogic::pointOnRunway(quint16 idPoint)
{
    QSharedPointer<GroundRunway> pRunway;
    if(mScene->getPoints().contains(idPoint))
    {
        GUIGroundPoint* point = dynamic_cast<GUIGroundPoint*>(mScene->getPoints()[idPoint].data());
        foreach(QSharedPointer<GroundRunway> pRw, mScene->getRunways())
        {
            GUIGroundRunway* runway = dynamic_cast<GUIGroundRunway*>(pRw.data());
            if(runway && runway->contains(point->pos()))
            {// point is on this runway
                return pRw;
            }
        }
    }
    return pRunway;
}

void GroundSceneLogic::markParkingStunt(QSharedPointer<CPlane> &pPlane, const QString &stand) const
{
    CustomStunt* stunt = pPlane->getLastCustomStunt();
    if(stunt->GetType() != CustomStunt::None)
        stunt->setProperty("stand", QVariant(stand));
    else
        delete stunt;
}

void GroundSceneLogic::handleLanding(QSharedPointer<CAirplane> &pAirplane)
{
    //find on which runway this plane is
    if(pAirplane &&
            (!pAirplane->GetCustomStunt().isCustomStunt() ||
             pAirplane->GetCustomStunt().GetType() == CustomStunt::StuntSSA) &&
            !pAirplane->hasLanded())
    {
        int idx = -1;
        QSharedPointer<GroundRunway> pRunway;

        if( (pRunway = airplaneOnRunway(pAirplane->GetID())) )
        {// plane is on this runway
            TStartList::const_iterator itStart = mScene->getStarts().begin();
            TStartList::const_iterator itEnd = mScene->getStarts().end();
            QString reverse = reverseDesignator(pRunway->name());
            QSharedPointer<GroundStart> pStart;
            QSharedPointer<GroundStart> pEnd;
            for(; itStart != itEnd; itStart++)
            {
                if( (*itStart)->name() == pRunway->name())
                {
                    pStart = *itStart;
                }
                if( (*itStart)->name() == reverse)
                {
                    pEnd = *itStart;
                }
            }

            //determine which direction it's moving
            if ( pStart )
            {
                idx = pStart->pointIndex();
                if ( pEnd )
                {
                    if(tva::GetDistance(pAirplane->GetCoord(), pStart->coord()) <
                            tva::GetDistance(pAirplane->GetCoord(), pEnd->coord()))
                        idx = pEnd->pointIndex();
                }
            }
        }

        if(idx != -1)
        {//create landing stunt
            pAirplane->setHasLanded(true);
            QSharedPointer<atd::CPoint> point(new atd::CPoint);
            point.data()->SetCoord(mScene->getPoints()[idx]->coord());
            point.data()->SetName(mScene->getPoints()[idx]->coord().GetAsString());
            pAirplane->ChangeNextPoint(point, true);
            //pAirplane->makeStop();
        }
    }
}

QVector<QSharedPointer<atd::CRoutePoint> > GroundSceneLogic::parseLandingPath(QString sPath, quint16 planeID)
{
    TPointList lRoute;
    QVector<QSharedPointer<atd::CRoutePoint> > route;
    CSimulator::TPlaneList::iterator itFindPlane =
            mScene->getSimulator()->GetPlanes().find(planeID);
    if (itFindPlane != mScene->getSimulator()->GetPlanes().end())
    {
        (*itFindPlane)->SetCustomStunt(CustomStunt());

        if(mIndexCache.contains(TIndexCacheKey(planeID, sPath)))
        {
            lRoute = mIndexCache[TIndexCacheKey(planeID, sPath)];
        }
        else
        {
            lRoute = computeLandingPathPoints(sPath, planeID);
        }

        createStunts(itFindPlane.value(), lRoute, false);
        return toRoutePoints(lRoute);
    }
    return route;
}

GroundScene::TPointList GroundSceneLogic::computeLandingPathPoints(QString sPath, quint16 planeID)
{
    TPointList lRoute;
    QStringList taxis = sPath.split(QChar(' '), QString::SkipEmptyParts);

    if(taxis.isEmpty())
        return lRoute;

    CSimulator::TPlaneList::iterator itFindPlane =
            mScene->getSimulator()->GetPlanes().find(planeID);
    if (itFindPlane != mScene->getSimulator()->GetPlanes().end())
    {
        if(mIndexCache.contains(TIndexCacheKey(planeID, sPath)))
        {
            return mIndexCache[TIndexCacheKey(planeID, sPath)];
        }

        int startIndex = getPlaneNextPointIndex(planeID);

        if(startIndex == -1)
        {
            startIndex = findPoint((*itFindPlane)->GetCoord(), false);
        }

        QString currentTaxiway = "";
        TPointList buf;
        buf.append(mScene->getPoints()[startIndex]);

        bool bFirst(true);
        while(!taxis.isEmpty())
        {
            bool bOk(false);
            int nPnt = taxis.first().toInt(&bOk);
            if(bOk)
            {
                buf.append(computePath(lRoute.isEmpty()? startIndex : lRoute.last()->index() ,
                                       nPnt,
                                       false));
                currentTaxiway = "";
            }
            else if (currentTaxiway.isEmpty())
            {
                quint16 idx = lRoute.isEmpty()? startIndex : lRoute.last()->index();
                buf.append(computePathToTaxiway(idx , taxis.first()));
                currentTaxiway = taxis.first();
            }
            else
            {
                int idx = findTaxiwayIntersection(taxis.first(), currentTaxiway)->index();

                if(idx == -1)   //there is a blank space between this taxiways
                {
                    buf.append(computePathToTaxiway(lRoute.last()->index() , taxis.first()));
                }
                else
                {
                    buf.append(computePath(lRoute.last()->index() , idx, true));
                }
                currentTaxiway = taxis.first();
            }

            if(buf.count() > 1 && bFirst)
            {// checking if first 2 points are opposite to airplane position
                bFirst = false;
                CCoord planeCoord = (*itFindPlane)->GetCoord();
                planeCoord.Center(mScene->GetCentralPoint()->GetCoord());
                double azim1 = tva::GetAzimuth( planeCoord, buf.at(0)->coord());
                double azim2 = tva::GetAzimuth( buf.at(0)->coord(), buf.at(1)->coord());

                if(tva::GetAzimuthDif(
                            azim1,
                            azim2) >= M_PI*150/180)
                {
                    buf.pop_front();
                }
            }

            lRoute += buf;
            buf.clear();
            taxis.pop_front();
        }

        qDebug() << "Land result:";
        foreach(QSharedPointer<GroundPoint> pPoint, lRoute)
        {
            qDebug() << " --> " << pPoint->index();
        }
        mIndexCache.insert(TIndexCacheKey(planeID, sPath), lRoute );
    }
    else
    {
        LOG_STR(CLog::enError, QString("No such plane"));
    }
    return lRoute;
}

bool GroundSceneLogic::parseCustomPath(QSharedPointer<CPlane>& pPlane, QSharedPointer<atd::CPoint> &pPoint, bool isLanding)
{
    if(pPlane)
    {
        qint16 idxPlane = -1;  //nearest to plane point index
        qint16 idxPoint = -1;  //point index which was clicked

        idxPlane = getAirplaneNearestPoint(pPlane->GetID());

        pPoint->GetCoord().Center(mScene->GetCentralPoint()->GetCoord());
        idxPoint = findPoint(pPoint->GetCoord() , false, ground_point::HOVER_RADIUS);

        if(idxPoint != -1)
        {// if there was a point clicked            
            TPointList buf = computePath(idxPlane ,mScene->getPoints()[idxPoint]->index() , false);
            buf.prepend(mScene->getPoints()[idxPlane]);

            if(buf.count() > 1)
            {
                CCoord planeCoord = pPlane->GetCoord();
                planeCoord.Center(mScene->GetCentralPoint()->GetCoord());
                double azim1 = tva::GetAzimuth( planeCoord, buf.at(0)->coord());
                double azim2 = tva::GetAzimuth( buf.at(0)->coord(), buf.at(1)->coord());

                if(tva::GetAzimuthDif(
                            azim1,
                            azim2) >= M_PI*150/180)
                {
                    buf.pop_front();
                }
            }
            if(isLanding && buf.size() > 2)
            {
                CCoord planeCoord = pPlane->GetCoord();
                planeCoord.Center(mScene->GetCentralPoint()->GetCoord());
                while(tva::GetDistance(planeCoord, buf.first()->coord()) <= 200)
                    buf.pop_front();
            }

            createStunts(pPlane, buf, false);
            return true;
        }
        else
        {// it wasn't a point
            //check if it was a taxiway
            // or check if it was a stand
            QLineF pointCheck;
            pointCheck.setP1( QPointF(pPoint->GetCoord().GetX(),
                                      pPoint->GetCoord().GetY()));

            TParkingMap::const_iterator itStand = mScene->getParkings().begin();
            TParkingMap::const_iterator itEnd = mScene->getParkings().end();
            for(; itStand != itEnd; itStand++)
            {
                pointCheck.setP2(QPointF((*itStand)->coord().GetX(),
                                         (*itStand)->coord().GetY()));

                if(pointCheck.length() <= ground_parking::HOVER_RADIUS)
                    break;  // assume this stand was clicked
            }
            if(itStand != mScene->getParkings().end())
            {//so this is stand
                TPointList buf = computePathToParking(idxPlane, (*itStand)->name());
                //buf.prepend(mScene->getPoints()[idxPlane]);
                if(isLanding && buf.size() > 2)
                {
                    CCoord planeCoord = pPlane->GetCoord();
                    planeCoord.Center(mScene->GetCentralPoint()->GetCoord());
                    while(tva::GetDistance(planeCoord, buf.first()->coord()) <= 200)
                        buf.pop_front();
                }

                createStunts(pPlane, buf, false);

                markParkingStunt(pPlane, (*itStand)->name());

                return true;
            }
            //else - this is a simple point
            return false;
        }
    }

    return false;
}

void GroundSceneLogic::takeoff(QSharedPointer<CPlane> &pPlane)
{
    if(pPlane)
    {
        // find first point of a route
        qint16 idxPlane = -1;  //nearest to plane point index

        // if moving by a stunt
        if(pPlane->GetCustomStunt().isCustomStunt() && pPlane->GetCustomStunt().GetType() != CustomStunt::StuntPoint)
        {
            QSharedPointer<atd::CRoutePoint> pCurPoint = pPlane->GetCustomStunt().currentPoint();
            idxPlane = findPoint(pCurPoint->GetPoint()->GetCoord(), true);
        }
        else
        {// check if standing on a taxiway
            CGUIAirplane* plane = dynamic_cast<CGUIAirplane*>(mScene->GetAirplanes()[pPlane->GetID()].data());
            if(plane)
            {
                foreach(QSharedPointer<GroundPath> pPath, mScene->getPaths())
                {
                    GUIGroundPath* path = dynamic_cast<GUIGroundPath*>(pPath.data());

                    if(path && path->contains(plane->getPlanePosition()) && path->GroundPath::type() != GroundPath::Parking)
                    {// plane is on this taxiway
                        if(tva::GetAzimuthDif(pPlane->GetAzimuth(), tva::GetAzimuth(path->startPointCoord(), path->endPointCoord())) < 90)
                            idxPlane = path->endPointIndex();
                        else
                            idxPlane = path->startPointIndex();
                        break;
                    }
                }
            }
        }

        //
        if(idxPlane == -1)
        {
            CCoord coord = pPlane->GetCoord();
            coord.Center(mScene->GetCentralPoint()->GetCoord());

            idxPlane = findPoint(coord, false);
        }

        // find runway ending point
        QSharedPointer<GroundRunway> runway = airplaneOnRunway(pPlane->GetID());
        if(runway )
        {// plane is on this runway
            QString startDesignator = runway->name();
            QString reverse = reverseDesignator(startDesignator);

            TStartList::const_iterator itStart = mScene->getStarts().begin();
            TStartList::const_iterator itEnd = mScene->getStarts().end();

            TStartList::const_iterator itForward = itEnd;
            TStartList::const_iterator itReverse = itEnd;
            for(; itStart != itEnd; itStart++)
            {// check if this is a starting point
                if(startDesignator == (*itStart)->name())
                {
                    itForward = itStart;
                }
                if(reverse == (*itStart)->name())
                {
                    itReverse = itStart;
                }
            }

            if( (itForward == itEnd) || (itReverse == itEnd) )
                return;

            //! check for direction
            QString sSID = pPlane->GetChain()->GetFPLs().first()->GetRoute().split(QChar(' ')).first(); // SID name
            CCoord sidCoord;    // first SID point coords
            int pointLLCount = pPlane->GetChain()->GetPointLL()[0].count();
            for(int ii = 0; ii < pointLLCount; ii++)
            {
                if(pPlane->GetChain()->GetPointLL()[0].at(ii)->GetProcedureName() == sSID)
                {
                    sidCoord = pPlane->GetChain()->GetPointLL()[0].at(ii+2)->GetPoint()->GetCoord();
                    break;
                }
            }
            CCoord planeCoord = mScene->GetAirplanes()[pPlane->GetID()]->GetCoord();
            CCoord rwCoord = (*itForward)->coord();
            sidCoord.Center(stuntCenter());
            planeCoord.Center(stuntCenter());
            rwCoord.Center(stuntCenter());
            double startDistance = tva::GetDistance(planeCoord, sidCoord) - 50;
            double endDistance = tva::GetDistance(rwCoord, sidCoord);
            //!
            TPointList buf;
            if(endDistance < startDistance) // if far end of a runway is nearest to the SID's 1st point
            {
                buf.append(getTakeoffSupportPoint(pPlane->GetID(), reverseDesignator( (*itForward)->name())));
                if ( ((*itForward)->pointIndex() >= 0) && ((*itForward)->pointIndex() < mScene->getPoints().size()) )
                    buf.append(mScene->getPoints()[(*itForward)->pointIndex()]);
            }
            else
            {
                buf.append(getTakeoffSupportPoint(pPlane->GetID(), reverseDesignator( (*itReverse)->name())));
                if ( ((*itReverse)->pointIndex() >= 0) && ((*itReverse)->pointIndex() < mScene->getPoints().size()) )
                    buf.append(mScene->getPoints()[(*itReverse)->pointIndex()]);
            }

            createStunts(pPlane, buf, true);
        }
    }
}

GroundSceneLogic::TPointList GroundSceneLogic::computePath(quint16 from, quint16 to, bool bKeepTaxiway)
{
    TPointList result;
    qDebug() << "Computing path from " << from << " to " << to;

    if(from == to)
        return TPointList() << mScene->getPoints()[to];

    // Dijkstra’s algorithm
    TPointMap points;   // Points to search in
    if(bKeepTaxiway)
    {
        qDebug() << "Taxiway";
        foreach(QString taxiway, mTaxiways)
        {
            TPointList taxiPoints = getPointsByTaxiName(taxiway);
            if(taxiPoints.contains(mScene->getPoints()[from]) && taxiPoints.contains(mScene->getPoints()[to]))
            {
                TPointList::iterator itPnt = taxiPoints.begin();
                TPointList::iterator itEnd = taxiPoints.end();
                for(; itPnt != itEnd; itPnt++)
                    points.insert((*itPnt)->index(), *itPnt);
            }
        }
    }
    else
    {
       points = mScene->getPoints();
    }
    TPathList paths  = mScene->getPaths();  //E //path
    QVector<int> visited;           //U //array of visited points
    QMap<int, double> pathLength;   //d //current calculated path length
    QMap<int, int> pathPoints;      //p //array of previous points

    double INF = 0xffffff;

    //init
    pathLength[from] = 0;
    foreach(QSharedPointer<GroundPoint> p, points)
    {
        if(p->index() != from)
            pathLength[p->index()] = INF;
        pathPoints[p->index()] = -1;
    }
    pathPoints[from] = from;

    //main
    QSharedPointer<GroundPoint> v;
    while( !(v = getNextSearchPoint(visited, pathLength, points)).isNull() )
    {
        if(v->index() == -1)
            break;

        visited.append(v->index());
        QSharedPointer<GroundPoint > u;
        foreach(QSharedPointer<GroundPath> p, paths)
        {
            if( p->type() != GroundPath::Parking)
            {
                bool bIsStart(false);
                bool bIsEnd(false);

                // TODO: insert traffic direction control here
                if( p->startPointIndex() == v->index() &&
                        !visited.contains(p->endPointIndex()) &&
                        points.contains(p->endPointIndex()))
                    bIsStart = true;    // point "v" is an starting point for path "p"

                if( p->endPointIndex() == v->index() &&
                        !visited.contains(p->startPointIndex()) &&
                        points.contains(p->startPointIndex()))
                    bIsEnd = true;    // point "v" is an ending point for path "p"

                if(bIsStart || bIsEnd)
                {
                    u = p->startPointIndex() == v->index()? points[p->endPointIndex()] : points[p->startPointIndex()];
                    double distance = tva::GetDistance(v->coord(), u->coord());
                    if(pathLength[u->index()] > pathLength[v->index()] + distance)
                    {
                        pathLength[u->index()] = pathLength[v->index()] + distance;
                        pathPoints[u->index()] = v->index();
                    }
                }
            }
        }

        if(v->index() == to)
            break;  // we'v found specified path
    }

    // form a path
    int current = to;
    qDebug() << "Result:";
    while(current != from && current >= 0)
    {
        qDebug() << current;
        result.prepend(mScene->getPoints()[current]);

        if(pathPoints.contains(current))
            current = pathPoints[current];
        else
            current = -1;
    }
    result.prepend(mScene->getPoints()[current]);

    return result;
}

GroundSceneLogic::TPointList GroundSceneLogic::computePathToParking(quint16 fromPoint, QString toParking)
{
    TPointList result;
    qDebug() << "Computing path from " << fromPoint << " to parking #" << toParking;

    // Dijkstra’s algorithm
    TPointMap points = mScene->getPoints();   // Points to search in
    TPathList paths  = mScene->getPaths();  //E //path
    QVector<int> visited;           //U //array of visited points
    QMap<int, double> pathLength;   //d //current calculated path length
    QMap<int, int> pathPoints;      //p //array of previous points

    double INF = 0xffffff;

    //init
    pathLength[fromPoint] = 0;

    QSharedPointer<GroundParking> pStand = mScene->getParkings()[toParking];
    if(!pStand)
        return result;

    int nStandIdx = pStand->number()*1000 + pStand->index();
    QSharedPointer<GroundPoint> pStandPoint(new GroundPoint);
    pStandPoint->setCoord(pStand->coord());
    pStandPoint->setType(GroundPoint::Normal);
    pStandPoint->setIndex(nStandIdx);//pStand->getIndex());
    points.insert(nStandIdx, pStandPoint);

    foreach(QSharedPointer<GroundPoint> p, points)
    {
        if(p->index() != fromPoint)
            pathLength[p->index()] = INF;
        pathPoints[p->index()] = -1;
    }
    pathPoints[fromPoint] = fromPoint;

    //main
    QSharedPointer<GroundPoint> v;
    while( !(v = getNextSearchPoint(visited, pathLength, points)).isNull() )
    {
        if(v->index() == -1)
            break;

        visited.append(v->index());
        QSharedPointer<GroundPoint > u;
        foreach(QSharedPointer<GroundPath> p, paths)
        {
            bool bIsStart(false);
            bool bIsEnd(false);

            int nEnd = p->endPointIndex();
            if(p->type() == GroundPath::Parking)
            {
                if(p->endPointIndex() == pStand->index())
                    nEnd = nStandIdx;
                else
                    continue;
            }

            // TODO: insert traffic direction control here
            if( p->startPointIndex() == v->index() &&
                    !visited.contains(nEnd) &&
                    points.contains(nEnd))
                bIsStart = true;    // point "v" is an starting point for path "p"

            if( nEnd == v->index() &&
                    !visited.contains(p->startPointIndex()) &&
                    points.contains(p->startPointIndex()))
                bIsEnd = true;    // point "v" is an ending point for path "p"

            if(bIsStart || bIsEnd)
            {
                u = bIsStart ? points[nEnd] : points[p->startPointIndex()];
                double distance = tva::GetDistance(v->coord(), u->coord());
                if(pathLength[u->index()] > pathLength[v->index()] + distance)
                {
                    pathLength[u->index()] = pathLength[v->index()] + distance;
                    pathPoints[u->index()] = v->index();
                }
            }
        }
    }

    // form a path
    qDebug() << "Result:";
    int lastPoint = nStandIdx;
    while(lastPoint != fromPoint && lastPoint >= 0)
    {
        qDebug() << lastPoint;
        result.prepend(points[lastPoint]);
        lastPoint = pathPoints[lastPoint];
    }
    result.prepend(mScene->getPoints()[fromPoint]);

    return result;
}

QSharedPointer<GroundPoint> GroundSceneLogic::getNextSearchPoint(QVector<int> visited, QMap<int, double> pathLength, const TPointMap& points)
{
    QSharedPointer<GroundPoint> result;

    QMap<int, double>::iterator itLen = pathLength.begin();
    QMap<int, double>::iterator itend = pathLength.end();
    double dMin = 0xffffff;
    for(; itLen != itend; itLen++)
    {
        if(dMin > itLen.value() && !visited.contains(itLen.key()))
        {
            dMin = itLen.value();
            result = points[itLen.key()];
        }
    }

    return result;
}

QSharedPointer<GroundPoint> GroundSceneLogic::findTaxiwayIntersection(QString first, QString second)
{
    TPointList fst = getPointsByTaxiName(first);
    TPointList snd = getPointsByTaxiName(second);
    QSharedPointer<GroundPoint> res(new GroundPoint);

    qDebug() << "Intersection of " << first << " and " << second;
    if(!fst.isEmpty() && !snd.isEmpty())
    {
        foreach(QSharedPointer<GroundPoint> p, fst)
        {
            if(snd.contains(p))
            {
                res = p;
                qDebug() << p->index();
                break;  // ACHTUNG!!! There migth be multiple points when intersecting with "blank" taxiway designator!
            }
        }
    }

    return res;
}

GroundSceneLogic::TPointList GroundSceneLogic::getParkingApproachPoints(QString sParking)
{
    TPointList result;

    qDebug() << "Parking approaches for " << sParking;
    QSharedPointer<GroundParking> pParking = mScene->getParkings()[sParking];
    if(pParking)
    {
        foreach(QSharedPointer<GroundPath> p, mScene->getPaths())
        {
            if(p->type() == GroundPath::Parking &&
                     p->endPointIndex() == pParking->index() )
            {
                //this is this path!
                result.append(mScene->getPoints()[p->startPointIndex()]);
                qDebug() << result.last()->index();
            }
        }
    }

    return result;
}

GroundSceneLogic::TPointList GroundSceneLogic::computePathToTaxiway(quint16 from, QString to, bool bReverse)
{
    TPointList result;
    qDebug() << "Computing path from " << from << " to taxiway " << to;

    // Dijkstra’s algorithm
    TPointMap points = mScene->getPoints(); // Points to search in
    TPathList paths  = mScene->getPaths();  //E //path
    QVector<int> visited;           //U //array of visited points
    QMap<int, double> pathLength;   //d //current calculated path length
    QMap<int, int> pathPoints;      //p //array of previous points

    double INF = 0xffffff;

    //init
    pathLength[from] = 0;
    foreach(QSharedPointer<GroundPoint> p, points)
    {
        if(p->index() != from)
            pathLength[p->index()] = INF;
        pathPoints[p->index()] = -1;
    }
    pathPoints[from] = from;

    //main
    bool bEnd(false);
    QSharedPointer<GroundPoint> v;
    int iEnd = -1;
    while( !(v = getNextSearchPoint(visited, pathLength, points)).isNull()  && !bEnd)
    {
        if(v->index() == -1)
            break;

        visited.append(v->index());
        QSharedPointer<GroundPoint > u;
        foreach(QSharedPointer<GroundPath> p, paths)
        {
            if( p->type() != GroundPath::Parking)
            {
                bool bIsStart(false);
                bool bIsEnd(false);

                // TODO: insert traffic direction control here
                if( p->startPointIndex() == v->index() &&
                        !visited.contains(p->endPointIndex()) &&
                        points.contains(p->endPointIndex()))
                    bIsStart = true;    // point "v" is an starting point for path "p"

                if( p->endPointIndex() == v->index() &&
                        !visited.contains(p->startPointIndex()) &&
                        points.contains(p->startPointIndex()))
                    bIsEnd = true;    // point "v" is an ending point for path "p"

                if(bIsStart || bIsEnd)
                {
                    u = p->startPointIndex() == v->index()? points[p->endPointIndex()] : points[p->startPointIndex()];
                    double distance = tva::GetDistance(v->coord(), u->coord());
                    if(pathLength[u->index()] > pathLength[v->index()] + distance)
                    {
                        pathLength[u->index()] = pathLength[v->index()] + distance;
                        pathPoints[u->index()] = v->index();
                    }

                    if(p->name() == to)
                    {
                        bEnd = true;
                        iEnd = v->index();
                        break;
                    }
                }
            }
        }
    }

    // form a path
    int current = iEnd;
    qDebug() << "Result:";
    while(current != from && current >= 0)
    {
        qDebug() << current;
        if(bReverse)
            result.append(mScene->getPoints()[current]);
        else
            result.prepend(mScene->getPoints()[current]);

        if(pathPoints.contains(current))
            current = pathPoints[current];
        else
            current = -1;
    }
    if(bReverse)
    {
        result.append(mScene->getPoints()[from]);
    }

    return result;
}

int GroundSceneLogic::getPlaneNextPointIndex(quint16 planeID)
{
    CSimulator::TPlaneList::iterator itFindPlane =
            mScene->getSimulator()->GetPlanes().find(planeID);
    if (itFindPlane != mScene->getSimulator()->GetPlanes().end())
    {
        QSharedPointer<atd::CRoutePoint> pRoutePoint = (*itFindPlane)->GetCustomStunt().currentPoint();
        if(pRoutePoint)
        {
            foreach(QSharedPointer<GroundPoint> pPoint, mScene->getPoints())
            {
                if(pPoint->coord() == pRoutePoint->GetPoint()->GetCoord())
                    return pPoint->index();
            }
        }
    }
    return -1;
}

QVector<QSharedPointer<atd::CRoutePoint> > GroundSceneLogic::toRoutePoints(GroundScene::TPointList points)
{
    QVector<QSharedPointer<atd::CRoutePoint> > result;

    foreach(QSharedPointer<GroundPoint> pPoint, points)
    {
        QSharedPointer<atd::CRoutePoint> pRoutePoint(new atd::CRoutePoint);
        QSharedPointer<atd::CPoint> pnt(new atd::CPoint);
        pnt->SetCoord(pPoint->coord());
        pnt->GetCoord().Center(stuntCenter());
        pnt->SetName(mScene->name());
        pnt->SetType(atd::CPoint::enAirportPoint);
        pRoutePoint->SetPoint(pnt);
        pRoutePoint->SetCustom(true);
        pRoutePoint->SetLevel(300);
        result.append(pRoutePoint);
    }
    return result;
}

void GroundSceneLogic::createStunts(QSharedPointer<CPlane> &plane, GroundScene::TPointList points, bool bTakeOff)
{
    if(plane.data())
    {
        bool bHoldFlag; // determines if needed to stop on the next HOLD_SHORT
        if(airplaneOnRunway(plane->GetID()))
            bHoldFlag = false;
        else
            bHoldFlag = true;

        int nHold = 0;
        int pointsCount = points.count();
        for(int i = 0; i < pointsCount; ++i)
            if(points[i]->GroundPoint::type() == GroundPoint::HOLD_SHORT)
                nHold++;

        if( !(nHold % 2) )  // since we parse points in reverse, got to reverse switch for even amount of HOLD_SHORTs
            bHoldFlag = !bHoldFlag;

        plane->clearStuntList();
        plane->SetCustomStunt(CustomStunt());

        QVector<QSharedPointer<atd::CRoutePoint> > route = toRoutePoints(points);
        if(bTakeOff)
            route.last()->SetSpeed(plane->getTakeOffIAS());
        else
            route.last()->SetSpeed(0);

        for(int i = 0; i < points.size() - 1; ++i)
        {
            if(points.at(i)->type() == GroundPoint::HOLD_SHORT)
            {
                if(bHoldFlag)   //skip or not this HOLD_SHORT
                {
                    route[i]->SetSpeed(0);  //stop at hold shorts
                }
                bHoldFlag = !bHoldFlag;
            }
        }

        CustomStunt pointStunt;
        pointStunt.SetType(CustomStunt::StuntHolding);
        pointStunt.SetID(QUuid::createUuid());
        pointStunt.SetName(QString("Ground"));
        pointStunt.SetSN(0);
        pointStunt.GetPointList().append(route.toList());//toRoutePoints(current).toList());
        plane->ChangeCustomStunt(pointStunt.toString());
        plane->MarkAimAzimuth(false);
        plane->SetBank(0);
        plane->SetIsChanged(true);
        plane->GetCustomStunt().GetPointList().clear();
        plane->GetCustomStunt().GetPointList().append(route.toList());//toRoutePoints(current).toList());
    }
}

void GroundSceneLogic::setScene(GroundScene *scene)
{
    mScene = scene;
    mScene->setLogic(this);
    connect(&scene->emitter(), SIGNAL(planeLanding(QSharedPointer<CAirplane>)),
            this, SLOT(onPlaneLanding(QSharedPointer<CAirplane>)));
    connect(&scene->emitter(), SIGNAL(destroyed()), this, SLOT(deleteLater()));
}

void GroundSceneLogic::parseVehiclePath(QSharedPointer<Vehicle> vehicle, const CCoord &coord)
{
    int idxVehicle = findPoint(vehicle->GetCoord(), false);
    int idxPoint = findPoint(coord , false, ground_point::HOVER_RADIUS);

    QVector<QSharedPointer<atd::CRoutePoint> > route;
    QSharedPointer<atd::CRoutePoint> pCurrentPoint(new atd::CRoutePoint);
    QSharedPointer<atd::CPoint> pnt(new atd::CPoint);
    pnt->SetCoord(vehicle->GetCoord());
    pnt->GetCoord().Center(stuntCenter());
    pnt->SetName(mScene->name());
    pnt->SetType(atd::CPoint::enAirportPoint);
    pCurrentPoint->SetPoint(pnt);
    pCurrentPoint->SetCustom(true);
    pCurrentPoint->SetLevel(300);

    route.append(pCurrentPoint);

    if(idxVehicle != -1 && idxPoint != -1)
    {
        route += toRoutePoints( computePath(idxVehicle, idxPoint) );
        vehicle->setRoute( route.toList() );
    }
    else
    {
        QSharedPointer<atd::CRoutePoint> pNewPoint(new atd::CRoutePoint);
        QSharedPointer<atd::CPoint> pPoint(QSharedPointer<atd::CPoint>(new atd::CPoint));
        pPoint.data()->SetCoord(coord);
        pPoint->GetCoord().Center(stuntCenter());
        pPoint.data()->SetName(pPoint.data()->GetCoord().GetAsString());
        pNewPoint->SetPoint(pPoint);
        pNewPoint->SetCustom(true);
        pNewPoint->SetLevel(300);

        route.append(pNewPoint);

        vehicle->setRoute( route.toList() );
    }
}

QVector<QSharedPointer<atd::CRoutePoint> > GroundSceneLogic::computeVehiclePath(const QVector<CCoord> &coordinates)
{
    QVector<QSharedPointer<atd::CRoutePoint> > resultRoute;

    int previousIndex = -1;
    foreach(CCoord currentPoint, coordinates)
    {
        //int pointIdx = findPoint(currentPoint, true);
        int pointIdx = findPoint(currentPoint , false, ground_point::HOVER_RADIUS);
        if(resultRoute.isEmpty() || pointIdx == -1)
        {// if first point or non-GroundPoint
            QSharedPointer<atd::CRoutePoint> pRoutePoint(new atd::CRoutePoint);
            QSharedPointer<atd::CPoint> pnt(new atd::CPoint);
            pnt->SetCoord(currentPoint);
            pnt->GetCoord().Center(stuntCenter());
            pnt->SetName(mScene->name());
            pnt->SetType(atd::CPoint::enAirportPoint);
            pRoutePoint->SetPoint(pnt);
            pRoutePoint->SetCustom(true);

            resultRoute.append(pRoutePoint);
        }
        else
        {
            //resultRoute.pop_back();
            if(previousIndex == -1)
            {// if previous point was non-GroundPoint
                previousIndex = findPoint(resultRoute.last()->GetPoint()->GetCoord(), false);
            }
            resultRoute += toRoutePoints( computePath(previousIndex, pointIdx) );
        }

        previousIndex = pointIdx;
    }

    return resultRoute;
}
#endif
