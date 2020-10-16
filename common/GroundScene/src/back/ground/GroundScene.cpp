#include "GroundScene.h"
#include <QStringList>

#ifndef GROUND_CWP
#include "utils/TVAMath.h"
#include "GroundSceneLogic.h"
#include "Simulator.h"
#include "back/core/Scene.h"
#else
#include "XMasterWindow.h"
#endif
#include "front/ground/GUIGroundParking.h"
#include <QDebug>
#include "back/common/VehicleState.h"

GroundScene::~GroundScene()
{

}

void GroundScene::SetFormularDefaultPos()
{
    TAirplanesMap::iterator itAirplans = GetAirplanes().begin();
    for(; itAirplans != GetAirplanes().end(); ++itAirplans)
    {
        (*itAirplans)->SetFormularDefaultPos();
    }
}

#ifndef GROUND_CWP
void GroundScene::SetSimulator(CSimulator *pSimulator)
{
    m_pSimulator = pSimulator;
}

void GroundScene::SetCentralPoint(const QSharedPointer<atd::CPoint>& pCentral)
{
    m_pCentralPoint = pCentral;
}

const QSharedPointer<atd::CPoint>& GroundScene::GetCentralPoint() const
{
    return m_pCentralPoint;
}
#else
void GroundScene::SetCentralPoint(const QSharedPointer<CPoint>& pCentral)
{
    m_pCentralPoint = pCentral;
}

const QSharedPointer<CPoint>& GroundScene::GetCentralPoint() const
{
    return m_pCentralPoint;
}
#endif

void GroundScene::UpdateAirplane(CAirplaneState &AirplaneState)
{
//    if (AirplaneState.GetTrackNumber() == CAirplaneState::kClearAllTracks)
//    {
//        // Clear
//        foreach (const QSharedPointer<CAirplane> &a, m_Airplanes)
//            DelAirplane(a);
//        return;
//    }

    CCoord CoordTemp(AirplaneState.GetCoordinate());
    if (IsSquareIncluded(CoordTemp, 3000))
    {
        QSharedPointer<CAirplane> pAirplane;
        TAirplanesMap::iterator itFind = m_Airplanes.find(AirplaneState.GetTrackNumber());
        if (itFind == m_Airplanes.end())
        {
            // Create
            pAirplane = QSharedPointer<CAirplane>(CreateAirplane());
            pAirplane->SetID(AirplaneState.GetTrackNumber());
            pAirplane->SetOperName(QObject::tr("   "));
            pAirplane->SetMode3AReplyInit(AirplaneState.GetMode3AReply());
            pAirplane->setIsFlying(false);
        } else
        {
            // Update
            pAirplane = *itFind;
#ifdef GROUND_CWP
            if(AirplaneState.GetTrackStatus() == CAirplaneState::EndTrack)
            {
                DelAirplane(*itFind);
                itFind = m_Airplanes.end();
                return;
            }
#endif
        }

        pAirplane->SetMode3AReply(AirplaneState.GetMode3AReply());
        pAirplane->SetCoord(CoordTemp);
        pAirplane->SetFL(AirplaneState.GetFlightLevel());
        //check if velocity has decreased & has become < than takeoff velocity (for updated airplane Only)
#ifndef GROUND_CWP

        double landingIAS = 40;
        if(pAirplane->GetChain() && pAirplane->GetChain()->GetFPLs().size() > pAirplane->GetCurFPLPlan())
            landingIAS = pAirplane->GetChain()->GetFPLs()[pAirplane->GetCurFPLPlan()]->GetAirplanePtr()->GetIAS(0) - 1.4;

        if (itFind != m_Airplanes.end() &&
                AirplaneState.GetFlightLevel() < mAltitude + 50 &&
                AirplaneState.GetVelocity().GetV() < landingIAS)
        {
            mEmitter.handleLanding(pAirplane);
        }
        pAirplane->SetUpdated(true);
#else
        pAirplane->setUpdated(true);
        if (pAirplane->GetACID().isEmpty() && !AirplaneState.GetCallsign().isEmpty())
            pAirplane->SetACID(AirplaneState.GetCallsign().trimmed());
#endif
        pAirplane->SetVelocity(AirplaneState.GetVelocity());
        pAirplane->SetVz(AirplaneState.GetZVelocity());
        pAirplane->SetSPI(AirplaneState.GetSPI());
        if (itFind == m_Airplanes.end())
            AddAirplane(pAirplane);

    } else
    {
        TAirplanesMap::iterator itFind = m_Airplanes.find(AirplaneState.GetTrackNumber());
        if (itFind != m_Airplanes.end())
        {
            // Erase
            DelAirplane(*itFind);
        }
    }
}

bool GroundScene::IsSquareIncluded(CCoord& Coord, int iDelta)
{
    if ( !m_pCentralPoint ) return false;
    Coord.Center(m_pCentralPoint->GetCoord());
    if (Coord.GetX() >= m_BoundMin.GetX() - iDelta &&
        Coord.GetX() <= m_BoundMax.GetX() + iDelta &&
        Coord.GetY() >= m_BoundMin.GetY() - iDelta &&
        Coord.GetY() <= m_BoundMax.GetY() + iDelta)
        return true;
    return false;
}

void GroundScene::AddAirplane(const QSharedPointer<CAirplane> &pAirplane)
{
    m_Airplanes[pAirplane->GetID()] = pAirplane;
    if (m_pCentralPoint && !pAirplane->GetCoord().IsCenter())
    {
        CCoord centeredCoord = pAirplane->GetCoord();
        centeredCoord.Center(m_pCentralPoint->GetCoord());
        pAirplane->SetCoord(centeredCoord);
    }
#ifndef GROUND_CWP
    pAirplane->SetSimulator(m_pSimulator);
#else
    pAirplane->SetMaster(master());
#endif
}

void GroundScene::AddVehicle(const QSharedPointer<GroundVehicle> &pVehicle)
{
    m_Vehicles[pVehicle->GetID()] = pVehicle;
    if (m_pCentralPoint && !pVehicle->GetCoord().IsCenter())
    {
        CCoord centeredCoord = pVehicle->GetCoord();
        centeredCoord.Center(m_pCentralPoint->GetCoord());
        pVehicle->SetCoord(centeredCoord);
    }
#ifndef GROUND_CWP
    pVehicle->SetSimulator(m_pSimulator);
#else
    //pAirplane->SetMaster(master());
#endif
}

void GroundScene::AddPoint(const QSharedPointer<GroundPoint> &pPoint)
{
    if (m_pCentralPoint && !pPoint->coord().IsCenter())
    {
        CCoord centeredCoord = pPoint->coord();
        centeredCoord.Center(m_pCentralPoint->GetCoord());
        pPoint->setCoord(centeredCoord);
    }

    m_Points.insert(pPoint->index(), pPoint);
    m_Boundary.append(pPoint->coord());
}

void GroundScene::AddTower(const QSharedPointer<GroundTower> &pTower)
{
    if (m_pCentralPoint && !pTower->coord().IsCenter())
    {
        CCoord centeredCoord = pTower->coord();
        centeredCoord.Center(m_pCentralPoint->GetCoord());
        pTower->setCoord(centeredCoord);
    }

    m_Towers.append(pTower);
    m_Boundary.append(pTower->coord());
}

void GroundScene::AddStart(const QSharedPointer<GroundStart> &pStart)
{
    if (m_pCentralPoint && !pStart->coord().IsCenter())
    {
        CCoord centeredCoord = pStart->coord();
        centeredCoord.Center(m_pCentralPoint->GetCoord());
        pStart->setCoord(centeredCoord);
    }

    m_Starts.append(pStart);
    m_Boundary.append(pStart->coord());
}

void GroundScene::AddRunway(const QSharedPointer<GroundRunway> &pRunway)
{
    if (m_pCentralPoint.isNull())
    {
#ifndef GROUND_CWP
        m_pCentralPoint = QSharedPointer<atd::CPoint>(new atd::CPoint);
#else
        m_pCentralPoint = QSharedPointer<CPoint>(new CPoint);
#endif
        m_pCentralPoint->SetName(QString("Central point"));
        m_pCentralPoint->SetCoord(pRunway->coord());
    }

    if (m_pCentralPoint && !pRunway->coord().IsCenter())
    {
        CCoord centeredCoord = pRunway->coord();
        centeredCoord.Center(m_pCentralPoint->GetCoord());
        pRunway->setCoord(centeredCoord);
    }

    m_Runways.append(pRunway);
    m_Boundary.append(pRunway->coord());
}

void GroundScene::AddApron(const QSharedPointer<GroundApron> &pApron)
{
    if (m_pCentralPoint)
    {
        int vertexCount = pApron->vertex().count();
        for(int i = 0; i < vertexCount; i++)
        {
            if(!pApron->vertex()[i].IsCenter())
            {
                pApron->centerVertex(i, m_pCentralPoint->GetCoord());
            }
            m_Boundary.append(pApron->vertex()[i]);
        }
    }

    m_Aprons.append(pApron);
}

void GroundScene::AddPath(const QSharedPointer<GroundPath> &pPath)
{
    if (m_pCentralPoint && !pPath->startPointCoord().IsCenter())
    {
        CCoord centeredCoord = pPath->startPointCoord();
        centeredCoord.Center(m_pCentralPoint->GetCoord());
        pPath->setStartPointCoord(centeredCoord);
    }
    if (m_pCentralPoint && !pPath->endPointCoord().IsCenter())
    {
        CCoord centeredCoord = pPath->endPointCoord();
        centeredCoord.Center(m_pCentralPoint->GetCoord());
        pPath->setEndPointCoord(centeredCoord);
    }

    m_Paths.append(pPath);
}

void GroundScene::AddTrace(const QSharedPointer<GroundTrace> &pTrace)
{
    CCoord centeredCoord;
    if (m_pCentralPoint && !pTrace->startPointCoord().IsCenter())
    {
        centeredCoord = pTrace->startPointCoord();
        centeredCoord.Center(m_pCentralPoint->GetCoord());
        pTrace->setStartPointCoord(centeredCoord);
    }
    if (m_pCentralPoint && !pTrace->endPointCoord().IsCenter())
    {
        centeredCoord = pTrace->endPointCoord();
        centeredCoord.Center(m_pCentralPoint->GetCoord());
        pTrace->setEndPointCoord(centeredCoord);
    }

    m_Traces.append(pTrace);
}

void GroundScene::AddParking(const QSharedPointer<GroundParking> &pParking)
{
    if (m_pCentralPoint && !pParking->coord().IsCenter())
    {
        CCoord centeredCoord = pParking->coord();
        centeredCoord.Center(m_pCentralPoint->GetCoord());
        pParking->setCoord(centeredCoord);
    }

    m_Parkings.insert(pParking->name(), pParking);
    m_Boundary.append(pParking->coord());
}

void GroundScene::AddBuilding(const QSharedPointer<GroundBuilding> &pBuilding)
{
    if (m_pCentralPoint && !pBuilding->coord().IsCenter())
    {
        CCoord centeredCoord = pBuilding->coord();
        centeredCoord.Center(m_pCentralPoint->GetCoord());
        pBuilding->setCoord(centeredCoord);
    }

    m_Buildings.append(pBuilding);
    m_Boundary.append(pBuilding->coord());
}

void GroundScene::AddSign(const QSharedPointer<GroundSign> &pSign)
{
    if (m_pCentralPoint && !pSign->coord().IsCenter())
    {
        CCoord centeredCoord = pSign->coord();
        centeredCoord.Center(m_pCentralPoint->GetCoord());
        pSign->setCoord(centeredCoord);
    }

    m_Signs.append(pSign);
    m_Boundary.append(pSign->coord());
}

void GroundScene::AddCrossroads(const QSharedPointer<GroundCrossroads> &pCrossroads)
{
    if (m_pCentralPoint && !pCrossroads->coord().IsCenter())
    {
        CCoord centeredCoord = pCrossroads->coord();
        centeredCoord.Center(m_pCentralPoint->GetCoord());
        pCrossroads->setCoord(centeredCoord);
    }

    m_Crossroads.insert(pCrossroads->index(), pCrossroads);
    m_Boundary.append(pCrossroads->coord());
}

void GroundScene::AddRadioRestriction(const QSharedPointer<GroundRadioRestriction> &pRestriction)
{
    if (m_pCentralPoint)
    {
        int vertexCount = pRestriction->vertex().count();
        for(int i = 0; i < vertexCount; i++)
        {
            if(!pRestriction->vertex()[i].IsCenter())
            {
                pRestriction->centerVertex(i, m_pCentralPoint->GetCoord());
            }
            m_Boundary.append(pRestriction->vertex()[i]);
        }
    }

    m_RadioRestrictions.append(pRestriction);
}

void GroundScene::AddWaypointMarker(const QSharedPointer<GroundWaypointMarker> &pMarker)
{
    if (m_pCentralPoint && !pMarker->coord().IsCenter())
    {
        CCoord centeredCoord = pMarker->coord();
        centeredCoord.Center(m_pCentralPoint->GetCoord());
        pMarker->setCoord(centeredCoord);
    }

    m_WaypointMarkers.insert(pMarker->SN(), pMarker);
}

void GroundScene::DelAirplane(QSharedPointer<CAirplane> pAirplane, bool bSend)
{
    Q_UNUSED(bSend)
    m_Airplanes.remove(pAirplane->GetID());

#ifndef GROUND_CWP
    if (m_Airplanes.isEmpty())
        mLogic->clearIndexCache();
#endif
}

void GroundScene::UpdateVehicle(const VehicleState &vehicleState)
{
    CCoord CoordTemp(vehicleState.coord);

    if (IsSquareIncluded(CoordTemp, 3000))
    {
        QSharedPointer<GroundVehicle> pVehicle;
        TVehicleMap::iterator itFind = m_Vehicles.find(vehicleState.ID);
        if (itFind == m_Vehicles.end())
        {
            // Create
            pVehicle = QSharedPointer<GroundVehicle>(CreateVehicle());
            pVehicle->SetID(vehicleState.ID);
            //pVehicle->SetMode3AReplyInit(vehicleState.name);
        } else
            // Update
            pVehicle = *itFind;
        pVehicle->setName(vehicleState.name);
        pVehicle->SetIAS(vehicleState.IAS);
        //pVehicle->SetMode3AReply(vehicleState.name);
        pVehicle->SetCoord(CoordTemp);

        pVehicle->setIsHidden(vehicleState.hidden);
#ifdef GROUND_CWP
        //pVehicle->setUpdated(true);
#else
        //pVehicle->SetUpdated(true);
        pVehicle->SetVelocity(vehicleState.velocity);
#endif
        if (itFind == m_Vehicles.end())
            AddVehicle(pVehicle);

    } else
    {
        TVehicleMap::iterator itFind = m_Vehicles.find(vehicleState.ID);
        if (itFind != m_Vehicles.end())
        {
            // Erase
            DelVehicle(*itFind);
        }
    }
}

void GroundScene::DelVehicle(QSharedPointer<GroundVehicle> pVehicle, bool bSend)
{
    Q_UNUSED(bSend)
    m_Vehicles.remove(pVehicle->GetID());
}

void GroundScene::BearingData(CBearingData& BearingData)
{
    Q_UNUSED(BearingData)
}

void GroundScene::SetSTCA(AirplaneCollision &ac)
{
    Q_UNUSED(ac)
}

QSharedPointer<GroundPoint> GroundScene::CreatePoint() const
{
    return QSharedPointer<GroundPoint>(new GroundPoint());
}

QSharedPointer<GroundTower> GroundScene::CreateTower() const
{
    return QSharedPointer<GroundTower>(new GroundTower());
}

QSharedPointer<GroundStart> GroundScene::CreateStart() const
{
    return QSharedPointer<GroundStart>(new GroundStart());
}

QSharedPointer<GroundRunway> GroundScene::CreateRunway() const
{
    return QSharedPointer<GroundRunway>(new GroundRunway());
}

QSharedPointer<GroundApron> GroundScene::CreateApron() const
{
    return QSharedPointer<GroundApron>(new GroundApron());
}

QSharedPointer<GroundPath> GroundScene::CreatePath() const
{
    return QSharedPointer<GroundPath>(new GroundPath());
}

QSharedPointer<GroundTrace> GroundScene::CreateTrace() const
{
    return QSharedPointer<GroundTrace>(new GroundTrace());
}

QSharedPointer<GroundParking> GroundScene::CreateParking() const
{
    return QSharedPointer<GroundParking>(new GroundParking());
}

QSharedPointer<GroundBuilding> GroundScene::CreateBuilding() const
{
    return QSharedPointer<GroundBuilding>(new GroundBuilding());
}

QSharedPointer<GroundSign> GroundScene::CreateSign() const
{
    return QSharedPointer<GroundSign>(new GroundSign());
}

QSharedPointer<GroundCrossroads> GroundScene::CreateCrossroads() const
{
    return QSharedPointer<GroundCrossroads>(new GroundCrossroads());
}

QSharedPointer<CAirplane> GroundScene::CreateAirplane() const
{
    return QSharedPointer<CAirplane>(new CAirplane());
}

QSharedPointer<GroundVehicle> GroundScene::CreateVehicle() const
{
    return QSharedPointer<GroundVehicle>(new GroundVehicle());
}

QSharedPointer<GroundRadioRestriction> GroundScene::CreateRadioRestriction() const
{
    return QSharedPointer<GroundRadioRestriction>(new GroundRadioRestriction());
}

QSharedPointer<GroundWaypointMarker> GroundScene::CreateWaypointMarker() const
{
    return QSharedPointer<GroundWaypointMarker>(new GroundWaypointMarker());
}


void GroundScene::DelWaypointMarker(QSharedPointer<GroundWaypointMarker> pMarker)
{
    m_WaypointMarkers.remove(pMarker->SN());
}
