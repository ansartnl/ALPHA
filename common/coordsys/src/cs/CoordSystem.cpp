#include "CoordSystem.h"


QPointF CCoordSystem::toSceneWithTransform(const QVector3D &pointModel) const
{
    QPointF pointScene = toScene(pointModel);
    return m_mtxScene.map(pointScene);
}

QPointF CCoordSystem::toScreen(const QPointF &pointScene) const
{
    return m_mtxViewport.map(pointScene);
}

QPointF CCoordSystem::fromScreen(const QPointF &pointScreen) const
{
    return m_mtxViewportInverted.map(pointScreen);
}

qreal CCoordSystem::distanceToScene(qreal distModel) const
{
    // Average distance from 2 directions.
    return ( ((distModel * m_ScaleFactorW) + (distModel * m_ScaleFactorH)) / 2.0 );
}

qreal CCoordSystem::distanceToSceneWT(qreal distModel) const
{
    qreal distScene = distanceToScene(distModel);
    // As scale is the same for X and Y axis - retrieve only X value.
    return m_mtxScene.map(QPointF(distScene,distScene)).x();
}

qreal CCoordSystem::distanceToModel(qreal distScene) const
{
    // Average distance from 2 directions.
    return ( ((distScene / m_ScaleFactorW) + (distScene / m_ScaleFactorH)) / 2.0 );
}

void CCoordSystem::setSceneTransform(qreal distance, qreal pxlInMmWidth, qreal pxlInMmHeight)
{
    m_pxlInMmWidth = pxlInMmWidth;
    m_pxlInMmHeight = pxlInMmHeight;

    m_ScaleFactorW = (pxlInMmWidth*10.0) / distance;
    m_ScaleFactorH = (pxlInMmHeight*10.0) / distance;
}

void CCoordSystem::setViewportTransform(const QTransform &mtx)
{
    bool invertible;
    m_mtxViewport = mtx;
    m_mtxViewportInverted = mtx.inverted(&invertible);
}

void CCoordSystem::defineProjectionRotation(qreal angle)
{
    m_angle = angle;
    m_mtxAngle = QTransform().rotate(angle);
}
