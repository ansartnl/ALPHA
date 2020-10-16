#include "CoordSystemGK.h"
#include "GaussKrugerCS.h"

#include <QtCore/QObject>
#include <QtCore/QLineF>
#include <QtGui/QVector2D>


CCoordSystemGK::CCoordSystemGK()
: m_CenterPoint(0.0, 0.0, 0.0),
  m_CenterZone(1)
{
}

QString CCoordSystemGK::GetClassProjectionName()
{
    return QObject::tr("Gauss-Kruger");
}

QString CCoordSystemGK::GetProjectionName()
{
    return CCoordSystemGK::GetClassProjectionName();
}

QVector3D CCoordSystemGK::toModel(const QVector3D &pointGeo) const
{
    QVector3D pointGeoDegree(pointGeo.x() * 180.0 / M_PI,
        pointGeo.y() * 180.0 / M_PI,
        0.0);

    return toModelDegree(pointGeoDegree);
}

QVector3D CCoordSystemGK::toModelDegree(const QVector3D &pointGeoDegree) const
{
    qreal x, y;
    geo2xy(pointGeoDegree.x(), pointGeoDegree.y(), y, x, m_CenterZone);

    return QVector3D(x, y, 0.0);
}

void CCoordSystemGK::toModel(CCoord &coord) const
{
    qreal X, Y;
    geo2xy(coord.GetLatitude().GetAngle(),
           coord.GetLongitude().GetAngle(),
           Y, X,
           m_CenterZone);

    coord.SetX(X);
    coord.SetY(Y);
    coord.SetZ(0);
}

QVector3D CCoordSystemGK::toGeo(const QVector3D &pointModel) const
{
    qreal B, L;
    xy2geo(pointModel.y(), pointModel.x(), B, L, m_CenterZone);

    return QVector3D(B, L, 0.0);
}

QVector3D CCoordSystemGK::toGeoDegree(const QVector3D &pointModel) const
{
    QVector3D pointGeo = toGeo(pointModel);

    return QVector3D(pointGeo.x() * 180.0 / MM_PI,
        pointGeo.y() * 180.0 / MM_PI,
        pointGeo.z());
}

void CCoordSystemGK::toGeo(CCoord &coord) const
{
    qreal B, L;
    xy2geo(coord.GetY(), coord.GetX(),
           B, L,
           m_CenterZone);

    coord.SetLatitude(CAngle(B*180.0/MM_PI));
    coord.SetLongitude(CAngle(L*180.0/MM_PI));
    coord.SetHeight(0.0);
}

QVector3D CCoordSystemGK::toModel(const QPointF &pointScene) const
{
    QVector3D pointModel(
            pointScene.x()/m_ScaleFactorW,
            -pointScene.y()/m_ScaleFactorH,
            0.0);

    return ( pointModel + m_CenterPoint );
}

QPointF CCoordSystemGK::toScene(const QVector3D &pointModel) const
{
    QVector3D pointScene3d = pointModel - m_CenterPoint;

//    return QPointF(pointScene3d.x()*m_ScaleFactorW,
//                   -pointScene3d.y()*m_ScaleFactorH);

    return m_mtxAngle.map( QPointF(pointScene3d.x()*m_ScaleFactorW,
                                   -pointScene3d.y()*m_ScaleFactorH) );
}

QPointF CCoordSystemGK::toSceneNoRotation(const QVector3D &pointModel) const
{
    QVector3D pointScene3d = pointModel - m_CenterPoint;

    return QPointF(pointScene3d.x()*m_ScaleFactorW,
                   -pointScene3d.y()*m_ScaleFactorH);
}

QPointF CCoordSystemGK::toScene(const CCoord &coord) const
{
    return toScene( QVector3D(coord.GetX(), coord.GetY(), coord.GetZ()) );
}

qreal CCoordSystemGK::distance(const CCoord &coord1, const CCoord &coord2) const
{
    return QLineF(QPointF(coord1.GetX(), coord1.GetY()),
                  QPointF(coord2.GetX(), coord2.GetY())).length();
}

void CCoordSystemGK::defineCenterPoint(qreal latitude, qreal longitude)
{
    m_CenterBL = QPointF(latitude, longitude);

    qreal X, Y;
    geo2xy(latitude, longitude, X, Y);

    // In Gauss Kruger projection axis X and Y are reversed.
    m_CenterPoint = QVector3D(Y, X, 0.0);
    m_CenterZone = qint32(Y / 1000000);
}

qreal CCoordSystemGK::findAngleToNorth(qreal latitude, qreal longitude) const
{
    QVector3D pointCenterModel = toModelDegree(QVector3D(latitude, longitude, 0.0));
    QVector3D pointNorthModel = toModelDegree(QVector3D(90.0, longitude, 0.0));

    QPointF pointCenterScene = toScene(pointCenterModel),
        pointNorthScene = toScene(pointNorthModel);

//    qreal angleQt = QLineF(QPointF(pointCenterModel.x(), pointCenterModel.y()),
//           QPointF(pointNorthModel.x(), pointNorthModel.y())).angle();
    qreal angleQt = QLineF(QPointF(0.0, 0.0),
           pointNorthScene - pointCenterScene).angle();

    return fmod((360.0 - angleQt + 90.0), 360.0);
}
