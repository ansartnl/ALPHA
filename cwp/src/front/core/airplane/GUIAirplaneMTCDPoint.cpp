#include "GUIAirplaneMTCDPoint.h"
#include "GUIInterface.h"

#include "main.h"

#include <QtDebug>
#include <QtGui/QPainter>

CGUIAirplaneMTCDPoint::CGUIAirplaneMTCDPoint(QGraphicsItem* parent/* = 0*/) :
    CGUIObject(parent)
{
    setZValue(gui::Zmtcdpoint);
    //SetColor(Qt::white);
    setAcceptHoverEvents(true);
}

void CGUIAirplaneMTCDPoint::SetCoord(const CCoord& coord)
{
    m_Coord = coord;
}

void CGUIAirplaneMTCDPoint::SetPos(const QPointF& pos)
{
    QRectF rect = boundingRect();
    setPos(pos.x()-rect.width()/2, pos.y()-rect.height()/2);
}

QRectF CGUIAirplaneMTCDPoint::boundingRect() const
{
    QRectF rect(0,0,10,10);
    return rect;
}

QPainterPath CGUIAirplaneMTCDPoint::shape() const
{
    return CGUIObject::shape();
}

void CGUIAirplaneMTCDPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(isVisible())
    {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);

        QPen qPen;
        qPen.setWidth(1);
        qPen.setColor(Qt::white);
        painter->setPen(qPen);
        painter->setBrush(Qt::white);
        QRectF rect = boundingRect();
        painter->drawEllipse(rect);

        qPen.setColor(Qt::red);
        painter->setPen(qPen);
        painter->setBrush(Qt::red);
        rect.adjust(2,2,-2,-2);
        painter->drawEllipse(rect);

        painter->restore();
    }
}

void CGUIAirplaneMTCDPoint::UpdatePosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord& centralCoord)
{
    if(!m_Coord.IsNull() && m_Coord.IsCenter())
    {
        //prepareGeometryChange();
        QPointF tempPoint;
        tempPoint.setX((m_Coord.GetX() - qllXMin) * dScale);
        tempPoint.setY((qllYMax - m_Coord.GetY()) * dScale);
        SetPos(mapToParent(mapFromScene(tempPoint)));
        //update();
    }
}

void CGUIAirplaneMTCDPoint::SetListMTCDText(const QStringList& sList, bool bAppend)
{
    if(bAppend)
        m_sTextMTCD.append(sList);
    else
        m_sTextMTCD = sList;
    m_sTextMTCD.removeDuplicates();
}

void CGUIAirplaneMTCDPoint::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    CGUIObject::hoverEnterEvent(event);

    if(m_sTextMTCD.size())
        setToolTip(m_sTextMTCD.join(" "));
}
