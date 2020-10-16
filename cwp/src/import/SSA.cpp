#include "SSA.h"

#include "Calculation.h"
#include "RouteFragmentation.h"

#if defined(_MSC_VER) && QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#define _USE_MATH_DEFINES
#endif
#include <math.h>

void CSSAPoint::SetType(const QString& sType)
{
    QString sUpperType = sType.toUpper();
    if (sUpperType == "IF")
        SetType(enIF);
    else if (sUpperType == "TF")
        SetType(enTF);
    else if (sUpperType == "CF")
        SetType(enCF);
    else if (sUpperType == "DF")
        SetType(enDF);
    else if (sUpperType == "CA")
        SetType(enCA);
    else if (sUpperType == "CI")
        SetType(enCI);
    else
        SetType(enUN);
}

void CSSAPoint::SetTurn(const char cTurn)
{
    if (cTurn == enLeft)
        SetTurn(enLeft);
    else if (cTurn == enRight)
        SetTurn(enRight);
    else
        SetTurn(enUnknown);
}

void CSSA::AddPoint(const CSSA::TPointPtr& pPoint)
{
    m_Points[pPoint->GetSN()] = pPoint;
}

void CSSA::CalculatePath(qint64 qllXMin, qint64 qllYMax, double dScale,
                         bool bRoute, bool bAddRunwayPoint)
{
    m_bRoute = bRoute;
    m_dScale = dScale;
    m_Path = QPainterPath();
    m_pRoute = QSharedPointer<CProcFrag> (new CProcFrag());
    m_pRoute->SetType((GetType() == enSID) ? (CProcFrag::enSID)
        : (CProcFrag::enSTAR));
    m_pRoute->SetName(GetName());
    m_iSN = 1;
    m_dAlt = 0;
    TVA_CATCHALL_TRY
    {
        QPointF RunwayPnt((GetParent()->GetPlace().GetX() - qllXMin) * dScale,
                          (qllYMax - GetParent()->GetPlace().GetY()) * dScale);
        TPointPtr pRunwayPoint(new CSSAPoint);
        pRunwayPoint->SetAlt1(0);
        pRunwayPoint->SetSpeedLim(50);

        bool bFirst(true);
        if ( GetType() == enSID ) {
            //pRunwayPoint->SetAlt1(GetParent()->GetRunwayAlt());
            pRunwayPoint->SetAlt1(0);
            m_Path.moveTo(RunwayPnt);
            _AddPoint(RunwayPnt, pRunwayPoint, false);
            QLineF RunwayLine;
            RunwayLine.setP1(RunwayPnt);
            RunwayLine.setAngle(AzimuthToAngle(GetParent()->GetDirection() * M_PI
                                               / 180) * 180 / M_PI);
            RunwayLine.setLength(qBound(0.0, GetParent()->GetRunwayLength(), 500.0) * m_dScale);
            _AddLine(RunwayLine.p2(), pRunwayPoint, false);
            bFirst = false;
        }
        TPoints::const_iterator itScan = GetPoints().begin();
        for (; itScan != GetPoints().end(); ++itScan) {
//            if (itScan == GetPoints().begin() && GetType() == enSTAR)
//                (*itScan)->SetAlt1(GetTransitAlt()); Pasha

            QPointF pnt;
            QPointF pntNext;
            if (!(*itScan)->GetPoint().isNull()) {
                pnt.setX(((*itScan)->GetPoint()->GetCoord().GetX() - qllXMin)
                         * dScale);
                pnt.setY((qllYMax - (*itScan)->GetPoint()->GetCoord().GetY())
                         * dScale);
            }
            if ((itScan + 1) != GetPoints().end()
                && !(*(itScan + 1))->GetPoint().isNull()) {
                pntNext.setX(((*(itScan + 1))->GetPoint()->GetCoord().GetX()
                              - qllXMin) * dScale);
                pntNext.setY((qllYMax
                              - (*(itScan + 1))->GetPoint()->GetCoord().GetY()) * dScale);
            }
            if (bFirst && !pnt.isNull()) {
                m_Path.moveTo(pnt);
                bFirst = false;
            }

            if ((*itScan)->GetType() == CSSAPoint::enIF)
                _CalcTF(*itScan, pnt);
            else if ((*itScan)->GetType() == CSSAPoint::enTF)
                _CalcTF(*itScan, pnt);
            else if ((*itScan)->GetType() == CSSAPoint::enCF)
                _CalcCF(*itScan, pnt);
            else if ((*itScan)->GetType() == CSSAPoint::enDF)
                _CalcDF(*itScan, pnt);
            else if ((*itScan)->GetType() == CSSAPoint::enCA)
                _CalcCA(*itScan, pnt);
            else if ((*itScan)->GetType() == CSSAPoint::enCI && itScan + 1
                     != GetPoints().end())
                _CalcCI(*itScan, *(itScan + 1), pntNext);
            else {
                TVA_THROW(QObject::tr("Unknow leg"), E_FAIL);
            }
        }
        if ( GetType() == enSTAR )
        {
            if ( bAddRunwayPoint )
                _AddLine(RunwayPnt, pRunwayPoint, false);
            else
                _AddPoint(RunwayPnt, pRunwayPoint, false);
            if(m_bRoute && GetRoute())
            {
                GetRoute()->GetSourcePointFragList().last()->SetLevel(GetParent()->GetRunwayAlt());
            }
        }

    }TVA_CATCHALL(err){
		Q_UNUSED(err)
    m_Path = QPainterPath();
    throw;
}
}

void CSSA::_CalcTF(const TPointPtr& pPoint, const QPointF& pnt)
{
    TVA_CHECK(!pnt.isNull(), QObject::tr("Leg TF has no point"), E_FAIL);
    _AddLine(pnt, pPoint, true);
}

void CSSA::_CalcCA(const TPointPtr& pPoint, const QPointF& pnt)
{
    TVA_CHECK(pPoint->IsAlt1(), QObject::tr("Leg CA has no altitude 1"), E_FAIL);
    TVA_CHECK(pPoint->IsCourse(), QObject::tr("Leg CA has no course"), E_FAIL);
    double dLength = (pPoint->GetAlt1() - m_dAlt) / (500 * 0.3048 / 1852);
    m_dAlt = pPoint->GetAlt1();
    dLength *= m_dScale;    
    double dAngle = AzimuthToAngle( (pPoint->GetCourse() + GetParent()->GetDeclination()) * M_PI / 180);
    QLineF CALine;
    CALine.setP1(m_Path.currentPosition());
    CALine.setAngle(dAngle * 180 / M_PI);
    CALine.setLength(dLength);
    _AddLine(CALine.p2(), pPoint, true);

}

void CSSA::_CalcCF(const TPointPtr& pPoint, const QPointF& pnt)
{
    TVA_CHECK(!pnt.isNull(), QObject::tr("Leg CF has no point"), E_FAIL);
    TVA_CHECK(pPoint->IsCourse(), QObject::tr("Leg CF has no course"), E_FAIL);
    if (m_Path.isEmpty()) {
        _CalcTF(pPoint, pnt);
        return;
    }

    QLineF Line1;
    Line1.setP1(m_Path.currentPosition());
    Line1.setAngle(m_Path.angleAtPercent(1) + GetParent()->GetDeclination());
    Line1.setLength(100);
    QLineF Line2;
    Line2.setP1(pnt);
    Line2.setAngle(AzimuthToAngle( (pPoint->GetCourse() + GetParent()->GetDeclination()) * M_PI / 180) * 180 / M_PI);
    Line2.setLength(100);

    double dOriginAngle = Line1.angleTo(Line2) * M_PI / 180;
    double dAngle(dOriginAngle);
    if (dAngle > M_PI)
        dAngle = 2 * M_PI - dAngle;
    bool bRight(avia::IsRightTurn(Line1, Line2.p1()));
    bool bCircleRight(pPoint->GetTurn() != CSSAPoint::enLeft
                      && (pPoint->GetTurn() == CSSAPoint::enRight || bRight));

    double dSin = sin(dAngle);
    if (dSin > 2 * M_PI / 180 || dSin < -2 * M_PI / 180) {
        dAngle = M_PI - dAngle;
        dAngle /= 2;

        QPointF Intersect;
        Line1.intersect(Line2, &Intersect);

        // calculate intersaction vectors
        QLineF LineTemp1(Line1.p1(), Intersect);
        QLineF LineTemp2(Line2.p1(), Intersect);

        double dTempAngle1(Line1.angleTo(LineTemp1));
        bool bBigCircle(dTempAngle1 > 5 && dTempAngle1 < 355);

        // GOVI4A
        if (!bBigCircle && (bRight != bCircleRight)) {
            //LineTemp1.setLength(LineTemp1.length() + 1000 * m_dScale);
            LineTemp1.setLength(LineTemp1.length() + TURN_MINRADIUS/2 * m_dScale);
            _AddLine(LineTemp1.p2(), pPoint, false);
            _CalcCF(pPoint, pnt);
            return;
        }

        // radius vectors
        double dAngleTan = tan(dAngle);
        // !!! 90 degree
        if (dAngleTan > 11) {
            _CalcTF(pPoint, pnt);
            return;
        }

        double dR = LineTemp1.length() * dAngleTan;
        if (dR < 0)
            dR *= -1;


        QLineF RadLine1 = Line1.normalVector();
        if (bCircleRight) {
            RadLine1 = RadLine1.normalVector();
            RadLine1 = RadLine1.normalVector();
        }
        RadLine1.setLength(dR);
        RadLine1.setPoints(RadLine1.p2(), RadLine1.p1());

        QLineF RadLine2;
        RadLine2.setP1(RadLine1.p1());
        RadLine2.setLength(dR);
        RadLine2.setAngle(RadLine1.angle() + (M_PI - 2 * dAngle) * 180 / M_PI);
        qreal rErrorLength = QLineF(RadLine2.p2(), Intersect).length()
                             - LineTemp1.length();
        if (rErrorLength > 1 || rErrorLength < -1)
            RadLine2.setAngle(RadLine1.angle() - (M_PI - 2 * dAngle) * 180 / M_PI);

        LineTemp2.setPoints(RadLine2.p2(), Intersect);
        QLineF Line2Exit(LineTemp2.p1(), Line2.p1());
        double dTempAngle2 = LineTemp2.angleTo(Line2Exit);
        double dTempAngle3 = LineTemp2.angleTo(Line2);
        bool bInverse2 = (dTempAngle2 < 355 && dTempAngle2 > 5);
        bool bPointInverse2 = (dTempAngle3 < 355 && dTempAngle3 > 5);

        // Because of GOVI4A && KOLO4A
        if (!((!bBigCircle && bInverse2 && bPointInverse2) || (bBigCircle
                                                               && !bInverse2 && !bPointInverse2))) {
            _CalcDF(pPoint, pnt);
            return;
        }

        // have to fly direct a little
        LineTemp2.setPoints(Line2.p1(), Intersect);
        // Because of GOVI4A
        if ((!bBigCircle && LineTemp2.length() < LineTemp1.length())) {
            qreal rLength = LineTemp1.length() - LineTemp2.length();
            if (rLength < 0)
                rLength *= -1;
            Line1.setLength(rLength + 1);
            _AddLine(Line1.p2(), pPoint, false);
            _CalcCF(pPoint, pnt);
            return;
        }

        QRectF Rect;
        Rect.setTopLeft(RadLine1.p1());
        Rect.setWidth(2 * dR);
        Rect.setHeight(2 * dR);
        Rect.translate(-dR, -dR);

        qreal rAngle1 = RadLine1.angle();
        qreal rAngle2 = RadLine1.angleTo(RadLine2);
        if (bBigCircle) {
            if (rAngle2 < 180) {
                rAngle2 = 360 - rAngle2;
            }
        } else {
            if (rAngle2 > 180) {
                rAngle2 = 360 - rAngle2;
            }
        }
        if (bCircleRight)
            rAngle2 *= -1;
        _AddArc(Rect, rAngle1, rAngle2, pPoint);

        _AddLine(pnt, pPoint, true);

    } else {
        if (dOriginAngle < 2 * M_PI / 180 || dOriginAngle > 358 * M_PI / 180) {
            _CalcDF(pPoint, pnt);
            return;
        }
        QLineF RadLine1 = Line1.normalVector();
        QPointF NormInter;
        RadLine1.intersect(Line2, &NormInter);
        double dR = pow(pow(NormInter.x() - Line1.p1().x(), 2) + pow(NormInter.y()
                                                                     - Line1.p1().y(), 2), 0.5) / 2;
        RadLine1.setP2(NormInter);
        RadLine1.setLength(dR);
        QRectF Rect;
        Rect.setTopLeft(RadLine1.p2());
        Rect.setWidth(2 * dR);
        Rect.setHeight(2 * dR);
        Rect.translate(-dR, -dR);
        RadLine1.setPoints(RadLine1.p2(), RadLine1.p1());
        qreal rAngle1 = RadLine1.angle();
        qreal rAngle2 = 180;
        if (pPoint->GetTurn() != CSSAPoint::enLeft && (pPoint->GetTurn()
            == CSSAPoint::enRight || bRight))
            rAngle2 = -180;
        _AddArc(Rect, rAngle1, rAngle2, pPoint);
        _AddLine(pnt, pPoint, true);
    }
}

void CSSA::_CalcDF(const TPointPtr& pPoint, const QPointF& pnt)
{
    TVA_CHECK(!pnt.isNull(), QObject::tr("Leg DF has no point"), E_FAIL);
    if (m_Path.isEmpty()) {
        _CalcTF(pPoint, pnt);
        return;
    }
    //double dR = 3800 * m_dScale;
    double dR = TURN_MINRADIUS * m_dScale;
    QLineF Line1;
    Line1.setP1(m_Path.currentPosition());
    Line1.setAngle(m_Path.angleAtPercent(1));
    Line1.setLength(100);
    QLineF Line2;
    Line2.setP1(m_Path.currentPosition());
    Line2.setP2(pnt);

    double dAngle = Line1.angleTo(Line2);
    if (dAngle < 5 || dAngle > 355) {
        _CalcTF(pPoint, pnt);
        return;
    }
    bool bRight(avia::IsRightTurn(Line1, Line2.p2()));
    bool bCircleRight(pPoint->GetTurn() != CSSAPoint::enLeft
                      && (pPoint->GetTurn() == CSSAPoint::enRight || bRight));

    QLineF NormLine1 = Line1.normalVector();
    if (pPoint->GetTurn() != CSSAPoint::enLeft && (pPoint->GetTurn()
        == CSSAPoint::enRight || bRight)) {
        NormLine1 = NormLine1.normalVector();
        NormLine1 = NormLine1.normalVector();
    }

    NormLine1.setLength(dR);
    QLineF CentrLine(NormLine1.p2(), pnt);
    bool bSkipLine(false);
    if (dR > CentrLine.length()) {
        dR = CentrLine.length();
        NormLine1.setLength(dR);
        bSkipLine = true;
    }

    QRectF Rect;
    Rect.setTopLeft(NormLine1.p2());
    Rect.setWidth(2 * dR);
    Rect.setHeight(2 * dR);
    Rect.translate(-dR, -dR);

    NormLine1.setPoints(NormLine1.p2(), NormLine1.p1());
    qreal rAngle1 = NormLine1.angle();

    double dTan(0);
    if (CentrLine.length() - dR > 0) {
        double dLeg = pow(pow(CentrLine.length(), 2) - pow(dR, 2), 0.5);
        dTan = atan2(dR, dLeg);
    } else
        dTan = M_PI_2;

    if (bCircleRight)
        dTan *= -1;
    CentrLine.setAngle(CentrLine.angle() + dTan * 180 / M_PI);

    qreal rAngle2 = Line1.angleTo(CentrLine);

    if (bCircleRight)
        rAngle2 = rAngle2 - 360;

    _AddArc(Rect, rAngle1, rAngle2, pPoint);
    if (!bSkipLine)
        _AddLine(pnt, pPoint, true);
}

void CSSA::_CalcCI(const TPointPtr& pPoint, const TPointPtr& pPointNext,
                   const QPointF& pnt)
{
    TVA_CHECK(!pnt.isNull(), QObject::tr("Leg CI has no next point"), E_FAIL);
    TVA_CHECK(pPoint->IsCourse(), QObject::tr("Leg CI has no course"), E_FAIL);
    TVA_CHECK(pPointNext->IsCourse(), QObject::tr("Leg CI has no next course"), E_FAIL);

    QLineF line1;

    if (!m_Path.isEmpty()) {
        _CalcAzimuth(pPoint);
        line1.setP1(m_Path.currentPosition());
        line1.setLength(100);
        line1.setAngle(m_Path.angleAtPercent(1) + GetParent()->GetDeclination());
    } else {
        line1.setP1(m_Path.currentPosition());
        line1.setLength(100);
        line1.setAngle(AzimuthToAngle( (pPoint->GetCourse() + GetParent()->GetDeclination()) * M_PI / 180) * 180
                       / M_PI);
    }
    QLineF line2;
    line2.setP1(pnt);
    line2.setLength(100);    
    line2.setAngle(AzimuthToAngle( pPointNext->GetCourse() * M_PI / 180) * 180
                   / M_PI);
    QPointF LineIntersect;
    TVA_CHECK(
            line2.intersect(line1, &LineIntersect) != QLineF::NoIntersection,
            QObject::tr("CI leg does not intersect the next leg"),
            E_FAIL);
    QLineF line3(line1.p1(), LineIntersect);

    double dAngle = line1.angleTo(line3);

    if (dAngle < 5 || dAngle > 355) {
        _AddLine(LineIntersect, pPoint, false);
    }
}

void CSSA::_CalcAzimuth(const TPointPtr& pPoint)
{
    TVA_CHECK(pPoint->IsCourse(), QObject::tr("Leg has no course"), E_FAIL);
    if (m_Path.isEmpty()) {
        return;
    }

    QLineF Line1;
    Line1.setP1(m_Path.currentPosition());
    Line1.setAngle(m_Path.angleAtPercent(1) + GetParent()->GetDeclination());
    Line1.setLength(100);

    bool bRight(avia::GetTurningOrientation(AngleToAzimuth(Line1.angle() * M_PI
                                                           / 180), pPoint->GetCourse() * M_PI / 180) == 1);
    bool bBigCircle((pPoint->GetTurn() == CSSAPoint::enLeft && bRight)
                    || (pPoint->GetTurn() == CSSAPoint::enRight && !bRight));
    bool bCircleRight(pPoint->GetTurn() != CSSAPoint::enLeft
                      && (pPoint->GetTurn() == CSSAPoint::enRight || bRight));

    //double dR = 3800 * m_dScale;
    double dR = TURN_MINRADIUS * m_dScale;

    QLineF RadLine1 = Line1.normalVector();
    if (bCircleRight) {
        RadLine1 = RadLine1.normalVector();
        RadLine1 = RadLine1.normalVector();
    }
    RadLine1.setLength(dR);
    RadLine1.setPoints(RadLine1.p2(), RadLine1.p1());

    QRectF Rect;
    Rect.setTopLeft(RadLine1.p1());
    Rect.setWidth(2 * dR);
    Rect.setHeight(2 * dR);
    Rect.translate(-dR, -dR);

    qreal rAngle1 = RadLine1.angle();
    qreal rAngle2 = AzimuthToAngle(pPoint->GetCourse() * M_PI / 180) * 180 / M_PI;
    if (bCircleRight)
        rAngle2 += 90;
    else
        rAngle2 -= 90;
    rAngle2 = fmod(rAngle2 - rAngle1, 360);
    if (rAngle2 < 0)
        rAngle2 += 360;
    if (bBigCircle) {
        if (rAngle2 < 180) {
            rAngle2 = 360 - rAngle2;
        }
    } else {
        if (rAngle2 > 180) {
            rAngle2 = 360 - rAngle2;
        }
    }
    if (bCircleRight)
        rAngle2 *= -1;
    if (rAngle2 > 1 || rAngle2 < -1)
        _AddArc(Rect, rAngle1, rAngle2, pPoint);
}

void CSSA::_AddArc(const QRectF& Rect, qreal rAngle1, qreal rAngle2,
                   const TPointPtr& pPoint)
{
    if (m_bRoute) {
        QPainterPath TempPath;
        TempPath.moveTo(m_Path.currentPosition());
        TempPath.arcTo(Rect, rAngle1, rAngle2);
        int i = 0;
        for (; i < 9; ++i) {
            QPointF TempPoint = TempPath.pointAtPercent((double) i / 8);
            _AddPoint(TempPoint, pPoint, false);
        }
        m_Path.addPath(TempPath);
    } else {
        m_Path.arcTo(Rect, rAngle1, rAngle2);
    }
}

void CSSA::_AddLine(const QPointF& pnt, const TPointPtr& pPoint, bool bInclude)
{
    QPointF pntTemp(pnt);
    if(pPoint && (pPoint->GetFlyOver() & 2))
    {
        QLineF line(m_Path.currentPosition(), pnt);
        qreal len = line.length();
        if(len > TURN_MINRADIUS * m_dScale)
        {
            line.setLength(len - TURN_MINRADIUS * m_dScale);
            pntTemp = line.p2();
            bInclude = false;
        }
    }
    m_Path.lineTo(pntTemp);
    if (m_bRoute)
        _AddPoint(pntTemp, pPoint, bInclude);
}

void CSSA::_AddPoint(const QPointF& pnt, const TPointPtr& pSSAPoint,
                     bool bInclude)
{
    if (m_bRoute) {
        QSharedPointer<CPoint> pPoint;
        QSharedPointer<CPointFrag> pRoutePoint(new CPointFrag);
        if (bInclude && pSSAPoint && pSSAPoint->GetPoint()) {
            pPoint = pSSAPoint->GetPoint();
        } else {
            pRoutePoint->SetGenerated(true);
            pPoint = QSharedPointer<CPoint> (new CPoint);
            pPoint->SetName(GetName() + QString::number(m_iSN));
            pPoint->GetCoord().SetX(pnt.x());
            pPoint->GetCoord().SetY(pnt.y());
        }

        pRoutePoint->SetPoint(pPoint);

        if (pSSAPoint) {
            if (pSSAPoint->IsAlt1())
            {
                if (IsTransitAlt() && pSSAPoint->GetAlt1() < GetTransitAlt())
                    pRoutePoint->SetLevel(GetParent()->GetRunwayAlt() + pSSAPoint->GetAlt1());
                else
                    pRoutePoint->SetLevel(pSSAPoint->GetAlt1());
            }
            if (pSSAPoint->IsSpeedLim())
                pRoutePoint->SetSpeed(pSSAPoint->GetSpeedLim());
        }
        m_pRoute->AddPointFrag(pRoutePoint);
        ++m_iSN;
    }
}
