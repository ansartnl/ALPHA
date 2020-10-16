#include "CoordSystemProj.h"
#include "GaussKrugerCS.h"

#include <QtDebug>
#include <QVector2D>
#include <QLineF>

#include "proj_api.h"

CCoordSystemProj::CCoordSystemProj()
: m_projHandler(0), m_CenterPoint(0.0, 0.0, 0.0),
  m_CenterZone(1)
{
    m_projHandler = pj_init_plus("+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs");
}

CCoordSystemProj::~CCoordSystemProj()
{
    if ( m_projHandler )
        pj_free(m_projHandler);
}

QVector3D CCoordSystemProj::toModel(const QVector3D &pointGeo) const
{
    QVector3D pointGeoDegree(pointGeo.x() * 180.0 / M_PI,
        pointGeo.y() * 180.0 / M_PI,
        0.0);

    return toModelDegree(pointGeoDegree);
}

QVector3D CCoordSystemProj::toModelDegree(const QVector3D &pointGeoDegree) const
{
    if ( !m_projHandler )
        return QVector3D(0.0, 0.0, 0.0);

    projLP bl;
    bl.v = pointGeoDegree.x() * DEG_TO_RAD;
    bl.u = pointGeoDegree.y() * DEG_TO_RAD;


    projXY xy = pj_fwd(bl, m_projHandler);


    if ( xy.u == HUGE_VAL )
    {
        qDebug("%s", pj_strerrno(pj_errno));
        return QVector3D(0.0, 0.0, 0.0);
    }

    qreal x, y;
    x = xy.u;
    y = xy.v;
//    geo2xy(pointGeoDegree.x(), pointGeoDegree.y(), y, x, m_CenterZone);

    return QVector3D(x, y, 0.0);
}

void CCoordSystemProj::toModel(CCoord &coord) const
{
    if ( !m_projHandler )
    {
        coord.SetX(0);
        coord.SetY(0);
        coord.SetZ(0);
        return;
    }

    qreal X, Y;
//    geo2xy(coord.GetLatitude().GetAngle(),
//           coord.GetLongitude().GetAngle(),
//           Y, X,
//           m_CenterZone);

    projLP bl;
    bl.v = coord.GetLatitude().GetAngle() * DEG_TO_RAD;
    bl.u = coord.GetLongitude().GetAngle() * DEG_TO_RAD;

    qDebug("latitude %.4f, longitude %.4f", bl.u, bl.v);
    projXY xy = pj_fwd(bl, m_projHandler);
    qDebug("x %.4f, y %.4f", xy.u, xy.v);

    if ( xy.u == HUGE_VAL )
    {
        qDebug("%s", pj_strerrno(pj_errno));
        coord.SetX(0);
        coord.SetY(0);
        coord.SetZ(0);
        return;
    }

    X = xy.u;
    Y = xy.v;

    coord.SetX(X);
    coord.SetY(Y);
    coord.SetZ(0);
}

QVector3D CCoordSystemProj::toGeo(const QVector3D &pointModel) const
{
    if ( !m_projHandler )
        return QVector3D(0.0, 0.0, 0.0);

    qreal B, L;
//    xy2geo(pointModel.y(), pointModel.x(), B, L, m_CenterZone);

    projXY xy;
    xy.u = pointModel.x();
    xy.v = pointModel.y();
    projLP bl = pj_inv(xy, m_projHandler);

    if ( bl.u == HUGE_VAL )
    {
        qDebug("%s", pj_strerrno(pj_errno));
        return QVector3D(0.0, 0.0, 0.0);
    }

    B = bl.u;
    L = bl.v;

    return QVector3D(B, L, 0.0);
}

QVector3D CCoordSystemProj::toGeoDegree(const QVector3D &pointModel) const
{
    QVector3D pointGeo = toGeo(pointModel);

    return QVector3D(pointGeo.x() * 180.0 / MM_PI,
        pointGeo.y() * 180.0 / MM_PI,
        pointGeo.z());
}

void CCoordSystemProj::toGeo(CCoord &coord) const
{
    if ( !m_projHandler )
    {
        coord.SetLatitude(CAngle(0.0));
        coord.SetLongitude(CAngle(0.0));
        coord.SetHeight(0.0);
        return;
    }

    qreal B, L;
//    xy2geo(coord.GetY(), coord.GetX(),
//           B, L,
//           m_CenterZone);

    projXY xy;
    xy.u = coord.GetX();
    xy.v = coord.GetY();
    projLP bl = pj_inv(xy, m_projHandler);

    if ( bl.u == HUGE_VAL )
    {
        qDebug("%s", pj_strerrno(pj_errno));
        coord.SetLatitude(CAngle(0.0));
        coord.SetLongitude(CAngle(0.0));
        coord.SetHeight(0.0);
        return;
    }

    B = bl.u;
    L = bl.v;

    coord.SetLatitude(CAngle(B*180.0/MM_PI));
    coord.SetLongitude(CAngle(L*180.0/MM_PI));
    coord.SetHeight(0.0);
}

QVector3D CCoordSystemProj::toModel(const QPointF &pointScene) const
{
    QVector3D pointModel(
            pointScene.x()/m_ScaleFactorW,
            -pointScene.y()/m_ScaleFactorH,
            0.0);

    return ( pointModel + m_CenterPoint );
}

QPointF CCoordSystemProj::toScene(const QVector3D &pointModel) const
{
    QVector3D pointScene3d = pointModel - m_CenterPoint;

//    return QPointF(pointScene3d.x()*m_ScaleFactorW,
//                   -pointScene3d.y()*m_ScaleFactorH);

    return m_mtxAngle.map( QPointF(pointScene3d.x()*m_ScaleFactorW,
                                   -pointScene3d.y()*m_ScaleFactorH) );
}

QPointF CCoordSystemProj::toSceneNoRotation(const QVector3D &pointModel) const
{
    QVector3D pointScene3d = pointModel - m_CenterPoint;

    return QPointF(pointScene3d.x()*m_ScaleFactorW,
                   -pointScene3d.y()*m_ScaleFactorH);
}

QPointF CCoordSystemProj::toScene(const CCoord &coord) const
{
    return toScene( QVector3D(coord.GetX(), coord.GetY(), coord.GetZ()) );
}

qreal CCoordSystemProj::distance(const CCoord &coord1, const CCoord &coord2) const
{
    return QLineF(QPointF(coord1.GetX(), coord1.GetY()),
                  QPointF(coord2.GetX(), coord2.GetY())).length();
}

void CCoordSystemProj::defineCenterPoint(qreal latitude, qreal longitude)
{
    m_CenterBL = QPointF(latitude, longitude);

    qreal zoneNumber = qint32((6.0+longitude)/6.0);

    if ( m_projHandler )
        pj_free(m_projHandler);

//    m_projHandler = pj_init_plus("+proj=merc +lon_0=0 +k=1 +x_0=0 +y_0=0 +ellps=WGS84 +datum=WGS84 +units=m +no_defs");
    m_projHandler = pj_init_plus("+proj=merc +lon_0=28e +k=1 +x_0=0 +y_0=0 +ellps=WGS84 +datum=WGS84 +units=m +no_defs");



    if ( !m_projHandler )
    {
        qDebug("CCoordSystemProj::defineCenterPoint: %s", pj_strerrno(pj_errno));
        return;
    }

    projLP bl;

    bl.v = latitude * DEG_TO_RAD;
    bl.u = longitude * DEG_TO_RAD;

    projXY xy = pj_fwd(bl, m_projHandler);

    if ( xy.u == HUGE_VAL )
    {
        if ( m_projHandler )
            pj_free(m_projHandler);
        m_projHandler = 0;
        qDebug("%s", pj_strerrno(pj_errno));
        return;
    }

    qreal X, Y;
    X = xy.u;
    Y = xy.v;

    // In Gauss Kruger projection axis X and Y are reversed.
    //m_CenterPoint = QVector3D(X, Y, 0.0);
    m_CenterPoint = QVector3D(0.0, 0.0, 0.0);
    m_CenterZone = qint32(X / 1000000);

    if ( m_projHandler )
        pj_free(m_projHandler);

//    m_projHandler = pj_init_plus("+proj=merc +lon_0=0 +k=1 +x_0=0 +y_0=0 +ellps=WGS84 +datum=WGS84 +units=m +no_defs");
    m_projHandler = pj_init_plus("+proj=merc +lon_0=28e +k=1 +x_0=0 +y_0=0 +ellps=WGS84 +datum=WGS84 +units=m +no_defs");

    if ( !m_projHandler )
        qDebug("CCoordSystemProj::defineCenterPoint: %s", pj_strerrno(pj_errno));
}

qreal CCoordSystemProj::findAngleToNorth(qreal latitude, qreal longitude) const
{
    QVector3D pointCenterModel = toModelDegree(QVector3D(latitude, longitude, 0.0));
    QVector3D pointNorthModel = toModelDegree(QVector3D(90.0, longitude, 0.0));

    QPointF pointCenterScene = toScene(pointCenterModel),
        pointNorthScene = toScene(pointNorthModel);

    qreal angleQt = QLineF(QPointF(0.0, 0.0),
           pointNorthScene - pointCenterScene).angle();

    return fmod((360.0 - angleQt + 90.0), 360.0);
}
