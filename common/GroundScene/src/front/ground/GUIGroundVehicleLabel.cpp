#include "GUIGroundVehicleLabel.h"
#include "back/ground/GroundMain.h"
#include "GUIGroundVehicle.h"
#include <QPainter>
#include <QBitmap>
#include <QGraphicsScene>
#include <QPushButton>
#include <QGraphicsGridLayout>
#include <QGraphicsSceneMouseEvent>

#include <QGraphicsLinearLayout>

#ifndef GROUND_CWP
#include "back/proto/obelix/CmdVehicleControl.h"
#include "Simulator.h"
#else
#include <QApplication>
#endif

const char* GUIGroundVehicleLabel::BACKGROUND_STYLE = "background-color: lightGray;";

GUIGroundVehicleLabel::GUIGroundVehicleLabel(QGraphicsItem *parent)
    : QGraphicsWidget(parent)
    , m_iPenWidth(1), m_color(Qt::black), m_backgroundBrush(Qt::lightGray)
    , m_bMoving(false), m_bCheck(false), m_bAutoCollapse(true), m_bFullShow(false), m_iAzimuth2(361)
    , mVehicle(0), mItemCheck(0), mItemName(0), m_parent(0)
{
    setZValue(60);
    setFlag(ItemIsMovable, true);
    setFlag(ItemDoesntPropagateOpacityToChildren, true);
    setAcceptedMouseButtons(Qt::LeftButton|Qt::RightButton);
    setOpacity(0.75);
    setAcceptHoverEvents(true);
}

GUIGroundVehicleLabel::~GUIGroundVehicleLabel()
{
}

QRectF GUIGroundVehicleLabel::boundingRect() const
{
    if(!mVehicle) return QRectF();
    QPainterPath paintPath;
    paintPath.addRect(m_clRect);
    if(!m_clRect.contains(mapFromParent(mVehicleParentPoint)))
    {
        paintPath.moveTo(mapFromParent(mVehicleParentPoint));
        paintPath.lineTo(intersectRect(QLineF(mapFromParent(mVehicleParentPoint), m_clRect.center()), m_clRect));
    }
    QRectF rect(paintPath.boundingRect());
    rect.adjust(-1,-1,1,1);
    return rect;
}

QPainterPath GUIGroundVehicleLabel::shape() const
{
    if(!mVehicle) return QPainterPath();
    QPainterPath paintPath;
    paintPath.addRect(m_clRect);
    return paintPath;
}

void GUIGroundVehicleLabel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(!mVehicle) return;

    painter->save();
    QColor color(m_color);
    QPen qPen(color);
    qPen.setWidth(m_iPenWidth);
    painter->setPen(qPen);
    painter->setBrush(m_backgroundBrush);
    painter->drawRect(m_clRect);
    if(!m_clRect.contains(mapFromParent(mVehicleParentPoint)))
        painter->drawLine(mapFromParent(mVehicleParentPoint), intersectRect(QLineF(mapFromParent(mVehicleParentPoint), m_clRect.center()), m_clRect));
    painter->restore();
}

void GUIGroundVehicleLabel::UpdatePosition(const QPointF point)
{
    mVehicleParentPoint = point;

    if(!mItemName)
    {
        initGrid();
        loadTypeGrid(m_bFullShow);
    }
    updateGrid();
}

void GUIGroundVehicleLabel::SetVehicle(GUIGroundVehicle *pVehicle)
{
    mVehicle = pVehicle;
}

void GUIGroundVehicleLabel::SetPoint(const CCoord *coord)
{
#ifndef GROUND_CWP
    QSharedPointer<atd::CPoint> point(QSharedPointer<atd::CPoint>(new atd::CPoint));
    point.data()->SetCoord(*coord);
    point.data()->SetName(coord->GetAsString());
    mNextPointCoord = point;
    mItemRoute->setText(tr("Point"));
    //mItemRoute->setText(coord->GetAsString());
#endif    
    adjustGrid();
}

QPointF GUIGroundVehicleLabel::intersectRect(const QLineF line, const QRectF rect) const
{
    QPointF point;
    if(line.intersect(QLineF(rect.topLeft(), rect.topRight()), &point) == QLineF::BoundedIntersection) return point;
    if(line.intersect(QLineF(rect.topRight(), rect.bottomRight()), &point) == QLineF::BoundedIntersection) return point;
    if(line.intersect(QLineF(rect.bottomRight(), rect.bottomLeft()), &point) == QLineF::BoundedIntersection) return point;
    if(line.intersect(QLineF(rect.bottomLeft(), rect.topLeft()), &point) == QLineF::BoundedIntersection) return point;
    return point;
}

void GUIGroundVehicleLabel::SetColor()
{
}

void GUIGroundVehicleLabel::showGrid(bool bShow)
{
    loadTypeGrid(bShow);
    m_bFullShow = bShow;
    adjustGrid();
}

void GUIGroundVehicleLabel::adjustGrid()
{
    prepareGeometryChange();

    if(layout())
    {
//        QGraphicsGridLayout* pGrid = dynamic_cast<QGraphicsGridLayout*>(layout());
//        pGrid->setColumnPreferredWidth(0, mItemRoute->boundingRect().width());
        layout()->invalidate();
        layout()->activate();
    }

    adjustSize();



    QRectF clRect = rect();
    clRect.adjust(-1,-1,1,1);
    m_clRect = clRect.normalized();

    update();
}

void GUIGroundVehicleLabel::updateGrid()
{
    if(!mVehicle)
        return;

    // NAME
    if(!mVehicle->name().isEmpty())
        mItemName->setText(mVehicle->name());
    else
        mItemName->setText(QString("<< Car #%1 >>").arg(mVehicle->GetID()));

    // SPEED
    QString sText;
    double dIAS = mVehicle->GetIAS() * TransformM_SEC();
    sText.sprintf("%03.0f km/h", dIAS);
    mItemSpeed->setText(sText);

    switchButtons(dIAS != 0);

    if(mVehicle->isHidden())
    {
#ifdef GROUND_CWP
        //hide();
#else
        m_bCheck = true;
        QPixmap pixMap(QString(m_bCheck ? QL1S(":/PressIcons/Check") : QL1S(":/PressIcons/Uncheck")));
        pixMap.setMask(pixMap.createHeuristicMask());
        mItemCheck->setPixmap(pixMap);
#endif
    }
    else
    {
#ifdef GROUND_CWP
        //show();
#else
        m_bCheck = false;
        QPixmap pixMap(QString(m_bCheck ? QL1S(":/PressIcons/Check") : QL1S(":/PressIcons/Uncheck")));
        pixMap.setMask(pixMap.createHeuristicMask());
        mItemCheck->setPixmap(pixMap);
#endif
    }

    SetColor();
    adjustGrid();
}

void GUIGroundVehicleLabel::initGrid()
{
    mItemName = new CGUILabelText(this);
    mItemName->setText(QString("Vehicle"));

    mItemSpeed = new CGUILabelText(this);
    mItemSpeed->setText(QString("Speed"));
#ifndef GROUND_CWP
    mItemSpeed->SetReadOnly(false);
    mItemSpeed->SetTypeDlg(CGUIControlDlg::GroundSpeedType);
    connect(mItemSpeed, SIGNAL( endEdit(const CGUILabelText*, const QStringList) ),
                    this, SLOT( OnEndEdit(const CGUILabelText*, const QStringList) ));

    mItemCheck = new CGUIPixmapItem(this);
    QPixmap pixMap = QPixmap(QString(m_bCheck ? QL1S(":/PressIcons/Check") : QL1S(":/PressIcons/Uncheck")));
    pixMap.setMask(pixMap.createHeuristicMask());
    mItemCheck->setToolTip(QString("Make hidden for CWP's"));
    mItemCheck->setPixmap(pixMap);

    mItemRoute = new CGUILabelText(this);
    mItemRoute->SetReadOnly(false);
    mItemRoute->SetTypeDlg(CGUIControlDlg::GroundRouteType);
    mItemRoute->setText(tr("Route"));
    connect(mItemRoute, SIGNAL( endEdit(const CGUILabelText*, const QStringList) ),
                    this, SLOT( OnEndEdit(const CGUILabelText*, const QStringList) ));

    QPushButton* button = new QPushButton(QString(), 0);
    button->setStyleSheet(BACKGROUND_STYLE);
    button->setIcon(QIcon(QString(":/FormularIcons/Start")));
    button->setMinimumWidth(30);
    m_groundButtonMove = scene()->addWidget(button);
    connect(button, SIGNAL(clicked()), this, SLOT(onMove()));
    m_groundButtonMove->setVisible(false);

    button = new QPushButton(QString(), 0);
    button->setStyleSheet(BACKGROUND_STYLE);
    button->setIcon(QIcon(QString(":/FormularIcons/Return")));
    button->setMinimumWidth(30);
    m_groundButtonReverse = scene()->addWidget(button);
    connect(button, SIGNAL(clicked()), this, SLOT(onReverse()));
    m_groundButtonReverse->setVisible(false);

    button = new QPushButton(QString(), 0);
    button->setStyleSheet(BACKGROUND_STYLE);
    button->setIcon(QIcon(QString(":/FormularIcons/Stop")));
    button->setMinimumWidth(30);
    m_groundButtonStop = scene()->addWidget(button);
    connect(button, SIGNAL(clicked()), this, SLOT(onStop()));
    m_groundButtonStop->setVisible(false);

    mButtonFrame = new QGraphicsWidget(this);
    QGraphicsLinearLayout *lay = new QGraphicsLinearLayout();
    lay->setOrientation(Qt::Horizontal);
    lay->addItem(m_groundButtonMove);
    lay->addItem(m_groundButtonStop);
    lay->addItem(m_groundButtonReverse);
    lay->setContentsMargins(0,0,0,0);
    lay->setSpacing(2);
    mButtonFrame->setLayout(lay);

#endif
    QGraphicsGridLayout* pGrid = new QGraphicsGridLayout(this);
    pGrid->setHorizontalSpacing(1);
    pGrid->setVerticalSpacing(5);
    pGrid->setContentsMargins(2,2,2,2);

    setLayout(pGrid);
}

void GUIGroundVehicleLabel::loadTypeGrid(bool bFullShow)
{
    QGraphicsGridLayout* pGrid = dynamic_cast<QGraphicsGridLayout*>(layout());
    if(pGrid && (!pGrid->count() || bFullShow != m_bFullShow))
    {
        QList<QGraphicsItem*> listWidgets = childItems();
        QList<QGraphicsItem*>::iterator itList = listWidgets.begin();
        for(; itList != listWidgets.end(); ++itList)
        {
            (*itList)->setVisible(false);
        }

        for(int i = pGrid->count()-1; i >= 0; --i) pGrid->removeAt(i);

        if(mVehicle)
        {
#ifndef GROUND_CWP
            if(bFullShow)
            {
                pGrid->addItem(mItemName,               0,0,1,3, Qt::AlignLeft);
                pGrid->addItem(mItemCheck,              0,2,1,1, Qt::AlignRight);

                pGrid->addItem(mButtonFrame,            1,0,1,3, Qt::AlignCenter);

                pGrid->addItem(mItemRoute,              2,0,1,1, Qt::AlignLeft);
                pGrid->addItem(mItemSpeed,              2,1,1,2, Qt::AlignRight);
            }
            else
            {
                pGrid->addItem(mItemName,               0,0,1,1, Qt::AlignLeft);
                pGrid->addItem(mItemSpeed,              1,0,1,1, Qt::AlignLeft);
            }
#else
            pGrid->addItem(mItemName,      0,0,1,2, Qt::AlignHCenter);
            pGrid->addItem(mItemSpeed,     1,0,1,1, Qt::AlignHCenter);
#endif
            pGrid->setHorizontalSpacing(5);

            for(int i = 0; i < pGrid->count(); ++i)
            {
                if(dynamic_cast<CGUILabelText*>(pGrid->itemAt(i)))
                    dynamic_cast<CGUILabelText*>(pGrid->itemAt(i))->setVisible(true);
                else
                {
                    QGraphicsWidget* pProxyItem = dynamic_cast<QGraphicsWidget*>(pGrid->itemAt(i));
                    if(pProxyItem)
                    {
                        pProxyItem->setVisible(true);
                        for(int j = 0; j < pProxyItem->layout()->count(); ++j)
                        {
                            if(dynamic_cast<QGraphicsWidget*>(pProxyItem->layout()->itemAt(j)))
                                dynamic_cast<QGraphicsWidget*>(pProxyItem->layout()->itemAt(j))->setVisible(true);
                        }
                    }
                }
            }
            if(mItemCheck && bFullShow)
                mItemCheck->setVisible(true);
        }
    }
}

double GUIGroundVehicleLabel::TransformFeet(bool bHundredMeters)
{
    if(bHundredMeters)
        // hundred feet -> hundred meters
        return 0.3048;
    else
        // hundred feet -> tens of meters
        return 0.3048 * 10;
}

void GUIGroundVehicleLabel::switchButtons(bool isMoving)
{
#ifndef GROUND_CWP
    if(isMoving)
    {
        (dynamic_cast<QPushButton*>(m_groundButtonReverse->widget()))->setEnabled(false);
        (dynamic_cast<QPushButton*>(m_groundButtonStop->widget()))->setEnabled(true);
        (dynamic_cast<QPushButton*>(m_groundButtonMove->widget()))->setEnabled(false);
    }
    else
    {
        (dynamic_cast<QPushButton*>(m_groundButtonReverse->widget()))->setEnabled(true);
        (dynamic_cast<QPushButton*>(m_groundButtonStop->widget()))->setEnabled(false);
        (dynamic_cast<QPushButton*>(m_groundButtonMove->widget()))->setEnabled(!mVehicle->isAtEnd());
    }
#endif
}

double GUIGroundVehicleLabel::TransformM_SEC()
{
    return 3.6;
}

void GUIGroundVehicleLabel::OnUpdateLabel()
{
    UpdatePosition(mVehicleParentPoint);
}

void GUIGroundVehicleLabel::OnUpdateFont()
{
    QFont font1 = qApp->font();
    QList<QGraphicsItem*> listWidgets = childItems();
    QList<QGraphicsItem*>::iterator itList = listWidgets.begin();
    for(; itList != listWidgets.end(); ++itList)
    {
        CGUILabelText* pItem = dynamic_cast<CGUILabelText*>(*itList);
        if(pItem)
            pItem->setFont(font1);
    }
    UpdateLabel();
}

void GUIGroundVehicleLabel::Translate()
{
#ifndef GROUND_CWP
    if(mItemName)
    {
        if(mNextPointCoord)
            mItemRoute->setText(tr("Point"));
        else
            mItemRoute->setText(tr("Route"));
    }
#endif
}

void GUIGroundVehicleLabel::onMove()
{
#ifndef GROUND_CWP
    if(mVehicle && !mVehicle->isAtEnd())
        mVehicle->makeMove();
#endif
}

void GUIGroundVehicleLabel::onReverse()
{
#ifndef GROUND_CWP
    if(mVehicle)
        mVehicle->makeReverse();
#endif
}

void GUIGroundVehicleLabel::onStop()
{
#ifndef GROUND_CWP
    if(mVehicle)
        mVehicle->makeStop();
#endif
}

void GUIGroundVehicleLabel::OnEndEdit(const CGUILabelText *sender, const QStringList sValues)
{
#ifndef GROUND_CWP
    if(mVehicle)
    {
        if(sender == mItemSpeed && !mVehicle->isAtEnd())
        {
            double dV = sValues.at(0).toDouble();
            if(dV < 20)
                dV = 20;

            mItemSpeed->setText(sValues.at(0));
            mVehicle->ChangeV(dV / TransformM_SEC());
        }
        else if(sender == mItemRoute)
        {
            if(sValues.at(0).isEmpty())
            {
                mVehicle->GetVehicleNewCoord();
            }
            else
            {
                mVehicle->ChangeNextPoint(mNextPointCoord);//sValues.at(0));
            }
        }
    }
    adjustGrid();
#endif
}

void GUIGroundVehicleLabel::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsWidget::mouseReleaseEvent(event);

    if(event->button() == Qt::RightButton)
    {
        if(mItemName)
        {
            if(mVehicle)
            {
                mVehicle->ShowPathPoints(false);
            }
        }
    }
#ifndef GROUND_CWP
    if((event->button() == Qt::LeftButton) && !m_bMoving)
    {
        QPointF point = event->pos();

        CGUILabelText* pItem = 0;
        CGUIPixmapItem* pPressItem = 0;
        QList<QGraphicsItem*> listWidgets = childItems();
        QList<QGraphicsItem*>::const_iterator itList = listWidgets.begin();
        for(; itList != listWidgets.end(); ++itList)
        {
            QRectF rect = (*itList)->boundingRect();
            rect = (*itList)->mapRectToParent(rect);
            if(rect.contains(point))
            {
                pItem = dynamic_cast<CGUILabelText*>(*itList);
                if(pItem)
                {
                    if(!pItem->isVisible())
                        pItem = 0;
                    else
                        break;
                }
                pPressItem = dynamic_cast<CGUIPixmapItem*>(*itList);
                if(pPressItem)
                {
                    if(!pPressItem->isVisible())
                        pPressItem = 0;
                    else
                        break;
                }
            }
        }
        if(mVehicle)
        {
            QStringList sValues1, sValues2, sValues3;
            QString sText;

            if(pPressItem == mItemCheck)
            {
                m_bCheck = !m_bCheck;
                QPixmap pixMap(QString(m_bCheck ? QL1S(":/PressIcons/Check") : QL1S(":/PressIcons/Uncheck")));
                pixMap.setMask(pixMap.createHeuristicMask());
                mItemCheck->setPixmap(pixMap);
                mVehicle->setIsHidden(m_bCheck);
                QSharedPointer<obelix::CCmdVehicleControl> pCmd(new obelix::CCmdVehicleControl(mVehicle->GetID()));
                pCmd->SetHidden(m_bCheck);
                simulator()->GetObClient().XMLSend(pCmd.staticCast<CXMLCmd>());
            }


            if(pItem == mItemSpeed)
            {
                int iMin = 20;
                int iMax = 60;

                for(int i = iMax; i >= iMin; i -= 5)
                {
                    sText.sprintf("%03d", i);
                    sValues1 << sText;
                }
                int iVal = pItem->text().toInt();
                sText.sprintf("%03d", iVal);
            }
            else if(pItem == mItemRoute)
            {
                sText = pItem->text();
            }

            if(pItem)
            {
                pItem->SetValues(sValues1, sValues2, sValues3, sText);
                pItem->OnChoose();
            }
        }
    }
#endif
}

void GUIGroundVehicleLabel::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsWidget::mouseDoubleClickEvent(event);
}

void GUIGroundVehicleLabel::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    showGrid(true);
    if(mVehicle)
        mVehicle->setZValue(mVehicle->zValue() + 1);
    QGraphicsWidget::hoverEnterEvent(event);
}

void GUIGroundVehicleLabel::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if(m_bAutoCollapse)
        showGrid(false);

    if(mVehicle)
        mVehicle->setZValue(mVehicle->zValue() - 1);
    QGraphicsWidget::hoverLeaveEvent(event);
}

bool GUIGroundVehicleLabel::event(QEvent *event)
{
    bool bRes = QGraphicsWidget::event(event);
    if(event->type() == QEvent::FontChange)
    {
        QMetaObject::invokeMethod(this, "OnUpdateFont", Qt::QueuedConnection);
    }
    return bRes;
}

void GUIGroundVehicleLabel::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
#ifndef GROUND_CWP
    if(event->button() == Qt::LeftButton)
    {
        event->accept();
        return;
    }

    if(event->button() == Qt::RightButton)
    {
        QPointF point = event->pos();
        if(mItemName)
        {
            QRectF rect = mItemRoute->boundingRect();
            rect = mItemRoute->mapRectToParent(rect);
            if(rect.contains(point))
            {
                if(mVehicle)
                    mVehicle->ShowPathPoints(true);
            }
        }
    }
#endif
}


void GUIGroundVehicleLabel::SetAutoCollapse(bool bCollapse)
{
    m_bAutoCollapse = bCollapse;
    if(mVehicle)
    {
        showGrid(!bCollapse);
    }
    else
        m_bFullShow = !bCollapse;
}


void GUIGroundVehicleLabel::UpdateLabel()
{
    UpdatePosition(mVehicleParentPoint);
}
