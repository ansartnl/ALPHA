#include "GUIView.h"

#include <math.h>
#include <QWheelEvent>
#include <QGraphicsView>
#include <QScrollBar>
#include <QIcon>
#include <QDateTime>
#include "GUIScene.h"
#include "GUIPeleng.h"
#include "GUIMeteoVector.h"
#include "GUIMeteoContour.h"
#include "GUIPoint.h"
#include "GUISSA.h"
#include "GUICorridor.h"
#include "GUIAirport.h"
#include "GUIRoute.h"
#include "GUIRestriction.h"
#include "GUIHolding.h"
#include "front/common/UserSettings.h"


//! Geodesy graphics engine
namespace geoGE {

// Mouse press position.
QPoint gMouseOrigin;

GUIView::GUIView(IParent* pParent)
    : m_iScale(0)
    , m_pParent(pParent), m_bPull(false)
    , m_xratio(1.0), m_yratio(1.0)
    , m_viewMode(int(VM_None))
    , mZoomRect(0)
{
	setMouseTracking(true);

    m_scaleRuler.SetColor( UserSettings::Accessor::instance().color(UserSettings::CtMap) );

    double pixelInMmW = (double)logicalDpiX() / 25.4;
    double pixelInMmH = (double)logicalDpiY() / 25.4;
    // 50 km = 1cm
    GetCoordSys()->setSceneTransform(500000, pixelInMmW, pixelInMmH);

    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
}

GUIView::~GUIView()
{
	TSceneList::iterator itSceneScan = m_SceneList.begin();
	for(; itSceneScan != m_SceneList.end(); ++itSceneScan)
	{
        GUIScene* pScene = dynamic_cast<GUIScene*>(itSceneScan->data());
		if (pScene)
		{
            GUIScene::TListItem items = pScene->GetItemList();
            GUIScene::TListItem::const_iterator itItemScan = items.begin();
            for(; itItemScan != items.end(); ++itItemScan)
				scene()->removeItem(*itItemScan);
		}
	}
}

void GUIView::AddScene(const QString& sKey, const TScenePtr& pScene)
{
    View::AddScene(sKey, pScene);

    GUIScene* pGUIScene = dynamic_cast<GUIScene*>(pScene.data());
	if (pScene)
	{
        GUIScene::TListItem items = pGUIScene->GetItemList();
        GUIScene::TListItem::const_iterator itItemScan = items.begin();
        for(; itItemScan != items.end(); ++itItemScan)
            scene()->addItem(*itItemScan);
    }
}

void GUIView::RemoveScene(const QString& sKey)
{
    View::TSceneList::iterator itScene = GetScenes().find(sKey);
    if ( itScene == GetScenes().end() )
        return;

    GUIScene* pGUIScene = dynamic_cast<GUIScene*>((*itScene).data());
    if (pGUIScene)
    {
        GUIScene::TListItem items = pGUIScene->GetItemList();
        GUIScene::TListItem::const_iterator itItemScan = items.begin();
        for(; itItemScan != items.end(); ++itItemScan)
            scene()->removeItem(*itItemScan);
    }

    View::RemoveScene(sKey);
}

void GUIView::AddMeteo(const QSharedPointer<MeteoVector>& pMeteoVec)
{
    View::AddMeteo(pMeteoVec);
    QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*>(pMeteoVec.data());
    if (pItem)
        scene()->addItem(pItem);
}

void GUIView::AddMeteo(const QSharedPointer<MeteoContour>& pMeteoZone)
{
    View::AddMeteo(pMeteoZone);
    QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*>(pMeteoZone.data());
    if (pItem)
        scene()->addItem(pItem);
}

void GUIView::AddPeleng(const QSharedPointer<Peleng>& pPeleng)
{
    View::AddPeleng(pPeleng);
    QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*>(pPeleng.data());
    if (pItem)
        scene()->addItem(pItem);
}

QSharedPointer<MeteoVector> GUIView::CreateMeteoVec() const
{
    return QSharedPointer<MeteoVector>(new GUIMeteoVector);
}

QSharedPointer<MeteoContour> GUIView::CreateMeteoZone() const
{
    return QSharedPointer<MeteoContour>(new GUIMeteoContour);
}

QSharedPointer<Peleng> GUIView::CreatePeleng() const
{
    return QSharedPointer<Peleng>(new GUIPeleng);
}

void GUIView::SetZoom(int iValue)
{
	int iDelta = iValue - m_iScale;
	m_iScale = iValue;

    if ( iDelta != 0 )
        Zoom(viewport()->rect().center(), iDelta);
}

void GUIView::ResetZoom()
{
    m_iScale = 0;
}

void GUIView::Prepare(qreal b, qreal l, qreal angle)
{
    // If there are no loaded FIRs - define empty scene
    if ( m_SceneList.empty() )
    {
        PrepareEmpty(b, l, angle);
        return;
    }

    // Set center point for coordinate system for first FIR only. Other FIRs will
    // use first FIR center point.
    if ( !m_SceneList.isEmpty() && m_SceneList.size() < 2 )
    {
        GUIScene* pScene = dynamic_cast<GUIScene*>(m_SceneList.begin()->data());
        if (pScene)
            pScene->Prepare(b, l);

        // May be temporary solution to set new coordinate system center point.
        // Center point is retrieved form the first FIR in the scene.
        // Just recalculate coordinates.
        View::SetCenterPoint(b, l, angle);

        // Apply settings of a grid once first FIR is loaded.
        // Update grid settings.
        OnUpdateGrid();
    }

    UpdateSceneRect();

    Draw(true);
//    ViewportUpdateMode mode = viewportUpdateMode();
//    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
//    //resetCachedContent();
//    scene()->invalidate(mapToScene(viewport()->rect()).boundingRect());
//    scene()->update(mapToScene(viewport()->rect()).boundingRect());
//    setViewportUpdateMode(mode);

	SetLayouts();

    m_scaleRuler.SetVisible(true);
}

void GUIView::PrepareEmpty(qreal b, qreal l, qreal angle)
{
//    // Default center and angle.
//    qreal b = 54.0,
//        l = 28.0,
//        angle = 0.0;

    // May be temporary solution to set new coordinate system center point.
    // Center point is retrieved form the first FIR in the scene.
    // Just recalculate coordinates.
    View::SetCenterPoint(b, l, angle);

    // Apply settings of a grid once first FIR is loaded.
    // Update grid settings.
    OnUpdateGrid();

    UpdateSceneRect();

    Draw(true);
//    ViewportUpdateMode mode = viewportUpdateMode();
//    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
//    //resetCachedContent();
//    scene()->invalidate(mapToScene(viewport()->rect()).boundingRect());
//    scene()->update(mapToScene(viewport()->rect()).boundingRect());
//    setViewportUpdateMode(mode);

//    SetLayouts();

    m_scaleRuler.SetVisible(true);
}

void GUIView::UpdateSceneRect()
{
    const TCoordSystem& pCS = GetCoordSys();

    QPointF pointMin(0.0, 0.0),
            pointMax(0.0, 0.0);

    if ( !m_SceneList.empty() )
    {
        qint64 xMin, yMin, xMax, yMax;
        xMin = yMin = INT_MAX;
        xMax = yMax = -INT_MAX;

        TSceneList::iterator itSceneScan = m_SceneList.begin();
        for(; itSceneScan != m_SceneList.end(); ++itSceneScan)
        {
            GUIScene* pScene = dynamic_cast<GUIScene*>(itSceneScan->data());
            if (pScene)
            {
                qint64 iXMin(0), iYMin(0), iXMax(0), iYMax(0);
                pScene->Prepare(iXMin, iYMin, iXMax, iYMax);

                xMin = qMin(xMin, iXMin);
                yMin = qMin(yMin, iYMin);
                xMax = qMax(xMax, iXMax);
                yMax = qMax(yMax, iYMax);
            }
        }

        QPointF point1 = pCS->toScene(QVector3D(xMin, yMin, 0.0));
        QPointF point2 = pCS->toScene(QVector3D(xMin, yMax, 0.0));
        QPointF point3 = pCS->toScene(QVector3D(xMax, yMax, 0.0));
        QPointF point4 = pCS->toScene(QVector3D(xMax, yMin, 0.0));
        QPointF pointCP = pCS->toScene(pCS->toModelDegree(
                QVector3D(pCS->getCenterLatitude(), pCS->getCenterLongitude(), 0.0)));

        int xmin = qMin(point1.x(), qMin(point2.x(), qMin(point3.x(), qMin(point4.x(), pointCP.x())))),
            xmax = qMax(point1.x(), qMax(point2.x(), qMax(point3.x(), qMax(point4.x(), pointCP.x())))),
            ymin = qMin(point1.y(), qMin(point2.y(), qMin(point3.y(), qMin(point4.y(), pointCP.y())))),
            ymax = qMax(point1.y(), qMax(point2.y(), qMax(point3.y(), qMax(point4.y(), pointCP.y()))));

        pointMin = QPointF(xmin, ymin);
        pointMax = QPointF(xmax, ymax);
    }

    qreal kmAdd = pCS->distanceToScene(10000000);

    pointMin -= QPointF(kmAdd, kmAdd);
    pointMax += QPointF(kmAdd, kmAdd);

    QRectF rectScene(pointMin.x(), pointMin.y(),
                     pointMax.x()-pointMin.x(), pointMax.y()-pointMin.y());

    scene()->setSceneRect(rectScene);
    setSceneRect(rectScene);
}

void GUIView::wheelEvent(QWheelEvent* event)
{
    if (!event->buttons())
    {
		int numSteps = event->delta() / (8.0 * 15.0);

        if ((m_iScale < MAX_SCALE && numSteps > 0) ||
            (m_iScale > 0 && numSteps < 0))
        {
			m_iScale += numSteps;

			if (m_iScale > MAX_SCALE)
				m_iScale = MAX_SCALE;
			else if (m_iScale + numSteps < 0)
				m_iScale = 0;

            Zoom(event->pos(), numSteps);

			if (m_pParent)
				m_pParent->ZoomEvent(m_iScale);
        }
	} else
	{
		QGraphicsView::wheelEvent(event);
	}
}

void GUIView::mouseMoveEvent(QMouseEvent * event)
{
	if (m_bPull)
	{
        moveToPos( event->pos() );

	} else
	{
		if (m_pParent)
			m_pParent->CoordinateEvent(GetCoordAtPoint(mapToScene(event->pos())));

        // Zoom rect
        if ( m_viewMode == (int)VM_ZoomRect )
        {
            if ( mZoomRect )
                mZoomRect->setGeometry(QRect(gMouseOrigin, event->pos()).normalized());
        }
    }
	QGraphicsView::mouseMoveEvent(event);
}

void GUIView::mousePressEvent(QMouseEvent *event)
{
	QGraphicsView::mousePressEvent(event);

    if (event->buttons() == Qt::MidButton)
	{
		setCursor(Qt::ClosedHandCursor);
		m_bPull = true;
        m_PullPoint = (mapToScene(event->pos()));
    }
    else
	{
        if(m_bPull)
            unsetCursor();
		m_bPull = false;

        if ( event->button() == Qt::LeftButton )
        {
            // Move current mouse position to center of the view.
            if ( m_viewMode == (int)VM_MoveCenter )
            {
                centerOn( mapToScene(event->pos()) );
                SetViewMode(VM_None);
            }

            // Zoom rect
            else if ( m_viewMode == (int)VM_ZoomRect )
            {
                gMouseOrigin = event->pos();
                if ( !mZoomRect )
                    mZoomRect = new QRubberBand(QRubberBand::Rectangle, this);
                mZoomRect->setGeometry(QRect(gMouseOrigin, QSize()));
                mZoomRect->show();
            }
        }
    }
}

void GUIView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MidButton)
	{
		if(m_bPull)
		{
            moveToPos( event->pos() );
            unsetCursor();
		}
		m_bPull = false;

        // Restore cursor for some modes.
        if ( m_viewMode == (int)VM_MoveCenter || m_viewMode == (int)VM_ZoomRect )
        {
            int viewMode = m_viewMode;
            m_viewMode = VM_None;
            SetViewMode(viewMode);
        }
	}
    else
    {
        // Zoom rect
        if ( m_viewMode == (int)VM_ZoomRect )
        {
            if ( mZoomRect )
            {
                QRect rectZoom = QRect(gMouseOrigin, event->pos()).normalized();
                mZoomRect->setGeometry(rectZoom);
                mZoomRect->hide();
                // Change zoom only if user selected bigger than 2x2 pixel rectangle.
                if ( rectZoom.width() > 2 && rectZoom.height() > 2 )
                    emit zoomChanged( rectZoom );
                mZoomRect->deleteLater();
                mZoomRect = 0;
            }
        }
    }
	QGraphicsView::mouseReleaseEvent(event);
}

void GUIView::keyPressEvent(QKeyEvent *event)
{
	if(event->key() == Qt::Key_Escape)
	{
		unsetCursor();

        SetViewMode(VM_None);
	}
	QGraphicsView::keyPressEvent(event);
}

void GUIView::moveToPos(const QPoint &pos)
{
    QPointF NewPoint(mapToScene(pos));
    QPointF DeltaPoint = NewPoint - m_PullPoint;

    QPointF CenterPoint = mapToScene(viewport()->rect().center());
    CenterPoint -= DeltaPoint;
    // centerOn(CenterPoint);
    QMetaObject::invokeMethod(this, "onCenterOn", Qt::QueuedConnection, Q_ARG(QPointF, CenterPoint));
    m_PullPoint = NewPoint - DeltaPoint;
}

void GUIView::onCenterOn(QPointF CenterPoint)
{
    centerOn(CenterPoint);
}

void GUIView::drawForeground(QPainter *painter, const QRectF &rect)
{
    QGraphicsView::drawForeground(painter, rect);

    const UserSettings::Grid& grid = UserSettings::Accessor::instance().grid();

    // Optimization include
    if ( !grid.mIsScale )
        return;

    bool isWorldMatrixEnabled = painter->worldMatrixEnabled();
    painter->setWorldMatrixEnabled(false);

    // Draw scaler
    if ( grid.mIsScale )
    {
        // Need these transformations for printing.
        QRect rectRuler = viewport()->rect();
        rectRuler.setWidth(rectRuler.width()*m_xratio);
        rectRuler.setHeight(rectRuler.height()*m_xratio);

        m_scaleRuler.paint(*painter, rectRuler);
    }

    painter->setWorldMatrixEnabled(isWorldMatrixEnabled);
}

void GUIView::scrollContentsBy(int dx, int dy)
{
    QGraphicsView::scrollContentsBy(dx, dy);

    const UserSettings::Grid& grid = UserSettings::Accessor::instance().grid();

    // Update position of grid labels if mode is set.
    if ( grid.mIsAutoLabelPos )
    {
        for ( TGridMap::iterator it = m_grids.begin(); it != m_grids.end(); ++it )
            (*it)->UpdateLabelPosition();
    }

    // Optimization include
    if ( !grid.mIsScale )
        return;

    int dyAbs = qAbs(dy);
//    int dyAbs = qAbs(dy),
//        dxAbs = qAbs(dx);

    // Update viewport screen here to show unmoved objects.

    // Update scale ruler at the bottom-right of the screen.
    if ( grid.mIsScale )
    {
        QRect rectViewport = viewport()->rect();
        viewport()->update(0,rectViewport.bottom()-50-dyAbs, rectViewport.width(), 50+dyAbs);
    }

//    viewport()->update();
}

void GUIView::Draw(bool bForce)
{
	QList<QGraphicsItem*> ItemList = items();
	QList<QGraphicsItem*>::const_iterator itScan = ItemList.begin();
	for(; itScan != ItemList.end(); ++itScan)
	{
//        GUIScene* pSceneItem = dynamic_cast<GUIScene*>(*itScan);
//        if ( pSceneItem )
//        {
//            pSceneItem->SetPosition( GetCoordSys() );
//            continue;
//        }

        IGUIItemHandler* pItem = dynamic_cast<IGUIItemHandler*>(*itScan);
        if (pItem && (bForce || pItem->IsTransformNeeded()) )
        {
            pItem->SetPosition( GetCoordSys() );
        }
	}

//    for ( TGridMap::iterator it = m_grids.begin(); it != m_grids.end(); ++it )
//    {
//        IGUIItemHandler* pItem = dynamic_cast<IGUIItemHandler*>(it->data());
//        if (pItem && (bForce || pItem->IsTransformNeeded()) )
//        {
//            pItem->SetPosition( GetCoordSys() );
//        }
//    }


    // Update items visibility - relative to scale ...

    // Distance meters/cm
    qreal distanceModel = distanceModelInCm();

    // Update visibility of items in each scene.
    for ( TSceneList::const_iterator itScene = GetScenes().begin(); itScene != GetScenes().end(); ++itScene )
    {
        GUIScene* pScene = dynamic_cast<GUIScene*>(itScene->data());
        if ( !pScene )
            continue;

        // Points
        typedef arinc::Scene::TPointsMap    TPointsMap;
        TPointsMap &points = pScene->GetPoints();
        for ( TPointsMap::iterator it = points.begin(); it != points.end(); ++it )
        {
            geoGE::GUIPoint* guiPoint = static_cast<geoGE::GUIPoint*>((*it).data());

            qreal distanceModelMax = guiPoint->GetType() == arinc::Point::enWaypoint ? 70000 : 5000;
            updateItemVisibility(guiPoint, distanceModel, 0.0, distanceModelMax);
        }

        // Airports
        typedef arinc::Scene::TAirportsMap  TAirportsMap;
        TAirportsMap &airports = pScene->GetAirports();
        for ( TAirportsMap::iterator ita = airports.begin(); ita != airports.end(); ++ita )
        {
            arinc::Airport *airport = (*ita).data();

            geoGE::GUIAirport* guiAirport = static_cast<geoGE::GUIAirport*>(airport);
            updateItemVisibility(guiAirport, distanceModel, 0.0, 35000);

            // Runways
            typedef GUIAirport::TGUIRunways     TGUIRunways;
            TGUIRunways runwayItems = guiAirport->GetRunwayItems();
            for ( TGUIRunways::iterator itr = runwayItems.begin(); itr != runwayItems.end(); ++itr )
            {
                GUIRunway *guiRunway = (*itr).data();
                updateItemVisibility(guiRunway, distanceModel, 0.0, 35000);
            }

            typedef arinc::Airport::TRunways    TRunways;
            TRunways &runways = airport->GetRunways();
            for ( TRunways::iterator itr = runways.begin(); itr != runways.end(); ++itr )
            {
                arinc::Runway *runway = (*itr).data();

                // Sid Star Approach
                typedef arinc::Runway::TSSA     TSSA;
                TSSA &ssaMap = runway->GetSSA();
                for ( TSSA::iterator itssa = ssaMap.begin(); itssa != ssaMap.end(); ++itssa )
                {
                    geoGE::GUISSA* guiSSA = static_cast<geoGE::GUISSA*>( (*itssa).data() );
                    updateItemVisibility(guiSSA, distanceModel, 0.0, 35000);
                }
            }
        }

        // Corridors
        typedef arinc::Scene::TCorridorsMap    TCorridorsMap;
        TCorridorsMap &corridors = pScene->GetCorridors();
        for ( TCorridorsMap::iterator it = corridors.begin(); it != corridors.end(); ++it )
        {
            geoGE::GUICorridor* guiCorridor = static_cast<geoGE::GUICorridor*>((*it).data());
            updateItemVisibility(guiCorridor, distanceModel, 0.0, 40000);
        }

        // Routes
        typedef arinc::Scene::TRoutsMap    TRoutsMap;
        TRoutsMap &routs = pScene->GetRoutes();
        for ( TRoutsMap::iterator it = routs.begin(); it != routs.end(); ++it )
        {
            geoGE::GUIRoute* guiRoute = static_cast<geoGE::GUIRoute*>((*it).data());
            updateItemVisibility(guiRoute, distanceModel, 0.0, 150000);
        }

        // Restrictions
        typedef arinc::Scene::TRestrictionList    TRestrictionList;
        TRestrictionList &restrictions = pScene->GetRestriction();
        for ( TRestrictionList::iterator it = restrictions.begin(); it != restrictions.end(); ++it )
        {
            geoGE::GUIRestriction* guiRestriction = static_cast<geoGE::GUIRestriction*>((*it).data());
            updateItemVisibility(guiRestriction, distanceModel, 0.0, 200000);
        }

        // Holdings
        typedef arinc::Scene::THoldingList    THoldingList;
        THoldingList &holdings = pScene->GetHolding();
        for ( THoldingList::iterator it = holdings.begin(); it != holdings.end(); ++it )
        {
            geoGE::GUIHolding* guiHolding = static_cast<geoGE::GUIHolding*>((*it).data());
            updateItemVisibility(guiHolding, distanceModel, 0.0, 45000);
        }
    }
}

void GUIView::RenderDocument(QPainter * painter, const QRectF & target,
                              const QRect & source,
                              Qt::AspectRatioMode aspectRatioMode)
{
    // Default source rect = viewport rect
    QRect sourceRect = source;
    if (source.isNull())
        sourceRect = viewport()->rect();

    // Default target rect = device rect
    QRectF targetRect = target;
    if (target.isNull()) {
        if (painter->device()->devType() == QInternal::Picture)
            targetRect = sourceRect;
        else
            targetRect.setRect(0, 0, painter->device()->width(), painter->device()->height());
    }

    // Find the ideal x / y scaling ratio to fit \a source into \a target.
    m_xratio = targetRect.width() / sourceRect.width();
    m_yratio = targetRect.height() / sourceRect.height();

    m_xratio = m_yratio = qMin(m_xratio, m_yratio);

    TCoordSystem& pCS = GetCoordSys();

    // Prepare untransformable items for rendering ...

    QList<QGraphicsItem*> ItemList = items();
    QList<QGraphicsItem*>::const_iterator itScan = ItemList.begin();
    for(; itScan != ItemList.end(); ++itScan)
    {
        GUIUntransformableObject* pUntransItem = dynamic_cast<GUIUntransformableObject*>(*itScan);
        if ( pUntransItem )
        {
            pUntransItem->SetAspectRatio(m_xratio, m_yratio);
            // Update item
            IGUIItemHandler* pItemHandler = dynamic_cast<IGUIItemHandler*>(*itScan);
            if ( pItemHandler )
                pItemHandler->SetPosition( pCS );
        }
    }

    // Render fitting the viewport contents into a full page
    render(painter);

    // Restore aspect ratio for untransformable items ...

    for(itScan = ItemList.begin(); itScan != ItemList.end(); ++itScan)
    {
        GUIUntransformableObject* pUntransItem = dynamic_cast<GUIUntransformableObject*>(*itScan);
        if ( pUntransItem )
        {
            pUntransItem->SetAspectRatio(1.0, 1.0);
            // Update item
            IGUIItemHandler* pItemHandler = dynamic_cast<IGUIItemHandler*>(*itScan);
            if ( pItemHandler )
                pItemHandler->SetPosition( pCS );
        }
    }

    // Restore ratio factors.
    m_xratio = m_yratio = 1.0;
}

void GUIView::Zoom(const QPoint& MousePos, int iCurScale)
{
    QPointF CenterPoint = mapToScene(viewport()->rect().center());
    QPointF pointPosScene1 = mapToScene( MousePos );

    double scaleFactor = pow(1.125, iCurScale);
    scale(scaleFactor, scaleFactor);

    QPointF pointPosScene2 = mapToScene( MousePos );

    TCoordSystem& pCS = GetCoordSys();

    pCS->setSceneTransform( transform() );
    pCS->setViewportTransform( viewportTransform() );

    CenterPoint -= pointPosScene2 - pointPosScene1;
    centerOn(CenterPoint);

    // Change ruler distance in 1 cm.
    m_scaleRuler.SetIntervalDistance( distanceModelInCm() / 1000.0 );

    Draw();
//    ViewportUpdateMode mode = viewportUpdateMode();
//    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
//    //resetCachedContent();
//    scene()->invalidate(mapToScene(viewport()->rect()).boundingRect());
//    scene()->update(mapToScene(viewport()->rect()).boundingRect());
//    setViewportUpdateMode(mode);
}

void GUIView::UpdateMeteo(const MeteoVector& meteoVector)
{
    View::UpdateMeteo(meteoVector);

    const TMeteoVecMap::const_iterator itc = GetMeteoVec().find(meteoVector.GetID());
    if ( itc != GetMeteoVec().end() )
        static_cast<GUIMeteoVector *>(itc->data())->SetPosition( GetCoordSys() );
}

void GUIView::UpdateMeteo(const MeteoContour& meteoZone)
{
    View::UpdateMeteo(meteoZone);

    const TMeteoZoneMap::const_iterator itc = GetMeteoZones().find(meteoZone.GetID());
    if ( itc != GetMeteoZones().end() )
        static_cast<GUIMeteoContour *>(itc->data())->SetPosition( GetCoordSys() );
}

void GUIView::UpdatePeleng(const Peleng& peleng)
{
    View::UpdatePeleng(peleng);

    // Update peleng object according to the newly received data.
    TPelengMap::const_iterator itc = GetPelengs().find(peleng.GetID());
    if ( itc != GetPelengs().constEnd() )
    {
        Q_ASSERT(dynamic_cast<GUIPeleng*>(itc->data()));
        static_cast<GUIPeleng*>(itc->data())->SetPosition( GetCoordSys() );
    }
}

CCoord GUIView::GetCoordAtPoint(QPointF pos) const
{
	CCoord Coordinate;

    const TCoordSystem& pCS = GetCoordSys();

    QVector3D pointModel = pCS->toModel(pos);
    Coordinate.SetX(pointModel.x());
    Coordinate.SetY(pointModel.y());
    Coordinate.SetZ(pointModel.z());

    pCS->toGeo( Coordinate );

	return Coordinate;
}

void GUIView::SetLayouts()
{
	TSceneList::const_iterator itScene = GetScenes().begin();
	for(; itScene != GetScenes().end(); ++itScene)
	{
        GUIScene* pScene = dynamic_cast<GUIScene*>(itScene->data());
		if(pScene) pScene->SetLayouts();
	}

	Draw();
}

void GUIView::setCursor(const QCursor& cursor)
{
	viewport()->setCursor(cursor);
	QGraphicsView::setCursor(cursor);
}

void GUIView::unsetCursor()
{
	viewport()->unsetCursor();
	QGraphicsView::unsetCursor();
}

void GUIView::OnUpdateGrid()
{
    OnUpdateAzimuthGrid();
    OnUpdateCartesianGrid();
    OnUpdateGeodesicGrid();
    OnUpdateScaleRuler();
}

void GUIView::OnUpdateAzimuthGrid()
{
    const QString nameAzimuthGrid = ("Azimuth");

    const UserSettings::Grid& grid = UserSettings::Accessor::instance().grid();

    TGridMap::iterator itA = m_grids.find(nameAzimuthGrid);

    // Destroy azimuth grid if needed
    if ( !grid.mIsAzimuthRange && itA != m_grids.end() )
    {
        scene()->removeItem( (*itA).data() );
        m_grids.erase(itA);
    }

    // Create or update azimuth grid if needed
    if ( grid.mIsAzimuthRange )
    {
        QSharedPointer<GUIGrid> pGrid;
        if ( itA != m_grids.end() )
            pGrid = *itA;
        else
        {
            pGrid = QSharedPointer<GUIGrid>(new GUIGridAzimuth);
            m_grids[nameAzimuthGrid] = pGrid;

            scene()->addItem( pGrid.data() );
        }

        GUIGridAzimuth* pAGrid = dynamic_cast<GUIGridAzimuth*>(pGrid.data());
        if ( pAGrid )
        {
            pAGrid->SetUnits((GUIGridRange::Units)grid.mDistUnit);
            pAGrid->SetDistance(grid.mRange);
            pAGrid->SetAzimuth(grid.mAzimuth);
            pAGrid->SetFullscreen(grid.mIsFullScreen);
            pAGrid->SetGridFlags(grid.mIsAutoLabelPos ?
                                 (pAGrid->GetGridFlags() | GUIGrid::AutoLabelPosition) :
                                 (pAGrid->GetGridFlags() & ~GUIGrid::AutoLabelPosition));
            // Update graphics scene
            pAGrid->SetPosition(GetCoordSys());
        }
    }
}

void GUIView::OnUpdateCartesianGrid()
{
    const QString nameCartesianGrid = ("Cartesian");

    const UserSettings::Grid& grid = UserSettings::Accessor::instance().grid();

    TGridMap::iterator itC = m_grids.find(nameCartesianGrid);

    // Destroy cartesian grid if needed
    if ( !grid.mIsCartesian && itC != m_grids.end() )
    {
        scene()->removeItem( (*itC).data() );
        m_grids.erase(itC);
    }

    // Create or update cartesian grid if needed
    if ( grid.mIsCartesian )
    {
        QSharedPointer<GUIGrid> pGrid;
        if ( itC != m_grids.end() )
            pGrid = *itC;
        else
        {
            pGrid = QSharedPointer<GUIGrid>(new GUIGridCartesian);
            m_grids[nameCartesianGrid] = pGrid;

            scene()->addItem( pGrid.data() );
        }

        GUIGridCartesian* pCGrid = dynamic_cast<GUIGridCartesian*>(pGrid.data());
        if ( pCGrid )
        {
            pCGrid->SetUnits((GUIGridRange::Units)grid.mDistUnit);
            pCGrid->SetDistance(grid.mRange);
            pCGrid->SetFullscreen(grid.mIsFullScreen);
            pCGrid->SetGridFlags(grid.mIsAutoLabelPos ?
                                 (pCGrid->GetGridFlags() | GUIGrid::AutoLabelPosition) :
                                 (pCGrid->GetGridFlags() & ~GUIGrid::AutoLabelPosition));
            // Update graphics scene
            pCGrid->SetPosition(GetCoordSys());
        }
    }
}

void GUIView::OnUpdateGeodesicGrid()
{
    const QString nameGeodesicGrid = ("Geodesic");

    const UserSettings::Grid& grid = UserSettings::Accessor::instance().grid();

    TGridMap::iterator itG = m_grids.find(nameGeodesicGrid);

    // Destroy geodesic grid if needed
    if ( !grid.mIsGeodesic && itG != m_grids.end() )
    {
        scene()->removeItem( (*itG).data() );
        m_grids.erase(itG);
    }

    // Create or update geodesic grid if needed
    if ( grid.mIsGeodesic )
    {
        QSharedPointer<GUIGrid> pGrid;
        if ( itG != m_grids.end() )
            pGrid = *itG;
        else
        {
            pGrid = QSharedPointer<GUIGrid>(new GUIGridGeodesic);
            m_grids[nameGeodesicGrid] = pGrid;

            scene()->addItem( pGrid.data() );
        }

        GUIGridGeodesic* pGGrid = dynamic_cast<GUIGridGeodesic*>(pGrid.data());
        if ( pGGrid )
        {
            pGGrid->SetFullscreen(grid.mIsFullScreen);
            pGGrid->SetGridFlags(grid.mIsAutoLabelPos ?
                                 (pGGrid->GetGridFlags() | GUIGrid::AutoLabelPosition) :
                                 (pGGrid->GetGridFlags() & ~GUIGrid::AutoLabelPosition));
            // Update graphics scene
            pGGrid->SetPosition(GetCoordSys());
        }
    }
}

void GUIView::OnUpdateScaleRuler()
{
    const UserSettings::Grid& grid = UserSettings::Accessor::instance().grid();

    m_scaleRuler.SetUnits((CXScaleRuler::Units)grid.mDistUnit);

    double pixelInMmW = (double)width() / (double)widthMM();
    double pixelInMmH = (double)height() / (double)heightMM();
    double pixelInCm = ((pixelInMmW * 10.0) + (pixelInMmH * 10.0)) / 2.0;

    m_scaleRuler.SetUnits((CXScaleRuler::Units)grid.mDistUnit);
    m_scaleRuler.SetIntervalLength((int)(pixelInCm *
        (grid.mDistUnit == UserSettings::Grid::Kilometers ? 1.0 : 1.852)));
    m_scaleRuler.SetVisible(grid.mIsScale);
}

void GUIView::SetGridColor(const QColor& color)
{
    for ( TGridMap::iterator it = m_grids.begin(); it != m_grids.end(); ++it )
        (*it)->SetColor(color);
}

void GUIView::SetMapColor(const QColor& color)
{
    m_scaleRuler.SetColor(color);
}

void GUIView::SetMeteoColor(const QColor& color)
{
    // Meteo vectors
    for ( TMeteoVecMap::iterator it = GetMeteoVec().begin(); it != GetMeteoVec().end(); ++it )
    {
        Q_ASSERT(dynamic_cast<GUIMeteoVector *>((*it).data()));
        static_cast<GUIMeteoVector *>((*it).data())->SetColor(color);
    }
    // Meteo contours
    for ( TMeteoZoneMap::iterator it = GetMeteoZones().begin(); it != GetMeteoZones().end(); ++it )
    {
        Q_ASSERT(dynamic_cast<GUIMeteoContour *>((*it).data()));
        static_cast<GUIMeteoContour *>((*it).data())->SetColor(color);
    }
}

void GUIView::SetPelengColor(const QColor& color)
{
    for ( TPelengMap::iterator it = GetPelengs().begin(); it != GetPelengs().end(); ++it )
    {
        Q_ASSERT(dynamic_cast<GUIPeleng *>((*it).data()));
        static_cast<GUIPeleng *>((*it).data())->SetColor(color);
    }
}

void GUIView::OnUpdateFont()
{
    for ( TPelengMap::iterator it = GetPelengs().begin(); it != GetPelengs().end(); ++it )
    {
        Q_ASSERT(dynamic_cast<GUIPeleng *>(it->data()));
        static_cast<GUIPeleng *>(it->data())->UpdateFont();
    }
}

void GUIView::SetCenterPoint(qreal latitude, qreal longitude, qreal angle)
{
    View::SetCenterPoint(latitude, longitude, angle);

    const TCoordSystem& pCS = GetCoordSys();

    // Update rect of the scene relative to the new center point
    UpdateSceneRect();

    if ( pCS->isModelCentered() )
    {
        // Force to redraw the scene.
        Draw(true);
    }
}

void GUIView::Clear()
{
    SetViewMode(VM_None);

    typedef QList<QGraphicsItem *>      TItemList;
    // Remove all items from scene.
    TItemList items = scene()->items();
    foreach ( QGraphicsItem *item, items )
        scene()->removeItem(item);

    m_grids.clear();

    View::Clear();
}

void GUIView::SetViewMode(int mode)
{
    if ( mode == int(m_viewMode) )
        return;

    if ( m_viewMode == (int)VM_MoveCenter || m_viewMode == (int)VM_ZoomRect )
        unsetCursor();

    m_viewMode = mode;

    if ( mode == (int)VM_MoveCenter )
        setCursor(Qt::CrossCursor);
    else if ( mode == (int)VM_ZoomRect )
    {
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Images/images/ZoomIn.png"), QSize(), QIcon::Normal, QIcon::Off);
        setCursor(icon.pixmap(QSize(16,16)));
    }
}

qreal GUIView::distanceModelInCm() const
{
    const TCoordSystem &pCS = GetCoordSys();

    double pixelInMmW = (double)width() / (double)widthMM();
    double pixelInMmH = (double)height() / (double)heightMM();
    double pixelInCm = ((pixelInMmW * 10.0) + (pixelInMmH * 10.0)) / 2.0;

    QPoint pointCenterScreen = viewport()->rect().center();
    QPointF pointCenterScreenF(pointCenterScreen.x(), pointCenterScreen.y());
    QPointF pointScene1 = pCS->fromScreen(pointCenterScreenF);
    QPointF pointScene2 = pCS->fromScreen(pointCenterScreenF + QPointF(pixelInCm, pixelInCm));
    QVector3D p1 = pCS->toModel(pointScene1);
    QVector3D p2 = pCS->toModel(pointScene2);

    return (qAbs(p2.x()-p1.x()) + qAbs(p2.y()-p1.y())) / 2;
}

template <class T>
void GUIView::updateItemVisibility(T *guiItem, qreal distMInCm, qreal minDist, qreal maxDist)
{
    if ( minDist > distMInCm || distMInCm > maxDist )
    {
        if ( guiItem->isVisible() )
            guiItem->setVisible(false);
    }
    else
    {
        if ( !guiItem->isVisible() )
            guiItem->setVisible(true);
    }
}

}   // namespace geoGE
