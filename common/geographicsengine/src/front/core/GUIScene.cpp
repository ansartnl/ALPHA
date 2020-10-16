#include "GUIScene.h"

#include <QGraphicsScene>
#include <QPen>
#include <QPainter>
#include <QDebug>

#include "GUIPoint.h"
#include "GUIAirport.h"
#include "GUIRoute.h"
#include "GUICorridor.h"
#include "GUISSA.h"
#include "GUISector.h"
#include "GUIRestriction.h"
#include "GUIHolding.h"
#include "front/common/UserSettings.h"
#include "front/common/ZOrderValue.h"

//! Geodesy graphics engine
namespace geoGE {

GUIScene::GUIScene()
{
    setZValue( getZOrderValue(ZOrderFIR) );
	m_ItemList.append(this);
    SetColor( UserSettings::Accessor::instance().color(UserSettings::CtFIR) );

    m_Border.reset(new GUISceneBorder(this));
}

GUIScene::TListItem GUIScene::GetItemList() const
{
    TListItem itemList = m_ItemList;
    if ( m_Border )
        itemList.append(m_Border.data());
    return itemList;
}

void GUIScene::AddPoint(const QSharedPointer<arinc::Point>& pPoint)
{
    try
    {
        Scene::AddPoint(pPoint);
        QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*>(pPoint.data());
        if (pItem)
            m_ItemList.append(pItem);
    }
    catch ( ... )
    {
        // Avoid of adding point twice.
    }
}

void GUIScene::AddAirport(const QSharedPointer<arinc::Airport>& pAirport)
{
    try
    {
        Scene::AddAirport(pAirport);
        QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*>(pAirport.data());
        if (pItem)
            m_ItemList.append(pItem);
            //pItem->setParentItem(this);
            //addToGroup(pItem);
        arinc::Airport::TRunways::const_iterator itScanRun = pAirport->GetRunways().begin();
        for(; itScanRun != pAirport->GetRunways().end(); ++itScanRun)
        {
            GUIAirport* airportGui = dynamic_cast<GUIAirport*>(pAirport.data());
            // Add GUI runway graphic items into scene.
            if ( airportGui )
            {
                GUIAirport::TGUIRunway runwayGui = airportGui->CreateRunway(itScanRun.key());
                if ( runwayGui )
                    m_ItemList.append(runwayGui.data());
            }

            arinc::Runway::TSSA::const_iterator itScanSSA = (*itScanRun)->GetSSA().begin();
            for(; itScanSSA != (*itScanRun)->GetSSA().end(); ++itScanSSA)
            {
                pItem = dynamic_cast<QGraphicsItem*>(itScanSSA->data());
                if (pItem)
                    m_ItemList.append(pItem);
                    //pItem->setParentItem(this);
                    //addToGroup(pItem);
            }
        }
    }
    catch ( ... )
    {
        // Avoid of creation airports with the identical id.
    }
}

void GUIScene::AddRoute(const QSharedPointer<arinc::Route>& pRoute)
{
    try
    {
        Scene::AddRoute(pRoute);
        QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*>(pRoute.data());
        if (pItem)
            m_ItemList.append(pItem);
    }
    catch ( ... )
    {
        // Avoid of adding route twice.
    }
}

void GUIScene::AddCorridor(const QSharedPointer<arinc::Route>& pRoute)
{
    try
    {
        Scene::AddCorridor(pRoute);
        QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*>(pRoute.data());
        if (pItem)
            m_ItemList.append(pItem);
    }
    catch ( ... )
    {
        // Avoid of adding corridor twice.
    }
}

void GUIScene::AddSector(const QSharedPointer<arinc::Scene>& pSector)
{
    Scene::AddSector(pSector);
	QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*>(pSector.data());
	if (pItem)
		m_ItemList.append(pItem);
		//pItem->setParentItem(this);
		//addToGroup(pItem);
}

void GUIScene::AddRestriction(const QSharedPointer<arinc::Restriction>& pRestriction)
{
    Scene::AddRestriction(pRestriction);
	QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*>(pRestriction.data());
	if (pItem)
		m_ItemList.append(pItem);
		//pItem->setParentItem(this);
		//addToGroup(pItem);
}

void GUIScene::AddHolding(const QSharedPointer<arinc::Holding>& pHolding)
{
    Scene::AddHolding(pHolding);
	QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*>(pHolding.data());
	if (pItem)
		m_ItemList.append(pItem);
		//pItem->setParentItem(this);
		//addToGroup(pItem);
}

QSharedPointer<arinc::Point> GUIScene::CreatePoint(arinc::Point::Type type) const
{
//	if (type != arinc::CPoint::enAirportPoint)
        return QSharedPointer<arinc::Point>(new GUIPoint);
//	return Scene::CreatePoint(type);
}

QSharedPointer<arinc::Airport> GUIScene::CreateAirport() const
{
    return QSharedPointer<arinc::Airport>(new GUIAirport);
}

QSharedPointer<arinc::Route> GUIScene::CreateRoute() const
{
    return QSharedPointer<arinc::Route>(new GUIRoute);
}

QSharedPointer<arinc::Route> GUIScene::CreateCorridor() const
{
    return QSharedPointer<arinc::Route>(new GUICorridor);
}

QSharedPointer<arinc::Restriction> GUIScene::CreateRestriction() const
{
    return QSharedPointer<arinc::Restriction>(new GUIRestriction);
}

QSharedPointer<arinc::Holding> GUIScene::CreateHolding() const
{
    return QSharedPointer<arinc::Holding>(new GUIHolding(this));
}

QSharedPointer<arinc::SSA> GUIScene::CreateSSA() const
{
    return QSharedPointer<arinc::SSA>(new GUISSA(this));
}

QSharedPointer<arinc::Scene> GUIScene::CreateSector() const
{
    return QSharedPointer<arinc::Scene>(new GUISector);
}

void GUIScene::Prepare(qint64& iXMin, qint64& iYMin, qint64& iXMax, qint64& iYMax)
{
	TBoundary::iterator itScan = m_Boundary.begin();
	m_qXMin = (*itScan)->GetCoord().GetX();
	m_qYMin = (*itScan)->GetCoord().GetY();
	m_qXSize = m_qXMin;
	m_qYSize = m_qYMin;
	for(++itScan; itScan != m_Boundary.end(); ++itScan)
	{
		if ((*itScan)->GetCoord().GetX() < m_qXMin)
			m_qXMin = (*itScan)->GetCoord().GetX();
		else if ((*itScan)->GetCoord().GetX() > m_qXSize)
			m_qXSize = (*itScan)->GetCoord().GetX();
		if ((*itScan)->GetCoord().GetY() < m_qYMin)
			m_qYMin = (*itScan)->GetCoord().GetY();
		else if ((*itScan)->GetCoord().GetY() > m_qYSize)
			m_qYSize = (*itScan)->GetCoord().GetY();
	}
	iXMin = m_qXMin; iYMin = m_qYMin; iXMax = m_qXSize; iYMax = m_qYSize;

	m_qXSize -= m_qXMin;
	m_qYSize -= m_qYMin;
}

void GUIScene::Prepare(qreal& b, qreal& l) const
{
    // Return value from cache if was initizalized.
    if ( !m_centerPoint.isNull() )
    {
        b = m_centerPoint.m_b;
        l = m_centerPoint.m_l;
        return;
    }

    // Calculate center point in latitude, longitude value ...

    TBoundary::const_iterator itScan = m_Boundary.begin();

    qreal bMin = (*itScan)->GetCoord().GetLatitude().GetAngle(),
          lMin = (*itScan)->GetCoord().GetLongitude().GetAngle();
    qreal bMax = bMin,
          lMax = lMin;

    for(++itScan; itScan != m_Boundary.end(); ++itScan)
    {
        b = (*itScan)->GetCoord().GetLatitude().GetAngle();
        l = (*itScan)->GetCoord().GetLongitude().GetAngle();

        bMin = qMin(bMin, b);
        bMax = qMax(bMax, b);

        lMin = qMin(lMin, l);
        lMax = qMax(lMax, l);
    }

    b = (bMin + bMax) / 2.0;
    l = (lMin + lMax) / 2.0;

    // Cache center point.
    m_centerPoint.m_b = b;
    m_centerPoint.m_l = l;
}

QRectF GUIScene::boundingRect() const
{
    return m_boundingRect;
}

QPainterPath GUIScene::shape() const
{
	return m_Path;
}

void GUIScene::paint(
	QPainter *painter,
	const QStyleOptionGraphicsItem *option,
	QWidget *widget)
{
    painter->setBrush( GetColor() );
    painter->drawPath(m_Path);
}

void GUIScene::SetPosition(const TCoordSystem &cs)
{
    prepareGeometryChange();

    m_Path = QPainterPath();

    TBoundary::const_iterator itScan = GetBoundary().begin();
    for(; itScan != GetBoundary().end(); ++itScan)
    {
        const CCoord &coord = (*itScan)->GetCoord();
        QPointF pnt = cs->toScene(QVector3D(coord.GetX(), coord.GetY(), coord.GetZ()));

        if (itScan == GetBoundary().begin())
            m_Path.moveTo(pnt);
        else
            m_Path.lineTo(pnt);
    }
    m_Path.closeSubpath();

    m_boundingRect = m_Path.boundingRect();
    m_boundingRect.adjust(-4,-4,4,4);

    update();
}

void GUIScene::SetCorridorColor(const QColor& color)
{
    typedef arinc::Scene::TCorridorsMap  TCorridorsMap;
    TCorridorsMap& corridors = GetCorridors();

    for ( TCorridorsMap::iterator itCorr = corridors.begin();
        itCorr != corridors.end(); ++itCorr )
    {
        GUICorridor* pCorridor = dynamic_cast<GUICorridor*>( itCorr->data() );
        if ( pCorridor )
            pCorridor->SetColor( color );
    }
}

void GUIScene::SetMapColor(const QColor& color)
{
    // Map changes points color
    typedef arinc::Scene::TPointsMap  TPointsMap;
    TPointsMap& points = GetPoints();

    for ( TPointsMap::iterator it = points.begin();
        it != points.end(); ++it )
    {
        GUIPoint* pPoint = dynamic_cast<GUIPoint*>( it->data() );
        if ( pPoint )
            pPoint->SetColor( color );
    }

    // Map changes routes color
    typedef arinc::Scene::TRoutsMap  TRoutesMap;
    TRoutesMap& routes = GetRoutes();

    for ( TRoutesMap::iterator it = routes.begin();
        it != routes.end(); ++it )
    {
        GUIRoute* pRoute = dynamic_cast<GUIRoute*>( it->data() );
        if ( pRoute )
            pRoute->SetColor( color );
    }

    // Map changes airports color
    typedef arinc::Scene::TAirportsMap   TAirportsMap;
    TAirportsMap& airports = GetAirports();

    for ( TAirportsMap::iterator it = airports.begin();
        it != airports.end(); ++it )
    {
        GUIAirport* pAirport = dynamic_cast<GUIAirport*>( it->data() );
        if ( pAirport )
        {
            pAirport->SetColor( color );
            //
            foreach ( const GUIAirport::TGUIRunway &guiRunway, pAirport->GetRunwayItems() )
                guiRunway->SetColor( color );
        }
    }

    // Map changes holdings color
    typedef arinc::Scene::THoldingList   THoldingList;
    THoldingList& holdings = GetHolding();

    for ( THoldingList::iterator it = holdings.begin();
        it != holdings.end(); ++it )
    {
        GUIHolding* pHolding = dynamic_cast<GUIHolding*>( it->data() );
        if ( pHolding )
            pHolding->SetColor( color );
    }
}

void GUIScene::SetBorderColor(const QColor& color)
{
    if ( m_Border )
        m_Border->SetColor(color);
}

void GUIScene::SetSectorColor(const QColor& color)
{
    typedef arinc::Scene::TSceneList TSceneList;
    TSceneList& sectors = GetSectorList();

    for ( TSceneList::iterator it = sectors.begin();
        it != sectors.end(); ++it )
    {
        GUISector* pSector = dynamic_cast<GUISector*>( it->data() );
        if ( pSector )
            pSector->SetColor( color );
    }
}

void GUIScene::SetApproachColor(const QColor& color)
{
    // Map changes for SID, STAR and APPROACH color (SSA)
    typedef arinc::Scene::TAirportsMap   TAirportsMap;
    TAirportsMap& airports = GetAirports();

    for ( TAirportsMap::iterator it = airports.begin();
        it != airports.end(); ++it )
    {
        GUIAirport* pAirport = dynamic_cast<GUIAirport*>( it->data() );
        if ( pAirport )
        {
            // Map changes - runways color
            typedef arinc::Airport::TRunways     TRunways;
            TRunways& runways = pAirport->GetRunways();

            for ( TRunways::iterator itRW = runways.begin();
                itRW != runways.end(); ++itRW )
            {
                typedef arinc::Runway::TSSA  TSSA;
                TSSA& ssa = (*itRW)->GetSSA();

                for ( TSSA::iterator itSSA = ssa.begin();
                    itSSA != ssa.end(); ++itSSA )
                {
                    GUISSA* pSSA = dynamic_cast<GUISSA*>( itSSA->data() );
                    if ( pSSA )
                        pSSA->SetColor( color );
                }
            }
        }
    }
}


/////////////////////////////////////////////////////////////////////////////////////
// GUISceneBorder implementation.

GUISceneBorder::GUISceneBorder(GUIScene *scene)
    : mScene(scene)
{
    setZValue( getZOrderValue(ZOrderFIRBorder) );
    mColor = UserSettings::Accessor::instance().color(UserSettings::CtBorder);
}

QRectF GUISceneBorder::boundingRect() const
{
    return mScene->m_boundingRect;
}

QPainterPath GUISceneBorder::shape() const
{
    return mScene->m_Path;
}

void GUISceneBorder::paint(
    QPainter *painter,
    const QStyleOptionGraphicsItem *option,
    QWidget *widget)
{
    QPen qPen( mColor );
    qPen.setCosmetic(true);
    qPen.setWidth(2);
    painter->setPen(qPen);

    painter->drawPath(mScene->m_Path);
}

}   // namespace geoGE
