#include "GUISvgItem.h"

#include "main.h"
#include <cmath>
#include <QtDebug>

#include <QtGui/QPainter>


CGUISvgItem::CGUISvgItem(const QString & pathImage, QGraphicsItem * parent)
    : QGraphicsSvgItem(pathImage, parent), SvgItem(pathImage)//, CGUIObject(parent)
    , m_iPenWidth(2), m_dImageAngle(0)//, m_dScale(0), m_dScaleCoeff(1)
{
    setZValue(gui::Zssa);
    //setFlags(QGraphicsItem::ItemClipsToShape);
    setCacheMode(QGraphicsItem::NoCache);
}
CGUISvgItem::~CGUISvgItem()
{

}
QRectF CGUISvgItem::boundingRect() const
{
    QRectF rect = m_painterPath.boundingRect();
    rect.adjust(-1000,-1000, 1000, 1000);
    return rect;
}

QPainterPath CGUISvgItem::shape() const
{
    QPainterPath path;
    path.addRegion(QRegion(boundingRect().toRect()));
    return path;
}

void CGUISvgItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing);
    painter->save();
    {
        QRectF rect = m_painterPath.boundingRect();
        //painter->drawRect(rect);
        qreal scale_after_rotate = std::sqrt(rect.width()*rect.width() +
        rect.height()*rect.height())/rect.width();
        if(point.x() <= pointNext.x())
        {
            if(point.y() <= pointNext.y())
            {
                painter->translate(rect.x(), rect.y());
            }
            else
            {
                painter->translate(rect.x(), rect.y()+ rect.height());
            }

        }
        else
        {
            if(point.y() <= pointNext.y())
            {
                painter->translate(rect.x() + rect.width(), rect.y());
            }
            else
            {
                painter->translate(rect.x()+rect.width(), rect.y()+ rect.height());
            }
        }
        //painter->drawRect(rect);
        rect.setX(0);
        rect.setY(0);
        rect.setWidth(m_painterPath.boundingRect().width());
        rect.setHeight(m_painterPath.boundingRect().height());
        painter->save();
        //painter->drawRect(rect);
        //qDebug() << GetAngle();
        painter->rotate(GetAngle());
        //painter->drawRect(rect);
        painter->translate(0, -rect.height()/2);
        //painter->drawRect(rect);
        painter->scale(scale_x, scale_y);
        painter->scale(scale_after_rotate, 1);
        //painter->drawRect(QGraphicsSvgItem::boundingRect());
        QGraphicsSvgItem::paint(painter, option, widget);
        painter->restore();
    }
    painter->restore();
}

void CGUISvgItem::SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord& centralCoord)
{
    prepareGeometryChange();
    m_painterPath = QPainterPath();
    point.setX((GetCoordStart().GetX() - qllXMin) * dScale);
    point.setY((qllYMax - GetCoordStart().GetY()) * dScale);
    point = mapFromScene(point);
    pointNext.setX((GetCoordFinish().GetX() - qllXMin) * dScale);
    pointNext.setY((qllYMax - GetCoordFinish().GetY()) * dScale);
    pointNext = mapFromScene(pointNext);
    m_painterPath.moveTo(point);
    m_painterPath.lineTo(pointNext);
    if(point.x() <= pointNext.x())
    {
        if(point.y() <= pointNext.y())
        {
            qreal dif_x = pointNext.x() - point.x();
            qreal dif_y = pointNext.y() - point.y();
            qreal angle = atan(dif_y/dif_x)* (180./M_PI);
            SetAngle(angle);
        }
        else
        {
            qreal dif_x = pointNext.x() - point.x();
            qreal dif_y = point.y() - pointNext.y();
            qreal angle = atan(dif_y/dif_x)* (180./M_PI);
            SetAngle(-angle);
        }
    }
    else
    {
        if(point.y() <= pointNext.y())
        {
            qreal dif_x = point.x() - pointNext.x();
            qreal dif_y = pointNext.y() - point.y();
            qreal angle = atan(dif_y/dif_x)* (180./M_PI);
            SetAngle(180-angle);
        }
        else
        {
            qreal dif_x = point.x() - pointNext.x();
            qreal dif_y = point.y() - pointNext.y();
            qreal angle = atan(dif_y/dif_x)* (180./M_PI);
            SetAngle(180+angle);
        }
    }
    update();
    scale_x = m_painterPath.boundingRect().width()/QGraphicsSvgItem::boundingRect().width();
    scale_y = m_painterPath.boundingRect().height()/QGraphicsSvgItem::boundingRect().height();
}

//void CGUISvgItem::SetScaleCoeff(double dCoeff)
//{
//    Q_UNUSED(dCoeff)
//    double dPlaneInMeters = 30;
//    m_dScaleCoeff = 1;
//    QRectF rect = QGraphicsSvgItem::boundingRect();
//    if(rect.width())
//    {
//        m_dScaleCoeff = dPlaneInMeters / rect.width();
//    }
//}

//double CGUISvgItem::GetScale() const
//{
//    double dScale = m_dScale * m_dScaleCoeff;
//    if(dScale < 0.5)
//        dScale = 0.5;
//    return dScale;
//}
void CGUISvgItem::MarkItemActive(bool bActive)
{
    SvgItem::MarkItemActive(bActive);
    setVisible(bActive);
}
