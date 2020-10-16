#include "CoordSystemStereo.h"
#include "StereoCS.h"

#include <QtCore/QObject>
#include <QtCore/QLineF>
#include <QtGui/QVector2D>


CCoordSystemStereo::CCoordSystemStereo()
: m_CenterPoint(0.0, 0.0, 0.0),
  // WSG 84 coefficients
  mEquaRadius(6378206.4), mEccentricity(0.0822719), mScaleK0(0.9999)
{
}

QString CCoordSystemStereo::GetClassProjectionName()
{
    return QObject::tr("Stereographic");
}

QString CCoordSystemStereo::GetProjectionName()
{
    return CCoordSystemStereo::GetClassProjectionName();
}

QVector3D CCoordSystemStereo::toModel(const QVector3D &pointGeo) const
{
    QVector3D pointGeoDegree(pointGeo.x() * RAD_TO_DEG,
        pointGeo.y() * RAD_TO_DEG,
        0.0);

    return toModelDegree(pointGeoDegree);
}

QVector3D CCoordSystemStereo::toModelDegree(const QVector3D &pointGeoDegree) const
{
    qreal x, y;
    stereo_fwd(pointGeoDegree.x(), pointGeoDegree.y(),
               m_CenterBL.x(), m_CenterBL.y(),
               mScaleK0, mEquaRadius, mEccentricity,
               x, y);

    return QVector3D(x, y, 0.0);
}

void CCoordSystemStereo::toModel(CCoord &coord) const
{
    qreal x, y;
    stereo_fwd(coord.GetLatitude().GetAngle(), coord.GetLongitude().GetAngle(),
               m_CenterBL.x(), m_CenterBL.y(),
               mScaleK0, mEquaRadius, mEccentricity,
               x, y);

    coord.SetX(x);
    coord.SetY(y);
    coord.SetZ(0);
}

QVector3D CCoordSystemStereo::toGeo(const QVector3D &pointModel) const
{
    qreal b, l;
    stereo_inv(pointModel.x(), pointModel.y(),
               m_CenterBL.x(), m_CenterBL.y(),
               mScaleK0, mEquaRadius, mEccentricity,
               b, l);

    return QVector3D(b*DEG_TO_RAD, l*DEG_TO_RAD, 0.0);
}

QVector3D CCoordSystemStereo::toGeoDegree(const QVector3D &pointModel) const
{
    QVector3D pointGeo = toGeo(pointModel);

    return QVector3D(pointGeo.x() * RAD_TO_DEG,
        pointGeo.y() * RAD_TO_DEG,
        pointGeo.z());
}

void CCoordSystemStereo::toGeo(CCoord &coord) const
{
    qreal b, l;
    stereo_inv(coord.GetX(), coord.GetY(),
               m_CenterBL.x(), m_CenterBL.y(),
               mScaleK0, mEquaRadius, mEccentricity,
               b, l);

    coord.SetLatitude(CAngle(b));
    coord.SetLongitude(CAngle(l));
    coord.SetHeight(0.0);
}

QVector3D CCoordSystemStereo::toModel(const QPointF &pointScene) const
{
    QVector3D pointModel(
            pointScene.x()/m_ScaleFactorW,
            -pointScene.y()/m_ScaleFactorH,
            0.0);

    return ( pointModel + m_CenterPoint );
}

QPointF CCoordSystemStereo::toScene(const QVector3D &pointModel) const
{
    QVector3D pointScene3d = pointModel - m_CenterPoint;

//    return QPointF(pointScene3d.x()*m_ScaleFactorW,
//                   -pointScene3d.y()*m_ScaleFactorH);

    return m_mtxAngle.map( QPointF(pointScene3d.x()*m_ScaleFactorW,
                                   -pointScene3d.y()*m_ScaleFactorH) );
}

QPointF CCoordSystemStereo::toSceneNoRotation(const QVector3D &pointModel) const
{
    QVector3D pointScene3d = pointModel - m_CenterPoint;

    return QPointF(pointScene3d.x()*m_ScaleFactorW,
                   -pointScene3d.y()*m_ScaleFactorH);
}

QPointF CCoordSystemStereo::toScene(const CCoord &coord) const
{
    return toScene( QVector3D(coord.GetX(), coord.GetY(), coord.GetZ()) );
}

qreal CCoordSystemStereo::distance(const CCoord &coord1, const CCoord &coord2) const
{
    return QLineF(QPointF(coord1.GetX(), coord1.GetY()),
                  QPointF(coord2.GetX(), coord2.GetY())).length();
}

void CCoordSystemStereo::defineCenterPoint(qreal latitude, qreal longitude)
{
    m_CenterBL = QPointF(latitude, longitude);

    qreal x, y;
    stereo_fwd(latitude, longitude,
               latitude, longitude,
               mScaleK0, mEquaRadius, mEccentricity,
               x, y);

    m_CenterPoint = QVector3D(x, y, 0.0);
}

qreal CCoordSystemStereo::findAngleToNorth(qreal latitude, qreal longitude) const
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
