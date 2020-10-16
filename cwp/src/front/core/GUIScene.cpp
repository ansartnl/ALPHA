#include "GUIScene.h"
#include "XMasterWindow.h"
#include "GUIPoint.h"
#include "GUIAirport.h"
#include "GUIRoute.h"
#include "GUISSA.h"
#include "GUISector.h"
#include "GUIRestriction.h"
#include "GUIHolding.h"
#include "GUIRunway.h"
#include "GUIMapNote.h"
#include "GUIUserPics.h"
#include "GUISvgItem.h"
#include "LayoutDlg.h"
#include "RunwayWidget.h"
#include "ProfileSettings.h"

#include <QtDebug>

#include <QGraphicsScene>
#include <QtGui/QPen>
#include <QtGui/QPainter>

CGUIScene::CGUIScene() :
    m_iPenWidth(1)
{
    setZValue(gui::Zscene);
    m_ItemList.append(this);
    penColor = UserProfile::CSettings::Instance().GetColor(
        UserProfile::CtBorder);
    brushColor
            = UserProfile::CSettings::Instance().GetColor(UserProfile::CtFIR);
}

CGUIScene::~CGUIScene()
{
    //	QList<QGraphicsItem*> ItemList = childItems();
    //	QList<QGraphicsItem*>::iterator itScan = ItemList.begin();
    //	for(; itScan != ItemList.end(); ++itScan)
    //		(*itScan)->setParentItem(NULL);
    //removeFromGroup(*itScan);

    /*TBoundary::iterator itBoundScan = m_Boundary.begin();
     for(; itBoundScan != m_Boundary.end(); ++itBoundScan)
     {
     QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*>(itBoundScan->data());
     if (pItem)
     scene()->removeItem(pItem);
     }*/
}

bool CGUIScene::AddPoint(const QSharedPointer<CPoint>& pPoint)
{
    if(Scene::AddPoint(pPoint))
    {
        QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*> (pPoint.data());
        if (pItem)
            m_ItemList.append(pItem);
        //pItem->setParentItem(this);
        //addToGroup(pItem);
        return true;
    }
    return false;
}

bool CGUIScene::AddAirport(const QSharedPointer<CAirport>& pAirport)
{
    if(Scene::AddAirport(pAirport))
    {
        QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*> (pAirport.data());
        if (pItem)
            m_ItemList.append(pItem);
        //pItem->setParentItem(this);
        //addToGroup(pItem);
        CAirport::TRunways::const_iterator itScanRun =
                pAirport->GetRunways().begin();
        for (; itScanRun != pAirport->GetRunways().end(); ++itScanRun)
        {
            pItem = dynamic_cast<QGraphicsItem*> (itScanRun->data());
            if (pItem)
                m_ItemList.append(pItem);

            CRunway::TSSA::const_iterator itScanSSA =
                    (*itScanRun)->GetSSA().begin();
            for (; itScanSSA != (*itScanRun)->GetSSA().end(); ++itScanSSA)
            {
                pItem = dynamic_cast<QGraphicsItem*> (itScanSSA->data());
                if (pItem)
                    m_ItemList.append(pItem);
                //pItem->setParentItem(this);
                //addToGroup(pItem);
            }
        }
        return true;
    }
    return false;
}

bool CGUIScene::AddRoute(const QSharedPointer<CRoute>& pRoute)
{
    if(Scene::AddRoute(pRoute))
    {
        QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*> (pRoute.data());
        if (pItem)
            m_ItemList.append(pItem);
        //pItem->setParentItem(this);
        //addToGroup(pItem);
        return true;
    }
    return false;
}

void CGUIScene::AddSector(const QSharedPointer<Scene>& pSector)
{
    Scene::AddSector(pSector);
    QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*> (pSector.data());
    if (pItem)
        m_ItemList.append(pItem);
    CGUISector* pGUISector = dynamic_cast<CGUISector*> (pSector.data());
    if (pGUISector)
    {
        GUISectorBorder* pGUIBorder = pGUISector->GetBorderItem();
        if (pGUIBorder)
            m_ItemList.append(pGUIBorder);
    }
    //pItem->setParentItem(this);
    //addToGroup(pItem);
}

void CGUIScene::AddNote(const QSharedPointer<MapNote> &pNote)
{
    pNote->setID(qHash(QString("%1%2")
          .arg(pNote->coord().GetAsString())
          .arg(pNote->text())));

    connect(static_cast<GUIMapNote*>(pNote.data()), SIGNAL(deleteNote(quint32)), this, SLOT(OnNoteDelete(quint32)));
    connect(static_cast<GUIMapNote*>(pNote.data()), SIGNAL(editNote(quint32)), this, SLOT(OnNoteEdit(quint32)));
    Scene::AddNote(pNote);
    QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*>(pNote.data());
    if (pItem)
    {
        m_ItemList.append(pItem);
        if (scene())
            scene()->addItem(pItem);

    }
}

void CGUIScene::AddUserPics(const QSharedPointer<CUserPics>& pUserPics)
{
    Scene::AddUserPics(pUserPics);
    QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*> (pUserPics.data());
    if (pItem)
        m_ItemList.append(pItem);
}

void CGUIScene::AddSvgItem(const QSharedPointer<SvgItem>& pSvgItem)
{
    Scene::AddSvgItem(pSvgItem);
    QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*> (pSvgItem.data());
    if (pItem)
        m_ItemList.append(pItem);
}

void CGUIScene::AddRestriction(const QSharedPointer<CRestriction>& pRestriction)
{
    Scene::AddRestriction(pRestriction);
    QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*> (pRestriction.data());
    if (pItem)
        m_ItemList.append(pItem);
    //pItem->setParentItem(this);
    //addToGroup(pItem);
}

void CGUIScene::AddUserRestriction(const QSharedPointer<CRestrictionUser>& pRestriction)
{
    Scene::AddUserRestriction(pRestriction);
    QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*> (pRestriction.data());
    if (pItem)
        m_ItemList.append(pItem);
    //pItem->setParentItem(this);
    //addToGroup(pItem);

    // Need to call this function to show/hide zone according to time plan
    // (time_begin .. time_end).
    CGUIRestrictionUser *guiRestriction = dynamic_cast<CGUIRestrictionUser *>(pRestriction.data());
    if (guiRestriction)
        guiRestriction->updateVisibility();
}

void CGUIScene::AddHolding(const QSharedPointer<CHolding>& pHolding)
{
    Scene::AddHolding(pHolding);
    QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*> (pHolding.data());
    if (pItem)
        m_ItemList.append(pItem);
    //pItem->setParentItem(this);
    //addToGroup(pItem);
}

QSharedPointer<CRestrictionUser> CGUIScene::DelUserRestriction(quint32 id)
{
    QSharedPointer<CRestrictionUser> restriction = Scene::DelUserRestriction(id);
    // Delete zone from item list cache.
    QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*> (restriction.data());
    if (pItem)
        m_ItemList.removeAll(pItem);
    return restriction;
}

QSharedPointer<CUserPics> CGUIScene::DelUserPics(quint32 id)
{
    QSharedPointer<CUserPics> userpics = Scene::DelUserPics(id);
    QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*> (userpics.data());
    if (pItem)
        m_ItemList.removeAll(pItem);
    return userpics;
}

QSharedPointer<SvgItem> CGUIScene::DelSvgItem(quint32 id)
{
    QSharedPointer<SvgItem> svgitem = Scene::DelSVG(id);
    QGraphicsItem* pItem = dynamic_cast<QGraphicsItem*> (svgitem.data());
    if (pItem)
        m_ItemList.removeAll(pItem);
    return svgitem;
}

QSharedPointer<CPoint> CGUIScene::CreatePoint(CPoint::Type type) const
{
    if (type != CPoint::enAirportPoint)
        return QSharedPointer<CPoint> (new CGUIPoint);
    return Scene::CreatePoint(type);
}

QSharedPointer<CAirport> CGUIScene::CreateAirport() const
{
    return QSharedPointer<CAirport> (new CGUIAirport);
}

QSharedPointer<CRunway> CGUIScene::CreateRunway() const
{
    return QSharedPointer<CRunway> (new CGUIRunway);
}

QSharedPointer<CRoute> CGUIScene::CreateRoute() const
{
    return QSharedPointer<CRoute> (new CGUIRoute);
}

QSharedPointer<CUserPics> CGUIScene::CreateUserPics() const
{
    return QSharedPointer<CUserPics> (new CGUIUserPics);
}

QSharedPointer<SvgItem> CGUIScene::CreateSvgItem(const QString& path) const
{
    return QSharedPointer<SvgItem> (new CGUISvgItem(path));
}

QSharedPointer<CRestriction> CGUIScene::CreateRestriction() const
{
    return QSharedPointer<CRestriction> (new CGUIRestriction);
}

QSharedPointer<CRestrictionUser> CGUIScene::CreateUserRestriction() const
{
    return QSharedPointer<CRestrictionUser> (new CGUIRestrictionUser);
}

QSharedPointer<CHolding> CGUIScene::CreateHolding() const
{
    return QSharedPointer<CHolding> (new CGUIHolding);
}

QSharedPointer<CSSA> CGUIScene::CreateSSA() const
{
    return QSharedPointer<CSSA> (new CGUISSA);
}

QSharedPointer<Scene> CGUIScene::CreateSector() const
{
    return QSharedPointer<Scene> (new CGUISector);
}

QSharedPointer<MapNote> CGUIScene::CreateNote() const
{
    return QSharedPointer<MapNote> (new GUIMapNote);
}

void CGUIScene::prepare(qint64& iXMin, qint64& iYMin, qint64& iXMax,
    qint64& iYMax)
{
    TBoundary::iterator itScan = m_Boundary.begin();
    if (itScan == m_Boundary.end())
        return;
    m_qXMin = (*itScan)->GetCoord().GetX();
    m_qYMin = (*itScan)->GetCoord().GetY();
    m_qXSize = m_qXMin;
    m_qYSize = m_qYMin;
    for (++itScan; itScan != m_Boundary.end(); ++itScan)
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
    iXMin = m_qXMin;
    iYMin = m_qYMin;
    iXMax = m_qXSize;
    iYMax = m_qYSize;

    m_qXSize -= m_qXMin;
    m_qYSize -= m_qYMin;

}

QRectF CGUIScene::boundingRect() const
{
    return m_Path.boundingRect();
}

QPainterPath CGUIScene::shape() const
{
    return m_Path;
}

void CGUIScene::paint(QPainter *painter,
    const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    QPen qPen(penColor);
    qPen.setWidth(m_iPenWidth);
    painter->setPen(qPen);
    QColor backColor = brushColor;
    if(!m_bIsMainFIR)
        backColor.setAlpha(backColor.alpha()/7);
    painter->setBrush(QBrush(backColor));
    painter->drawPath(m_Path);
    painter->restore();
}

void CGUIScene::SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale,
    const CCoord& centralCoord)
{
    m_Path = QPainterPath();
    TBoundary::const_iterator itScan = GetBoundary().begin();
    for (; itScan != GetBoundary().end(); ++itScan)
    {
        QPointF pnt(((*itScan)->GetCoord().GetX() - qllXMin) * dScale, (qllYMax
                - (*itScan)->GetCoord().GetY()) * dScale);
        if (itScan == GetBoundary().begin())
            m_Path.moveTo(pnt);
        else
            m_Path.lineTo(pnt);
    }
    m_Path.closeSubpath();
    //	setPos(m_Path.boundingRect().topLeft());
    //	m_Path.translate(-m_Path.boundingRect().topLeft());
}

void CGUIScene::SetLayouts()
{
    TRestrictionList::const_iterator itRestriction = GetRestriction().begin();
    for (; itRestriction != GetRestriction().end(); ++itRestriction)
    {
        CGUIRestriction *restriction = dynamic_cast<CGUIRestriction*> (itRestriction->data());
        if (restriction)
            restriction->MarkItemActive(LayoutDlg::IsRestrictionEnable(
                                             QString("system"), (qint32)restriction->GetType()));
    }

    TRestrictionUserList::const_iterator itRestrictionUser = GetRestrictionUser().begin();
    for (; itRestrictionUser != GetRestrictionUser().end(); ++itRestrictionUser)
    {
        if ((*itRestrictionUser)->isValid(QDateTime::currentDateTime()))
        {
            CGUIRestrictionUser *restriction = dynamic_cast<CGUIRestrictionUser*>(itRestrictionUser->data());
            if (restriction) {
                if (restriction->GetType() == CRestrictionUser::enMap)
                    restriction->MarkItemActive(LayoutDlg::IsAreaEnable(GetIDName(), restriction->GetName()));
                else
                    restriction->MarkItemActive(LayoutDlg::IsRestrictionEnable(QString("user"), (qint32)restriction->GetType()));
            }
        }
    }

    THoldingList::const_iterator itHolding = GetHolding().begin();
    for (; itHolding != GetHolding().end(); ++itHolding)
    {
        CGUIHolding* pHolding = dynamic_cast<CGUIHolding*> (itHolding->data());
        if (pHolding)
        {
//            pHolding->MarkItemActive(LayoutDlg::IsLayoutEnable(GetIDName(),
//                LayoutDlg::Holding));
            pHolding->MarkItemActive(LayoutDlg::IsHoldingEnable(GetIDName(),
                (*itHolding)->GetID()));
        }
    }

    TPointsMap::const_iterator itPoint = GetPoints().begin();
    for (; itPoint != GetPoints().end(); ++itPoint)
    {
        CGUIPoint* pPoint = dynamic_cast<CGUIPoint*> (itPoint->data());
        if (pPoint)
        {
            pPoint->MarkItemActive(LayoutDlg::IsLayoutEnable(GetIDName(),
                LayoutDlg::Point));
            if(pPoint->GetType() == CPoint::enSSAWaypoint)
            {
                pPoint->MarkItemActive(false);
            }
        }
    }

    TNotesMap::const_iterator itNote = GetNotes().begin();
    for(; itNote != GetNotes().end(); ++itNote)
    {
        GUIMapNote* pNote = dynamic_cast<GUIMapNote*>(itNote->data());
        if(pNote)
            pNote->MarkItemActive(LayoutDlg::IsLayoutEnable(GetIDName(), LayoutDlg::MapNote));
            //pNote->setVisible(LayoutDlg::IsLayoutEnable(GetIDName(), LayoutDlg::MapNote));
    }

    TRoutsMap::const_iterator itRoute = GetRoutes().begin();
    for (; itRoute != GetRoutes().end(); ++itRoute)
    {
        CGUIRoute* pRoute = dynamic_cast<CGUIRoute*> (itRoute->data());
        if (pRoute)
            pRoute->MarkItemActive(LayoutDlg::IsLayoutEnable(GetIDName(),
                LayoutDlg::Route));
    }

    QString airport = networkSettings()->value("meteo/airport").toString();
    //QString runway;
    QString rw1,rw2;
    if (networkSettings()->value("RW1/runway1") == RunwayWidget::currentRunway1())
        rw1 = networkSettings()->value("RW1/runway1/*id", RunwayWidget::currentRunway1()).toString();
    if (networkSettings()->value("RW1/runway2") == RunwayWidget::currentRunway1())
        rw1 = networkSettings()->value("RW1/runway2/*id", RunwayWidget::currentRunway1()).toString();
    if (networkSettings()->value("RW2/runway1") == RunwayWidget::currentRunway2())
        rw2 = networkSettings()->value("RW2/runway1/*id", RunwayWidget::currentRunway2()).toString();
    if (networkSettings()->value("RW2/runway2") == RunwayWidget::currentRunway2())
        rw2 = networkSettings()->value("RW2/runway2/*id", RunwayWidget::currentRunway2()).toString();


    TAirportsMap::const_iterator itAirport = GetAirports().begin();
    for (; itAirport != GetAirports().end(); ++itAirport)
    {
        CGUIAirport* pAirport = dynamic_cast<CGUIAirport*> (itAirport->data());
        if (pAirport)
            pAirport->MarkItemActive(LayoutDlg::IsLayoutEnable(GetIDName(),
                LayoutDlg::Airport));
        CAirport::TSvgItems::ConstIterator itSvg = pAirport->GetSvgItems().begin();

        for (; itSvg != pAirport->GetSvgItems().end(); ++itSvg)
        {
            CGUISvgItem* pGUISvgItem = dynamic_cast<CGUISvgItem*> (itSvg.value().data());
            if (pGUISvgItem)
            {
                pGUISvgItem->MarkItemActive(LayoutDlg::IsSvgEnable(
                                                GetIDName(), (*itAirport)->GetID(), itSvg->data()->GetSvgItemID()));
            }
        }

        CAirport::TRunways::const_iterator itRunway =
                pAirport->GetRunways().begin();
        for (; itRunway != pAirport->GetRunways().end(); ++itRunway)
        {
            CGUIRunway* pRunway = dynamic_cast<CGUIRunway*> (itRunway->data());
            if (pRunway)
                pRunway->MarkItemActive(LayoutDlg::IsLayoutEnable(GetIDName(),
                    LayoutDlg::Runway));

            CRunway::TSSA::const_iterator itSSA = (*itRunway)->GetSSA().begin();
            for (; itSSA != (*itRunway)->GetSSA().end(); ++itSSA)
            {
                CGUISSA* pSSA = dynamic_cast<CGUISSA*> (itSSA->data());

                if (pSSA)
                    pSSA->MarkItemActive(pAirport->GetName() == airport
                                         && (pRunway->GetName() == rw1 || pRunway->GetName() == rw2)
                                         && LayoutDlg::IsSSAEnable(GetIDName(),
                                                                   (*itAirport)->GetID(),
                                                                   (*itRunway)->GetID(),
                                                                   (*itSSA)->GetID()));
                if (pSSA && pSSA->GetType() == CSSA::enSID)
                    pSSA->SetColor( UserProfile::CSettings::Instance().GetColor(UserProfile::CtBlastoff));
                if (pSSA && pSSA->GetType() == CSSA::enSTAR)
                    pSSA->SetColor( UserProfile::CSettings::Instance().GetColor(UserProfile::CtApproach));
            }
        }
    }
    SetSignActivePoint();
    update();
}

void CGUIScene::SetSignActivePoint()
{
    QString airport = networkSettings()->value("meteo/airport").toString();
    //QString runway;
    QString rw1,rw2;
    if (networkSettings()->value("RW1/runway1") == RunwayWidget::currentRunway1())
        rw1 = networkSettings()->value("RW1/runway1/*id", RunwayWidget::currentRunway1()).toString();
    if (networkSettings()->value("RW1/runway2") == RunwayWidget::currentRunway1())
        rw1 = networkSettings()->value("RW1/runway2/*id", RunwayWidget::currentRunway1()).toString();
    if (networkSettings()->value("RW2/runway1") == RunwayWidget::currentRunway2())
        rw2 = networkSettings()->value("RW2/runway1/*id", RunwayWidget::currentRunway2()).toString();
    if (networkSettings()->value("RW2/runway2") == RunwayWidget::currentRunway2())
        rw2 = networkSettings()->value("RW2/runway2/*id", RunwayWidget::currentRunway2()).toString();

    //проходим по всему списку точек
    TPointsMap::const_iterator itPoint = GetPoints().begin();
    for (; itPoint != GetPoints().end(); ++itPoint)
    {
        CGUIPoint* pPoint = dynamic_cast<CGUIPoint*> (itPoint->data());
        if (pPoint)
        {
            // если точка нужного типа 8, то проверяем ее на принадлежность хотя бя одной активной схеме
            if(pPoint->GetType() == CPoint::enSSAWaypoint)
            {
                bool signIsActive = false;
                TAirportsMap::const_iterator itAirport = GetAirports().begin();
                for (; itAirport != GetAirports().end(); ++itAirport)
                {
                    CGUIAirport* pAirport = dynamic_cast<CGUIAirport*> (itAirport->data());
                    if (pAirport)
                        pAirport->MarkItemActive(LayoutDlg::IsLayoutEnable(GetIDName(),
                            LayoutDlg::Airport));
                    CAirport::TSvgItems::ConstIterator itSvg = pAirport->GetSvgItems().begin();

                    for (; itSvg != pAirport->GetSvgItems().end(); ++itSvg)
                    {
                        CGUISvgItem* pGUISvgItem = dynamic_cast<CGUISvgItem*> (itSvg.value().data());
                        if (pGUISvgItem)
                        {
                            pGUISvgItem->MarkItemActive(LayoutDlg::IsSvgEnable(
                                                            GetIDName(), (*itAirport)->GetID(), itSvg->data()->GetSvgItemID()));
                        }
                    }

                    CAirport::TRunways::const_iterator itRunway =
                            pAirport->GetRunways().begin();
                    for (; itRunway != pAirport->GetRunways().end(); ++itRunway)
                    {
                        CGUIRunway* pRunway = dynamic_cast<CGUIRunway*> (itRunway->data());
                        if (pRunway)
                            pRunway->MarkItemActive(LayoutDlg::IsLayoutEnable(GetIDName(),
                                LayoutDlg::Runway));

                        CRunway::TSSA::const_iterator itSSA = (*itRunway)->GetSSA().begin();
                        for (; itSSA != (*itRunway)->GetSSA().end(); ++itSSA)
                        {
                            CGUISSA* pSSA = dynamic_cast<CGUISSA*> (itSSA->data());

                            if (pSSA)
                                pSSA->MarkItemActive(pAirport->GetName() == airport
                                                     && (pRunway->GetName() == rw1 || pRunway->GetName() == rw2)
                                                     && LayoutDlg::IsSSAEnable(GetIDName(),
                                                                               (*itAirport)->GetID(),
                                                                               (*itRunway)->GetID(),
                                                                               (*itSSA)->GetID()));
                            if (pSSA && pSSA->GetType() == CSSA::enSID)
                                pSSA->SetColor( UserProfile::CSettings::Instance().GetColor(UserProfile::CtBlastoff));
                            if (pSSA && pSSA->GetType() == CSSA::enSTAR)
                                pSSA->SetColor( UserProfile::CSettings::Instance().GetColor(UserProfile::CtApproach));

                            CSSA::TPoints::const_iterator itSSAPoint = (*itSSA)->GetPoints().begin();
                            for (; itSSAPoint != (*itSSA)->GetPoints().end(); ++itSSAPoint)
                            {
                                CSSAPoint* pSSAPoint = dynamic_cast<CSSAPoint*> (itSSAPoint->data());
                                if (pSSAPoint)
                                {
                                    if(pAirport->GetName() == airport
                                            && (pRunway->GetName() == rw1 || pRunway->GetName() == rw2)
                                            && LayoutDlg::IsSSAEnable(GetIDName(),
                                                                      (*itAirport)->GetID(),
                                                                      (*itRunway)->GetID(),
                                                                      (*itSSA)->GetID()))
                                    if(pPoint->GetName() == pSSAPoint->GetName())
                                    {
                                        signIsActive = true;
                                    }
                                }
                            }
                        }
                    }
                }
                pPoint->MarkItemActive(signIsActive);
                pPoint->SetSignActive(signIsActive);
            }
        }
    }
}

void CGUIScene::SetMapColor(const QColor& color)
{
    // Map changes points color
    typedef Scene::TPointsMap TPointsMap;
    TPointsMap& points = GetPoints();

    for (TPointsMap::iterator it = points.begin(); it != points.end(); ++it)
    {
        CGUIPoint* pPoint = dynamic_cast<CGUIPoint*> (it->data());
        if (pPoint)
            pPoint->SetColor(color);
    }

    // Map changes airports color
    typedef Scene::TAirportsMap TAirportsMap;
    TAirportsMap& airports = GetAirports();

    for (TAirportsMap::iterator it = airports.begin(); it != airports.end(); ++it)
    {
        CGUIAirport* pAirport = dynamic_cast<CGUIAirport*> (it->data());
        if (pAirport)
            pAirport->SetColor(color);
    }

    // Map changes holdings color
    typedef Scene::THoldingList THoldingList;
    THoldingList& holdings = GetHolding();

    for (THoldingList::iterator it = holdings.begin(); it != holdings.end(); ++it)
    {
        CGUIHolding* pHolding = dynamic_cast<CGUIHolding*> (it->data());
        if (pHolding)
            pHolding->SetColor(color);
    }
}

void CGUIScene::SetFIRColor(const QColor& color)
{
    brushColor = color;
}

void CGUIScene::SetBorderColor(const QColor& color)
{
    if (penColor == color)
        return;

    penColor = color;
    typedef Scene::TSceneList TSceneList;
    TSceneList& sectors = GetSectorList();

    for (TSceneList::iterator it = sectors.begin(); it != sectors.end(); ++it)
    {
        CGUISector* pSector = dynamic_cast<CGUISector*> (it->data());
        if (pSector)
            pSector->setBorderColor(color);
    }
}

void CGUIScene::SetSectorColor(const QColor& color)
{
    if (penColor == color)
        return;

    penColor = color;
    typedef Scene::TSceneList TSceneList;
    TSceneList& sectors = GetSectorList();

    for (TSceneList::iterator it = sectors.begin(); it != sectors.end(); ++it)
    {
        CGUISector* pSector = dynamic_cast<CGUISector*> (it->data());
        if (pSector)
            pSector->setActiveColor(color);
    }
}

void CGUIScene::SetRouteColor(const QColor& color)
{
    // Map changes routes color
    typedef Scene::TRoutsMap TRoutesMap;
    TRoutesMap& routes = GetRoutes();

    for (TRoutesMap::iterator it = routes.begin(); it != routes.end(); ++it)
    {
        CGUIRoute* pRoute = dynamic_cast<CGUIRoute*> (it->data());
        if (pRoute)
            pRoute->SetColor(color);
    }
}

void CGUIScene::SetHoldingColor(const QColor& color)
{
    // Map changes holding color
    typedef Scene::THoldingList THoldsList;
    THoldsList& holds = GetHolding();

    for (THoldsList::iterator it = holds.begin(); it != holds.end(); ++it)
    {
        CGUIHolding* pHold = dynamic_cast<CGUIHolding*> (it->data());
        if (pHold)
            pHold->SetColor(color);
    }
}

void CGUIScene::SetCorridorColor(const QColor& color)
{
    // Map changes Corridor color
    typedef Scene::TRoutsMap TRoutesMap;
    TRoutesMap& routes = GetRoutes();

    for (TRoutesMap::iterator it = routes.begin(); it != routes.end(); ++it)
    {
        CGUIRoute* pRoute = dynamic_cast<CGUIRoute*> (it->data());
        if (pRoute)
            pRoute->SetCorridorColor(color);
    }
}

void CGUIScene::OnNoteDelete(quint32 id)
{
    GetNotes().remove(id);
}

void CGUIScene::OnNoteEdit(quint32 id)
{
    ((XMasterWindow*)getMainWindow())->EditMapNote(id);
}

void CGUIScene::SetApproachColor(const QColor& color)
{
    // Map changes for STAR and APPROACH color (SSA)
    typedef Scene::TAirportsMap TAirportsMap;
    TAirportsMap& airports = GetAirports();

    for (TAirportsMap::iterator it = airports.begin(); it != airports.end(); ++it)
    {
        CGUIAirport* pAirport = dynamic_cast<CGUIAirport*> (it->data());
        if (pAirport)
        {
            // Map changes - runways color
            typedef CAirport::TRunways TRunways;
            TRunways& runways = pAirport->GetRunways();

            for (TRunways::iterator itRW = runways.begin(); itRW
                    != runways.end(); ++itRW)
            {
                CGUIRunway* pRunway = dynamic_cast<CGUIRunway*> (itRW->data());
                if (pRunway)
                    pRunway->SetColor(color);

                typedef CRunway::TSSA TSSA;
                TSSA& ssa = (*itRW)->GetSSA();

                for (TSSA::iterator itSSA = ssa.begin(); itSSA != ssa.end(); ++itSSA)
                {
                    CGUISSA* pSSA = dynamic_cast<CGUISSA*> (itSSA->data());
                    if (pSSA && pSSA->GetType() == CSSA::enSTAR)
                        pSSA->SetColor(color);
                }
            }
        }
    }
}

void CGUIScene::SetBlastoffColor(const QColor& color)
{
    // Map changes for SID
    typedef Scene::TAirportsMap TAirportsMap;
    TAirportsMap& airports = GetAirports();

    for (TAirportsMap::iterator it = airports.begin(); it != airports.end(); ++it)
    {
        CGUIAirport* pAirport = dynamic_cast<CGUIAirport*> (it->data());
        if (pAirport)
        {
            // Map changes - runways color
            typedef CAirport::TRunways TRunways;
            TRunways& runways = pAirport->GetRunways();

            for (TRunways::iterator itRW = runways.begin(); itRW
                    != runways.end(); ++itRW)
            {
                CGUIRunway* pRunway = dynamic_cast<CGUIRunway*> (itRW->data());
                if (pRunway)
                    pRunway->SetColor(color);

                typedef CRunway::TSSA TSSA;
                TSSA& ssa = (*itRW)->GetSSA();

                for (TSSA::iterator itSSA = ssa.begin(); itSSA != ssa.end(); ++itSSA)
                {
                    CGUISSA* pSSA = dynamic_cast<CGUISSA*> (itSSA->data());
                    if (pSSA && pSSA->GetType() == CSSA::enSID)
                        pSSA->SetColor(color);
                }
            }
        }
    }
}
