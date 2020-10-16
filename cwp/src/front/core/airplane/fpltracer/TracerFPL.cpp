#include "TracerFPL.h"
#include "../GUIScene.h"
#include <QMessageBox>
#include "common/Calculation.h"
#include "core/GUIAirplane.h"
#include "core/airplane/GUIAirplaneMTCDPoint.h"
#include "core/GUIView.h"
#include <QGraphicsScene>

#include "Master.h"

CTracerFPL::CTracerFPL(const QSharedPointer<CFPLFrag>& FPLFrag, const QSharedPointer<IScene> &pScene, const QColor &color)
    : QGraphicsItem(0)
    , m_FPLFrag(FPLFrag), m_pAirplane(0), m_pPlaneLine(0), m_color(color)
    , m_qllXMin(0), m_qllYMax(0), m_dScale(0), m_bPosChange(false), m_bReadOnly(false)
{
    m_pView = QWeakPointer<CView>(qSharedPointerDynamicCast<CView>(pScene));
    setZValue(gui::Ztracerfpl);
    CreateGUIFPL();
}

CTracerFPL::~CTracerFPL()
{
    DeleleTracer();
}

void CTracerFPL::DeleleTracer()
{
    if(scene())
    {
        TTracerPoints::const_iterator itPoint = m_listTracerPoints.constBegin();
        for(; itPoint != m_listTracerPoints.constEnd(); ++itPoint)
            scene()->removeItem(*itPoint);
        TTracerLines::const_iterator itLine = m_listTracerLines.constBegin();
        for(; itLine != m_listTracerLines.constEnd(); ++itLine)
            scene()->removeItem(*itLine);
        if(m_pPlaneLine)
            scene()->removeItem(m_pPlaneLine);
        foreach(CGUIAirplaneMTCDPoint* pMTCDpoint, m_mapMTCDPoints)
            scene()->removeItem(pMTCDpoint);
        scene()->removeItem(this);
    }
    TTracerPoints::const_iterator itPoint = m_listTracerPoints.constBegin();
    for(; itPoint != m_listTracerPoints.constEnd(); ++itPoint)
        delete *itPoint;
    TTracerLines::const_iterator itLine = m_listTracerLines.constBegin();
    for(; itLine != m_listTracerLines.constEnd(); ++itLine)
        delete *itLine;
    m_listTracerPoints.clear();
    m_listTracerLines.clear();
    if(m_pPlaneLine)
        delete m_pPlaneLine;
    m_pPlaneLine = 0;

    foreach(CGUIAirplaneMTCDPoint* pMTCDpoint, m_mapMTCDPoints)
        delete pMTCDpoint;
    m_mapMTCDPoints.clear();
}

void CTracerFPL::CreateGUIFPL()
{
    DeleleTracer();
    if(m_FPLFrag)
    {
        CFPLFrag::TProcFragList ProcList = m_FPLFrag->GetProctFragList();
        CFPLFrag::TProcFragList::const_iterator itProc = ProcList.constBegin();
        for(; itProc != ProcList.constEnd(); ++itProc)
        {
            CProcFrag::TPointFragList PointList = (*itProc)->GetPointFragList();
            CProcFrag::TPointFragList::const_iterator itPoint = PointList.constBegin();
            for(; itPoint != PointList.constEnd(); ++itPoint)
            {
                CTracerPoint::PointType typePoint = (*itProc)->GetType() == CProcFrag::enRoute ? CTracerPoint::FPL : CTracerPoint::FPL_SSA;
                CTracerPoint* pTracerPoint = new CTracerPoint(this, (*itPoint)->GetPoint()->GetName(), (*itPoint)->GetPoint()->GetCoord(),
                                                              typePoint, this);
                pTracerPoint->SetColor(m_color);
                pTracerPoint->SetReadOnly(m_bReadOnly);
                pTracerPoint->SetFPLProc(*itProc);
                pTracerPoint->SetFPLPoint(*itPoint);
                if(m_listTracerPoints.size())
                {
                    CTracerPoint* pPrevPoint = m_listTracerPoints.last();
                    CGUILineText* pLine(new CGUILineText(this, this));
                    pLine->SetColor(m_color);
                    pLine->SetPenWidth(1);
                    pLine->SetUseTextShape(false);
                    pLine->SetPoints(pPrevPoint->GetPointCoord(), pTracerPoint->GetPointCoord(), CGUILineText::NonePoints);
                    QVariant val;
                    val.setValue(pPrevPoint);
                    pLine->SetProperty("Point1", val);
                    val.setValue(pTracerPoint);
                    pLine->SetProperty("Point2", val);
                    m_listTracerLines.append(pLine);

                    val.setValue(pLine);
                    pPrevPoint->setProperty("Line2", val);
                    pTracerPoint->setProperty("Line1", val);
                }
                m_listTracerPoints.append(pTracerPoint);
            }
        }
    }
}


void CTracerFPL::UpdateTracer(CTracerPoint* pPoint)
{
    if (m_bPosChange)
        return;
    if (pPoint)
    {
        QVariant var = pPoint->property("Line1");
        CGUILineText* pLine1 = var.isNull() ? 0 : var.value<CGUILineText*>();
        var = pPoint->property("Line2");
        CGUILineText* pLine2 = var.isNull() ? 0 : var.value<CGUILineText*>();
        //CCoord pointCoord = GetCoordAtPoint(mapToScene(pPoint->pos()));
        if(pLine1)
        {
            //pLine1->SetPoint2(pointCoord, CGUILineText::NonePoints);
            pLine1->SetPoint2(pPoint->GetPointCoord(), CGUILineText::NonePoints);
        }
        if(pLine2)
        {
            //pLine2->SetPoint1(pointCoord, CGUILineText::NonePoints);
            pLine2->SetPoint1(pPoint->GetPointCoord(), CGUILineText::NonePoints);
        }
        if(m_pPlaneLine)
        {
            var = m_pPlaneLine->GetProperty("Point2");
            CTracerPoint* pPlanePoint = var.isNull() ? 0 : var.value<CTracerPoint*>();
            if(pPlanePoint == pPoint)
            {
                m_pPlaneLine->SetPoint2(pPoint->GetPointCoord(), CGUILineText::NonePoints);
            }
        }
    }
}

void CTracerFPL::SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord& centralCoord)
{
    m_qllXMin = qllXMin;
    m_qllYMax = qllYMax;
    m_dScale = dScale;
    m_CentralCoord = centralCoord;

    m_bPosChange = true;
    TTracerPoints::const_iterator itPoint = m_listTracerPoints.constBegin();
    for(; itPoint != m_listTracerPoints.constEnd(); ++itPoint)
        (*itPoint)->SetPosition(qllXMin, qllYMax, dScale, centralCoord);
    TTracerLines::const_iterator itLine = m_listTracerLines.constBegin();
    for(; itLine != m_listTracerLines.constEnd(); ++itLine)
        (*itLine)->SetPosition(qllXMin, qllYMax, dScale, centralCoord);
    if(m_pPlaneLine)
        m_pPlaneLine->SetPosition(qllXMin, qllYMax, dScale, centralCoord);
    m_bPosChange = false;

    foreach(CGUIAirplaneMTCDPoint* pMTCDpoint, m_mapMTCDPoints)
        pMTCDpoint->UpdatePosition(qllXMin, qllYMax, dScale, centralCoord);
}

QRectF CTracerFPL::boundingRect() const
{
    return QRectF();
}

QPainterPath CTracerFPL::shape() const
{
    return QPainterPath();
}

void CTracerFPL::paint(
        QPainter *painter,
        const QStyleOptionGraphicsItem *option,
        QWidget *widget)
{
}

bool CTracerFPL::getPlanePoint(const QSharedPointer<CPointFrag> &cop_point, const QString &cop_point_name, QString &point_name, QPointF &plane_point)
{
    if (cop_point || !cop_point_name.isEmpty()) {
        CCoord coord;
        if (cop_point) {
            coord = cop_point->GetPoint()->GetCoord();
            point_name = cop_point->GetPoint()->GetName();
        } else {
            QSharedPointer<CPoint> point = m_pAirplane->GetMaster()->findPointByName(cop_point_name);
            if (!point)
                return false;
            coord = point->GetCoord();
            point_name = cop_point_name;
        }

        plane_point.setX(((coord.GetX() + m_pAirplane->GetVelocity().GetVx()) - m_qllXMin) * m_dScale);
        plane_point.setY((m_qllYMax - (coord.GetY() + m_pAirplane->GetVelocity().GetVy())) * m_dScale);
        return true;
    }

    return false;
}

void CTracerFPL::UpdateTracerPlane(CGUIAirplane* pPlane)
{
    m_pAirplane = pPlane;
    TTracerPoints::const_iterator itPoint = m_listTracerPoints.constBegin();
    for (; itPoint != m_listTracerPoints.constEnd(); ++itPoint)
    {
        bool bVisible = !(*itPoint)->GetFPLPoint()->IsPassed();
        if (bVisible != (*itPoint)->isVisible())
            (*itPoint)->setVisible(bVisible);
    }
    TTracerLines::const_iterator itLine = m_listTracerLines.constBegin();
    for (; itLine != m_listTracerLines.constEnd(); ++itLine)
    {
        bool bVisible = false;
        QVariant var = (*itLine)->GetProperty("Point1");
        CTracerPoint* pPoint1 = var.isNull() ? 0 : var.value<CTracerPoint*>();
        var = (*itLine)->GetProperty("Point2");
        CTracerPoint* pPoint2 = var.isNull() ? 0 : var.value<CTracerPoint*>();
        if (pPoint1 && pPoint2)
            bVisible = pPoint1->isVisible() && pPoint2->isVisible();
        if (bVisible != (*itLine)->isVisible())
            (*itLine)->setVisible(bVisible);
    }
    if (m_pAirplane)
    {
        if (!m_pPlaneLine)
        {
            m_pPlaneLine = new CGUILineText(0, this);
            m_pPlaneLine->SetColor(m_color);
            m_pPlaneLine->SetPenWidth(1);
            m_pPlaneLine->SetUseTextShape(false);
        }
        CTracerPoint* pPoint2 = 0;
        CCoord coord2;
        if (m_FPLFrag)
        {
            if (m_FPLFrag->GetNextPoint() >= 0 && m_FPLFrag->GetNextPoint() < m_listTracerPoints.size())
            {
                pPoint2 = m_listTracerPoints.at(m_FPLFrag->GetNextPoint());
                coord2 = pPoint2->GetPointCoord();
            }
            else if (m_FPLFrag->GetNextCustomPoint())
            {
                coord2 = m_FPLFrag->GetNextCustomPoint()->GetCoord();
            }
        }

        if (!pPoint2 && coord2.IsNull())
        {
            QPointF PlanePoint1;
            PlanePoint1.setX((m_pAirplane->GetCurGUIPlaneCoord().GetX()-m_qllXMin)*m_dScale);
            PlanePoint1.setY((m_qllYMax-m_pAirplane->GetCurGUIPlaneCoord().GetY())*m_dScale);
            QPointF PlanePoint2;
            PlanePoint2.setX(((m_pAirplane->GetCurGUIPlaneCoord().GetX()+m_pAirplane->GetVelocity().GetVx())-m_qllXMin)*m_dScale);
            PlanePoint2.setY((m_qllYMax-(m_pAirplane->GetCurGUIPlaneCoord().GetY()+m_pAirplane->GetVelocity().GetVy()))*m_dScale);
            QLineF line(PlanePoint1, PlanePoint2);
            line.setLength(10000000*m_dScale); // 10000 km
            coord2 = GetCoordAtPoint(line.p2());
        }
        QVariant var;
        var.setValue(pPoint2);
        m_pPlaneLine->SetProperty("Point2", var);
        m_pPlaneLine->setPositionAndPoints(m_qllXMin, m_qllYMax, m_dScale, m_CentralCoord, m_pAirplane->GetCurGUIPlaneCoord(), coord2, CGUILineText::NonePoints);

        mtcd::TListMTCDPtr listMTCD = m_pAirplane->GetMTCD();
        if (listMTCD && listMTCD->size())
        {
            QMap<CCoord, bool> mapVisibility;
            for (mtcd::TListMTCD::const_iterator itList = listMTCD->constBegin(); itList != listMTCD->constEnd(); ++itList)
            {
                const CCoord coordRendezvous = (*itList)->coordRendezvous;
                if (!coordRendezvous.IsNull() && coordRendezvous.IsCenter())
                {
                    TMapMTCDPoints::iterator itPoint = m_mapMTCDPoints.find(coordRendezvous);
                    if (itPoint == m_mapMTCDPoints.end())
                    {
                        CGUIAirplaneMTCDPoint* pPoint = new CGUIAirplaneMTCDPoint(0);
                        pPoint->SetCoord(coordRendezvous);
                        itPoint = m_mapMTCDPoints.insert(coordRendezvous, pPoint);
                        if (scene())
                            scene()->addItem(pPoint);
                    }
                    (*itPoint)->SetListMTCDText(GetListMTCDText((*itList)->planeID1, (*itList)->planeID2), true);
                    (*itPoint)->UpdatePosition(m_qllXMin, m_qllYMax, m_dScale, m_CentralCoord);
                    (*itPoint)->setVisible(true);
                    mapVisibility.insert(coordRendezvous, true);
                }
            }
            for (TMapMTCDPoints::iterator itMap = m_mapMTCDPoints.begin(); itMap != m_mapMTCDPoints.end(); ++itMap)
            {
                if (!mapVisibility.contains(itMap.key()))
                    (*itMap)->setVisible(false);
            }
        }
        else
        {
            foreach (CGUIAirplaneMTCDPoint* pMTCDpoint, m_mapMTCDPoints)
            {
                if (scene())
                    scene()->removeItem(pMTCDpoint);
                delete pMTCDpoint;
            }
            m_mapMTCDPoints.clear();
        }
    }
    else
    {
        if (m_pPlaneLine)
        {
            if (scene())
                scene()->removeItem(m_pPlaneLine);
            delete m_pPlaneLine;
        }
        m_pPlaneLine = 0;

        foreach (CGUIAirplaneMTCDPoint* pMTCDpoint, m_mapMTCDPoints)
        {
            if (scene())
                scene()->removeItem(pMTCDpoint);
            delete pMTCDpoint;
        }
        m_mapMTCDPoints.clear();
    }
    RecalculateTracer();
}

void CTracerFPL::RecalculateTracer(quint32 seconds/* = 0*/)
{
    if (m_pAirplane) // && m_FPLFrag
    {
        QTime dt1(QTime::currentTime());
        dt1 = dt1.addSecs(seconds);
        QTime dt2(QTime::currentTime());
        dt2 = dt2.addSecs(seconds);
        if (m_pPlaneLine)
        {
            QString sText;
            QVariant var = m_pPlaneLine->GetProperty("Point2");
            CTracerPoint* pPlanePoint = var.isNull() ? 0 : var.value<CTracerPoint*>();
            if (pPlanePoint)
            {
                dt1 = dt1.addSecs(avia::GetTimeRoute(m_pAirplane->GetCurGUIPlaneCoord(), pPlanePoint->GetPointCoord(), m_pAirplane->GetGS()));
                sText = dt1.toString("hh:mm");
                if(m_pAirplane->GetIAS())
                {
                    dt2 = dt2.addSecs(avia::GetTimeRoute(m_pAirplane->GetCurGUIPlaneCoord(), pPlanePoint->GetPointCoord(), m_pAirplane->GetTAS()));
                    int delta = dt1.secsTo(dt2);
                    if(delta > 59)
                        sText += "(+" + QTime().addSecs(delta).toString("hh:mm") + ")";
                    if(delta < -59)
                        sText += "(-" + QTime().addSecs(qAbs(delta)).toString("hh:mm") + ")";
                }
            }
            m_pPlaneLine->SetText(sText, Qt::AlignRight);
        }
        TTracerPoints::const_iterator itPoint = m_listTracerPoints.constBegin();
        CCoord prevCoord;
        for (int i = 0; itPoint != m_listTracerPoints.constEnd(); ++itPoint)
        {
            if ((*itPoint)->isVisible())
            {
                CCoord coord = (*itPoint)->GetPointCoord();
                if (i == 0)
                {
                    prevCoord = coord;
                    ++i;
                }
                else
                {
                    QString sText;
                    dt1 = dt1.addSecs(avia::GetTimeRoute(prevCoord, coord, m_pAirplane->GetGS()));
                    sText = dt1.toString("hh:mm");
                    if(m_pAirplane->GetIAS())
                    {
                        dt2 = dt2.addSecs(avia::GetTimeRoute(prevCoord, coord, m_pAirplane->GetTAS()));
                        int delta = dt1.secsTo(dt2);
                        if(delta > 59)
                            sText += "(+" + QTime().addSecs(delta).toString("hh:mm") + ")";
                        if(delta < -59)
                            sText += "(-" + QTime().addSecs(qAbs(delta)).toString("hh:mm") + ")";
                    }
                    QVariant var = (*itPoint)->property("Line1");
                    CGUILineText* pLine = var.isNull() ? 0 : var.value<CGUILineText*>();
                    if(pLine)
                        pLine->SetText(sText, Qt::AlignRight);

                    prevCoord = coord;
                }
            }
        }
    }
}

void CTracerFPL::DoubleClickLineText(CGUILineText* pLineText, QPointF scenePos)
{
    if(m_bReadOnly) return;

    if(pLineText)
    {
        QVariant var = pLineText->GetProperty("Point1");
        CTracerPoint* pPoint1 = var.isNull() ? 0 : var.value<CTracerPoint*>();
        var = pLineText->GetProperty("Point2");
        CTracerPoint* pPoint2 = var.isNull() ? 0 : var.value<CTracerPoint*>();
        int numPoint = 0;
        if(pPoint1 && pPoint2)
        {
            if(pPoint1->GetType() == CTracerPoint::FPL)
            {
                numPoint = 1;
            }
            else
            {
                if(pPoint2->GetType() == CTracerPoint::FPL)
                {
                    numPoint = 2;
                }
                else
                    return;
            }
        }
        else
            return;
        const CCoord coord = GetCoordAtPoint(scenePos);
        QSharedPointer<CPoint> pRoutePoint;
        if(!IsCanAddPoint(coord.GetAsString(), coord, pRoutePoint))
            return;
        if(pRoutePoint && ((numPoint == 1) || (numPoint == 2)))
        {
            CTracerPoint* pNewPoint = new CTracerPoint(this, coord.GetAsString(), coord,
                                                       CTracerPoint::FPL, this);
            m_listTracerPoints.insert(m_listTracerPoints.indexOf(pPoint2), pNewPoint);
            pNewPoint->SetColor(m_color);
            pNewPoint->SetPosition(m_qllXMin, m_qllYMax, m_dScale, m_CentralCoord);
            QSharedPointer<CPointFrag> pCustRoutePoint(new CPointFrag);
            pCustRoutePoint->SetPoint(pRoutePoint);
            pNewPoint->SetFPLPoint(pCustRoutePoint);
            if(numPoint == 1)
            {
                int index = pPoint1->GetFPLProc()->GetSourcePointFragList().indexOf(pPoint1->GetFPLPoint());
                if(index != -1)
                {
                    pPoint1->GetFPLProc()->GetSourcePointFragList().insert(index+1, pCustRoutePoint);
                    pNewPoint->SetFPLProc(pPoint1->GetFPLProc());
                }
                else
                {
                    return;
                }
            }
            else if(numPoint == 2)
            {
                int index = pPoint2->GetFPLProc()->GetSourcePointFragList().indexOf(pPoint2->GetFPLPoint());
                if(index != -1)
                {
                    pPoint2->GetFPLProc()->GetSourcePointFragList().insert(index, pCustRoutePoint);
                    pNewPoint->SetFPLProc(pPoint2->GetFPLProc());
                }
                else
                {
                    return;
                }
            }
            else
                return;

            var.setValue(pNewPoint);
            pLineText->SetProperty("Point2", var);
            var.setValue(pLineText);
            pNewPoint->setProperty("Line1", var);
            pLineText->SetPoint2(pNewPoint->GetPointCoord(), CGUILineText::NonePoints);

            CGUILineText* pLine(new CGUILineText(this, this));
            pLine->SetColor(m_color);
            pLine->SetPenWidth(1);
            pLine->SetUseTextShape(false);
            pLine->SetPoints(pNewPoint->GetPointCoord(), pPoint2->GetPointCoord(), CGUILineText::NonePoints);
            pLine->SetPosition(m_qllXMin, m_qllYMax, m_dScale, m_CentralCoord);
            m_listTracerLines.append(pLine);

            var.setValue(pNewPoint);
            pLine->SetProperty("Point1", var);
            var.setValue(pPoint2);
            pLine->SetProperty("Point2", var);

            var.setValue(pLine);
            pPoint2->setProperty("Line1", var);
            pNewPoint->setProperty("Line2", var);

            UpdateTracer(pPoint1);
            UpdateTracer(pNewPoint);
            UpdateTracer(pPoint2);
            RecalculateTracer();
        }
    }
}

void CTracerFPL::DeleteTracerPoint(CTracerPoint* pPoint)
{
    if(m_bReadOnly) return;

    if(pPoint && pPoint->GetType() == CTracerPoint::FPL)
    {
        CTracerPoint* pPoint1 = 0;
        CTracerPoint* pPoint2 = 0;
        QVariant var = pPoint->property("Line1");
        CGUILineText* pLine1 = var.isNull() ? 0 : var.value<CGUILineText*>();
        var = pPoint->property("Line2");
        CGUILineText* pLine2 = var.isNull() ? 0 : var.value<CGUILineText*>();
        if(pLine1)
        {
            var = pLine1->GetProperty("Point1");
            pPoint1 = var.isNull() ? 0 : var.value<CTracerPoint*>();
        }
        if(pLine2)
        {
            var = pLine2->GetProperty("Point2");
            pPoint2 = var.isNull() ? 0 : var.value<CTracerPoint*>();
        }
        if(pLine1)
        {
            var.setValue(pPoint2);
            pLine1->SetProperty("Point2", var);
            if(pPoint2)
            {
                var.setValue(pLine1);
                pPoint2->setProperty("Line1", var);
            }
            else
            {
                // delete line1
                if(scene())
                    scene()->removeItem(pLine1);
                m_listTracerLines.removeOne(pLine1);
                delete pLine1;
                pLine1 = 0;
                if(pPoint1)
                {
                    var.setValue(pLine1);
                    pPoint1->setProperty("Line2", var);
                }
            }
        }
        if(pLine2)
        {
            if(scene())
                scene()->removeItem(pLine2);
            m_listTracerLines.removeOne(pLine2);
            delete pLine2;
            pLine2 = 0;
            if(pPoint2)
            {
                var.setValue(pLine1);
                pPoint2->setProperty("Line1", var);
            }
        }
        if(scene())
            scene()->removeItem(pPoint);
        m_listTracerPoints.removeOne(pPoint);
        const int index = pPoint->GetFPLProc()->GetSourcePointFragList().indexOf(pPoint->GetFPLPoint());
        if(index != -1)
            pPoint->GetFPLProc()->GetSourcePointFragList().remove(index);
        if(m_pPlaneLine)
        {
            var = m_pPlaneLine->GetProperty("Point2");
            CTracerPoint* pPlanePoint = var.isNull() ? 0 : var.value<CTracerPoint*>();
            if(pPlanePoint == pPoint)
            {
                var.setValue(pPoint2);
                m_pPlaneLine->SetProperty("Point2", var);
            }
        }
        delete pPoint;

        UpdateTracer(pPoint1);
        UpdateTracer(pPoint2);
        RecalculateTracer();
    }
}

bool CTracerFPL::IsCanAddPoint(const QString& sName, const CCoord& coord, QSharedPointer<CPoint>& pPoint)
{
    if(m_bReadOnly) return false;

    bool bRes = true;
    TTracerPoints::const_iterator itPoint = m_listTracerPoints.constBegin();
    for(; itPoint != m_listTracerPoints.constEnd(); ++itPoint)
    {
        QString sPointName;
        CCoord coordPoint;
        (*itPoint)->GetPointParam(sPointName, coordPoint);
        if((sPointName == sName) && (coordPoint == coord))
        {
            bRes = false;
            break;
        }
    }
    if(bRes && m_pView)
    {
        QSharedPointer<CView> pView = m_pView.toStrongRef();
        if(pView && pView->GetMainFIR())
        {
            bool bFind = false;
            CView::TSceneList::const_iterator itFIR = pView->GetScenes().constBegin();
            for(; itFIR != pView->GetScenes().constEnd(); ++itFIR)
            {
                Scene::TPointsMap::const_iterator itScenePoint = (*itFIR)->GetPoints().find(CPointKey(sName));
                if(itScenePoint != (*itFIR)->GetPoints().constEnd() && (*itScenePoint)->GetCoord() == coord)
                {
                    pPoint = *itScenePoint;
                    bFind = true;
                    break;
                }
            }
            if(!bFind)
            {
                // custom point
                QSharedPointer<CPoint> pCustPoint(new CPoint);
                pCustPoint->SetName(sName);
                pCustPoint->SetCoord(coord);
                pPoint = pCustPoint;
            }
        }
    }
    return bRes;
}

CCoord CTracerFPL::GetCoordAtPoint(const QPointF &pos) const
{
    if (m_pView) {
        CGUIView *pView = dynamic_cast<CGUIView*>(m_pView.data());
        if (pView)
            return pView->GetCoordAtPoint(pos);
    }
    return CCoord();
}

void CTracerFPL::SetReadOnly(bool bReadOnly/* = true*/)
{
    m_bReadOnly = bReadOnly;
    TTracerPoints::iterator itPoint = m_listTracerPoints.begin();
    for(; itPoint != m_listTracerPoints.end(); ++itPoint)
        (*itPoint)->SetReadOnly(bReadOnly);
}

void CTracerFPL::SetColor(const QColor &color)
{
    m_color = color;

    foreach (CTracerPoint *point, m_listTracerPoints)
    {
        point->SetColor(m_color);
        point->update();
    }

    foreach (CGUILineText *line, m_listTracerLines)
    {
        line->SetColor(m_color);
        line->update();
    }

    if (m_pPlaneLine)
    {
        m_pPlaneLine->SetColor(m_color);
        m_pPlaneLine->update();
    }
}

void CTracerFPL::SetCGUIAirplane(CGUIAirplane *pPlane)
{
    m_pAirplane = pPlane;
}

bool CTracerFPL::IsCanDeletePoint(CTracerPoint* pPoint)
{
    return !m_bReadOnly;
}

QStringList CTracerFPL::GetListMTCDText(int id1, int id2)
{
    QStringList listRes;
    if(m_pView)
    {
        QSharedPointer<CView> pView = m_pView.toStrongRef();
        if(pView)
        {
            for(int i = 0; i < 2; ++i)
            {
                int id = id1;
                if(i)
                    id = id2;
                CView::TAirplanesMap::const_iterator itPlane = pView->GetAirplanes().constFind(id);
                if(itPlane != pView->GetAirplanes().constEnd())
                {
                    //listRes.append((*itPlane)->GetMode3AReplyToString());
                    if((*itPlane)->GetSFPL())
                        listRes.append((*itPlane)->GetSFPL()->GetACID());
                }
            }
        }
    }
    return listRes;
}
