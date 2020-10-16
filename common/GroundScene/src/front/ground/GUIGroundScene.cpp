#include "GUIGroundScene.h"

#include <math.h>
#include <QWheelEvent>
#include <QGraphicsSceneWheelEvent>
#include <QScrollBar>
#include <QApplication>
#include <QShortcut>

#include "back/ground/GroundMain.h"
#include "back/ground/GroundSceneLogic.h"

#ifndef GROUND_CWP
#include "back/Simulator.h"
#else
#include "front/core/airplane/AClist/ListAC.h"
#include "obelix/CmdRequestAllFPL.h"
#include "XMasterWindow.h"
#endif

#include "main.h"

#include "front/core/GUIRoute.h"
#include "front/ground/GUIGroundVehicle.h"
//#include <QDebug>

GUIGroundScene::GUIGroundScene(GUIGroundScene::IParent *pParent)
    : m_qWidth(800), m_qHeight(800), m_iScale(0)
    , m_pParent(pParent), m_bPull(false), m_Moved(false), m_pPlaneNeedCoord(0), m_pVehicleNeedCoord(0),
#ifndef GROUND_CWP
    m_SI(0),
#endif
      m_bShowGauge(false), m_bAutoCollapse(true), m_qEchelon(50), m_qPress(1013/*760*/)
    , m_Deflexion(0,2)
    , m_pTempLine(0), m_WaypointManager(this), m_bRefreshListACC(false), m_VehicleRouteEditing(false),
      m_deletingWaypoint(false), mRestoreLabelsShortcut(0)
{
    setMouseTracking(true);
    bool antialiasing(false);

#ifdef GROUND_CWP
    antialiasing = networkSettings()->value("users/antialiasing", false).toBool();
#else
    antialiasing = simulator()->groundAntialiasing();
#endif

    if(antialiasing)
        setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    else
        setRenderHints(QPainter::TextAntialiasing);
}

GUIGroundScene::~GUIGroundScene()
{
    if(mRestoreLabelsShortcut)
    {
        delete mRestoreLabelsShortcut;
    }
}

void GUIGroundScene::Translate()
{
    TVehicleMap::const_iterator itVehicle = getVehicles().begin();
    for(; itVehicle != getVehicles().end(); ++itVehicle)
    {
        GUIGroundVehicle* pVehicle = dynamic_cast<GUIGroundVehicle*>(itVehicle->data());
        if(pVehicle)
            pVehicle->Translate();
    }
}

void GUIGroundScene::AddAirplane(const QSharedPointer<CAirplane> &pAirplane)
{
    GroundScene::AddAirplane(pAirplane);
    CGUIAirplane* pItem = dynamic_cast<CGUIAirplane*>(pAirplane.data());
    if (pItem)
    {
        pItem->SetEchelon(m_qEchelon);
        pItem->SetPress(m_qPress);
        pItem->SetAutoCollapse(m_bAutoCollapse);
        pItem->SetDeflexion(m_Deflexion);

#ifndef GROUND_CWP
        connect(pItem, SIGNAL( resetBearing() ), this, SLOT( OnResetBearing() ));
        connect(pItem, SIGNAL( calcCoordAtPoint(const QPointF&,CCoord&)), this, SLOT(OnCalcCoordAtPoint(const QPointF&,CCoord&)));
#endif
        connect(pItem, SIGNAL( getCoord(CAirplane*) ), this, SLOT( OnSetPlaneNeedCoord(CAirplane*) ));
        connect(pItem, SIGNAL( setActivePlane(CAirplane*) ), this, SLOT( OnSetActivePlane(CAirplane*) ));

        scene()->addItem(pItem);

        pItem->SetPosition(m_qXMin, m_qYMin + m_qYSize, m_dScale, GetCentralPoint()->GetCoord());
#ifndef GROUND_CWP
        pItem->SetSI(m_SI.GetSI());
#else
        CListAC::AddPlaneRecord(pAirplane);

        if(!m_bRefreshListACC)
        {
            m_bRefreshListACC = true;
            QTimer::singleShot(3000, this, SLOT(OnTimerRefreshListACC()));
        }
#endif
        pItem->setVisible(true);
    }    
}

void GUIGroundScene::AddPoint(const QSharedPointer<GroundPoint> &pPoint)
{
    GroundScene::AddPoint(pPoint);

    connect( dynamic_cast<GUIGroundPoint*>(pPoint.data()), SIGNAL(hoverEnter(int)), this, SLOT(pointHoverEnter(int)));
    connect( dynamic_cast<GUIGroundPoint*>(pPoint.data()), SIGNAL(hoverLeave(int)), this, SLOT(pointHoverLeave(int)));

    QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*>(pPoint.data());
    if (pItem)
        scene()->addItem(pItem);
}

void GUIGroundScene::AddTower(const QSharedPointer<GroundTower> &pTower)
{
    GroundScene::AddTower(pTower);
    QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*>(pTower.data());
    if (pItem)
        scene()->addItem(pItem);
}

void GUIGroundScene::AddStart(const QSharedPointer<GroundStart> &pStart)
{
    GroundScene::AddStart(pStart);
    QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*>(pStart.data());
    if (pItem)
        scene()->addItem(pItem);
}

void GUIGroundScene::AddRunway(const QSharedPointer<GroundRunway> &pRunway)
{
    GroundScene::AddRunway(pRunway);
    QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*>(pRunway.data());
    if (pItem)
        scene()->addItem(pItem);
}

void GUIGroundScene::AddApron(const QSharedPointer<GroundApron> &pApron)
{
    GroundScene::AddApron(pApron);
    QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*>(pApron.data());
    if (pItem)
        scene()->addItem(pItem);
}

void GUIGroundScene::AddPath(const QSharedPointer<GroundPath> &pPath)
{
    GroundScene::AddPath(pPath);
    QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*>(pPath.data());
    if (pItem)
        scene()->addItem(pItem);
}

void GUIGroundScene::AddTrace(const QSharedPointer<GroundTrace> &pTrace)
{
    GroundScene::AddTrace(pTrace);
    QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*>(pTrace.data());
    if (pItem)
        scene()->addItem(pItem);
}

void GUIGroundScene::AddParking(const QSharedPointer<GroundParking> &pParking)
{
    GroundScene::AddParking(pParking);

    connect( dynamic_cast<GUIGroundParking*>(pParking.data()), SIGNAL(hoverEnter(QString)), this, SLOT(standHoverEnter(QString)));
    connect( dynamic_cast<GUIGroundParking*>(pParking.data()), SIGNAL(hoverLeave(QString)), this, SLOT(standHoverLeave(QString)));

    QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*>(pParking.data());
    if (pItem)
        scene()->addItem(pItem);
}

void GUIGroundScene::AddBuilding(const QSharedPointer<GroundBuilding> &pBuilding)
{
    GroundScene::AddBuilding(pBuilding);
    QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*>(pBuilding.data());
    if (pItem)
        scene()->addItem(pItem);
}

void GUIGroundScene::AddSign(const QSharedPointer<GroundSign> &pSign)
{
    GroundScene::AddSign(pSign);
    QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*>(pSign.data());
    if (pItem)
        scene()->addItem(pItem);
}

void GUIGroundScene::AddCrossroads(const QSharedPointer<GroundCrossroads> &pCrossroads)
{
    GroundScene::AddCrossroads(pCrossroads);
    QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*>(pCrossroads.data());
    if (pItem)
        scene()->addItem(pItem);
}

void GUIGroundScene::AddVehicle(const QSharedPointer<GroundVehicle> &pVehicle)
{
    GroundScene::AddVehicle(pVehicle);
    GUIGroundVehicle* pItem = dynamic_cast<GUIGroundVehicle*>(pVehicle.data());
    if (pItem)
    {
        pItem->SetAutoCollapse(m_bAutoCollapse);
        connect(pItem, SIGNAL( getCoord(GroundVehicle*) ), this, SLOT( OnSetVehicleNeedCoord(GroundVehicle*) ));
        connect(pItem, SIGNAL( calcCoordAtPoint(const QPointF&,CCoord&)), this, SLOT(OnCalcCoordAtPoint(const QPointF&,CCoord&)));

        scene()->addItem(pItem);

        pItem->SetPosition(m_qXMin, m_qYMin + m_qYSize, m_dScale, GetCentralPoint()->GetCoord());
        pItem->setVisible(true);
    }
}

void GUIGroundScene::AddRadioRestriction(const QSharedPointer<GroundRadioRestriction> &pRestriction)
{
    GroundScene::AddRadioRestriction(pRestriction);
    QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*>(pRestriction.data());
    if (pItem)
        scene()->addItem(pItem);
}

void GUIGroundScene::AddWaypointMarker(const QSharedPointer<GroundWaypointMarker> &pMarker)
{
    GroundScene::AddWaypointMarker(pMarker);
    QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*>(pMarker.data());
    if (pItem)
        scene()->addItem(pItem);
}

void GUIGroundScene::DelAirplane(QSharedPointer<CAirplane> pAirplane, bool bSend)
{
    if(m_pPlaneNeedCoord == pAirplane.data())
    {
        disconnect(dynamic_cast<CGUIAirplane*>(m_pPlaneNeedCoord), SIGNAL( getCoord(CAirplane*) ), this, SLOT( OnSetPlaneNeedCoord(CAirplane*) ));
        m_pPlaneNeedCoord = 0;
        setRouteHovering(false);
    }
//    if(m_pReminder)
//        m_pReminder->DeletePlane(pAirplane);
#ifdef GROUND_CWP
    CListAC::DelPlaneRecord(pAirplane);
#endif
    QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*>(pAirplane.data());
    if (pItem)
        scene()->removeItem(pItem);
    GroundScene::DelAirplane(pAirplane);
}

void GUIGroundScene::DelVehicle(QSharedPointer<GroundVehicle> pVehicle, bool bSend)
{
    Q_UNUSED(bSend)

    if(m_pVehicleNeedCoord == pVehicle.data())
    {
        disconnect(dynamic_cast<GUIGroundVehicle*>(m_pVehicleNeedCoord), SIGNAL( getCoord(GroundVehicle*) ), this, SLOT( OnSetVehicleNeedCoord(GroundVehicle*) ));
        m_pVehicleNeedCoord = 0;
        setRouteHovering(false);
    }

    QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*>(pVehicle.data());
    if (pItem)
        scene()->removeItem(pItem);
    GroundScene::DelVehicle(pVehicle);
}

void GUIGroundScene::DelWaypointMarker(QSharedPointer<GroundWaypointMarker> pMarker)
{
    QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*>(pMarker.data());
    if (pItem)
        scene()->removeItem(pItem);
    GroundScene::DelWaypointMarker(pMarker);
}

QSharedPointer<GroundPoint> GUIGroundScene::CreatePoint() const
{
    return QSharedPointer<GroundPoint>(new GUIGroundPoint);
}

QSharedPointer<GroundTower> GUIGroundScene::CreateTower() const
{
    return QSharedPointer<GroundTower>(new GUIGroundTower);
}

QSharedPointer<GroundStart> GUIGroundScene::CreateStart() const
{
    return QSharedPointer<GroundStart>(new GUIGroundStart);
}

QSharedPointer<GroundRunway> GUIGroundScene::CreateRunway() const
{
    return QSharedPointer<GroundRunway>(new GUIGroundRunway);
}

QSharedPointer<GroundApron> GUIGroundScene::CreateApron() const
{
    return QSharedPointer<GroundApron>(new GUIGroundApron);
}

QSharedPointer<GroundPath> GUIGroundScene::CreatePath() const
{
    return QSharedPointer<GroundPath>(new GUIGroundPath);
}

QSharedPointer<GroundTrace> GUIGroundScene::CreateTrace() const
{
    return QSharedPointer<GroundTrace>(new GUIGroundTrace);
}

QSharedPointer<GroundParking> GUIGroundScene::CreateParking() const
{
    return QSharedPointer<GroundParking>(new GUIGroundParking);
}

QSharedPointer<GroundBuilding> GUIGroundScene::CreateBuilding() const
{
    return QSharedPointer<GroundBuilding>(new GUIGroundBuilding);
}

QSharedPointer<GroundSign> GUIGroundScene::CreateSign() const
{
    return QSharedPointer<GroundSign>(new GUIGroundSign);
}

QSharedPointer<GroundCrossroads> GUIGroundScene::CreateCrossroads() const
{
    return QSharedPointer<GroundCrossroads>(new GUIGroundCrossroads);
}

QSharedPointer<CAirplane> GUIGroundScene::CreateAirplane() const
{
    return QSharedPointer<CAirplane>(new CGUIAirplane);
}

QSharedPointer<GroundVehicle> GUIGroundScene::CreateVehicle() const
{
    return QSharedPointer<GroundVehicle>(new GUIGroundVehicle);
}

QSharedPointer<GroundRadioRestriction> GUIGroundScene::CreateRadioRestriction() const
{
    return QSharedPointer<GroundRadioRestriction>(new GUIGroundRadioRestriction);
}

QSharedPointer<GroundWaypointMarker> GUIGroundScene::CreateWaypointMarker() const
{
    return QSharedPointer<GroundWaypointMarker>(new GUIGroundWaypointMarker);
}

void GUIGroundScene::SetShortcuts(const GUIGroundScene::TShortcutMap &shortcut)
{
    if(mRestoreLabelsShortcut)
    {
        delete mRestoreLabelsShortcut;
    }

    mRestoreLabelsShortcut = new QShortcut(QKeySequence(shortcut["RestoreLabels"]), this, SLOT(OnSetFormularDefaultPos()));
}

void GUIGroundScene::SetZoom(int iValue)
{
    int iDelta = iValue - m_iScale;
    m_iScale = iValue;
    Zoom(mapToScene(viewport()->rect().center()), iDelta);
}

void GUIGroundScene::Draw()
{
    m_dScale = m_qWidth/m_qXSize;
    if (m_dScale > m_qHeight/m_qYSize)
        m_dScale = m_qHeight/m_qYSize;
    m_dScale *= pow(1.125, m_iScale);

    QList<QGraphicsItem*> ItemList = items();
    QList<QGraphicsItem*>::const_iterator itScan = ItemList.begin();
    for(; itScan != ItemList.end(); ++itScan)
    {
        IGUIItemHandler* pItem = dynamic_cast<IGUIItemHandler*>(*itScan);
        if (pItem)
            pItem->SetPosition(m_qXMin, m_qYMin + m_qYSize, m_dScale,GetCentralPoint()->GetCoord());
    }

    QMap<CGUILineText*, QSharedPointer<CGUILineText> >::const_iterator itList = m_mapGauge.begin();
    for(; itList != m_mapGauge.end(); ++itList)
        if(!itList->isNull()) itList->data()->SetText(QString::null);

//    TMapSTCA::const_iterator itSTCA = m_mapSTCA.constBegin();
//    for(; itSTCA != m_mapSTCA.constEnd(); ++itSTCA)
//        if(!itSTCA->isNull()) itSTCA->data()->SetText(QString::null);
}

void GUIGroundScene::Prepare()
{
    if(m_Boundary.size())
    {
        QVector<CCoord>::iterator itScan = m_Boundary.begin();
        m_qXMin = (*itScan).GetX();
        m_qYMin = (*itScan).GetY();
        m_qXSize = m_qXMin;
        m_qYSize = m_qYMin;
        for(++itScan; itScan != m_Boundary.end(); ++itScan)
        {
            if ((*itScan).GetX() < m_qXMin)
                m_qXMin = (*itScan).GetX();
            else if ((*itScan).GetX() > m_qXSize)
                m_qXSize = (*itScan).GetX();
            if ((*itScan).GetY() < m_qYMin)
                m_qYMin = (*itScan).GetY();
            else if ((*itScan).GetY() > m_qYSize)
                m_qYSize = (*itScan).GetY();
        }

        m_BoundMin.SetX(m_qXMin);
        m_BoundMin.SetY(m_qYMin);

        m_BoundMax.SetX(m_qXSize);
        m_BoundMax.SetY(m_qYSize);

        /*
        m_qXMin -= 150;
        m_qYMin -= 100;
        m_qXSize -= m_qXMin - 150;
        m_qYSize -= m_qYMin - 800;
        */

        // centered aerodrome
        qint64 widthX = m_qXSize - m_qXMin;
        qint64 heightY = m_qYSize - m_qYMin;
        double koeff = double(heightY) / double(widthX ? widthX : heightY);
        if ( qFuzzyIsNull(koeff) )
            koeff = 1;
        m_qXMin -= qRound( koeff * widthX ) + qRound( widthX / 2.0 );
        m_qYMin -= qRound( (1 / koeff) * heightY ) + qRound( heightY / 2.0 );
        m_qXSize -= m_qXMin - qRound( koeff * widthX ) - qRound( widthX / 2.0 );
        m_qYSize -= m_qYMin - qRound( (1 / koeff) * heightY ) - qRound( heightY / 2.0 );

        SetLayouts();

        connect(&m_AirplaneTimer, SIGNAL(timeout()), this, SLOT(OnDrawAirplanes()));
        m_AirplaneTimer.start(2000);

        SetZoom(m_iScale);

        scene()->addItem(&m_WaypointManager);

        Draw();
    }
}

void GUIGroundScene::SetLayouts()
{
    TApronList::const_iterator itApron = getAprons().begin();
    for(; itApron != getAprons().end(); ++itApron)
    {
        GUIGroundApron* pApron = dynamic_cast<GUIGroundApron*>(itApron->data());
        if(pApron)
            pApron->setVisible(true);
    }

    TAirplanesMap::const_iterator itAirplane = GetAirplanes().begin();
    for(; itAirplane != GetAirplanes().end(); ++itApron)
    {
        CGUIAirplane* pAirplane = dynamic_cast<CGUIAirplane*>(itAirplane->data());
        if(pAirplane)
            pAirplane->setVisible(true);
    }

    TVehicleMap::const_iterator itVehicle = getVehicles().begin();
    for(; itVehicle != getVehicles().end(); ++itVehicle)
    {
        GUIGroundVehicle* pVehicle = dynamic_cast<GUIGroundVehicle*>(itVehicle->data());
        if(pVehicle)
            pVehicle->setVisible(true);
    }

    TBuildingList::const_iterator itBuilding = getBuildings().begin();
    for(; itBuilding != getBuildings().end(); ++itBuilding)
    {
        GUIGroundBuilding* pBuilding = dynamic_cast<GUIGroundBuilding*>(itBuilding->data());
        if(pBuilding)
            pBuilding->setVisible(true);
    }

    TParkingMap::const_iterator itParking = getParkings().begin();
    for(; itParking != getParkings().end(); ++itParking)
    {
        GUIGroundParking* pParking = dynamic_cast<GUIGroundParking*>(itParking->data());
        if(pParking)
            pParking->setVisible(true);
    }

    TPathList::const_iterator itPath = getPaths().begin();
    for(; itPath != getPaths().end(); ++itPath)
    {
        GUIGroundPath* pPath = dynamic_cast<GUIGroundPath*>(itPath->data());
        if(pPath)
            pPath->setVisible(true);
    }

    TTraceList::const_iterator itTrace = getTraces().begin();
    for(; itTrace != getTraces().end(); ++itTrace)
    {
        GUIGroundPath* pTrace = dynamic_cast<GUIGroundPath*>(itTrace->data());
        if(pTrace)
            pTrace->setVisible(true);
    }

    TPointMap::const_iterator itPoint = getPoints().begin();
    for(; itPoint != getPoints().end(); ++itPoint)
    {
        GUIGroundPoint* pPoint = dynamic_cast<GUIGroundPoint*>( itPoint->data());
        if(pPoint)
            pPoint->setVisible(true);
    }

    TRunwayList::const_iterator itRunway = getRunways().begin();
    for(; itRunway != getRunways().end(); ++itRunway)
    {
        GUIGroundRunway* pRunway = dynamic_cast<GUIGroundRunway*>(itRunway->data());
        if(pRunway)
            pRunway->setVisible(true);
    }

    TStartList::const_iterator itStart = getStarts().begin();
    for(; itStart != getStarts().end(); ++itStart)
    {
        GUIGroundStart* pStart = dynamic_cast<GUIGroundStart*>(itStart->data());
        if(pStart)
            pStart->setVisible(true);
    }

    TTowerList::const_iterator itTower = getTowers().begin();
    for(; itTower != getTowers().end(); ++itTower)
    {
        GUIGroundTower* pTower = dynamic_cast<GUIGroundTower*>(itTower->data());
        if(pTower)
            pTower->setVisible(true);
    }
}

#ifndef GROUND_CWP
void GUIGroundScene::SetSI(qint16 SI)
{
    m_SI.SetSI(SI);
    QList<QGraphicsItem*> ItemList = items();
    QList<QGraphicsItem*>::const_iterator itScan = ItemList.begin();
    for(; itScan != ItemList.end(); ++itScan)
    {
        IGUIItemHandler* pItem = dynamic_cast<IGUIItemHandler*>(*itScan);
        if (pItem)
            pItem->SetSI(SI);
    }
}
#endif

void GUIGroundScene::SetGauge(bool bShow)
{
    m_bShowGauge = bShow;
    if(!bShow)
    {
        QMap<CGUILineText*, QSharedPointer<CGUILineText> >::iterator itGauge = m_mapGauge.begin();
        for(; itGauge != m_mapGauge.end(); ++itGauge)
        {
            if(!itGauge->isNull())
            {
                scene()->removeItem(itGauge->data());
            }
        }
        m_mapGauge.clear();
    }
}

void GUIGroundScene::SetAutoCollapse(bool bCollapse)
{
    m_bAutoCollapse = bCollapse;
    TAirplanesMap::const_iterator itAirplans = GetAirplanes().begin();
    for(; itAirplans != GetAirplanes().end(); ++itAirplans)
    {
        CGUIAirplane* pPlane = dynamic_cast<CGUIAirplane*>(itAirplans->data());
        if(pPlane)
            pPlane->SetAutoCollapse(bCollapse);
    }
    TVehicleMap::const_iterator itVehicle = getVehicles().begin();
    for(; itVehicle != getVehicles().end(); ++itVehicle)
    {
        GUIGroundVehicle* pVehicle = dynamic_cast<GUIGroundVehicle*>(itVehicle->data());
        if(pVehicle)
            pVehicle->SetAutoCollapse(bCollapse);
    }
}

void GUIGroundScene::SetEchelon(qint16 echelon)
{
    m_qEchelon = echelon;
    TAirplanesMap::const_iterator itAirplans = GetAirplanes().begin();
    for(; itAirplans != GetAirplanes().end(); ++itAirplans)
    {
        CGUIAirplane* pPlane = dynamic_cast<CGUIAirplane*>(itAirplans->data());
        if(pPlane)
            pPlane->SetEchelon(echelon);
    }
}

void GUIGroundScene::SetPress(qint16 press)
{
    m_qPress = press;
    TAirplanesMap::const_iterator itAirplans = GetAirplanes().begin();
    for(; itAirplans != GetAirplanes().end(); ++itAirplans)
    {
        CGUIAirplane* pPlane = dynamic_cast<CGUIAirplane*>(itAirplans->data());
        if(pPlane)
            pPlane->SetPress(press);
    }
}

void GUIGroundScene::SetDeflexion(int type, int value)
{
    m_Deflexion.first = type;
    m_Deflexion.second = value;
    TAirplanesMap::const_iterator itPlane = GetAirplanes().constBegin();
    for(; itPlane != GetAirplanes().constEnd(); ++itPlane)
    {
        CGUIAirplane* pAirplane = dynamic_cast<CGUIAirplane*>(itPlane->data());
        if(pAirplane)
            pAirplane->SetDeflexion(m_Deflexion);
    }
}

void GUIGroundScene::DisplayRadioRestrictions(bool show)
{
    TRadioRestrictionList::const_iterator itZone = getRadioRestricions().begin();
    for(; itZone != getRadioRestricions().end(); ++itZone)
    {
        GUIGroundRadioRestriction* pZone = dynamic_cast<GUIGroundRadioRestriction*>(itZone->data());
        if(pZone)
            pZone->setVisible(show);
    }
}

CCoord GUIGroundScene::GetCoordAtPoint(QPointF pos) const
{
    CCoord Coordinate;

    if(GetCentralPoint().data())
    {
        qint64 illX = pos.x() / m_dScale + m_qXMin;
        qint64 illY = m_qYMin + m_qYSize - pos.y() / m_dScale;

        Coordinate.SetX(illX);
        Coordinate.SetY(illY);
        Coordinate.DeCenter(GetCentralPoint()->GetCoord());
    }
    return Coordinate;
}

void GUIGroundScene::DoubleClickLineText(CGUILineText *pLineText)
{
    QMap<CGUILineText*, QSharedPointer<CGUILineText> >::iterator itFind = m_mapGauge.find(pLineText);
    if(itFind != m_mapGauge.end())
    {
        if(!itFind->isNull())
        {
            scene()->removeItem(itFind->data());
            m_mapGauge.erase(itFind);
        }
    }
}

void GUIGroundScene::DoubleClickLineText(CGUILineText *pLineText, QPointF scenePos)
{
    Q_UNUSED(scenePos);
    DoubleClickLineText(pLineText);
}

void GUIGroundScene::StartCoordsPicking()
{
    StopCoordsPicking();

    m_coordsDigitizer = TCoordsDigitizer(new CoordsDigitizer());
    m_coordsDigitizer->setVisible(true);
    scene()->addItem(m_coordsDigitizer.data());
}

void GUIGroundScene::StopCoordsPicking()
{
    if ( m_coordsDigitizer )
    {
        scene()->removeItem(m_coordsDigitizer.data());
        m_coordsDigitizer.clear();
    }
}

void GUIGroundScene::displayRouteWaypoints(bool enabled, QVector<CCoord> coords)
{
    GetWaypointMarkers().clear();
    m_WaypointManager.clearWaypoints();
    m_VehicleRouteEditing = enabled;
    foreach(CCoord coord, coords)
    {
        QSharedPointer<GroundWaypointMarker> marker = CreateWaypointMarker();
        marker->setCoord(coord);
        marker->setSN(m_WaypointManager.addNew(coord));
        connect((GUIGroundWaypointMarker*)marker.data(), SIGNAL(isMoved(int, QPointF)), &m_WaypointManager, SLOT(onWaypointMoved(int,QPointF)));
        connect((GUIGroundWaypointMarker*)marker.data(), SIGNAL(isDeleted(int)), &m_WaypointManager, SLOT(onWaypointDeleted(int)));
        AddWaypointMarker(marker);
    }
    m_WaypointManager.recalculate();
    Draw();
}

void GUIGroundScene::onWaypointsRequested()
{
    emit sendWaypoints(m_WaypointManager.waypoints());
}


void GUIGroundScene::pointHoverEnter(int idx)
{
    if(routeHovering())
    {
#ifndef GROUND_CWP
        if(m_pPlaneNeedCoord)
            logic()->pointHoverEnter( m_pPlaneNeedCoord ,idx);
        if(m_pVehicleNeedCoord)
            logic()->pointHoverEnter( m_pVehicleNeedCoord ,idx);
#endif
    }
}

void GUIGroundScene::pointHoverLeave(int idx)
{
    if(routeHovering())
    {
#ifndef GROUND_CWP
        if(m_pPlaneNeedCoord)
            logic()->pointHoverLeave( m_pPlaneNeedCoord ,idx);
        if(m_pVehicleNeedCoord)
            logic()->pointHoverLeave( m_pVehicleNeedCoord ,idx);
#endif
    }
}

void GUIGroundScene::standHoverEnter(QString name)
{
    if(routeHovering())
    {
#ifndef GROUND_CWP
        if(m_pPlaneNeedCoord)
            logic()->standHoverEnter( m_pPlaneNeedCoord ,name);
        if(m_pVehicleNeedCoord)
            logic()->standHoverEnter( m_pVehicleNeedCoord ,name);
#endif
    }
}

void GUIGroundScene::standHoverLeave(QString name)
{
    if(routeHovering())
    {
#ifndef GROUND_CWP
        if(m_pPlaneNeedCoord)
            logic()->standHoverLeave( m_pPlaneNeedCoord ,name);
        if(m_pVehicleNeedCoord)
            logic()->standHoverLeave( m_pVehicleNeedCoord ,name);
#endif
    }
}

void GUIGroundScene::OnDrawAirplanes()
{
    TAirplanesMap::const_iterator itAirplans = GetAirplanes().begin();
    for(; itAirplans != GetAirplanes().end(); ++itAirplans)
    {
        CGUIAirplane* pPlane = dynamic_cast<CGUIAirplane*>(itAirplans->data());
        if(pPlane)
            pPlane->UpdatePosition();
#ifdef GROUND_CWP
        CListAC::ModPlaneRecord(*itAirplans);
#endif
    }

    TVehicleMap::const_iterator itVehicle = getVehicles().begin();
    for(; itVehicle != getVehicles().end(); ++itVehicle)
    {
        GUIGroundVehicle* pVehicle = dynamic_cast<GUIGroundVehicle*>(itVehicle->data());
        if(pVehicle)
            pVehicle->UpdatePosition();
    }

    QMap<CGUILineText*, QSharedPointer<CGUILineText> >::const_iterator itList = m_mapGauge.begin();
    for(; itList != m_mapGauge.end(); ++itList)
        if(!itList->isNull()) itList->data()->SetText(QString::null);
}

void GUIGroundScene::OnSetPlaneNeedCoord(CAirplane *receiver)
{
    m_pPlaneNeedCoord = dynamic_cast<CGUIAirplane*>(receiver);
    unsetCursor();
    QGraphicsView::setCursor(Qt::PointingHandCursor);
    setRouteHovering(true);
}

void GUIGroundScene::OnSetVehicleNeedCoord(GroundVehicle *receiver)
{
    m_pVehicleNeedCoord = dynamic_cast<GUIGroundVehicle*>(receiver);
    unsetCursor();
    QGraphicsView::setCursor(Qt::PointingHandCursor);
    setRouteHovering(true);
}

void GUIGroundScene::OnCalcCoordAtPoint(const QPointF &point, CCoord &coord)
{
    coord = GetCoordAtPoint(point);
}

void GUIGroundScene::OnPullScene(QPointF CenterPoint)
{
    centerOn(CenterPoint);
}

#ifdef GROUND_CWP
void GUIGroundScene::OnTimerRefreshListACC()
{
    m_bRefreshListACC = false;
    CListAC::RefreshAndSelect(0);
    QSharedPointer<obelix::CCmdRequestAllFPL> pCmd(new obelix::CCmdRequestAllFPL());
    if(getMainWindow())
        ((XMasterWindow*)getMainWindow())->GetObClient().XMLSend(pCmd.staticCast<CXMLCmd>());
}
#endif

void GUIGroundScene::wheelEvent(QWheelEvent *event)
{
    if (!event->buttons())
    {
        if(!scene())
        {
            QAbstractScrollArea::wheelEvent(event);
            return;
        }
        event->ignore();

        QGraphicsSceneWheelEvent wheelEvent(QEvent::GraphicsSceneWheel);
        wheelEvent.setWidget(viewport());
        wheelEvent.setScenePos(mapToScene(event->pos()));
        wheelEvent.setScreenPos(event->globalPos());
        wheelEvent.setButtons(event->buttons());
        wheelEvent.setModifiers(event->modifiers());
        wheelEvent.setDelta(event->delta());
        wheelEvent.setOrientation(event->orientation());
        wheelEvent.setAccepted(false);
        QApplication::sendEvent(scene(), &wheelEvent);
        event->setAccepted(wheelEvent.isAccepted());
        if (!event->isAccepted())
        {
            event->accept();

            int numSteps = event->delta() / (8.0 * 15.0);

            if ((m_iScale < MAX_SCALE && numSteps > 0) ||
                (m_iScale > 0 && numSteps < 0))
            {
                m_iScale += numSteps;

                if (m_iScale > MAX_SCALE)
                {
                    numSteps -= m_iScale - MAX_SCALE;
                    m_iScale = MAX_SCALE;
                }
                else if (m_iScale + numSteps < 0)
                    m_iScale = 0;

                Zoom(mapToScene(event->pos()), numSteps);
                if (m_pParent)
                    m_pParent->ZoomEvent(m_iScale);
            }
        }
    } else
    {
        QGraphicsView::wheelEvent(event);
    }
}

void GUIGroundScene::mouseMoveEvent(QMouseEvent *event)
{    
    if (m_bPull)
    {
        QPointF NewPoint(mapToScene(event->pos()));
        QPointF DeltaPoint = NewPoint - m_PullPoint;

        QPointF CenterPoint = mapToScene(viewport()->rect().center());
        CenterPoint -= DeltaPoint;
        //centerOn(CenterPoint);
        QMetaObject::invokeMethod(this, "OnPullScene", Qt::QueuedConnection, Q_ARG(QPointF, CenterPoint));
        m_PullPoint = NewPoint - DeltaPoint;
        m_Moved = true;
    } else
    {
        if (m_pParent)
        {
            QString sRoute;
            QList<QGraphicsItem*> listItems = scene()->items(mapToScene(event->pos()));
            QList<QGraphicsItem*>::const_iterator itList = listItems.constBegin();
            CGUIRoute* pRoute = 0;
            for(; itList != listItems.constEnd(); ++itList)
            {
                pRoute = dynamic_cast<CGUIRoute*>(*itList);
                if(pRoute)
                    break;
            }
            if(pRoute)
                sRoute = pRoute->GetName();
            m_pParent->CoordinateEvent(GetCoordAtPoint(mapToScene(event->pos())), sRoute);
        }

        if(m_pTempLine)
        {
            m_pTempLine->SetPoint2(GetCoordAtPoint(mapToScene(event->pos())));
        }
    }
    QGraphicsView::mouseMoveEvent(event);
}

void GUIGroundScene::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);

    if (event->button() == Qt::RightButton)
    {
        setCursor(Qt::ClosedHandCursor);
        m_bPull = true;
        m_PullPoint = (mapToScene(event->pos()));

        m_Moved = false;
    } else
    {
        if(m_pPlaneNeedCoord)
        {
            static_cast<CGUIAirplane*>(m_pPlaneNeedCoord)->SetPlaneNewCoord(GetCoordAtPoint(mapToScene(event->pos())));
#ifndef GROUND_CWP
            static_cast<CGUIAirplane*>(m_pPlaneNeedCoord)->clearPathPoints();
#endif
            static_cast<CGUIAirplane*>(m_pPlaneNeedCoord)->ShowPathPoints(false);
            m_pPlaneNeedCoord = 0;
            unsetCursor();
            setRouteHovering(false);
        }
        if(m_pVehicleNeedCoord)
        {
            static_cast<GUIGroundVehicle*>(m_pVehicleNeedCoord)->SetVehicleNewCoord(GetCoordAtPoint(mapToScene(event->pos())));
#ifndef GROUND_CWP
            static_cast<GUIGroundVehicle*>(m_pVehicleNeedCoord)->clearPathPoints();
#endif
            static_cast<GUIGroundVehicle*>(m_pVehicleNeedCoord)->ShowPathPoints(false);
            m_pVehicleNeedCoord = 0;
            unsetCursor();
            setRouteHovering(false);
        }

        if(m_bPull) unsetCursor();
        m_bPull = false;

        if(m_bShowGauge)
        {
            if ((event->button() == Qt::LeftButton) && !event->isAccepted())
            {
                m_pTempLine = new CGUILineText(this);
                m_pTempLine->setZValue(100);
                m_pTempLine->SetColor(Qt::black);
                m_pTempLine->SetPenWidth(1.5);
#ifndef GROUND_CWP
                m_pTempLine->SetSI(m_SI.GetSI());
#endif
                //m_pTempLine->SetAutoText(CGUILineText::Distance);
                m_pTempLine->SetAutoText(CGUILineText::DistanceWithAngle);
                m_pTempLine->SetPosition(m_qXMin, m_qYMin + m_qYSize, m_dScale, GetCentralPoint()->GetCoord());

                QSharedPointer<CGUILineText> pLine = QSharedPointer<CGUILineText>(m_pTempLine);
                m_mapGauge.insert(pLine.data(), pLine);
                scene()->addItem(pLine.data());

                CCoord coord1(GetCoordAtPoint(mapToScene(event->pos())));
                QList<QGraphicsItem*> listItems = scene()->items(mapToScene(event->pos()));
                QList<QGraphicsItem*>::const_iterator itList = listItems.begin();
                CGUIAirplane* pPlane = 0;
                for(; itList != listItems.end(); ++itList)
                {
                    CGUIAirplanePoint* pPlanePoint = dynamic_cast<CGUIAirplanePoint*>(*itList);
                    if(pPlanePoint)
                        pPlane = dynamic_cast<CGUIAirplane*>(pPlanePoint->parentItem());
                    else
                        pPlane = dynamic_cast<CGUIAirplane*>(*itList);
                    if(pPlane)
                        break;
                }
                if(pPlane)
                {
                    TAirplanesMap::const_iterator itFind = GetAirplanes().find(pPlane->GetID());
                    if(itFind != GetAirplanes().end())
                        pLine->SetPoint1( (*itFind)->GetCoord() );
                }
                else
                    pLine->SetPoint1(coord1);
                pLine->SetPoint2(coord1);
            }
        }
        else
        {
            // Digitizing points.
            if ( m_coordsDigitizer )
            {
                m_coordsDigitizer->addCoord( GetCoordAtPoint( mapToScene(event->pos()) ) );
#ifndef GROUND_CWP
                m_coordsDigitizer->SetPosition(m_qXMin, m_qYMin + m_qYSize, m_dScale, GetCentralPoint()->GetCoord());
#else
                m_coordsDigitizer->updatePosition(m_qXMin, m_qYMin + m_qYSize, m_dScale, GetCentralPoint()->GetCoord());
#endif
            }
        }
    }
}

void GUIGroundScene::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        if(m_bPull)
        {
            unsetCursor();
            if(m_pPlaneNeedCoord)
            {
                if(!m_Moved)
                {
                    m_pPlaneNeedCoord = 0;
                    unsetCursor();

                    // Stop digitizing points.
                    if ( m_coordsDigitizer )
                    {
                        StopCoordsPicking();
                    }
                }
                else
                {
                    setCursor(Qt::PointingHandCursor);
                }
            }
            if(m_pVehicleNeedCoord)
            {
                if(!m_Moved)
                {
                    m_pVehicleNeedCoord = 0;
                    unsetCursor();

                    // Stop digitizing points.
                    if ( m_coordsDigitizer )
                    {
                        StopCoordsPicking();
                    }
                }
                else
                {
                    setCursor(Qt::PointingHandCursor);
                }
            }

        }
        m_bPull = false;
    }
    if (event->button() == Qt::LeftButton)
    {
        if(m_pTempLine)
        {
            CCoord coord2(GetCoordAtPoint(mapToScene(event->pos())));
            QList<QGraphicsItem*> listItems = scene()->items(mapToScene(event->pos()));
            QList<QGraphicsItem*>::const_iterator itList = listItems.begin();
            CGUIAirplane* pPlane = 0;
            for(; itList != listItems.end(); ++itList)
            {
                CGUIAirplanePoint* pPlanePoint = dynamic_cast<CGUIAirplanePoint*>(*itList);
                if(pPlanePoint)
                    pPlane = dynamic_cast<CGUIAirplane*>(pPlanePoint->parentItem());
                else
                    pPlane = dynamic_cast<CGUIAirplane*>(*itList);
                if(pPlane)
                    break;
            }
            if(pPlane)
            {
                TAirplanesMap::const_iterator itFind = GetAirplanes().find(pPlane->GetID());
                if(itFind != GetAirplanes().end())
                    m_pTempLine->SetPoint2( (*itFind)->GetCoord());
            }
            else
                m_pTempLine->SetPoint2(coord2);

            m_pTempLine = 0;
        }
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void GUIGroundScene::mouseDoubleClickEvent(QMouseEvent *event)
{
    // Stop digitizing points.
    if ( m_coordsDigitizer )
    {
        m_coordsDigitizer->toClipboard();
        StopCoordsPicking();
    }

    QGraphicsView::mouseDoubleClickEvent(event);

    if(m_VehicleRouteEditing && !m_deletingWaypoint)
    {
        QSharedPointer<GroundWaypointMarker> marker = CreateWaypointMarker();
        CCoord coord = GetCoordAtPoint( mapToScene( event->pos() ));
        coord.Center(GetCentralPoint()->GetCoord());

        connect((GUIGroundWaypointMarker*)marker.data(), SIGNAL(isMoved(int, QPointF)), &m_WaypointManager, SLOT(onWaypointMoved(int,QPointF)));
        connect((GUIGroundWaypointMarker*)marker.data(), SIGNAL(isDeleted(int)), &m_WaypointManager, SLOT(onWaypointDeleted(int)));

        marker->setCoord(coord);
        marker->setSN(m_WaypointManager.addNew(coord));
        m_WaypointManager.recalculate();
        AddWaypointMarker(marker);
        Draw();
    }
    else
        m_deletingWaypoint = false;
}

void GUIGroundScene::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
    {
        if(m_pPlaneNeedCoord)
        {
#ifndef GROUND_CWP
            static_cast<CGUIAirplane*>(m_pPlaneNeedCoord)->clearPathPoints();
#endif
            static_cast<CGUIAirplane*>(m_pPlaneNeedCoord)->ShowPathPoints(false);
            m_pPlaneNeedCoord = 0;
        }
        if(m_pVehicleNeedCoord)
        {
#ifndef GROUND_CWP
            static_cast<GUIGroundVehicle*>(m_pVehicleNeedCoord)->clearPathPoints();
#endif
            static_cast<GUIGroundVehicle*>(m_pVehicleNeedCoord)->ShowPathPoints(false);
            m_pVehicleNeedCoord = 0;
        }

        unsetCursor();
        setRouteHovering(false);

        // Stop digitizing points.
        if ( m_coordsDigitizer )
        {
            StopCoordsPicking();
        }
    }
    QGraphicsView::keyPressEvent(event);
}

void GUIGroundScene::Zoom(const QPointF &PointOld, int iCurScale)
{
    QPointF CenterPoint = mapToScene(viewport()->rect().center());

    double dScale = pow(1.125, m_iScale);
    setSceneRect(0, 0, m_qWidth * dScale, m_qHeight * dScale);

    QPointF PointNew(PointOld * pow(1.125, iCurScale));
    PointNew -= PointOld;
    CenterPoint += PointNew;
    centerOn(CenterPoint);

    Draw();
    ViewportUpdateMode mode = viewportUpdateMode();
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    //resetCachedContent();
    scene()->invalidate(mapToScene(viewport()->rect()).boundingRect());
    scene()->update(mapToScene(viewport()->rect()).boundingRect());
    setViewportUpdateMode(mode);
}

void GUIGroundScene::setCursor(const QCursor &cursor)
{
    viewport()->setCursor(cursor);
    QGraphicsView::setCursor(cursor);
}

void GUIGroundScene::unsetCursor()
{
    viewport()->unsetCursor();
    QGraphicsView::unsetCursor();
}

#ifndef GROUND_CWP
void GUIGroundScene::OnResetBearing()
{
    if(simulator())
    {
        db::CDB::TGroundMap::const_iterator itScene = simulator()->GetGroundScenes().constBegin();
        for(; itScene != simulator()->GetGroundScenes().constEnd(); ++itScene)
        {
            TAirplanesMap::const_iterator itAirplane = (*itScene)->GetAirplanes().begin();
            for(; itAirplane != (*itScene)->GetAirplanes().end(); ++itAirplane)
            {
                CGUIAirplane* pAirplane = dynamic_cast<CGUIAirplane*>(itAirplane->data());
                if(pAirplane)
                    pAirplane->SetBearing(false);
            }
        }
    }
}
#endif

void GUIGroundScene::OnSetActivePlane(CAirplane *pPlane)
{
    TAirplanesMap::const_iterator itAirplane = GetAirplanes().begin();
    for(; itAirplane != GetAirplanes().end(); ++itAirplane)
    {
        CGUIAirplane* pAirplane = dynamic_cast<CGUIAirplane*>(itAirplane->data());
        if(pAirplane)
        {
            pAirplane->SetActivationFlag(false);
        }
    }
    CGUIAirplane* pParamPlane = dynamic_cast<CGUIAirplane*>(pPlane);
    pParamPlane->SetActivationFlag(true);    
#ifdef GROUND_CWP
    const QSharedPointer<CAirplane>& pAirplane = GetAirplanes()[pPlane->GetID()];
    CListAC::SelPlaneRecord(pAirplane);
#endif
}

void GUIGroundScene::OnSetFormularDefaultPos()
{
    TAirplanesMap::iterator itAirplans = GetAirplanes().begin();
    for(; itAirplans != GetAirplanes().end(); ++itAirplans)
    {
        CGUIAirplane* pPlane = dynamic_cast<CGUIAirplane*>(itAirplans->data());
        if(pPlane)
            pPlane->SetFormularDefaultPos();
    }

    TVehicleMap::iterator itVehicle = getVehicles().begin();
    for(; itVehicle != getVehicles().end(); ++itVehicle)
    {
        GUIGroundVehicle* pVehicle = dynamic_cast<GUIGroundVehicle*>(itVehicle->data());
        if(pVehicle)
            pVehicle->SetFormularDefaultPos();
    }
}
