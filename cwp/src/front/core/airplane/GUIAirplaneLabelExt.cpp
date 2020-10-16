 #include "GUIAirplaneLabelExt.h"
#include "GUILabelText.h"
#include "GUIAirplane.h"
//#include "front/ground/GUIGroundAirplane.h"
#include "GUIAirplaneMTCDPoint.h"

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

const QString kSpecSignal = QObject::tr("spec signal");

CGUIAirplaneLabelExt::CGUIAirplaneLabelExt(QGraphicsItem * parent/* = 0*/)
    : QGraphicsWidget(parent)
    , m_ConstructorFlag(false), m_iPenWidth(1), m_color(Qt::black), m_colorForeground(Qt::black), m_colorBackground(Qt::lightGray)
    , m_Transparent(25), m_bFullShow(false)
    , m_bMoving(false), m_bActive(false), m_bPress(false), m_bPressGsp(false), m_bPressW(false), m_bMachLevel(false)
    , m_iAzimuth2(kInvalidAzimuth), m_pAirplane(0)
    , m_itemCODE(0), m_itemACID(0), m_itemType(0)
    , m_itemAfl(0), m_itemCfl(0), m_itemTfl(0)
    , m_itemAdep(0), m_itemAdes(0), m_itemAlter(0)
    , RouteMiddle("----")
    , m_itemIask(0),m_itemIasm(0),m_itemGsp(0)
    , m_groundItemStatus(0), m_groundItemACID(0)
    , plane_type_(CAirplaneData::None),/* spec_signal_(false),*/ spi_timer_(new QTimer(this)), emg_timer_(new QTimer(this))
    , move_(false)
{
    m_bFullShow = false;// networkSettings()->value("airplane/auto_collapse", "yes").toString() != "yes";
    setZValue(gui::Zairplanelabel);
    setFlag(ItemDoesntPropagateOpacityToChildren, true);
    setAcceptedMouseButtons(Qt::LeftButton|Qt::RightButton|Qt::MiddleButton);

    setAcceptHoverEvents(true);
    m_ActiveTimer = new QTimer(this);
    connect(m_ActiveTimer, SIGNAL(timeout()), this, SLOT(OnActivePlane()));

    spi_timer_->setInterval(10 * 1000);
    connect(spi_timer_, SIGNAL(timeout()), this, SLOT(onSpiTimeout()));

    emg_timer_->setInterval(8 * 1000);
    connect(emg_timer_, SIGNAL(timeout()), this, SLOT(onEmgTimeout()));


}

CGUIAirplaneLabelExt::~CGUIAirplaneLabelExt()
{
}

QRectF CGUIAirplaneLabelExt::boundingRect() const
{
    if(!m_pAirplane) return QRectF();
    QPainterPath paintPath;
    paintPath.addRect(m_clRect);

    QRectF rect(paintPath.boundingRect());
    rect.adjust(-1,-1,1,1);
    return rect;
}

QPainterPath CGUIAirplaneLabelExt::shape() const
{
    if (!m_pAirplane)
        return QPainterPath();
    QPainterPath paintPath;
    paintPath.addRect(m_clRect);
    return paintPath;
}

void CGUIAirplaneLabelExt::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (!m_pAirplane)
        return;

    painter->save();
    QColor color(m_color);
    if (m_pAirplane && m_pAirplane->IsLastActive())
        color = QColor(Qt::green);
    if (m_bActive)
        color = m_colorActive;
    QPen qPen(color);
    qPen.setWidth(m_iPenWidth);
    painter->setPen(qPen);
    if (!(globalSettings()->value("XMasterWindow/Settings/ShowFormularBorder", 1).toInt() & 1))
        painter->setPen(Qt::NoPen);
    QColor colorBackground(m_colorBackground);

    painter->setBrush(QBrush(colorBackground));
    painter->drawRect(m_clRect);

    painter->restore();
}

QRectF CGUIAirplaneLabelExt::boundingRectOnView(QGraphicsView *view) const
{
    return deviceTransform(view->viewportTransform()).mapRect(boundingRect());
}

void CGUIAirplaneLabelExt::UpdatePosition(QPointF point)
{
    m_PlainParentPoint = point;

    if (!m_itemCODE && m_pAirplane->isFlying())
        initGrid();
    if (!m_groundItemACID && !m_pAirplane->isFlying())
        initGrid();
    loadTypeGrid(m_bFullShow);
    updateGrid();
}

void CGUIAirplaneLabelExt::returnPosition(qreal dif_x, qreal dif_y)
{
    moveBy(dif_x, dif_y);
}


void CGUIAirplaneLabelExt::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
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

void CGUIAirplaneLabelExt::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    m_bMoving = false;
    if (m_pAirplane)
        m_pAirplane->SetActivePlane(true);

    QGraphicsWidget::mousePressEvent(event);

    drag_point_ = event->pos();

    if (event->button() == Qt::RightButton) {
        CGUILabelText *selected_field = fieldByPos(event->pos());
        if (selected_field == m_itemACID)
        {
            setVisible(false);
            m_pAirplane->ClearLabelExtflag();
        }

    }
    else if (event->button() == Qt::MiddleButton) {
            CGUILabelText *selected_field = fieldByPos(event->pos());
            if (selected_field == m_itemRoute)
                QToolTip::showText(event->screenPos(), QString("%1").arg(RouteMiddle));
    }
    event->accept();
}

void CGUIAirplaneLabelExt::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsWidget::mouseReleaseEvent(event);
    if (event->button() == Qt::MiddleButton) {
        move_ = true;
        m_pAirplane->UpdateFormularPos();

        emit unlockMovingView();
    }

}
bool CGUIAirplaneLabelExt::isMoved()
{
    return move_;
}
void CGUIAirplaneLabelExt::SetMoved(bool value)
{
    move_ = value;
}
QVariant CGUIAirplaneLabelExt::itemChange(GraphicsItemChange change, const QVariant& value)
{
    if (change == QGraphicsItem::ItemPositionChange)
        m_bMoving = true;
    return QGraphicsWidget::itemChange(change, value);
}

void CGUIAirplaneLabelExt::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    showGrid(true);
    if (m_pAirplane)
        m_pAirplane->SetActivePlane(false);
    QGraphicsWidget::hoverEnterEvent(event);
}

void CGUIAirplaneLabelExt::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if (!m_bPress)
        showGrid(false);
    QGraphicsWidget::hoverLeaveEvent(event);
}

void CGUIAirplaneLabelExt::initGrid()
{
    if (!m_pAirplane)
        return;

    if (m_pAirplane->isFlying())
    {
#ifdef QT_DEBUG
        item_cs_debug_ = new CGUILabelText(this);
        item_cs_debug_->SetReadOnly(true);
        item_cs_debug_->setText("cs");

        item_ns_debug_ = new CGUILabelText(this);
        item_ns_debug_->SetReadOnly(true);
        item_ns_debug_->setText("ns");
#endif

        item_spec_signal_1 = new CGUILabelText(this);
        item_spec_signal_1->setText(kSpecSignal);
        item_spec_signal_1->setBoldFont(true);

        item_spec_signal_2 = new CGUILabelText(this);
        item_spec_signal_2->setText(kSpecSignal);
        item_spec_signal_2->setBoldFont(true);

        item_spec_signal_3 = new CGUILabelText(this);
        item_spec_signal_3->setText(kSpecSignal);
        item_spec_signal_3->setBoldFont(true);

        m_itemACID = new CGUILabelText(this);
        m_itemACID->SetReadOnly(true);
        m_itemACID->SetTypeDlg(CGUIControlDlg::FPLType);
        m_itemACID->setText(tr("ACID"));

        m_itemCODE = new CGUILabelText(this);
        m_itemCODE->SetReadOnly(true);
        m_itemCODE->SetTypeDlg(CGUIControlDlg::ListWithEditType);
        m_itemCODE->setText(tr("CODE"));

        m_itemSpace = new CGUILabelText(this);
        m_itemSpace->SetReadOnly(true);
        //m_itemSpace->setText(tr("XXXXXX"));
        m_itemSpace->setText(tr("*********"));

        m_itemType = new CGUILabelText(this);
        m_itemType->SetReadOnly(true);
        m_itemType->SetTypeDlg(CGUIControlDlg::ListWithEditType);
        m_itemType->setText(tr("Type"));

        m_itemAfl = new CGUILabelText(this);
        m_itemAfl->SetReadOnly(true);
        m_itemAfl->setText("AFL");

        m_itemCfl = new CGUILabelText(this);
        m_itemCfl->SetReadOnly(true);
        m_itemCfl->setText("CFL");

        m_itemTfl = new CGUILabelText(this);
        m_itemTfl->SetReadOnly(true);
        m_itemTfl->setText("TFL");

        m_itemAdep = new CGUILabelText(this);
        m_itemAdep->SetReadOnly(true);
        m_itemAdep->SetTypeDlg(CGUIControlDlg::ListWithEditType);
        m_itemAdep->setText(tr("-----"));

        m_itemAdes = new CGUILabelText(this);
        m_itemAdes->SetReadOnly(true);
        m_itemAdes->SetTypeDlg(CGUIControlDlg::ListWithEditType);
        m_itemAdes->setText(tr("-----"));

        m_itemAlter = new CGUILabelText(this);
        m_itemAlter->SetReadOnly(true);
        m_itemAlter->SetTypeDlg(CGUIControlDlg::ListWithEditType);
        m_itemAlter->setText(tr("-----"));

        m_itemPoint = new CGUILabelText(this);
        m_itemPoint->SetReadOnly(true);
        m_itemPoint->setText("XXXXX");

        m_itemEq = new CGUILabelText(this);
        m_itemEq->SetReadOnly(true);
        m_itemEq->setText("EQ");

        m_itemLit = new CGUILabelText(this);
        m_itemLit->SetReadOnly(true);
        m_itemLit->setText("LIT");

        m_itemRoute = new CGUILabelText(this);
        m_itemRoute->SetReadOnly(true);
        m_itemRoute->setText("ROUTE...");

        m_itemStar = new CGUILabelText(this);
        m_itemStar->SetReadOnly(true);
        m_itemStar->setText("STAR");

        m_itemIask = new CGUILabelText(this);
        m_itemIask->SetReadOnly(true);
        m_itemIask->setText("IASk");

        m_itemIasm = new CGUILabelText(this);
        m_itemIasm->SetReadOnly(true);
        m_itemIasm->setText("IASm");

        m_itemGsp = new CGUILabelText(this);
        m_itemGsp->SetReadOnly(true);
        m_itemGsp->setText("GSP");

        m_itemHdg = new CGUILabelText(this);
        m_itemHdg->SetReadOnly(true);
        m_itemHdg->setText("hdg");

        m_itemTas = new CGUILabelText(this);
        m_itemTas->SetReadOnly(true);
        m_itemTas->setText("TAS");

        //item_spd_ = new CGUILabelText(this);
        //item_spd_->SetReadOnly(false);
       //item_spd_->SetTypeDlg(CGUIControlDlg::speedWidget);
        //item_spd_->setText("spd");
        //connect(item_spd_, SIGNAL(rejected()), this, SLOT(onRejected()));

        m_itemVrc = new CGUILabelText(this);
        m_itemVrc->SetReadOnly(true);
        //m_itemVrc->SetTypeDlg(CGUIControlDlg::ListType);
        //m_itemVrc->SetTypeDlg(CGUIControlDlg::verticalSpeedWidget);
        m_itemVrc->setText(tr("vrc"));
        //connect(m_itemVRC, SIGNAL(rejected()), this, SLOT(onRejected()));

       /* item_gsp_ = new CGUILabelText(this);
        item_gsp_->setText(tr("GSP"));*/

        /*item_w_ = new CGUILabelText(this);
        item_w_->setText(tr("W"));*/

        m_itemBps = new CGUILabelText(this);
        m_itemBps->SetReadOnly(true);
        m_itemBps->setText("BPS");

        m_itemSfl = new CGUILabelText(this);
        m_itemSfl->SetReadOnly(true);
        m_itemSfl->setText("SFL");

        m_itemAcas = new CGUILabelText(this);
        m_itemAcas->SetReadOnly(true);
        m_itemAcas->setText("ACAS");

    }
    else
    {
        //Ground Items
       // m_groundItemStatus = new CGUILabelText(this);
       // m_groundItemStatus->SetTypeDlg(CGUIControlDlg::ListType);
       // m_groundItemStatus->SetReadOnly(false);
       // m_groundItemStatus->setText(tr("<None>"));
       // connect(m_groundItemStatus, SIGNAL( endEdit(const CGUILabelText*, const QStringList) ),
       //                 this, SLOT( OnEndEdit(const CGUILabelText*, const QStringList) ));

       // m_groundItemACID = new CGUILabelText(this);
       // m_groundItemACID->setText(QL1S("ACID"));

        //m_groundItemSQUAWK = new CGUILabelText(this);
        //m_groundItemSQUAWK->SetFocusable(true);
        //m_groundItemSQUAWK->setText(QL1S("SQUAWK"));

       // m_groundItemSpeed = new CGUILabelText(this);
       // m_groundItemSpeed->SetReadOnly(false);
        //m_groundItemSpeed->SetTypeDlg(CGUIControlDlg::);
       // m_groundItemSpeed->setText(QL1S("Speed"));
    //    connect(m_groundItemSpeed, SIGNAL( endEdit(const CGUILabelText*, const QStringList) ),
    //                    this, SLOT( OnEndEdit(const CGUILabelText*, const QStringList) ));

       // m_groundItemSpeedLabel = new CGUILabelText(this);
       // m_groundItemSpeedLabel->setText(tr("Speed, km/h: "));
    }

    QGraphicsGridLayout *grid = new QGraphicsGridLayout(this);
    grid->setHorizontalSpacing(10);
    grid->setVerticalSpacing(1);
    grid->setContentsMargins(2,2,2,2);
    setLayout(grid);
}

void CGUIAirplaneLabelExt::loadTypeGrid(bool bFullShowVal)
{
    QGraphicsGridLayout *grid = dynamic_cast<QGraphicsGridLayout*>(layout());
    if (!grid)
        return;

    XGridLabel::LabelType label_type = XGridLabel::None;
    bool selected = bFullShowVal;
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
                                bool b2 = controlled_sectors.contains(sfpl->GetInSector());
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
        }
    }

    XGridLabel& GridLabel = GetGridLabel();
    m_colorForeground = GridLabel.GetForeground();
    m_colorBackground = GridLabel.GetBackground();
    m_Transparent = GridLabel.GetTransparent();

    for (int i = grid->count()-1; i >= 0; --i)
        grid->removeAt(i);

    if (m_pAirplane && !m_pAirplane->isFlying())
    {
       /* grid->addItem(m_groundItemStatus,      0,0,1,3, Qt::AlignHCenter);
        grid->addItem(m_groundItemACID,        1,0,1,1, Qt::AlignLeft);
        grid->addItem(m_groundItemSQUAWK,      1,2,1,1, Qt::AlignRight);
        grid->addItem(m_groundItemSpeedLabel,  2,0,1,2, Qt::AlignLeft);
        grid->addItem(m_groundItemSpeed,       2,2,1,1, Qt::AlignRight);

        grid->setHorizontalSpacing(5);
        for (int i = 0; i < grid->count(); ++i)
            dynamic_cast<CGUILabelText*>(grid->itemAt(i))->setVisible(true);*/
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
                grid->addItem(m_itemACID, itemPos.GetRow(), itemPos.GetColumn(), itemPos.GetRowSpan(), itemPos.GetColumnSpan(), itemPos.GetAlignment());
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
                grid->addItem(m_itemCODE, itemPos.GetRow(), itemPos.GetColumn(), itemPos.GetRowSpan(), itemPos.GetColumnSpan(), itemPos.GetAlignment());
                m_itemCODE->SetTextColor(m_colorForeground);
            }
            else
            {
                m_itemCODE->SetVisible(false);
                m_itemCODE->setVisible(false);
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
            fillLabelFields(grid, label_type);

            for (int i = 0; i < grid->count(); ++i) {

                if(dynamic_cast<CGUILabelText*>(grid->itemAt(i))->text() != "spec signal") {
                    dynamic_cast<CGUILabelText*>(grid->itemAt(i))->SetVisible(true);
                    dynamic_cast<QGraphicsItem*>(grid->itemAt(i))->setVisible(true);
                }
            }

            processFlightLevels(label_type);
        }
    }

    SetColor();
}

void CGUIAirplaneLabelExt::updateGrid()
{
    if (!m_pAirplane)
        return;

    if (m_pAirplane->isFlying())
    {
        if (m_pAirplane->isTransferTimeOut())
            m_pAirplane->MayBeCancelTransfer();

        QString sText;

        if (m_pAirplane->GetMode3AReply() == 0) {
            m_itemCODE->setText(tr("none"));
        } else {
            sText = m_pAirplane->GetMode3AReplyToString();
            if (!m_pAirplane->GetSFPL()){
                //sText += "/" + m_pAirplane->GetACID();
            } else {
                //automatic save CODE to Redis
               if(!(m_itemCODE->text() == sText))
                {
                    const QSharedPointer<CSFPL> &sfpl = m_pAirplane->GetSFPL();
                    sfpl->SetCODE(m_pAirplane->GetMode3AReply());
                    if (master()->getRedisDB()) {
                        bool ok = master()->getRedisDB()->saveSFPL(sfpl);
                        if (!ok)
                            qCritical() << "error saving CODE";
                }
             }
            }
            //-----------------------
            m_itemCODE->setText(sText);


        }

        //const CAirplaneData::PlaneType plane_type = m_pAirplane->GetPlaneType();
        const QSharedPointer<CSFPL> &sfpl = m_pAirplane->GetSFPL();

        processAcid(sfpl);
#ifdef QT_DEBUG
        if (sfpl) {
            item_cs_debug_->setText("cs " + sfpl->GetCurSector());
            item_ns_debug_->setText("ns " + sfpl->GetNextSector());
        }
#endif
        if (sfpl) {
            if(!sfpl->GetTypeFlight().isEmpty())
            {
                sText =sfpl->GetTypeFlight();
                m_itemType->setText(sText);
            }
            else sText = "Type";
            if(!sfpl->GetCFL().toString().isEmpty())
            {
                const double cfl = sfpl->GetCFL().GetF();
                sText =QString::number(cfl);
            }
            else sText = "CFL";
            m_itemCfl->setText(sText);
            if(!sfpl->GetTFL().toString().isEmpty())
            {
                const double tfl = sfpl->GetTFL().GetF();
                sText = QString::number(tfl);
            }
            else sText = "TFL";
            m_itemTfl->setText(sText);
            if(!sfpl->GetDepAerodrome().isEmpty())
            {
                sText =sfpl->GetDepAerodrome();
                m_itemAdep->setText(sText);
            }
            else sText = "-----";
            if(!sfpl->GetDestAerodrome().isEmpty())
            {
                sText =sfpl->GetDestAerodrome();
                m_itemAdes->setText(sText);
            }
            else sText = "-----";
            if(!sfpl->GetAlt1Aerodrome().isEmpty())
            {
                sText = sfpl->GetAlt1Aerodrome();
                m_itemAlter->setText(sText);
            }
            else sText = "-----";

            const QString &point = sfpl->GetCOPout();
            m_itemPoint->setText(point.isEmpty() ? "XXXXX" : point);

            if(!sfpl->GetEquipment().isEmpty())
                sText = sfpl->GetEquipment();
            else sText = "EQ";
            m_itemEq->setText(sText);

            if(!sfpl->GetLit().isEmpty())
                sText = sfpl->GetLit();
            else sText = "LIT";
            m_itemLit->setText(sText);

            if(!sfpl->GetRoute().isEmpty())
            {
                sText = sfpl->GetRoute();
                RouteMiddle = sText;
            }

            if(!sfpl->GetSTAR().isEmpty())
                sText = sfpl->GetSTAR();
            else sText = "STAR";
            m_itemStar->setText(sText);
        }

        if (!m_pAirplane->IsHeightQNH())
        {
            double dAFL = PressOnAirdromeFromSee(m_pAirplane->GetFL()) * TransformFeet();
            sText = QString("%1").arg(dAFL > 0 ? dAFL : 0,
                                      AppSettings::IsSISet(AppSettings::SIFeet) ? 3 : AppSettings::METER_CAPACITY, 'f', 0, QChar('0'));
            if (m_pAirplane->GetEchelon() > m_pAirplane->GetFL() && !m_pAirplane->isUvd())
                sText.prepend("A");
        }
        else
            sText = QString("A%1").arg(m_pAirplane->GetHeightQNH(), 3, 'f', 0, QChar('0'));
        m_itemAfl->setText(sText);

        if (sfpl) {
            const QString &vrc = sfpl->GetVrc();
            m_itemVrc->setText(vrc.isEmpty() ? "vrc" : vrc);
        }

        if (sfpl) {
            const QString &hdg = sfpl->GetHdg();
            m_itemHdg->setText(hdg.isEmpty() ? "hdg" : hdg + UTF8_QSTRING("\u00B0"));
        }
        //m_itemHDG->setText(QString::number(m_pAirplane->heading()));
        m_itemHdg->setText(QString::number(static_cast<int>(m_pAirplane->heading())));

        double dVal = m_pAirplane->GetTAS() * Transform_Knot_SI();
        if (AppSettings::IsSISet(AppSettings::SIKnot))
            sText.sprintf("K%03.0f", dVal );
        else
            sText.sprintf("%03.0f", dVal);
        m_itemTas->setText(dVal ? sText : "TAS");

        const QString ia = (m_pAirplane->indicatedAirspeed() ? "K"+QString::number(m_pAirplane->indicatedAirspeed()) : QString("-"));
        const QString iam = ((m_pAirplane->indicatedAirspeedMach() > 0.000001) ? QString::number(m_pAirplane->indicatedAirspeedMach(), 'f', 2) : QString("-"));
        //item_spd_->setText(QString("%1/%2").arg(ia).arg(iam));
        m_itemIask->setText(QString("%1").arg(ia));
        m_itemIasm->setText(QString("%1").arg(iam));

        m_itemVrc->setText(QString::number(static_cast<int>(m_pAirplane->verticalRate())));

        double dVal_gsp = m_pAirplane->groundAirspeed()*Transform_Nms_SI();
        if (AppSettings::IsSISet(AppSettings::SIKnot))
            sText.sprintf("K%03.0f", dVal_gsp );
        else
            sText.sprintf("%03.0f", dVal_gsp);
        m_itemGsp->setText(dVal_gsp ? sText : "GSP"  );


        sText.sprintf("%03.0f",m_pAirplane->sfl()/100.);
        m_itemSfl->setText(m_pAirplane->sfl() ? sText : "SFL" );

        m_itemBps->setText(m_pAirplane->IsBPS() ? QString::number(m_pAirplane->GetBPS() + 800) : "BPS");       
        m_itemAcas->setText(m_pAirplane->IsACAS() ? m_pAirplane->GetACAS().toString("HH:mm") : "ACAS");
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
    SetColor();
    adjustGrid();
}

void CGUIAirplaneLabelExt::adjustGrid()
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

    update();
}

void CGUIAirplaneLabelExt::showGrid(bool bShow)
{
    loadTypeGrid(bShow);
    m_bFullShow = bShow;
    adjustGrid();
}

void CGUIAirplaneLabelExt::SetAirplane(CGUIAirplane *pPlane)
{
    m_pAirplane = pPlane;

}

void CGUIAirplaneLabelExt::onRejected()
{
    showGrid(false);
}

void CGUIAirplaneLabelExt::SetColor()
{
    if (m_pAirplane && m_pAirplane->isFlying())
    {
        foreach (QGraphicsItem *child_item, childItems()) {
            if (CGUILabelText *field = dynamic_cast<CGUILabelText*>(child_item))
                field->SetTextColor(m_colorForeground);
        }
        item_spec_signal_1->SetTextColor(Qt::yellow);
        item_spec_signal_1->setBackgroundColor(Qt::red);
        item_spec_signal_2->SetTextColor(Qt::yellow);
        item_spec_signal_2->setBackgroundColor(Qt::red);
        item_spec_signal_3->SetTextColor(Qt::yellow);
        item_spec_signal_3->setBackgroundColor(Qt::red);

        updateHdgSpdVrcCflFieldsColor();
    }
}

double CGUIAirplaneLabelExt::TransformM_SEC()
{
    if (AppSettings::IsSISet(AppSettings::SIKnot))
        // m/s -> knots
        return 1 / 0.5144;
    else
        // m/s -> km/h
        return 3.6;
}

double CGUIAirplaneLabelExt::Transform_Nms_SI() // convert Nm/s -> SI(knot or km/h)
{
    if (AppSettings::IsSISet(AppSettings::SIKnot))
        // Nm/s -> knots
        return 3600;
    else
        // Nm/s -> km/h
        return 3600*1.852;
}

double CGUIAirplaneLabelExt::Transform_Knot_SI() // convert Knot -> SI(knot or km/h)
{
    if (AppSettings::IsSISet(AppSettings::SIKnot))
        // Knot -> knots
        return 1;
    else
        // Knot -> km/h
        return 1.852;
}

double CGUIAirplaneLabelExt::TransformFeet(bool bHundredMeters/* = false*/)
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

double CGUIAirplaneLabelExt::PressOnAirdromeFromSee(double FL) const
{
    double height = FL;
    if (m_pAirplane->GetEchelon() > FL && !m_pAirplane->isUvd())
    {
        FL *= 100 * 0.3048; // hundred feet -> meters

        height = FL + 8.25*(m_pAirplane->GetPress()-1013);
        height /= 100 * 0.3048;
    }
    return height;
}

void CGUIAirplaneLabelExt::SetActivePlane(bool bActive)
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

void CGUIAirplaneLabelExt::OnActivePlane()
{
    return; // removed flashing
    if (m_colorActive == m_color)
        m_colorActive = Qt::white;
    else
        m_colorActive = m_color;
    update();
}

void CGUIAirplaneLabelExt::OnTimerAssume()
{
    //WriteLog("NS", "ASSUME");
    m_pAirplane->AssumeOperator();
    SetColor();
}

void CGUIAirplaneLabelExt::UpdateXFont()
{
    QMetaObject::invokeMethod(this, "adjustGrid", Qt::QueuedConnection);
}

void CGUIAirplaneLabelExt::UpdateFormularType()
{
    SetGridLabel(XGridLabel());
    if (m_itemCODE || m_groundItemACID)
    {
        loadTypeGrid(m_bFullShow);
        updateGrid();
    }
}

void CGUIAirplaneLabelExt::setLabelColors(XGridLabel::LabelType label_type)
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
        m_GridLabel.SetForeground(stg.GetColor(UserProfile::CtUnrelated));
        m_GridLabel.SetBackground(stg.GetColor(UserProfile::CtUnrelated_back));
        m_colorForeground = stg.GetColor(UserProfile::CtUnrelated);
        m_colorBackground = stg.GetColor(UserProfile::CtUnrelated_back);
    break;

    default:
    break;
    }

}

void CGUIAirplaneLabelExt::onSpiTimeout()
{
    spi_timer_->stop();
    loadTypeGrid(m_bFullShow);
}

void CGUIAirplaneLabelExt::onEmgTimeout()
{
    emg_timer_->stop();
    loadTypeGrid(m_bFullShow);
}



void CGUIAirplaneLabelExt::fillLabelFields(QGraphicsGridLayout *grid, XGridLabel::LabelType label_type)
{
    int row = 0;
#ifdef QT_DEBUG
    grid->addItem(item_cs_debug_, row, 0, 1, 2, Qt::AlignLeft);
    grid->addItem(item_ns_debug_, row, 2, 1, 1, Qt::AlignLeft);

    ++row;
#endif // QT_DEBUG

    bool end = false;
    for (int i = 0; i < 4; ++i) {
        if (end)
            break;
        if(i == 1)
            i++;

        switch(i) {
        case 0:
        {   if (CAirplane::sListSpecCodes.contains(m_pAirplane->GetMode3AReplyToString())) {
                if(!(m_pAirplane->GetSPI() || spi_timer_->isActive()) &&
                        (!m_pAirplane->isSTCA()) &&
                        m_pAirplane->meteoWarning().isEmpty() &&
                        m_pAirplane->restrictionWarning().isEmpty())
                    end = true;
                item_spec_signal_1->setText(m_pAirplane->GetMode3AReplyToString());

            } else if (m_pAirplane->GetSPI() || spi_timer_->isActive()) {
                if(!(m_pAirplane->dbl()) &&
                        (!m_pAirplane->isSTCA()) &&
                        (m_pAirplane->meteoWarning().isEmpty()) &&
                        (m_pAirplane->restrictionWarning().isEmpty()))
                    end = true;
                item_spec_signal_1->setText("SPI");

            } else if (m_pAirplane->dbl() && m_pAirplane->GetMode3AReplyToString() != "7000" && m_pAirplane->GetMode3AReplyToString() != "2000") {
                if((!m_pAirplane->isSTCA()) &&
                    (m_pAirplane->meteoWarning().isEmpty()) &&
                    (m_pAirplane->restrictionWarning().isEmpty()))
                   end = true;
                item_spec_signal_1->setText("DBL " + m_pAirplane->GetMode3AReplyToString());

            } else if (m_pAirplane->isSTCA()) {
                if((m_pAirplane->meteoWarning().isEmpty()) &&
                    (m_pAirplane->restrictionWarning().isEmpty()))
                end = true;
                item_spec_signal_1->setText("STCA");

            } else if (!m_pAirplane->meteoWarning().isEmpty()) {
                if((m_pAirplane->restrictionWarning().isEmpty()))
                    end = true;
                item_spec_signal_1->setText(m_pAirplane->meteoWarning());

            } else if (!m_pAirplane->restrictionWarning().isEmpty()) {
                end = true;
                item_spec_signal_1->setText(m_pAirplane->restrictionWarning());

            } else {
                end = true;
                break;
            }
            item_spec_signal_1->setVisible(true);
            item_spec_signal_1->SetVisible(true);
            grid->addItem(item_spec_signal_1, row, i, 1, 2, Qt::AlignLeft);
            break;
        }
        case 2:
        {
            if ((m_pAirplane->GetSPI() || spi_timer_->isActive()) && item_spec_signal_1->text() != "SPI") {
                if(!(m_pAirplane->dbl()) &&
                        (!m_pAirplane->isSTCA()) &&
                        (m_pAirplane->meteoWarning().isEmpty()) &&
                        (m_pAirplane->restrictionWarning().isEmpty()))
                    end = true;
                item_spec_signal_2->setText("SPI");
            } else if (m_pAirplane->dbl() && m_pAirplane->GetMode3AReplyToString() != "7000" && m_pAirplane->GetMode3AReplyToString() != "2000" &&
                       item_spec_signal_1->text() != ("DBL " + m_pAirplane->GetMode3AReplyToString())) {
                if((!m_pAirplane->isSTCA()) &&
                    (m_pAirplane->meteoWarning().isEmpty()) &&
                    (m_pAirplane->restrictionWarning().isEmpty()))
                   end = true;
                item_spec_signal_2->setText("DBL " + m_pAirplane->GetMode3AReplyToString());

            } else if (m_pAirplane->isSTCA() && item_spec_signal_1->text() != "STCA") {
                if((m_pAirplane->meteoWarning().isEmpty()) &&
                    (m_pAirplane->restrictionWarning().isEmpty()))
                end = true;
                item_spec_signal_2->setText("STCA");

            } else if (!m_pAirplane->meteoWarning().isEmpty() && item_spec_signal_1->text() != m_pAirplane->meteoWarning()) {
                if((m_pAirplane->restrictionWarning().isEmpty()))
                    end = true;
                item_spec_signal_2->setText(m_pAirplane->meteoWarning());

            } else if (!m_pAirplane->restrictionWarning().isEmpty() && item_spec_signal_1->text() != m_pAirplane->restrictionWarning()) {
                end = true;
                item_spec_signal_2->setText(m_pAirplane->restrictionWarning());

            } else {
                end = true;
                break;
            }
            item_spec_signal_2->setVisible(true);
            item_spec_signal_2->SetVisible(true);
            grid->addItem(item_spec_signal_2, row, i, 1, 2, Qt::AlignLeft);
            break;
        }
        case 3:
        {   if (((m_pAirplane->dbl() && m_pAirplane->GetMode3AReplyToString() != "7000" && m_pAirplane->GetMode3AReplyToString() != "2000") &&
                item_spec_signal_2->text() != ("DBL " + m_pAirplane->GetMode3AReplyToString()))) {
                item_spec_signal_3->setText("DBL " + m_pAirplane->GetMode3AReplyToString());

            } else if (m_pAirplane->isSTCA() && item_spec_signal_2->text() != "STCA") {
                item_spec_signal_3->setText("STCA");

            } else if (!m_pAirplane->meteoWarning().isEmpty() && item_spec_signal_2->text() != m_pAirplane->meteoWarning()) {
                item_spec_signal_3->setText(m_pAirplane->meteoWarning());

            } else if (!m_pAirplane->restrictionWarning().isEmpty() && item_spec_signal_2->text() != m_pAirplane->restrictionWarning()) {
                item_spec_signal_3->setText(m_pAirplane->restrictionWarning());

            } else {
                end = true;
                break;
            }
            item_spec_signal_3->setVisible(true);
            item_spec_signal_3->SetVisible(true);
            grid->addItem(item_spec_signal_3, row, i, 1, 2, Qt::AlignLeft);
            break;
        }
        default:
        break;
        }

    }


    ++row;

    grid->addItem(m_itemACID, row, 0, 1, 2, Qt::AlignLeft);
    //grid->addItem(m_itemSpace, row, 1, 1, 1, Qt::AlignLeft);
    grid->addItem(m_itemCODE, row, 2, 1, 1, Qt::AlignRight);
    grid->addItem(m_itemType, row, 3, 1, 1, Qt::AlignRight);

    ++row;

    grid->addItem(m_itemAfl, row, 0, 1, 1, Qt::AlignLeft);
    grid->addItem(m_itemCfl, row, 2, 1, 1, Qt::AlignRight);
    grid->addItem(m_itemTfl, row, 3, 1, 1, Qt::AlignRight);

    ++row;

    grid->addItem(m_itemAdep, row, 0, 1, 1, Qt::AlignLeft);
    grid->addItem(m_itemAdes, row, 2, 1, 1, Qt::AlignRight);
    grid->addItem(m_itemAlter, row, 3, 1, 1, Qt::AlignRight);
    //if(m_bPressGsp)
    //grid->addItem(item_gsp_, row, 3, 1, 1, Qt::AlignRight);//

    ++row;

    grid->addItem(m_itemPoint, row, 0, 1, 1, Qt::AlignLeft);
    grid->addItem(m_itemEq, row, 2, 1, 1, Qt::AlignRight);
    grid->addItem(m_itemLit, row, 3, 1, 1, Qt::AlignRight);

    ++row;

    grid->addItem(m_itemRoute, row, 0, 1, 1, Qt::AlignLeft);
    grid->addItem(m_itemStar, row, 3, 1, 1, Qt::AlignRight);

    ++row;

    grid->addItem(m_itemIask, row, 0, 1, 2, Qt::AlignLeft);
    grid->addItem(m_itemIasm, row, 2, 1, 1, Qt::AlignRight);
    grid->addItem(m_itemGsp, row, 3, 1, 1, Qt::AlignRight);

   /* ++row;

    grid->addItem(item_gsp_, row, 0, 1, 2, Qt::AlignLeft);
    grid->addItem(m_itemSpace, row, 2, 1, 1, Qt::AlignLeft);
    grid->addItem(item_w_, row, 3, 1, 1, Qt::AlignRight);*/

    ++row;

    grid->addItem(m_itemHdg, row, 0, 1, 2, Qt::AlignLeft);
    grid->addItem(m_itemTas, row, 2, 1, 1, Qt::AlignRight);
    grid->addItem(m_itemVrc, row, 3, 1, 1, Qt::AlignRight);

    ++row;

    grid->addItem(m_itemBps, row, 0, 1, 2, Qt::AlignLeft);
    grid->addItem(m_itemSfl, row, 2, 1, 1, Qt::AlignRight);
    grid->addItem(m_itemAcas, row, 3, 1, 1, Qt::AlignRight);



}

void CGUIAirplaneLabelExt::processFlightLevels(XGridLabel::LabelType label_type)
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

    const double afl = m_pAirplane->GetFL();
    const double tfl = m_pAirplane->GetTFL().GetF();

    bool cfl_visible = false;
    bool tfl_visible = false;

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
}

void CGUIAirplaneLabelExt::processAcid(const QSharedPointer<CSFPL> &sfpl)
{
    m_itemACID->setText("ACID");

    if (sfpl)
        m_itemACID->setText(sfpl->GetACID());
    else {
            if (!m_pAirplane->GetACID().isEmpty())
                m_itemACID->setText(m_pAirplane->GetACID());
            else
                m_itemACID->setText("-------");
    }

    if (m_pAirplane->isUvd()) {
        m_itemACID->setBackgroundColor(qRgb(0, 200, 0));
    }
}

bool CGUIAirplaneLabelExt::isAssumed() const
{
    bool bRes(false);
    if (m_pAirplane) {
        const CAirplaneData::PlaneType plane_type = m_pAirplane->GetPlaneType();
        if ((CAirplaneData::ASSUME == plane_type) || (CAirplaneData::ActOutInFir == plane_type)/* || (CAirplaneData::ActOut == plane_type)*/)
            bRes = true;
    }
    return bRes;
}

CGUILabelText * CGUIAirplaneLabelExt::fieldByPos(const QPointF &pos) const
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

void CGUIAirplaneLabelExt::updateHdgSpdVrcCflFieldsColor()
{
    /*const QSharedPointer<CSFPL> &sfpl = m_pAirplane->GetSFPL();
    if (sfpl) {
        bool ok;

        const QString sAcid = sfpl->GetACID().trimmed();
        const QString aAcid = m_pAirplane->GetACID().trimmed();
        if (!aAcid.isEmpty())
            m_itemACID->setFrame();
        if (!aAcid.isEmpty() && sAcid != aAcid) {
            m_itemACID->setBackgroundColor(Qt::yellow);
        }
        if (m_pAirplane && m_pAirplane->IsLastActive())
        {
            QColor color = QColor(Qt::green);
             m_itemACID->SetTextColor(color);
        }
    }*/
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();
    m_itemIask->SetTextColor(stg.GetColor(UserProfile::CtExtended));
    m_itemIasm->SetTextColor(stg.GetColor(UserProfile::CtExtended));
    m_itemGsp->SetTextColor(stg.GetColor(UserProfile::CtExtended));
    m_itemHdg->SetTextColor(stg.GetColor(UserProfile::CtExtended));
    m_itemTas->SetTextColor(stg.GetColor(UserProfile::CtExtended));
    m_itemVrc->SetTextColor(stg.GetColor(UserProfile::CtExtended));
    m_itemBps->SetTextColor(stg.GetColor(UserProfile::CtExtended));
    m_itemSfl->SetTextColor(stg.GetColor(UserProfile::CtExtended));
    m_itemAcas->SetTextColor(stg.GetColor(UserProfile::CtExtended));
}

