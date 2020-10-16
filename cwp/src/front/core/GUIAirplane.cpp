#include "GUIAirplane.h"

#include "back/common/Calculation.h"
#include "back/Master.h"
#include "back/core/RouteFragmentation.h"
#include "back/core/View.h"

#include "main.h"

#include <QPainter>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QGraphicsView>

#include "airplane/fpltracer/TracerFPL.h"

#include "AClist/ListAC.h"
#include "OLDIMessages.h"
#include "back/utils/ProfileSettings.h"
#include "obelix/CmdChangeSFPL.h"
#include "GUIView.h"
int CGUIAirplane::m_zValue = gui::Zactiveairplane;
const QPair<int,int> CGUIAirplane::RVSMLayer(QPair<int,int>(290,410));

CGUIAirplane::CGUIAirplane(QGraphicsItem * parent/* = 0*/)
    : CGUIObject(parent)
    , m_iPenWidth(1), m_color(Qt::white)
    , m_bLastActive(false)
    , m_Plane(this), m_Label(this),m_LabelExt(this)
    , m_highlightPlane(CHighlightPlane::None, 0)
    , m_fixedFormular(false)
    , m_formularLinkAngle(135)
    , m_formularLinkLength(100)
    , m_bSignShowRoute(false)
    , LabelExtFlag (false), was_set_ext(false), m_from_zoom_change(false)
{
    setZValue(gui::Zairplane);
    m_Plane.setPos(QPointF(0, 0));
    m_Label.setPos(QPointF(15, -90));

    SetColor(UserProfile::CSettings::Instance().GetColor(UserProfile::CtFormularPassive));
    m_Plane.SetColor(UserProfile::CSettings::Instance().GetColor(UserProfile::CtFormularPassive));

    connect(&m_Label, SIGNAL(lockMovingView()), SIGNAL(lockMovingView()));
    connect(&m_Label, SIGNAL(unlockMovingView()), SIGNAL(unlockMovingView()));
}

CGUIAirplane::~CGUIAirplane()
{
    if (m_pActDlg)
        m_pActDlg.reset();
    if (m_pTracer)
        m_pTracer.clear();
}

QRectF CGUIAirplane::boundingRect() const
{
    QRectF rect(m_painterPath.boundingRect());
    if (!m_painterPathEquipment.isEmpty())
        rect |= m_painterPathEquipment.boundingRect();
    //rect |= QRectF(-15,-15,30,30);
    rect.adjust(-1,-1,1,1);
    return rect;
}

QPainterPath CGUIAirplane::shape() const
{
    //    QPainterPath path(m_painterPath);
    //    path.addRect(QRectF(-15,-15,30,30));
    //    return path;

        return m_painterPath;
}

void CGUIAirplane::paint(
        QPainter *painter,
        const QStyleOptionGraphicsItem *option,
        QWidget *widget)
{
    if (!m_painterPath.isEmpty() || !m_painterPathEquipment.isEmpty())
    {
        QColor color = GetColor();
        if (isSTCA())
            color = Qt::red;

        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        QPen qPen(color);
        qPen.setWidth(m_iPenWidth);
        painter->setPen(qPen);
        painter->setBrush(color);
        painter->drawPath(m_painterPath);
        if (!m_painterPathEquipment.isEmpty())
        {
            painter->setPen(QPen(QBrush(color), 1, Qt::SolidLine, Qt::RoundCap));
            painter->setBrush(Qt::NoBrush);
            painter->drawPath(m_painterPathEquipment);
        }
        painter->restore();
    }
}

void CGUIAirplane::SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord& centralCoord)
{
    m_qllXMin = qllXMin;
    m_qllYMax = qllYMax;
    m_dScale = dScale;
    m_CentralCoord = centralCoord;

    if (!GetHistoryPoints().size())
        UpdatePosition();
    else
        RecalculateAll(true);
}

void CGUIAirplane::updateExtAfterSceneUpdated()
{
    /*if(m_from_zoom_change && LabelExtFlag && was_set_ext)
    {
        m_from_zoom_change = false;
        QGraphicsView * view = scene()->views().last();
    }*/
}

void CGUIAirplane::UpdatePosition()
{
    GetHistoryPoints().push(CHistoryPoint(GetCoord(), QDateTime::currentDateTime()));
    if (networkSettings()->value("simulator/server_type").toString() == "server")
        CheckTransfer();
    RecalculateAll();

    m_Plane.SetAirplane(this);
    m_Plane.SetPosition(mapFromParent(m_CenterPoint), GetCoord());
    m_Plane.UpdatePosition(m_qllXMin, m_qllYMax, m_dScale);

    m_Label.SetAirplane(this);
    m_Label.UpdatePosition(mapFromParent(m_CenterPoint));
    if(LabelExtFlag)
        m_LabelExt.UpdatePosition(mapFromParent(m_CenterPoint));
    if (!m_fixedFormular && m_Label.twitch())
        setFormularPos(m_formularLinkAngle, m_formularLinkLength);

    CheckPlaneAlt();
}

void CGUIAirplane::RecalculateAll(bool from_Zoom_change)
{
    if (!GetHistoryPoints().size())
        return;

    prepareGeometryChange();
    m_painterPath = QPainterPath();
    m_arrPoints.clear();
    QVectorIterator<CHistoryPoint> itCoords(GetHistoryPoints());
    itCoords.toBack();

    int numberOfPoints;
    if (!m_Plane.isVisible())
        numberOfPoints = 0;
    else
        numberOfPoints = networkSettings()->value("airplane/history_number_of_points", 5).toInt();

    for (int i = 0; (i <= numberOfPoints) && (itCoords.hasPrevious()); ++i)
    {
        const CCoord coord = itCoords.previous().coord;
        if (i == 0)
        {
            m_CenterPoint.setX((coord.GetX() - m_qllXMin) * m_dScale);
            m_CenterPoint.setY((m_qllYMax - coord.GetY()) * m_dScale);

            if(LabelExtFlag && m_LabelExt.isMoved())
            {
                m_LabelExt.SetMoved(false);
                was_set_ext = true;
            }
            if(from_Zoom_change && LabelExtFlag && was_set_ext)
            {
                /*m_from_zoom_change = true;
                QGraphicsView * view = scene()->views().last();
                QPointF scene_pos = mapToScene(0,0);
                //qDebug() << scene_pos.x() << scene_pos.y();
                QPoint viewport_pos = view->mapFromScene(scene_pos);
                QPoint screen_pos = view->viewport()->mapToParent(viewport_pos);
                screen_pos = view->mapToGlobal(screen_pos);
                qDebug() << screen_pos.x() << screen_pos.y();*/
            }
            QPointF old_pos = pos();
            setPos(m_CenterPoint);
            if(LabelExtFlag && was_set_ext && !from_Zoom_change)
            {
                //m_from_zoom_change = true;
                qreal dif_x = old_pos.x() - m_CenterPoint.x();
                qreal dif_y = old_pos.y() - m_CenterPoint.y();
                qDebug() << dif_x << " " << dif_y;
                m_LabelExt.moveBy(dif_x, dif_y);
            }
            if(!m_Label.twitch() && !from_Zoom_change)
            {
                qreal dif_x = old_pos.x()- m_CenterPoint.x();
                qreal dif_y = old_pos.y()- m_CenterPoint.y();
                m_Label.returnPosition(dif_x, dif_y);
                if(LabelExtFlag && !was_set_ext)
                    m_LabelExt.returnPosition(dif_x, dif_y);
            }
        }
        else
        {
            QPointF point;
            point.setX((coord.GetX() - m_qllXMin) * m_dScale);
            point.setY((m_qllYMax - coord.GetY()) * m_dScale);
            if (point == m_CenterPoint)
            {
                --i;
            }
            else
            {
                point = mapFromScene(point);
                if (m_arrPoints.size() && (point == m_arrPoints.last()))
                {
                    --i;
                }
                else
                {
                    m_painterPath.addEllipse(point, 2, 2);
                    m_arrPoints.append(point);
                }
            }
        }
    }
    m_painterPathEquipment = QPainterPath();
    //if (GetSFPL() && !GetSFPL()->isRVSM())
    if (GetSFPL() && GetSFPL()->isRVSM())
    {
        if ((GetFL() >= RVSMLayer.first && GetFL() <= RVSMLayer.second) ||
                (GetCFL() >= RVSMLayer.first && GetCFL() <= RVSMLayer.second))
        {
            //const double dRadius = 12500 * m_dScale; // diameter 25 km
            const double dRadius = 5000 * m_dScale; // diameter 10 km
            m_painterPathEquipment.addEllipse(QPointF(0,0), dRadius, dRadius);
        }
    }
    if (isShowPathPoints())
        ShowPathPoints();
    m_Plane.UpdatePosition(m_qllXMin, m_qllYMax, m_dScale);
    update();
}

void CGUIAirplane::SetActivePlane(bool bSetActive)
{
    emit setActivePlane(this);
    if (bSetActive)
    {
        ActivatePlane(false);
        SetLastActive(true);
        setZValue(++m_zValue);
        update();
    }
}

void CGUIAirplane::SetPlaneNewCoord(const CCoord &coord)
{
    QSharedPointer<CPoint> point(new CPoint);
    point.data()->SetCoord(coord);
    point.data()->SetName(coord.GetAsString());
    ChangeNextPoint(point);
    m_Label.SetPoint(&coord);
}

void CGUIAirplane::GetPlaneNewCoord()
{
    emit getCoord(this);
}

void CGUIAirplane::ShowPathPoints(bool bShow/* = true*/)
{
    if (bShow)
    {
        if (!m_pTracer)
        {
            QSharedPointer<CFPLFrag> FPLFrag;
            if (GetSFPL())
                FPLFrag = GetSFPL()->GetFPLFrag();

            QColor color = UserProfile::CSettings::Instance().GetColor(IsLastActive() ? UserProfile::CtFormularActive : UserProfile::CtFormularPassive);
            m_pTracer = QSharedPointer<CTracerFPL>(new CTracerFPL(FPLFrag, GetMaster()->GetView(), color));
            m_pTracer->SetReadOnly(true);
            if(scene())
                scene()->addItem(m_pTracer.data());
        }
        m_pTracer->SetPosition(m_qllXMin, m_qllYMax, m_dScale, m_CentralCoord);
        m_pTracer->UpdateTracerPlane(this);
        setSignShowRoute(true);
    }
    else
    {
        m_pTracer.clear();
        setSignShowRoute(false);
    }
}

bool CGUIAirplane::isShowPathPoints() const
{
    return m_pTracer ? m_pTracer->isVisible() : false;
}

bool CGUIAirplane::setSignShowRoute(bool sign)
{
    m_bSignShowRoute = sign;
}

bool CGUIAirplane::getSignShowRoute() const
{
    return m_bSignShowRoute;
}

void CGUIAirplane::SetBearing(bool bVal)
{
    bool bOnlyDelete = CAirplane::GetBearing() && bVal;
    CAirplane::SetBearing(bVal);
    if(CAirplane::GetBearing())
    {
        emit resetBearing();
        CAirplane::SetBearing(!bOnlyDelete);
        TVA_CATCHALL_TRY
        {
            if (GetSFPL() && GetMaster())
                dao()->SaveBearing(
                            GetOperName(),
                            GetSFPL()->GetACID(),
                            bOnlyDelete);
        } TVA_CATCHALL(err)
        {
            QMessageBox::critical(getMainWindow(), tr("Bearing"), err.reason());
        }
    }
    m_Plane.update();
}

quint16 CGUIAirplane::GetBearingAngle(CCoord BearingCoord)
{
    BearingCoord.Center(m_CentralCoord);
    const QPointF point1(
                (BearingCoord.GetX() - m_qllXMin) * m_dScale,
                (m_qllYMax - BearingCoord.GetY()) * m_dScale);
    const QPointF point2(
                (GetCoord().GetX() - m_qllXMin) * m_dScale,
                (m_qllYMax - GetCoord().GetY()) * m_dScale);
    const QLineF line(point1, point2);
    return int((360-line.angle())+90)%360;
}

int CGUIAirplane::GetAzimuth2() const
{
    return m_Label.GetAzimuth2();
}

void CGUIAirplane::SetAzimuth2(int degree)
{
    m_Label.SetAzimuth2(degree);
}

void CGUIAirplane::ClearAzimuth2()
{
    m_Label.ClearAzimuth2();
}

void CGUIAirplane::setNextPoint(const QSharedPointer<CPoint> &next_point)
{
    m_Label.setNextPoint(next_point);
}

void CGUIAirplane::ActivatePlane(bool bActive)
{
    if (bActive)
    {
        setZValue(++m_zValue);
        //Pasha emit setActivePlane(this);
    }
    m_Label.SetActivePlane(bActive);
    update();
}

void CGUIAirplane::SetColor(const QColor& color)
{
    m_Plane.SetColor(color);
    CGUIObject::SetColor(color);
}

void CGUIAirplane::ChangeSFPL()
{
    QTimer::singleShot(0, this, SLOT(OnChangeSFPL()));
}

void CGUIAirplane::OnChangeSFPL()
{
    m_pActDlg.reset(new CACTDlg (GetMaster(), GetMode3AReply(), GetSFPL() ? GetSFPL()->GetID() : 0, getMainWindow()));
    if (m_pActDlg->exec() == QDialog::Accepted)
    {
        QSharedPointer<CSFPL> pSFPL = m_pActDlg->GetSFPL();
        if (pSFPL)
        {
            QSharedPointer<obelix::CCmdChangeSFPL> pCmd(new obelix::CCmdChangeSFPL(obelix::CCmdChangeSFPL::Change));
            pCmd->SetValue(obelix::CCmdChangeSFPL::XML_AIRID, QString::number( GetID()));
            pCmd->SetValue(obelix::CCmdChangeSFPL::XML_CODE, QString::number(pSFPL->GetCODE()));
            pCmd->SetValue(obelix::CCmdChangeSFPL::XML_COPIN, pSFPL->GetCOPin());
            pCmd->SetValue(obelix::CCmdChangeSFPL::XML_ETOIN, pSFPL->GetETOin().toString());
            pCmd->SetValue(obelix::CCmdChangeSFPL::XML_COPINFL, pSFPL->GetCOPinFL().toString());
            pCmd->SetValue(obelix::CCmdChangeSFPL::XML_FPLID, QString::number(pSFPL->GetID()));
            GetMaster()->GetObClient().XMLSend(pCmd.staticCast<CXMLCmd>());
        }
    }
}

void CGUIAirplane::SetAutoCollapse(bool bCollapse)
{
    m_Label.showGrid(!bCollapse);
}

void CGUIAirplane::SetLastActive(const bool bActive)
{
    if (bActive != m_bLastActive)
    {
        m_bLastActive = bActive;
        if (m_bLastActive)
        {
            emit resetLastActive();
            m_bLastActive = bActive;
        }
        m_Label.update();
        m_Label.SetColor();
        m_Plane.update();
        master()->setFixedFormular(GetID(), fixedFormular());

        if (m_pTracer)
        {
            QColor color = UserProfile::CSettings::Instance().GetColor(IsLastActive() ? UserProfile::CtFormularActive : UserProfile::CtFormularPassive);
            m_pTracer->SetColor(color);
        }
    }
}

void CGUIAirplane::SendHighlight(const CHighlightPlane& val)
{
    emit setHighlight(val);
}

void CGUIAirplane::SetHighlight(const CHighlightPlane& val)
{
    switch (val.GetType())
    {
    case CHighlightPlane::CFL:
    {
        double dVal1 = val.GetValue1().toDouble() < val.GetValue2().toDouble()
                ? val.GetValue1().toDouble() : val.GetValue2().toDouble();
        double dVal2 = val.GetValue1().toDouble() < val.GetValue2().toDouble()
                ? val.GetValue2().toDouble() : val.GetValue1().toDouble();
        dVal1 -= 0.1;
        dVal2 += 0.1;
        if(((dVal1 < GetCFL()) && (dVal2 > GetCFL()))
                || ((dVal1 < GetFL()) && (dVal2 > GetFL())))
            m_highlightPlane = val;
        else
            m_highlightPlane = CHighlightPlane(CHighlightPlane::None, 0);
    }
        break;
    case CHighlightPlane::TFL:
        if(qAbs(val.GetValue1().toDouble() - GetTFL().GetF()) < 10.1)
            m_highlightPlane = val;
        else
            m_highlightPlane = CHighlightPlane(CHighlightPlane::None, 0);
        break;
    case CHighlightPlane::AFL:
        if(qAbs(val.GetValue1().toDouble() - GetFL()) < 10.1)
            m_highlightPlane = val;
        else
            m_highlightPlane = CHighlightPlane(CHighlightPlane::None, 0);
        break;
    default: m_highlightPlane = CHighlightPlane(CHighlightPlane::None, 0);
    }
    m_Plane.update();
}

void CGUIAirplane::CheckPlaneAlt()
{
    bool bShow = false;
    bool bShowPlains = true;
    if (GetFL() >= networkSettings()->value("airplane/low_FL", 0).toDouble() &&
            GetFL() <= networkSettings()->value("airplane/high_FL", 900).toDouble())
        bShow = true;
    if (isFlying())
        bShow = isOnAir() ? bShow : false;
    else
        bShow = isOnAir() ? false : true;
//    if(isVisible() != bShow)
//        setVisible(bShow);
    if (networkSettings()->value("users/hide_plains").toBool())
           bShowPlains = false;
    if (m_Label.isVisible() != bShow)
        m_Label.setVisible(bShow);
    if (!bShow && m_Plane.isVisible() != bShowPlains)
        m_Plane.setVisible(bShowPlains);
    if(bShow)
        m_Plane.setVisible(true);
}

CCoord CGUIAirplane::GetCurGUIPlaneCoord()
{
    CCoord coord;
    if(GetHistoryPoints().size())
        coord = GetHistoryPoints().last().coord;
    else
        coord = GetCoord();
    return coord;
}

void CGUIAirplane::SetTracerHDG()
{
    emit setTracerHDG(this);
}

void CGUIAirplane::setNextPointSearchMode()
{
    emit setNextPointSearchMode(this);
}

qint64 CGUIAirplane::Get_qllXMin()
{
    return m_qllXMin;
}

double CGUIAirplane::Get_dScale()
{
    return m_dScale;
}

CCoord CGUIAirplane::Get_CentralCoord()
{
    return m_CentralCoord;
}

QSharedPointer<CTracerFPL> CGUIAirplane::Get_pTracer()
{
    return m_pTracer;
}

qint64 CGUIAirplane::Get_qllYMax()
{
    return m_qllYMax;
}

void CGUIAirplane::afterDirectAssume()
{
    ShowPathPoints(true);
    SendHighlight(CHighlightPlane(CHighlightPlane::AFL, GetFL()));
    QTimer::singleShot(3000, this, SLOT(OnAfterDirectAssume()));
}

void CGUIAirplane::OnAfterDirectAssume()
{
    ShowPathPoints(false);
    SendHighlight(CHighlightPlane(CHighlightPlane::None, 0));
}

void CGUIAirplane::SetFormularColor(const QColor& color)
{
    SetColor(color);
    update();
    m_Plane.SetColor(color);
    m_Plane.update();

    if (m_pTracer)
        m_pTracer->SetColor(color);
}
void CGUIAirplane::UpdateFormularType()
{
    m_Label.UpdateFormularType();
}

void CGUIAirplane::UpdateFormularPos()
{
    QRectF r = m_Label.shape().boundingRect();
    QLineF line(QPointF(0, 0), m_Label.pos() + QPointF(r.width() / 2, r.height() / 2));

    m_formularLinkAngle = line.angle() - m_Plane.angle();
    m_formularLinkLength = line.length();
}

bool CGUIAirplane::fixedFormular() const
{
    return m_fixedFormular || IsLastActive() || !m_Label.twitch();
}

void CGUIAirplane::setFixedFormular(bool fixed)
{
    m_fixedFormular = fixed;
    master()->setFixedFormular(GetID(), fixedFormular());
}

void CGUIAirplane::setFormularPos(qreal angle, int length)
{
    if (m_Label.move())
        return;

    m_formularLinkAngle = angle;
    m_formularLinkLength = length;

    QLineF line(QPointF(0, 0), QPointF(0, length));
    line.setAngle(angle + m_Plane.angle());

    QRectF r = m_Label.shape().boundingRect();
    m_Label.setPos(line.p2() - QPointF(r.width() / 2, r.height() / 2));
}

void CGUIAirplane::formularBoundingRect(QPointF &point, QRectF &rect) const
{
    point = mapToScene(QPointF());
    rect = m_Label.mapRectToScene(m_Label.shape().boundingRect());
}

void CGUIAirplane::ShowFormularExt()
{
    QPointF pf = m_Label.GetTopRight();
    if(!LabelExtFlag)
    {
        m_LabelExt.setPos(pf);
        m_LabelExt.setVisible(true);
        m_LabelExt.SetAirplane(this);
        m_LabelExt.UpdatePosition(mapFromParent(m_CenterPoint));
        LabelExtFlag = true;
    }
}

bool isFixedLinkAngle(qreal in_angle)
{
    static const QVector<qreal> fixed_angles(QVector<qreal>() << 90. << 135. << 225. << 270.);
    return fixed_angles.contains(in_angle);
}

qreal nextFixedLinkAngle(qreal in_angle)
{
    static const QVector<qreal> fixed_angles(QVector<qreal>() << 90. << 135. << 225. << 270.);
    QVector<qreal>::const_iterator it = qFind(fixed_angles, in_angle);
    if (it == fixed_angles.constEnd())
        return *fixed_angles.constBegin();

    return (++it == fixed_angles.constEnd() ? *fixed_angles.constBegin() : *it);
}
