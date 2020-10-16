#include "GUIRcGrid.h"
#include <QPainter>

#if defined(_MSC_VER) && QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include "Coordinate.h"
#include "main.h"
#include "back/utils/ProfileSettings.h"

extern QString currentUserName;

CGUIRcGrid::CGUIRcGrid()
{
    setZValue(gui::Zrcgrid);
    m_values.initDefault();
    SetColor(UserProfile::CSettings::Instance().GetColor(UserProfile::CtGrid));
    setRcGridValues(m_values);
}

QRectF CGUIRcGrid::boundingRect() const
{
    return m_boundingRect;
}

void CGUIRcGrid::paint(
        QPainter *painter,
        const QStyleOptionGraphicsItem *option,
        QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (!m_values.mRadius || m_values.mCenter.IsNull() || !m_values.mCenter.IsCenter())
        return;

    painter->save();

    qreal sceneRadius(m_values.mRadius * m_scale);

    QPen pen;
    pen.setCosmetic(true);

    int i(0);
    foreach (qreal stripeRadius, m_stripeRadiusList)
    {
        qreal stripeSceneRadius(stripeRadius * m_scale);
        pen.setColor(m_stripeColorList[i]);
        pen.setWidthF(m_stripeSizeList[i]);
        painter->setPen(pen);
        painter->drawEllipse(QPointF(0, 0),
                                 stripeSceneRadius, stripeSceneRadius);
        ++i;
    }

    i = 0;
    foreach (qreal sectorAngle, m_sectorAngleList)
    {
        pen.setColor(m_sectorColorList[i]);
        pen.setWidthF(m_sectorSizeList[i]);
        painter->setPen(pen);
        painter->drawLine(QLineF(0, 0,
                                 sceneRadius * cos((-sectorAngle + 90) * M_PI / 180),
                                 -sceneRadius * sin((-sectorAngle + 90) * M_PI / 180)));
        ++i;
    }

    painter->restore();
}

void CGUIRcGrid::SetPosition(
        qint64 qllXMin,
        qint64 qllYMax,
        double dScale,
        const CCoord &centralCoord)
{
    prepareGeometryChange();
    m_scale = dScale;
    m_boundingRect = QRectF();

    if (!m_values.mRadius || m_values.mCenter.IsNull() || !m_values.mCenter.IsCenter())
        return;

    QPointF pnt((m_values.mCenter.GetX() - qllXMin) * dScale,
                (qllYMax - m_values.mCenter.GetY()) * dScale);

    m_boundingRect.setWidth(m_values.mRadius * m_scale * 2);
    m_boundingRect.setHeight(m_values.mRadius * m_scale * 2);
    m_boundingRect.moveCenter(QPointF(0, 0));

    setPos(pnt);
}

QList<qreal> CGUIRcGrid::stripeRadiusList() const
{
    return m_stripeRadiusList;
}

QList<qreal> CGUIRcGrid::stripeSizeList() const
{
    return m_stripeSizeList;
}

QList<QColor> CGUIRcGrid::stripeColorList() const
{
    return m_stripeColorList;
}

QList<qreal> CGUIRcGrid::sectorAngleList() const
{
    return m_sectorAngleList;
}

QList<qreal> CGUIRcGrid::sectorSizeList() const
{
    return m_sectorSizeList;
}

QList<QColor> CGUIRcGrid::sectorColorList() const
{
    return m_sectorColorList;
}

bool CGUIRcGrid::appendSector(qreal angle, qreal size, const QColor &color)
{
    if (angle <= 0 || size <= 0|| !color.isValid())
    {
        return false;
    }

    m_sectorAngleList.append(angle);
    m_sectorSizeList.append(size);
    m_sectorColorList.append(color);

    return true;
}

bool CGUIRcGrid::appendStripe(qreal radius, qreal size, const QColor &color)
{
    if (radius <= 0 || size <= 0|| !color.isValid() || (radius > m_values.mRadius && m_values.mRadius != 0))
    {
        return false;
    }

    m_stripeRadiusList.append(radius);
    m_stripeSizeList.append(size);
    m_stripeColorList.append(color);

    return true;
}

void CGUIRcGrid::SaveCenterCoord(const QString &sFir, const CCoord &coord)
{
    if(!currentUserName.isNull())
    {
        QSettings &settings = *globalSettings();
        settings.beginGroup(currentUserName);
        QVariant var;
        var = settings.value("RcGrid");
        QDataStream qStream(var.toByteArray());
        QMap<QString, CCoord> map;
        qStream >> map;
        map[sFir] = coord;
        {
            QByteArray qArray;
            QDataStream qStream2(&qArray, QIODevice::ReadWrite);
            qStream2 << map;
            settings.setValue("RcGrid", qArray);
        }
        settings.endGroup();
    }
}

CCoord CGUIRcGrid::LoadCenterCoord(const QString &sFir)
{
    CCoord coord;
    if(!currentUserName.isNull())
    {
        QSettings &settings = *globalSettings();
        settings.beginGroup(currentUserName);
        QVariant var;
        var = settings.value("RcGrid");
        QDataStream qStream(var.toByteArray());
        QMap<QString, CCoord> map;
        qStream >> map;
        coord = map[sFir];
        settings.endGroup();
    }
    return coord;
}

void CGUIRcGrid::SetColor(const QColor &color)
{
    m_Color = color;
    for(int i = 0; i < m_stripeColorList.size(); ++i)
        m_stripeColorList[i] = m_Color;
    for(int i = 0; i < m_sectorColorList.size(); ++i)
        m_sectorColorList[i] = m_Color;

    update();
}

void CGUIRcGrid::setRcGridValues(const RcGridValues& values)
{
    m_values = values;
    if(m_values.mStep <= 0) m_values.mStep = 5000;
    m_values.mSector %= 360;
    if(m_values.mSector <= 0) m_values.mSector = 30;

    m_stripeRadiusList.clear();
    m_stripeSizeList.clear();
    m_stripeColorList.clear();
    for(int i = 0, iStep = 0; i < 1000; ++i)
    {
        iStep += m_values.mStep;
        if(iStep > m_values.mRadius)
            break;
        appendStripe(iStep, i%2 ? 2 : 1, m_Color);
    }
    m_sectorAngleList.clear();
    m_sectorSizeList.clear();
    m_sectorColorList.clear();
    int iDeclinationValue = 0;
    bool bDeclinationEnable = ("yes" == networkSettings()->value("users/declination_enable", "no").toString());
    if(bDeclinationEnable)
    {
        iDeclinationValue = networkSettings()->value("users/declination_value", 0).toInt();
        if(iDeclinationValue > 180 || iDeclinationValue < -180)
            iDeclinationValue = 0;
    }
    for(int i = 0, iStep = 0+iDeclinationValue; i < 1000; ++i)
    {
        appendSector(iStep, 1, m_Color);
        iStep += m_values.mSector;
        if(iStep > 360+iDeclinationValue)
            break;
    }
    update();
}

void CGUIRcGrid::SaveRcGridValues(const QString& sFir, const RcGridValues& values)
{
    if(!currentUserName.isNull())
    {
        QSettings &settings = *globalSettings();
        settings.beginGroup(currentUserName);
        QVariant var;
        var = settings.value("RcGridValues");
        QDataStream qStream(var.toByteArray());
        QMap<QString, RcGridValues> map;
        qStream >> map;
        map[sFir] = values;
        {
            QByteArray qArray;
            QDataStream qStream2(&qArray, QIODevice::ReadWrite);
            qStream2 << map;
            settings.setValue("RcGridValues", qArray);
        }
        settings.endGroup();
    }
}

RcGridValues CGUIRcGrid::LoadRcGridValues(const QString& sFir)
{
    RcGridValues values;
    if(!currentUserName.isNull())
    {
        QSettings &settings = *globalSettings();
        settings.beginGroup(currentUserName);
        QVariant var;
        var = settings.value("RcGridValues");
        QDataStream qStream(var.toByteArray());
        QMap<QString, RcGridValues> map;
        qStream >> map;
        values = map[sFir];
        settings.endGroup();
    }
    return values;
}
