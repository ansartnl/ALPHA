#include "GUIUserPics.h"
#include <QtGui/QPainter>
#include <QtCore/QTextStream>
#include <math.h>
#include "back/utils/ProfileSettings.h"
#include "front/common/XAppSettings.h"
CGUIUserPics::CGUIUserPics(QGraphicsItem *parent)
    : CGUIObject(parent), m_iPenWidth(1), m_index_selected_point(0)
{
    m_bPrimitiveArc = true;
    m_draw_selected = false;
    setZValue(gui::Zssa);
    SetColor(Qt::green);
    SetColor( UserProfile::CSettings::Instance().GetColor(UserProfile::CtApproach));
}

CGUIUserPics::~CGUIUserPics()
{

}
void CGUIUserPics::setSelectedPoint(int row, bool selected)
{
    m_index_selected_point = row;
    m_draw_selected = selected;
    if(!m_draw_selected)
    {
        update();
        return;
    }
    TPointList& points = GetPoints();
    if(row > points.size()-1)
    {
        return;
    }
    TPointList::const_iterator itScan = GetPoints().begin()+row;
    if(!itScan->data())
        return;
    m_selected_point.setX(((*itScan)->GetCoord().GetX() - m_last_qllXMin) * m_last_scale);
    m_selected_point.setY((m_last_qllYMax - (*itScan)->GetCoord().GetY()) * m_last_scale);
    m_selected_point = mapFromScene(m_selected_point);
    update();
}
QRectF CGUIUserPics::boundingRect() const
{
    QRectF rect = m_painterPath.boundingRect();
    QPointF topleftPoint = rect.topLeft();
    topleftPoint.setX(topleftPoint.x()-2);
    topleftPoint.setY(topleftPoint.y()-2);
    rect.setTopLeft(topleftPoint);
    QPointF bottomRightPoint = rect.bottomRight();
    bottomRightPoint.setX(bottomRightPoint.x()+2);
    bottomRightPoint.setY(bottomRightPoint.y()+2);
    rect.setBottomRight(bottomRightPoint);
    //return m_painterPath.boundingRect();
    return rect;
}

QPainterPath CGUIUserPics::shape() const
{
    return m_painterPath;
}

void CGUIUserPics::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (IsItemActive())
    {
        painter->save();
        //QPen qPen( GetColor() );
        QPen qPen(UserProfile::CSettings::Instance().GetColor(UserProfile::CtApproach));
        qPen.setWidth(m_iPenWidth);
        painter->setPen(qPen);
        painter->drawPath(m_painterPath);
        if(m_draw_selected)
        {
            painter->save();
            QColor color(255, 0, 0);
            qPen.setColor(color);
            QBrush brush;
            brush.setStyle(Qt::SolidPattern);
            brush.setColor(color);
            qPen.setWidth(1);
            painter->setPen(qPen);
            painter->setBrush(brush);
            painter->drawEllipse(m_selected_point, 2, 2);
            painter->restore();
        }
        painter->restore();
    }
}

void CGUIUserPics::SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord& centralCoord)
{
    if (IsItemActive())
    {
        m_last_qllXMin = qllXMin;
        m_last_qllYMax = qllYMax;
        m_last_scale = dScale;
        setSelectedPoint(m_index_selected_point, m_draw_selected);
        prepareGeometryChange();
        m_painterPath = QPainterPath();
        TPointList::const_iterator itScan = GetPoints().begin();
        for(; itScan != GetPoints().end(); ++itScan)
        {
            QPointF point(
                ((*itScan)->GetCoord().GetX() - qllXMin) * dScale,
                (qllYMax - (*itScan)->GetCoord().GetY()) * dScale);
            point = mapFromScene(point);
            QPointF pointArc(
                ((*itScan)->GetArcCoord().GetX() - qllXMin) * dScale,
                (qllYMax - (*itScan)->GetArcCoord().GetY()) * dScale);
            pointArc = mapFromScene(pointArc);

            TPointList::const_iterator itScanNext = itScan+1;
            if(itScanNext == GetPoints().end()) itScanNext = itScan;//GetPoints().begin();

            QPointF pointNext(
                ((*itScanNext)->GetCoord().GetX() - qllXMin) * dScale,
                (qllYMax - (*itScanNext)->GetCoord().GetY()) * dScale);
            pointNext = mapFromScene(pointNext);

            qreal qRadius = (*itScan)->GetArcDistance()* 100 * dScale;
            //qDebug() <<"Arc Distance:"<< (*itScan)->GetArcDistance() << " Scale:" << dScale <<" Radius:" << qRadius;
            if(itScan == GetPoints().begin())
            {
                m_painterPath.moveTo(point);
            }
            switch((*itScan)->GetType())
            {
            case CUserPicsPoint::enLine:
                m_painterPath.lineTo(pointNext);
                break;
            case CUserPicsPoint::enCircle:
                //m_painterPath.addEllipse(pointArc, qRadius, qRadius);
                m_painterPath.lineTo(pointNext);
                break;
            case CUserPicsPoint::enLeftArc:
            case CUserPicsPoint::enRightArc:
                {
                    if ( m_bPrimitiveArc )
                    {
                        QLineF line1(pointNext, point);
                        //qRadius = line1.length() / 2;
                        qreal half = line1.length() / 2;
                        if(qRadius < half)
                        {
                            qRadius = half;
                        }
                        //qDebug() << "Radius:" << qRadius << " Half:" << half;
                        qreal x1 = point.x();
                        qreal y1 = point.y();
                        //qDebug() << x1 << ", " << y1;
                        qreal x2 = pointNext.x();
                        qreal y2 = pointNext.y();
                        //qDebug() << x2 << ", " << y2;
                        //QPointF pointCenter_temp( x1 + (x2-x1)/2, y1 + (y2-y1)/2 );
                        //qDebug() << "x_t:" << pointCenter_temp.x() << " y_t:" << pointCenter_temp.y();
                        QPointF pointCenter;
                        qreal alpha1;
                        qreal alpha2;
                        qreal alpha_c;
                        alpha1 = acos(half/qRadius);
                        alpha2 = acos(fabs(x2 - x1)/(2*half));
                        alpha_c = alpha1 - alpha2;
                        if((*itScan)->GetType() == CUserPicsPoint::enLeftArc)
                        {
                            if(x1 < x2)
                            {
                                if(y1 < y2)
                                {
                                    //x1 < x2, y1 < y2
                                    pointCenter.setX(x1 + cos(alpha_c)*qRadius);
                                    pointCenter.setY(y1 - sin(alpha_c)*qRadius);
                                    alpha1 = 180.0 + atan((y1 - pointCenter.y())
                                                          /(pointCenter.x()- x1))*(180.0/M_PI);
                                    alpha2 = 270.0 + atan((x2 - pointCenter.x())
                                                          /(y2 - pointCenter.y()))*(180.0/M_PI) - alpha1;

                                }
                                else
                                {
                                    //x1 < x2, y1 > y2
                                    pointCenter.setX(x2 - cos(alpha_c)*qRadius);
                                    pointCenter.setY(y2 - sin(alpha_c)*qRadius);
                                    alpha1 = 270.0 - atan((pointCenter.x()- x1)
                                                          /(y1 - pointCenter.y()))*(180.0/M_PI);
                                    alpha2 = 360.0 + atan((pointCenter.y()- y2)
                                                          /(x2 - pointCenter.x()))*(180.0/M_PI) - alpha1;
                                }
                            }
                            else
                            {
                                if(y1 < y2)
                                {
                                    //x1 > x2, y1 < y2
                                    pointCenter.setX(x2 + cos(alpha_c)*qRadius);
                                    pointCenter.setY(y2 + sin(alpha_c)*qRadius);
                                    alpha1 = 90.0 - atan((x1 - pointCenter.x())
                                                          /(pointCenter.y() - y1))*(180.0/M_PI);
                                    alpha2 = 180.0 + atan((y2 - pointCenter.y())
                                                          /(pointCenter.x() - x2))*(180.0/M_PI) - alpha1;
                                }
                                else
                                {
                                    //x1 > x2, y1 > y2
                                    pointCenter.setX(x1 - cos(alpha_c)*qRadius);
                                    pointCenter.setY(y1 + sin(alpha_c)*qRadius);
                                    alpha1 = 0.0 - atan((y1 - pointCenter.y())
                                                          /(x1 - pointCenter.x()))*(180.0/M_PI);
                                    alpha2 = 90.0 + atan((pointCenter.x() - x2)
                                                          /(pointCenter.y() - y2))*(180.0/M_PI) - alpha1;
                                }
                            }
                        }
                        else
                        {
                            if(x1 < x2)
                            {
                                if(y1 < y2)
                                {
                                    //x1 < x2, y1 < y2
                                    pointCenter.setX(x2 - cos(alpha_c)*qRadius);
                                    pointCenter.setY(y2 + sin(alpha_c)*qRadius);
                                    alpha1 = 90.0 + atan((pointCenter.x() - x1)
                                                          /(pointCenter.y() - y1))*(180.0/M_PI);
                                    alpha2 = 0.0 - atan((y2 - pointCenter.y())
                                                          /(x2 - pointCenter.x()))*(180.0/M_PI) - alpha1;
                                }
                                else
                                {
                                    //x1 < x2, y1 > y2
                                    pointCenter.setX(x1 + cos(alpha_c)*qRadius);
                                    pointCenter.setY(y1 + sin(alpha_c)*qRadius);
                                    alpha1 = 180.0 + atan((y1 - pointCenter.y())
                                                          /(pointCenter.x() - x1))*(180.0/M_PI);
                                    alpha2 = 90.0 - atan((x2 - pointCenter.x())
                                                          /(pointCenter.y() - y2))*(180.0/M_PI) - alpha1;
                                }
                            }
                            else
                            {
                                if(y1 < y2)
                                {
                                    //x1 > x2, y1 < y2
                                    pointCenter.setX(x1 - cos(alpha_c)*qRadius);
                                    pointCenter.setY(y1 - sin(alpha_c)*qRadius);
                                    alpha1 = 360.0 + atan((pointCenter.y() - y1)
                                                          /(x1 - pointCenter.x()))*(180.0/M_PI);
                                    alpha2 = 270.0 - atan((pointCenter.x() - x2)
                                                          /(y2 - pointCenter.y()))*(180.0/M_PI) - alpha1;
                                }
                                else
                                {
                                    //x1 > x2, y1 > y2
                                    pointCenter.setX(x2 + cos(alpha_c)*qRadius);
                                    pointCenter.setY(y2 - sin(alpha_c)*qRadius);
                                    alpha1 = 270.0 + atan((x1 - pointCenter.x())
                                                          /(y1 - pointCenter.y()))*(180.0/M_PI);
                                    alpha2 = 180.0 - atan((pointCenter.y() - y2)
                                                          /(pointCenter.x() - x2))*(180.0/M_PI) - alpha1;
                                }
                            }
                        }
                        //qDebug() <<"alpha1:" << alpha1 << " alpha2:" << alpha2;
                        //qDebug() << "x:" << pointCenter.x() << " y:" << pointCenter.y();
                        QRectF rect(pointCenter.x()-qRadius, pointCenter.y()-qRadius, 2*qRadius, 2*qRadius);
                        //qDebug() <<"alpha1:" << alpha1 << " alpha2:" << alpha2;
                        m_painterPath.arcTo(rect, alpha1, alpha2); //clockwise
                    }
                    else
                    {
                        QRectF rect(QPointF(pointArc.x()-qRadius, pointArc.y()-qRadius), QSizeF(2*qRadius, 2*qRadius));
                        QLineF line1(pointArc, point);
                        QLineF line2(pointArc, pointNext);
                        if((*itScan)->GetType() == CUserPicsPoint::enLeftArc)
                            m_painterPath.arcTo(rect, line1.angle(), line1.angleTo(line2));
                        else
                            m_painterPath.arcTo(rect, line1.angle(), line1.angleTo(line2)-360);
                    }
                }
                break;
            default:
                qWarning("unknown CUserPicsPoint::Type");
            }
        }
        update();
    }
}

void CGUIUserPics::MarkItemActive(bool bActive)
{
    CUserPics::MarkItemActive(bActive);
    setVisible(bActive);
}
