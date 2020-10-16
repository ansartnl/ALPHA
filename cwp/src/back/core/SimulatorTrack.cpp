#include "SimulatorTrack.h"
#include "View.h"
#include "GUIView.h"

#include <QDebug>

SectorPoint::SectorPoint()
{

}

SectorPoint::~SectorPoint()
{

}

////////////////////////////////////////////////

SimulatorPoint::SimulatorPoint()
    : m_PointSN(-1), m_Seconds(0)
{

}

SimulatorPoint::SimulatorPoint(const CAirplaneData &ad)
    : CAirplaneData(ad),  m_PointSN(-1), m_Seconds(0)
{

}

SimulatorPoint::~SimulatorPoint()
{

}

void SimulatorPoint::AddSector(const Scene::TScenePtr &sector)
{
    if(sector)
    {
        SectorPoint sp;
        sp.SetSector(sector);
        int iUpLimit = sector->GetUpLimit();
        if(!iUpLimit)
            iUpLimit = 1000000;
        m_SectorList.insertMulti(iUpLimit, sp);
    }
}

bool SimulatorPoint::CompareSectors(const TSectorPointList &other) const
{
    if(other.size() == GetSectorList().size())
    {
        TSectorPointList::const_iterator itSector = other.constBegin();
        for(; itSector != other.constEnd(); ++itSector)
        {
            bool bInclude = false;
            TSectorPointList::const_iterator itS = GetSectorList().constBegin();
            for(; itS != GetSectorList().constEnd(); ++itS)
            {
                if(itSector->GetSector() == itS->GetSector())
                {
                    bInclude = true;
                    break;
                }
            }
            if(!bInclude)
                return false;
        }
        return true;
    }
    else
    {
        return false;
    }
}

////////////////////////////////////////////////

SimulatorTrack::SimulatorTrack()
    : m_PointSN(-1)
{

}

SimulatorTrack::~SimulatorTrack()
{

}

void SimulatorTrack::AddPoint(const CAirplaneData &ad, int iSeconds)
{
    QSharedPointer<SimulatorPoint> p(new SimulatorPoint(ad));
    p->SetPointSN(++m_PointSN);
    p->SetSeconds(iSeconds);
    m_AllPoints.append(p);
}

const SimulatorTrack::TPointList& SimulatorTrack::GetBoundaryPoints() const
{
    return m_BoundaryPoints;
}

const SimulatorTrack::TPointList& SimulatorTrack::GetAllPoints() const
{
    return m_AllPoints;
}

void SimulatorTrack::CalculateSectors(const QSharedPointer<IScene> &pScene)
{
    qDebug() << "Sectors calculation begin " << QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
    QSharedPointer<CView> pView = qSharedPointerDynamicCast<CView>(pScene);

//    CGUIView *pGuiView = dynamic_cast<CGUIView*> (pView.data());

    if(pView)
    {
        TPointList::iterator itPoint = m_AllPoints.begin();
        for(; itPoint != m_AllPoints.end(); ++itPoint)
        {
            double dAltitide = (*itPoint)->GetFL() * 30.48;
            CCoord coord = (*itPoint)->GetCoord();

//            if (itPoint != m_AllPoints.begin())
//            {
//                pGuiView->AddTempLine(coord, (*(itPoint-1))->GetCoord());
//            }
//            qDebug() << coord.GetAsString() << dAltitide;

            // Check all sectors
            CView::TSceneList::const_iterator itFIR = pView->GetScenes().constBegin();
            for(; itFIR != pView->GetScenes().constEnd(); ++itFIR)
            {
                Scene::TSceneList::const_iterator itScene = (*itFIR)->GetSectorList().constBegin();
                for (; itScene != (*itFIR)->GetSectorList().constEnd(); ++itScene)
                {
                    if ((dAltitide >= (*itScene)->GetLowLimit() && (dAltitide <= (*itScene)->GetUpLimit()
                        || (*itScene)->GetUpLimit() == 0)) && (*itScene)->IsIncluded(coord, 1))
                    {
                        (*itPoint)->AddSector(*itScene);
                        //qDebug() << (*itPoint)->GetPointSN() << dAltitide << (*itScene)->GetName();
                    }
                }
            }
        }

        //qDebug() << "Sectors BoundaryPoints ";
        // cut points inside sectors
        m_BoundaryPoints = m_AllPoints;
        itPoint = m_BoundaryPoints.begin();
        TPointList::iterator itPrev1 = itPoint;
        TPointList::iterator itPrev2 = itPoint;
        int i = 0;
        while(itPoint != m_BoundaryPoints.end())
        {
            ++i;
            if((*itPoint)->CompareSectors((*itPrev1)->GetSectorList()))
            {
                if(i == 1)
                {
                    ++itPoint;
                }
                else if(i == 2)
                {
                    itPrev1 = itPoint;
                    ++itPoint;
                }
                else
                {
                    if((*itPrev1)->CompareSectors((*itPrev2)->GetSectorList()))
                    {
                        m_BoundaryPoints.erase(itPrev1);
                    }
                    else
                    {
                        itPrev2 = itPrev1;
                    }
                    itPrev1 = itPoint;
                    ++itPoint;
                }
            }
            else
            {
                if(i == 1)
                {
                    ++itPoint;
                }
                else if(i == 2)
                {
                    itPrev1 = itPoint;
                    ++itPoint;
                }
                else
                {
                    itPrev2 = itPrev1;
                    itPrev1 = itPoint;
                    ++itPoint;
                }
            }
        }

        // find boundary points
        itPoint = m_BoundaryPoints.begin();
        for(QMultiMap<int, QSharedPointer<CPoint> > prevMap; itPoint != m_BoundaryPoints.end(); ++itPoint)
        {
            QMultiMap<int, QSharedPointer<CPoint> > curMap;
            // use one sector with min UpLimit, may be in future all sectors in list
            if((*itPoint)->GetSectorList().size())
            {
                SimulatorPoint::TSectorPointList::iterator itFirst = (*itPoint)->GetSectorList().begin();
                curMap = GetSectorBoundaryPoints((*itPoint)->GetCoord(), itFirst->GetSector());
                QSharedPointer<CPoint> pPoint;
                if(curMap.size())
                    pPoint = *(curMap.begin());
                itFirst->SetPoint(pPoint);
                if(itPoint != m_BoundaryPoints.begin())
                {
                    //TPointList::iterator itPointPrev = itPoint-1;
                    TPointList::iterator itPointPrev = itPoint;
                    --itPointPrev;
                    if((*itPointPrev)->GetSectorList().size())
                    {
                        SimulatorPoint::TSectorPointList::iterator itFirstPrev = (*itPointPrev)->GetSectorList().begin();
                        if(itFirst->GetSector() != itFirstPrev->GetSector())
                        {
                            pPoint = FindBoundaryPoint(curMap, prevMap);
                            if(pPoint)
                            {
                                itFirst->SetPoint(pPoint);
                                itFirstPrev->SetPoint(pPoint);
                            }
                        }
                        //qDebug() << (*itPoint)->GetPointSN() << "   " << (*itPointPrev)->GetSectorList().begin()->GetSector()->GetName();
                    }
                }
            }
            prevMap = curMap;
        }
    }
    qDebug() << "Sectors calculation end   " << QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
}

QMultiMap<int, QSharedPointer<CPoint> > SimulatorTrack::GetSectorBoundaryPoints(const CCoord &coord, const Scene::TScenePtr& sector)
{
    QMultiMap<int, QSharedPointer<CPoint> > mapPoints;
    if(sector)
    {
        Scene::TPointsMap::const_iterator itPoint = sector->GetPoints().constBegin();
        for(; itPoint != sector->GetPoints().constEnd(); ++itPoint)
        {
            int iDistance = qRound(GetDistance(coord, (*itPoint)->GetCoord()));
            if(iDistance < 3*1000 /*&& (*itPoint)->GetType() == CPoint::enWaypoint*/)
                mapPoints.insertMulti(iDistance, *itPoint);
        }
    }
    return mapPoints;
}

QSharedPointer<CPoint> SimulatorTrack::FindBoundaryPoint(const QMultiMap<int, QSharedPointer<CPoint> >& map1, const QMultiMap<int, QSharedPointer<CPoint> >& map2)
{
    QSharedPointer<CPoint> pPoint;
    if(map1.size() && map2.size())
    {
        if(*(map1.begin()) == *(map2.begin()))
        {
            pPoint = *(map1.begin());
        }
        else
        {
            //later
        }
    }
    else
    {
        //later
    }
    return pPoint;
}
