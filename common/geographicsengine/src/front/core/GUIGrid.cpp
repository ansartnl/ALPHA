#include "GUIGrid.h"
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <math.h>
#include "front/common/UserSettings.h"
#include "front/common/ZOrderValue.h"


//! Geodesy graphics engine
namespace geoGE {

///////////////////////////////////////////////////////////////////////////////////////
// GUIGrid implementation.

GUIGrid::GUIGrid(QGraphicsItem *parent)
    : GUIUntransformableObject(parent), m_isFullscreen(false)
{
    // Display grid above all objects in the scene.
    setZValue( getZOrderValue(ZOrderGrid) );
    SetColor( UserSettings::Accessor::instance().color(UserSettings::CtGrid) );
}

void GUIGrid::DefineFont()
{
    m_font = scene()->font();
    m_font.setPointSize(m_font.pointSize()-1);
}

QGraphicsView* GUIGrid::view() const
{
    QList<QGraphicsView *> views = scene()->views();
    return ( views.empty() ? 0 : views.at(0) );
}


///////////////////////////////////////////////////////////////////////////////////////
// GUIGridRange implementation.


void GUIGridRange::SetFullscreen(bool isFullscreen)
{
    GUIGrid::SetFullscreen(isFullscreen);

    // Workaround to reset number of major intervals.
    if ( !isFullscreen )
        SetUnits(m_units);
}

void GUIGridRange::updateMajorIntervalsCount(const TCoordSystem &cs)
{
    if ( m_isFullscreen )
    {
        QGraphicsView* viewGraphics = view();
        Q_ASSERT(viewGraphics);

        QRect rectViewport = viewGraphics->viewport()->rect();
        QPointF centerPointScene = viewGraphics->mapToScene(rectViewport.center());
        QPointF topLeft = viewGraphics->mapToScene(rectViewport.topLeft());

        qreal distModel = 1000.0*(qreal)m_distance * (m_units == Kilometers ? 1.0 : 1.852);

        m_majorNumber = qRound( cs->distanceToModel(
                QLineF(centerPointScene, topLeft).length() ) / distModel );
        // foolproof
        m_majorNumber = m_majorNumber < 1 ? 1 : m_majorNumber;
    }
}



///////////////////////////////////////////////////////////////////////////////////////
// GUIGridAzimuth implementation.

QRectF GUIGridAzimuth::boundingRect() const
{
    return m_painterPath.boundingRect();
}

QPainterPath GUIGridAzimuth::shape() const
{
    return m_painterPath;
}

void GUIGridAzimuth::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // Do not display the grid if its distance was reset.
    if ( m_distance == 0 )
        return;

    if ( !m_pathDash.isEmpty() )
    {
        QPen qPenDash( GetColor() );
        qPenDash.setStyle(Qt::DashLine);
        painter->setPen(qPenDash);

        painter->drawPath( m_pathDash );
    }

    if ( !m_pathThin.isEmpty() )
    {
        QPen qPenThin( GetColor() );
        painter->setPen(qPenThin);

        painter->drawPath( m_pathThin );
    }

    if ( !m_pathThick.isEmpty() )
    {
        QPen qPenThick( GetColor() );
        qPenThick.setWidth(2);
        painter->setPen(qPenThick);

        painter->drawPath( m_pathThick );
    }

    if ( !m_pathThick3.isEmpty() )
    {
        QPen qPenThick( GetColor() );
        qPenThick.setWidth(3);
        painter->setPen(qPenThick);

        painter->drawPath( m_pathThick3 );
    }

    // Draw labels ...

    painter->setFont(m_font);

    QFontMetricsF fm(m_font);
    const qreal heightFont = fm.height();

    QPen qPenLabel( GetColor() );
    painter->setPen(qPenLabel);

    int textNumber = m_majorNumber * 50.0 / m_distance;
    for ( int i = 0; i <= textNumber; i++ )
    {
        qreal upy = -m_shiftText*(qreal)i + heightFont;
        painter->drawText(3.0, upy, QString::number(m_distance*i));
    }
}

void GUIGridAzimuth::SetPosition(const TCoordSystem &cs)
{
    Q_ASSERT(m_distance <= 50 && m_distance >= 0);

    // Do not display the grid if its distance was reset.
    if ( m_distance == 0 )
        return;

    // This ratio is necessary when painting is squizzing or stretching (e.g. printing)
    // for untransformable items.
    qreal xyRatio = transformToRatio(QPointF(1.0, 1.0)).x();

    qreal distModel50 = 50000.0 * (m_units == Kilometers ? 1.0 : 1.852);
    qreal distScene50 = cs->distanceToSceneWT(distModel50)*xyRatio;

    qreal distHalfModel = 1000.0*(qreal)m_distance * (m_units == Kilometers ? 1.0 : 1.852) / 2.0;
    qreal distHalfScene = cs->distanceToSceneWT(distHalfModel)*xyRatio;

    // Need for fullscreen mode.
    updateMajorIntervalsCount(cs);

    prepareGeometryChange();

    qreal distMaxScene = distScene50 * (qreal)m_majorNumber;

    m_painterPath = QPainterPath();
    m_pathThick = QPainterPath();
    m_pathThick3 = QPainterPath();
    m_pathThin = QPainterPath();
    m_pathDash = QPainterPath();

    QPointF centerPoint(0.0, 0.0);

    // Draw center cross ...

    qreal distModelCross = 5000.0 * (m_units == Kilometers ? 1.0 : 1.852);
    qreal distSceneCross = cs->distanceToSceneWT(distModelCross)*xyRatio;

    m_pathThin.moveTo(QPointF(-distSceneCross, 0.0));
    m_pathThin.lineTo(QPointF(distSceneCross, 0.0));
    m_pathThin.moveTo(QPointF(0.0, distSceneCross));
    m_pathThin.lineTo(QPointF(0.0, -distSceneCross));

    // Draw major circle lines ...

    // major circles - 2 width thickness
    for ( int i = (m_distance == 50 ? 2 : 1);
          i <= m_majorNumber; i += 2 )
    {
        qreal radius = distScene50*(qreal)i;
        m_pathThick.addEllipse(centerPoint, radius, radius);
    }
    // major circles - 3 width thickness
    if ( m_distance < 50 )
    {
        for ( int i = 2; i <= m_majorNumber; i += 2 )
        {
            qreal radius = distScene50*(qreal)i;
            m_pathThick3.addEllipse(centerPoint, radius, radius);
        }
    }
    if ( m_azimuth < 30 && m_azimuth > 0 )
    {
        int azimuthLineNumber = 360 / 30;
        for ( int i = 0; i < azimuthLineNumber; i++ )
        {
            qreal angle = (qreal)i*30.0;

            QLineF lineDirection(centerPoint, QPointF(1.0, 0.0));
            lineDirection.setAngle(angle);

            m_pathThick.moveTo( QPointF(lineDirection.p2() * distHalfScene) );
            m_pathThick.lineTo( QPointF(lineDirection.p2() * distMaxScene) );
        }
    }

    // Draw minor circle and azimuth lines ...

    if ( m_distance == 50 )
    {
        for ( int i = 1; i <= m_majorNumber; i += 2 )
        {
            qreal radius = distScene50*(qreal)i;
            m_pathThin.addEllipse(centerPoint, radius, radius);
        }
    }
    if ( m_distance < 50 )
    {
        qreal distModel10 = 10000.0 * (m_units == Kilometers ? 1.0 : 1.852);
        qreal distScene10 = cs->distanceToSceneWT(distModel10)*xyRatio;

        int minorNumber = m_majorNumber*5;
        for ( int i = 1; i < minorNumber; i++ )
        {
            // Do not draw above major circle.
            if ( i % 5 == 0 )
                continue;

            qreal radius = distScene10*(qreal)i;
            m_pathThin.addEllipse(centerPoint, radius, radius);
        }
    }
    if ( m_azimuth > 0 )
    {
        int azimuth = m_azimuth < 30 ? 10 : m_azimuth;
        int azimuthLineNumber = 360 / azimuth;
        for ( int i = 0; i < azimuthLineNumber; i++ )
        {
            qreal angle = (qreal)i*azimuth;

            QLineF lineDirection(centerPoint, QPointF(1.0, 0.0));
            lineDirection.setAngle(angle);

            m_pathThin.moveTo( QPointF(lineDirection.p2() * distHalfScene) );
            m_pathThin.lineTo( QPointF(lineDirection.p2() * distMaxScene) );
        }
    }

    // Draw dash circle and azimuth lines ...

    if ( m_distance < 10 )
    {
        qreal distModelAny = 1000.0*(qreal)m_distance * (m_units == Kilometers ? 1.0 : 1.852);
        qreal distSceneAny = cs->distanceToSceneWT(distModelAny)*xyRatio;

        int dashNumber = m_majorNumber * 50 / m_distance;
        for ( int i = 1; i <= dashNumber; i++ )
        {
            int idi = i*m_distance;
            // Display dash circles only between 1 .. 9 km/NM range.
            if ( (idi >= 10) && (idi % 10 == 0) )
                continue;

            qreal radius = distSceneAny*(qreal)i;
            m_pathDash.addEllipse(centerPoint, radius, radius);
        }
    }
    if ( m_azimuth < 10 && m_azimuth > 0 )
    {
        int azimuthLineNumber = 360 / m_azimuth;
        for ( int i = 1; i < azimuthLineNumber; i++ )
        {
            // Do not draw every 10 degree line.
            if ( (i >= 10) && (i % 10 == 0) )
                continue;

            qreal angle = (qreal)i*m_azimuth;

            QLineF lineDirection(centerPoint, QPointF(1.0, 0.0));
            lineDirection.setAngle(angle);

            m_pathDash.moveTo( QPointF(lineDirection.p2() * distHalfScene) );
            m_pathDash.lineTo( QPointF(lineDirection.p2() * distMaxScene) );
        }
    }

    // Make path group

    m_painterPath.addPath(m_pathThick3);
    m_painterPath.addPath(m_pathThick);
    m_painterPath.addPath(m_pathThin);

    // Find shift for drawing labels ...

    qreal distModelAny = 1000.0*(qreal)m_distance * (m_units == Kilometers ? 1.0 : 1.852);
    m_shiftText = cs->distanceToSceneWT(distModelAny)*xyRatio;

    DefineFont();

    update();
}



///////////////////////////////////////////////////////////////////////////////////////
// GUIGridCartesian implementation.

QRectF GUIGridCartesian::boundingRect() const
{
    return m_boundingRect;
}

QPainterPath GUIGridCartesian::shape() const
{
    return m_painterPath;
}

void GUIGridCartesian::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // Do not display the grid if its distance was reset.
    if ( m_distance == 0 )
        return;

    if ( !m_pathDash.isEmpty() )
    {
        QPen qPenDash( GetColor() );
        qPenDash.setStyle(Qt::DashLine);
        painter->setPen(qPenDash);

        painter->drawPath( m_pathDash );
    }

    QPen qPenThin( GetColor() );
    painter->setPen(qPenThin);

    painter->drawPath( m_painterPath );

    // Draw labels ...

    painter->setFont(m_font);

    QFontMetricsF fm(m_font);
    const qreal heightFont = fm.height();

    QPen qPenLabel( GetColor() );
    painter->setPen(qPenLabel);

    if ( !(m_flags & AutoLabelPosition) )
    {
        const int textNumber = m_majorNumber * 50 / m_distance;
        for ( int i = 0; i <= textNumber; i++ )
        {
            qreal upy = -m_shiftText*(qreal)i + heightFont;
            painter->drawText(3.0, upy, QString::number(m_distance*i));
        }
    }
    else
    {
        if ( !(!scene() || scene()->views().size() < 1) )
        {
            QGraphicsView *view = scene()->views().at(0);

            QRect rectViewport = view->viewport()->rect();
            QPoint centerView = view->mapFromScene(QPointF(0.,0.));

            const int textNumber = m_majorNumber * 50 / m_distance;

            // Draw horizontal labels ...

            for ( int i = -textNumber; i <= textNumber; i++ )
            {
                // Draw labels on top of grid.
                for ( int j = -textNumber; j <= textNumber; j++ )
                {
                    QPointF pointScene(m_shiftText*(qreal)i, m_shiftText*(qreal)j);
                    if ( rectViewport.contains( centerView + pointScene.toPoint() ) )
                    {
                        painter->drawText(pointScene.x()+3.0, pointScene.y()+heightFont,
                                          QString::number(m_distance*qAbs(i)));
                        break;
                    }
                }
                // Draw labels on bottom of grid.
                for ( int j = textNumber; j >= -textNumber; j-- )
                {
                    QPointF pointScene(m_shiftText*(qreal)i, m_shiftText*(qreal)j);
                    if ( rectViewport.contains( centerView + pointScene.toPoint() ) )
                    {
                        painter->drawText(pointScene.x()+3.0, pointScene.y()+heightFont,
                                          QString::number(m_distance*qAbs(i)));
                        break;
                    }
                }
            }

            // Draw vertical labels ...

            for ( int j = -textNumber; j <= textNumber; j++ )
            {
                // Draw labels on top of grid.
                for ( int i = -textNumber; i <= textNumber; i++ )
                {
                    QPointF pointScene(m_shiftText*(qreal)i, m_shiftText*(qreal)j);
                    if ( rectViewport.contains( centerView + pointScene.toPoint() ) )
                    {
                        painter->drawText(pointScene.x()+3.0, pointScene.y()+heightFont,
                                          QString::number(m_distance*qAbs(j)));
                        break;
                    }
                }
                // Draw labels on bottom of grid.
                for ( int i = textNumber; i >= -textNumber; i-- )
                {
                    QPointF pointScene(m_shiftText*(qreal)i, m_shiftText*(qreal)j);
                    if ( rectViewport.contains( centerView + pointScene.toPoint() ) )
                    {
                        painter->drawText(pointScene.x()+3.0, pointScene.y()+heightFont,
                                          QString::number(m_distance*qAbs(j)));
                        break;
                    }
                }
            }
        }
    }
}

void GUIGridCartesian::SetPosition(const TCoordSystem &cs)
{
    Q_ASSERT(m_distance <= 50 && m_distance >= 0);

    // Do not display the grid if its distance was reset.
    if ( m_distance == 0 )
        return;

    prepareGeometryChange();

    m_painterPath = QPainterPath();
    m_pathDash = QPainterPath();

    // This ratio is necessary when painting is squizzing or stretching (e.g. printing)
    // for untransformable items.
    qreal xyRatio = transformToRatio(QPointF(1.0, 1.0)).x();

    QPointF centerPoint(0.0, 0.0);

    qreal distModel50 = 50000.0 * (m_units == Kilometers ? 1.0 : 1.852);
    qreal distScene50 = cs->distanceToSceneWT(distModel50)*xyRatio;

    // Need for fullscreen mode.
    updateMajorIntervalsCount(cs);

    // Draw major lines ...

    // Draw vertical lines
    {
        qreal yt = -distScene50*(qreal)m_majorNumber,
            yb = distScene50*(qreal)m_majorNumber;

        for ( int i = 1; i <= m_majorNumber; i++ )
        {
            qreal xl = -distScene50*(qreal)i,
                xr = distScene50*(qreal)i;

            // to left line
            m_painterPath.moveTo(xl, yt);
            m_painterPath.lineTo(xl, yb);
            // to right line
            m_painterPath.moveTo(xr, yt);
            m_painterPath.lineTo(xr, yb);
        }
        // line throught center point
        m_painterPath.moveTo(0.0, yt);
        m_painterPath.lineTo(0.0, yb);
    }
    // Draw horizontal lines
    {
        qreal xl = -distScene50*(qreal)m_majorNumber,
            xr = distScene50*(qreal)m_majorNumber;

        for ( int i = 1; i <= m_majorNumber; i++ )
        {
            qreal yt = -distScene50*(qreal)i,
                yb = distScene50*(qreal)i;

            // to top line
            m_painterPath.moveTo(xl, yt);
            m_painterPath.lineTo(xr, yt);
            // to bottom line
            m_painterPath.moveTo(xl, yb);
            m_painterPath.lineTo(xr, yb);
        }
        // line throught center point
        m_painterPath.moveTo(xl, 0.0);
        m_painterPath.lineTo(xr, 0.0);
    }

    // Draw dash minor lines ...

    if ( m_distance < 50 )
    {
        qreal distModelAny = 1000.0*(qreal)m_distance * (m_units == Kilometers ? 1.0 : 1.852);
        qreal distSceneAny = cs->distanceToSceneWT(distModelAny)*xyRatio;

        // Draw vertical lines
        {
            qreal yt = -distScene50*(qreal)m_majorNumber,
                yb = distScene50*(qreal)m_majorNumber;

            int dashNumber = m_majorNumber * 50 / m_distance;
            for ( int i = 1; i <= dashNumber; i++ )
            {
                int idi = i*m_distance;
                // Display dash lines only between 1 .. 49 km/NM range.
                if ( (idi >= 50) && (idi % 50 == 0) )
                    continue;

                qreal xl = -distSceneAny*(qreal)i,
                    xr = distSceneAny*(qreal)i;

                // to left line
                m_pathDash.moveTo(xl, yt);
                m_pathDash.lineTo(xl, yb);
                // to right line
                m_pathDash.moveTo(xr, yt);
                m_pathDash.lineTo(xr, yb);
            }
        }
        // Draw horizontal lines
        {
            qreal xl = -distScene50*(qreal)m_majorNumber,
                xr = distScene50*(qreal)m_majorNumber;

            int dashNumber = m_majorNumber * 50 / m_distance;
            for ( int i = 1; i <= dashNumber; i++ )
            {
                int idi = i*m_distance;
                // Display dash lines only between 1 .. 49 km/NM range.
                if ( (idi >= 50) && (idi % 50 == 0) )
                    continue;

                qreal yt = -distSceneAny*(qreal)i,
                    yb = distSceneAny*(qreal)i;

                // to top line
                m_pathDash.moveTo(xl, yt);
                m_pathDash.lineTo(xr, yt);
                // to bottom line
                m_pathDash.moveTo(xl, yb);
                m_pathDash.lineTo(xr, yb);
            }
        }
    }

    // Find shift for drawing labels ...

    qreal distModelAny = 1000.0*(qreal)m_distance * (m_units == Kilometers ? 1.0 : 1.852);
    m_shiftText = cs->distanceToSceneWT(distModelAny)*xyRatio;

    DefineFont();

    m_boundingRect = m_painterPath.boundingRect();
    if ( m_flags & AutoLabelPosition )
    {
        QFontMetrics fm(scene()->font());
        const qreal heightFont = fm.height();
        m_boundingRect.adjust(-2.*heightFont, -2.*heightFont, 2*heightFont, 2*heightFont);
    }

    update();
}

void GUIGridCartesian::UpdateLabelPosition()
{
    prepareGeometryChange();
}




///////////////////////////////////////////////////////////////////////////////////////
// GUIGridAzimuth implementation.

QRectF GUIGridGeodesic::boundingRect() const
{
    return m_boundingRect;
}

QPainterPath GUIGridGeodesic::shape() const
{
    return m_painterPath;
}

void GUIGridGeodesic::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen qPen( GetColor() );
    painter->setPen(qPen);

    painter->drawPath( m_painterPath );

    // Draw labels ...

    painter->setFont(m_font);

    QFontMetricsF fm(m_font);
    const qreal heightFont = fm.height();

    // Draw latitude labels ...

    if ( m_flags & AutoLabelPosition )
    {
        const int countLatLabels = m_posLat.size()/2;
        for ( int i = 0; i < countLatLabels; i++ )
        {
            QString latitude = QString::number(m_labelBox.left() + i);
            qreal widthText = fm.width(latitude);

            const QPointF& point1 = m_posLat[i*2];
            painter->drawText(point1.x()-widthText-3.0, point1.y()+heightFont, latitude);
            const QPointF& point2 = m_posLat[i*2+1];
            painter->drawText(point2.x()-widthText-3.0, point2.y()+heightFont, latitude);
        }
    }
    else
    {
        for ( int i = 0; i < m_posLat.size(); i++ )
        {
            QString latitude = QString::number(m_labelBox.left() + i);
            qreal widthText = fm.width(latitude);
            const QPointF& point = m_posLat[i];

            painter->drawText(point.x()-widthText-3.0, point.y()+heightFont, latitude);
        }
    }

    // Draw longitude labels ...

    if ( m_flags & AutoLabelPosition )
    {
        const int countLonLabels = m_posLon.size()/2;
        for ( int i = 0; i < countLonLabels; i++ )
        {
            QString longitude = QString::number(m_labelBox.top() + i);

            const QPointF& point1 = m_posLon[i*2];
            painter->drawText(point1.x()+3.0, point1.y()+heightFont, longitude);
            const QPointF& point2 = m_posLon[i*2+1];
            painter->drawText(point2.x()+3.0, point2.y()+heightFont, longitude);
        }
    }
    else
    {
        for ( int i = 0; i < m_posLon.size(); i++ )
        {
            QString longitude = QString::number(m_labelBox.top() + i);
            const QPointF& point = m_posLon[i];

            painter->drawText(point.x()+3.0, point.y()+heightFont, longitude);
        }
    }
}

void GUIGridGeodesic::SetPosition(const TCoordSystem &cs)
{
    if ( m_latLineNumber < 2 || m_lonLineNumber < 2 )
        return;

    updateMajorIntervalsCount(cs);

    prepareGeometryChange();

    m_painterPath = QPainterPath();

    int latCenter = (int) cs->getCenterLatitude(),
        lonCenter = (int) cs->getCenterLongitude();

    int latS = latCenter - m_latLineNumber/2 + 1,
        latE = latCenter + m_latLineNumber/2 + 1,
        lonS = lonCenter - m_lonLineNumber/2 + 1,
        lonE = lonCenter + m_lonLineNumber/2 + 1;

    m_points.resize(0);
    m_points.reserve(m_latLineNumber * m_lonLineNumber);

    // Make grid of points.
    for ( int iLat = latS; iLat < latE; iLat++ )
    {
        for ( int iLon = lonS; iLon < lonE; iLon++ )
        {
            QVector3D pointModel = cs->toModelDegree( QVector3D((qreal)iLat, (qreal)iLon, 0.0) );
            m_points.push_back( transformToRatio( cs->toSceneWithTransform(pointModel) ) );
        }
    }

    int latLineNumber = latE - latS;
    int lonLineNumber = lonE - lonS;

    m_labelBox.setRect(latS, lonS, latLineNumber, lonLineNumber);

    // Draw latitude lines ...

    for ( int iLat = latS; iLat < latE; iLat++ )
    {
        const int iLatShift = (iLat-latS)*lonLineNumber;
        QPointF point = m_points[iLatShift];

        m_painterPath.moveTo(point);

        for ( int iLon = lonS+1; iLon < lonE; iLon++ )
        {
            QPointF point = m_points[iLatShift + (iLon-lonS)];
            m_painterPath.lineTo(point);
        }
    }

    // Draw longitude lines ...

    for ( int iLon = lonS; iLon < lonE; iLon++ )
    {
        QPointF point = m_points[iLon-lonS];

        m_painterPath.moveTo(point);

        for ( int iLat = latS+1; iLat < latE; iLat++ )
        {
            const int iLatShift = (iLat-latS)*lonLineNumber;

            QPointF point = m_points[iLatShift + (iLon-lonS)];
            m_painterPath.lineTo(point);
        }
    }

    // Prepare positions where latitude and longitude labels are displayed ...

    m_posLat.clear();
    m_posLon.clear();
    m_posLat.reserve(latLineNumber);
    m_posLon.reserve(lonLineNumber);

    if ( m_flags & AutoLabelPosition )
        UpdateLabelPosition();
    else
    {
        // Cache latitude positions.
        for ( int iLat = latS; iLat < latE; iLat++ )
        {
            m_posLat.push_back( m_points[(iLat-latS)*lonLineNumber] );
        }
        // Cache longitude positions.
        for ( int iLon = lonS; iLon < lonE; iLon++ )
        {
            m_posLon.push_back( m_points[(latLineNumber-1)*lonLineNumber+(iLon-lonS)] );
        }

        // Do not need cache any more for this mode.
        m_points.resize(0);
    }

    DefineFont();

    QFontMetricsF fm(m_font);
    qreal widthDegree = fm.width("XXX"),
        heightDegree = fm.height();
    m_boundingRect = m_painterPath.boundingRect();
    m_boundingRect.adjust(-widthDegree, -heightDegree, 2.*widthDegree, 2.*heightDegree);

    update();
}

void GUIGridGeodesic::SetFullscreen(bool isFullscreen)
{
    GUIGrid::SetFullscreen(isFullscreen);

    if ( !isFullscreen )
    {
        m_latLineNumber = 8;
        m_lonLineNumber = 10;
    }
}

void GUIGridGeodesic::updateMajorIntervalsCount(const TCoordSystem &cs)
{
    if ( m_isFullscreen )
    {
        QGraphicsView* viewGraphics = view();
        Q_ASSERT(viewGraphics);

        QRect rectViewport = viewGraphics->viewport()->rect();

        QPoint centerView = rectViewport.center();
        QPoint centerViewTop = QPoint(centerView.x(), rectViewport.top());
        QPoint centerViewBottom = QPoint(centerView.x(), rectViewport.bottom());
        QPoint centerViewLeft = QPoint(rectViewport.left(), centerView.y());
        QPoint centerViewRight = QPoint(rectViewport.right(), centerView.y());

        QPointF centerSceneTop = viewGraphics->mapToScene(centerViewTop);
        QPointF centerSceneBottom = viewGraphics->mapToScene(centerViewBottom);
        QPointF centerSceneLeft = viewGraphics->mapToScene(centerViewLeft);
        QPointF centerSceneRight = viewGraphics->mapToScene(centerViewRight);

        QVector3D centerGeoTop = cs->toGeoDegree( cs->toModel( centerSceneTop ) );
        QVector3D centerGeoBottom = cs->toGeoDegree( cs->toModel( centerSceneBottom ) );
        QVector3D centerGeoLeft = cs->toGeoDegree( cs->toModel( centerSceneLeft ) );
        QVector3D centerGeoRight = cs->toGeoDegree( cs->toModel( centerSceneRight ) );

        qreal latTop = centerGeoTop.x();
        latTop = latTop > 90.0 ? 90.0 : (latTop < -90.0 ? -90.0 : latTop);
        qreal latBottom = centerGeoBottom.x();
        latBottom = latBottom > 90.0 ? 90.0 : (latBottom < -90.0 ? -90.0 : latBottom);
        qreal lonLeft = centerGeoLeft.y();
        lonLeft = lonLeft > 360.0 ? 360.0 : (lonLeft < -360.0 ? -360.0 : lonLeft);
        qreal lonRight = centerGeoRight.y();
        lonRight = lonRight > 360.0 ? 360.0 : (lonRight < -360.0 ? -360.0 : lonRight);

        latTop = qRound(latTop);
        latBottom = qRound(latBottom);
        lonLeft = qRound(lonLeft);
        lonRight = qRound(lonRight);

        m_latLineNumber = qAbs(latBottom-latTop)+2;
        m_lonLineNumber = qAbs(lonRight-lonLeft)+2;

        const int minNumberOfLines = 4;
        // foolproof
        m_latLineNumber = m_latLineNumber < minNumberOfLines ? minNumberOfLines : m_latLineNumber;
        m_lonLineNumber = m_lonLineNumber < minNumberOfLines ? minNumberOfLines : m_lonLineNumber;
    }
}

void GUIGridGeodesic::UpdateLabelPosition()
{
    if ( !scene() || scene()->views().size() < 1 || m_points.size() < 1 )
        return;

    prepareGeometryChange();

    int latLineNumber = m_labelBox.width();
    int lonLineNumber = m_labelBox.height();

    int latS = m_labelBox.x(),
        latE = m_labelBox.x()+latLineNumber,
        lonS = m_labelBox.y(),
        lonE = m_labelBox.y()+lonLineNumber;


    QGraphicsView *view = scene()->views().at(0);
    Q_ASSERT(view);

    QRect rectViewport = view->viewport()->rect();
    QPoint centerView = view->mapFromScene(QPointF(0.,0.));

    if ( m_posLat.size() != 2*latLineNumber )
        m_posLat.resize(2*latLineNumber);
    // Cache latitude positions.
    for ( int iLat = latS, i = 0; iLat < latE; iLat++, i++ )
    {
        // Cache points on the left longitude.
        for ( int iLon = lonS; iLon < lonE; iLon++ )
        {
            QPointF pointScene = m_points[(iLat-latS)*lonLineNumber+(iLon-lonS)];
            if ( rectViewport.contains( centerView + pointScene.toPoint() ) )
            {
                m_posLat[i] = pointScene;
                break;
            }
        }
        i++;
        // Cache points on the right longitude.
        for ( int iLon = lonE-1; iLon >= lonS; iLon-- )
        {
            QPointF pointScene = m_points[(iLat-latS)*lonLineNumber+(iLon-lonS)];
            if ( rectViewport.contains( centerView + pointScene.toPoint() ) )
            {
                m_posLat[i] = pointScene;
                break;
            }
        }
    }

    if ( m_posLon.size() != 2*lonLineNumber )
        m_posLon.resize(2*lonLineNumber);
    // Cache longitude positions.
    for ( int iLon = lonS, i = 0; iLon < lonE; iLon++, i++ )
    {
        // Cache points on the top latitude.
        for ( int iLat = latE-1; iLat >= latS; iLat-- )
        {
            QPointF pointScene = m_points[(iLat-latS)*lonLineNumber+(iLon-lonS)];
            if ( rectViewport.contains( centerView + pointScene.toPoint() ) )
            {
                m_posLon[i] = pointScene;
                break;
            }
        }
        i++;
        // Cache points on the bottom latitude.
        for ( int iLat = latS; iLat < latE; iLat++ )
        {
            QPointF pointScene = m_points[(iLat-latS)*lonLineNumber+(iLon-lonS)];
            if ( rectViewport.contains( centerView + pointScene.toPoint() ) )
            {
                m_posLon[i] = pointScene;
                break;
            }
        }
    }

    update();
}

}   // namespace geoGE
