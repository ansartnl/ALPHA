#include "GUIGroundSign.h"
#include <QToolTip>
#include <QPainter>
#include <math.h>

GUIGroundSign::GUIGroundSign(QGraphicsItem *parent)
    : QGraphicsItem(parent), mPenWidth(1), mColor(Qt::darkYellow), mParsedLabel()
{
    setZValue(gui::ZSign);
}

GUIGroundSign::~GUIGroundSign()
{
}

QRectF GUIGroundSign::boundingRect() const
{
    return mHoverPath.boundingRect();
}

QPainterPath GUIGroundSign::shape() const
{
    return mHoverPath;
}

void GUIGroundSign::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    QPen qPen(mColor);
    qPen.setWidth(mPenWidth);
    painter->setPen(qPen);
    painter->setBrush(QBrush(mColor, Qt::SolidPattern));
    painter->drawPath(mPainterPath);
    painter->restore();
}

void GUIGroundSign::SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord &centralCoord)
{
    prepareGeometryChange();
    mPainterPath = QPainterPath();
    mHoverPath = QPainterPath();

    QPointF point(
        (coord().GetX() - qllXMin) * dScale,
        (qllYMax - coord().GetY()) * dScale);
    point = mapFromScene(point);

    double scaledRadius = 6 *dScale;
    mPainterPath.addEllipse(point, scaledRadius, scaledRadius);

    scaledRadius = ground_sign::HOVER_RADIUS *dScale;
    mHoverPath.addEllipse(point, scaledRadius, scaledRadius);

    mParsedLabel = parseLabel();
    setToolTip(mParsedLabel);

    update();
}

QString GUIGroundSign::parseLabel() const
{
    QString res;
    QString original = label();
    bool bFirst = true;

    foreach(QChar ch, original)
    {
        if(ch ==  'd')
        {
            if(!bFirst)
                res.append(QString("</span>"));
            res.append(QString("<span style='color: black; background-color: yellow; font-size: 20pt;'>"));
            bFirst = false;
        }
        else if(ch ==  'l')
        {
            if(!bFirst)
                res.append(QString("</span>"));
            res.append(QString("<span style='color: yellow; background-color: black; font-size: 20pt;'>"));
            bFirst = false;
        }
        else if(ch ==  'm')
        {
            if(!bFirst)
                res.append(QString("</span>"));
            res.append(QString("<span style='color: white; background-color: red; font-size: 20pt;'>"));
            bFirst = false;
        }
        else if(ch ==  'i')
        {
            if(!bFirst)
                res.append(QString("</span>"));
            res.append(QString("<span style='color: white; background-color: black; font-size: 20pt;'>"));
            bFirst = false;
        }
        else if(ch ==  '>')
            res.append(QString("\u2192"));
        else if(ch ==  '<')
            res.append(QString("\u2190"));
        else
            res.append(ch);

    }
    if(!bFirst)
        res.append(QString("</span>"));

    return res;
}


