#include "GUILineText.h"
#include <QtGui/QPainter>
#include "front/core/GUIAirplane.h"
#include <math.h>
#include <QGraphicsSceneMouseEvent>
#include "main.h"

CGUILineText::CGUILineText(IParent* pParent/* = 0*/, QGraphicsItem* parent/* = 0*/)
    : QGraphicsItem(parent)
    , m_iPenWidth(1), m_color(Qt::yellow)
    , m_font("Courier", 8, QFont::Normal), m_qllXMin(0), m_qllYMax(0), m_dScale(0)
    , m_pParent(pParent), m_showPoints(NonePoints)
    , m_bIsPlane1(false), m_bIsPlane2(false)
    , m_alignment(Qt::AlignHCenter), m_autoText(None)
    , m_timeToForwardSector(0), m_timeToSideSector(120), m_timeToBackwardSector(240)
    , m_bUseTextShape(true), m_Measure(1)
{
    m_font.setStyleStrategy(QFont::PreferAntialias);
    m_font.setPointSize(AppSettings::GetTextSize().GetCurrentTextSize());
    setFlag(ItemIsSelectable, true);

    m_Measure = 1; //1=M, 2=F, 3=FM
    QRegExp rexp("([F,M]{1,1})([F,M]{0,1})");
    if (rexp.exactMatch(networkSettings()->value("users/gauge_SI", "M").toString()))
    {
        if (!rexp.cap(1).isEmpty())
        {
            if ("F" == rexp.cap(1))
                m_Measure = 2;
            else if ("M" == rexp.cap(1))
                m_Measure = 1;

            if (!rexp.cap(2).isEmpty())
                m_Measure = 3;
        }
    }
}

CGUILineText::~CGUILineText()
{
}

void CGUILineText::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    if (event->button() == Qt::RightButton)
        event->accept();
    else
        event->ignore();
}

void CGUILineText::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    if (m_pParent)
        m_pParent->DoubleClickLineText(this, event->scenePos());
}

QRectF CGUILineText::boundingRect() const
{
    QRectF rect(m_transform.mapRect(m_pathLine.boundingRect()));
    rect |= m_textTransform.mapRect(m_pathText.boundingRect());
    if (!rect.isNull())
        rect.adjust(-m_iPenWidth, -m_iPenWidth, m_iPenWidth, m_iPenWidth);
    return rect;
}

QPainterPath CGUILineText::shape() const
{
    QPainterPath path1(m_transform.map(m_pathLine));
    if (m_bUseTextShape)
    {
        QPainterPath path2;
        path2.addRect(m_pathText.boundingRect());
        path1.addPath(m_textTransform.map(path2));
    }
    return path1;
}

void CGUILineText::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if (m_pathLine.isEmpty())
        return;

    painter->save();

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter->setRenderHint(QPainter::HighQualityAntialiasing, true);

    QPen qPen(m_color);
    qPen.setWidth(m_iPenWidth);
    qPen.setCosmetic(true);
    painter->setPen(qPen);
    painter->setBrush(m_color);
    painter->drawPath(m_transform.map(m_pathLine));

    qPen.setWidth(0.5);
    painter->setPen(qPen);
    painter->drawPath(m_textTransform.map(m_pathText));

    painter->restore();
}

void CGUILineText::SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord& centralCoord)
{
    m_qllXMin = qllXMin;
    m_qllYMax = qllYMax;
    m_dScale = dScale;
    m_CentralCoord = centralCoord;
    Recalculate();
}

void CGUILineText::setPositionAndPoints(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord& centralCoord,
                                        const CCoord& p1, const CCoord& p2, ShowPoint showPoint)
{
    m_qllXMin = qllXMin;
    m_qllYMax = qllYMax;
    m_dScale = dScale;
    m_CentralCoord = centralCoord;

    m_Coord1 = p1;
    m_Coord2 = p2;
    m_pPlane1.clear();
    m_pPlane2.clear();
    m_bIsPlane1 = m_bIsPlane2 = false;
    m_showPoints = showPoint;

    Recalculate();
}

void CGUILineText::SetPoints(const CCoord& p1, const CCoord& p2, ShowPoint showPoint/* = AllPoints*/)
{
    m_Coord1 = p1;
    m_Coord2 = p2;
    m_pPlane1.clear();
    m_pPlane2.clear();
    m_bIsPlane1 = m_bIsPlane2 = false;
    m_showPoints = showPoint;
    Recalculate();
}

void CGUILineText::SetPoints(const CCoord& p1, const QSharedPointer<CAirplane>& p2, ShowPoint showPoint/* = AllPoints*/)
{
    m_Coord1 = p1;
    m_Coord2 = CCoord();
    m_pPlane1.clear();
    m_bIsPlane1 = false;
    m_pPlane2 = p2.toWeakRef();
    m_bIsPlane2 = true;
    m_showPoints = showPoint;
    Recalculate();
}

void CGUILineText::SetPoints(const QSharedPointer<CAirplane>& p1, const QSharedPointer<CAirplane>& p2, ShowPoint showPoint/* = AllPoints*/)
{
    m_Coord1 = CCoord();
    m_Coord2 = CCoord();
    m_pPlane1 = p1.toWeakRef();
    m_pPlane2 = p2.toWeakRef();
    m_bIsPlane1 = m_bIsPlane2 = true;
    m_showPoints = showPoint;
    Recalculate();
}

void CGUILineText::SetPoint1(const CCoord& p1, ShowPoint showPoint/* = AllPoints*/)
{
    m_Coord1 = p1;
    m_pPlane1.clear();
    m_bIsPlane1 = false;
    m_showPoints = showPoint;
    Recalculate();
}

void CGUILineText::SetPoint1(const QSharedPointer<CAirplane>& p1, ShowPoint showPoint/* = AllPoints*/)
{
    m_Coord1 = CCoord();
    m_pPlane1 = p1.toWeakRef();
    m_bIsPlane1 = true;
    m_showPoints = showPoint;
    Recalculate();
}

void CGUILineText::SetPoint2(const CCoord& p2, ShowPoint showPoint/* = AllPoints*/)
{
    m_Coord2 = p2;
    m_pPlane2.clear();
    m_bIsPlane2 = false;
    m_showPoints = showPoint;
    Recalculate();
}

void CGUILineText::SetPoint2(const QSharedPointer<CAirplane>& p2, ShowPoint showPoint/* = AllPoints*/)
{
    m_Coord2 = CCoord();
    m_pPlane2 = p2.toWeakRef();
    m_bIsPlane2 = true;
    m_showPoints = showPoint;
    Recalculate();
}

void CGUILineText::SetText(const QString &sText, Qt::Alignment alignment/* = Qt::AlignHCenter*/)
{
    m_sText = sText;
    m_alignment = alignment;
    Recalculate();
}

void CGUILineText::getPoint(const CCoord &coord, bool is_plane, const QWeakPointer<CAirplane> &plane, QPointF &point)
{
    if (!coord.IsNull())
        point = GetPoint(coord);
    if (is_plane) {
        if (plane) {
            QSharedPointer<CAirplane> pPlane = plane.toStrongRef();
            if (pPlane) {
                CGUIAirplane* pGUIPlane = dynamic_cast<CGUIAirplane*>(pPlane.data());
                if (pGUIPlane && pGUIPlane->isVisible()) point = mapFromScene(pGUIPlane->scenePos());
                else throw(555);
            } else throw(555);
        } else throw(555);
    }
}

void CGUILineText::Recalculate()
{
    prepareGeometryChange();

    const QRectF dirtRect(boundingRect());
    m_point1 = m_point2 = QPointF();
    m_point1Orig = m_point2Orig = QPointF();
    m_pathLine = m_pathText = QPainterPath();
    m_transform = QTransform();
    m_textTransform = QTransform();

    try
    {
        getPoint(m_Coord1, m_bIsPlane1, m_pPlane1, m_point1);

        getPoint(m_Coord2, m_bIsPlane2, m_pPlane2, m_point2);

        m_point1Orig = m_point1;
        m_point2Orig = m_point2;

        if (m_point1 == m_point2)
            throw(555);
    }
    catch (int)
    {
        update(dirtRect);
        return;
    }

    AddAutoText(m_sText, m_sTimeText);

    Qt::Alignment alignmentTemp = m_alignment;
    ShowPoint showPointsTemp = m_showPoints;
    // text upside down, if you do not need a commentary this
    if (m_point2.x() < m_point1.x())
    {
        QPointF point(m_point1);
        m_point1 = m_point2;
        m_point2 = point;

        if (m_alignment == Qt::AlignLeft)
            alignmentTemp = Qt::AlignRight;
        else if (m_alignment == Qt::AlignRight)
            alignmentTemp = Qt::AlignLeft;

        if (m_showPoints == LeftPoint)
            showPointsTemp = RightPoint;
        else if (m_showPoints == RightPoint)
            showPointsTemp = LeftPoint;
    }
    //-------------------------------------------------------

    QLineF line(m_point1, m_point2);
    qreal len = line.length();
    qreal angle = line.angle();
    line = QLineF(QPoint(0,0), QPointF(10,0));
    line.setLength(len);

    m_pathLine.moveTo(line.p1());
    m_pathLine.lineTo(line.p2());

    if(showPointsTemp & LeftPoint)
        m_pathLine.addEllipse(line.p1().x()-1.5, line.p1().y()-1.5, 3, 3);
    if(showPointsTemp & RightPoint)
        m_pathLine.addEllipse(line.p2().x()-1.5, line.p2().y()-1.5, 3, 3);

    QFontMetricsF metrics(m_font);

    QPointF pointText;

    // Add distance and angle text to painter path.
    QRectF rectText = metrics.boundingRect(m_sText);
    bool bText;
    bool bVert;
    if(m_autoText == DistanceWithMileAndAngle || m_autoText == DistanceWithMileAngleTime)
    {
        //not use, but for future
        bText = true;
        bVert = false;
    }
    else
    {
        bText = (len > rectText.width() || len > rectText.height()) && (!m_sText.isEmpty());
        bVert = (len <= rectText.width());
    }
    if(m_autoText == DistanceWithMileAndAngle || m_autoText == DistanceWithMileAngleTime)
    {
        m_pathText.addText(QPointF(0,0), m_font, m_sText);
    }
    else
    {
        if (bText)
        {
            if (bVert)
            {
                pointText = QPointF(m_iPenWidth + 2, 0);
                switch (alignmentTemp)
                {
                case Qt::AlignLeft:
                    pointText.ry() = 1;
                    break;
                case Qt::AlignRight:
                    pointText.ry() = line.dy() + rectText.height() / 2 + 1;
                    break;
                case Qt::AlignHCenter:
                    pointText.ry() = line.dy() / 2 + rectText.height() / 4;
                    break;
                default:
                    pointText.ry() = 1;
                    break;
                }
            }
            else
            {
                pointText = QPointF(0, -m_iPenWidth-2);
                switch (alignmentTemp)
                {
                case Qt::AlignLeft:
                    pointText.rx() = 1;
                    break;
                case Qt::AlignRight:
                    pointText.rx() = line.dx() - rectText.width() - 1;
                    break;
                case Qt::AlignHCenter:
                    pointText.rx() = line.dx() / 2 - rectText.width() / 2;
                    break;
                default:
                    pointText.rx() = 1;
                    break;
                }
            }
            m_pathText.addText(pointText, m_font, m_sText);
        }
    }
    // Add absolute time to point text to painter path.
    rectText = metrics.boundingRect(m_sTimeText);
    bText = (len > rectText.width()) && !m_sTimeText.isEmpty();
    if (bText)
    {
        if (bVert)
        {
            pointText = QPointF(2 * m_iPenWidth + 4 + rectText.width(), 0); // Needs to be rechecked
            switch (alignmentTemp)
            {
            case Qt::AlignLeft:
                pointText.ry() = 1;
                break;
            case Qt::AlignRight:
                pointText.ry() = line.dy() + rectText.height() / 2 + 1;
                break;
            case Qt::AlignHCenter:
                pointText.ry() = line.dy() / 2 + rectText.height() / 4;
                break;
            default:
                pointText.ry() = 1;
                break;
            }
        }
        else
        {
            pointText = QPointF(0, m_iPenWidth + 2 + rectText.height() / 2);
            switch (alignmentTemp)
            {
            case Qt::AlignLeft:
                pointText.rx() = 1;
                break;
            case Qt::AlignRight:
                pointText.rx() = line.dx() - rectText.width() - 1;
                break;
            case Qt::AlignHCenter:
                pointText.rx() = line.dx() / 2 - rectText.width() / 2;
                break;
            default:
                pointText.rx() = 1;
                break;
            }
        }
        m_pathText.addText(pointText, m_font, m_sTimeText);
    }

    m_transform.translate(m_point1.x(), m_point1.y());
    m_transform.rotate(360-angle);

    if (bVert)
    {
        m_textTransform.translate((m_point1Orig.x() + m_point2Orig.x()) / 2
                                  , (m_point1Orig.y() + m_point2Orig.y()) / 2);
        m_textTransform.rotate(90 - QLineF(m_point1Orig, m_point2Orig).angle());
    }
    else
    {
        if(m_autoText == DistanceWithMileAndAngle  || m_autoText == DistanceWithMileAngleTime)
        {
            rectText = metrics.boundingRect(m_sText);
            qreal height = rectText.height();
            qreal x1 = m_point1.x();
            qreal y1 = m_point1.y();
            qreal y2 = m_point2.y();
            QLineF line(m_point1, m_point2);
            if(y1 <= y2)
            {
                m_textTransform.translate(x1 + line.dx() / 2 +1, y1 + line.dy() / 2 - 2);
            }
            else
            {
                m_textTransform.translate(x1 + line.dx() / 2 +1, y1 + line.dy() / 2 + height/2 + 2);
            }
            m_textTransform.rotate(360);
        }
        else
        {
            m_textTransform.translate(m_point1.x(), m_point1.y());
            m_textTransform.rotate(360-angle);
        }
    }

    update();
}

QPointF CGUILineText::GetPoint(const CCoord &coord) const
{
    QPointF point;
    if (!coord.IsNull())
    {
        point.setX((coord.GetX() - m_qllXMin) * m_dScale);
        point.setY((m_qllYMax - coord.GetY()) * m_dScale);
    }
    return mapFromScene(point);
}

void CGUILineText::AddAutoText(QString &sText, QString &sTimeText)
{
    if (m_autoText != None)
    {
        sTimeText = "";

        CCoord coord1, coord2;
        if (!m_Coord1.IsNull()) coord1 = m_Coord1;
        if (!m_Coord2.IsNull()) coord2 = m_Coord2;

        QSharedPointer<CAirplane> pPlane1, pPlane2;

        if (m_pPlane1)
        {
            pPlane1 = m_pPlane1.toStrongRef();
            if(pPlane1)
                coord1 = pPlane1->GetCoord();
        }
        if (m_pPlane2)
        {
            pPlane2 = m_pPlane2.toStrongRef();
            if (pPlane2)
                coord2 = pPlane2->GetCoord();
        }

        if(!coord1.IsNull() && !coord2.IsNull())
        {
            // Add distance and angle ...

            double len = 0;
            if(!coord1.IsCenter())
                coord1.Center(m_CentralCoord);
            if(!coord2.IsCenter())
                coord2.Center(m_CentralCoord);
            len = GetDistance(coord1, coord2);
            double lenPure = len;

            bool bFeet = false;
            if(IsSISet(SIFeet))
            {
                if(m_Measure & 2)
                    bFeet = true;
                else
                    bFeet = false;
            }
            else
            {
                if(m_Measure & 1)
                    bFeet = false;
                else
                    bFeet = true;
            }
            if(bFeet) //feet
            {
                len /= 1852; // mile
            }
            else
            {
                len /= 1000; // kilometers
            }
            if(m_autoText == DistanceWithMileAndAngle || m_autoText == DistanceWithMileAngleTime)
            {
                len = bFeet ? len * 1852 : len * 1000;
                sText = QString::number(len/1000, 'f', 1);
                sText += QObject::tr("km");
                sText += " ";
                sText += QString::number(len/1852, 'f', 1);
                sText += QObject::tr("nm");
                sText += " ";
            }
            else
            {
                sText = QString::number(len, 'f', 1);
                sText += bFeet ? QObject::tr("nmi") : QObject::tr("km");
            }
            if(m_autoText == DistanceWithAngle || m_autoText == DistanceWithAngleAndTime || m_autoText == DistanceWithMileAndAngle || m_autoText == DistanceWithMileAngleTime)
            {
                QPointF point1
                        (((coord1.GetX() - m_qllXMin) * m_dScale),
                         (m_qllYMax - coord1.GetY()) * m_dScale);

                QPointF point2
                        (((coord2.GetX() - m_qllXMin) * m_dScale),
                         (m_qllYMax - coord2.GetY()) * m_dScale);
                QLineF line(point1, point2);
                qreal iDeclinationValue = 0;
                bool bDeclinationEnable = ("yes" == networkSettings()->value("users/declination_enable", "no").toString());
                if(bDeclinationEnable)
                {
                    iDeclinationValue = networkSettings()->value("users/declination_value", 0).toDouble();
                    if(iDeclinationValue > 180 || iDeclinationValue < -180)
                        iDeclinationValue = 0;
                }
                int angle = int((360 - (line.angle() + iDeclinationValue)) + 90) % 360;
                sText += QString("(%1\u00B0/%2\u00B0)").arg(angle).arg((angle + 180) % 360);
            }

            if (m_autoText == DistanceWithMileAngleTime)
            {
                QSharedPointer<CAirplane> pPlane;
                if (pPlane1 && !pPlane2)
                    pPlane = pPlane1;
//                if (pPlane2 && !pPlane1)
//                    pPlane = pPlane2;

                if (pPlane)
                {
                    const CVelocity &velocity = pPlane->GetVelocity();

                    QPointF point1
                            (((coord1.GetX() - m_qllXMin) * m_dScale),
                             (m_qllYMax - coord1.GetY()) * m_dScale);

                    QPointF point2
                            (((coord2.GetX() - m_qllXMin) * m_dScale),
                             (m_qllYMax - coord2.GetY()) * m_dScale);

                    QPointF point3
                            (((coord1.GetX() + velocity.GetVx() - m_qllXMin) * m_dScale),
                             (m_qllYMax - coord1.GetY() - velocity.GetVy()) * m_dScale);

                    QLineF line(point1, point2);
                    QLineF lineVel(point1, point3);

                    if (qAbs(line.angle() - lineVel.angle()) <= 15)
                    {
                        int secs = lenPure / velocity.GetV();
                        sText += " " + QTime::currentTime().addSecs(secs).toString("hh:mm");
                    }
                }
            }

            // Add absolute time to reach the point if first coord
            // is plane and second is any point on map.
            if ( (m_autoText == DistanceWithAngleAndTime) && pPlane1 && !pPlane2 )
            {
                const CVelocity &velocity1 = pPlane1->GetVelocity();
                QLineF lineVel(QPointF(0., 0.), QPointF(velocity1.GetVx(), velocity1.GetVy()));
                QLineF linePoint2(QPointF(0., 0.), QPointF(coord2.GetX()-coord1.GetX(), coord2.GetY()-coord1.GetY()));

                double secs = lenPure / lineVel.length();
                qreal angleToPoint2 = fmod( lineVel.angleTo(linePoint2), 360. );
                if ( angleToPoint2 >= 315. || 45. >= angleToPoint2 )
                    secs += m_timeToForwardSector;
                else if ( angleToPoint2 >= 135. && 225. >= angleToPoint2 )
                    secs += m_timeToBackwardSector;
                else
                    secs += m_timeToSideSector;

#ifdef _MSC_VER
#pragma message("FOR TASHKENT")
#else
#warning("FOR TASHKENT")
#endif
                //sTimeText = QTime::currentTime().addSecs(secs).toString("hh:mm:ss");
                sTimeText = QTime::currentTime().addSecs(secs).toString("hh:mm");
            }
        }
    }
}

void CGUILineText::SI()
{
    Recalculate();
}

void CGUILineText::UpdateXFont()
{
    m_font.setPointSize(AppSettings::GetTextSize().GetCurrentTextSize());
    Recalculate();
}

void CGUILineText::Update()
{
    Recalculate();
}

QLineF CGUILineText::GetLine() const
{
    return QLineF(m_point1Orig, m_point2Orig);
}

CCoord CGUILineText::GetCoordPoint1() const
{
    CCoord coord;
    if (!m_Coord1.IsNull())
        coord = m_Coord1;

    if (m_pPlane1) {
        QSharedPointer<CAirplane> pPlane = m_pPlane1.toStrongRef();
        if(pPlane)
            coord = pPlane->GetCoord();
    }
    return coord;
}

CCoord CGUILineText::GetCoordPoint2() const
{
    CCoord coord;
    if (!m_Coord2.IsNull())
        coord = m_Coord2;

    if (m_pPlane2) {
        QSharedPointer<CAirplane> pPlane = m_pPlane2.toStrongRef();
        if (pPlane)
            coord = pPlane->GetCoord();
    }
    return coord;
}

QSharedPointer<CAirplane> CGUILineText::GetPlane1() const
{
    QSharedPointer<CAirplane> pPlane;
    if(m_pPlane1)
    {
        pPlane = m_pPlane1.toStrongRef();
    }
    return pPlane;
}

QSharedPointer<CAirplane> CGUILineText::GetPlane2() const
{
    QSharedPointer<CAirplane> pPlane;
    if (m_pPlane2)
        pPlane = m_pPlane2.toStrongRef();

    return pPlane;
}
