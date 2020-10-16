#include "GUIGroundVehicle.h"
#include "back/common/Calculation.h"
#include <QPainter>
#include <QGraphicsScene>

#ifndef GROUND_CWP
#include "Simulator.h"
#endif

GUIGroundVehicle::GUIGroundVehicle(QGraphicsItem *parent)
    : GroundVehicle(), QGraphicsItem(parent)
    , m_iPenWidth(1), m_color(Qt::white), m_bMoving(false)
    , m_Vehicle(this), m_Label(this)
{
    setZValue(40);
    setFlag(ItemSendsGeometryChanges, true);
    m_Vehicle.setPos(QPointF(0,0));
    m_Label.setPos(QPointF(15,-90));
}

GUIGroundVehicle::~GUIGroundVehicle()
{
}

QRectF GUIGroundVehicle::boundingRect() const
{
    QRectF rect(m_painterPath.boundingRect());
    if(!m_painterPathPoints.isEmpty()) rect |= m_painterPathPoints.boundingRect();
    rect.adjust(-1,-1,1,1);
    return rect;
}

QPainterPath GUIGroundVehicle::shape() const
{
    return m_painterPath;
}

void GUIGroundVehicle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(!m_painterPath.isEmpty())
    {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        QPen qPen(m_color);
        qPen.setWidth(m_iPenWidth);
        painter->setPen(qPen);
        painter->setBrush(m_color);
        painter->drawPath(m_painterPath);
        if(!m_painterPathPoints.isEmpty())
        {
            painter->setPen(QPen(QBrush(Qt::yellow), 2, Qt::SolidLine, Qt::RoundCap));
            painter->setBrush(Qt::NoBrush);
            painter->drawPath(m_painterPathPoints);
        }
        painter->restore();
    }
}

void GUIGroundVehicle::Translate()
{
    m_Label.Translate();
}

void GUIGroundVehicle::SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord &centralCoord)
{
    m_qllXMin = qllXMin;
    m_qllYMax = qllYMax;
    m_dScale = dScale;
    m_CentralCoord = centralCoord;
    if(!m_listCoords.size())
        UpdatePosition();
    else
        RecalculateAll();
}

void GUIGroundVehicle::SetSI(qint16 SI)
{
    Q_UNUSED(SI)
}

void GUIGroundVehicle::UpdatePosition()
{
    m_listCoords.push(GetCoord());
    RecalculateAll();

//    QPointF point = scenePos();
//    CCoord coords;
//    emit calcCoordAtPoint(point, coords);

    m_Vehicle.SetVehicle(this);
    m_Vehicle.SetPosition(mapFromParent(m_CenterPoint), GetCoord());
    m_Vehicle.UpdatePosition(m_qllXMin, m_qllYMax, m_dScale);

    m_Label.SetVehicle(this);
    m_Label.UpdatePosition(mapFromParent(m_CenterPoint));
}

void GUIGroundVehicle::ShowPathPoints(bool bShow)
{
    prepareGeometryChange();
    m_painterPathPoints = QPainterPath();
    if(bShow)
    {
        QList<QSharedPointer<CGUILineText> >::const_iterator iter = m_listRoute.begin();
        for(; iter != m_listRoute.end(); ++iter)
            scene()->removeItem(iter->data());
        m_listRoute.clear();

#ifndef GROUND_CWP
        if(pathPoints().size())
        {
            TRoutePointList::const_iterator itPoints = pathPoints().constBegin();

            //QList<QSharedPointer<CPoint> >::const_iterator itPoints = pathCoords().constBegin();

            CCoord prevCoord;
            QTime dt(QTime::currentTime());
            for(int i = 0; itPoints != pathPoints().constEnd(); ++itPoints, ++i)
            {
                if(i == 0)
                {
                    if(m_listCoords.size())
                        prevCoord = m_listCoords.last();
                    else
                        prevCoord = GetCoord();
                }
                //if((*itPoints)->GetPoint())
                {
                    CCoord coord = (*itPoints)->coord();//->GetPoint()->GetCoord();
                    coord.Center(m_CentralCoord);
                    QSharedPointer<CGUILineText> line(new CGUILineText(0, this));
                    line->SetColor(Qt::yellow);
                    line->SetPenWidth(2);
                    line->SetPosition(m_qllXMin, m_qllYMax, m_dScale, m_CentralCoord);
                    line->SetPoints(prevCoord, coord, CGUILineText::AllPoints);
                    dt = dt.addSecs(avia::GetTimeRoute(prevCoord, coord, GetIAS()));
                    QString sText;
                    if((itPoints+1 != pathPoints().constEnd()) && i != 0)
                        sText = QString::null;
                    else
                        sText = dt.toString(QString("hh:mm:ss"));
                    line->SetText(sText, Qt::AlignRight);
                    m_listRoute.append(line);
                    prevCoord = coord;
                }
            }
        }
        else if(mRoutePoints.points.size())
        {
            CCoord prevCoord = mRoutePoints.points.at(0);
            prevCoord.Center(m_CentralCoord);
            QTime dt(QTime::currentTime());

            for(int i = 0; i < mRoutePoints.points.size(); ++i)
            {
                CCoord coord = mRoutePoints.points.at(i);
                coord.Center(m_CentralCoord);
                QSharedPointer<CGUILineText> line(new CGUILineText(0, this));
                line->SetColor(Qt::yellow);
                line->SetPenWidth(2);
                line->SetPosition(m_qllXMin, m_qllYMax, m_dScale, m_CentralCoord);
                line->SetPoints(prevCoord, coord, CGUILineText::AllPoints);
                dt = dt.addSecs(avia::GetTimeRoute(prevCoord, coord, GetIAS()));
                QString sText;
                sText = dt.toString(QString("hh:mm:ss"));
                line->SetText(sText, Qt::AlignRight);
                m_listRoute.append(line);
                prevCoord = coord;
            }
        }
#endif
    }
    else
    {
        QList<QSharedPointer<CGUILineText> >::const_iterator iter = m_listRoute.begin();
        for(; iter != m_listRoute.end(); ++iter)
            scene()->removeItem(iter->data());
        m_listRoute.clear();
    }
    update();
}

bool GUIGroundVehicle::isShowPathPoints() const
{
    return !m_listRoute.isEmpty() || !m_painterPathPoints.isEmpty();
}

QPointF GUIGroundVehicle::getVehiclePosition()
{
    QPointF point;
    if(m_listCoords.size())
    {
        QVectorIterator<CCoord> itCoords(m_listCoords);
        itCoords.toBack();
        CCoord coord = itCoords.previous();
        point.setX((coord.GetX() - m_qllXMin) * m_dScale);
        point.setY((m_qllYMax - coord.GetY()) * m_dScale);
    }
    return point;
}

void GUIGroundVehicle::GetVehicleNewCoord()
{
    emit getCoord(this);
}

void GUIGroundVehicle::SetVehicleNewCoord(const CCoord &coord)
{
#ifndef GROUND_CWP
    QSharedPointer<atd::CPoint> point(QSharedPointer<atd::CPoint>(new atd::CPoint));
    point.data()->SetCoord(coord);
    point.data()->SetName(coord.GetAsString());
    ChangeNextPoint(point);
    m_Label.SetPoint(&coord);
#endif
}

bool GUIGroundVehicle::isAtEnd()
{
    return (mRoutePoints.SN > mRoutePoints.points.size() ||
            mRoutePoints.SN < -1);
}

void GUIGroundVehicle::SetFormularDefaultPos()
{
    m_Label.setPos(QPointF(15,-90));
}

void GUIGroundVehicle::SetAutoCollapse(bool bCollapse)
{
    m_Label.SetAutoCollapse(bCollapse);
}

void GUIGroundVehicle::RecalculateAll()
{
    if(!m_listCoords.size()) return;

    prepareGeometryChange();
    m_painterPath = QPainterPath();
    QVector<QPointF>    arrPoints;
    QVectorIterator<CCoord> itCoords(m_listCoords);
    itCoords.toBack();
#ifndef GROUND_CWP
    int iCount = GetSimulator()->GetHistoryNumberOfPoints();
#else
    int iCount = 5;
#endif
    for(int i = 0; (i <= iCount) && (itCoords.hasPrevious()); ++i)
    {
        CCoord coord = itCoords.previous();
        if(i == 0)
        {
            m_CenterPoint.setX((coord.GetX() - m_qllXMin) * m_dScale);
            m_CenterPoint.setY((m_qllYMax - coord.GetY()) * m_dScale);
            setPos(m_CenterPoint);
        }
        else
        {
            QPointF point;
            point.setX((coord.GetX() - m_qllXMin) * m_dScale);
            point.setY((m_qllYMax - coord.GetY()) * m_dScale);
            if(point == m_CenterPoint)
            {
                    --i;
            }
            else
            {
                point = mapFromScene(point);
                if(arrPoints.size() && (point == arrPoints.last()))
                {
                    --i;
                }
                else
                {
                    m_painterPath.addEllipse(point, 2, 2);
                    arrPoints.append(point);
                }
            }
        }
    }

    if(isShowPathPoints()) ShowPathPoints();
    m_Vehicle.UpdatePosition(m_qllXMin, m_qllYMax, m_dScale);

#ifndef GROUND_CWP
    m_Label.setVisible(true);
#else
    m_Vehicle.setVisible(!isHidden());
    m_Label.setVisible(!isHidden());
    setVisible(!isHidden());
#endif

    update();
}
