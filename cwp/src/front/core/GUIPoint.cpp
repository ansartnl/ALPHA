#include "GUIPoint.h"

#include <QPen>
#include <QPainter>
#include <QGraphicsScene>
#include <QBitmap>

#include <math.h>
#include "back/utils/ProfileSettings.h"
#include "front/common/XAppSettings.h"
#include "main.h"

extern QString currentUserName;

CGUIPoint::CPointPixmap CGUIPoint::m_Pixmaps = CGUIPoint::CPointPixmap();

CGUIPoint::CPointPixmap::CPointPixmap()
{

}

CGUIPoint::CPointPixmap::~CPointPixmap()
{

}

const QPixmap& CGUIPoint::CPointPixmap::GetPixmap(CPoint::Type type)
{
    if(!m_map.contains(type))
    {
        QSettings &settings = *globalSettings();
        settings.beginGroup(currentUserName);
        settings.beginGroup("PointPixmaps");
        QPixmap pxm;
        switch(type)
        {
        case CPoint::enUnknown:
        case CPoint::enWaypoint:
        case CPoint::enSSAWaypoint:
        {
            QVariant var = settings.value("POD", "pod.png");
            settings.setValue("POD", var);
            pxm.load("images:/points/"+var.toString());
        }
            break;
        case CPoint::enVHFNavAid:
        {
            QVariant var = settings.value("VHF", "vhf.png");
            settings.setValue("VHF", var);
            pxm.load("images:/points/"+var.toString());
        }
            break;
        case CPoint::enNDBNavAid:
        case CPoint::enAirportNDB:
        {
            QVariant var = settings.value("NDB", "ndb.png");
            settings.setValue("NDB", var);
            pxm.load("images:/points/"+var.toString());
        }
            break;
        case CPoint::enAirportPoint:
        {
            QVariant var = settings.value("SID_STAR", "sid_star.png");
            settings.setValue("SID_STAR", var);
            pxm.load("images:/points/"+var.toString());
        }
            break;
        }
        settings.endGroup();
        settings.endGroup();
        if(!pxm.isNull())
        {
            //pxm.setMask(pxm.createHeuristicMask());
        }
        m_map.insert(type, pxm);
    }
    return m_map[type];
}

CGUIPoint::CGUIPoint()
    : m_iPenWidth(1)
{
    setZValue(gui::Zpoint);
    SetColor( UserProfile::CSettings::Instance().GetColor(UserProfile::CtMap) );
}

QRectF CGUIPoint::boundingRect() const
{
    return m_Zone;
}

QPainterPath CGUIPoint::shape() const
{
    QPainterPath qPath;
    const QPixmap& pxm = m_Pixmaps.GetPixmap(GetType());
    if(pxm.isNull())
    {
        QPolygonF qPolygon;
        qPolygon.push_back(QPointF(m_CenterPoint.x(), m_CenterPoint.y() - 5));
        qPolygon.push_back(QPointF(m_CenterPoint.x() + 5, m_CenterPoint.y() + 4));
        qPolygon.push_back(QPointF(m_CenterPoint.x() - 5, m_CenterPoint.y() + 4));
        qPath.addPolygon(qPolygon);
    }
    else
    {
        qPath.addRect(pxm.rect());
    }
    return qPath;
}

void CGUIPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    QPen qPen(GetColor());
    qPen.setWidth(m_iPenWidth);
    painter->setPen(qPen);
    
    if(!GetSignActive() && GetType() == CPoint::enSSAWaypoint)
    {
        // если точка принадлежит схемам, котрые неактивны, то ничего не рисуем
    }
    else
    {
        const QPixmap& pxm = m_Pixmaps.GetPixmap(GetType());
        if (pxm.isNull()) {
            QPolygonF qPolygon;
            qPolygon.push_back(QPointF(m_CenterPoint.x(), m_CenterPoint.y() - 5./3.));
            qPolygon.push_back(QPointF(m_CenterPoint.x() + 5./3., m_CenterPoint.y() + 4./3.));
            qPolygon.push_back(QPointF(m_CenterPoint.x() - 5./3., m_CenterPoint.y() + 4./3.));
            //	qPolygon.push_back(QPointF(m_CenterPoint.x(), m_CenterPoint.y() - 5));
            //	qPolygon.push_back(QPointF(m_CenterPoint.x() + 5, m_CenterPoint.y() + 4));
            //	qPolygon.push_back(QPointF(m_CenterPoint.x() - 5, m_CenterPoint.y() + 4));
            painter->drawPolygon(qPolygon);
        } else {
            painter->drawPixmap(m_CenterPoint.x() - qreal(pxm.width())/2, m_CenterPoint.y() - qreal(pxm.height())/2, pxm);
        }
    }

    //if ("yes" == networkSettings()->value("users/draw_points", "no").toString()) {

    QString p = networkSettings()->value("users/draw_points", "no").toString();
    QString Bp = networkSettings()->value("users/draw_Borderpoints", "no").toString();
    CPoint::Type type = GetType();
    if((("yes" == Bp) && (GetFirBorder()  == true || GetSectorBorder() == true))
            || (("yes" == p) && (GetFirBorder()  == false && GetSectorBorder() == false))
            || (type  == CPoint::enVHFNavAid)){

        QFont font = painter->font();
        font.setPointSize(AppSettings::GetTextSize().GetCurrentTextSize());
        painter->setFont(font);
        const QFontMetricsF metrics(painter->font());
        QRectF TextRect(metrics.boundingRect(GetName()));
        TextRect.adjust(-1, -1, 1, 1);
        TextRect.moveTo(0, 0);
        if (IsItemActive())
            painter->drawText(TextRect, GetName(), QTextOption(Qt::AlignCenter));
    }

    painter->restore();
}

void CGUIPoint::SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord& centralCoord)
{

    m_CenterPoint.setX((GetCoord().GetX() - qllXMin) * dScale);
    m_CenterPoint.setY((qllYMax - GetCoord().GetY()) * dScale);

    QFont font = scene()->font();
    font.setPointSize(AppSettings::GetTextSize().GetCurrentTextSize());
    QFontMetricsF metrics(font);
    QRectF TextRect(metrics.boundingRect(GetName()));
    TextRect.adjust(-1, -1, 1, 1);
    TextRect.moveTo(0, 0);
    TextRect.setBottom(TextRect.bottom() + 3);

    if(!GetSignActive() && GetType() == CPoint::enSSAWaypoint)
    {
        QPointF LeftTop(m_CenterPoint.x() - TextRect.right()/2, m_CenterPoint.y() - TextRect.bottom() - 5 - m_iPenWidth);
        setPos(LeftTop);
        m_CenterPoint.rx() -= LeftTop.x();
        m_CenterPoint.ry() -= LeftTop.y();

        m_Zone.setTop(0);
        m_Zone.setLeft(0);
        m_Zone.setRight(TextRect.right());
        m_Zone.setBottom(TextRect.bottom() + 9 + 2*m_iPenWidth);
    }
    else
    {
        const QPixmap& pxm = m_Pixmaps.GetPixmap(GetType());
        if(pxm.isNull())
        {
            QPointF LeftTop(m_CenterPoint.x() - TextRect.right()/2, m_CenterPoint.y() - TextRect.bottom() - 5 - m_iPenWidth);
            setPos(LeftTop);
            m_CenterPoint.rx() -= LeftTop.x();
            m_CenterPoint.ry() -= LeftTop.y();

            m_Zone.setTop(0);
            m_Zone.setLeft(0);
            m_Zone.setRight(TextRect.right());
            m_Zone.setBottom(TextRect.bottom() + 9 + 2*m_iPenWidth);
        }
        else
        {
            QPointF LeftTop(m_CenterPoint.x() - qMax(TextRect.right()/2, qreal(pxm.width())/2), m_CenterPoint.y() - TextRect.bottom() - qreal(pxm.height())/2);
            setPos(LeftTop);
            m_CenterPoint.rx() -= LeftTop.x();
            m_CenterPoint.ry() -= LeftTop.y();

            m_Zone.setTop(0);
            m_Zone.setLeft(0);
            m_Zone.setRight(TextRect.right());
            m_Zone.setBottom(TextRect.bottom() + pxm.height());
        }
    }
}

void CGUIPoint::MarkItemActive(bool bActive)
{
    CPoint::MarkItemActive(bActive);
    //setVisible(bActive);
}
