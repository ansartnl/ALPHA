#include "GUIView.h"
#include "main.h"

#include <math.h>
#include <QWheelEvent>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsView>
#include <QScrollBar>
#include <QSignalMapper>
#include <QInputDialog>
#include <QFutureWatcher>
#include <QtCore>

#include "databasesubscriber.h"
#include "GUIScene.h"
#include "GUILineText.h"
#include "GUIAirplane.h"
#include "GUISector.h"
#include "GUIRoute.h"
#include "GUIUserPics.h"
#include "meteo/GUIMeteo.h"
#include "LayoutDlg.h"
#include "front/core/reminder/XReminder.h"
#include "front/core/airplane/AClist/ListAC.h"
#include "back/utils/ProfileSettings.h"
#include "front/core/airplane/fpltracer/TracerHDG.h"
#include "restrictions/RestrictionsWidget.h"
#include "OLDIMessages.h"
#include "Master.h"
#include "XMasterWindow.h"
#include "controlwindow.h"
#include "back/core/airplane/AirplaneMTCD.h"
#include "dialog/UserPicsWidget.h"

#include "obelix/CmdRequestAllFPL.h"
#include "obelix/CmdChangeSFPL.h"

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QtConcurrent/QtConcurrent>
#endif

#ifdef CWP_AIRPLANE_THREAD
#   include "back/core/airplane/AirplaneThread.h"
#endif

CGUIView *gActiveView = 0;
volatile bool CGUIView::m_MTCDforceStop = false;
extern QString currentUserName;

CGUIView::CGUIView(IParent* pParent)
    : m_qWidth(800), m_qHeight(800), m_iScale(0)
    , m_pParent(pParent), m_bPull(false), m_pPlaneNeedCoord(0), mode_airplane_(0), mode_(NONE_MODE)
    , m_bShowGauge(false), m_qEchelon(50), m_qPress(1013/*760*/)
    , m_bRefreshListACC(false)
    , m_pReminder(0), m_pTempLine(0), m_rcGrid(0)
    , m_MTCDfutwatch(0)
    , m_isLocked(false), m_isDrawing(false)
    , m_radius(0)
    , m_circle(0)
{
    gActiveView = this;
    setMouseTracking(true);

    m_MTCDfutwatch = new QFutureWatcher<mtcd::TListMTCDPtr>(this);
    connect(m_MTCDfutwatch, SIGNAL(resultReadyAt(int)), this, SLOT(OnMTCDfutwatchResult(int)));
    connect(m_MTCDfutwatch, SIGNAL(finished()), this, SLOT(OnMTCDfutwatchFinish()));

    m_pBearingSignalMapper = new QSignalMapper(this);
    connect(m_pBearingSignalMapper, SIGNAL(mapped(int)), SLOT(DelBearing(int)));

    connect(DatabaseSubscriber::instance().subscribe("meteo"), SIGNAL(notification()), SLOT(OnMeteoRefresh()));

#ifdef CWP_AIRPLANE_THREAD
    // Create airplane update and set position delayed thread.
    m_airplaneThread = new AirplaneThread(this);
    connect(m_airplaneThread, SIGNAL(signalUpdate(quint16)), SLOT(OnUpdateAirplane(quint16)),
            Qt::QueuedConnection);
    connect(m_airplaneThread, SIGNAL(signalSet(quint16)), SLOT(OnSetAirplane(quint16)),
            Qt::QueuedConnection);
#endif
    //Pasha !!! startTimer(11*1000);
}

CGUIView::~CGUIView()
{
    m_MTCDforceStop = true;
    if (m_MTCDfutwatch)
    {
        m_MTCDfutwatch->cancel();
        m_MTCDfutwatch->waitForFinished();
    }
    m_MTCDforceStop = false;

    CListAC::StopFutureRefresh();

#ifdef CWP_AIRPLANE_THREAD
    // Stop airplane thread or terminate it.
    m_airplaneThread->stop();
    if ( m_airplaneThread->wait(1000) )
        m_airplaneThread->terminate();
#endif

    setBearingId(TBearingIdSet());

    TSceneList::iterator itSceneScan = m_SceneList.begin();
    for (; itSceneScan != m_SceneList.end(); ++itSceneScan)
    {
        CGUIScene* pscene = dynamic_cast<CGUIScene*>(itSceneScan->data());
        if (pscene)
        {
            CGUIScene::TListItem::const_iterator itItemScan = pscene->GetItemList().begin();
            for (; itItemScan != pscene->GetItemList().end(); ++itItemScan)
                scene()->removeItem(*itItemScan);
        }
    }
    if(m_rcGrid)
    {
        delete m_rcGrid;
    }
    gActiveView = 0;
}

void CGUIView::AddScene(const QString& sKey, const TScenePtr& pscene)
{
    if (!GetMainFIR().isNull())
    {
        Scene::TSceneList::iterator itSector = pscene->GetSectorList().begin();
        for (; itSector != pscene->GetSectorList().end(); ++itSector)
        {
            CGUISector* pSector = dynamic_cast<CGUISector*>(itSector->data());
            if (pSector)
                pSector->SetBorderVisible(false);
        }
    }

    CView::AddScene(sKey, pscene);

    CGUIScene* pGUIScene = dynamic_cast<CGUIScene*>(pscene.data());
    if (pscene)
    {
        CGUIScene::TListItem::const_iterator itItemScan = pGUIScene->GetItemList().begin();
        for(; itItemScan != pGUIScene->GetItemList().end(); ++itItemScan)
            scene()->addItem(*itItemScan);
    }
}

void CGUIView::AddAirplane(const QSharedPointer<CAirplane>& pAirplane)
{
    CView::AddAirplane(pAirplane);

    if (CGUIAirplane* pItem = dynamic_cast<CGUIAirplane*>(pAirplane.data()))
    {
        pItem->SetEchelon(m_qEchelon);
        pItem->SetPress(m_qPress);

        connect(pItem, SIGNAL(getCoord(CAirplane*) ), this, SLOT( OnSetPlaneNeedCoord(CAirplane*)));
        connect(pItem, SIGNAL(resetBearing() ), this, SLOT( OnResetBearing()));
        connect(pItem, SIGNAL(setActivePlane(CAirplane*) ), this, SLOT( OnSetActivePlane(CAirplane*)));
        connect(pItem, SIGNAL(changedSFPL(const QSharedPointer<CSFPL>&) ), this, SLOT( OnShangedSFPL(const QSharedPointer<CSFPL>&)));
        connect(pItem, SIGNAL(resetLastActive() ), this, SLOT( OnResetLastActive()));
        connect(pItem, SIGNAL(setHighlight(const CHighlightPlane&) ), this, SLOT( OnHighlightPlanes(const CHighlightPlane&)));
        connect(pItem, SIGNAL(setTracerHDG(CAirplane*) ), this, SLOT(OnTracerHDG(CAirplane*)));
        connect(pItem, SIGNAL(setNextPointSearchMode(CAirplane*)), this, SLOT(onSetNextPointSearchMode(CAirplane*)));
        connect(pItem, SIGNAL(lockMovingView()), SLOT(onLockMoving()));
        connect(pItem, SIGNAL(unlockMovingView()), SLOT(onUnlockMoving()));
        double dScale = m_qWidth/m_qXSize;
        if (dScale > m_qHeight/m_qYSize)
            dScale = m_qHeight/m_qYSize;
        dScale *= pow(1.125, m_iScale);

        scene()->addItem(pItem);

        pItem->SetPosition(m_qXMin, m_qYMin + m_qYSize, dScale, m_pViewCentralPoint->GetCoord());
        pItem->setFormularPos(master()->formularLinkAngle(), master()->formularLinkLength());
        //!!! pItem->setVisible(CXLayoutDlg::IsLayoutEnable(GetIDName(), CXLayoutDlg::Airplane));
    }

    CListAC::AddPlaneRecord(pAirplane);

    if (!m_bRefreshListACC) {
        m_bRefreshListACC = true;
        QTimer::singleShot(3000, this, SLOT(OnTimerRefreshListACC()));
    }
}

void CGUIView::AddMeteo(const QSharedPointer<core::CMeteo>& pMeteo)
{
    CView::AddMeteo(pMeteo);
    QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*>(pMeteo.data());
    if (pItem)
        scene()->addItem(pItem);
}

void CGUIView::DelAirplane(QSharedPointer<CAirplane> pAirplane, bool bSend /* = true*/)
{
    Q_UNUSED(bSend)
    if (m_pPlaneNeedCoord == pAirplane.data())
    {
        disconnect(dynamic_cast<CGUIAirplane*>(m_pPlaneNeedCoord), SIGNAL( getCoord(CAirplane*) ), this, SLOT( OnSetPlaneNeedCoord(CAirplane*) ));
        m_pPlaneNeedCoord = 0;
    }

    if (mode_airplane_ == pAirplane.data())
        resetModeData();

    if (m_pReminder)
        m_pReminder->DeletePlane(pAirplane);

    RemoveSTCA(pAirplane->GetID());

//    CListAC::DelPlaneRecord(pAirplane);

    CGUIAirplane *pGUIAirplane = dynamic_cast<CGUIAirplane*>(pAirplane.data());
    if (pGUIAirplane)
    {
        pGUIAirplane->ShowPathPoints(false);
        scene()->removeItem(pGUIAirplane);
    }

    CView::DelAirplane(pAirplane);
}

QSharedPointer<core::CMeteo> CGUIView::CreateMeteo() const
{
    return QSharedPointer<core::CMeteo>(new CGUIMeteo);
}

QSharedPointer<CAirplane> CGUIView::CreateAirplane() const
{
    return QSharedPointer<CAirplane>(new CGUIAirplane);
}

void CGUIView::SetZoom(int iValue)
{
    int iDelta = iValue - m_iScale;
    m_iScale = iValue;
    Zoom(mapToScene(viewport()->rect().center()), iDelta);
}

void CGUIView::prepare()
{
    bool first(true);
    TSceneList::iterator itSceneScan = m_SceneList.begin();
    for(; itSceneScan != m_SceneList.end(); ++itSceneScan)
    {
        CGUIScene* guiScene = dynamic_cast<CGUIScene*>(itSceneScan->data());
        if (guiScene)
        {
            if (first)
            {
                guiScene->prepare(m_qXMin, m_qYMin, m_qXSize, m_qYSize);
                first = false;
            } else
            {
                qint64 iXMin(0), iYMin(0), iXMax(0), iYMax(0);
                guiScene->prepare(iXMin, iYMin, iXMax, iYMax);

                if (iXMin < m_qXMin)
                    m_qXMin = iXMin;
                if (iXMax > m_qXSize)
                    m_qXSize = iXMax;
                if (iYMin < m_qYMin)
                    m_qYMin = iYMin;
                if (iYMax > m_qYSize)
                    m_qYSize = iYMax;
            }
        }
    }
    m_qXSize -= m_qXMin; m_qYSize -= m_qYMin;
    m_qXMin -= 500000; m_qYMin -= 500000; m_qXSize += 1000000; m_qYSize += 1000000;

    disconnect(&m_AirplaneTimer, SIGNAL(timeout()), this, SLOT(OnDrawAirplanes()));
    connect(&m_AirplaneTimer, SIGNAL(timeout()), this, SLOT(OnDrawAirplanes()));
    m_AirplaneTimer.start(4000);

#ifdef CWP_AIRPLANE_THREAD
    m_airplaneThread->start();
#endif

    SetZoom(m_iScale);
    if (m_pParent)
        m_pParent->ZoomEvent(m_iScale);

    if (!m_rcGrid)
    {
        if(GetMainFIR())
        {
            m_rcGrid = new CGUIRcGrid;
            RcGridValues values = CGUIRcGrid::LoadRcGridValues(GetMainFIR()->GetIDName());
            if(!values.mCenter.IsNull())
                values.mCenter.Center(m_pViewCentralPoint->GetCoord());
            m_rcGrid->setRcGridValues(values);
            scene()->addItem(m_rcGrid);
        }
    }

    SetLayouts();
}

void CGUIView::wheelEvent(QWheelEvent* event)
{
    if (m_isLocked) {
        return;
    }

    if (!event->buttons()) {
        if (!scene()) {
            QAbstractScrollArea::wheelEvent(event);
            return;
        }
        event->ignore();

        QPoint point;
        if(networkSettings()->value("users/point_zoom").toString() == "yes") {
            point.setX(networkSettings()->value("users/x_point_zoom").toInt());
            point.setY(networkSettings()->value("users/y_point_zoom").toInt());
        }
        else point = event->pos();

        QGraphicsSceneWheelEvent wheelEvent(QEvent::GraphicsSceneWheel);
        wheelEvent.setWidget(viewport());
        //wheelEvent.setScenePos(mapToScene(event->pos()));
        wheelEvent.setScenePos(mapToScene(point));
        wheelEvent.setScreenPos(event->globalPos());
        wheelEvent.setButtons(event->buttons());
        wheelEvent.setModifiers(event->modifiers());
        wheelEvent.setDelta(event->delta());
        wheelEvent.setOrientation(event->orientation());
        wheelEvent.setAccepted(false);
        QApplication::sendEvent(scene(), &wheelEvent);
        event->setAccepted(wheelEvent.isAccepted());
        if (!event->isAccepted()) {
            event->accept();

            int num_steps = -event->delta() / (8.0 * 15.0);

            if ((m_iScale < MAX_SCALE && num_steps > 0) || (m_iScale > 0 && num_steps < 0)) {
                m_iScale += num_steps;

                if (m_iScale > MAX_SCALE) {
                    num_steps -= m_iScale - MAX_SCALE;
                    m_iScale = MAX_SCALE;
                } else if (m_iScale + num_steps < 0) {
                    m_iScale = 0;
                }

                //Zoom(mapToScene(event->pos()), num_steps);
                Zoom(mapToScene(point), num_steps);
                if (m_pParent)
                    m_pParent->ZoomEvent(m_iScale);
            }
        }
    } else {
        QGraphicsView::wheelEvent(event);
    }
}

void CGUIView::OnPullScene(QPointF CenterPoint)
{
    centerOn(CenterPoint);
}

void CGUIView::mouseMoveEvent(QMouseEvent * event)
{
    if (m_bPull)
    {
        if (m_isLocked) {
            return;
        }

        QPointF NewPoint(mapToScene(event->pos()));
        QPointF DeltaPoint = NewPoint - m_PullPoint;

        QPointF CenterPoint = mapToScene(viewport()->rect().center());
        CenterPoint -= DeltaPoint;
        //centerOn(CenterPoint);
        QMetaObject::invokeMethod(this, "OnPullScene", Qt::QueuedConnection, Q_ARG(QPointF, CenterPoint));
        m_PullPoint = NewPoint - DeltaPoint;

    } else {
        if (m_pParent)
        {
            QString sRoute;
            QList<QGraphicsItem*> listItems = scene()->items(mapToScene(event->pos()));
            QList<QGraphicsItem*>::const_iterator itList = listItems.constBegin();
            CGUIRoute* pRoute = 0;
            for (; itList != listItems.constEnd(); ++itList) {
                pRoute = dynamic_cast<CGUIRoute*>(*itList);
                if(pRoute)
                    break;
            }
            if (pRoute)
                sRoute = pRoute->GetName();
            m_pParent->CoordinateEvent(GetCoordAtPoint(mapToScene(event->pos())), sRoute);
        }

        if (m_pTempLine)
            m_pTempLine->SetPoint2(GetCoordAtPoint(mapToScene(event->pos())));

        if (TRACER_HDG == mode_) {
            QHash<CAirplane*, QSharedPointer<CTracerHDG> >::iterator itTracer = airplane_to_tracer_.find(mode_airplane_);
            if (itTracer != airplane_to_tracer_.end())
                (*itTracer)->Update(GetCoordAtPoint(mapToScene(event->pos())));
        }
    }

    QGraphicsView::mouseMoveEvent(event);
}

void CGUIView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
    if (event->isAccepted()) {
        if(MAP_NOTES == mode_)
            return;
        else {
            resetModeData();
            return;
	}
    }
    if (event->button() == Qt::LeftButton)
    {
        if (m_bShowGauge && !TRACER_HDG == mode_ && !NEXT_POINT_SEARCH == mode_) {
            if (!m_pTempLine) {
                createVectorMeasurement(event->pos());
            } else {
                const CCoord coord2(GetCoordAtPoint(mapToScene(event->pos())));
                const QList<QGraphicsItem*> listItems = scene()->items(mapToScene(event->pos()));
                QList<QGraphicsItem*>::const_iterator itList = listItems.begin();
                CGUIAirplane *plane = 0;
                for (; itList != listItems.end(); ++itList) {
                    CGUIAirplanePoint *plane_point = dynamic_cast<CGUIAirplanePoint*>(*itList);
                    if (plane_point)
                        plane = dynamic_cast<CGUIAirplane*>(plane_point->parentItem());
                    else
                        plane = dynamic_cast<CGUIAirplane*>(*itList);
                    if (plane)
                        break;
                }
                if (plane) {
                    TAirplanesMap::const_iterator itFind = GetAirplanes().find(plane->GetID());
                    if (itFind != GetAirplanes().end())
                        m_pTempLine->SetPoint2(*itFind);
                } else {
                    m_pTempLine->SetPoint2(coord2);
                }

                m_pTempLine = 0;
            }
        }
    }
    if (event->button() == Qt::MiddleButton) {
        setCursor(Qt::ClosedHandCursor);
        m_bPull = true;
        m_PullPoint = (mapToScene(event->pos()));
    } else {
        if (m_pPlaneNeedCoord) {
            static_cast<CGUIAirplane*>(m_pPlaneNeedCoord)->SetPlaneNewCoord(GetCoordAtPoint(mapToScene(event->pos())));
            m_pPlaneNeedCoord = 0;
            unsetCursor();
        }

        if (m_bPull) {
            unsetCursor();
            m_bPull = false;
        }

        if (event->button() == Qt::LeftButton) {
            if (TRACER_HDG == mode_)
                OnTracerHDG(mode_airplane_);
            else if (NEXT_POINT_SEARCH == mode_)
                setNextPoint(GetCoordAtPoint(mapToScene(event->pos())));
        } else if (MAP_NOTES != mode_) {
            resetModeData();
        }

        // Digitizing points.
        if (m_coordsDigitizer && event->button() == Qt::LeftButton) {
            m_coordsDigitizer->addCoord( GetCoordAtPoint( mapToScene(event->pos()) ) );

            qint64 qllXMin, qllYMax;
            double dScale;
            CCoord centralCoord;
            GetCoordSystem(qllXMin, qllYMax, dScale, centralCoord);
            m_coordsDigitizer->updatePosition(qllXMin, qllYMax, dScale, centralCoord);

            if (!m_coordsDigitizer->multiplePick()) {
                m_coordsDigitizer->toClipboard();
                emit pickCoord(mPickCoordID);
                mPickCoordID = QUuid();
                stopCoordsPicking();
                emit singleCoordPicked();
            }
        }
    }
}

void CGUIView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton) {
        if (m_bPull) {
            unsetCursor();
            if (m_pPlaneNeedCoord)
                setCursor(Qt::PointingHandCursor);
            m_bPull = false;
        }
    }
    /*else if (event->button() == Qt::LeftButton)
    {
        if (m_bShowGauge) {
            if (!m_pTempLine) {
                createVectorMeasurement(event->pos());
            } else {
                const CCoord coord2(GetCoordAtPoint(mapToScene(event->pos())));
                const QList<QGraphicsItem*> listItems = scene()->items(mapToScene(event->pos()));
                QList<QGraphicsItem*>::const_iterator itList = listItems.begin();
                CGUIAirplane *plane = 0;
                for (; itList != listItems.end(); ++itList) {
                    CGUIAirplanePoint *plane_point = dynamic_cast<CGUIAirplanePoint*>(*itList);
                    if (plane_point)
                        plane = dynamic_cast<CGUIAirplane*>(plane_point->parentItem());
                    else
                        plane = dynamic_cast<CGUIAirplane*>(*itList);
                    if (plane)
                        break;
                }
                if (plane) {
                    TAirplanesMap::const_iterator itFind = GetAirplanes().find(plane->GetID());
                    if (itFind != GetAirplanes().end())
                        m_pTempLine->SetPoint2(*itFind);
                } else {
                    m_pTempLine->SetPoint2(coord2);
                }

                m_pTempLine = 0;
            }
        }
    }*/
    QGraphicsView::mouseReleaseEvent(event);
}

void CGUIView::mouseDoubleClickEvent(QMouseEvent *event)
{
    /*
    if(event->isAccepted())
        qDebug(QString("CGUIView::mouseDoubleClickEvent accept").toAscii());
    else
        qDebug(QString("CGUIView::mouseDoubleClickEvent ignore").toAscii());
    event->ignore();
    */
    QGraphicsView::mouseDoubleClickEvent(event);
    /*
    if(event->isAccepted())
        qDebug(QString("CGUIView::mouseDoubleClickEvent accept").toAscii());
    else
        qDebug(QString("CGUIView::mouseDoubleClickEvent ignore").toAscii());
    */

    // Stop digitizing points.
    if (m_coordsDigitizer) {
        m_coordsDigitizer->toClipboard();
        emit pickCoord(mPickCoordID);
        mPickCoordID = QUuid();
        stopCoordsPicking();
    }
}

void CGUIView::keyPressEvent(QKeyEvent *event)
{
    QGraphicsView::keyPressEvent(event);
}

void CGUIView::keyReleaseEvent(QKeyEvent *event)
{
    QGraphicsView::keyPressEvent(event);
    if (event->isAccepted())
        return;

    if (event->key() == Qt::Key_Escape) {
        m_pPlaneNeedCoord = 0;
        unsetCursor();

        if (MAP_NOTES != mode_)
            resetModeData();
    }
}

void CGUIView::Draw()
{
    double dScale = m_qWidth/m_qXSize;
    if (dScale > m_qHeight/m_qYSize)
        dScale = m_qHeight/m_qYSize;
    dScale *= pow(1.125, m_iScale);

#ifdef CWP_AIRPLANE_THREAD
    AirplaneThread::TListId airplaneIds;
    QList<QGraphicsItem*> ItemList = items();
    for ( QList<QGraphicsItem*>::const_iterator itScan = ItemList.begin();
         itScan != ItemList.end(); ++itScan )
    {
        CGUIAirplane* pPlane = dynamic_cast<CGUIAirplane*>(*itScan);
        if ( pPlane )
        {
            airplaneIds << pPlane->GetID();
            continue;
        }

        IGUIItemHandler* pItem = dynamic_cast<IGUIItemHandler*>(*itScan);
        if (pItem)
            pItem->SetPosition(m_qXMin, m_qYMin + m_qYSize, dScale, m_pViewCentralPoint->GetCoord());
    }
    m_airplaneThread->addIdList( airplaneIds, AirplaneThread::ForSet );
#else
    QList<QGraphicsItem*> ItemList = items();
    QList<QGraphicsItem*>::const_iterator itScan = ItemList.begin();
    for (; itScan != ItemList.end(); ++itScan)
    {
        IGUIItemHandler* pItem = dynamic_cast<IGUIItemHandler*>(*itScan);
        if (pItem)
            pItem->SetPosition(m_qXMin, m_qYMin + m_qYSize, dScale, m_pViewCentralPoint->GetCoord());
    }
#endif

    QMap<CGUILineText*, QSharedPointer<CGUILineText> >::const_iterator itList = m_mapGauge.constBegin();
    for (; itList != m_mapGauge.constEnd(); ++itList)
        if (!itList->isNull())
            itList->data()->SetText(QString::null);

    TMapSTCA::const_iterator itSTCA = m_mapSTCA.constBegin();
    for (; itSTCA != m_mapSTCA.constEnd(); ++itSTCA)
        if (!itSTCA->isNull()) itSTCA->data()->SetText(QString::null);

    if (m_coordsDigitizer)
        m_coordsDigitizer->updatePosition(m_qXMin, m_qYMin + m_qYSize, dScale, m_pViewCentralPoint->GetCoord());
}

void CGUIView::Redraw()
{
    ViewportUpdateMode mode = viewportUpdateMode();
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    //resetCachedContent();
    scene()->invalidate(mapToScene(viewport()->rect()).boundingRect());
    scene()->update(mapToScene(viewport()->rect()).boundingRect());
    setViewportUpdateMode(mode);
}

void CGUIView::Zoom(const QPointF& PointOld, int iCurScale)
{
    QPointF CenterPoint = mapToScene(viewport()->rect().center());

    double dScale = pow(1.125, m_iScale);
    setSceneRect(0, 0, m_qWidth * dScale, m_qHeight * dScale);

    QPointF PointNew(PointOld * pow(1.125, iCurScale));
    PointNew -= PointOld;
    CenterPoint += PointNew;
    centerOn(CenterPoint);

    Draw();
    Redraw();
    UpdateFormularsAfterUpdate();
}

void CGUIView::UpdateFormularsAfterUpdate()
{
    /*TAirplanesMap::const_iterator itAirplans = GetAirplanes().begin();
    for(; itAirplans != GetAirplanes().end(); ++itAirplans)
    {
        CGUIAirplane* pPlane = dynamic_cast<CGUIAirplane*>(itAirplans->data());
        if(pPlane)
            pPlane->updateExtAfterSceneUpdated();
    }*/
}

#ifdef CWP_AIRPLANE_THREAD

void CGUIView::OnSetAirplane(quint16 id)
{
    TAirplanesMap::const_iterator itAirplan = GetAirplanes().find(id);
    if ( itAirplan == GetAirplanes().end() )
        return;

    Q_ASSERT( dynamic_cast<CGUIAirplane*>(itAirplan->data()) );
    CGUIAirplane* pPlane = static_cast<CGUIAirplane*>(itAirplan->data());
    if ( pPlane )
    {
        qint64 qllXMin, qllYMax;
        double dScale;
        CCoord centralCoord;
        GetCoordSystem(qllXMin, qllYMax, dScale, centralCoord);

        pPlane->SetPosition(qllXMin, qllYMax, dScale, centralCoord);
    }
}

void CGUIView::OnUpdateAirplane(quint16 id)
{
    TAirplanesMap::const_iterator itAirplan = GetAirplanes().find(id);
    if ( itAirplan == GetAirplanes().end() )
        return;

    Q_ASSERT( dynamic_cast<CGUIAirplane*>(itAirplan->data()) );
    CGUIAirplane* pPlane = static_cast<CGUIAirplane*>(itAirplan->data());
    if(pPlane)
        pPlane->UpdatePosition();

    CListAC::ModPlaneRecord(*itAirplan);
}

#endif

void CGUIView::OnDrawAirplanes()
{
    m_isDrawing = true;
#ifdef CWP_AIRPLANE_THREAD
    AirplaneThread::TListId airplaneIds;
    TAirplanesMap::const_iterator itAirplans = GetAirplanes().begin();
    for(; itAirplans != GetAirplanes().end(); ++itAirplans)
    {
        Q_ASSERT( dynamic_cast<CGUIAirplane*>(itAirplans->data()) );
        CGUIAirplane* pPlane = static_cast<CGUIAirplane*>(itAirplans->data());
        if(pPlane)
            airplaneIds << pPlane->GetID();
    }
    m_airplaneThread->addIdList( airplaneIds, AirplaneThread::ForUpdate );
#else

    // don't use iterator for loop, they can be invalid
    foreach (const QSharedPointer<CAirplane> &airplane, GetAirplanes())
    {
        QSet<QSharedPointer<CAirplane>> doubleCode;

        CGUIAirplane* pPlane = dynamic_cast<CGUIAirplane*>(airplane.data());
        if (pPlane)
        {
            { // double code reminder
                QSharedPointer<CReminder> pRem(new CReminder(CReminder::DoubleCode, ""));
                pRem->SetLevel(CReminder::Alarm);
                pRem->SetComplete(true);
                pRem->SetClosable(false);
                pRem->SetTime(QDateTime::currentDateTime());

                if (!CAirplane::sListSpecCodes.contains(pPlane->GetMode3AReplyToString()))
                {
                    TAirplanesMap::const_iterator itAirplansOther = GetAirplanes().constBegin();
                    for(; itAirplansOther != GetAirplanes().constEnd(); ++itAirplansOther)
                    {
                        if ((*itAirplansOther)->GetMode3AReply() == airplane->GetMode3AReply())
                        {
                            if (*itAirplansOther != airplane)
                            {
                                doubleCode.insert(*itAirplansOther);
                            }
                        }
                    }
                }

                if (!doubleCode.isEmpty())
                {
                    pPlane->AddReminderLocal(pRem, false);
                    if (pPlane->GetSFPL() && (networkSettings()->value("oldi/double_code_re_act", "false").toString() == "true"))
                    {
                        //cancel ACT
                        QSharedPointer<obelix::CCmdChangeSFPL> pCmd(new obelix::CCmdChangeSFPL(obelix::CCmdChangeSFPL::Cancel));
                        pCmd->SetValue(obelix::CCmdChangeSFPL::XML_CODE, QString::number(pPlane->GetSFPL()->GetCODE()));
                        pCmd->SetValue(obelix::CCmdChangeSFPL::XML_COPIN, pPlane->GetSFPL()->GetCOPin());
                        pCmd->SetValue(obelix::CCmdChangeSFPL::XML_ETOIN, pPlane->GetSFPL()->GetETOin().toString());
                        pCmd->SetValue(obelix::CCmdChangeSFPL::XML_COPINFL, pPlane->GetSFPL()->GetCOPinFL().toString());
                        pCmd->SetValue(obelix::CCmdChangeSFPL::XML_FPLID, QString::number(pPlane->GetSFPL()->GetID()));
                        master()->GetObClient().XMLSend(pCmd.staticCast<CXMLCmd>());
                    }
                }
                else
                {
                    pPlane->RemoveReminder(pRem);
                }
            }

            pPlane->UpdatePosition();
        }

        bool doubleCodeWithoutSFPL = false;
        if (!airplane->GetSFPL() && !doubleCode.isEmpty())
        {
            doubleCodeWithoutSFPL = std::none_of(doubleCode.constBegin(), doubleCode.constEnd(),
                                                 [](QSharedPointer<CAirplane> a){ return a->GetSFPL(); });
        }

        if (doubleCodeWithoutSFPL)
        {
            auto sfpl = CListAC::GetSFPLByCode(airplane->GetMode3AReply());
            if (sfpl)
            {
                auto point = master()->findPointByName(sfpl->GetCOPin());
                if (point)
                {
                    double distance = GetDistance(point->GetCoord(), airplane->GetCoord());
                    bool isMinimalDistance = true;
                    foreach (const auto& a, doubleCode)
                    {
                        if (GetDistance(point->GetCoord(), a->GetCoord()) < distance)
                        {
                            isMinimalDistance = false;
                            break;
                        }
                    }

                    if (isMinimalDistance)
                    {
                        CListAC::AddPlaneRecord(airplane);
                    }
                }
                else
                {
                    CListAC::ModPlaneRecord(airplane);
                }
            }
        }
        else
        {
            CListAC::ModPlaneRecord(airplane);
        }
    }
#endif
    if (m_pReminder)
        m_pReminder->UpdateAzimuth();

    QMap<CGUILineText*, QSharedPointer<CGUILineText> >::const_iterator itList = m_mapGauge.begin();
    for (; itList != m_mapGauge.end(); ++itList)
        if (!itList->isNull())
            itList->data()->SetText(QString::null);

    TMapSTCA::iterator itSTCA = m_mapSTCA.begin();
    while(itSTCA != m_mapSTCA.end())
    {
        if(*itSTCA)
        {
            QSharedPointer<CAirplane> pPlane1 = (*itSTCA)->GetPlane1();
            QSharedPointer<CAirplane> pPlane2 = (*itSTCA)->GetPlane2();
            if(pPlane1 && pPlane2)
            {
                QSharedPointer<stca::Stca> pSTCA = pPlane1->GetSTCAtoOther(pPlane2->GetID());
                if(!pSTCA) // :( maybe another have
                    pSTCA = pPlane2->GetSTCAtoOther(pPlane1->GetID());
                if(pSTCA)
                {
                    if(!pSTCA->mCounter || (pSTCA->type == AirplaneCollision::NoCollision))
                    {
                        scene()->removeItem((*itSTCA).data());
                        itSTCA = m_mapSTCA.erase(itSTCA);
                    }
                    else
                    {
                        (*itSTCA)->SetText(QString::null);
                        ++itSTCA;
                    }
                }
                else
                {
                    scene()->removeItem((*itSTCA).data());
                    itSTCA = m_mapSTCA.erase(itSTCA);
                }
            }
            else
            {
                scene()->removeItem((*itSTCA).data());
                itSTCA = m_mapSTCA.erase(itSTCA);
            }
        }
        else
        {
            itSTCA = m_mapSTCA.erase(itSTCA);
        }
    }

    QHash<CAirplane*, QSharedPointer<CTracerHDG> >::const_iterator itTracer = airplane_to_tracer_.constBegin();
    for (; itTracer != airplane_to_tracer_.constEnd(); ++itTracer)
        (*itTracer)->Update();

    if (m_MTCDfutwatch && !m_MTCDfutwatch->isRunning() && !CListAC::isFutureRefreshRunning()
            && networkSettings()->value("airplane/MTCD_enable", true).toBool() /* and some conditions */)
        QMetaObject::invokeMethod(this, "OnMTCD", Qt::QueuedConnection);

    m_isDrawing = false;
}

void CGUIView::OnSetPlaneNeedCoord(CAirplane *receiver)
{
    m_pPlaneNeedCoord = receiver;
    unsetCursor();
    QGraphicsView::setCursor(Qt::PointingHandCursor);
}

CCoord CGUIView::GetCoordAtPoint(const QPointF &pos) const
{
    CCoord Coordinate;

    if(m_pViewCentralPoint)
    {
        double dScale = m_qWidth/m_qXSize;
        if (dScale > m_qHeight/m_qYSize)
            dScale = m_qHeight/m_qYSize;
        dScale *= pow(1.125, m_iScale);

        qint64 illX = pos.x() / dScale + m_qXMin;
        qint64 illY = m_qYMin + m_qYSize - pos.y() / dScale;

        Coordinate.SetX(illX);
        Coordinate.SetY(illY);
        Coordinate.DeCenter(m_pViewCentralPoint->GetCoord());
        Coordinate.Center(m_pViewCentralPoint->GetCoord()); //Pasha
    }
    return Coordinate;
}

QPointF CGUIView::CoordToScene(const CCoord &coord)
{
    double dScale = m_qWidth/m_qXSize;
    if (dScale > m_qHeight/m_qYSize)
        dScale = m_qHeight/m_qYSize;
    dScale *= pow(1.125, m_iScale);

    return QPointF((coord.GetX() - m_qXMin) * dScale,
            (m_qYSize - coord.GetY()) * dScale);
}

qreal CGUIView::DistanceToScene(qreal distance)
{
    double dScale = m_qWidth/m_qXSize;
    if (dScale > m_qHeight/m_qYSize)
        dScale = m_qHeight/m_qYSize;
    dScale *= pow(1.125, m_iScale);

    return distance * dScale;
}

void CGUIView::GetCoordSystem(qint64 &qllXMin, qint64 &qllYMax, double &dScale, CCoord& centralCoord)
{
    dScale = m_qWidth/m_qXSize;
    if (dScale > m_qHeight/m_qYSize)
        dScale = m_qHeight/m_qYSize;
    dScale *= pow(1.125, m_iScale);

    qllXMin = m_qXMin;
    qllYMax = m_qYMin + m_qYSize;
    centralCoord = m_pViewCentralPoint->GetCoord();
}

QPoint CGUIView::CoordToScreen(const CCoord &coord)
{
    return mapFromScene( CoordToScene(coord) );
}

void CGUIView::DoubleClickLineText(CGUILineText *line_text, QPointF scene_pos)
{
    Q_UNUSED(scene_pos)
    if (!m_bShowGauge)
        return;
    QMap<CGUILineText*, QSharedPointer<CGUILineText> >::iterator it = m_mapGauge.find(line_text);
    if (it != m_mapGauge.end()) {
        if (!it->isNull()) {
            if (m_pTempLine && (m_pTempLine == it->data()))
                m_pTempLine = 0;
            scene()->removeItem(it->data());
            m_mapGauge.erase(it);
        }
    }
}

void CGUIView::SetGauge(bool show)
{
    m_bShowGauge = show;
    if (m_pTempLine && !show) {
        QMap<CGUILineText*, QSharedPointer<CGUILineText> >::iterator it = m_mapGauge.find(m_pTempLine);
        if (it != m_mapGauge.end()) {
            scene()->removeItem(it->data());
            m_mapGauge.erase(it);
        }
        m_pTempLine = 0;
    }
}

void CGUIView::SetEchelon(qint16 echelon)
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

void CGUIView::SetPress(qint16 press)
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

void CGUIView::SetLayouts()
{
    TSceneList::const_iterator itScene = GetScenes().begin();
    for (; itScene != GetScenes().end(); ++itScene)
    {
        CGUIScene* pscene = dynamic_cast<CGUIScene*>(itScene->data());
        if (pscene)
            pscene->SetLayouts();
    }

//    КОСТЫЛЬ
//    TMeteoList::const_iterator itMeteo = GetMeteo().begin();
//    for (; itMeteo != GetMeteo().end(); ++itMeteo)
//    {
//        CGUIMeteo* pMeteo = dynamic_cast<CGUIMeteo*>(itMeteo->data());
//        if (pMeteo)
//            pMeteo->setVisible(LayoutDlg::IsLayoutEnable(GetMainFIR()->GetIDName(), LayoutDlg::Meteo));
//    }

    /*
    TAirplanesMap::const_iterator itAirplane = GetAirplanes().begin();
    for (; itAirplane != GetAirplanes().end(); ++itAirplane)
    {
        CGUIAirplane* pAirplane = dynamic_cast<CGUIAirplane*>(itAirplane->data());
        if (pAirplane)
            pAirplane->setVisible(CXLayoutDlg::IsLayoutEnable("111", CXLayoutDlg::Airplane));
    }
    */
    if (m_rcGrid && GetMainFIR())
        m_rcGrid->setVisible(LayoutDlg::IsLayoutEnable(GetMainFIR()->GetIDName(), LayoutDlg::RcGrid));

    /*
    QMap<CGUILineText*, QSharedPointer<CGUILineText> >::const_iterator itGauge = m_mapGauge.begin();
    for (; itGauge != m_mapGauge.end(); ++itGauge)
        if(!itGauge->isNull()) itGauge->data()->SetText(QString::null);
    */

    Draw();
}

void CGUIView::OnResetBearing()
{
    TAirplanesMap::const_iterator itAirplane = GetAirplanes().begin();
    for(; itAirplane != GetAirplanes().end(); ++itAirplane)
    {
        CGUIAirplane* pAirplane = dynamic_cast<CGUIAirplane*>(itAirplane->data());
        if(pAirplane)
            pAirplane->SetBearing(false);
    }
}

void CGUIView::setCursor(const QCursor& cursor)
{
    viewport()->setCursor(cursor);
    QGraphicsView::setCursor(cursor);
}

void CGUIView::unsetCursor()
{
    viewport()->unsetCursor();
    QGraphicsView::unsetCursor();
}

void CGUIView::OnSetActivePlane(CAirplane* pPlane)
{
    if(m_pReminder)
    {
        QSharedPointer<CAirplane>& pAirplane = GetAirplanes()[pPlane->GetID()];
        m_pReminder->SetPlane(pAirplane);
        m_pReminder->SetReminders(pPlane->GetReminders());
        m_pReminder->UpdateAzimuth();
    }

    const QSharedPointer<CAirplane>& pAirplane = GetAirplanes()[pPlane->GetID()];
    CListAC::SelPlaneRecord(pAirplane);
}
void CGUIView::SetFormularColor(const QColor& color)
{
    for ( CView::TAirplanesMap::iterator itPlane = GetAirplanes().begin();
    itPlane != GetAirplanes().end(); ++itPlane )
    {
        CGUIAirplane* pPlane = dynamic_cast<CGUIAirplane*>( itPlane->data() );
        if ( pPlane )
            pPlane->SetFormularColor( color );
    }
}
void CGUIView::SetGridColor(const QColor& color)
{
    if(m_rcGrid)
        m_rcGrid->SetColor(color);
}

void CGUIView::OnUpdateReminder(const QSharedPointer<CAirplane>& pPlane)
{
    if(m_pReminder)
        m_pReminder->UpdatePlane(pPlane);
}

void CGUIView::OnShangedSFPL(const QSharedPointer<CSFPL>& pSFPL)
{
    if(pSFPL)
        CListAC::RefreshAndSelect(pSFPL->GetID());
}

void CGUIView::SetAutoCollapse(bool bCollapse)
{
    TAirplanesMap::const_iterator itAirplans = GetAirplanes().begin();
    for(; itAirplans != GetAirplanes().end(); ++itAirplans)
    {
        CGUIAirplane* pPlane = dynamic_cast<CGUIAirplane*>(itAirplans->data());
        if(pPlane)
            pPlane->SetAutoCollapse(bCollapse);
    }
}

void CGUIView::OnResetLastActive()
{
    TAirplanesMap::const_iterator itAirplane = GetAirplanes().begin();
    for(; itAirplane != GetAirplanes().end(); ++itAirplane)
    {
        CGUIAirplane* pAirplane = dynamic_cast<CGUIAirplane*>(itAirplane->data());
        if(pAirplane)
            pAirplane->SetLastActive(false);
    }
}

void CGUIView::OnHighlightPlanes(const CHighlightPlane &val)
{
    TAirplanesMap::const_iterator itAirplane = GetAirplanes().begin();
    for(; itAirplane != GetAirplanes().end(); ++itAirplane)
    {
        CGUIAirplane* pAirplane = dynamic_cast<CGUIAirplane*>(itAirplane->data());
        if(pAirplane)
            pAirplane->SetHighlight(val);
    }
}

void CGUIView::SetSTCA(AirplaneCollision& ac)
{
    int idPlane1 = qMin(ac.id1, ac.id2);
    int idPlane2 = qMax(ac.id1, ac.id2);
    if(!m_mapSTCA.contains(QPair<int,int>(idPlane1, idPlane2)))
    {
        if(AirplaneCollision::NoCollision != ac.type)
        {
            TAirplanesMap::iterator itPlane1 = GetAirplanes().find(idPlane1);
            TAirplanesMap::iterator itPlane2 = GetAirplanes().find(idPlane2);
            if((itPlane1 != GetAirplanes().end()) && (itPlane2 != GetAirplanes().end()))
            {
                CGUILineText* pTempLine = new CGUILineText(0);
                pTempLine->setZValue(gui::Zstca);
                pTempLine->SetColor(Qt::red);
                pTempLine->SetPenWidth(1.5);
                pTempLine->SetAutoText(CGUILineText::Distance);

                double dScale = m_qWidth/m_qXSize;
                if (dScale > m_qHeight/m_qYSize)
                    dScale = m_qHeight/m_qYSize;
                dScale *= pow(1.125, m_iScale);
                pTempLine->SetPosition(m_qXMin, m_qYMin + m_qYSize, dScale, m_pViewCentralPoint->GetCoord());

                QSharedPointer<CGUILineText> pLine = QSharedPointer<CGUILineText>(pTempLine);
                m_mapSTCA.insert(QPair<int,int>(idPlane1, idPlane2), pLine);
                scene()->addItem(pLine.data());

                pLine->SetPoints(*itPlane1, *itPlane2);
            }
        }
    }

    if ( ac.type != AirplaneCollision::NoCollision )
        QApplication::beep();
}

void CGUIView::RemoveSTCA(quint16 idPlane)
{
    TMapSTCA::iterator itSTCA = m_mapSTCA.begin();
    while(itSTCA != m_mapSTCA.end())
    {
        QPair<int,int> keySTCA = itSTCA.key();
        if((keySTCA.first == idPlane) || (keySTCA.second == idPlane))
        {
            scene()->removeItem((*itSTCA).data());
            itSTCA = m_mapSTCA.erase(itSTCA);
        }
        else
        {
            ++itSTCA;
        }
    }
}

void CGUIView::CheckPlanesAlt()
{
    TAirplanesMap::const_iterator itAirplane = GetAirplanes().begin();
    for(; itAirplane != GetAirplanes().end(); ++itAirplane)
    {
        CGUIAirplane* pAirplane = dynamic_cast<CGUIAirplane*>(itAirplane->data());
        if(pAirplane)
            pAirplane->CheckPlaneAlt();
    }
}

void CGUIView::OnTracerHDG(CAirplane *receiver)
{
    QHash<CAirplane*, QSharedPointer<CTracerHDG> >::iterator itTracer = airplane_to_tracer_.find(receiver);
    if (itTracer != airplane_to_tracer_.end()) {
        scene()->removeItem((*itTracer).data());
        airplane_to_tracer_.erase(itTracer);

        CGUIAirplane *airplane = dynamic_cast<CGUIAirplane*>(receiver);
        if (airplane)
            airplane->ClearAzimuth2();

        mode_airplane_ = 0;
        mode_ = NONE_MODE;
    }
    else
    {
        TAirplanesMap::const_iterator itFind = GetAirplanes().find(receiver->GetID());
        if (itFind != GetAirplanes().end())
        {
            QSharedPointer<CTracerHDG> pTracerHDG(new CTracerHDG(*itFind));
            scene()->addItem(pTracerHDG.data());
            double dScale = m_qWidth/m_qXSize;
            if (dScale > m_qHeight/m_qYSize)
               dScale = m_qHeight/m_qYSize;
            dScale *= pow(1.125, m_iScale);
            pTracerHDG->SetPosition(m_qXMin, m_qYMin + m_qYSize, dScale, m_pViewCentralPoint->GetCoord());
            pTracerHDG->Update(GetCoordAtPoint(mapToScene(mapFromGlobal(QCursor::pos()))));
            airplane_to_tracer_.insert(receiver, pTracerHDG);
            mode_airplane_ = receiver;
            mode_ = TRACER_HDG;
        }
    }
}

void CGUIView::onSetNextPointSearchMode(CAirplane *receiver)
{
    setCursor(Qt::CrossCursor);
    mode_airplane_ = receiver;
    mode_ = NEXT_POINT_SEARCH;
}

void CGUIView::RecieveOLDIMessage(QSharedPointer<OLDIMessage>& pMes)
{
    TAirplanesMap::const_iterator itAirplane = GetAirplanes().begin();
    for(; itAirplane != GetAirplanes().end(); ++itAirplane)
    {
        if((*itAirplane)->RecieveOLDIMessage(pMes))
            return;
    }
}

void CGUIView::setCoordDigitizer(bool multipleCoords /* = true */)
{
    m_coordsDigitizer = TCoordsDigitizer(new CoordsDigitizer());
    m_coordsDigitizer->setMultiplePick(multipleCoords);
    m_coordsDigitizer->setVisible(true);
    scene()->addItem(m_coordsDigitizer.data());
}

void CGUIView::startCoordsPicking(bool multipleCoords /* = true */)
{
    stopCoordsPicking();

    setCoordDigitizer(multipleCoords);
}

void CGUIView::stopCoordsPicking()
{
    if (m_coordsDigitizer) {
        scene()->removeItem(m_coordsDigitizer.data());
        m_coordsDigitizer.clear();

        // Add new restriction zone if this mode is specified.
        // See RestrictionsWidget::Add command handler.
        if (gWidgetRestriction)
            gWidgetRestriction->addZone();
    }

    /////// ???
    if (MAP_NOTES == mode_)
        setCoordDigitizer(false);
}

void CGUIView::onSetMapNotesMode(bool checked)
{
    if (checked) {
        startCoordsPicking(false);
        mode_ = MAP_NOTES;
    } else {
        resetModeData();
        stopCoordsPicking();
    }
}

void CGUIView::onHighlightCenter()
{
    m_radius = kStartRadius;
    onDrawCircle();
}

void CGUIView::OnTimerRefreshListACC()
{
    m_bRefreshListACC = false;
    CListAC::RefreshAndSelect(0);
    QSharedPointer<obelix::CCmdRequestAllFPL> pCmd(new obelix::CCmdRequestAllFPL());
    ((XMasterWindow*)getMainWindow())->GetObClient().XMLSend(pCmd.staticCast<CXMLCmd>());
    //RequestAllFPL();
}

void CGUIView::OnChangeACID(quint16 idAirplane)
{
    CView::TAirplanesMap::iterator it = GetAirplanes().find(idAirplane);
    // Probably airplane was destroyed.
    if ( it == GetAirplanes().end() )
        return;

    QSharedPointer<CAirplane> airplane = *it;

    QInputDialog dialog(this);
    dialog.setWindowTitle(tr("Enter ACID (aircraft id)"));
    dialog.setLabelText(tr("Aircraft Id:"));
    dialog.setTextValue( airplane->GetACID() );
    dialog.setTextEchoMode(QLineEdit::Normal);
    connect(&dialog, SIGNAL(textValueChanged(const QString &)),
            this, SLOT(on_acid_textValueSelected(const QString &)));

    if ( dialog.exec() == QInputDialog::Accepted )
    {
        CGUIAirplane *guiAirplane = dynamic_cast<CGUIAirplane *>(airplane.data());
        if ( guiAirplane )
        {
            guiAirplane->SetACID( dialog.textValue() );
            guiAirplane->UpdatePosition();
        }
    }
}

void CGUIView::on_acid_textValueSelected(const QString & text)
{
    if ( text.length() > 7 )
    {
        QInputDialog *dlg = qobject_cast<QInputDialog *>( sender() );
        if ( dlg )
            dlg->setTextValue(text.left(7));
    }
}

void CGUIView::BearingData(CBearingData& BearingData)
{
    if (!m_bearingId.isEmpty() && GetMainFIR())
    {
        CBearingPoint bearingPoint;
        foreach (const CBearingPoint &p, m_listDF)
        {
            if (m_bearingId.contains(p.GetID()) &&
                    p.GetSAC() == BearingData.GetSAC() &&
                    p.GetSIC() == BearingData.GetSIC() &&
                    p.GetChannel() == BearingData.GetID())
            {
                bearingPoint = p;
                break;
            }
        }

        if (!bearingPoint.GetCoord().IsNull())
        {
            qint64 timestamp = QDateTime::currentMSecsSinceEpoch();
            if (timestamp - m_bearingTimestamps.value(bearingPoint.GetID()) >= 1000)
            {
                int declination;
                declination = bearingPoint.GetDeclination();
                DelBearing(bearingPoint.GetID());

                CGUILineText* pBearingLine = new CGUILineText(this);
                pBearingLine->setZValue(gui::Zbearing);

                //pBearingLine->SetColor(Qt::yellow);
                QString nameAirport = dao()->GetNameAirport(bearingPoint.GetAirportID());
                QString frequency_name = nameAirport + "_" + bearingPoint.GetName();
                UserProfile::CSettings& stg = UserProfile::CSettings::Instance();
                stg.SetFrequency(frequency_name);
                stg.Load();
                QColor colorNew= stg.GetColor(UserProfile::CtFrequency);
                pBearingLine->SetColor(colorNew);

                pBearingLine->SetPenWidth(1.5);
                double dScale = m_qWidth/m_qXSize;
                if (dScale > m_qHeight/m_qYSize)
                    dScale = m_qHeight/m_qYSize;
                dScale *= pow(1.125, m_iScale);
                pBearingLine->SetPosition(m_qXMin, m_qYMin + m_qYSize, dScale, m_pViewCentralPoint->GetCoord());
                scene()->addItem(pBearingLine);
                m_pBearingLines.insert(bearingPoint.GetID(), pBearingLine);

                bearingPoint.GetCoord().Center(m_pViewCentralPoint->GetCoord());
                QPointF point1;
                point1.setX((bearingPoint.GetCoord().GetX()-m_qXMin)*dScale);
                point1.setY((m_qYMin+m_qYSize-bearingPoint.GetCoord().GetY())*dScale);
                QLineF line;
                line.setP1(point1);

                double angle = BearingData.GetBearing();
                int angleB = qCeil(angle);
                angle = angle + declination;

                angle = (90 - angle) + 360;
                line.setAngle(fmod(angle, 360.0));
                line.setLength(10000000*dScale); // 10000 km
                CCoord coord2 = GetCoordAtPoint(line.p2());
                pBearingLine->SetPoints(bearingPoint.GetCoord(), coord2, CGUILineText::LeftPoint);

                QString dAngle = QString("%1%2").arg(angleB, 3, 10, QLatin1Char('0')).arg(QString::fromUtf8("\u00B0"));
                QString rAngle = QString("%1%2").arg((angleB + 180) % 360, 3, 10, QLatin1Char('0')).arg(QString::fromUtf8("\u00B0"));
                QString bearingText = QString("%1 (%2) %3").arg(dAngle).arg(rAngle).arg(bearingPoint.GetName());
                pBearingLine->SetText(bearingText, Qt::AlignLeft);

                m_bearingTimestamps.insert(bearingPoint.GetID(), timestamp);

            }

            if (!m_pBearingTimers.contains(bearingPoint.GetID()))
            {
                QTimer* timer = new QTimer(this);
                timer->setInterval(networkSettings()->value("simulator/bearing/bearing_show_time", "10").toInt() * 1000);
                timer->setSingleShot(true);
                connect(timer, SIGNAL(timeout()), m_pBearingSignalMapper, SLOT(map()));
                m_pBearingSignalMapper->setMapping(timer, bearingPoint.GetID());
                m_pBearingTimers.insert(bearingPoint.GetID(), timer);
            }
            m_pBearingTimers.value(bearingPoint.GetID())->start();
        }
    }
}

void CGUIView::DelBearing(int id)
{
    CGUILineText* pBearingLine = m_pBearingLines.take(id);
    if (pBearingLine)
    {
        QString m_plastBearing = pBearingLine->GetText().left(10).remove(5,1).replace(4,1,"/");
        emit lastBearing(m_plastBearing);
        scene()->removeItem(pBearingLine);
        delete pBearingLine;
    }
}

void CGUIView::timerEvent(QTimerEvent *event)
{
    if(CListAC::lastTimeRefresh().secsTo(QDateTime::currentDateTime()) > 21)
        CListAC::RefreshAndSelect(0);
}

void CGUIView::RequestAllFPL()
{
    // request all FPLs
    OLDIMesDataINF mesData;
    mesData.iMesNum = master()->GetOLDIProc()->GetNextMesNum();
    mesData.sFrom = networkSettings()->value("oldi/name", "Unknown").toString();
    mesData.sTo = networkSettings()->value("users/current_fdp_name", "FDP").toString();
    mesData.sTypeINF = oldi::INF_FPL;
    mesData.sACID = " ";
    mesData.sADEP = " ";
    mesData.sADES = " ";
    mesData.sCODE = " ";
    mesData.sSector = " ";
    QSharedPointer<OLDIMessage> pMes(new OLDIMesINF(mesData));
    //connect(pMes.data(), SIGNAL(messageAnswer(const OLDIMessage*,const QStringList&)), this, SLOT(OLDIMesAnswer(const OLDIMessage*,const QStringList&)));
    //connect(pMes.data(), SIGNAL(messageTimeOut(const OLDIMessage*)), this, SLOT(OLDIMesTimeout(const OLDIMessage*)));
    master()->GetOLDIProc()->SendMessage(pMes);
}

void CGUIView::OnMeteoRefresh()
{
    if(GetMainFIR())
    {
        TVA_CATCHALL_TRY
        {
            dao()->LoadMeteo(GetMainFIR()->GetID(), *this);
            Draw();
        } TVA_CATCHALL(err)
        {
            Q_UNUSED(err)
        }
    }
}

void CGUIView::DelMeteo(uint uiZoneID)
{
    TMeteoList::iterator itMeteo = GetMeteo().begin();
    for(; itMeteo != GetMeteo().end(); ++itMeteo)
    {
        if(uiZoneID == (*itMeteo)->GetZoneID())
        {
            QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*>(itMeteo->data());
            if (pItem && scene())
                scene()->removeItem(pItem);
        }
    }
    CView::DelMeteo(uiZoneID);
}

void CGUIView::SetRcGridCenter(const RcGridValues &values)
{
    if(GetMainFIR())
    {
        CGUIRcGrid::SaveRcGridValues(GetMainFIR()->GetIDName(), values);
        if(m_rcGrid)
        {
            RcGridValues cc(values);
            if(!cc.mCenter.IsNull())
                cc.mCenter.Center(m_pViewCentralPoint->GetCoord());
            m_rcGrid->setRcGridValues(cc);
            Draw();
        }
    }
}

void CGUIView::LoadDF()
{
    m_listDF.clear();

    if(GetMainFIR())
    {
        QString airportOur;
        TVA_CATCHALL_TRY
        {
            //dao()->LoadBearingList(GetMainFIR()->GetIDName(), m_listDF);
            airportOur = networkSettings()->value("meteo/airport").toString();
            quint32 idOurAirport = dao()->GetIdAirport(networkSettings()->value("meteo/airport").toString());
            dao()->LoadBearingList(idOurAirport, m_listDF);
        }TVA_CATCHALL(err){
        LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);}
        if(!m_listDF.size())
        {
            //LOG_STR(ERROR, tr("Not bearing point for %1").arg(GetMainFIR()->GetIDName()));
            LOG_STR(ERROR, tr("Not bearing point for %1").arg(airportOur));
        }
    }
}

QList<CBearingPoint>& CGUIView::GetListDF()
{
    return m_listDF;
}

void CGUIView::setBearingId(const TBearingIdSet &id)
{
    foreach (int id, m_bearingId - id)
    {
        DelBearing(id);

        QTimer* pBearingTimer = m_pBearingTimers.take(id);
        if (pBearingTimer)
        {
            delete pBearingTimer;
        }
    }

    m_bearingId = id;
}

void CGUIView::OnDraw()
{
    Draw();
    OnDrawAirplanes();
    Redraw();
}

void CGUIView::RedrawAll()
{
    QMetaObject::invokeMethod(this, "OnDraw", Qt::QueuedConnection);
}

void CGUIView::UpdateFormularType()
{
    TAirplanesMap::iterator itAirplan = GetAirplanes().begin();
    for(; itAirplan != GetAirplanes().end(); ++itAirplan)
    {
        CGUIAirplane* pPlane = dynamic_cast<CGUIAirplane*>(itAirplan->data());
        if(pPlane)
            pPlane->UpdateFormularType();
    }
}

void CGUIView::AddTempLine(const CCoord& coord1, const CCoord& coord2)
{
    CGUILineText* pTempLine = new CGUILineText(this);
    pTempLine->setZValue(gui::Zgauge);
    pTempLine->SetColor(Qt::red);
    pTempLine->SetAutoText(CGUILineText::DistanceWithAngleAndTime);
    double dScale = m_qWidth/m_qXSize;
    if (dScale > m_qHeight/m_qYSize)
        dScale = m_qHeight/m_qYSize;
    dScale *= pow(1.125, m_iScale);
    pTempLine->SetPosition(m_qXMin, m_qYMin + m_qYSize, dScale, m_pViewCentralPoint->GetCoord());
    scene()->addItem(pTempLine);
    pTempLine->SetPoints(coord1, coord2);
}

void CGUIView::OnMTCD()
{
    if(m_MTCDfutwatch && !m_MTCDfutwatch->isRunning() && networkSettings()->value("airplane/MTCD_enable", true).toBool())
    {
        if(!CListAC::isFutureRefreshRunning() /* + some conditions, sample QDateTime */)
        {
            qDebug() << "MTCD new run " << QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
            m_MTCDmapPlane.clear();
            QSharedPointer<QLinkedList<QSharedPointer<mtcd::MTCD> > > pListMTCD(new QLinkedList<QSharedPointer<mtcd::MTCD> >);
            //QLinkedList<QPair<QSharedPointer<mtcd::MTCD>, QSharedPointer<QLinkedList<QSharedPointer<mtcd::MTCD> > > > > listPair;
            m_listPair.clear();
            for(TAirplanesMap::const_iterator itPlane = GetAirplanes().constBegin(); itPlane != GetAirplanes().constEnd(); ++itPlane)
            {
                // append points
                if((*itPlane)->GetSFPL() && (*itPlane)->GetSFPL()->GetFPLFrag())
                {
                    QSharedPointer<mtcd::MTCD> planeMTCD(new mtcd::MTCD((*itPlane)->GetID(), (*itPlane)->GetGS(), m_pViewCentralPoint->GetCoord()));
                    int Hmin(0), Hmax(0);
                    if((*itPlane)->GetFL() >= GetEchelon())
                    {
                        Hmin = Hmax = qRound((*itPlane)->GetFLinMeters());
                        if((*itPlane)->GetCFL())
                        {
                            Hmin = qMin(Hmin, qRound((*itPlane)->GetCFL().GetM()));
                            Hmax = qMax(Hmax, qRound((*itPlane)->GetCFL().GetM()));
                        }
//                        if((*itPlane)->GetTFL())
//                        {
//                            Hmin = qMin(Hmin, qRound((*itPlane)->GetTFL().GetM()));
//                            Hmax = qMax(Hmax, qRound((*itPlane)->GetTFL().GetM()));
//                        }
                    }
                    CCoord coord1, coord2;
                    CProcFrag::TPointFragList pPointList = (*itPlane)->GetSFPL()->GetFPLFrag()->GetNotPassedPoint();
                    CProcFrag::TPointFragList::const_iterator itPoint = pPointList.constBegin();
                    for(int i = 0; itPoint != pPointList.constEnd(); ++i, ++itPoint)
                    {
                        if(i == 0)
                        {
                            coord1 = (*itPlane)->GetCoord();
                            coord2 = (*itPoint)->GetPoint() ? (*itPoint)->GetPoint()->GetCoord() : CCoord();
                        }
                        else
                        {
                            coord1 = coord2;
                            coord2 = (*itPoint)->GetPoint() ? (*itPoint)->GetPoint()->GetCoord() : CCoord();
                        }
//                        if((*itPoint)->IsLevel())
//                        {
//                            if((*itPoint)->GetLevel() >= GetEchelon()*30.48)
//                            {
//                                Hmin = qMin(Hmin, qRound((*itPoint)->GetLevel()));
//                                Hmax = qMax(Hmax, qRound((*itPoint)->GetLevel()));
//                            }
//                        }
                        mtcd::MTCDin mtcdin;
                        mtcdin.coord1 = coord1;
                        mtcdin.coord2 = coord2;
                        mtcdin.Hmin = Hmin;
                        mtcdin.Hmax = Hmax;

                        planeMTCD->AddPoint(mtcdin);
                    }
                    if(!planeMTCD->IsEmpty())
                    {
                        pListMTCD->append(planeMTCD);
                        QPair<QSharedPointer<mtcd::MTCD>, QSharedPointer<QLinkedList<QSharedPointer<mtcd::MTCD> > > > pr(planeMTCD, pListMTCD);
                        m_listPair.append(pr);
                    }
                }
            }
            if(m_listPair.size())
            {
                m_MTCDfutwatch->setFuture(QtConcurrent::mapped(m_listPair, &CGUIView::CalcMTCD));
                // for debug
//                QLinkedList<QPair<QSharedPointer<mtcd::MTCD>, QSharedPointer<QLinkedList<QSharedPointer<mtcd::MTCD> > > > >::const_iterator itListPair = listPair.constBegin();
//                for(; itListPair != listPair.constEnd(); ++itListPair)
//                {
//                    mtcd::TListMTCDPtr pList = CalcMTCD(*itListPair);
//                    debugOnMTCDfutwatchResult(pList);
//                }
//                debugOnMTCDfutwatchFinish();
                ///////////////////////////////////////////////////
            }
        }
    }
    else
    {
        if(m_MTCDfutwatch)
            qDebug() << "MTCD running";
    }
}

void CGUIView::OnMTCDfutwatchResult(int index)
{
    if (m_MTCDforceStop) return;

    qDebug() << QString("MTCD result for index = %1 ").arg(index) << QDateTime::currentDateTime().toString("hh:mm:ss.zzz");

    mtcd::TListMTCDPtr listMTCD = m_MTCDfutwatch->resultAt(index);
    if (listMTCD && listMTCD->size())
    {
        mtcd::TMTCDPtr pMTCD = listMTCD->first();
        if (pMTCD)
        {
            CView::TAirplanesMap::iterator itPlane = GetAirplanes().find(pMTCD->planeID1);
            if (itPlane != GetAirplanes().end())
            {
                (*itPlane)->SetMTCD(listMTCD);
                m_MTCDmapPlane.insert(pMTCD->planeID1, listMTCD);
            }
        }
    }
}

void CGUIView::OnMTCDfutwatchFinish()
{
    qDebug() << "MTCD finish "<< QDateTime::currentDateTime().toString("hh:mm:ss.zzz");

    if (m_MTCDforceStop) return;

    for (TAirplanesMap::const_iterator itPlane = GetAirplanes().constBegin(); itPlane != GetAirplanes().constEnd(); ++itPlane)
    {
        if (!m_MTCDmapPlane.contains((*itPlane)->GetID()))
            (*itPlane)->SetMTCD(mtcd::TListMTCDPtr());
    }
}

void CGUIView::onLockMoving()
{
    m_isLocked = true;
}

void CGUIView::onUnlockMoving()
{
    m_isLocked = false;
}

void CGUIView::onDrawCircle()
{
    if (m_circle) {
        delete m_circle;
        m_circle = 0;
    }

    if (!m_radius)
        return;

    const QPointF center = mapToScene(viewport()->rect().center());
    const QRect rect(center.x() - m_radius / 2, center.y() - m_radius / 2, m_radius, m_radius);
    m_circle = scene()->addEllipse(rect, QPen(qRgb(0, 0, 0)));
    m_circle->setZValue(gui::Zcircles);

    m_radius /= kDivisor;

    QTimer::singleShot(35, this, SLOT(onDrawCircle()));
}

void CGUIView::debugOnMTCDfutwatchResult(mtcd::TListMTCDPtr pList)
{
    if (pList && pList->size())
    {
        mtcd::TMTCDPtr pMTCD = pList->first();
        if (pMTCD)
        {
            CView::TAirplanesMap::iterator itPlane = GetAirplanes().find(pMTCD->planeID1);
            if (itPlane != GetAirplanes().end())
            {
                (*itPlane)->SetMTCD(pList);
                m_MTCDmapPlane.insert(pMTCD->planeID1, pList);
            }
        }
    }
}

void CGUIView::debugOnMTCDfutwatchFinish()
{
    for (TAirplanesMap::const_iterator itPlane = GetAirplanes().constBegin(); itPlane != GetAirplanes().constEnd(); ++itPlane)
    {
        if (!m_MTCDmapPlane.contains((*itPlane)->GetID()))
            (*itPlane)->SetMTCD(mtcd::TListMTCDPtr());
    }
}

mtcd::TListMTCDPtr CGUIView::CalcMTCD(const QPair<QSharedPointer<mtcd::MTCD>, QSharedPointer<QLinkedList<QSharedPointer<mtcd::MTCD> > > >& planeMTCD)
{
    mtcd::TListMTCDPtr listRet(new mtcd::TListMTCD);
    if (planeMTCD.first && planeMTCD.second)
    {
        QLinkedList<QSharedPointer<mtcd::MTCD> >::const_iterator itList = planeMTCD.second->constBegin();
        for(; itList != planeMTCD.second->constEnd(); ++itList)
        {
            if(m_MTCDforceStop) return mtcd::TListMTCDPtr();

            mtcd::TListMTCDPtr listTmp = planeMTCD.first->CalculateMTCD(*(*itList));
            if(listTmp && listTmp->size())
                *listRet += *listTmp;
        }
    }
    return listRet;
}

qint16 CGUIView::GetEchelon() const
{
    return m_qEchelon;
}

qint16 CGUIView::GetPress() const
{
     return m_qPress;
}

void CGUIView::UpdateGaugeColor()
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();
    QColor gaugeColor = stg.GetColor(UserProfile::CtGauge);
    QMap<CGUILineText*, QSharedPointer<CGUILineText> >::iterator itList = m_mapGauge.begin();
    for(; itList != m_mapGauge.end(); ++itList)
    {
        if(!itList->isNull())
        {
            itList->data()->SetText(QString::null);
            itList->data()->SetColor(gaugeColor);
        }
    }
}

void CGUIView::createVectorMeasurement(const QPoint &pos)
{
    m_pTempLine = new CGUILineText(this);
    m_pTempLine->setZValue(gui::Zgauge);
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();
    m_pTempLine->SetColor(stg.GetColor(UserProfile::CtGauge));
    m_pTempLine->SetPenWidth(1.5);
    m_pTempLine->SetAutoText(CGUILineText::DistanceWithMileAngleTime);
    m_pTempLine->SetTurnTimes(networkSettings()->value("airplane/turn_time/forward").toUInt(),
                              networkSettings()->value("airplane/turn_time/side").toUInt(),
                              networkSettings()->value("airplane/turn_time/backward").toUInt());

    double dScale = m_qWidth/m_qXSize;
    if (dScale > m_qHeight/m_qYSize)
        dScale = m_qHeight/m_qYSize;
    dScale *= pow(1.125, m_iScale);
    m_pTempLine->SetPosition(m_qXMin, m_qYMin + m_qYSize, dScale, m_pViewCentralPoint->GetCoord());

    QSharedPointer<CGUILineText> vector(m_pTempLine);
    m_mapGauge.insert(vector.data(), vector);
    scene()->addItem(vector.data());

    CCoord coord1(GetCoordAtPoint(mapToScene(pos)));
    QList<QGraphicsItem*> listItems = scene()->items(mapToScene(pos));
    QList<QGraphicsItem*>::const_iterator itList = listItems.begin();
    CGUIAirplane* pPlane = 0;
    for (; itList != listItems.end(); ++itList) {
        CGUIAirplanePoint *plane_point = dynamic_cast<CGUIAirplanePoint*>(*itList);
        if (plane_point)
            pPlane = dynamic_cast<CGUIAirplane*>(plane_point->parentItem());
        else
            pPlane = dynamic_cast<CGUIAirplane*>(*itList);
        if (pPlane)
            break;
    }
    if (pPlane) {
        TAirplanesMap::const_iterator itFind = GetAirplanes().find(pPlane->GetID());
        if (itFind != GetAirplanes().end())
            vector->SetPoint1(*itFind);
    } else {
        vector->SetPoint1(coord1);
    }
    vector->SetPoint2(coord1);
}

void CGUIView::resetModeData()
{
    unsetCursor();

    QHash<CAirplane*, QSharedPointer<CTracerHDG> >::iterator it_tracer = airplane_to_tracer_.find(mode_airplane_);
    if (it_tracer != airplane_to_tracer_.end()) {
        scene()->removeItem((*it_tracer).data());
        airplane_to_tracer_.erase(it_tracer);
    }

    mode_airplane_ = 0;
    mode_ = NONE_MODE;
}

void CGUIView::setNextPoint(const CCoord &coords)
{
    //QSharedPointer<CGUIView> guard(this, &CGUIView::resetModeData);

    CGUIAirplane *airplane = dynamic_cast<CGUIAirplane*>(mode_airplane_);
    if (airplane && airplane->GetSFPL() && airplane->GetSFPL()->GetFPLFrag())
    {
        double minDist = 10000;

        bool hasRoute = !airplane->GetSFPL()->GetRoute().isEmpty();
        if (hasRoute)
        {
            int pointIndex = -1;

            CProcFrag::TPointFragList routePoints = airplane->GetSFPL()->GetFPLFrag()->GetAllPoint();
            CProcFrag::TPointFragList::const_iterator it_point = routePoints.constBegin();
            for (; it_point != routePoints.constEnd(); ++it_point)
            {
                const QSharedPointer<CPoint> point = (*it_point)->GetPoint();
                if (point)
                {
                    if ( (point->GetType() == CPoint::enVHFNavAid) ||
                         (point->GetType() == CPoint::enNDBNavAid) ||
                         (point->GetType() == CPoint::enWaypoint)
                         )
                    {
                        double dist = GetDistance(point->GetCoord(), coords);
                        if (dist < 5000)
                        {
                            if (dist < minDist)
                            {
                                minDist = dist;
                                pointIndex = it_point - routePoints.constBegin();
                            }
                        }
                    }
                }
            }

            if (pointIndex != -1)
            {
                airplane->GetSFPL()->GetFPLFrag()->SetNextPoint(pointIndex);
                airplane->CalculateSectors(airplane->GetSFPL());

                QStringList points;
                foreach (const CProcFrag::TPointFragPtr &point, airplane->GetSFPL()->GetFPLFrag()->GetNotPassedPoint())
                {
                    points.append(point->GetPoint()->GetName());
                }
                logOperation(tr("%1: route changed to %2, new sectors list: %3")
                             .arg(airplane->GetSFPL()->GetACID())
                             .arg(points.join("-"))
                             .arg(airplane->GetSFPL()->GetAllSectors()));

                airplane->setNextPoint(routePoints[pointIndex]->GetPoint());
                if (airplane->isShowPathPoints())
                    airplane->ShowPathPoints();
            }
        }
        else
        {
            QSharedPointer<CPoint> findPoint;

            CView::TSceneList::const_iterator it_fir = GetScenes().constBegin();
            for (; it_fir != GetScenes().constEnd(); ++it_fir)
            {
                Scene::TPointsMap::const_iterator it_point = (*it_fir)->GetPoints().begin();
                for (; it_point != (*it_fir)->GetPoints().end(); ++it_point)
                {
                    const QSharedPointer<CPoint> point = *it_point;
                    if (point)
                    {
                        if ( (point->GetType() == CPoint::enVHFNavAid) ||
                             (point->GetType() == CPoint::enNDBNavAid) ||
                             (point->GetType() == CPoint::enWaypoint)
                             )
                        {
                            double dist = GetDistance(point->GetCoord(), coords);
                            if (dist < 5000)
                            {
                                if (dist < minDist)
                                {
                                    minDist = dist;
                                    findPoint = point;
                                }
                            }
                        }
                    }
                }
            }

            if (findPoint)
            {
                airplane->GetSFPL()->GetFPLFrag()->SetNextCustomPoint(findPoint);

                airplane->setNextPoint(findPoint);
                if (airplane->isShowPathPoints())
                    airplane->ShowPathPoints();
            }
        }
    }

    resetModeData();
}

void CGUIView::onGetPickCoord(bool bMulti, QUuid id)
{
    mPickCoordID = id;
    startCoordsPicking(bMulti);
}

void CGUIView::onUserPicsChanged()
{
    if(GetMainFIR())
    {
        TVA_CATCHALL_TRY
        {
            { // delete
                Scene::TUserPicsList& pList = GetMainFIR()->GetUserPics();
                while(pList.size())
                {
                    QSharedPointer<CUserPics> userpics = GetMainFIR()->DelUserPics(pList[0]->GetID());
                    if(userpics)
                    {
                        CGUIUserPics* pItem = dynamic_cast<CGUIUserPics*>(userpics.data());
                        if(pItem)
                            scene()->removeItem(pItem);
                    }
                }
            }

            dao()->LoadUserPics(*GetMainFIR());

            { //add
                Scene::TUserPicsList& pList = GetMainFIR()->GetUserPics();
                for(Scene::TUserPicsList::iterator itList = pList.begin(); itList != pList.end(); ++itList)
                {
                    CGUIUserPics* pItem = dynamic_cast<CGUIUserPics*>((*itList).data());
                    if(pItem)
                        scene()->addItem(pItem);
                }
            }
            onUserPicsVisabilityChanged();
            Draw();
        } TVA_CATCHALL(err)
        {
            Q_UNUSED(err)
        }
    }
}
void CGUIView::userPicsSelectionDrop()
{
    if(!m_last_selected_show)
    {
        return;
    }
    if(GetMainFIR())
    {
        m_last_selected_show = false;
        Scene::TUserPicsList& pList = GetMainFIR()->GetUserPics();
        if(m_last_selected_pics <= pList.size()-1)
        {
            CGUIUserPics* pItem = dynamic_cast<CGUIUserPics*>(pList[m_last_selected_pics].data());
            if(pItem)
            {
                pItem->setSelectedPoint(m_last_selected_pic_row, m_last_selected_show);
            }
        }
    }
}
void CGUIView::userPicsSelectChanged(int index_pic, int row, bool bshow)
{
    if(GetMainFIR())
    {
        Scene::TUserPicsList& pList = GetMainFIR()->GetUserPics();
        if(index_pic <= pList.size()-1)
        {
            CGUIUserPics* pItem = dynamic_cast<CGUIUserPics*>(pList[index_pic].data());
            if(pItem)
            {
                pItem->setSelectedPoint(row, bshow);
                m_last_selected_pics = index_pic;
                m_last_selected_pic_row = row;
                m_last_selected_show = bshow;
            }
        }
    }
}
void CGUIView::onUserPicsVisabilityChanged()
{
    if(GetMainFIR())
    {
        Scene::TUserPicsList& pList = GetMainFIR()->GetUserPics();
        for(Scene::TUserPicsList::iterator itList = pList.begin(); itList != pList.end(); ++itList)
        {
            CGUIUserPics* pItem = dynamic_cast<CGUIUserPics*>((*itList).data());
            if(pItem)
                pItem->MarkItemActive(UserPicsWidget::IsUserPicstEnable(currentUserName, GetMainFIR()->GetID(), pItem->GetID()));
        }
    }
}
