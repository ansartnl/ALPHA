#include "GUIAirplanePoint.h"
#include "GUIAirplane.h"

#include "AirplaneCollision.h"

#include "main.h"

#include <QtDebug>

#include <QPainter>
#include <QBitmap>
#include "utils/ProfileSettings.h"
CGUIAirplanePoint::CGUIAirplanePoint(QGraphicsItem * parent/* = 0*/) :
    CGUIObject(parent), m_iPenWidth(2), m_intervalNum(1), m_intervalInc(1)
  , m_pAirplane(0)
  , angle_(0)
{
    setZValue(gui::Zairplanepoint);
}

void CGUIAirplanePoint::SetAirplane(CGUIAirplane *pPlane)
{
    m_pAirplane = pPlane;
}

QRectF CGUIAirplanePoint::boundingRect() const
{
    QRectF rect(m_painterPath.boundingRect());
    rect.adjust(-1, -1, 1, 1);
    return rect;
}

QPainterPath CGUIAirplanePoint::shape() const
{
    QTransform transform;
    transform.scale(2, 2);
    return transform.map(m_painterPath);
}

void CGUIAirplanePoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (!m_pAirplane)
        return;

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    QColor color(GetColor());
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
    if (m_pAirplane->GetBearing())
    {
        color = Qt::darkGreen;
    }
    else
    {
        if (m_pAirplane->trackStatus() == CAirplane::Lost)
            color = Qt::yellow;
        else if (m_pAirplane->trackStatus() == CAirplane::Nohope)
            color = Qt::red;
    }
    /*if (m_pAirplane->dbl())
        color = m_pAirplane->dblColor();*/
    if (m_pAirplane->GetHighlight().GetType())
        color = Qt::darkGreen;
    if (m_pAirplane->isSTCA())
        color = Qt::red;

    QPen pen;
    pen.setColor(color);
    pen.setWidth(m_iPenWidth);
    pen.setCapStyle(Qt::RoundCap);
    painter->setPen(pen);
    painter->setBrush(color);
    painter->drawPath(m_painterPath);
    painter->restore();
}

void CGUIAirplanePoint::SetPosition(QPointF point, CCoord coord)
{
    m_CenterPoint = point;
    // TODO create airplane velocity vector
    // If seconds

    int length = 0;
    if (networkSettings()->value("airplane/deflexion_type").toString() == "sec")
    {
        m_intervalNum = networkSettings()->value("airplane/deflexion_sec").toInt();
        length = m_intervalNum * 60;
        m_intervalInc = 1;
    }
    else if (networkSettings()->value("airplane/deflexion_type").toString() == "kilometer")
    {
        m_intervalNum = networkSettings()->value("airplane/deflexion_kilometer").toInt();
        if (m_pAirplane)
            length = m_intervalNum * 1000 / m_pAirplane->GetVelocity().GetV();
        m_intervalInc = 5; // show marks via 5 km(miles)
    }

    if (m_intervalNum < 1 ||
         networkSettings()->value("airplane/deflexion_marks").toString().toLower() != "true")
        m_intervalNum = 1;

    if (!m_pAirplane->isFlying())
        length /= 500;
    m_Coord.SetX(coord.GetX() + length * m_pAirplane->GetVelocity().GetVx());
    m_Coord.SetY(coord.GetY() + length * m_pAirplane->GetVelocity().GetVy());
}

void CGUIAirplanePoint::UpdatePosition(qint64 qllXMin, qint64 qllYMax, double dScale)
{
    prepareGeometryChange();
    m_painterPath = QPainterPath();
    const int radius = networkSettings()->value("airplane/size_plane_point", 3).toInt();
    const QPointF center(mapFromParent(m_CenterPoint));
    /*if (m_pAirplane && m_pAirplane->dbl()) {
        const QRectF rect(QPointF(center.x() - radius, center.y() - radius), QSizeF(radius*2, radius*2));
        m_painterPath.moveTo(rect.topLeft());
        m_painterPath.lineTo(rect.bottomRight());
        m_painterPath.moveTo(rect.topRight());
        m_painterPath.lineTo(rect.bottomLeft());
    } else {*/
        m_painterPath.addEllipse(center, radius, radius);
    //}

    QPointF tempPoint;
    tempPoint.setX((m_Coord.GetX() - qllXMin) * dScale);
    tempPoint.setY((qllYMax - m_Coord.GetY()) * dScale);
    QLineF line(center, mapFromScene(tempPoint));
    m_painterPath.moveTo(line.p1());
    m_painterPath.lineTo(line.p2());
    angle_ = line.angle();
    // Draw marks each minute or kilometer on the velocity vector.
    {
        const qreal unitVecLength = line.length() / m_intervalNum;
        for (int i = m_intervalInc; i < m_intervalNum; i += m_intervalInc)
        {
            line.setLength(i*unitVecLength);
            m_painterPath.addEllipse(line.p2(), m_iPenWidth-1, m_iPenWidth-1);
        }
    }
    update();
}

void CGUIAirplanePoint::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    m_pAirplane->setFixedFormular(false);
}
