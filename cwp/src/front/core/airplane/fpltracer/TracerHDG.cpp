#include "TracerHDG.h"
#include <QPainter>
#include <QGraphicsScene>
#include "front/core/GUILineText.h"
#include "front/core/GUIAirplane.h"

#include "main.h"

CTracerHDG::CTracerHDG(const QSharedPointer<CAirplane>& pPlane, QGraphicsItem* parent)
    : QGraphicsObject(parent)
    , m_pLine1(0), m_pLine2(0), m_pAirplane(pPlane)
{
    setZValue(gui::Ztracerhdg);
    m_pLine1 = new CGUILineText(0, this);
    m_pLine1->SetColor(Qt::yellow);
    m_pLine1->SetPenWidth(1);
    m_pLine1->SetUseTextShape(true);
    m_pLine1->SetPoints(m_pAirplane, m_pAirplane, CGUILineText::NonePoints);

    m_pLine2 = new CGUILineText(0, this);
    m_pLine2->SetColor(Qt::yellow);
    m_pLine2->SetPenWidth(1);
    m_pLine2->SetUseTextShape(true);
    m_pLine2->SetPoints(m_pAirplane, m_pAirplane, CGUILineText::RightPoint);
}

CTracerHDG::~CTracerHDG()
{
    if (scene())
        scene()->removeItem(m_pLine1);
    delete m_pLine1;
    if (scene())
        scene()->removeItem(m_pLine2);
    delete m_pLine2;
}

QRectF CTracerHDG::boundingRect() const
{
    return QRectF();
}

QPainterPath CTracerHDG::shape() const
{
    return QPainterPath();
}

void CTracerHDG::paint(
        QPainter *painter,
        const QStyleOptionGraphicsItem *option,
        QWidget *widget)
{
}

void CTracerHDG::SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord& centralCoord)
{
    m_pLine1->SetPosition(qllXMin, qllYMax, dScale, centralCoord);
    m_pLine2->SetPosition(qllXMin, qllYMax, dScale, centralCoord);
}

void CTracerHDG::Update()
{
    UpdateLines();
}

void CTracerHDG::Update(CCoord coord)
{
    m_pLine1->SetPoint2(coord, CGUILineText::NonePoints);
    m_pLine2->SetPoints(coord, coord, CGUILineText::RightPoint);
    UpdateLines();
}

void CTracerHDG::UpdateLines()
{
    const QLineF azim = m_pLine1->GetLine();
    const int iAzimuth = int((360-azim.angle())+90)%360;
    QString sText = QString::number(iAzimuth) + UTF8_QSTRING("\u00B0");

    CCoord coord2 = m_pLine2->GetCoordPoint1();
    if (m_pAirplane)
    {
        QSharedPointer<CAirplane> pAirplane = m_pAirplane.toStrongRef();
        if (pAirplane && pAirplane->GetSFPL() && pAirplane->GetSFPL()->GetFPLFrag())
        {
            CGUIAirplane* pGUIPlane = dynamic_cast<CGUIAirplane*>(pAirplane.data());
            if (pGUIPlane)
                pGUIPlane->SetAzimuth2(iAzimuth);
            QSharedPointer<CPoint> pPoint;
            CProcFrag::TPointFragList listPoints = pAirplane->GetSFPL()->GetFPLFrag()->GetNotPassedPoint();
            if (!listPoints.empty())
                pPoint = listPoints[0]->GetPoint();
            for (int i = 0; i < listPoints.size(); ++i)
            {
                m_pLine2->SetPoint2(listPoints[i]->GetPoint()->GetCoord(), CGUILineText::RightPoint);
                QLineF line2 = m_pLine2->GetLine();
                qreal dAngle = azim.angleTo(QLineF(line2.p2(), line2.p1()));
                if (90 < dAngle && 270 > dAngle)
                {
                    pPoint = listPoints[i]->GetPoint();
                    break;
                }
            }
            if (pPoint)
                coord2 = pPoint->GetCoord();
        }
        if (pAirplane)
        {
            CGUIAirplane* pGUIPlane = dynamic_cast<CGUIAirplane*>(pAirplane.data());
            if (pGUIPlane)
                pGUIPlane->SetAzimuth2(iAzimuth);
            int iPlaneAzimuth = pAirplane->GetAzimuth();
            int iLAzimuth(0), iRAzimuth(0);
            if (iPlaneAzimuth < iAzimuth)
            {
                iRAzimuth = iAzimuth - iPlaneAzimuth;
                iLAzimuth = 360 - iRAzimuth;
            }
            else
            {
                iLAzimuth = iPlaneAzimuth - iAzimuth;
                iRAzimuth = 360 - iLAzimuth;
            }
            sText += QString("(L%1%2R%3%4)").arg(iLAzimuth).arg(UTF8_QSTRING("\u00B0")).arg(iRAzimuth).arg(UTF8_QSTRING("\u00B0"));
        }
    }
    m_pLine1->SetText(sText);
    m_pLine2->SetPoint2(coord2, CGUILineText::RightPoint);
}
