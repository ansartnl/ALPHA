#include "GUIAirplaneLabel.h"
#include "GUILabelText.h"
#include "GUIAirplane.h"
//#include "front/ground/GUIGroundAirplane.h"
#include "GUIAirplaneMTCDPoint.h"
#include "GUIConflict.h"

#include "utils/Coordinate.h"
#include "back/common/Calculation.h"
#include "back/Master.h"
#include "GUIReminder.h"
#include "Sectoring.h"
#include "Holding.h"

#include <QToolTip>

#include "conversionfunctions.h"

#include "main.h"

#if defined(_MSC_VER) && QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#define _USE_MATH_DEFINES
#endif
#include <math.h>

#include <QTimer>

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsGridLayout>
#include <QGraphicsLinearLayout>
#include <QMessageBox>
#include <QBitmap>

#include "obelix/CmdAirplanaCmd.h"
#include "XMasterWindow.h"
#include "utils/ProfileSettings.h"

const QString STR_NEXT_SECTOR = QObject::tr("IS");
const QString kSpecSignal = QObject::tr("spec signal");

EchelonValues CGUIAirplaneLabel::m_echelonValues = EchelonValues();

QSet<CGUIAirplaneLabel*> CGUIAirplaneLabel::liveObjects;

CGUIAirplaneLabel::CGUIAirplaneLabel(QGraphicsItem * parent/* = 0*/)
    : QGraphicsWidget(parent)
    , m_ConstructorFlag(false), m_iPenWidth(1), m_color(Qt::black), m_colorForeground(Qt::black), m_colorBackground(Qt::lightGray)
    , m_Transparent(25), m_bFullShow(false)//, m_bShowReminder(false)
    , m_bMoving(false), m_bActive(false), m_bPress(false), m_bPressGsp(false), m_bPressW(false), m_bMachLevel(false),m_bGSPorIAS(true), m_bNextPointIAS(true)
    , m_iAzimuth2(kInvalidAzimuth)
    , m_pAirplane(0), m_itemCODE(0), m_itemACID(0), m_itemCFL(0), m_itemVRC(0), m_itemHDG(0), m_itemHDG2(0)
    , item_spd_(0)
    , m_groundItemStatus(0), m_groundItemACID(0)
    , plane_type_(CAirplaneData::None), spec_signal_(false), spi_timer_(new QTimer(this)), emg_timer_(new QTimer(this))
    , move_(false), twitch_(true), NextPointText(""),np_type(NextPoint)
    , linear0(0), linear1(0), linear1_1(0), linear2(0), linear3(0), linear4(0), linear5(0)
{
    liveObjects.insert(this);

    m_bFullShow = false;// networkSettings()->value("airplane/auto_collapse", "yes").toString() != "yes";
    setZValue(gui::Zairplanelabel);
    setFlag(ItemDoesntPropagateOpacityToChildren, true);
    setAcceptedMouseButtons(Qt::LeftButton|Qt::RightButton|Qt::MiddleButton);
    //setOpacity(0.75);
    setAcceptHoverEvents(true);
    m_ActiveTimer = new QTimer(this);
    connect(m_ActiveTimer, SIGNAL(timeout()), this, SLOT(OnActivePlane()));

    spi_timer_->setInterval(10 * 1000);
    connect(spi_timer_, SIGNAL(timeout()), this, SLOT(onSpiTimeout()));

    emg_timer_->setInterval(8 * 1000);
    connect(emg_timer_, SIGNAL(timeout()), this, SLOT(onEmgTimeout()));

    //m_pReminder = new CGUIReminder(this);
    m_pConflict = new CGUIConflict(this);

    m_pMTCDpoint = new CGUIAirplaneMTCDPoint(this);
    m_pMTCDpoint->setVisible(false);

    m_echelonValues.init(QSqlDatabase::database());

}

CGUIAirplaneLabel::~CGUIAirplaneLabel()
{
    if(linear0)
        delete linear0;
    if(linear1)
        delete linear1;
    if(linear1_1)
        delete linear1_1;
    if(linear2)
        delete linear2;
    if(linear3)
        delete linear3;
    if(linear4)
        delete linear4;
    if(linear5)
        delete linear5;
    liveObjects.remove(this);
}

QRectF CGUIAirplaneLabel::boundingRect() const
{
    if(!m_pAirplane) return QRectF();
    QPainterPath paintPath;
    paintPath.addRect(m_clRect);
    if(!m_clRect.contains(mapFromParent(m_PlainParentPoint)))
    {
        paintPath.moveTo(mapFromParent(m_PlainParentPoint));
        paintPath.lineTo(intersectRect(QLineF(mapFromParent(m_PlainParentPoint), m_clRect.center()), m_clRect));
    }
    QRectF rect(paintPath.boundingRect());
    rect.adjust(-1,-1,1,1);
    return rect;
}

QPainterPath CGUIAirplaneLabel::shape() const
{
    if (!m_pAirplane)
        return QPainterPath();
    QPainterPath paintPath;
    paintPath.addRect(m_clRect);
    return paintPath;
}

void CGUIAirplaneLabel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (!m_pAirplane)
        return;

    painter->save();
    QColor color(m_color);
    if (m_pAirplane)
    {
        if(m_pAirplane->IsLastActive())
        {
            color = UserProfile::CSettings::Instance().GetColor( UserProfile::CtFormularActive);
        }
        else
        {
            color = UserProfile::CSettings::Instance().GetColor( UserProfile::CtFormularPassive);
        }
    }
    if (m_bActive)
        color = m_colorActive;
    QPen qPen(color);
    qPen.setWidth(m_iPenWidth);
    painter->setPen(qPen);
    if (!(globalSettings()->value("XMasterWindow/Settings/ShowFormularBorder", 1).toInt() & 1))
        painter->setPen(Qt::NoPen);
    QColor colorBackground(m_colorBackground);
    //int iVal = m_Transparent * 2.55;
    //iVal %= 256;
    //colorBackground.setAlpha(255 - iVal);
    painter->setBrush(QBrush(colorBackground));
    painter->drawRect(m_clRect);
    if (!m_clRect.contains(mapFromParent(m_PlainParentPoint)))
    {
        painter->setPen(qPen);
        painter->drawLine(mapFromParent(m_PlainParentPoint), intersectRect(QLineF(mapFromParent(m_PlainParentPoint), m_clRect.center()), m_clRect));
    }
    painter->restore();
}

void CGUIAirplaneLabel::UpdatePosition(QPointF point)
{
    m_PlainParentPoint = point;

    if (!m_itemCODE && m_pAirplane->isFlying())
        initGrid();
    if (!m_groundItemACID && !m_pAirplane->isFlying())
        initGrid();
    loadTypeGrid(m_bFullShow);
    updateGrid();
}

void CGUIAirplaneLabel::returnPosition(qreal dif_x, qreal dif_y)
{
    moveBy(dif_x, dif_y);
}

void CGUIAirplaneLabel::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!(event->buttons() & Qt::MiddleButton))
        return;

    const QPointF delta(event->pos() - drag_point_);

    if (master()->formularLinkLengthLimited())
    {
        QLineF line(mapFromParent(m_PlainParentPoint), m_clRect.center() + delta);
        qreal newLength = line.length();
        qreal length = QLineF(mapFromParent(m_PlainParentPoint), m_clRect.center()).length();

        if (length < master()->formularLinkMinLength() && newLength < length)
        {
            line.setLength(master()->formularLinkMinLength() - 1);
            setPos(mapToParent(line.p2() + pos() - mapToParent(m_clRect.center())));

            return;
        }

        if (length > master()->formularLinkMaxLength() && newLength > length)
        {
            line.setLength(master()->formularLinkMaxLength() + 1);
            setPos(mapToParent(line.p2() + pos() - mapToParent(m_clRect.center())));

            return;
        }
    }

    setPos(pos() + delta);
    m_pAirplane->setFixedFormular(true);
}

void CGUIAirplaneLabel::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    m_bMoving = false;
    if (m_pAirplane)
        m_pAirplane->SetActivePlane(true);

    QGraphicsWidget::mousePressEvent(event);

    drag_point_ = event->pos();

    if (event->button() == Qt::RightButton) {
        CGUILabelText *selected_field = fieldByPos(event->pos());
        if (selected_field == m_itemNextPoint || selected_field == m_Runway)
        {
            if(m_pAirplane->isOutAirport())
            {
                switch(np_type)
                {
                    case NextPoint:
                        np_type = PathPoints;
                        break;
                    case PathPoints:
                        np_type = Mhdg;
                        break;
                    case Mhdg:
                        np_type = NextPoint;
                        break;
                    default:
                    break;
                }

                const QSharedPointer<CSFPL> &sfpl = m_pAirplane->GetSFPL();
                if (sfpl) {
                QString cop = sfpl->GetCOPout();
                if (cop.isEmpty())
                    cop = sfpl->GetCOPin();
                switch(np_type)
                {
                    case NextPoint:
                    {
                        m_itemNextPoint->setText(cop.isEmpty() ? "XXXXX" : cop);
                        m_pAirplane->ShowPathPoints(false);
                        break;
                    }
                    case PathPoints:
                        m_pAirplane->ShowPathPoints(!m_pAirplane->isShowPathPoints());
                        break;
                    case Mhdg:
                    {
                        m_itemNextPoint->setText(QString::number(static_cast<int>(m_pAirplane->heading())));
                        m_pAirplane->ShowPathPoints(!m_pAirplane->isShowPathPoints());
                        break;
                    }
                    default:
                    break;
                }
                }
             }
            else
                m_pAirplane->ShowPathPoints(!m_pAirplane->isShowPathPoints());

            if (selected_field == m_itemNextPoint && m_itemNextPoint->CGUILabelText::text() == m_Mhdg->CGUILabelText::text())
            {
                m_bNextPointIAS = false;
                UpdateFormularType();
            }
            else
            {
                m_bNextPointIAS = true;
                UpdateFormularType();
            }
        }
        else if (selected_field == m_itemCFL)
            m_pAirplane->SendHighlight(CHighlightPlane(CHighlightPlane::CFL, m_pAirplane->GetCFL().GetF(), m_pAirplane->GetFL()));
        else if (selected_field == m_itemTFL)
            m_pAirplane->SendHighlight(CHighlightPlane(CHighlightPlane::TFL, m_pAirplane->GetTFL().GetF()));
        else if (selected_field == m_itemAFL)
            m_pAirplane->SendHighlight(CHighlightPlane(CHighlightPlane::AFL, m_pAirplane->GetFL()));
        else if (selected_field == m_itemHDG)
        {
            if (!isAssumed())
                return;
            const QSharedPointer<CSFPL> &sfpl = m_pAirplane->GetSFPL();
            if (sfpl)
            {
                const QString azimyth_str = "";
                sfpl->SetHdg(azimyth_str);

                if (master()->getRedisDB()) {
                    bool ok = master()->getRedisDB()->saveSFPL(sfpl);
                    if (!ok)
                        qCritical() << "error saving sfpl azimuth";
                }
            }
            m_itemHDG->setText(tr("hdg"));
        }
        else if (selected_field == m_itemVRC)
        {
            if (!isAssumed())
                return;
            const QSharedPointer<CSFPL> &sfpl = m_pAirplane->GetSFPL();
            if (sfpl)
            {
                const QString vrc_str = "";
                sfpl->SetVrc(vrc_str);

                if (master()->getRedisDB()) {
                    bool ok = master()->getRedisDB()->saveSFPL(sfpl);
                    if (!ok)
                        qCritical() << "error saving sfpl vrc";
                }
            }
            m_itemVRC->setText(tr("vrc"));
        }
        else if (selected_field == item_spd_)
        {
            if (!isAssumed())
                return;
            const QSharedPointer<CSFPL> &sfpl = m_pAirplane->GetSFPL();
            if (sfpl)
            {
                const QString spd_str = "";
                sfpl->SetSpd(spd_str);

                if (master()->getRedisDB()) {
                    bool ok = master()->getRedisDB()->saveSFPL(sfpl);
                    if (!ok)
                        qCritical() << "error saving sfpl spd";
                }
            }
            item_spd_->setText(tr("spd"));
        }
        else if (selected_field == m_itemACID)
                m_pAirplane->ShowFormularExt();

        else if (selected_field == item_gsp_){
            m_bGSPorIAS = false;
            UpdateFormularType();
        }
        else if (selected_field == item_w_){
            m_bGSPorIAS = true;
            UpdateFormularType();
        }
        } else if (event->button() == Qt::MiddleButton) {
        CGUILabelText *selected_field = fieldByPos(event->pos());
        if (selected_field == m_itemHDG)
            QToolTip::showText(event->screenPos(), QString("%1").arg(static_cast<int>(m_pAirplane->heading())));
        else if (selected_field == m_itemVRC)
            QToolTip::showText(event->screenPos(), QString("%1").arg(static_cast<int>(m_pAirplane->verticalRate())));
        else if (selected_field == item_spd_) {
            const QString ia = (m_pAirplane->indicatedAirspeed() ? QString::number(m_pAirplane->indicatedAirspeed()) : QString("-"));
            const QString iam = ((m_pAirplane->indicatedAirspeedMach() > 0.000001) ? QString::number(m_pAirplane->indicatedAirspeedMach(), 'f', 2) : QString("-"));
            QToolTip::showText(event->screenPos(), QString("%1/%2").arg(ia).arg(iam));
        } else if (selected_field == m_itemCFL)
            QToolTip::showText(event->screenPos(), QString("%1").arg(static_cast<int>(m_pAirplane->cfl())));
        else if (selected_field == m_itemACID)
            QToolTip::showText(event->screenPos(), m_pAirplane->GetACID());
        else if (selected_field == m_Runway)
        {
            const QSharedPointer<CSFPL> &sfpl = m_pAirplane->GetSFPL();
            if (sfpl) {
                    const QString rw = (!sfpl->GetSTAR().isEmpty() ? sfpl->GetSTAR() : QString("-"));
                    QToolTip::showText(event->screenPos(),rw);
            }
        }

        move_ = true;

        emit lockMovingView();
    }

    event->accept();
}

void CGUIAirplaneLabel::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsWidget::mouseReleaseEvent(event);

    if (event->button() == Qt::RightButton)
    {
        const QPointF point = event->pos();

        CGUILabelText *pItem = 0;
        QList<QGraphicsItem*> listWidgets = childItems();
        QList<QGraphicsItem*>::const_iterator itList = listWidgets.begin();
        for (; itList != listWidgets.end(); ++itList)
        {
            QRectF rect = (*itList)->boundingRect();
            rect = (*itList)->mapRectToParent(rect);
            if (rect.contains(point))
            {
                pItem = dynamic_cast<CGUILabelText*>(*itList);
                if (pItem) {
                    if (!pItem->GetVisible())
                        pItem = 0;
                    else
                        break;
                }
            }
        }

        /*if (pItem == m_itemNextPoint && !m_ConstructorFlag) {
            if (m_pAirplane)
                m_pAirplane->ShowPathPoints(!m_pAirplane->isShowPathPoints());
        }*/

        if (m_pAirplane)
            m_pAirplane->SendHighlight(CHighlightPlane(CHighlightPlane::None, 0));
    } else if (event->button() == Qt::MiddleButton) {
        move_ = false;
        m_pAirplane->UpdateFormularPos();

        emit unlockMovingView();
    } else if (event->button() == Qt::LeftButton) {
        QPointF point = event->pos();

        CGUILabelText* pItem = 0;
        QList<QGraphicsItem*> listWidgets = childItems();
        QList<QGraphicsItem*>::const_iterator itList = listWidgets.begin();
        for (; itList != listWidgets.end(); ++itList)
        {
            QRectF rect = (*itList)->boundingRect();
            rect = (*itList)->mapRectToParent(rect);
            if (rect.contains(point))
            {
                pItem = dynamic_cast<CGUILabelText*>(*itList);
                if (pItem)
                {
                    if(!pItem->GetVisible())
                        pItem = 0;
                    else
                        break;
                }
            }
        }

        if (pItem && !pItem->IsReadOnly())// && m_pAirplane->GetSFPL())
        {
            const CAirplaneData::PlaneType plane_type = m_pAirplane->GetPlaneType();

            QStringList sValues1, sValues2;
            QString sText;
            if (pItem == m_itemACID)
            {
                if (!isAssumed())
                    return;
                //sText = QString::number((unsigned long)m_pAirplane);
                //sValues1 << sText;
                m_pAirplane->showActIn();
                return;
            }
            else if (pItem == m_itemCODE)
            {
                if (!isAssumed())
                    return;
                sValues1 << QString::number(m_pAirplane->GetMode3AReplyInit(), 8).rightJustified(4, '0') << CAirplane::sListSpecCodes << tr("none");
                sText = pItem->text();
            }
            else if (pItem == m_itemCS)
            {
                const QSharedPointer<CSFPL> &sfpl = m_pAirplane->GetSFPL();
                if (CAirplaneData::ACT == plane_type) {
                    if (sfpl) {
                        const QString cur_sector = networkSettings()->value("users/current_sector").toString();
                        //if (cur_sector != sfpl->GetNextSector() || cur_sector.isEmpty() || cur_sector == tr("Custom"))
                        if(cur_sector.isEmpty() || cur_sector == tr("Custom") || (!sfpl->GetCurSector().isEmpty() && sfpl->GetNextSector().isEmpty()))
                            return;

                        pItem->SetTypeDlg(CGUIControlDlg::NSAssume);

                        if (!sfpl->GetNextSector().isEmpty() && cur_sector != sfpl->GetNextSector())
                        {
                            //const QString cur_sector = networkSettings()->value("users/current_sector").toString();
                            if (!cur_sector.isEmpty())
                            {
                                bool b1 = (cur_sector != sfpl->GetNextSector());
                                const QStringList controlled_sectors = networkSettings()->value("users/current_sectors_short_name").toString().split("-", QString::SkipEmptyParts);
                                bool b2 = controlled_sectors.contains(sfpl->GetNextSector());
                                if (b1 && !b2)
                                {
                                    pItem->SetTypeDlg(CGUIControlDlg::NSCalculate_sector);
                                     sValues1.append(sfpl->GetNextSector());
                                }
                            }
                        }

                    } else {
                        return;
                    }
                } else if (CAirplaneData::Transfered == plane_type) {
                    CGUIControlDlg::Type dlgtype = CGUIControlDlg::NSAssume;
                    if (sfpl && !sfpl->GetCurSector().isEmpty() && m_pAirplane->GetTransition().isEmpty())
                    {
                        if (networkSettings()->value("oldi/disable_ROF", "false").toString() == "true")
                            return;
                        dlgtype = CGUIControlDlg::NSROF;
                    }
                    pItem->SetTypeDlg(dlgtype);
                } else if (CAirplaneData::ActOut == plane_type) {
                    pItem->SetTypeDlg(CGUIControlDlg::NSTransfer_out);
                } else if (CAirplaneData::None == plane_type) {
                    const QString cur_sector = networkSettings()->value("users/current_sector").toString();
                    if (cur_sector.isEmpty() || cur_sector == tr("Custom"))
                        return;

                    //pItem->SetTypeDlg(CGUIControlDlg::NSAssume);
                    pItem->SetTypeDlg(CGUIControlDlg::NSDailyPlan);
                    pItem->SetACID(m_pAirplane->GetACID());
                    pItem->SetCODE(m_pAirplane->GetMode3AReplyToString());

                    int maxLevel = 0;
                    sText = QString::number(maxLevel);
                    sValues1.append(sText);
                    sValues2.append("DailyPlan");
                } else if (CAirplaneData::Transfer == plane_type) {
                      const QString cur_sector = networkSettings()->value("users/current_sector").toString();
                      if (cur_sector.isEmpty() || cur_sector == tr("Custom"))
                          return;

                      pItem->SetTypeDlg(CGUIControlDlg::NSAssume);
                } else {
                    if (!isAssumed())
                        return;

                    sValues1 = networkSettings()->value("users/transfer_sectors").toString().split("-", QString::SkipEmptyParts);

                    pItem->SetTypeDlg(CGUIControlDlg::NSTransfer);
                }

                sText = pItem->text();
            }
            else if (pItem == m_itemCFL)
            {
                if (!isAssumed())
                    return;
                const QSharedPointer<CAirplaneType> &airplane = m_pAirplane->GetSFPL()->GetAirplanePtr();
                if (airplane) {
                    int maxLevel = airplane->GetLevelMax();// meters
                    sText = QString::number(maxLevel);
                    sValues1.append(sText);
                } else {
                    sValues1.append("0");
                }
                sValues2.append("CFL");
//                sText = QString::null;
//                sText.append("%1");
//                sText = sText.arg(m_pAirplane->GetCFL().GetF());
                sText = m_pAirplane->GetCFL().toString();
            }
            else if (pItem == m_itemTFL)
            {
                if (!isAssumed())
                    return;
                const QSharedPointer<CAirplaneType> &airplane = m_pAirplane->GetSFPL()->GetAirplanePtr();
                if (airplane) {
                    int maxLevel = airplane->GetLevelMax();// meters
                    sText = QString::number(maxLevel);
                    sValues1.append(sText);
                } else {
                    sValues1.append("0");
                }
                sValues2.append("TFL");
//                sText = QString::null;
//                sText.append("%1");
//                sText = sText.arg(m_pAirplane->GetTFL().GetF());
                sText = PressOnAirdromeFromSee(m_pAirplane->GetTFL()).toString();
            }
            else if (pItem == m_itemVRC)
            {
                if (!isAssumed())
                    return;
                const QSharedPointer<CSFPL> &sfpl = m_pAirplane->GetSFPL();
                if (sfpl)
                    sText = sfpl->GetVrc();
                /*double maxVSpeed = 0;
                const QSharedPointer<CAirplaneType> &airplane = sfpl->GetAirplanePtr();
                if (airplane) {
                    bool isDescend = m_pAirplane->GetVz() < 0;
                    if (isDescend)
                        maxVSpeed = airplane->GetMaxDescend(m_pAirplane->GetFLinMeters());
                    else
                        maxVSpeed = airplane->GetMaxClimb(m_pAirplane->GetFLinMeters());
                }
                if (AppSettings::IsSISet(AppSettings::SIKnot))//feet
                {
                    int i = maxVSpeed * 60 / 0.3048 / 100;
                    if (i%5)
                    {
                        sText.sprintf("%02d", i);
                        sValues1 << sText;
                        i = i-(i%5);
                    }
                    for (; i >= 5; i -= 5)
                    {
                        sText.sprintf("%02d", i);
                        sValues1 << sText;
                    }
                }
                else //meters
                {
                    int i = maxVSpeed;
                    if (i%5)
                    {
                        sText.sprintf("%02d", i);
                        sValues1 << sText;
                        i = i-(i%5);
                    }
                    for (; i >= 5; i -= 5)
                    {
                        sText.sprintf("%02d", i);
                        sValues1 << sText;
                    }
                }
                int iVal = pItem->text().toInt();
                iVal = iVal-(iVal%5);
                sText.sprintf("%02d", iVal);*/
            } else if (pItem == item_spd_) {
                if (!isAssumed())
                    return;

                const QSharedPointer<CSFPL> &sfpl = m_pAirplane->GetSFPL();
                if (sfpl)
                {
                    sText = sfpl->GetSpd();
                    int afl = m_pAirplane->GetFL();//
                    sValues1.append(QString::number(afl));//
                }
            } else if (pItem == m_itemHDG || pItem == m_itemHDG2)
            {
                if (!isAssumed())
                    return;
                m_pAirplane->SetTracerHDG();
                return;
            }
            else if(pItem == m_itemROLL)
            {
                if (!m_pAirplane->IsOur())
                    return;
                int i = m_pAirplane->GetSFPL()->GetAirplanePtr()->GetBankMax();
                if (i%5)
                {
                    sText.sprintf("%02d", i);
                    sValues1 << sText;
                    i = i-(i%5);
                }
                for (; i >= 5; i -= 5)
                {
                    sText.sprintf("%02d", i);
                    sValues1 << sText;
                }
                int iVal = pItem->text().toInt();
                iVal = iVal-(iVal%5);
                sText.sprintf("%02d", iVal);
            }
            else if (pItem == m_itemNextPoint)
            {
                if (!isAssumed())
                    return;

                m_pAirplane->setNextPointSearchMode();
                return;
                /*m_mapRoutePoints.clear();
                m_mapHoldingZones.clear();
                if (m_pAirplane->GetSFPL() && m_pAirplane->GetSFPL()->GetFPLFrag())
                {
                    CProcFrag::TPointFragList pPointList = m_pAirplane->GetSFPL()->GetFPLFrag()->GetAllPoint();
                    for (int i = 0; i < pPointList.size(); ++i)
                        if (!pPointList[i]->IsGenerated())
                        {
                            m_mapRoutePoints.insert(sValues1.size(), i);
                            sValues1 << pPointList[i]->GetPoint()->GetName();
                        }
                }
                QList<QSharedPointer<CHolding> > listHolding;
                TVA_CATCHALL_TRY
                {
                    dao()->LoadHoldings(listHolding, m_pAirplane->GetSFPL()->GetFirName());
                } TVA_CATCHALL(err)
                {
                    QMessageBox::critical(getMainWindow(), tr("Holding"), err.reason());
                }
                QList<QSharedPointer<CHolding> >::const_iterator iterHold = listHolding.begin();
                for(; iterHold != listHolding.end(); ++iterHold)
                {
                    m_mapHoldingZones.insert(sValues2.size(), (*iterHold)->GetID());
                    sText.sprintf("%03.0f", (*iterHold)->GetInCourse());
                    sValues2 << (*iterHold)->GetPoint()->GetName()+"/"+sText;
                }
                sText = pItem->text();*/
            }
            else if(pItem == m_itemSPD1)
            {
                if(!m_pAirplane->IsOur())
                    return;

                int iMin = m_pAirplane->GetSFPL()->GetAirplanePtr()->GetMinIAS(m_pAirplane->GetFLinMeters()) * TransformM_SEC();
                int iMax = m_pAirplane->GetSFPL()->GetAirplanePtr()->GetMaxIAS(m_pAirplane->GetFLinMeters()) * TransformM_SEC();
                if(iMin%10) iMin = iMin+(10-(iMin%10));
                iMax = iMax-(iMax%10);
                for(int i = iMax; i >= iMin; i -= 10)
                {
                    sText.sprintf("%03d", i);
                    sValues1 << sText;
                }
                int iVal = pItem->text().toInt();
                iVal = iVal-(iVal%10);
                sText.sprintf("%03d", iVal);

                pItem->SetTypeDlg(CGUIControlDlg::Spd1Type);
            }
            else if(pItem == m_itemSPD2)
            {
                if(!m_pAirplane->IsOur()) return;
                if(m_pAirplane->GetFLinMeters() < m_pAirplane->GetMachLevel())
                {
                    m_bMachLevel = false;
                    int iMin = m_pAirplane->GetSFPL()->GetAirplanePtr()->GetMinIAS(m_pAirplane->GetFLinMeters()) * TransformM_SEC();
                    iMin *= avia::GetIASFactor(m_pAirplane->GetFLinMeters());
                    int iMax = m_pAirplane->GetSFPL()->GetAirplanePtr()->GetMaxIAS(m_pAirplane->GetFLinMeters()) * TransformM_SEC();
                    iMax *= avia::GetIASFactor(m_pAirplane->GetFLinMeters());
                    if(iMin%10) iMin = iMin+(10-(iMin%10));
                    iMax = iMax-(iMax%10);
                    for(int i = iMax; i >= iMin; i -= 10)
                    {
                        sText.sprintf("%03d", i);
                        sValues1 << sText;
                    }
                    int iVal = m_pAirplane->GetTAS() * TransformM_SEC();
                    iVal = iVal-(iVal%10);
                    sText.sprintf("%03d", iVal);
                }
                else
                {
                    m_bMachLevel = true;
                    double dMin = m_pAirplane->GetSFPL()->GetAirplanePtr()->GetMinIAS(m_pAirplane->GetFLinMeters());
                    dMin *= avia::GetIASFactor(m_pAirplane->GetFLinMeters());
                    dMin /= avia::GetSonicSpeed(m_pAirplane->GetFLinMeters());
                    double dMax = m_pAirplane->GetSFPL()->GetAirplanePtr()->GetMaxIAS(m_pAirplane->GetFLinMeters());
                    dMax *= avia::GetIASFactor(m_pAirplane->GetFLinMeters());
                    dMax /= avia::GetSonicSpeed(m_pAirplane->GetFLinMeters());
                    for (double d = dMax; d >= dMin; d -= 0.01)
                    {
                        sText.sprintf("%1.02f", d);
                        sValues1 << sText;
                    }
                    const double dVal = pItem->text().mid(1).toDouble();
                    sText = QString("%1").arg(dVal, 1, 'f', 2, QChar('0'));
                }
            }
            else if (pItem == m_groundItemStatus)
            {
                sValues1 << tr("Engine start") << tr("Pads removed") << tr("Holding point") << tr("Executive start");
            }
            pItem->SetValues(sValues1, sValues2, sText);
            pItem->OnChoose();
        }
    }
}

void CGUIAirplaneLabel::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsWidget::mouseDoubleClickEvent(event);

    /*if ((event->button() == Qt::LeftButton) && !m_bMoving && m_pAirplane->GetSFPL())
    {
        CGUILabelText *selected_field = fieldByPos(event->pos());
        if (selected_field == m_itemCS && !selected_field->IsReadOnly())
        {
            if (CAirplaneData::ACT == m_pAirplane->GetPlaneType() || CAirplaneData::Transfered == m_pAirplane->GetPlaneType())
            {
                //QTimer::singleShot(500, this, SLOT(OnTimerAssume()));
            }
        }
    }*/

   /* if (!m_bPress)
    {
        showGrid(true);
        //if (m_pAirplane)
        //m_pAirplane->SetActivePlane(false);
        m_bPress = true;
    }
    else
    {
        showGrid(false);
        m_bPress = false;
    }*/
    CGUILabelText *selected_field = fieldByPos(event->pos());
   /* switch (selected_field)
    {
        case item_gsp_:
            m_bPressGsp = m_bPressGsp ? false : true;
            break;
        case item_w_:
            m_bPressW = m_bPressW ? false : true;
            break;
    }*/
    if ((selected_field == item_gsp_) && (!m_bPressGsp))
        m_bPressGsp = true;
    else
    {
        if ((selected_field == item_gsp_) && (m_bPressGsp))
        m_bPressGsp = false;
    }
    if ((selected_field == item_w_) && (!m_bPressW))
        m_bPressW = true;
    else
    {
        if ((selected_field == item_w_) && (m_bPressW))
        m_bPressW = false;
    }
}

QVariant CGUIAirplaneLabel::itemChange(GraphicsItemChange change, const QVariant& value)
{
    if (change == QGraphicsItem::ItemPositionChange)
        m_bMoving = true;
    return QGraphicsWidget::itemChange(change, value);
}

void CGUIAirplaneLabel::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    showGrid(true);
    if (m_pAirplane)
        m_pAirplane->SetActivePlane(false);
    twitch_ = false;
    //emit lockMovingView();
    master()->setFixedFormular(m_pAirplane->GetID(), m_pAirplane->fixedFormular());

    QGraphicsWidget::hoverEnterEvent(event);

}

void CGUIAirplaneLabel::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if (!m_bPress)
        showGrid(false);
    twitch_ = true;
    //emit unlockMovingView();
    m_pAirplane->UpdateFormularPos();
    master()->setFixedFormular(m_pAirplane->GetID(), m_pAirplane->fixedFormular());

    QGraphicsWidget::hoverLeaveEvent(event);

}

void CGUIAirplaneLabel::initGrid()
{
    if (!m_pAirplane)
        return;

    if (m_pAirplane->isFlying())
    {
        m_itemACID = new CGUILabelText(this);
        m_itemACID->SetReadOnly(false);
        m_itemACID->SetTypeDlg(CGUIControlDlg::FPLType);
        m_itemACID->setText(tr("ACID"));
        connect(m_itemACID, SIGNAL(endEdit(const CGUILabelText*, const QStringList)),
                        this, SLOT(OnEndEdit(const CGUILabelText*, const QStringList)));

        m_itemCODE = new CGUILabelText(this);
        m_itemCODE->SetReadOnly(false);
        m_itemCODE->SetTypeDlg(CGUIControlDlg::ListWithEditType);
        m_itemCODE->setText(tr("CODE"));
        connect(m_itemCODE, SIGNAL(endEdit(const CGUILabelText*, const QStringList)),
                        this, SLOT(OnEndEdit(const CGUILabelText*, const QStringList)));

        m_itemAFL = new CGUILabelText(this);
        m_itemAFL->setText("AFL");

        m_itemTEND = new CGUILabelText(this);
        m_itemTEND->setText("^");

        m_itemHDG = new CGUILabelText(this);
        m_itemHDG->SetReadOnly(false);
        m_itemHDG->SetTypeDlg(CGUIControlDlg::RelativeType);
        m_itemHDG->setText("hdg");
        connect(m_itemHDG, SIGNAL(endEdit(const CGUILabelText*, const QStringList)),
                        this, SLOT(OnEndEdit(const CGUILabelText*, const QStringList)));

        m_itemHDG2 = new CGUILabelText(this);
        m_itemHDG2->SetReadOnly(false);
        m_itemHDG2->SetTypeDlg(CGUIControlDlg::RelativeType);
        m_itemHDG2->setText("hdg");
        connect(m_itemHDG2, SIGNAL(endEdit(const CGUILabelText*, const QStringList)),
                        this, SLOT(OnEndEdit(const CGUILabelText*, const QStringList)));

        m_itemROLL = new CGUILabelText(this);
        m_itemROLL->SetTypeDlg(CGUIControlDlg::ListType);
        m_itemROLL->setText("15");
        connect(m_itemROLL, SIGNAL( endEdit(const CGUILabelText*, const QStringList) ),
                        this, SLOT( OnEndEdit(const CGUILabelText*, const QStringList) ));

        item_spd_ = new CGUILabelText(this);
        item_spd_->SetReadOnly(false);
        item_spd_->SetTypeDlg(CGUIControlDlg::speedWidget);
        item_spd_->SetACID(m_pAirplane->GetACID());
        item_spd_->setText("spd");
        connect(item_spd_, SIGNAL(endEdit(const CGUILabelText*, const QStringList)),
                this, SLOT(OnEndEdit(const CGUILabelText*, const QStringList)));
        connect(item_spd_, SIGNAL(dialogOpened()), this, SLOT(onDialogOpened()));
        connect(item_spd_, SIGNAL(dialogClosed()), this, SLOT(onDialogClosed()));

        m_itemSPD1 = new CGUILabelText(this);
        m_itemSPD1->SetReadOnly(false);
        m_itemSPD1->SetTypeDlg(CGUIControlDlg::ListType);
        m_itemSPD1->setText("SPD1");
        connect(m_itemSPD1, SIGNAL( endEdit(const CGUILabelText*, const QStringList) ),
                        this, SLOT( OnEndEdit(const CGUILabelText*, const QStringList) ));

        m_itemSPD2 = new CGUILabelText(this);
        m_itemSPD2->SetReadOnly(false);
        m_itemSPD2->SetTypeDlg(CGUIControlDlg::ListType);
        m_itemSPD2->setText("SPD2");
        connect(m_itemSPD2, SIGNAL( endEdit(const CGUILabelText*, const QStringList) ),
                        this, SLOT( OnEndEdit(const CGUILabelText*, const QStringList) ));

        m_itemLX = new CGUILabelText(this);
        m_itemLX->setText(tr("LX"));

        m_itemCS = new CGUILabelText(this);
        m_itemCS->SetReadOnly(false);
        m_itemCS->SetTypeDlg(CGUIControlDlg::ListType);
        m_itemCS->setText(STR_NEXT_SECTOR);
        connect(m_itemCS, SIGNAL(endEdit(const CGUILabelText*, const QStringList)),
                        this, SLOT(OnEndEdit(const CGUILabelText*, const QStringList))
                , Qt::QueuedConnection);

#ifdef QT_DEBUG
        item_cs_debug_ = new CGUILabelText(this);
        item_cs_debug_->SetReadOnly(true);
        item_cs_debug_->setText("cs");

        item_ns_debug_ = new CGUILabelText(this);
        item_ns_debug_->SetReadOnly(true);
        item_ns_debug_->setText("ns");
#endif

        m_itemCFL = new CGUILabelText(this);
        m_itemCFL->SetReadOnly(false);
        m_itemCFL->SetTypeDlg(CGUIControlDlg::EchelonsWidgetType);
        m_itemCFL->SetACID(m_pAirplane->GetACID());
        m_itemCFL->setText("000");
        connect(m_itemCFL, SIGNAL( endEdit(const CGUILabelText*, const QStringList) ),
                        this, SLOT( OnEndEdit(const CGUILabelText*, const QStringList)));
        connect(m_itemCFL, SIGNAL(dialogOpened()), this, SLOT(onDialogOpened()));
        connect(m_itemCFL, SIGNAL(dialogClosed()), this, SLOT(onDialogClosed()));

        m_itemTFL = new CGUILabelText(this);
        m_itemTFL->SetReadOnly(false);
        m_itemTFL->SetTypeDlg(CGUIControlDlg::EchelonsWidgetType);
        m_itemTFL->SetACID(m_pAirplane->GetACID());
        m_itemTFL->setText("000");
        connect(m_itemTFL, SIGNAL( endEdit(const CGUILabelText*, const QStringList) ),
                        this, SLOT( OnEndEdit(const CGUILabelText*, const QStringList)));
        connect(m_itemTFL, SIGNAL(dialogOpened()), this, SLOT(onDialogOpened()));
        connect(m_itemTFL, SIGNAL(dialogClosed()), this, SLOT(onDialogClosed()));

        m_itemVRC = new CGUILabelText(this);
        m_itemVRC->SetReadOnly(false);
        //m_itemVRC->SetTypeDlg(CGUIControlDlg::ListType);
        m_itemVRC->SetTypeDlg(CGUIControlDlg::verticalSpeedWidget);
        m_itemVRC->setText(tr("vrc"));
        connect(m_itemVRC, SIGNAL( endEdit(const CGUILabelText*, const QStringList) ),
                        this, SLOT( OnEndEdit(const CGUILabelText*, const QStringList)));
        connect(m_itemVRC, SIGNAL(dialogOpened()), this, SLOT(onDialogOpened()));
        connect(m_itemVRC, SIGNAL(dialogClosed()), this, SLOT(onDialogClosed()));

        m_itemNextPoint = new CGUILabelText(this);
        m_itemNextPoint->SetReadOnly(false);
        m_itemNextPoint->SetTypeDlg(CGUIControlDlg::ListPointType);
        m_itemNextPoint->setText(tr("XXXXX")); //m_itemNextPoint->setText(tr("Point"));
        connect(m_itemNextPoint, SIGNAL(endEdit(const CGUILabelText*, const QStringList)),
                        this, SLOT(OnEndEdit(const CGUILabelText*, const QStringList)));

        m_itemV = new CGUILabelText(this);
        m_itemV->setText(tr("V"));

        item_gsp_ = new CGUILabelText(this);
        item_gsp_->setText(tr("GSP"));

        item_w_ = new CGUILabelText(this);
        item_w_->setText(tr("W"));

        m_itemUVD = new CGUILabelText(this);
        m_itemUVD->setText(tr("UVD"));

        m_itemPEL = new CGUILabelText(this);
        m_itemPEL->setText(tr("PEL"));

        m_itemRFL = new CGUILabelText(this);
        m_itemRFL->setText(tr("RFL"));

        m_itemAzimuth = new CGUILabelText(this);
        m_itemAzimuth->setText(m_AzimuthAndDistance.GetAzimuthText(m_pAirplane));

        m_itemDistance = new CGUILabelText(this);
        m_itemDistance->setText(m_AzimuthAndDistance.GetDistanceText(m_pAirplane));

        m_itemQNH = new CGUILabelText(this);
        m_itemQNH->setText("QNH");

        item_spec_signal_ = new CGUILabelText(this);
        item_spec_signal_->setText(kSpecSignal);
        item_spec_signal_->setBoldFont(true);

        item_assume_ = new CGUILabelText(this);
        item_assume_->setText(tr("ASSUME"));

        item_transfer_ = new CGUILabelText(this);
        item_transfer_->setText(tr("TRANSFER"));

        item_act_out_ = new CGUILabelText(this);
        item_act_out_->setText(tr("ACT OUT"));

        m_itemL = new CGUILabelText(this);
        m_itemL->setText(tr(""));

        m_itemM = new CGUILabelText(this);
        m_itemM->setText(tr(""));

        m_itemLM = new CGUILabelText(this);
        m_itemLM->setText(tr(""));

        m_Mhdg = new CGUILabelText(this);
        m_Mhdg->setText(tr("Mhdg"));

        m_Runway = new CGUILabelText(this);
        m_Runway->setText(tr(""));

        m_itemType = new CGUILabelText(this);
        m_itemType->SetReadOnly(true);
        m_itemType->setText(tr("Type"));
    }
    else
    {
        //Ground Items
        m_groundItemStatus = new CGUILabelText(this);
        m_groundItemStatus->SetTypeDlg(CGUIControlDlg::ListType);
        m_groundItemStatus->SetReadOnly(false);
        m_groundItemStatus->setText(tr("<None>"));
        connect(m_groundItemStatus, SIGNAL( endEdit(const CGUILabelText*, const QStringList) ),
                        this, SLOT( OnEndEdit(const CGUILabelText*, const QStringList) ));

        m_groundItemACID = new CGUILabelText(this);
        m_groundItemACID->setText(QL1S("ACID"));

        m_groundItemSQUAWK = new CGUILabelText(this);
        //m_groundItemSQUAWK->SetFocusable(true);
        m_groundItemSQUAWK->setText(QL1S("SQUAWK"));

        m_groundItemSpeed = new CGUILabelText(this);
        m_groundItemSpeed->SetReadOnly(false);
        //m_groundItemSpeed->SetTypeDlg(CGUIControlDlg::);
        m_groundItemSpeed->setText(QL1S("Speed"));
    //    connect(m_groundItemSpeed, SIGNAL( endEdit(const CGUILabelText*, const QStringList) ),
    //                    this, SLOT( OnEndEdit(const CGUILabelText*, const QStringList) ));

        m_groundItemSpeedLabel = new CGUILabelText(this);
        m_groundItemSpeedLabel->setText(tr("Speed, km/h: "));
    }

    grid = new QGraphicsLinearLayout(Qt::Orientation::Vertical, this);
    setLayout(grid);
    linear0 = new QGraphicsLinearLayout(Qt::Orientation::Horizontal, grid);
    linear1 = new QGraphicsLinearLayout(Qt::Orientation::Horizontal, grid);
    linear1_1 = new QGraphicsLinearLayout(Qt::Orientation::Horizontal, grid);
    linear2 = new QGraphicsLinearLayout(Qt::Orientation::Horizontal, grid);
    linear3 = new QGraphicsLinearLayout(Qt::Orientation::Horizontal, grid);
    linear4 = new QGraphicsLinearLayout(Qt::Orientation::Horizontal, grid);
    linear5 = new QGraphicsLinearLayout(Qt::Orientation::Horizontal, grid);
    grid->insertItem(0, linear0);
    grid->setItemSpacing(1, 1);
    grid->insertItem(1, linear1);
    grid->insertItem(2, linear1_1);
    grid->insertItem(3, linear2);
    grid->insertItem(4, linear3);
    grid->insertItem(5, linear4);
    grid->insertItem(6, linear5);
    /*QGraphicsGridLayout *grid = new QGraphicsGridLayout(this);
    grid->setHorizontalSpacing(5);
    grid->setVerticalSpacing(1);*/
    grid->setContentsMargins(2,2,2,2);
}

void CGUIAirplaneLabel::loadTypeGrid(bool bFullShowVal)
{
    grid = dynamic_cast<QGraphicsLinearLayout*>(layout());
    if (!grid)
        return;

    linear0 = dynamic_cast<QGraphicsLinearLayout*>(grid->itemAt(0));
    linear1 = dynamic_cast<QGraphicsLinearLayout*>(grid->itemAt(1));
    linear1_1 = dynamic_cast<QGraphicsLinearLayout*>(grid->itemAt(2));
    linear2 = dynamic_cast<QGraphicsLinearLayout*>(grid->itemAt(3));
    linear3 = dynamic_cast<QGraphicsLinearLayout*>(grid->itemAt(4));
    linear4 = dynamic_cast<QGraphicsLinearLayout*>(grid->itemAt(5));
    linear5 = dynamic_cast<QGraphicsLinearLayout*>(grid->itemAt(6));

    //QGraphicsLinearLayout *linear3 = dynamic_cast<QGraphicsLinearLayout*>(grid->itemAt(3));

    /*QGraphicsGridLayout *grid = dynamic_cast<QGraphicsGridLayout*>(layout());
    if (!grid)
        return;*/

    XGridLabel::LabelType label_type = XGridLabel::None;
    bool selected = bFullShowVal;
    //qDebug() << "Selected:" << selected;
    if (m_pAirplane && m_pAirplane->isFlying())
    {
        if (!m_ConstructorFlag)
        {
            label_type = selected ? XGridLabel::Unrelated : XGridLabel::UnrelatedShort;

            const CAirplaneData::PlaneType plane_type = m_pAirplane->GetPlaneType();

            switch (plane_type)
            {
            case CAirplaneData::None:
                if (m_pAirplane->IsInOurSector())
                    label_type = selected ? XGridLabel::RelatedSelected : XGridLabel::Related;
                break;
            case CAirplaneData::ACT:
                {
                    if (m_pAirplane->IsInOurSector()) {
                        label_type = selected ? XGridLabel::RelatedSelected : XGridLabel::Related;
                    } /*else*/ {
                        const QString cur_sector = networkSettings()->value("users/current_sector").toString();
                        if (!cur_sector.isEmpty()) {
                            const QSharedPointer<CSFPL> &sfpl = m_pAirplane->GetSFPL();
                            if (sfpl) {
                                bool b1 = (cur_sector == sfpl->GetNextSector());
                                const QStringList controlled_sectors = networkSettings()->value("users/current_sectors_short_name").toString().split("-", QString::SkipEmptyParts);
                                //bool b2 = controlled_sectors.contains(sfpl->GetInSector());
                                bool b2 = controlled_sectors.contains(sfpl->GetNextSector());
                                if (b1 || b2)
                                    label_type = selected ? XGridLabel::InSelected : XGridLabel::In;
                            }
                        }
                    }
                }
                break;
            case CAirplaneData::ASSUME:
            case CAirplaneData::Transfer:
            case CAirplaneData::ActOutInFir:
            case CAirplaneData::ActOut:
                label_type = selected ? XGridLabel::ControlledSelected : XGridLabel::Controlled;
                break;
            case CAirplaneData::Transfered:
                if (m_pAirplane->IsInOurSector())
                    label_type = selected ? XGridLabel::RelatedSelected : XGridLabel::Related;
                break;
            }
            //setLabelColors(label_type);
            bool spec_signal = isSpecSignalExist();
            if (label_type != GetGridLabel().GetType() || m_pAirplane->isSTCA()) {
                XGridLabel::TMapGrid mapGrid;
                XGridLabel::Load(mapGrid);
                SetGridLabel(mapGrid[label_type]);
            } else if (plane_type_ == plane_type) {
                if (spec_signal == spec_signal_)
                {
                    setLabelColors(label_type);
                    fillLabelFields(grid, label_type);
                    processFlightLevels(label_type);
                    return;
                }
            }

            spec_signal_ = spec_signal;
            plane_type_ = plane_type;
        }
    }

    XGridLabel& GridLabel = GetGridLabel();
    m_colorForeground = GridLabel.GetForeground();
    m_colorBackground = GridLabel.GetBackground();
    m_Transparent = GridLabel.GetTransparent();
    //UserProfile::CSettings& stg = UserProfile::CSettings::Instance();
    //qDebug() << stg.GetColor(UserProfile::CtRelated).name() << ", " << stg.GetColor(UserProfile::CtRelated_back).name();
    //qDebug() << m_colorForeground.name() << ", " << m_colorBackground.name();
    for (int i = grid->count()-1; i >= 0; --i)
        grid->removeAt(i);

    if (m_pAirplane && !m_pAirplane->isFlying())
    {
        /*grid->addItem(m_groundItemStatus,      0,0,1,3, Qt::AlignHCenter);
        grid->addItem(m_groundItemACID,        1,0,1,1, Qt::AlignLeft);
        grid->addItem(m_groundItemSQUAWK,      1,2,1,1, Qt::AlignRight);
        grid->addItem(m_groundItemSpeedLabel,  2,0,1,2, Qt::AlignLeft);
        grid->addItem(m_groundItemSpeed,       2,2,1,1, Qt::AlignRight);*/

        //grid->setHorizontalSpacing(5);
        linear0->addItem(m_groundItemStatus);

        linear1->addItem(m_groundItemACID);
        linear1->addItem(m_groundItemSQUAWK);

        linear2->addItem(m_groundItemSpeedLabel);
        linear2->addItem(m_groundItemSpeed);

        for (int i = 0; i < grid->count(); ++i)
            dynamic_cast<CGUILabelText*>(grid->itemAt(i))->setVisible(true);
    }
    else
    {
        const XGridLabel::TMapItem &labelMap = GridLabel.GetMapItem();
        if (labelMap.size())
        {
            if (labelMap.contains(XGridItem::ACID))
            {
                m_itemACID->SetVisible(true);
                m_itemACID->setVisible(true);
                const XGridItem& itemPos = labelMap[XGridItem::ACID];
                //grid->addItem(m_itemACID, itemPos.GetRow(), itemPos.GetColumn(), itemPos.GetRowSpan(), itemPos.GetColumnSpan(), itemPos.GetAlignment());
                linear0->addItem(m_itemACID);
                m_itemACID->SetTextColor(m_colorForeground);
            }
            else
            {
                m_itemACID->SetVisible(false);
                m_itemACID->setVisible(false);
            }
            if (labelMap.contains(XGridItem::CODE))
            {
                m_itemCODE->SetVisible(true);
                m_itemCODE->setVisible(true);
                const XGridItem& itemPos = labelMap[XGridItem::CODE];
                //grid->addItem(m_itemCODE, itemPos.GetRow(), itemPos.GetColumn(), itemPos.GetRowSpan(), itemPos.GetColumnSpan(), itemPos.GetAlignment());
                linear0->addItem(m_itemCODE);
                m_itemCODE->SetTextColor(m_colorForeground);
            }
            else
            {
                m_itemCODE->SetVisible(false);
                m_itemCODE->setVisible(false);
            }
            if (labelMap.contains(XGridItem::LX))
            {
                m_itemLX->SetVisible(true);
                m_itemLX->setVisible(true);
                const XGridItem& itemPos = labelMap[XGridItem::LX];
                //grid->addItem(m_itemLX, itemPos.GetRow(), itemPos.GetColumn(), itemPos.GetRowSpan(), itemPos.GetColumnSpan(), itemPos.GetAlignment());
                linear0->addItem(m_itemLX);
                m_itemLX->SetTextColor(m_colorForeground);
            }
            else
            {
                m_itemLX->SetVisible(false);
                m_itemLX->setVisible(false);
            }
            if (labelMap.contains(XGridItem::NS))
            {
                m_itemCS->SetVisible(true);
                m_itemCS->setVisible(true);
                const XGridItem& itemPos = labelMap[XGridItem::NS];
                //grid->addItem(m_itemCS, itemPos.GetRow(), itemPos.GetColumn(), itemPos.GetRowSpan(), itemPos.GetColumnSpan(), itemPos.GetAlignment());
                linear0->addItem(m_itemCS);
                m_itemCS->SetTextColor(m_colorForeground);
            }
            else
            {
                m_itemCS->SetVisible(false);
                m_itemCS->setVisible(false);
            }
            if (labelMap.contains(XGridItem::AFL))
            {
                m_itemAFL->SetVisible(true);
                m_itemAFL->setVisible(true);
                const XGridItem& itemPos = labelMap[XGridItem::AFL];
                //grid->addItem(m_itemAFL, itemPos.GetRow(), itemPos.GetColumn(), itemPos.GetRowSpan(), itemPos.GetColumnSpan(), itemPos.GetAlignment());
                linear0->addItem(m_itemAFL);
                m_itemAFL->SetTextColor(m_colorForeground);
            }
            else
            {
                m_itemAFL->SetVisible(false);
                m_itemAFL->setVisible(false);
            }
            if (labelMap.contains(XGridItem::CFL))
            {
                m_itemCFL->SetVisible(true);
                m_itemCFL->setVisible(true);
                const XGridItem& itemPos = labelMap[XGridItem::CFL];
                //grid->addItem(m_itemCFL, itemPos.GetRow(), itemPos.GetColumn(), itemPos.GetRowSpan(), itemPos.GetColumnSpan(), itemPos.GetAlignment());
                linear0->addItem(m_itemCFL);
                m_itemCFL->SetTextColor(m_colorForeground);
            }
            else
            {
                m_itemCFL->SetVisible(false);
                m_itemCFL->setVisible(false);
            }
            if (labelMap.contains(XGridItem::TFL))
            {
                m_itemTFL->SetVisible(true);
                m_itemTFL->setVisible(true);
                const XGridItem& itemPos = labelMap[XGridItem::TFL];
                //grid->addItem(m_itemTFL, itemPos.GetRow(), itemPos.GetColumn(), itemPos.GetRowSpan(), itemPos.GetColumnSpan(), itemPos.GetAlignment());
                linear0->addItem(m_itemTFL);
                m_itemTFL->SetTextColor(m_colorForeground);
            }
            else
            {
                m_itemTFL->SetVisible(false);
                m_itemTFL->setVisible(false);
            }
            if (labelMap.contains(XGridItem::TREND))
            {
                m_itemTEND->SetVisible(true);
                m_itemTEND->setVisible(true);
                const XGridItem& itemPos = labelMap[XGridItem::TREND];
                //grid->addItem(m_itemTEND, itemPos.GetRow(), itemPos.GetColumn(), itemPos.GetRowSpan(), itemPos.GetColumnSpan(), itemPos.GetAlignment());
                linear0->addItem(m_itemTEND);
                m_itemTEND->SetTextColor(m_colorForeground);
            }
            else
            {
                m_itemTEND->SetVisible(false);
                m_itemTEND->setVisible(false);
            }
            if (labelMap.contains(XGridItem::VRC))
            {
                m_itemVRC->SetVisible(true);
                m_itemVRC->setVisible(true);
                const XGridItem& itemPos = labelMap[XGridItem::VRC];
                //grid->addItem(m_itemVRC, itemPos.GetRow(), itemPos.GetColumn(), itemPos.GetRowSpan(), itemPos.GetColumnSpan(), itemPos.GetAlignment());
                linear0->addItem(m_itemVRC);
                m_itemVRC->SetTextColor(m_colorForeground);
            }
            else
            {
                m_itemVRC->SetVisible(false);
                m_itemVRC->setVisible(false);
            }
            if (labelMap.contains(XGridItem::HDG))
            {
                m_itemHDG->SetVisible(true);
                m_itemHDG->setVisible(true);
                const XGridItem& itemPos = labelMap[XGridItem::HDG];
                //grid->addItem(m_itemHDG, itemPos.GetRow(), itemPos.GetColumn(), itemPos.GetRowSpan(), itemPos.GetColumnSpan(), itemPos.GetAlignment());
                linear0->addItem(m_itemHDG);
                m_itemHDG->SetTextColor(m_colorForeground);
            }
            else
            {
                m_itemHDG->SetVisible(false);
                m_itemHDG->setVisible(false);
            }
            if (labelMap.contains(XGridItem::HDG2))
            {
                m_itemHDG2->SetVisible(true);
                m_itemHDG2->setVisible(true);
                const XGridItem& itemPos = labelMap[XGridItem::HDG2];
                //grid->addItem(m_itemHDG2, itemPos.GetRow(), itemPos.GetColumn(), itemPos.GetRowSpan(), itemPos.GetColumnSpan(), itemPos.GetAlignment());
                linear0->addItem(m_itemHDG2);
                m_itemHDG2->SetTextColor(m_colorForeground);
            }
            else
            {
                m_itemHDG2->SetVisible(false);
                m_itemHDG2->setVisible(false);
            }
            if (labelMap.contains(XGridItem::BANK))
            {
                m_itemROLL->SetVisible(true);
                m_itemROLL->setVisible(true);
                const XGridItem& itemPos = labelMap[XGridItem::BANK];
                //grid->addItem(m_itemROLL, itemPos.GetRow(), itemPos.GetColumn(), itemPos.GetRowSpan(), itemPos.GetColumnSpan(), itemPos.GetAlignment());
                linear0->addItem(m_itemROLL);
                m_itemROLL->SetTextColor(m_colorForeground);
            }
            else
            {
                m_itemROLL->SetVisible(false);
                m_itemROLL->setVisible(false);
            }
            if (labelMap.contains(XGridItem::NextPoint))
            {
                m_itemNextPoint->SetVisible(true);
                m_itemNextPoint->setVisible(true);
                const XGridItem& itemPos = labelMap[XGridItem::NextPoint];
                //grid->addItem(m_itemNextPoint, itemPos.GetRow(), itemPos.GetColumn(), itemPos.GetRowSpan(), itemPos.GetColumnSpan(), itemPos.GetAlignment());
                linear0->addItem(m_itemNextPoint);
                m_itemNextPoint->SetTextColor(m_colorForeground);
            }
            else
            {
                m_itemNextPoint->SetVisible(false);
                m_itemNextPoint->setVisible(false);
            }
            if (labelMap.contains(XGridItem::SPD1))
            {
                m_itemSPD1->SetVisible(true);
                m_itemSPD1->setVisible(true);
                const XGridItem& itemPos = labelMap[XGridItem::SPD1];
                //grid->addItem(m_itemSPD1, itemPos.GetRow(), itemPos.GetColumn(), itemPos.GetRowSpan(), itemPos.GetColumnSpan(), itemPos.GetAlignment());
                linear0->addItem(m_itemSPD1);
                m_itemSPD1->SetTextColor(m_colorForeground);
            }
            else
            {
                m_itemSPD1->SetVisible(false);
                m_itemSPD1->setVisible(false);
            }
            if (labelMap.contains(XGridItem::SPD2))
            {
                m_itemSPD2->SetVisible(true);
                m_itemSPD2->setVisible(true);
                const XGridItem& itemPos = labelMap[XGridItem::SPD2];
                //grid->addItem(m_itemSPD2, itemPos.GetRow(), itemPos.GetColumn(), itemPos.GetRowSpan(), itemPos.GetColumnSpan(), itemPos.GetAlignment());
                linear0->addItem(m_itemSPD2);
                m_itemSPD2->SetTextColor(m_colorForeground);
            }
            else
            {
                m_itemSPD2->SetVisible(false);
                m_itemSPD2->setVisible(false);
            }
            if (labelMap.contains(XGridItem::V))
            {
                m_itemV->SetVisible(true);
                m_itemV->setVisible(true);
                const XGridItem& itemPos = labelMap[XGridItem::V];
                //grid->addItem(m_itemV, itemPos.GetRow(), itemPos.GetColumn(), itemPos.GetRowSpan(), itemPos.GetColumnSpan(), itemPos.GetAlignment());
                linear0->addItem(m_itemV);
                m_itemV->SetTextColor(m_colorForeground);
            }
            else
            {
                m_itemV->SetVisible(false);
                m_itemV->setVisible(false);
            }
            if (labelMap.contains(XGridItem::UVD))
            {
                m_itemUVD->SetVisible(true);
                m_itemUVD->setVisible(true);
                const XGridItem& itemPos = labelMap[XGridItem::UVD];
                //grid->addItem(m_itemUVD, itemPos.GetRow(), itemPos.GetColumn(), itemPos.GetRowSpan(), itemPos.GetColumnSpan(), itemPos.GetAlignment());
                linear0->addItem(m_itemUVD);
                m_itemUVD->SetTextColor(m_colorForeground);
            }
            else
            {
                m_itemUVD->SetVisible(false);
                m_itemUVD->setVisible(false);
            }
            if (labelMap.contains(XGridItem::PEL))
            {
                m_itemPEL->SetVisible(true);
                m_itemPEL->setVisible(true);
                const XGridItem& itemPos = labelMap[XGridItem::PEL];
                //grid->addItem(m_itemPEL, itemPos.GetRow(), itemPos.GetColumn(), itemPos.GetRowSpan(), itemPos.GetColumnSpan(), itemPos.GetAlignment());
                linear0->addItem(m_itemPEL);
                m_itemPEL->SetTextColor(m_colorForeground);
            }
            else
            {
                m_itemPEL->SetVisible(false);
                m_itemPEL->setVisible(false);
            }
            if (labelMap.contains(XGridItem::RFL))
            {
                m_itemRFL->SetVisible(true);
                m_itemRFL->setVisible(true);
                const XGridItem& itemPos = labelMap[XGridItem::RFL];
                //grid->addItem(m_itemRFL, itemPos.GetRow(), itemPos.GetColumn(), itemPos.GetRowSpan(), itemPos.GetColumnSpan(), itemPos.GetAlignment());
                linear0->addItem(m_itemRFL);
                m_itemRFL->SetTextColor(m_colorForeground);
            }
            else
            {
                m_itemRFL->SetVisible(false);
                m_itemRFL->setVisible(false);
            }
            if (labelMap.contains(XGridItem::AZIMUTH))
            {
                m_itemAzimuth->SetVisible(true);
                m_itemAzimuth->setVisible(true);
                const XGridItem& itemPos = labelMap[XGridItem::AZIMUTH];
                //grid->addItem(m_itemAzimuth, itemPos.GetRow(), itemPos.GetColumn(), itemPos.GetRowSpan(), itemPos.GetColumnSpan(), itemPos.GetAlignment());
                linear0->addItem(m_itemAzimuth);
                m_itemAzimuth->SetTextColor(m_colorForeground);
            }
            else
            {
                m_itemAzimuth->SetVisible(false);
                m_itemAzimuth->setVisible(false);
            }
            if (labelMap.contains(XGridItem::DISTANCE))
            {
                m_itemDistance->SetVisible(true);
                m_itemDistance->setVisible(true);
                const XGridItem& itemPos = labelMap[XGridItem::DISTANCE];
                //grid->addItem(m_itemDistance, itemPos.GetRow(), itemPos.GetColumn(), itemPos.GetRowSpan(), itemPos.GetColumnSpan(), itemPos.GetAlignment());
                linear0->addItem(m_itemDistance);
                m_itemDistance->SetTextColor(m_colorForeground);
            }
            else
            {
                m_itemDistance->SetVisible(false);
                m_itemDistance->setVisible(false);
            }
            if (labelMap.contains(XGridItem::QNH))
            {
                m_itemQNH->SetVisible(true);
                m_itemQNH->setVisible(true);
                const XGridItem& itemPos = labelMap[XGridItem::QNH];
                //grid->addItem(m_itemQNH, itemPos.GetRow(), itemPos.GetColumn(), itemPos.GetRowSpan(), itemPos.GetColumnSpan(), itemPos.GetAlignment());
                linear0->addItem(m_itemQNH);
                m_itemQNH->SetTextColor(m_colorForeground);
            }
            else
            {
                m_itemQNH->SetVisible(false);
                m_itemQNH->setVisible(false);
            }
        }
        else
        {
            QList<QGraphicsItem*> child_items = childItems();
            QList<QGraphicsItem*>::const_iterator it_child = child_items.begin();
            for (; it_child != child_items.end(); ++it_child) {
                CGUILabelText *item = dynamic_cast<CGUILabelText*>(*it_child);
                if (item) {
                    item->SetVisible(false);
                    item->setVisible(false);
                }
            }
            setLabelColors(label_type);
            if(linear0)
                delete linear0;
            if(linear1)
                delete linear1;
            if(linear1_1)
                delete linear1_1;
            if(linear2)
                delete linear2;
            if(linear3)
                delete linear3;
            if(linear4)
                delete linear4;
            if(linear5)
                delete linear5;
            fillLabelFields(grid, label_type);

            for (int i = 0; i < grid->count(); ++i) {
                QGraphicsLinearLayout *linear = dynamic_cast<QGraphicsLinearLayout*>(grid->itemAt(i));
                for(int  j = 0; j < linear->count(); j++)
                {
                    dynamic_cast<CGUILabelText*>(linear->itemAt(j))->SetVisible(true);
                    dynamic_cast<QGraphicsItem*>(linear->itemAt(j))->setVisible(true);
                }
                //dynamic_cast<CGUILabelText*>(grid->itemAt(i))->SetVisible(true);
                //dynamic_cast<QGraphicsItem*>(grid->itemAt(i))->setVisible(true);
            }

            processFlightLevels(label_type);
        }
    }
    SetColor();
}

void CGUIAirplaneLabel::updateGrid()
{
    //if (!m_pAirplane->GetTransition().isEmpty())
    //if (CAirplaneData::Transfer == m_pAirplane->GetPlaneType())
    if (!m_pAirplane)
        return;

    if (m_pAirplane->isFlying())
    {
        if (m_pAirplane->isTransferTimeOut())
            m_pAirplane->MayBeCancelTransfer();

        QString sText;
        //line0
        if (m_pAirplane->GetMode3AReply() == 0) {
            m_itemCODE->setText(tr("none"));
        } else {
            sText = m_pAirplane->GetMode3AReplyToString();
            if (!m_pAirplane->GetSFPL()){
                //sText += "/" + m_pAirplane->GetACID();
            } else {
                //automatic save CODE to Redis
                if(!(m_itemCODE->text() == sText) && (!CAirplane::sListSpecCodes.contains(sText)))
                {
                    const QSharedPointer<CSFPL> &sfpl = m_pAirplane->GetSFPL();
                    sfpl->SetCODE(m_pAirplane->GetMode3AReply());
                    if (master()->getRedisDB()) {
                        bool ok = master()->getRedisDB()->saveSFPL(sfpl);
                        if (!liveObjects.contains(this))
                            return;
                        if (!ok)
                            qCritical() << "error saving CODE";
                    }
                }
            }
            //-----------------------
            m_itemCODE->setText(sText);

            /*if (CAirplane::sListSpecCodes.contains(sText) && isAssumed())
            {
                QSharedPointer<CReminder> pRem(new CReminder(CReminder::SpecCodes, sText));
                pRem->SetComplete(true);
                pRem->SetTime(QDateTime::currentDateTime());
                if (!m_pAirplane->GetTempReminders().contains(CReminderKey(*pRem)))
                    m_pAirplane->AddReminder(pRem);
            }
            else
            {
                m_pAirplane->RemoveReminderByType(CReminder::SpecCodes);
            }*/
        }

        const CAirplaneData::PlaneType plane_type = m_pAirplane->GetPlaneType();
        const QSharedPointer<CSFPL> &sfpl = m_pAirplane->GetSFPL();

        processAcid(sfpl);

        if (sfpl) {
            const QString &lit = sfpl->GetLit();
            m_itemL->setText(lit.isEmpty() ? "" : lit);
            const QString &type = sfpl->GetTypeFlight();
            m_itemType->setText(type.isEmpty() ? "Type" : type);
        }


        if (sfpl) {
            if(!(m_pAirplane->isOutAirport() || m_pAirplane->isInAirport())) {
                const QString &mil = sfpl->GetFlightType();
                m_itemM->setText((mil.isEmpty() || mil != "M") ? " " : mil);
            }
            if(m_pAirplane->isOutAirport()) {
                const QString &mil = sfpl->GetFlightType();
                m_itemM->setText((mil.isEmpty() || mil != "M") ? " " : mil);

                //const QString &mil = sfpl->GetTurbulence();
                //m_itemM->setText((mil.isEmpty()) ? "" : mil);
            }
            if(m_pAirplane->isInAirport()) {
                const QString &mil = sfpl->GetTurbulence();
                m_itemM->setText((mil.isEmpty()) ? " " : mil);
            }
            m_itemLM->setText(m_itemL->text() + m_itemM->text());
        }

        /*if (sfpl) {
          if(m_pAirplane->isOutAirport() ||m_pAirplane->isInAirport()) {
            const QString &lit = sfpl->GetLit();
            const QString &mil = sfpl->GetTurbulence();
            if(!lit.isEmpty())
                m_itemACID->setText(m_itemACID->text() + " " + lit);
            if(!mil.isEmpty())
                m_itemACID->setText(m_itemACID->text() + " " + mil);
          }
        }*/
        int mhdg = static_cast<int>(m_pAirplane->heading());
        m_Mhdg->setText(QString::number(mhdg));

        if(master() && master()->getRedisDB() && master()->GetView())
        {
            QSharedPointer<CView> pView = qSharedPointerDynamicCast<CView>(master()->GetView());
            if (pView)
                if(pView->GetMainFIR())
                {
                    if(m_pAirplane->isOurAirport())
                    {
                        if(!networkSettings()->value("RW1/name").toString().isEmpty() && networkSettings()->value("RW2/name").toString().isEmpty()) {
                            RunwayWidgetDataFIR rwf(pView->GetMainFIR()->GetIDName());
                            if(master()->getRedisDB()->loadRWF1(pView->GetMainFIR()->GetIDName(), rwf))
                                m_Runway->setText(rwf.currentRunway());
                        }
                        if(!networkSettings()->value("RW2/name").toString().isEmpty() && networkSettings()->value("RW1/name").toString().isEmpty()) {
                            RunwayWidgetDataFIR rwf(pView->GetMainFIR()->GetIDName());
                            if(master()->getRedisDB()->loadRWF2(pView->GetMainFIR()->GetIDName(), rwf))
                                m_Runway->setText(rwf.currentRunway());
                        }
                        if(!networkSettings()->value("RW1/name").toString().isEmpty() && !networkSettings()->value("RW2/name").toString().isEmpty())
                            m_Runway->setText("RW");
                    }
                }
        }

        sText = STR_NEXT_SECTOR;
        if (!m_pAirplane->GetTransition().isEmpty()) {
            sText = m_pAirplane->GetTransition();
        } else {
            if (sfpl) {
                if (CAirplaneData::ASSUME == plane_type) {
                    if (!sfpl->GetCurSector().isEmpty())
                        sText = sfpl->GetCurSector();
                } else if (CAirplaneData::Transfer == plane_type) {
                    if (!sfpl->GetNextSector().isEmpty())
                        sText = sfpl->GetNextSector();
                } else {
                    if (!sfpl->GetCurSector().isEmpty())
                        sText = sfpl->GetCurSector();
                }
            }
        }
        m_itemCS->setText(sText);

        item_assume_->setText(sText);

#ifdef QT_DEBUG
        if (sfpl) {
            item_cs_debug_->setText("cs " + sfpl->GetCurSector());
            item_ns_debug_->setText("ns " + sfpl->GetNextSector());
        }
#endif

        int trend = 0;
        //line2
        QString sTextTemp;
        QString sTextOldOld;
        if (!m_pAirplane->IsHeightQNH())
        {
            double dAFL = PressOnAirdromeFromSee(m_pAirplane->GetFL()) * TransformFeet();
            //double dAFLOld = PressOnAirdromeFromSee(m_pAirplane->GetFLOld()) * TransformFeet();
            double dAFLOldOld = PressOnAirdromeFromSee(m_pAirplane->GetFLOldOld()) * TransformFeet();
            sText = QString("%1").arg(dAFL > 0 ? dAFL : 0,
                                      AppSettings::IsSISet(AppSettings::SIFeet) ? 3 : AppSettings::METER_CAPACITY, 'f', 0, QChar('0'));
            //sTextTemp = QString("%1").arg(dAFL > 0 ? dAFL : 0,
            //                          AppSettings::IsSISet(AppSettings::SIFeet) ? 3 : AppSettings::METER_CAPACITY, 'f', 1, QChar('0'));
           // const QString sTextOld = QString("%1").arg(dAFLOld > 0 ? dAFLOld : 0,
           //                                            AppSettings::IsSISet(AppSettings::SIFeet) ? 3 : AppSettings::METER_CAPACITY, 'f', 0, QChar('0'));
            sTextOldOld = QString("%1").arg(dAFLOldOld > 0 ? dAFLOldOld : 0,
                                                       AppSettings::IsSISet(AppSettings::SIFeet) ? 3 : AppSettings::METER_CAPACITY, 'f', 0, QChar('0'));
            if (sText != sTextOldOld && m_pAirplane->GetFLOldOld()) {
                trend = (dAFL < dAFLOldOld) ? -1 : 1;
            }
            if (m_pAirplane->GetEchelon() > m_pAirplane->GetFL() && !m_pAirplane->isUvd())
                sText.prepend("A");

        }
        else
        {
            sText = QString("A%1").arg(m_pAirplane->GetHeightQNH(), 3, 'f', 0, QChar('0'));
            const QString sTextOld = QString("A%1").arg(m_pAirplane->GetHeightQNHOld(), 3, 'f', 0, QChar('0'));
            if (sText != sTextOld && m_pAirplane->GetHeightQNHOld()) {
                trend = (m_pAirplane->GetHeightQNH() < m_pAirplane->GetHeightQNHOld()) ? -1 : 1;
            }
        }
#ifdef QT_DEBUG
        item_cs_debug_->setText(sText);
        item_ns_debug_->setText(sTextOldOld);

#endif
        m_itemAFL->setText(sText);

        m_itemTEND->setText(trend > 0 ? UTF8_QSTRING("\u2191") : (trend < 0 ? UTF8_QSTRING("\u2193") : ""));


        processCfl(plane_type, sfpl);

//        if (!m_pAirplane->GetTFL()) {
//            if (sfpl) {
//                if (sfpl->GetCOPoutPoint() && sfpl->GetCOPoutPoint()->IsLevel())
//                    m_pAirplane->SetTFL(FLVal(sfpl->GetCOPoutPoint()->GetLevel(), false));
//                else
//                    m_pAirplane->SetTFL(FLVal(sfpl->GetLevelS()));
//            }
//        }
        if (m_pAirplane->GetTFL()) {
            //sText.sprintf("%03.0f", m_pAirplane->GetTFL() * TransformFeet(true));
            sText = GetFLText(m_pAirplane->GetTFL(), true);
            m_itemTFL->setText(sText);
        } else /*if (isAssumed())*/ {
            m_itemTFL->setText(QString("000"));
        }

        /*if (AppSettings::IsSISet(AppSettings::SIKnot))
            sText.sprintf("%02d", abs((int)PreRound(m_pAirplane->GetVz() / 100)));
        else
            sText.sprintf("%02d", abs((int)PreRound(m_pAirplane->GetVz() / 60 * 0.3048)));
        m_itemVRC->setText(sText);*/
        if (sfpl) {
            const QString &vrc = sfpl->GetVrc();
            m_itemVRC->setText(vrc.isEmpty() ? "vrc" : vrc);
        }

        /*sText.sprintf("%03d", m_pAirplane->GetAzimuth());
        sText += UTF8_QSTRING("\u00B0");
        m_itemHDG->setText(sText);*/
        if (sfpl) {
            const QString &hdg = sfpl->GetHdg();
            m_itemHDG->setText(hdg.isEmpty() ? "hdg" : hdg + UTF8_QSTRING("\u00B0"));
        }

        sText.sprintf("%02.0f", m_pAirplane->GetBank() * 180 / M_PI);
        m_itemROLL->setText(sText);

        /*if (m_pAirplane->IsHolding())
        {
            QSharedPointer<CHolding> pHolding = m_pAirplane->GetHolding();
            if (pHolding) {
                sText.sprintf("%03.0f", pHolding->GetInCourse());
                m_itemNextPoint->setText(pHolding->GetPoint()->GetName() + "/" + sText);
            }
        }
        else
        {
            if (m_pAirplane->IsCustomPoint()) {
                sText = m_pAirplane->GetCustomPoint()->GetName();
            } else if (sfpl && sfpl->GetFPLFrag() && sfpl->GetFPLFrag()->GetNextPoint() >= 0
                       && sfpl->GetFPLFrag()->GetNextPoint() < sfpl->GetFPLFrag()->GetAllPoint().size())
            {
                CProcFrag::TPointFragList pPointList = sfpl->GetFPLFrag()->GetAllPoint();
                QSharedPointer<CPoint> pPoint = pPointList[sfpl->GetFPLFrag()->GetNextPoint()]->GetPoint();
                sText = pPoint->GetName();
                for (int i = sfpl->GetFPLFrag()->GetNextPoint(); i < pPointList.size(); ++i)
                {
                    if (!pPointList[i]->IsGenerated())
                    {
                        sText = pPointList[i]->GetPoint()->GetName();
                        break;
                    }
                }
                m_itemNextPoint->setText(sText);
            } else {
                m_itemNextPoint->setText(tr("Point"));
            }
        }*/
        if (sfpl) {
            QString cop = sfpl->GetCOPout();
            if (cop.isEmpty())
                cop = sfpl->GetCOPin();
            if(m_pAirplane->isOutAirport())
            {
                switch(np_type)
                {
                    case NextPoint:
                        m_itemNextPoint->setText(cop.isEmpty() ? "XXXXX" : cop);
                        break;
                    case PathPoints:
                        m_itemNextPoint->setText(cop.isEmpty() ? "XXXXX" : cop);
                        break;
                    case Mhdg:
                        m_itemNextPoint->setText(QString::number(static_cast<int>(m_pAirplane->heading())));
                        break;
                    default:
                    break;
                }

            }
            else
                m_itemNextPoint->setText(cop.isEmpty() ? "XXXXX" : cop);
        }

        //line4
        double dVal = m_pAirplane->GetIAS() * TransformM_SEC();
        sText.sprintf("%03.0f", dVal);
        m_itemSPD1->setText(dVal ? sText : "spd1");

        if (sfpl) {
            const QString &spd = sfpl->GetSpd();
            item_spd_->setText(spd.isEmpty() ? "spd" : spd);
        }

        //  GSP field of main airplane label
        QString gsp;
        if (m_pAirplane->groundAirspeed() > 0.)
            gsp.sprintf("%03.0f", m_pAirplane->groundAirspeed()*Transform_Nms_SI());
        else
            gsp.sprintf("%03.0f", sqrt(pow(m_pAirplane->GetVelocity().GetVx(),2.)+pow(m_pAirplane->GetVelocity().GetVy(),2.)) * TransformM_SEC()  ) ;
        item_gsp_->setText(gsp);

        dVal = m_pAirplane->GetTAS();
        if (m_pAirplane->GetFLinMeters() >= m_pAirplane->GetMachLevel())
        {
            dVal = dVal / avia::GetSonicSpeed(m_pAirplane->GetFLinMeters());
            sText.sprintf("M%1.02f", dVal);
        }
        else
        {
            if (AppSettings::IsSISet(AppSettings::SIKnot))
                sText.sprintf("K%03.0f", dVal * TransformM_SEC());
            else
                sText.sprintf("%03.0f", dVal * TransformM_SEC());
        }
        m_itemSPD2->setText(dVal ? sText : "spd2");

        dVal = m_pAirplane->indicatedAirspeed() * Transform_Knot_SI();
        sText.sprintf("%03.0f", dVal);
        item_w_->setText(sText);

        m_itemUVD->setText(QString::number(m_pAirplane->GetTrackNumberUvd()));

        if (sfpl && sfpl->GetCOPinFL()) {
            //sText.sprintf("%03.0f", m_pAirplane->GetSFPL()->GetCOPinFL() * TransformFeet(true));
            sText = GetFLText(sfpl->GetCOPinFL(), true);
            m_itemPEL->setText(sText);
        } else {
            m_itemPEL->setText(tr("PEL"));
        }

        if (sfpl) {
            double dRFL = 0;
            if (sfpl->GetCOPoutPoint() && sfpl->GetCOPoutPoint()->IsLevel())
                dRFL = sfpl->GetCOPoutPoint()->GetLevel() / 30.48;
            else
                dRFL = sfpl->GetLevel() / 30.48;
            //sText.sprintf("%03.0f", dRFL * TransformFeet(true));
            sText = GetFLText(dRFL, true);
            m_itemRFL->setText(sText);
        }
        else
            m_itemRFL->setText(tr("RFL"));

        m_itemAzimuth->setText(m_AzimuthAndDistance.GetAzimuthText(m_pAirplane));
        m_itemDistance->setText(m_AzimuthAndDistance.GetDistanceText(m_pAirplane));

        sText = QString("%1").arg(m_pAirplane->GetFL() * TransformFeet(),
                                  AppSettings::IsSISet(AppSettings::SIFeet) ? 3 : AppSettings::METER_CAPACITY, 'f', 0, QChar('0'));
        m_itemQNH->setText(sText);

        SetHDG2Text();

        /*m_bShowReminder = false;
        CReminder::TListReminder list;
        CAirplaneData::TReminderMap::const_iterator itReminder = m_pAirplane->GetTempReminders().begin();
        for (; itReminder != m_pAirplane->GetTempReminders().end(); ++itReminder)
        {
            if ((*itReminder)->GetComplete())
                m_bShowReminder = true;
            list.append(*itReminder);
        }
        if (m_bShowReminder)
            showGrid(true);
        m_pReminder->SetReminders(list);*/
    }
    else
    {
        //SQUAWK
        if (m_pAirplane->GetMode3AReply() == 0)
            m_groundItemSQUAWK->setText(tr("none"));
        else
            m_groundItemSQUAWK->setText(m_pAirplane->GetMode3AReplyToString());

        //ACID
        QString sText = QString::null;
        if (m_pAirplane->GetSFPL()) {
            m_groundItemACID->setText(m_pAirplane->GetSFPL()->GetACID());
        } else {
                if (m_pAirplane->GetACID().isEmpty())
                    m_groundItemACID->setText(tr("No FPL"));
                else
                    m_groundItemACID->setText(m_pAirplane->GetACID());
        }
        //Speed
        double dIAS = m_pAirplane->GetGS() * TransformM_SEC();
        sText.sprintf("%03.0f", dIAS);
        m_groundItemSpeed->setText(sText);

        if (dIAS <= 1.4 && m_pAirplane->wasMoving())
        {
  //          m_groundItemStatus->setText(tr("Stop"));
            m_pAirplane->setWasMoving(false);
        }
        if (dIAS > 1.4)
            m_pAirplane->setWasMoving(true);
    }

    if (m_pAirplane && m_pAirplane->GetSFPL())
    {
        m_pConflict->SetConflicts(m_pAirplane->GetSFPL()->GetConflicts());
    }

    SetColor();
    adjustGrid();
}

void CGUIAirplaneLabel::adjustGrid()
{
    prepareGeometryChange();

    if (layout()) {
        layout()->invalidate();
        layout()->activate();
    }
    adjustSize();

    QRectF clRect = rect();
    clRect.adjust(-1, -1, 1, 1);
    m_clRect = clRect.normalized();

    //m_pReminder->UpdateWidth();

    m_pMTCDpoint->SetPos(m_clRect.topRight());
    m_pMTCDpoint->setVisible(m_pAirplane && m_pAirplane->isMTCD());
    if (m_pMTCDpoint->isVisible())
        m_pMTCDpoint->SetListMTCDText(m_pAirplane->GetListMTCDText(false), false);

    update();
}

void CGUIAirplaneLabel::showGrid(bool bShow)
{
    loadTypeGrid(bShow);
    m_bFullShow = bShow;// || m_bShowReminder/* || (networkSettings()->value("airplane/auto_collapse", "yes").toString() != "yes")*/;
    adjustGrid();
}

void CGUIAirplaneLabel::SetAirplane(CGUIAirplane *pPlane)
{
    m_pAirplane = pPlane;
    //m_pReminder->SetPlane(m_pAirplane);
}

QPointF CGUIAirplaneLabel::intersectRect(const QLineF &line, const QRectF &rect) const
{
    QPointF point;
    if (line.intersect(QLineF(rect.topLeft(), rect.topRight()), &point) == QLineF::BoundedIntersection) return point;
    if (line.intersect(QLineF(rect.topRight(), rect.bottomRight()), &point) == QLineF::BoundedIntersection) return point;
    if (line.intersect(QLineF(rect.bottomRight(), rect.bottomLeft()), &point) == QLineF::BoundedIntersection) return point;
    if (line.intersect(QLineF(rect.bottomLeft(), rect.topLeft()), &point) == QLineF::BoundedIntersection) return point;
    return point;
}

void CGUIAirplaneLabel::onDialogOpened()
{
//    m_bPress = true;
}

void CGUIAirplaneLabel::onDialogClosed()
{
//    m_bPress = false;
    showGrid(false);
}

void CGUIAirplaneLabel::OnEndEdit(const CGUILabelText *sender, const QStringList sValues)
{
    //showGrid(false);
    if (sender == m_itemCODE)
    {
        WriteLog("CODE", sValues.at(0));
        if (sValues.at(0) != tr("none")) {
            m_pAirplane->ChangeMode3AReply(sValues.at(0).toInt(0, 8));
            m_itemCODE->setText(sValues.at(0).rightJustified(4, '0'));
        } else {
            m_pAirplane->ChangeMode3AReply(0);
            m_itemCODE->setText(tr("none"));
        }
    }
    else if (sender == m_itemACID)
    {
        if (m_pAirplane) {
            m_pAirplane->ChangeSFPL();
            return;
        }
    }
    else if (sender == m_itemCS)
    {
        if (sValues.at(0) == "ASSUME") {
            QTimer::singleShot(4000, this, SLOT(afterSleep()));
        } else {
            if (sValues.at(0) == "OUT") {
                logOperation(tr("Out: %1").arg(m_pAirplane->GetMode3AReplyToString()));
                WriteLog("NS", sValues.at(0));
                //m_pAirplane->showActOutForm();
                //m_pAirplane->ChangeOperator(0, "");
                m_pAirplane->transfer(QString());
            } else {
                QString next_sector(sValues.at(1));
                logOperation(tr("Transfer: %1 to %2").arg(m_pAirplane->GetMode3AReplyToString())
                             .arg(next_sector));
                WriteLog("NS", next_sector);
                m_pAirplane->transfer(next_sector);
                //m_itemCS->setText(next_sector);
                //m_pAirplane->SetPlaneType(CAirplaneData::Transfer);
            }
        }
        SetColor();
    }
    else if (sender == m_itemCFL)
    {
        if (sValues.size() >= 3) {
            //always send height over see level
            FLVal fl(sValues.at(0).toDouble(), sValues.at(2).toInt());
            WriteLog("CFL", QString("%1(%2)").arg(sValues.at(0)).arg(fl.GetF()));
            m_pAirplane->ChangeFL(fl);
            QString sText = GetFLText(fl, false, false);
            m_itemCFL->setText(sText);

            const QSharedPointer<CSFPL> &sfpl = m_pAirplane->GetSFPL();
            if (sfpl) {
                sfpl->SetCFL(fl);
                m_pAirplane->SetCFL(fl);
                m_pAirplane->UpdateFPLFrag(sfpl->GetFPLFragState());
                m_pAirplane->CalculateSectors(sfpl);
                logOperation(tr("%1: CFL changed, new sectors list: %2").arg(sfpl->GetACID()).arg(sfpl->GetAllSectors()));

                if (master()->getRedisDB()) {
                    /*bool ok = */master()->getRedisDB()->saveSFPL(sfpl);
                    /*if (!ok)
                    QMessageBox::critical(this, tr("Error"), tr("Error saving FPL"));*/
                }
            }

            processFlightLevels(XGridLabel::Controlled);
            QGraphicsSceneHoverEvent *event(0);
            hoverLeaveEvent(event);
        }
    }
    else if (sender == m_itemTFL)
    {
        if (sValues.size() >= 3)
        {
            //always send height over see level
            FLVal fl(PressOnAirdromeToSee(FLVal(sValues.at(0).toDouble(), sValues.at(1).toDouble(), sValues.at(2).toInt())));
            WriteLog("TFL", QString("%1(%2)").arg(sValues.at(0)).arg(fl.GetF()));
            m_pAirplane->ChangeTFL(fl);
            QString sText = GetFLText(fl, true);
            m_itemTFL->setText(sText);

            const QSharedPointer<CSFPL> &sfpl = m_pAirplane->GetSFPL();
            if (sfpl) {
                sfpl->SetTFL(fl);
                m_pAirplane->SetTFL(fl);

                if (master()->getRedisDB()) {
                    /*bool ok = */master()->getRedisDB()->saveSFPL(sfpl);
                    /*if (!ok)
                    QMessageBox::critical(this, tr("Error"), tr("Error saving FPL"));*/
                }
            }

            processFlightLevels(XGridLabel::Controlled);
            QGraphicsSceneHoverEvent *event(0);
            hoverLeaveEvent(event);
        }
    }
    else if (sender == m_itemVRC)
    {
        WriteLog("VRC", sValues.at(0));
        QString vrc = sValues.at(0);
        /*if (AppSettings::IsSISet(AppSettings::SIKnot))
            m_pAirplane->ChangeVz(sValues.at(0).toDouble() * 100);
        else
            m_pAirplane->ChangeVz(sValues.at(0).toDouble() * 60 / 0.3048);*/
        m_itemVRC->setText(vrc);

        const QSharedPointer<CSFPL> &sfpl = m_pAirplane->GetSFPL();
        if (sfpl) {
            sfpl->SetVrc(vrc);
            if (master()->getRedisDB()) {
                /*bool ok = */master()->getRedisDB()->saveSFPL(sfpl);
                /*if (!ok)
                    QMessageBox::critical(this, tr("Error"), tr("Error saving FPL"));*/
            }
            QGraphicsSceneHoverEvent *event(0);
            hoverLeaveEvent(event);
        }
    } else if (sender == item_spd_) {
        WriteLog("SPD", sValues.at(0));
        const QString spd = sValues.at(0);
        item_spd_->setText(spd);

        const QSharedPointer<CSFPL> &sfpl = m_pAirplane->GetSFPL();
        if (sfpl) {
            sfpl->SetSpd(spd);
            if (master()->getRedisDB()) {
                /*bool ok = */master()->getRedisDB()->saveSFPL(sfpl);
                /*if (!ok)
                    QMessageBox::critical(this, tr("Error"), tr("Error saving FPL"));*/
            }
            QGraphicsSceneHoverEvent *event(0);
            hoverLeaveEvent(event);
        }
    }
    //else if (sender == m_itemHDG2)//
    else if (sender == m_itemHDG)
    {
        QString sText = m_itemHDG->text();
        QString sVal = sText.left(sText.size()-1);
        int iAzum = sVal.toInt();
        int iVal = sValues.at(1).toInt();
        sText = sValues.at(0);
        if (sText == "LEFT")
            iVal = (iAzum-iVal+360)%360;
        if (sText == "RIGHT")
            iVal = (iAzum+iVal)%360;
        //m_pAirplane->ChangeAzimuth(iVal);
        //m_pAirplane->SetPlaneAzimuth2(iVal);
        m_iAzimuth2 = iVal;
        sText = QString::number(iVal);
        sText += UTF8_QSTRING("\u00B0");
        WriteLog("HDG", sText);
        m_itemHDG->setText(sText);
    }
    else if (sender == m_itemROLL)
    {
        WriteLog("BANK", sValues.at(0));
        m_pAirplane->ChangeBank(sValues.at(0).toDouble() * M_PI / 180);
        m_itemROLL->setText(sValues.at(0));
    }
    else if (sender == m_itemNextPoint)
    {
        if (sValues.at(0).isEmpty())
        {
            m_pAirplane->GetPlaneNewCoord();
        }
        else
        {
            int iIndex = 0;
            if (sValues.at(0) == "Point")
            {
                WriteLog("NextPoint", sValues.at(2));
                iIndex = m_mapRoutePoints.value(sValues.at(1).toInt());
            }
            else if (sValues.at(0) == "Zone")
            {
                WriteLog("NextPoint", "holding " + sValues.at(2));
                iIndex = m_mapHoldingZones.value(sValues.at(1).toInt(), 0);
            }
            QSharedPointer<obelix::CCmdAirplaneCmd> pCmd(new obelix::CCmdAirplaneCmd(m_pAirplane->GetID(),
                                                                                     obelix::CCmdAirplaneCmd::NextPoint,
                                                                                     QString::number(iIndex)));
            pCmd->SetAltValue(sValues.at(0));
            ((XMasterWindow*)getMainWindow())->GetObClient().XMLSend(pCmd.staticCast<CXMLCmd>());
//            if (sValues.at(0) == "Point")
//            {
//                if (m_pAirplane->GetSFPL() && m_pAirplane->GetSFPL()->GetFPLFrag())
//                {
//                    WriteLog("NextPoint", sValues.at(2));
//                    m_pAirplane->GetSFPL()->GetFPLFrag()->SetNextPoint(m_mapRoutePoints.value(sValues.at(1).toInt()));
//                    m_itemNextPoint->setText(sValues.at(2));
//                }
//            }
//            else if (sValues.at(0) == "Zone")
//            {
//                WriteLog("NextPoint", "holding " + sValues.at(2));
//                m_pAirplane->ChangeHolding(m_mapHoldingZones.value(sValues.at(1).toInt(), 0));
//                m_itemNextPoint->setText(sValues.at(2));
//            }
        }
    }
    else if (sender == m_itemSPD1)
    {
        WriteLog("SPD1", sValues.at(0));
        m_itemSPD1->setText(sValues.at(0));
        //m_pAirplane->ChangeV(sValues.at(0).toDouble() / TransformM_SEC());
        //send Obelix here
        QSharedPointer<obelix::CCmdAirplaneCmd> pCmd(new obelix::CCmdAirplaneCmd(m_pAirplane->GetID(),
                                                                                 obelix::CCmdAirplaneCmd::spd1,
                                                                                 QString::number((sValues.at(0).toDouble() / TransformM_SEC()))));
        ((XMasterWindow*)getMainWindow())->GetObClient().XMLSend(pCmd.staticCast<CXMLCmd>());

        //m_pAirplane->SetIAS(sValues.at(0).toDouble() / TransformM_SEC());
    }
    else if (sender == m_itemSPD2)
    {
        if (!m_bMachLevel)
        //if(m_pAirplane->GetFLinMeters() < m_pAirplane->GetMachLevel())
        {
            WriteLog("SPD2", sValues.at(0));
            m_itemSPD2->setText(sValues.at(0));
            double dVal = sValues.at(0).toDouble();
            dVal /= TransformM_SEC();
            dVal /= avia::GetIASFactor(m_pAirplane->GetFLinMeters()); //CAS
            //send Obelix here
            QSharedPointer<obelix::CCmdAirplaneCmd> pCmd(new obelix::CCmdAirplaneCmd(m_pAirplane->GetID(),
                                                                                     obelix::CCmdAirplaneCmd::spd2,
                                                                                     QString::number(dVal)));
            ((XMasterWindow*)getMainWindow())->GetObClient().XMLSend(pCmd.staticCast<CXMLCmd>());
            //m_pAirplane->SetIAS(dVal);
        }
        else
        {
            double dVal = sValues.at(0).toDouble();
            QString sText;
            sText.sprintf("M%1.02f", dVal);
            WriteLog("SPD2", sText);
            m_itemSPD2->setText(sText);
            dVal *= avia::GetSonicSpeed(m_pAirplane->GetFLinMeters()); //TAS
            dVal /= avia::GetIASFactor(m_pAirplane->GetFLinMeters()); //CAS
            //m_pAirplane->ChangeV(dVal);
            //send Obelix here
            QSharedPointer<obelix::CCmdAirplaneCmd> pCmd(new obelix::CCmdAirplaneCmd(m_pAirplane->GetID(),
                                                                                     obelix::CCmdAirplaneCmd::spd2,
                                                                                     QString::number(dVal)));
            ((XMasterWindow*)getMainWindow())->GetObClient().XMLSend(pCmd.staticCast<CXMLCmd>());
            //m_pAirplane->SetIAS(dVal);
        }
    }
    else if (sender == m_groundItemStatus)
    {
        m_groundItemStatus->setText(sValues.at(0));
    }

    adjustGrid();
}

void CGUIAirplaneLabel::SetPoint(const CCoord* coord)
{
    WriteLog("NextPoint", coord->GetAsString());
    m_itemNextPoint->setText(coord->GetAsString());
    adjustGrid();
}

void CGUIAirplaneLabel::SetColor()
{
    if (m_pAirplane && m_pAirplane->isFlying())
    {
        foreach (QGraphicsItem *child_item, childItems()) {
            if (CGUILabelText *field = dynamic_cast<CGUILabelText*>(child_item))
                field->SetTextColor(m_colorForeground);
        }

        if (!m_pAirplane->GetTransition().isEmpty())
        {
            m_itemCS->setText(m_pAirplane->GetTransition());

            if (CAirplaneData::Transfer == m_pAirplane->GetPlaneType())
                m_itemCS->setBrush(Qt::white);
            else
                m_itemCS->setBrush(Qt::white);
        }

        updateHdgSpdVrcCflFieldsColor();

        item_spec_signal_->SetTextColor(Qt::yellow);
        item_spec_signal_->setBackgroundColor(Qt::red);
        item_assume_->SetTextColor(Qt::yellow);
        item_transfer_->SetTextColor(Qt::yellow);
        item_act_out_->SetTextColor(Qt::yellow);
#ifdef QT_DEBUG
        item_cs_debug_->SetTextColor(Qt::yellow);
        item_ns_debug_->SetTextColor(Qt::yellow);
#endif
    }
}

double CGUIAirplaneLabel::TransformM_SEC()
{
    if (AppSettings::IsSISet(AppSettings::SIKnot))
        // m/s -> knots
        return 1 / 0.5144;
    else
        // m/s -> km/h
        return 3.6;
}

double CGUIAirplaneLabel::Transform_Nms_SI() // convert Nm/s -> SI(knot or km/h)
{
    if (AppSettings::IsSISet(AppSettings::SIKnot))
        // Nm/s -> knots
        return 3600;
    else
        // Nm/s -> km/h
        return 3600*1.852;
}

double CGUIAirplaneLabel::Transform_Knot_SI() // convert Knot -> SI(knot or km/h)
{
    if (AppSettings::IsSISet(AppSettings::SIKnot))
        // Knot -> knots
        return 1;
    else
        // Knot -> km/h
        return 1.852;
}

double CGUIAirplaneLabel::TransformFeet(bool bHundredMeters/* = false*/)
{
    if (AppSettings::IsSISet(AppSettings::SIFeet))
        // hundred feet
        return 1;
    else
        if (bHundredMeters)
            // hundred feet -> hundred meters
            return 0.3048;
        else
            // hundred feet -> tens of meters
            return 0.3048 * 10;
}

QString CGUIAirplaneLabel::GetFLText(const FLVal& val, bool bHundredMeters/* = false*/, bool recalcToAirdrome) const
{
    QString sRes = "000";
    if (val)
    {
        FLVal flval(recalcToAirdrome ? PressOnAirdromeFromSee(val) : val);
        if (flval.isFeet())
        {
            // hundred feet
            //sRes = QString("F%1").arg(flval.GetF() > 0 ? flval.GetF() : 0, 3, 'f', 0, QLatin1Char('0'));
            sRes = QString("%1").arg(flval.GetF() > 0 ? flval.GetF() : 0, 3, 'f', 0, QLatin1Char('0'));
        }
        else
        {
            if (bHundredMeters)
                //hundred meters
                sRes = QString("%1").arg(flval.GetM() > 0 ? flval.GetM()/AppSettings::CFL_METER_DIVIDER : 0, AppSettings::CFL_METER_CAPACITY, 'f', 0, QLatin1Char('0'));
            else
                //tens of meters
                sRes = QString("%1").arg(flval.GetM() > 0 ? flval.GetM()/AppSettings::METER_DIVIDER : 0, AppSettings::METER_CAPACITY, 'f', 0, QLatin1Char('0'));
        }
    }
    return sRes;
}

double CGUIAirplaneLabel::PressOnAirdromeFromSee(double FL) const
{
    double height = FL;
    if (m_pAirplane->GetEchelon() > FL && !m_pAirplane->isUvd())
    {
        FL *= 100 * 0.3048; // hundred feet -> meters
        //QFE
        //height = FL - (760-m_pAirplane->GetPress())*11;
        //QNH
        height = FL + 8.25*(m_pAirplane->GetPress()-1013);
        height /= 100 * 0.3048;
    }
    return height;
}

FLVal CGUIAirplaneLabel::PressOnAirdromeFromSee(const FLVal& val) const
{
    FLVal ret(val);
    if (ret && (m_pAirplane->GetEchelon() > ret.GetF()))
    {
        FLVal tmp(PressOnAirdromeFromSee(ret.GetF()));
        ret = FLVal(tmp.GetF(), tmp.GetM(), ret.isFeet());
    }
    return ret;
}

double CGUIAirplaneLabel::PressOnAirdromeToSee(double FL) const
{
    double height = FL;
    if (m_pAirplane->GetEchelon() > FL)
    {
        FL *= 100 * 0.3048; // hundred feet -> meters
        //QFE
        //height = FL + (760-m_pAirplane->GetPress())*11;
        //QNH
        height = FL - 8.25*(m_pAirplane->GetPress()-1013);
        height /= 100 * 0.3048;
    }
    return height;
}

FLVal CGUIAirplaneLabel::PressOnAirdromeToSee(const FLVal& val) const
{
    FLVal ret(val);
    if (ret && (m_pAirplane->GetEchelon() > ret.GetF()))
    {
        const FLVal tmp(PressOnAirdromeToSee(ret.GetF()));
        ret = FLVal(tmp.GetF(), tmp.GetM(), ret.isFeet());
    }
    return ret;
}

void CGUIAirplaneLabel::SetActivePlane(bool bActive)
{
    if (m_bActive != bActive)
    {
        m_bActive = bActive;
        if (bActive)
            m_ActiveTimer->start(500);
        else
            m_ActiveTimer->stop();
        update();
    }
}

void CGUIAirplaneLabel::OnActivePlane()
{   //this?
    return; // removed flashing
    if (m_colorActive == m_color)
        m_colorActive = Qt::white;//Qt::darkRed;
    else
        m_colorActive = m_color;
    update();
}

void CGUIAirplaneLabel::setNextPoint(const QSharedPointer<CPoint> &next_point)
{
    const QSharedPointer<CSFPL> &sfpl = m_pAirplane->GetSFPL();
    if (sfpl) {
        m_itemNextPoint->setText(next_point->GetName());
        sfpl->SetCOPout(next_point->GetName());
        QTime eto_out_time(QTime::currentTime());
        eto_out_time = eto_out_time.addSecs(avia::GetTimeRoute(m_pAirplane->GetCurGUIPlaneCoord(), next_point->GetCoord(), m_pAirplane->GetGS()));
        sfpl->SetETOout(eto_out_time);

        if (master()->getRedisDB()) {
            bool ok = master()->getRedisDB()->saveSFPL(sfpl);
            if (!ok)
                qCritical() << "error saving sfpl next point";
        }
    }
}

void CGUIAirplaneLabel::WriteLog(QString sType, QString sValue)
{
    if (m_pAirplane)
    {
        QString sCode;
        if (m_pAirplane->GetMode3AReply() == 0)
            sCode = "none";
        else
            sCode = m_pAirplane->GetMode3AReplyToString();
        LOG_OPER_STR(sCode, sType, sValue);
    }
}

void CGUIAirplaneLabel::OnTimerAssume()
{
    WriteLog("NS", "ASSUME");
    m_pAirplane->AssumeOperator();
    SetColor();
}

void CGUIAirplaneLabel::SetAzimuth2(int degree)
{
    m_iAzimuth2 = qBound(0, degree, 360);
    m_iAzimuth2 %= 360;

    m_pAirplane->SetPlaneAzimuth2(m_iAzimuth2);

    SetHDG2Text();
}

void CGUIAirplaneLabel::ClearAzimuth2()
{
    const QSharedPointer<CSFPL> &sfpl = m_pAirplane->GetSFPL();
    if (sfpl) {
        const QString azimyth_str = QString("%1").arg(m_iAzimuth2, 3, 10, QLatin1Char('0'));
        sfpl->SetHdg(azimyth_str);

        if (master()->getRedisDB()) {
            bool ok = master()->getRedisDB()->saveSFPL(sfpl);
            if (!ok)
                qCritical() << "error saving sfpl azimuth";
        }
    }

    m_iAzimuth2 = kInvalidAzimuth;
    m_pAirplane->SetPlaneAzimuth2(m_iAzimuth2);
    SetHDG2Text();
}

void CGUIAirplaneLabel::SetHDG2Text()
{
    if (m_itemHDG2 && m_pAirplane)
    {
        m_iAzimuth2 = m_pAirplane->GetPlaneAzimuth2();
        if (m_iAzimuth2 != kInvalidAzimuth)
        {
            int iPlaneAzimuth = m_pAirplane->GetAzimuth();
            bool bTurnRight = false;
            int delta = 0;
            QString sTemp;
            if (iPlaneAzimuth < m_iAzimuth2)
            {
                bTurnRight = true;
                delta = m_iAzimuth2 - iPlaneAzimuth;
            }
            else
            {
                bTurnRight = false;
                delta = iPlaneAzimuth - m_iAzimuth2;
            }
            if (delta > 180)
            {
                bTurnRight = !bTurnRight;
                delta = 360 - delta;
            }
            if (delta)
            {
                sTemp = QString("(%1%2)")
                        .arg(bTurnRight ? tr("R") : tr("L"))
                        .arg(delta, 3, 10, QLatin1Char('0'));
            }
            QString sText = QString("%1%2%3")
                    .arg(m_iAzimuth2, 3, 10, QLatin1Char('0'))
                    .arg(UTF8_QSTRING("\u00B0"))
                    .arg(sTemp);
            m_itemHDG2->setText(sText);
        } else {
            m_itemHDG2->setText(tr("hdg"));
        }

        adjustGrid();
    }
}

void CGUIAirplaneLabel::UpdateXFont()
{
    QMetaObject::invokeMethod(this, "adjustGrid", Qt::QueuedConnection);
}

void CGUIAirplaneLabel::SI()
{
    UpdateFormularType();
}

void CGUIAirplaneLabel::UpdateFormularType()
{
    SetGridLabel(XGridLabel());
    if (m_itemCODE || m_groundItemACID)
    {
        loadTypeGrid(m_bFullShow);
        updateGrid();
    }
}

void CGUIAirplaneLabel::setLabelColors(XGridLabel::LabelType label_type)
{
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();
    m_GridLabel.SetType(label_type);

    switch (label_type) {
    case XGridLabel::Controlled:
    case XGridLabel::ControlledShort:
    m_GridLabel.SetForeground(stg.GetColor(UserProfile::CtControlled));
    m_GridLabel.SetBackground(stg.GetColor(UserProfile::CtControlled_back));
    m_colorForeground = stg.GetColor(UserProfile::CtControlled);
    m_colorBackground = stg.GetColor(UserProfile::CtControlled_back);
    break;

    case XGridLabel::ControlledSelected:
//    m_GridLabel.SetForeground(stg.GetColor(UserProfile::CtControlledSelected));
//    m_GridLabel.SetBackground(stg.GetColor(UserProfile::CtControlledSelected_back));
//    m_colorForeground = stg.GetColor(UserProfile::CtControlledSelected);
//    m_colorBackground = stg.GetColor(UserProfile::CtControlledSelected_back);
      m_GridLabel.SetForeground(stg.GetColor(UserProfile::CtControlled));
      m_GridLabel.SetBackground(stg.GetColor(UserProfile::CtControlled_back));
      m_colorForeground = stg.GetColor(UserProfile::CtControlled);
      m_colorBackground = stg.GetColor(UserProfile::CtControlled_back);
    break;

    case XGridLabel::In:
    case XGridLabel::InShort:
    m_GridLabel.SetForeground(stg.GetColor(UserProfile::CtIn));
    m_GridLabel.SetBackground(stg.GetColor(UserProfile::CtIn_back));
    m_colorForeground = stg.GetColor(UserProfile::CtIn);
    m_colorBackground = stg.GetColor(UserProfile::CtIn_back);
    break;

    case XGridLabel::InSelected:
//    m_GridLabel.SetForeground(stg.GetColor(UserProfile::CtInSelected));
//    m_GridLabel.SetBackground(stg.GetColor(UserProfile::CtInSelected_back));
//    m_colorForeground = stg.GetColor(UserProfile::CtInSelected);
//    m_colorBackground = stg.GetColor(UserProfile::CtInSelected_back);
        m_GridLabel.SetForeground(stg.GetColor(UserProfile::CtIn));
        m_GridLabel.SetBackground(stg.GetColor(UserProfile::CtIn_back));
        m_colorForeground = stg.GetColor(UserProfile::CtIn);
        m_colorBackground = stg.GetColor(UserProfile::CtIn_back);
    break;

    case XGridLabel::Related:
    case XGridLabel::RelatedShort:
    m_GridLabel.SetForeground(stg.GetColor(UserProfile::CtRelated));
    m_GridLabel.SetBackground(stg.GetColor(UserProfile::CtRelated_back));
    m_colorForeground = stg.GetColor(UserProfile::CtRelated);
    m_colorBackground = stg.GetColor(UserProfile::CtRelated_back);
    break;

    case XGridLabel::RelatedSelected:
//    m_GridLabel.SetForeground(stg.GetColor(UserProfile::CtRelatedSelected));
//    m_GridLabel.SetBackground(stg.GetColor(UserProfile::CtRelatedSelected_back));
//    m_colorForeground = stg.GetColor(UserProfile::CtRelatedSelected);
//    m_colorBackground = stg.GetColor(UserProfile::CtRelatedSelected_back);
        m_GridLabel.SetForeground(stg.GetColor(UserProfile::CtRelated));
        m_GridLabel.SetBackground(stg.GetColor(UserProfile::CtRelated_back));
        m_colorForeground = stg.GetColor(UserProfile::CtRelated);
        m_colorBackground = stg.GetColor(UserProfile::CtRelated_back);
    break;

    case XGridLabel::Unrelated:
    case XGridLabel::UnrelatedShort:
    m_GridLabel.SetForeground(stg.GetColor(UserProfile::CtUnrelated));
    m_GridLabel.SetBackground(stg.GetColor(UserProfile::CtUnrelated_back));
    m_colorForeground = stg.GetColor(UserProfile::CtUnrelated);
    m_colorBackground = stg.GetColor(UserProfile::CtUnrelated_back);

    break;

    case XGridLabel::UnrelatedSelected:
//    m_GridLabel.SetForeground(stg.GetColor(UserProfile::CtUnrelatedSelected));
//    m_GridLabel.SetBackground(stg.GetColor(UserProfile::CtUnrelatedSelected_back));
//    m_colorForeground = stg.GetColor(UserProfile::CtUnrelatedSelected);
//    m_colorBackground = stg.GetColor(UserProfile::CtUnrelatedSelected_back);
        m_GridLabel.SetForeground(stg.GetColor(UserProfile::CtUnrelated));
        m_GridLabel.SetBackground(stg.GetColor(UserProfile::CtUnrelated_back));
        m_colorForeground = stg.GetColor(UserProfile::CtUnrelated);
        m_colorBackground = stg.GetColor(UserProfile::CtUnrelated_back);
    break;

    default:
    break;
    }

}

void CGUIAirplaneLabel::onSpiTimeout()
{
    spi_timer_->stop();
    loadTypeGrid(m_bFullShow);
}

void CGUIAirplaneLabel::onEmgTimeout()
{
    emg_timer_->stop();
     loadTypeGrid(m_bFullShow);
}

bool CGUIAirplaneLabel::isSpecSignalExist() const
{
    bool is_spi = m_pAirplane->GetSPI();
    if (is_spi)
        spi_timer_->start();
    else if (spi_timer_->isActive())
        is_spi = true;
    bool is_restriction_warning = !m_pAirplane->restrictionWarning().isEmpty();
    bool is_meteo_warning = !m_pAirplane->meteoWarning().isEmpty();
    bool is_emg = m_pAirplane->lastEmg().isValid() && (m_pAirplane->lastEmg().msecsTo(QDateTime::currentDateTimeUtc()) <= 8000);
    return (CAirplane::sListSpecCodes.contains(m_pAirplane->GetMode3AReplyToString()))
            || is_spi || is_restriction_warning || is_meteo_warning || is_emg || m_pAirplane->dbl();
}

void CGUIAirplaneLabel::fillLabelFields(QGraphicsLinearLayout *grid, XGridLabel::LabelType label_type)
{
    int row = 0;
    int preferredWidth = 0;
    QString sHDG, sSPD, sAFL, sTEND;
    int count_grid = grid->count();

    if( count_grid == 0)
    {
        linear0 = new QGraphicsLinearLayout(Qt::Orientation::Horizontal);
        linear1 = new QGraphicsLinearLayout(Qt::Orientation::Horizontal);
        linear1_1 = new QGraphicsLinearLayout(Qt::Orientation::Horizontal);
        linear2 = new QGraphicsLinearLayout(Qt::Orientation::Horizontal);
        linear3 = new QGraphicsLinearLayout(Qt::Orientation::Horizontal);
        linear4 = new QGraphicsLinearLayout(Qt::Orientation::Horizontal);
        linear5 = new QGraphicsLinearLayout(Qt::Orientation::Horizontal);

        grid->insertItem(0, linear0);
        grid->setItemSpacing(0, 0.5);
        grid->insertItem(1, linear1);
        grid->setItemSpacing(1, 0.5);
        grid->insertItem(2, linear1_1);
        grid->setItemSpacing(2, 0.5);
        grid->insertItem(3, linear2);
        grid->setItemSpacing(3, 0.5);
        grid->insertItem(4, linear3);
        grid->setItemSpacing(4, 0.5);
        grid->insertItem(5, linear4);
        grid->setItemSpacing(5, 0.5);
        grid->insertItem(6, linear5);
        grid->setItemSpacing(6, 0.5);
    }
    else
    {
        linear0 = dynamic_cast<QGraphicsLinearLayout*>(grid->itemAt(0));
        linear1 = dynamic_cast<QGraphicsLinearLayout*>(grid->itemAt(1));
        linear1_1 = dynamic_cast<QGraphicsLinearLayout*>(grid->itemAt(2));
        linear2 = dynamic_cast<QGraphicsLinearLayout*>(grid->itemAt(3));
        linear3 = dynamic_cast<QGraphicsLinearLayout*>(grid->itemAt(4));
        linear4 = dynamic_cast<QGraphicsLinearLayout*>(grid->itemAt(5));
        linear5 = dynamic_cast<QGraphicsLinearLayout*>(grid->itemAt(6));
    }
    int lenM = 0;
    QFont font = this->font();
    QFontMetrics fm(font);
    //QFont font("times", 24);
    QString sACID = m_itemACID->CGUILabelText::text();
    QString sItemCS = m_itemCS->CGUILabelText::text();
    QString sItemCODE = m_itemCODE->CGUILabelText::text();
    QString sItemAFL = m_itemAFL->CGUILabelText::text();
    QString sItemTEND = m_itemTEND->CGUILabelText::text();
    QString sItemCFL = m_itemCFL->CGUILabelText::text();
    QString sItem_gsp_ = item_gsp_->CGUILabelText::text();
    QString sItem_w_ = item_w_->CGUILabelText::text();
    QString sItemTFL = m_itemTFL->CGUILabelText::text();
    QString sItemNextPoint = m_itemNextPoint->CGUILabelText::text();
    QString sItemHDG = m_itemHDG->CGUILabelText::text();
    QString sItem_spd_ = item_spd_->CGUILabelText::text();
    QString sItemVRC = m_itemVRC->CGUILabelText::text();
    QString sItemLM = m_itemLM->CGUILabelText::text();
    QString sMhdg = m_Mhdg->CGUILabelText::text();
    QString sItemType = m_itemType->CGUILabelText::text();

#ifdef QT_DEBUG
    QString sItem_cs_debug_ = item_cs_debug_->CGUILabelText::text();
    QString sItem_ns_debug_ = item_ns_debug_->CGUILabelText::text();
    linear0->insertItem(0, item_cs_debug_);
    linear0->setAlignment(item_cs_debug_, Qt::AlignLeft);
    preferredWidth = item_cs_debug_->preferredWidth();
    lenM = fm.width(sItem_cs_debug_);
    item_cs_debug_->setMaximumWidth(lenM);
    linear0->setItemSpacing(0, 5);

    linear0->insertItem(1, item_ns_debug_);
    linear0->setAlignment(item_ns_debug_, Qt::AlignLeft);
    preferredWidth = item_ns_debug_->preferredWidth();
    lenM = fm.width(sItem_ns_debug_);
    item_ns_debug_->setMaximumWidth(lenM);
    linear0->setItemSpacing(1, 5);

    ++row;
#endif // QT_DEBUG

    if (CAirplane::sListSpecCodes.contains(m_pAirplane->GetMode3AReplyToString())) {
        if((m_pAirplane->GetSPI() || spi_timer_->isActive()) ||
                m_pAirplane->isSTCA() ||
                !m_pAirplane->meteoWarning().isEmpty() ||
                !m_pAirplane->restrictionWarning().isEmpty())
            item_spec_signal_->setText("*" + m_pAirplane->GetMode3AReplyToString());
        else item_spec_signal_->setText(m_pAirplane->GetMode3AReplyToString());
        item_spec_signal_->setVisible(true);
        item_spec_signal_->SetVisible(true);
        linear1->insertItem(0, item_spec_signal_);
        ++row;

    } else if (m_pAirplane->GetSPI() || spi_timer_->isActive()) {
        if((m_pAirplane->dbl() && m_pAirplane->GetMode3AReplyToString() != "7000" && m_pAirplane->GetMode3AReplyToString() != "2000") ||
                (m_pAirplane->isSTCA()) ||
                (!m_pAirplane->meteoWarning().isEmpty()) ||
                (!m_pAirplane->restrictionWarning().isEmpty()))
            item_spec_signal_->setText("*SPI");
        else item_spec_signal_->setText("SPI");
        item_spec_signal_->setVisible(true);
        item_spec_signal_->SetVisible(true);
        linear1->insertItem(0, item_spec_signal_);
        ++row;
    } else if (m_pAirplane->dbl() && m_pAirplane->GetMode3AReplyToString() != "7000" && m_pAirplane->GetMode3AReplyToString() != "2000") {
        if((m_pAirplane->isSTCA()) ||
                (!m_pAirplane->meteoWarning().isEmpty()) ||
                (!m_pAirplane->restrictionWarning().isEmpty()))
            item_spec_signal_->setText("*DBL " + m_pAirplane->GetMode3AReplyToString());
        else item_spec_signal_->setText("DBL " + m_pAirplane->GetMode3AReplyToString());
        item_spec_signal_->setVisible(true);
        item_spec_signal_->SetVisible(true);
        linear1->insertItem(0, item_spec_signal_);

        ++row;
    } else if (m_pAirplane->isSTCA()) {
        if((!m_pAirplane->meteoWarning().isEmpty()) ||
            (!m_pAirplane->restrictionWarning().isEmpty()))
            item_spec_signal_->setText("*STCA");
        else item_spec_signal_->setText("STCA");
        item_spec_signal_->setVisible(true);
        item_spec_signal_->SetVisible(true);
        linear1->insertItem(0, item_spec_signal_);

        ++row;
    } else if (!m_pAirplane->meteoWarning().isEmpty()) {
        if(!m_pAirplane->restrictionWarning().isEmpty())
            item_spec_signal_->setText("*" + m_pAirplane->meteoWarning());
        else item_spec_signal_->setText(m_pAirplane->meteoWarning());
        item_spec_signal_->setVisible(true);
        item_spec_signal_->SetVisible(true);
        linear1->insertItem(0, item_spec_signal_);

        ++row;
    } else if (!m_pAirplane->restrictionWarning().isEmpty()) {
        item_spec_signal_->setText(m_pAirplane->restrictionWarning());
        item_spec_signal_->setVisible(true);
        item_spec_signal_->SetVisible(true);
        linear1->insertItem(0, item_spec_signal_);

        ++row;
    } else {
        item_spec_signal_->setVisible(false);
        item_spec_signal_->SetVisible(false);

        ++row;
    }

    const int emgTimeMs = m_pAirplane->lastEmg().msecsTo(QDateTime::currentDateTimeUtc());
    if (m_pAirplane->lastEmg().isValid() && (emgTimeMs <= 8000)) {
        item_spec_signal_->setText("EMG");
        linear1->insertItem(0, item_spec_signal_);
        item_spec_signal_->setVisible(true);
        item_spec_signal_->SetVisible(true);

        ++row;
        emg_timer_->stop();
        emg_timer_->start(8000 - emgTimeMs);
    }

    const CAirplaneData::PlaneType plane_type = m_pAirplane->GetPlaneType();
    if (CAirplaneData::Transfer == plane_type) {
        const QSharedPointer<CSFPL> &sfpl = m_pAirplane->GetSFPL();
        if (sfpl && sfpl->GetNextSector().isEmpty())
        {
            linear1_1->insertItem(0, item_act_out_);
        }
        else
        {
            linear1_1->insertItem(0, item_transfer_);
        }

        ++row;
    } else if (CAirplaneData::ActOut == plane_type) {
        linear1_1->insertItem(0, item_transfer_);
        ++row;
    }

    if(linear0->count() == 0)
    {
        linear0->setMaximumHeight(0.001);
        linear0->setMinimumHeight(0.001);
    }
    if(linear1->count() == 0)
    {
        linear1->setMaximumHeight(0.001);
        linear1->setMinimumHeight(0.001);
    }
    if(linear1_1->count() == 0)
    {
        linear1_1->setMaximumHeight(0.001);
        linear1_1->setMinimumHeight(0.001);
    }

    if(!(m_pAirplane->isInAirport() || m_pAirplane->isOutAirport()))
    {
        linear2->insertItem(0, m_itemACID);
        linear2->setAlignment(m_itemACID, Qt::AlignLeft);
        preferredWidth = m_itemACID->preferredWidth();
        lenM = fm.width(sACID);
        m_itemACID->setMaximumWidth(lenM);
        linear2->setItemSpacing(0, 5);

        switch (label_type) {
        case XGridLabel::Controlled:
        {
            const QSharedPointer<CSFPL> &sfpl = m_pAirplane->GetSFPL();
            if (!sfpl)
            {
                linear2->insertItem(2, m_itemCS);
                linear2->setAlignment(m_itemCS, Qt::AlignLeft);
                preferredWidth = m_itemCS->preferredWidth();
                lenM = fm.width(sItemCS);
                m_itemCS->setMaximumWidth(lenM);
                linear2->setItemSpacing(2, 5);
            }
        }
            break;
        case XGridLabel::ControlledSelected:
            linear2->insertItem(2, m_itemCODE);
            linear2->setAlignment(m_itemCODE, Qt::AlignLeft);
            preferredWidth = m_itemCODE->preferredWidth();
            lenM = fm.width(sItemCODE);
            m_itemCODE->setMaximumWidth(lenM);
            linear2->setItemSpacing(2, 5);

            linear2->insertItem(3, m_itemCS);
            linear2->setAlignment(m_itemCS, Qt::AlignLeft);
            preferredWidth = m_itemCS->preferredWidth();
            lenM = fm.width(sItemCS);
            m_itemCS->setMaximumWidth(lenM);
            linear2->setItemSpacing(3, 5);
            break;
        case XGridLabel::In:
            linear2->insertItem(2, m_itemCS);
            linear2->setAlignment(m_itemCS, Qt::AlignLeft);
            preferredWidth = m_itemCS->preferredWidth();
            lenM = fm.width(sItemCS);
            m_itemCS->setMaximumWidth(lenM);
            linear2->setItemSpacing(2, 5);
            break;
        case XGridLabel::InSelected:
            linear2->insertItem(2, m_itemCODE);
            linear2->setAlignment(m_itemCODE, Qt::AlignLeft);
            preferredWidth = m_itemCODE->preferredWidth();
            lenM = fm.width(sItemCODE);
            m_itemCODE->setMaximumWidth(lenM);
            linear2->setItemSpacing(2, 5);

            linear2->insertItem(3, m_itemCS);
            linear2->setAlignment(m_itemCS, Qt::AlignLeft);
            preferredWidth = m_itemCS->preferredWidth();
            lenM = fm.width(sItemCS);
            m_itemCS->setMaximumWidth(lenM);
            linear2->setItemSpacing(3, 5);
            break;
            break;
        case XGridLabel::Related:
        {
            const QSharedPointer<CSFPL> &sfpl = m_pAirplane->GetSFPL();
            if (!sfpl)
            {
                linear2->insertItem(2, m_itemCS);
                linear2->setAlignment(m_itemCS, Qt::AlignLeft);
                preferredWidth = m_itemCS->preferredWidth();
                lenM = fm.width(sItemCS);
                m_itemCS->setMaximumWidth(lenM);
                linear2->setItemSpacing(2, 5);
            }
        }
            break;///
        case XGridLabel::RelatedSelected:
            linear2->insertItem(2, m_itemCODE);
            linear2->setAlignment(m_itemCODE, Qt::AlignLeft);
            preferredWidth = m_itemCODE->preferredWidth();
            lenM = fm.width(sItemCODE);
            m_itemCODE->setMaximumWidth(lenM);
            linear2->setItemSpacing(2, 5);

            linear2->insertItem(3, m_itemCS);
            linear2->setAlignment(m_itemCS, Qt::AlignLeft);
            preferredWidth = m_itemCS->preferredWidth();
            lenM = fm.width(sItemCS);
            m_itemCS->setMaximumWidth(lenM);
            linear2->setItemSpacing(3, 5);
            break;
        case XGridLabel::Unrelated:
            linear2->insertItem(2, m_itemCODE);
            linear2->setAlignment(m_itemCODE, Qt::AlignLeft);
            preferredWidth = m_itemCODE->preferredWidth();
            lenM = fm.width(sItemCODE);
            m_itemCODE->setMaximumWidth(lenM);
            linear2->setItemSpacing(2, 5);

            linear2->insertItem(3, m_itemCS);
            linear2->setAlignment(m_itemCS, Qt::AlignLeft);
            preferredWidth = m_itemCS->preferredWidth();
            lenM = fm.width(sItemCS);
            m_itemCS->setMaximumWidth(lenM);
            linear2->setItemSpacing(3, 5);
            break;
        case XGridLabel::UnrelatedShort:
            linear2->insertItem(2, m_itemCS);
            linear2->setAlignment(m_itemCS, Qt::AlignLeft);
            preferredWidth = m_itemCS->preferredWidth();
            lenM = fm.width(sItemCS);
            m_itemCS->setMaximumWidth(lenM);
            linear2->setItemSpacing(2, 5);
            break;
        case XGridLabel::UnrelatedSelected:

        default:
            break;
        }

        ++row;

        linear3->insertItem(0, m_itemAFL);
        linear3->setAlignment(m_itemAFL, Qt::AlignLeft);
        preferredWidth = m_itemAFL->preferredWidth();
        lenM = fm.width(sItemAFL);
        m_itemAFL->setMaximumWidth(lenM);
        sAFL = m_itemAFL->CGUILabelText::text();
        /*if(sAFL.size() == 4 )
            linear3->setItemSpacing(0, 12);
        else
            if(sAFL.size() == 5 )
                linear3->setItemSpacing(0, 18);
            else*/
                linear3->setItemSpacing(0, 5);

        linear3->insertItem(1, m_itemTEND);
        linear3->setAlignment(m_itemTEND, Qt::AlignLeft);
        preferredWidth = m_itemTEND->preferredWidth();
        lenM = fm.width(sItemTEND);
        m_itemTEND->setMaximumWidth(lenM);
        sTEND = m_itemTEND->CGUILabelText::text();
        if(sTEND == " ")
            linear3->setItemSpacing(1, 5);
        else
            linear3->setItemSpacing(1, 7);

        linear3->insertItem(2, m_itemCFL);
        linear3->setAlignment(m_itemCFL, Qt::AlignLeft);
        preferredWidth = m_itemCFL->preferredWidth();
        lenM = fm.width(sItemCFL);
        m_itemCFL->setMaximumWidth(lenM);
        linear3->setItemSpacing(2, 5);

        switch (label_type) {
        case XGridLabel::ControlledSelected:
        case XGridLabel::InSelected:
        case XGridLabel::RelatedSelected:
            if(m_bGSPorIAS){
                linear3->insertItem(3, item_gsp_);
                linear3->setAlignment(item_gsp_, Qt::AlignLeft);
                preferredWidth = item_gsp_->preferredWidth();
                lenM = fm.width(sItem_gsp_);
                item_gsp_->setMaximumWidth(lenM);
                linear3->setItemSpacing(3, 5);
                item_gsp_->setBackgroundColor(QColor());
            }
            else {
                linear3->insertItem(3, item_w_);
                linear3->setAlignment(item_w_, Qt::AlignLeft);
                lenM = fm.width(sItem_w_);
                item_w_->setMaximumWidth(lenM);
                linear3->setItemSpacing(3, 5);
                item_w_->setBackgroundColor(Qt::white);
            }
            break;
        case XGridLabel::UnrelatedSelected:
        case XGridLabel::Unrelated:
            if(m_bGSPorIAS){
                linear3->insertItem(3, item_gsp_);
                linear3->setAlignment(item_gsp_, Qt::AlignLeft);
                preferredWidth = item_gsp_->preferredWidth();
                lenM = fm.width(sItem_gsp_);
                item_gsp_->setMaximumWidth(lenM);
                linear3->setItemSpacing(3, 5);
                item_gsp_->setBackgroundColor(QColor());
            }
            else {
                linear3->insertItem(3, item_w_);
                linear3->setAlignment(item_w_, Qt::AlignLeft);
                preferredWidth = item_w_->preferredWidth();
                lenM = fm.width(sItem_w_);
                item_w_->setMaximumWidth(lenM);
                linear3->setItemSpacing(3, 5);
                item_w_->setBackgroundColor(Qt::white);
            }
            break;
        case XGridLabel::UnrelatedShort:
            break;
        default:
            break;
        }

        ++row;

        linear4->insertItem(0, m_itemTFL);
        linear4->setAlignment(m_itemTFL, Qt::AlignLeft);
        preferredWidth = m_itemTFL->preferredWidth();
        lenM = fm.width(sItemTFL);
        m_itemTFL->setMaximumWidth(lenM);
        linear4->setItemSpacing(0, 5);
        switch (label_type) {
        case XGridLabel::ControlledSelected:
            linear4->insertItem(1, m_itemNextPoint);
            linear4->setAlignment(m_itemNextPoint, Qt::AlignLeft);
            preferredWidth = m_itemNextPoint->preferredWidth();
            lenM = fm.width(sItemNextPoint);
            m_itemNextPoint->setMaximumWidth(lenM);
            linear4->setItemSpacing(1, 5);
            break;
        case XGridLabel::In:
            linear4->insertItem(1, m_itemNextPoint);
            linear4->setAlignment(m_itemNextPoint, Qt::AlignLeft);
            preferredWidth = m_itemNextPoint->preferredWidth();
            lenM = fm.width(sItemNextPoint);
            m_itemNextPoint->setMaximumWidth(lenM);
            linear4->setItemSpacing(1, 5);
            break;
        case XGridLabel::InSelected:
            linear4->insertItem(1, m_itemNextPoint);
            linear4->setAlignment(m_itemNextPoint, Qt::AlignLeft);
            preferredWidth = m_itemNextPoint->preferredWidth();
            lenM = fm.width(sItemNextPoint);
            m_itemNextPoint->setMaximumWidth(lenM);
            linear4->setItemSpacing(1, 5);
            break;
        case XGridLabel::Related:
            linear4->insertItem(1, m_itemNextPoint);
            linear4->setAlignment(m_itemNextPoint, Qt::AlignLeft);
            preferredWidth = m_itemNextPoint->preferredWidth();
            lenM = fm.width(sItemNextPoint);
            m_itemNextPoint->setMaximumWidth(lenM);
            linear4->setItemSpacing(1, 5);
            break;
        case XGridLabel::RelatedSelected:
            linear4->insertItem(1, m_itemNextPoint);
            linear4->setAlignment(m_itemNextPoint, Qt::AlignLeft);
            preferredWidth = m_itemNextPoint->preferredWidth();
            lenM = fm.width(sItemNextPoint);
            m_itemNextPoint->setMaximumWidth(lenM);
            linear4->setItemSpacing(1, 5);
            break;
        case XGridLabel::UnrelatedSelected:
        case XGridLabel::Unrelated:
            linear4->insertItem(2, m_itemNextPoint);
            linear4->setAlignment(m_itemNextPoint, Qt::AlignLeft);
            preferredWidth = m_itemNextPoint->preferredWidth();
            lenM = fm.width(sItemNextPoint);
            m_itemNextPoint->setMaximumWidth(lenM);
            linear4->setItemSpacing(2, 5);
            break;

        default:
            break;
        }

        ++row;
        QString sSPD;

        switch (label_type) {
        case XGridLabel::ControlledSelected:
            linear5->insertItem(0, m_itemHDG);
            linear5->setAlignment(m_itemHDG, Qt::AlignLeft);
            preferredWidth = m_itemHDG->preferredWidth();
            lenM = fm.width(sItemHDG);
            m_itemHDG->setMaximumWidth(lenM);
            sHDG = m_itemHDG->CGUILabelText::text();
            if(sHDG.size() == 3)
                linear5->setItemSpacing(0, 5);
            else
                linear5->setItemSpacing(0, 10);

            sSPD = item_spd_->CGUILabelText::text();
            linear5->insertItem(1, item_spd_);
            linear5->setAlignment(item_spd_, Qt::AlignLeft);
            preferredWidth = item_spd_->preferredWidth();
            lenM = fm.width(sItem_spd_);
            item_spd_->setMaximumWidth(lenM);
            sSPD = item_spd_->CGUILabelText::text();
            if(sSPD.size() == 3)
                linear5->setItemSpacing(1, 5);
            else
                linear5->setItemSpacing(1, 10);

            linear5->insertItem(2, m_itemVRC);
            linear5->setAlignment(m_itemVRC, Qt::AlignLeft);
            preferredWidth = m_itemVRC->preferredWidth();
            lenM = fm.width(sItemVRC);
            m_itemVRC->setMaximumWidth(lenM);
            linear5->setItemSpacing(2, 5);
            break;
        case XGridLabel::InSelected:
            linear5->insertItem(0, m_itemHDG);
            linear5->setAlignment(m_itemHDG, Qt::AlignLeft);
            preferredWidth = m_itemHDG->preferredWidth();
            lenM = fm.width(sItemHDG);
            m_itemHDG->setMaximumWidth(lenM);
            sHDG = m_itemHDG->CGUILabelText::text();
            if(sHDG.size() == 3)
                linear5->setItemSpacing(0, 5);
            else
                linear5->setItemSpacing(0, 10);

            linear5->insertItem(1, item_spd_);
            linear5->setAlignment(item_spd_, Qt::AlignLeft);
            preferredWidth = item_spd_->preferredWidth();
            lenM = fm.width(sItem_spd_);
            item_spd_->setMaximumWidth(lenM);
            sSPD = item_spd_->CGUILabelText::text();
            if(sSPD.size() == 3)
                linear5->setItemSpacing(1, 5);
            else
                linear5->setItemSpacing(1, 10);

            linear5->insertItem(2, m_itemVRC);
            linear5->setAlignment(m_itemVRC, Qt::AlignLeft);
            preferredWidth = m_itemVRC->preferredWidth();
            lenM = fm.width(sItemVRC);
            m_itemVRC->setMaximumWidth(lenM);
            linear5->setItemSpacing(2, 5);
            break;
        case XGridLabel::RelatedSelected:
            linear5->insertItem(0, m_itemHDG);
            linear5->setAlignment(m_itemHDG, Qt::AlignLeft);
            preferredWidth = m_itemHDG->preferredWidth();
            lenM = fm.width(sItemHDG);
            m_itemHDG->setMaximumWidth(lenM);
            sHDG = m_itemHDG->CGUILabelText::text();
            if(sHDG.size() == 3)
                linear5->setItemSpacing(0, 5);
            else
                linear5->setItemSpacing(0, 10);

            linear5->insertItem(1, item_spd_);
            linear5->setAlignment(item_spd_, Qt::AlignLeft);
            preferredWidth = item_spd_->preferredWidth();
            lenM = fm.width(sItem_spd_);
            item_spd_->setMaximumWidth(lenM);
            sSPD = item_spd_->CGUILabelText::text();
            if(sSPD.size() == 3)
                linear5->setItemSpacing(1, 5);
            else
                linear5->setItemSpacing(1, 10);

            linear5->insertItem(2, m_itemVRC);
            linear5->setAlignment(m_itemVRC, Qt::AlignLeft);
            preferredWidth = m_itemVRC->preferredWidth();
            lenM = fm.width(sItemVRC);
            m_itemVRC->setMaximumWidth(lenM);
            linear5->setItemSpacing(2, 5);
            break;
        case XGridLabel::UnrelatedSelected:
        case XGridLabel::Unrelated:
            linear5->insertItem(0, m_itemHDG);
            linear5->setAlignment(m_itemHDG, Qt::AlignLeft);
            preferredWidth = m_itemHDG->preferredWidth();
            lenM = fm.width(sItemHDG);
            m_itemHDG->setMaximumWidth(lenM);
            sHDG = m_itemHDG->CGUILabelText::text();
            if(sHDG.size() == 3)
                linear5->setItemSpacing(0, 5);
            else
                linear5->setItemSpacing(0, 10);

            linear5->insertItem(1, item_spd_);
            linear5->setAlignment(item_spd_, Qt::AlignLeft);
            preferredWidth = item_spd_->preferredWidth();
            lenM = fm.width(sItem_spd_);
            item_spd_->setMaximumWidth(lenM);
            sSPD = item_spd_->CGUILabelText::text();
            if(sSPD.size() == 3)
                linear5->setItemSpacing(1, 5);
            else
                linear5->setItemSpacing(1, 10);

            linear5->insertItem(2, m_itemVRC);
            linear5->setAlignment(m_itemVRC, Qt::AlignLeft);
            preferredWidth = m_itemVRC->preferredWidth();
            lenM = fm.width(sItemVRC);
            m_itemVRC->setMaximumWidth(lenM);
            linear5->setItemSpacing(2, 5);
            break;
        default:
            break;
        }

    }
    else {
        if(m_pAirplane->isOutAirport()) {
            linear2->insertItem(0, m_itemACID);
            linear2->setAlignment(m_itemACID, Qt::AlignLeft);
            preferredWidth = m_itemACID->preferredWidth();
            lenM = fm.width(sACID);
            m_itemACID->setMaximumWidth(lenM);
            linear2->setItemSpacing(0, 5);

            if(m_itemLM->text() == "  ") {
                switch (label_type) {
                case XGridLabel::Controlled:
                {
                    const QSharedPointer<CSFPL> &sfpl = m_pAirplane->GetSFPL();
                    if (!sfpl)
                    {
                        linear2->insertItem(2, m_itemCS);
                    }
                }
                    break;
                case XGridLabel::ControlledSelected:
                    linear2->insertItem(2, m_itemCS);
                    linear2->setAlignment(m_itemCS, Qt::AlignLeft);
                    preferredWidth = m_itemCS->preferredWidth();
                    lenM = fm.width(sItemCS);
                    m_itemCS->setMaximumWidth(lenM);
                    linear2->setItemSpacing(2, 5);
                    break;
                case XGridLabel::InSelected:
                    linear2->insertItem(2, m_itemCS);
                    linear2->setAlignment(m_itemCS, Qt::AlignLeft);
                    preferredWidth = m_itemCS->preferredWidth();
                    lenM = fm.width(sItemCS);
                    m_itemCS->setMaximumWidth(lenM);
                    linear2->setItemSpacing(2, 5);
                    break;
                case XGridLabel::In:
                    linear2->insertItem(2, m_itemCS);
                    linear2->setAlignment(m_itemCS, Qt::AlignLeft);
                    preferredWidth = m_itemCS->preferredWidth();
                    lenM = fm.width(sItemCS);
                    m_itemCS->setMaximumWidth(lenM);
                    linear2->setItemSpacing(2, 5);
                    break;
                case XGridLabel::RelatedSelected:
                    linear2->insertItem(2, m_itemCS);
                    linear2->setAlignment(m_itemCS, Qt::AlignLeft);
                    preferredWidth = m_itemCS->preferredWidth();
                    lenM = fm.width(sItemCS);
                    m_itemCS->setMaximumWidth(lenM);
                    linear2->setItemSpacing(2, 5);
                    break;
                case XGridLabel::Related:
                {
                    const QSharedPointer<CSFPL> &sfpl = m_pAirplane->GetSFPL();
                    if (!sfpl)
                    {
                        linear2->insertItem(2, m_itemCS);
                        linear2->setAlignment(m_itemCS, Qt::AlignLeft);
                        preferredWidth = m_itemCS->preferredWidth();
                        lenM = fm.width(sItemCS);
                        m_itemCS->setMaximumWidth(lenM);
                        linear2->setItemSpacing(2, 5);
                    }
                }
                    break;
                case XGridLabel::Unrelated:
                    linear2->insertItem(2, m_itemCS);
                    linear2->setAlignment(m_itemCS, Qt::AlignLeft);
                    preferredWidth = m_itemCS->preferredWidth();
                    lenM = fm.width(sItemCS);
                    m_itemCS->setMaximumWidth(lenM);
                    linear2->setItemSpacing(2, 5);
                    break;
                case XGridLabel::UnrelatedShort:
                    linear2->insertItem(2, m_itemCS);
                    linear2->setAlignment(m_itemCS, Qt::AlignLeft);
                    preferredWidth = m_itemCS->preferredWidth();
                    lenM = fm.width(sItemCS);
                    m_itemCS->setMaximumWidth(lenM);
                    linear2->setItemSpacing(2, 5);
                    break;
                default:
                    break;
                }
            }
            else {
                linear2->insertItem(2, m_itemLM);
                linear2->setAlignment(m_itemLM, Qt::AlignLeft);
                preferredWidth = m_itemLM->preferredWidth();
                lenM = fm.width(sItemLM);
                m_itemLM->setMaximumWidth(lenM);
                linear2->setItemSpacing(2, 5);
                switch (label_type) {
                case XGridLabel::Controlled:
                {
                    const QSharedPointer<CSFPL> &sfpl = m_pAirplane->GetSFPL();
                    if (!sfpl)
                    {
                        linear2->insertItem(3, m_itemCS);
                    }
                }
                    break;
                case XGridLabel::ControlledSelected:
                    linear2->insertItem(3, m_itemCS);
                    break;
                case XGridLabel::InSelected:
                    linear2->insertItem(3, m_itemCS);
                    break;
                case XGridLabel::In:
                    linear2->insertItem(3, m_itemCS);
                    break;
                case XGridLabel::RelatedSelected:
                    linear2->insertItem(3, m_itemCS);
                    break;
                case XGridLabel::Related:
                {
                    const QSharedPointer<CSFPL> &sfpl = m_pAirplane->GetSFPL();
                    if (!sfpl)
                    {
                        linear2->insertItem(3, m_itemCS);
                    }
                }
                    break;
                case XGridLabel::Unrelated:
                    linear2->insertItem(3, m_itemCS);
                    break;
                case XGridLabel::UnrelatedShort:
                    linear2->insertItem(3, m_itemCS);
                    break;
                default:
                    break;
                }
            }

            ++row;

            linear3->insertItem(0, m_itemAFL);
            linear3->setAlignment(m_itemAFL, Qt::AlignLeft);
            preferredWidth = m_itemAFL->preferredWidth();
            lenM = fm.width(sItemAFL);
            m_itemAFL->setMaximumWidth(lenM);
            sAFL = m_itemAFL->CGUILabelText::text();
            /*if(sAFL.size() == 4 )
                linear3->setItemSpacing(0, 12);
            else
                if(sAFL.size() == 5 )
                    linear3->setItemSpacing(0, 18);
                else*/
                    linear3->setItemSpacing(0, 5);

            linear3->insertItem(1, m_itemTEND);
            linear3->setAlignment(m_itemTEND, Qt::AlignLeft);
            preferredWidth = m_itemTEND->preferredWidth();
            lenM = fm.width(sItemTEND);
            m_itemTEND->setMaximumWidth(lenM);
            sTEND = m_itemTEND->CGUILabelText::text();
            if(sTEND == " ")
                linear3->setItemSpacing(1, 5);
            else
                linear3->setItemSpacing(1, 7);

            linear3->insertItem(2, m_itemCFL);
            linear3->setAlignment(m_itemCFL, Qt::AlignLeft);
            preferredWidth = m_itemCFL->preferredWidth();
            lenM = fm.width(sItemCFL);
            m_itemCFL->setMaximumWidth(lenM);
            linear3->setItemSpacing(2, 5);

            switch (label_type) {
            case XGridLabel::Controlled:
                break;
            case XGridLabel::ControlledSelected:
                if(m_bGSPorIAS){
                    linear3->insertItem(3, item_gsp_);
                    item_gsp_->setBackgroundColor(QColor());
                }
                else {
                    linear3->insertItem(3, item_w_);
                    item_w_->setBackgroundColor(Qt::white);
                }
                break;
            case XGridLabel::InSelected:
                if(m_bGSPorIAS){
                    linear3->insertItem(3, item_gsp_);
                    item_gsp_->setBackgroundColor(QColor());
                }
                else {
                    linear3->insertItem(3, item_w_);
                    item_w_->setBackgroundColor(Qt::white);
                }
                break;
            case XGridLabel::RelatedSelected:
                if(m_bGSPorIAS){
                    linear3->insertItem(3, item_gsp_);
                    item_gsp_->setBackgroundColor(QColor());
                }
                else {
                    linear3->insertItem(3, item_w_);
                    item_w_->setBackgroundColor(Qt::white);
                }
                break;
            case XGridLabel::Unrelated:
                if(m_bGSPorIAS){
                    linear3->insertItem(3, item_gsp_);
                    item_gsp_->setBackgroundColor(QColor());
                }
                else {
                    linear3->insertItem(3, item_w_);
                    item_w_->setBackgroundColor(Qt::white);
                }
                break;
            case XGridLabel::UnrelatedShort:
                break;
            default:
                break;
            }


            ++row;

            linear4->insertItem(0, m_itemTFL);
            if(!m_bNextPointIAS && sItemNextPoint == sMhdg)
            {
                linear4->insertItem(1, m_itemNextPoint);
                m_itemNextPoint->setBackgroundColor(Qt::white);
            }
            else
            {
                linear4->insertItem(1, m_itemNextPoint);
                m_itemNextPoint->setBackgroundColor(QColor());
            }

            ++row;

            switch (label_type) {
            case XGridLabel::Controlled:
                return;
            case XGridLabel::ControlledSelected:
                linear5->insertItem(0, m_itemHDG);
                linear5->setAlignment(m_itemHDG, Qt::AlignLeft);
                preferredWidth = m_itemHDG->preferredWidth();
                lenM = fm.width(sItemHDG);
                m_itemHDG->setMaximumWidth(lenM);
                sHDG = m_itemHDG->CGUILabelText::text();
                if(sHDG.size() == 3)
                    linear5->setItemSpacing(0, 5);
                else
                    linear5->setItemSpacing(0, 10);

                linear5->insertItem(1, item_spd_);
                linear5->setAlignment(item_spd_, Qt::AlignLeft);
                preferredWidth = item_spd_->preferredWidth();
                lenM = fm.width(sItem_spd_);
                item_spd_->setMaximumWidth(lenM);
                sSPD = item_spd_->CGUILabelText::text();
                if(sSPD.size() == 3)
                    linear5->setItemSpacing(1, 5);
                else
                    linear5->setItemSpacing(1, 10);

                linear5->insertItem(2, m_itemVRC);
                linear5->setAlignment(m_itemVRC, Qt::AlignLeft);
                preferredWidth = m_itemVRC->preferredWidth();
                lenM = fm.width(sItemVRC);
                m_itemVRC->setMaximumWidth(lenM);
                linear5->setItemSpacing(2, 5);
                break;
            case XGridLabel::InSelected:
                linear5->insertItem(0, m_itemHDG);
                linear5->setAlignment(m_itemHDG, Qt::AlignLeft);
                preferredWidth = m_itemHDG->preferredWidth();
                lenM = fm.width(sItemHDG);
                m_itemHDG->setMaximumWidth(lenM);
                sHDG = m_itemHDG->CGUILabelText::text();
                if(sHDG.size() == 3)
                    linear5->setItemSpacing(0, 5);
                else
                    linear5->setItemSpacing(0, 10);

                linear5->insertItem(1, item_spd_);
                linear5->setAlignment(item_spd_, Qt::AlignLeft);
                preferredWidth = item_spd_->preferredWidth();
                lenM = fm.width(sItem_spd_);
                item_spd_->setMaximumWidth(lenM);
                sSPD = item_spd_->CGUILabelText::text();
                if(sSPD.size() == 3)
                    linear5->setItemSpacing(1, 5);
                else
                    linear5->setItemSpacing(1, 10);

                linear5->insertItem(2, m_itemVRC);
                linear5->setAlignment(m_itemVRC, Qt::AlignLeft);
                preferredWidth = m_itemVRC->preferredWidth();
                lenM = fm.width(sItemVRC);
                m_itemVRC->setMaximumWidth(lenM);
                linear5->setItemSpacing(2, 5);
            case XGridLabel::RelatedSelected:
                linear5->insertItem(0, m_itemHDG);
                linear5->setAlignment(m_itemHDG, Qt::AlignLeft);
                preferredWidth = m_itemHDG->preferredWidth();
                lenM = fm.width(sItemHDG);
                m_itemHDG->setMaximumWidth(lenM);
                sHDG = m_itemHDG->CGUILabelText::text();
                if(sHDG.size() == 3)
                    linear5->setItemSpacing(0, 5);
                else
                    linear5->setItemSpacing(0, 10);

                linear5->insertItem(1, item_spd_);
                linear5->setAlignment(item_spd_, Qt::AlignLeft);
                preferredWidth = item_spd_->preferredWidth();
                lenM = fm.width(sItem_spd_);
                item_spd_->setMaximumWidth(lenM);
                sSPD = item_spd_->CGUILabelText::text();
                if(sSPD.size() == 3)
                    linear5->setItemSpacing(1, 5);
                else
                    linear5->setItemSpacing(1, 10);

                linear5->insertItem(2, m_itemVRC);
                linear5->setAlignment(m_itemVRC, Qt::AlignLeft);
                preferredWidth = m_itemVRC->preferredWidth();
                lenM = fm.width(sItemVRC);
                m_itemVRC->setMaximumWidth(lenM);
                linear5->setItemSpacing(2, 5);
            case XGridLabel::Unrelated:
                linear5->insertItem(0, m_itemHDG);
                linear5->setAlignment(m_itemHDG, Qt::AlignLeft);
                preferredWidth = m_itemHDG->preferredWidth();
                lenM = fm.width(sItemHDG);
                m_itemHDG->setMaximumWidth(lenM);
                sHDG = m_itemHDG->CGUILabelText::text();
                if(sHDG.size() == 3)
                    linear5->setItemSpacing(0, 5);
                else
                    linear5->setItemSpacing(0, 10);

                linear5->insertItem(1, item_spd_);
                linear5->setAlignment(item_spd_, Qt::AlignLeft);
                preferredWidth = m_itemHDG->preferredWidth();
                lenM = fm.width(sItem_spd_);
                item_spd_->setMaximumWidth(lenM);
                sSPD = item_spd_->CGUILabelText::text();
                if(sSPD.size() == 3)
                    linear5->setItemSpacing(1, 5);
                else
                    linear5->setItemSpacing(1, 10);

                linear5->insertItem(2, m_itemVRC);
                linear5->setAlignment(m_itemVRC, Qt::AlignLeft);
                preferredWidth = m_itemVRC->preferredWidth();
                lenM = fm.width(sItemVRC);
                m_itemVRC->setMaximumWidth(lenM);
                linear5->setItemSpacing(2, 5);
            case XGridLabel::UnrelatedShort:
                return;
            default:
                break;
            }


        }
        if(m_pAirplane->isInAirport())
        {
            linear2->insertItem(0, m_itemACID);
            linear2->setAlignment(m_itemACID, Qt::AlignLeft);
            preferredWidth = m_itemACID->preferredWidth();
            lenM = fm.width(sACID);
            m_itemACID->setMaximumWidth(lenM);
            linear2->setItemSpacing(0, 5);

            if(m_itemLM->text() != "  "){
                linear2->insertItem(1, m_itemLM);
                linear2->setAlignment(m_itemLM, Qt::AlignLeft);
                preferredWidth = m_itemLM->preferredWidth();
                lenM = fm.width(sItemLM);
                m_itemLM->setMaximumWidth(lenM);
                linear2->setItemSpacing(1, 5);
                switch (label_type) {
                case XGridLabel::Controlled:
                {
                    const QSharedPointer<CSFPL> &sfpl = m_pAirplane->GetSFPL();
                    if (!sfpl)
                    {
                        linear2->insertItem(3, m_itemCS);
                        linear2->setAlignment(m_itemCS, Qt::AlignLeft);
                        preferredWidth = m_itemCS->preferredWidth();
                        lenM = fm.width(sItemCS);
                        m_itemCS->setMaximumWidth(lenM);
                        linear2->setItemSpacing(3, 5);
                    }
                    break;
                }
                case XGridLabel::ControlledSelected:
                    linear2->insertItem(2, m_Mhdg);
                    linear2->setAlignment(m_Mhdg, Qt::AlignLeft);
                    preferredWidth = m_Mhdg->preferredWidth();
                    lenM = fm.width(sMhdg);
                    m_Mhdg->setMaximumWidth(lenM);
                    linear2->setItemSpacing(2, 5);

                    linear2->insertItem(3, m_itemCS);
                    linear2->setAlignment(m_itemCS, Qt::AlignLeft);
                    preferredWidth = m_itemCS->preferredWidth();
                    lenM = fm.width(sItemCS);
                    m_itemCS->setMaximumWidth(lenM);
                    linear2->setItemSpacing(3, 5);
                    break;
                case XGridLabel::InSelected:
                    linear2->insertItem(3, m_Mhdg);
                    linear2->setAlignment(m_Mhdg, Qt::AlignLeft);
                    preferredWidth = m_Mhdg->preferredWidth();
                    lenM = fm.width(sMhdg);
                    m_Mhdg->setMaximumWidth(lenM);
                    linear2->setItemSpacing(3, 5);

                    linear2->insertItem(4, m_itemCS);
                    linear2->setAlignment(m_itemCS, Qt::AlignLeft);
                    preferredWidth = m_itemCS->preferredWidth();
                    lenM = fm.width(sItemCS);
                    m_itemCS->setMaximumWidth(lenM);
                    linear2->setItemSpacing(4, 5);
                    break;
                case XGridLabel::In:
                    linear2->insertItem(3, m_itemCS);
                    linear2->setAlignment(m_itemCS, Qt::AlignLeft);
                    preferredWidth = m_itemCS->preferredWidth();
                    lenM = fm.width(sItemCS);
                    m_itemCS->setMaximumWidth(lenM);
                    linear2->setItemSpacing(3, 5);
                    break;
                case XGridLabel::RelatedSelected:
                    linear2->insertItem(3, m_Mhdg);
                    linear2->setAlignment(m_Mhdg, Qt::AlignLeft);
                    preferredWidth = m_Mhdg->preferredWidth();
                    lenM = fm.width(sMhdg);
                    m_Mhdg->setMaximumWidth(lenM);
                    linear2->setItemSpacing(3, 5);

                    linear2->insertItem(4, m_itemCS);
                    linear2->setAlignment(m_itemCS, Qt::AlignLeft);
                    preferredWidth = m_itemCS->preferredWidth();
                    lenM = fm.width(sItemCS);
                    m_itemCS->setMaximumWidth(lenM);
                    linear2->setItemSpacing(4, 5);
                    break;
                case XGridLabel::Related:
                {
                    const QSharedPointer<CSFPL> &sfpl = m_pAirplane->GetSFPL();
                    if (!sfpl)
                    {
                        linear2->insertItem(3, m_itemCS);
                        linear2->setAlignment(m_itemCS, Qt::AlignLeft);
                        preferredWidth = m_itemCS->preferredWidth();
                        lenM = fm.width(sItemCS);
                        m_itemCS->setMaximumWidth(lenM);
                        linear2->setItemSpacing(3, 5);
                    }
                    break;
                }
                case XGridLabel::Unrelated:
                    linear2->insertItem(3, m_Mhdg);
                    linear2->setAlignment(m_Mhdg, Qt::AlignLeft);
                    preferredWidth = m_Mhdg->preferredWidth();
                    lenM = fm.width(sMhdg);
                    m_Mhdg->setMaximumWidth(lenM);
                    linear2->setItemSpacing(3, 5);

                    linear2->insertItem(4, m_itemCS);
                    linear2->setAlignment(m_itemCS, Qt::AlignLeft);
                    preferredWidth = m_itemCS->preferredWidth();
                    lenM = fm.width(sItemCS);
                    m_itemCS->setMaximumWidth(lenM);
                    linear2->setItemSpacing(4, 5);
                    break;
                case XGridLabel::UnrelatedShort:
                    linear2->insertItem(3, m_itemCS);
                    linear2->setAlignment(m_itemCS, Qt::AlignLeft);
                    preferredWidth = m_itemCS->preferredWidth();
                    lenM = fm.width(sItemCS);
                    m_itemCS->setMaximumWidth(lenM);
                    linear2->setItemSpacing(3, 5);
                    break;
                default:
                    break;
            }
            }
            else {
                switch (label_type) {
                case XGridLabel::Controlled:
                {
                    const QSharedPointer<CSFPL> &sfpl = m_pAirplane->GetSFPL();
                    if (!sfpl)
                    {
                        linear3->insertItem(2, m_itemCS);
                    }
                    break;
                }
                case XGridLabel::ControlledSelected:
                    linear3->insertItem(2, m_Mhdg);
                    linear3->insertItem(3, m_itemCS);
                    break;
                case XGridLabel::InSelected:
                    linear3->insertItem(2, m_Mhdg);
                    linear3->insertItem(3, m_itemCS);
                    break;
                case XGridLabel::In:
                    linear3->insertItem(2, m_itemCS);
                    break;
                case XGridLabel::RelatedSelected:
                    linear3->insertItem(2, m_Mhdg);
                    linear3->insertItem(3, m_itemCS);
                    break;
                case XGridLabel::Related:
                {
                    const QSharedPointer<CSFPL> &sfpl = m_pAirplane->GetSFPL();
                    if (!sfpl)
                    {
                        linear3->insertItem(2, m_itemCS);
                    }
                    break;
                }
                case XGridLabel::Unrelated:
                    linear3->insertItem(2, m_Mhdg);
                    linear3->insertItem(3, m_itemCS);
                    break;
                case XGridLabel::UnrelatedShort:
                    linear3->insertItem(2, m_itemCS);
                    break;
                default:
                    break;
                }
            }

            ++row;

            linear3->insertItem(0, m_itemAFL);
            linear3->setAlignment(m_itemAFL, Qt::AlignLeft);
            preferredWidth = m_itemAFL->preferredWidth();
            lenM = fm.width(sItemAFL);
            m_itemAFL->setMaximumWidth(lenM);
            sAFL = m_itemAFL->CGUILabelText::text();
            /*if(sAFL.size() == 4 )
                linear3->setItemSpacing(0, 12);
            else
                if(sAFL.size() == 5 )
                    linear3->setItemSpacing(0, 18);
                else*/
                    linear3->setItemSpacing(0, 5);

            linear3->insertItem(1, m_itemTEND);
            linear3->setAlignment(m_itemTEND, Qt::AlignLeft);
            preferredWidth = m_itemTEND->preferredWidth();
            lenM = fm.width(sItemTEND);
            m_itemTEND->setMaximumWidth(lenM);
            sTEND = m_itemTEND->CGUILabelText::text();
            if(sTEND == " ")
                linear3->setItemSpacing(1, 5);
            else
                linear3->setItemSpacing(1, 7);

            linear3->insertItem(2, m_itemCFL);
            linear3->setAlignment(m_itemCFL, Qt::AlignLeft);
            preferredWidth = m_itemCFL->preferredWidth();
            lenM = fm.width(sItemCFL);
            m_itemCFL->setMaximumWidth(lenM);
            linear3->setItemSpacing(2, 5);

            switch (label_type) {
            case XGridLabel::Controlled:
                break;
            case XGridLabel::ControlledSelected:
                if(m_bGSPorIAS){
                    linear3->insertItem(3, item_gsp_);
                    item_gsp_->setBackgroundColor(QColor());
                }
                else {
                    linear3->insertItem(3, item_w_);
                    item_w_->setBackgroundColor(Qt::white);
                }
                linear3->insertItem(4, m_Runway);
                break;
            case XGridLabel::InSelected:
                if(m_bGSPorIAS){
                    linear3->insertItem(3, item_gsp_);
                    item_gsp_->setBackgroundColor(QColor());
                }
                else {
                    linear3->insertItem(3, item_w_);
                    item_w_->setBackgroundColor(Qt::white);
                }
                linear3->insertItem(4, m_Runway);
                break;
            case XGridLabel::RelatedSelected:
                if(m_bGSPorIAS){
                    linear3->insertItem(3, item_gsp_);
                    item_gsp_->setBackgroundColor(QColor());
                }
                else {
                    linear3->insertItem(3, item_w_);
                    item_w_->setBackgroundColor(Qt::white);
                }
                linear3->insertItem(4, m_Runway);
                break;
            case XGridLabel::Unrelated:
                if(m_bGSPorIAS){
                    linear3->insertItem(3, item_gsp_);
                    item_gsp_->setBackgroundColor(QColor());
                }
                else {
                    linear3->insertItem(3, item_w_);
                    item_w_->setBackgroundColor(Qt::white);
                }
                linear3->insertItem(4, m_Runway);
                break;
            case XGridLabel::UnrelatedShort:
                break;
            default:
                break;
            }

            ++row;
            QString sVRC;

            switch (label_type) {
            case XGridLabel::Controlled:
                return;
            case XGridLabel::ControlledSelected:
                linear4->insertItem(0, m_itemHDG);
                linear4->setAlignment(m_itemHDG, Qt::AlignLeft);
                preferredWidth = m_itemHDG->preferredWidth();
                lenM = fm.width(sItemHDG);
                m_itemHDG->setMaximumWidth(lenM);
                sHDG = m_itemHDG->CGUILabelText::text();
                if(sHDG.size() == 3)
                    linear4->setItemSpacing(0, 5);
                else
                    linear4->setItemSpacing(0, 10);

                linear4->insertItem(1, item_spd_);
                linear4->setAlignment(item_spd_, Qt::AlignLeft);
                preferredWidth = item_spd_->preferredWidth();
                lenM = fm.width(sItem_spd_);
                item_spd_->setMaximumWidth(lenM);
                sSPD = item_spd_->CGUILabelText::text();
                if(sSPD.size() == 3)
                    linear4->setItemSpacing(1, 5);
                else
                    linear4->setItemSpacing(1, 10);

                linear4->insertItem(2, m_itemVRC);
                linear4->setAlignment(m_itemVRC, Qt::AlignLeft);
                preferredWidth = m_itemVRC->preferredWidth();
                lenM = fm.width(sItemVRC);
                m_itemVRC->setMaximumWidth(lenM);
                sVRC = m_itemVRC->CGUILabelText::text();
                if(sVRC.size() == 3)
                    linear4->setItemSpacing(2, 5);
                else
                    if(sVRC.size() > 3)
                        linear4->setItemSpacing(2, 5 * sVRC.size());

                linear4->insertItem(3, m_itemType);
                linear4->setAlignment(m_itemType, Qt::AlignLeft);
                preferredWidth = m_itemType->preferredWidth();
                lenM = fm.width(sItemType);
                m_itemType->setMaximumWidth(lenM);
                linear4->setItemSpacing(3, 5);
                break;
            case XGridLabel::In:
                break;
            case XGridLabel::InSelected:
                linear4->insertItem(0, m_itemHDG);
                linear4->setAlignment(m_itemHDG, Qt::AlignLeft);
                preferredWidth = m_itemHDG->preferredWidth();
                lenM = fm.width(sItemHDG);
                m_itemHDG->setMaximumWidth(lenM);
                sHDG = m_itemHDG->CGUILabelText::text();
                if(sHDG.size() == 3)
                    linear4->setItemSpacing(0, 5);
                else
                    linear4->setItemSpacing(0, 10);

                linear4->insertItem(1, item_spd_);
                linear4->setAlignment(item_spd_, Qt::AlignLeft);
                preferredWidth = item_spd_->preferredWidth();
                lenM = fm.width(sItem_spd_);
                item_spd_->setMaximumWidth(lenM);
                if(sSPD.size() == 3)
                    linear4->setItemSpacing(1, 5);
                else
                    linear4->setItemSpacing(1, 10);

                linear4->insertItem(2, m_itemVRC);
                linear4->setAlignment(m_itemVRC, Qt::AlignLeft);
                preferredWidth = m_itemVRC->preferredWidth();
                lenM = fm.width(sItemVRC);
                m_itemVRC->setMaximumWidth(lenM);
                sVRC = m_itemVRC->CGUILabelText::text();
                if(sVRC.size() == 3)
                    linear4->setItemSpacing(2, 7);
                else
                    if(sVRC.size() == 6)
                        linear4->setItemSpacing(2, 10);
                    else
                        if(sVRC.size() == 4)
                            linear4->setItemSpacing(2, 10);

                linear4->insertItem(3, m_itemType);
                linear4->setAlignment(m_itemType, Qt::AlignLeft);
                preferredWidth = m_itemType->preferredWidth();
                lenM = fm.width(sItemType);
                m_itemType->setMaximumWidth(lenM);
                linear4->setItemSpacing(3, 5);
            case XGridLabel::Related:
                break;
            case XGridLabel::RelatedSelected:
                linear4->insertItem(0, m_itemHDG);
                linear4->setAlignment(m_itemHDG, Qt::AlignLeft);
                preferredWidth = m_itemHDG->preferredWidth();
                lenM = fm.width(sItemHDG);
                m_itemHDG->setMaximumWidth(lenM);
                sHDG = m_itemHDG->CGUILabelText::text();
                if(sHDG.size() == 3)
                    linear4->setItemSpacing(0, 5);
                else
                    linear4->setItemSpacing(0, 10);

                linear4->insertItem(1, item_spd_);
                linear4->setAlignment(item_spd_, Qt::AlignLeft);
                preferredWidth = item_spd_->preferredWidth();
                lenM = fm.width(sItem_spd_);
                item_spd_->setMaximumWidth(lenM);
                if(sSPD.size() == 3)
                    linear4->setItemSpacing(1, 5);
                else
                    linear4->setItemSpacing(1, 10);

                linear4->insertItem(2, m_itemVRC);
                linear4->setAlignment(m_itemVRC, Qt::AlignLeft);
                preferredWidth = m_itemVRC->preferredWidth();
                lenM = fm.width(sItemVRC);
                m_itemVRC->setMaximumWidth(lenM);
                linear4->setItemSpacing(2, 5);

                linear4->insertItem(3, m_itemType);
                linear4->setAlignment(m_itemType, Qt::AlignLeft);
                preferredWidth = m_itemType->preferredWidth();
                lenM = fm.width(sItemType);
                m_itemType->setMaximumWidth(lenM);
                linear4->setItemSpacing(3, 5);
            case XGridLabel::Unrelated:
                linear4->insertItem(0, m_itemHDG);
                linear4->setAlignment(m_itemHDG, Qt::AlignLeft);
                preferredWidth = m_itemHDG->preferredWidth();
                lenM = fm.width(sItemHDG);
                m_itemHDG->setMaximumWidth(lenM);
                sHDG = m_itemHDG->CGUILabelText::text();
                if(sHDG.size() == 3)
                    linear4->setItemSpacing(0, 5);
                else
                    linear4->setItemSpacing(0, 10);

                linear4->insertItem(1, item_spd_);
                linear4->setAlignment(item_spd_, Qt::AlignLeft);
                preferredWidth = item_spd_->preferredWidth();
                lenM = fm.width(sItem_spd_);
                item_spd_->setMaximumWidth(lenM);
                if(sSPD.size() == 3)
                    linear4->setItemSpacing(1, 5);
                else
                    linear4->setItemSpacing(1, 10);

                linear4->insertItem(2, m_itemVRC);
                linear4->setAlignment(m_itemVRC, Qt::AlignLeft);
                preferredWidth = m_itemVRC->preferredWidth();
                lenM = fm.width(sItemVRC);
                m_itemVRC->setMaximumWidth(lenM);
                linear4->setItemSpacing(2, 5);

                linear4->insertItem(3, m_itemType);
                linear4->setAlignment(m_itemType, Qt::AlignLeft);
                preferredWidth = m_itemType->preferredWidth();
                lenM = fm.width(sItemType);
                m_itemType->setMaximumWidth(lenM);
                linear4->setItemSpacing(3, 5);
            case XGridLabel::UnrelatedShort:
                return;
            default:
                break;
            }

        }
    }
}

void CGUIAirplaneLabel::processFlightLevels(XGridLabel::LabelType label_type)
{
    switch (label_type) {
    case XGridLabel::ControlledSelected:
    case XGridLabel::InSelected:
    case XGridLabel::RelatedSelected:
    case XGridLabel::UnrelatedSelected:
        return;
    default:
        break;
    }

    const bool flight_level_monitoring = networkSettings()->value("users/flight_level_monitoring", "no").toString() == "yes";
    if (!flight_level_monitoring)
    {
        if(!m_pAirplane->isInAirport()){
            m_itemTFL->SetVisible(true);
            m_itemTFL->setVisible(true);
        }
        m_itemCFL->SetVisible(true);
        m_itemCFL->setVisible(true);
        return;
    }

    bool cfl_visible = false;
    bool tfl_visible = false;

    const double afl = m_pAirplane->GetFL();
    const double tfl = m_pAirplane->GetTFL().GetF();
    if(label_type == XGridLabel::Unrelated)
        tfl_visible = true;

    const QSharedPointer<CSFPL> &sfpl = m_pAirplane->GetSFPL();
    if (sfpl) {
        const CAirplaneData::PlaneType plane_type = m_pAirplane->GetPlaneType();
        if (CAirplaneData::ACT == plane_type) {
            const QString cur_sector = networkSettings()->value("users/current_sector").toString();
            if (cur_sector == sfpl->GetNextSector()) {
                if (sfpl->GetCOPinFL()) {
                    const double cfl = sfpl->GetCOPinFL().GetF();
                    if (qAbs(afl - cfl) > 0.9)
                        cfl_visible = true;

                    if (m_pAirplane->GetTFL()) {
                        if (qAbs(tfl - cfl) > 0.9)
                            tfl_visible = true;
                    }
                }
            } else {
                cfl_visible = true;
            }
        } else {
            if (sfpl->GetCFL()) {
                const double cfl = sfpl->GetCFL().GetF();
                if (qAbs(afl - cfl) > 0.9)
                    cfl_visible = true;

                if (m_pAirplane->GetTFL()) {
                    if (qAbs(tfl - cfl) > 0.9)
                        tfl_visible = true;
                }
            } else if (m_pAirplane->GetTFL()) {
                if (qAbs(tfl - afl) > 0.9)
                    tfl_visible = true;
            }
        }
    } else if (m_pAirplane->GetTFL()) {
        cfl_visible = true;

        if (qAbs(tfl - afl) > 0.9)
            tfl_visible = true;
    } else {
        cfl_visible = true;
    }

    if(!m_pAirplane->isInAirport())
        if (!tfl_visible && m_pAirplane->GetTFL()) {
            m_itemTFL->SetVisible(tfl_visible);
            m_itemTFL->setVisible(tfl_visible);
        } else {
            m_itemTFL->SetVisible(tfl_visible);
            //m_itemTFL->SetVisible(true);
            m_itemTFL->setVisible(tfl_visible);
        }
    else  {
        m_itemTFL->SetVisible(false);
        m_itemTFL->setVisible(false);
    }
    if (!cfl_visible) {
        if (sfpl && !sfpl->GetCFL())
        {// always show 000
        } else {
            m_itemCFL->SetVisible(cfl_visible);
            m_itemCFL->setVisible(cfl_visible);
        }
    } else {
        m_itemCFL->SetVisible(cfl_visible);
        m_itemCFL->setVisible(cfl_visible);
    }
}

void CGUIAirplaneLabel::processAcid(const QSharedPointer<CSFPL> &sfpl)
{
    m_itemACID->setText("ACID");

    if (sfpl) {
        m_itemACID->setText(sfpl->GetACID());
    } else {
        if (m_pAirplane->GetACID().isEmpty()) {
            XGridLabel::LabelType label_type = GetGridLabel().GetType();
            //if ((XGridLabel::Unrelated == label_type) || (XGridLabel::UnrelatedShort == label_type)) {
            if (XGridLabel::UnrelatedShort == label_type) {
                m_itemACID->setText(m_pAirplane->GetMode3AReplyToString());
            } else {
                m_itemACID->setText(tr("No FPL"));
            }
        } else {
            //QString text = m_pAirplane->GetMode3AReplyToString();
            //text += "/" + m_pAirplane->GetACID();
            //m_itemACID->setText(text);
            m_itemACID->setText(m_pAirplane->GetACID());
        }
    }

    if (m_pAirplane->isUvd()) {
        m_itemACID->setBackgroundColor(qRgb(0, 200, 0));
    }

    m_itemCFL->SetACID(m_itemACID->text());
    m_itemTFL->SetACID(m_itemACID->text());
}

void CGUIAirplaneLabel::processCfl(CAirplaneData::PlaneType plane_type, const QSharedPointer<CSFPL> &sfpl)
{
    //m_itemCFL->setText(QString("CFL"));
    m_itemCFL->setText(QString("000"));
    if (m_pAirplane->GetCFL())
       m_itemCFL->setText(GetFLText(m_pAirplane->GetCFL(), false, false));

//    if (CAirplaneData::ACT == plane_type) {
//        if (sfpl) {
//            const QStringList fir_sectors = networkSettings()->value("users/fir_sectors").toString().split("-", QString::SkipEmptyParts);
//            const QString cur_sector = networkSettings()->value("users/current_sector").toString();
//            if (cur_sector == sfpl->GetNextSector()) {
//                if (sfpl->GetCOPinFL()) {
//                    //sText.sprintf("%03.0f", sfpl->GetCOPinFL() * TransformFeet(true));
//                    m_itemCFL->setText(GetFLText(sfpl->GetCOPinFL(), true));
//                }
//            } else if (fir_sectors.contains(sfpl->GetCurSector())) {
//                if (sfpl->GetCFL())
//                    m_itemCFL->setText(GetFLText(sfpl->GetCFL(), true));
//            }
//        }
//    } else {
//        if (sfpl && sfpl->GetCFL()) {
//            //sText.sprintf("%03.0f", sfpl->GetCFL() * TransformFeet(true));
//            m_itemCFL->setText(GetFLText(sfpl->GetCFL(), true));
//        } else if (isAssumed()) {
//            m_itemCFL->setText(QString("000"));
//        }
//    }
}

bool CGUIAirplaneLabel::isAssumed() const
{
    bool bRes(false);
    if (m_pAirplane) {
        const CAirplaneData::PlaneType plane_type = m_pAirplane->GetPlaneType();
        if ((CAirplaneData::ASSUME == plane_type) || (CAirplaneData::ActOutInFir == plane_type)/* || (CAirplaneData::ActOut == plane_type)*/)
            bRes = true;
    }
    return bRes;
}

CGUILabelText * CGUIAirplaneLabel::fieldByPos(const QPointF &pos) const
{
    foreach (QGraphicsItem *child_item, childItems()) {
        QRectF rect = child_item->boundingRect();
        rect = child_item->mapRectToParent(rect);
        if (rect.contains(pos)) {
            CGUILabelText *selected_field = dynamic_cast<CGUILabelText*>(child_item);
            if (selected_field) {
                if (selected_field->GetVisible())
                    return selected_field;
            }
        }
    }

    return 0;
}

void CGUIAirplaneLabel::updateHdgSpdVrcCflFieldsColor()
{
    if (m_itemACID)
        m_itemACID->setBackgroundColor(QColor());

    const QSharedPointer<CSFPL> &sfpl = m_pAirplane->GetSFPL();
    if (sfpl) {
        const QString sAcid = sfpl->GetACID().trimmed();
        const QString aAcid = m_pAirplane->GetACID().trimmed();
        if (!aAcid.isEmpty())
            m_itemACID->setFrame();
        if (!aAcid.isEmpty() && sAcid != aAcid) {
            m_itemACID->setBackgroundColor(Qt::yellow);
        }
        if (m_pAirplane && m_pAirplane->IsLastActive())
        {
            UserProfile::CSettings& stg = UserProfile::CSettings::Instance();
            m_itemACID->SetTextColor(stg.GetColor(UserProfile::CtFormularActive));
        }

    }
}

void CGUIAirplaneLabel::afterSleep()
{
if (!m_pAirplane->GetSFPL()) {
    const QString cur_sector = networkSettings()->value("users/current_sector").toString();
    QSharedPointer<CSFPL> sfpl(new CSFPL);
    const QString &acid = m_pAirplane->GetACID();
    if (acid.isEmpty())
        sfpl->SetACID(QString("!!!%1").arg(m_pAirplane->GetMode3AReplyToString()));
    else
        sfpl->SetACID(acid);
    sfpl->SetCODE(m_pAirplane->GetMode3AReply());
    sfpl->SetCOPin("XXXXX");
    sfpl->SetETOin(QDateTime::currentDateTimeUtc().time());
    sfpl->SetCOPinFL(m_pAirplane->GetCFL());
    //sfpl->SetNextSector(cur_sector);
    sfpl->SetCurSector(cur_sector);

    if (master()->getRedisDB()->getConnectionStatus() == 1 && DBConnectionsManager::instance().getConnectionStatus() == 1)  {
        bool ok = master()->getRedisDB()->saveSFPL(sfpl);
        if (!ok) {
            QMessageBox::critical(0, tr("Error"), tr("Error saving FPL"));
            return;
        }
    } else {
        QMessageBox::critical(0, tr("Error"), tr("Error saving FPL"));
        return;
    }

    m_pAirplane->SetSFPL(sfpl);
    m_pAirplane->SetPlaneType(CAirplaneData::ASSUME);
}
    else{
        QSharedPointer<CSFPL> sfpl = m_pAirplane->GetSFPL();
        if (sfpl->GetNextSector() == "" && sfpl->GetCurSector() == "")
        {
            const QString cur_sector = networkSettings()->value("users/current_sector").toString();
            sfpl->SetNextSector(cur_sector);//SetCurSector(cur_sector);??
            if (master()->getRedisDB()) {
                bool ok = master()->getRedisDB()->saveSFPL(sfpl);
                if (!ok) {
                    QMessageBox::critical(0, tr("Error"), tr("Error saving FPL"));
                    return;
                }
            } else {
                QMessageBox::critical(0, tr("Error"), tr("Error saving FPL"));
                return;
            }
            m_pAirplane->SetSFPL(sfpl);
        }
        else
          {
              const CAirplaneData::PlaneType plane_type = m_pAirplane->GetPlaneType();
              if (CAirplaneData::Transfer == plane_type)
              {
                logOperation(tr("ReAssume: %1").arg(m_pAirplane->GetMode3AReplyToString()));
                m_pAirplane->onTimerReAssume();
                return;
              }
          }
        m_pAirplane->assume();
    }
    m_pAirplane->SetOur(true);
    WriteLog("NS", "ASSUME");
    logOperation(tr("Assume: %1").arg(m_pAirplane->GetMode3AReplyToString()));
    //m_pAirplane->AssumeOperator();
    //m_pAirplane->assume();
}


// class CAzimuthAndDistance

CAzimuthAndDistance::CAzimuthAndDistance()
    : m_bInit(false), m_bFind(false)
{

}

bool CAzimuthAndDistance::Init(CGUIAirplane* pAirplane)
{
    if (!m_bInit)
    {
        m_bFind = false;
        QSharedPointer<CView> view = qSharedPointerDynamicCast<CView>(master()->GetView());
        if (master() && view && pAirplane && pAirplane->GetSFPL())
        {
            CView::TScenePtr pScene = view->GetMainFIR();
            if (pScene)
            {
                Scene::TAirportsMap::const_iterator itAirport = pScene->GetAirports().find(
                        CAirportKey(pAirplane->GetSFPL()->GetDestAerodrome()));
                if (itAirport != pScene->GetAirports().constEnd())
                {
                    m_coord = (*itAirport)->GetCoord();
                    m_bFind = true;
                }
                m_bInit = true;
            }
        }
    }
    return m_bFind && !m_coord.IsNull() && m_coord.IsCenter();
}

QString CAzimuthAndDistance::GetAzimuthText(CGUIAirplane* pAirplane)
{
    QString sText = "-";
    if (Init(pAirplane))
    {
        double angle = GetAzimuth(pAirplane->GetCurGUIPlaneCoord(), m_coord); //radians
        angle *= 180/M_PI;
        int alpha = qRound(angle);
        int beta = alpha - 180;
        if(beta < 0)
            beta += 360;
        if (!alpha)
            alpha = 360;
        if (!beta)
            beta = 360;
        sText = QString("%1%2/%3%4").arg(alpha).arg(UTF8_QSTRING("\u00B0")).arg(beta).arg(UTF8_QSTRING("\u00B0"));
    }
    return sText;
}

QString CAzimuthAndDistance::GetDistanceText(CGUIAirplane* pAirplane)
{
    QString sText = "-";
    if (Init(pAirplane))
    {
        double len = GetDistance(pAirplane->GetCurGUIPlaneCoord(), m_coord);
        if (AppSettings::IsSISet(AppSettings::SIFeet))
        {
            // miles
            len /= 1852;
        }
        else
        {
            // kilometers
            len /= 1000;
        }
        len = qRound(len);
        if (len > 0)
            sText = QString::number(quint32(len));
    }
    return sText;
}
