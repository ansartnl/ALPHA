#include "TracerPoint.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include "front/core/GUIPoint.h"
#include "main.h"

CTracerPoint::CTracerPoint(ITracerHandler* pTracer, const QString& sName, const CCoord& coord, PointType type, QGraphicsItem* parent)
    : QGraphicsWidget(parent), m_pTracer(pTracer)
    , m_iPenWidth(1), m_color(Qt::yellow)
    , m_qllXMin(0), m_qllYMax(0), m_dScale(0)
    , m_sPointName(sName), m_Coord(coord), m_Type(type)
    , m_bMoving(false), m_bMousePress(false), m_bReadOnly(false)
{
    SetType(type);
    setAcceptedMouseButtons(Qt::LeftButton|Qt::RightButton);
}

CTracerPoint::~CTracerPoint()
{
}

void CTracerPoint::SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord& centralCoord)
{
    m_qllXMin = qllXMin;
    m_qllYMax = qllYMax;
    m_dScale = dScale;
    m_CentralCoord = centralCoord;

    if(!m_bMousePress)
    {
        prepareGeometryChange();
        m_painterPath = QPainterPath();
        QPointF point;
        point.setX((m_Coord.GetX()-qllXMin)*dScale);
        point.setY((qllYMax-m_Coord.GetY())*dScale);
        point = mapFromScene(point);
        point = mapToParent(point);
        setPos(point);
        if((m_Type == New) || (m_Type == FPL))
        {
            m_painterPath.addEllipse(QPointF(0,0), 4, 4);
        }
        update();
    }
}

QRectF CTracerPoint::boundingRect() const
{
    QRectF rect(m_painterPath.boundingRect());
    if(!rect.isNull()) rect.adjust(-m_iPenWidth, -m_iPenWidth, m_iPenWidth, m_iPenWidth);
    return rect;
}

QPainterPath CTracerPoint::shape() const
{
    return m_painterPath;
}

void CTracerPoint::paint(
        QPainter *painter,
        const QStyleOptionGraphicsItem *option,
        QWidget *widget)
{
    if(m_painterPath.isEmpty()) return;

    painter->save();

    painter->setRenderHint(QPainter::Antialiasing, true);

    QPen qPen(m_color);
    qPen.setWidth(m_iPenWidth);
    qPen.setCosmetic(true);
    painter->setPen(qPen);
    painter->setBrush(m_color);
    painter->drawPath(m_painterPath);
    painter->restore();
}

QVariant CTracerPoint::itemChange(GraphicsItemChange change,
                                  const QVariant& value)
{
    if(change == QGraphicsItem::ItemPositionChange)
        if(m_Type && !m_bReadOnly) m_bMoving = true;
    return QGraphicsWidget::itemChange(change, value);
}

void CTracerPoint::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    m_bMousePress = true;
    QGraphicsWidget::mousePressEvent(event);
    m_bMoving = false;
    parentItem()->setZValue(++ITracerHandler::m_zValue);
    setZValue(++ITracerHandler::m_zValue);
}

void CTracerPoint::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    m_bMousePress = false;
    QGraphicsWidget::mouseReleaseEvent(event);
    if(m_bMoving)
    {
        QList<QGraphicsItem*> listItems = scene()->items(event->scenePos());
        QList<QGraphicsItem*>::const_iterator itList = listItems.begin();
        CGUIPoint* pPoint = 0;
        for(; itList != listItems.end(); ++itList)
        {
            pPoint = dynamic_cast<CGUIPoint*>(*itList);
            if(pPoint)
                break;
        }
        QSharedPointer<CPoint> pFPLPoint;
        if(pPoint)
        {
            if(m_pTracer && !m_pTracer->IsCanAddPoint(pPoint->GetName(), pPoint->GetCoord(), pFPLPoint))
            {
                pPoint = 0;
            }
        }
        else
        {
            if(m_pTracer)
            {
                CCoord custCoord = m_pTracer->GetCoordAtPoint(event->scenePos());
                if(!m_pTracer->IsCanAddPoint(custCoord.GetAsString(), custCoord, pFPLPoint))
                {
                    pPoint = 0;
                }
            }
        }
        if(pFPLPoint)
        {
            m_pFragPoint->SetPoint(pFPLPoint);
            m_pFragPoint->SetGenerated(false);
            m_Coord = pFPLPoint->GetCoord();
            m_sPointName = pFPLPoint->GetName();
            if(m_Type == New) SetType(FPL);
        }
        else
        {
            m_Coord = m_pFragPoint->GetPoint()->GetCoord();
        }
        QPointF point;
        point.setX((m_Coord.GetX()-m_qllXMin)*m_dScale);
        point.setY((m_qllYMax-m_Coord.GetY())*m_dScale);
        point = mapFromScene(point);
        point = mapToParent(point);
        setPos(point);
        if(m_pTracer)
        {
            m_pTracer->UpdateTracer(this);
            m_pTracer->RecalculateTracer();
        }

        event->accept();
    }
    m_bMoving = false;
}

void CTracerPoint::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsWidget::mouseDoubleClickEvent(event);
    event->accept();
    if(!m_bReadOnly && m_pTracer && ((m_Type == New) || (m_Type == FPL))
            && m_pTracer->IsCanDeletePoint(this))
    {
        m_pMessBox.reset(new QMessageBox(QMessageBox::Warning, tr("Tracer"), tr("Delete point '%1'?").arg(m_sPointName)
                                                  , QMessageBox::Yes|QMessageBox::No, getMainWindow()));
        m_pMessBox->setDefaultButton(QMessageBox::No);
        int btn = m_pMessBox->exec();
        if(btn == int(QMessageBox::Yes))
            m_pTracer->DeleteTracerPoint(this);
    }
}

void CTracerPoint::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsWidget::mouseMoveEvent(event);
    if(m_bMoving && m_pTracer)
    {
        m_Coord = m_pTracer->GetCoordAtPoint(event->scenePos());
        m_pTracer->UpdateTracer(this);
        // slow
        //m_pTracer->RecalculateTracer();
    }
}

void CTracerPoint::SetType(PointType type)
{
    m_Type = type;
    switch(type)
    {
    case Custom:
    case Normal:
    case FPL_SSA:
        setFlag(ItemIsMovable, false);
        break;
    case New:
    case FPL:
        setFlag(ItemIsMovable, !m_bReadOnly);
        break;
    }
}

void CTracerPoint::SetReadOnly(bool bReadOnly)
{
    m_bReadOnly = bReadOnly;
    SetType(m_Type);
}

void CTracerPoint::GetPointParam(QString& sName, CCoord& coord) const
{
    sName = m_sPointName;
    coord = m_Coord;
}

void CTracerPoint::setVisible(bool visible)
{
    QGraphicsItem::setVisible(visible);
    if(!visible)
    {
        m_bMoving = m_bMousePress = false;
        if(m_pFragPoint && m_pFragPoint->GetPoint())
            m_Coord = m_pFragPoint->GetPoint()->GetCoord();
    }
}
