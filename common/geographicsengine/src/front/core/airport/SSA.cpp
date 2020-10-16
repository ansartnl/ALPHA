#include "SSA.h"
#include "SSALegs.h"

#include <QtCore/qmath.h>
#include <QtCore/qnumeric.h>
#include "back/common/Algorithms.h"


//! Geodesy graphics engine
namespace geoGE {

/////////////////////////////////////////////////////////////////////////////////////////////
// SSA implementation.

void SSA::CalculatePath(const TCoordSystem &cs, bool bRoute, qreal angleAzimuthCorrection)
{
    // BERI2A - STAR
    // BERI4A - SID

    // KOSAN3, RUMIK1, OLAG1B, RATIN3
//    if ( GetName() != QL1S("KI2C") )
//        return;
//    if ( GetName() != QString::fromUtf8("ИТИН2Б") )
//        return;
//    if ( GetType() != arinc::SSA::enApproach )
//        return;

    m_angleAzCorrect = angleAzimuthCorrection;
    m_bRoute = bRoute;
    m_Path = QPainterPath();
    m_Route.clear();
    m_iSN = 1;
    m_dAlt = 0;
    try
    {
        const CCoord &coordPlace = GetParent()->GetPlace();
        QPointF RunwayPnt = cs->toScene(QVector3D(coordPlace.GetX(), coordPlace.GetY(), coordPlace.GetZ()));

        // For test purpose - uncomment if you wanna see all SSA point connected
        // with lines.
//        bool bFirstPoint = true;
//        if (GetType() == enSID)
//        {
//            m_Path.moveTo(RunwayPnt);
//            bFirstPoint = false;
//        }
//        TPoints::const_iterator itScan1 = GetPoints().begin();
//        for(; itScan1 != GetPoints().end(); ++itScan1)
//        {
//            if ( (*itScan1)->GetPoint().isNull() )
//                continue;

//            const CCoord &coord = (*itScan1)->GetPoint()->GetCoord();
//            QPointF pnt = cs->toScene(QVector3D(coord.GetX(), coord.GetY(), coord.GetZ()));

//            if (bFirstPoint)
//            {
//                m_Path.moveTo(pnt);
//                bFirstPoint = false;
//            }
//            else
//                m_Path.lineTo(pnt);
//        }
//        m_Path.closeSubpath();
//        return;

//        TPoints::const_iterator itScan1 = GetPoints().begin();
//        for(; itScan1 != GetPoints().end(); ++itScan1)
//        {
//            if ( (*itScan1)->GetPoint().isNull() )
//                continue;

//            const CCoord &coord = (*itScan1)->GetPoint()->GetCoord();
//            QPointF pnt = cs->toScene(QVector3D(coord.GetX(), coord.GetY(), coord.GetZ()));

//            m_Path.addEllipse(pnt, 0.02, 0.02);
//        }
//        return;

        TPointPtr pRunwayPoint(new arinc::SSAPoint);
        pRunwayPoint->SetAlt1(0);
        pRunwayPoint->SetSpeedLim(50);

        bool bFirst(true);
        // SID starts from the Runway. But nicer if SID starts from point close to first
        // Leg point - thus below code is commented.
//        if (GetType() == enSID)
//        {
//            // Length of take off distance is not bigger than 1000 meters.
//            qreal lengthRunway = GetParent()->GetRunwayLength();
//            lengthRunway = lengthRunway > 1000. ? 1000. : lengthRunway;

//            m_Path.moveTo(RunwayPnt);
//            _AddPoint(RunwayPnt, pRunwayPoint, false);
//            QLineF RunwayLine;
//            RunwayLine.setP1(RunwayPnt);
////            RunwayLine.setAngle(algo::azimuthToAngle(
////                    _CorrectAngle(GetParent()->GetDirection()) * M_PI / 180) * 180 / M_PI);
//            qreal angleRunway = algo::azimuthToAngle(GetParent()->GetDirection() * M_PI / 180)
//                                * 180 / M_PI;
//            RunwayLine.setAngle(angleRunway-m_angleAzCorrect);
//            RunwayLine.setLength( cs->distanceToScene( lengthRunway ) );
//            _AddLine(RunwayLine.p2(), pRunwayPoint, false);
//            bFirst = false;
//        }
        TPoints::const_iterator itScan = GetPoints().begin();
        for(; itScan != GetPoints().end(); ++itScan)
        {
            if (itScan ==  GetPoints().begin() && GetType() == enSTAR)
                (*itScan)->SetAlt1(GetTransitAlt());

            QPointF pnt;
            QPointF pntNext;
            if (!(*itScan)->GetPoint().isNull())
            {
                const CCoord &coord = (*itScan)->GetPoint()->GetCoord();
                pnt = cs->toScene(QVector3D(coord.GetX(), coord.GetY(), coord.GetZ()));
            }
            if ((itScan+1) != GetPoints().end() &&
                !(*(itScan+1))->GetPoint().isNull())
            {
                const CCoord &coord = (*(itScan+1))->GetPoint()->GetCoord();
                pntNext = cs->toScene(QVector3D(coord.GetX(), coord.GetY(), coord.GetZ()));
            }
            if (bFirst && !pnt.isNull())
            {
                if (GetType() != enSID)
                    m_Path.moveTo(pnt);
                // For 'Departure' procedure we add very short line to be able to
                // create nice turn to the next SID Leg. If we do not add this
                // trick current algorithm will not be able to make nice turn
                // of first and continue Legs.
                else
                {
                    QLineF line(RunwayPnt, pnt);
                    QLineF unitVec = line.unitVector();

                    QPointF unitPnt = unitVec.p2()-unitVec.p1();
                    QPointF pntDir = line.p2() - unitPnt*line.length()/100000.;
                    m_Path.moveTo(pntDir);
                }
                bFirst = false;
            }

            if ((*itScan)->GetType() == arinc::SSAPoint::enIF)
                _CalcIF(*itScan, pnt);
            else if ((*itScan)->GetType() == arinc::SSAPoint::enTF)
                _CalcTF(*itScan, pnt);
            else if ((*itScan)->GetType() == arinc::SSAPoint::enCF)
                _CalcCF(cs, *itScan, pnt);
            else if ((*itScan)->GetType() == arinc::SSAPoint::enDF)
                _CalcDF(cs, *itScan, pnt);
            else if ((*itScan)->GetType() == arinc::SSAPoint::enCA)
                _CalcCA(cs, *itScan, pnt);
            else if ((*itScan)->GetType() == arinc::SSAPoint::enCI &&
                     itScan+1 != GetPoints().end())
                _CalcCI(cs, *itScan, *(itScan+1), pntNext);
            else
                throw QObject::tr("Unknow leg");
        }
        // According to the 'eAIP Belarus' document STAR does not ends on the Runway.
        // Thus we do not extend line to the runway point.
//        if (GetType() == enSTAR)
//            _AddLine(RunwayPnt, pRunwayPoint, false);

    }
    catch ( QString & )
    {
        m_Path = QPainterPath();
        throw;
    }
    catch ( ... )
    {
        m_Path = QPainterPath();
        throw QObject::tr("Unknown exception");
    }
}

void SSA::_CalcIF(const TPointPtr& pPoint, const QPointF& pnt)
{
    if ( pnt.isNull() )
        throw QObject::tr("Leg IF has no point");
    m_Path.moveTo(pnt);
}

void SSA::_CalcTF(const TPointPtr& pPoint, const QPointF& pnt)
{
    if ( pnt.isNull() )
        throw QObject::tr("Leg TF has no point");
    SsaPainter::drawTF(m_Path, pnt);
}

void SSA::_CalcCA(const TCoordSystem &cs, const TPointPtr& pPoint, const QPointF& pnt)
{
    if ( !pPoint->IsAlt1() ) throw QObject::tr("Leg CA has no altitude 1");
    if ( !pPoint->IsCourse() ) throw QObject::tr("Leg CA has no course");

	double dLength = (pPoint->GetAlt1() - m_dAlt) / (500 * 0.3048 / 1852);
	m_dAlt = pPoint->GetAlt1();
    dLength = cs->distanceToScene(dLength);
    double dAngle = algo::azimuthToAngle(_CorrectAngle(pPoint->GetCourse()) * M_PI / 180);
	QLineF CALine;
	CALine.setP1(m_Path.currentPosition());
	CALine.setAngle(dAngle * 180 / M_PI);
	CALine.setLength(dLength);
	_AddLine(CALine.p2(), pPoint, true);

}

void SSA::_CalcCF(const TCoordSystem &cs, const TPointPtr& pPoint, const QPointF& pnt)
{
    if ( pnt.isNull() ) throw QObject::tr("Leg CF has no point");
    if ( !pPoint->IsCourse() ) throw QObject::tr("Leg CF has no course");

    qreal angle = m_Path.angleAtPercent(1);
    // For the case when path was not initialized yet - draw TF leg.
    if ( m_Path.isEmpty() || qIsNaN(angle) )
    {
        SsaPainter::drawTF(m_Path, pnt);
        return;
    }

    QLineF linePrev;
    linePrev.setP1(m_Path.currentPosition());
    linePrev.setAngle( angle );
    QLineF lineCF;
    lineCF.setP1(pnt);
    qreal angleLeg = fmod(90 - _CorrectAngle(pPoint->GetCourse()), 360.);
    lineCF.setAngle(angleLeg);

    int turnDirection = pPoint->GetTurn() == arinc::SSAPoint::enRight ? 1 :
                        (pPoint->GetTurn() == arinc::SSAPoint::enLeft ? 2 : 0);
    SsaPainter::drawCF(m_Path, linePrev, lineCF, turnDirection, cs->distanceToScene(1500));
}

void SSA::_CalcDF(const TCoordSystem &cs, const TPointPtr& pPoint, const QPointF& pnt)
{
    if ( pnt.isNull() ) throw QObject::tr("Leg DF has no point");
	if (m_Path.isEmpty())
	{
		_CalcTF(pPoint, pnt);
		return;
	}
    double dR = cs->distanceToScene(3800);
    QLineF Line1;
	Line1.setP1(m_Path.currentPosition());
	Line1.setAngle(m_Path.angleAtPercent(1));
    Line1.setLength( cs->distanceToScene(100) );
	QLineF Line2;
	Line2.setP1(m_Path.currentPosition());
	Line2.setP2(pnt);

	double dAngle = Line1.angleTo(Line2);
	if (dAngle < 5 || dAngle >355)
	{
		_CalcTF(pPoint, pnt);
		return;
	}
    bool bRight(algo::isRightSide(Line1, Line2.p2()));
	bool bCircleRight(
        pPoint->GetTurn() != arinc::SSAPoint::enLeft &&
        (pPoint->GetTurn() == arinc::SSAPoint::enRight || bRight));

	QLineF NormLine1 = Line1.normalVector();
    if (pPoint->GetTurn() != arinc::SSAPoint::enLeft &&
        (pPoint->GetTurn() == arinc::SSAPoint::enRight || bRight))
	{
		NormLine1 = NormLine1.normalVector();
		NormLine1 = NormLine1.normalVector();
	}

	NormLine1.setLength(dR);
	QLineF CentrLine(NormLine1.p2(), pnt);
	bool bSkipLine(false);
	if (dR > CentrLine.length())
	{
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
	if (CentrLine.length() - dR > 0)
	{
        double dLeg = pow(pow(CentrLine.length(), 2) -
						  pow(dR, 2), 0.5);
		dTan = atan2(dR, dLeg);
	} else
		dTan = M_PI_2;

	if (bCircleRight)
		dTan *= -1;
	CentrLine.setAngle(CentrLine.angle() + dTan * 180 / M_PI);


	qreal rAngle2 = Line1.angleTo(CentrLine);

	/*if (rAngle2 > 180)
		rAngle2 = 360 - rAngle2;
    if (pPoint->GetTurn() != SSAPoint::enLeft &&
        (pPoint->GetTurn() == SSAPoint::enRight || bRight))
		rAngle2 *= -1;*/
	if (bCircleRight)
		rAngle2 = rAngle2 - 360;

	_AddArc(Rect, rAngle1, rAngle2, pPoint);
	if(!bSkipLine)
		_AddLine(pnt, pPoint, true);
}

void SSA::_CalcCI(
    const TCoordSystem& cs,
	const TPointPtr& pPoint,
	const TPointPtr& pPointNext,
	const QPointF& pnt)
{
    if ( pnt.isNull() ) throw QObject::tr("Leg CI has no next point");
    if ( !pPoint->IsCourse() ) throw QObject::tr("Leg CI has no course");
    if ( !pPointNext->IsCourse() ) throw QObject::tr("Leg CI has no next course");

	QLineF line1;

	if (!m_Path.isEmpty())
	{
        _CalcAzimuth(cs, pPoint);
		line1.setP1(m_Path.currentPosition());
        line1.setLength( cs->distanceToScene(100) );
		line1.setAngle(m_Path.angleAtPercent(1));
	} else
	{
		line1.setP1(m_Path.currentPosition());
        line1.setLength( cs->distanceToScene(100) );
        line1.setAngle(algo::azimuthToAngle(_CorrectAngle(pPoint->GetCourse()) * M_PI / 180) * 180 / M_PI);
	}
	QLineF line2;
	line2.setP1(pnt);
    line2.setLength( cs->distanceToScene(100) );
    line2.setAngle(algo::azimuthToAngle(_CorrectAngle(pPointNext->GetCourse()) * M_PI / 180) * 180 / M_PI);
	QPointF LineIntersect;
    if ( !line2.intersect(line1, &LineIntersect) != QLineF::NoIntersection )
        throw QObject::tr("CI leg does not intersect the next leg");
	QLineF line3(line1.p1(), LineIntersect);

	double dAngle = line1.angleTo(line3);

	if (dAngle < 5 || dAngle > 355)
		_AddLine(LineIntersect, pPoint, false);
}

void SSA::_CalcAzimuth(const TCoordSystem &cs, const TPointPtr& pPoint)
{
    if ( !pPoint->IsCourse() )
        throw QObject::tr("Leg has no course");

	if (m_Path.isEmpty())
		return;

	QLineF Line1;
	Line1.setP1(m_Path.currentPosition());
	Line1.setAngle(m_Path.angleAtPercent(1));
    Line1.setLength( cs->distanceToScene(100) );

    bool bRight(algo::getTurningOrientation(
            algo::angleToAzimuth(Line1.angle() * M_PI / 180),
			pPoint->GetCourse() * M_PI / 180) == 1);
	bool bBigCircle(
        (pPoint->GetTurn() == arinc::SSAPoint::enLeft && bRight) ||
        (pPoint->GetTurn() == arinc::SSAPoint::enRight && !bRight));
	bool bCircleRight(
        pPoint->GetTurn() != arinc::SSAPoint::enLeft &&
        (pPoint->GetTurn() == arinc::SSAPoint::enRight || bRight));

    double dR = cs->distanceToScene(3800);

	QLineF RadLine1 = Line1.normalVector();
	if (bCircleRight)
	{
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
    qreal rAngle2 = algo::azimuthToAngle(_CorrectAngle(pPoint->GetCourse()) * M_PI / 180) * 180 / M_PI;
	if (bCircleRight)
		rAngle2 += 90;
	else
		rAngle2 -= 90;
	rAngle2 = fmod(rAngle2 - rAngle1, 360);
	if (rAngle2 < 0)
		rAngle2 += 360;
	if (bBigCircle)
	{
		if (rAngle2 < 180)
		{
			rAngle2 = 360 - rAngle2;
		}
	}
	else
	{
		if (rAngle2 > 180)
		{
			rAngle2 = 360 - rAngle2;
		}
	}
	if (bCircleRight)
		rAngle2 *= -1;
	if (rAngle2 > 1 || rAngle2 < -1)
		_AddArc(Rect, rAngle1, rAngle2, pPoint);
}

void SSA::_AddArc(
	const QRectF& Rect,
	qreal rAngle1,
	qreal rAngle2,
	const TPointPtr& pPoint)
{
	if (m_bRoute)
	{
		QPainterPath TempPath;
		TempPath.moveTo(m_Path.currentPosition());
		TempPath.arcTo(Rect, rAngle1, rAngle2);
		int i = 0;
		for (; i < 9; ++i)
		{
			QPointF TempPoint = TempPath.pointAtPercent((double)i / 8);
			_AddPoint(TempPoint, pPoint, false);
		}
		m_Path.addPath(TempPath);
	} else
	{
		m_Path.arcTo(Rect, rAngle1, rAngle2);
	}
}

void SSA::_AddLine(
	const QPointF& pnt,
	const TPointPtr& pPoint,
	bool bInclude)
{
	m_Path.lineTo(pnt);
	if (m_bRoute)
		_AddPoint(pnt, pPoint, bInclude);
}

void SSA::_AddPoint(
	const QPointF& pnt,
	const TPointPtr& pSSAPoint,
	bool bInclude)
{
	if (m_bRoute)
	{
        QSharedPointer<arinc::Point> pPoint;
        QSharedPointer<arinc::RoutePoint> pRoutePoint(new arinc::RoutePoint);
		pRoutePoint->SetPoint(pPoint);
		if (bInclude && pSSAPoint && pSSAPoint->GetPoint())
		{
			pPoint = pSSAPoint->GetPoint();
		} else
		{
			pRoutePoint->SetSN(m_iSN);
			pRoutePoint->SetCustom(true);
            pPoint = QSharedPointer<arinc::Point>(new arinc::Point);
			pPoint->SetName(GetName() + QString::number(m_iSN));
			pPoint->GetCoord().SetX(pnt.x());
			pPoint->GetCoord().SetY(pnt.y());
		}

		pRoutePoint->SetPoint(pPoint);

		if (pSSAPoint)
		{
			if (pSSAPoint->IsAlt1())
				pRoutePoint->SetLevel(GetParent()->GetRunwayAlt() + pSSAPoint->GetAlt1());
			if (pSSAPoint->IsSpeedLim())
				pRoutePoint->SetSpeed(pSSAPoint->GetSpeedLim());
		}
		m_Route.append(pRoutePoint);
		++m_iSN;
	}
}

qreal SSA::_CorrectAngle(qreal angle)
{
    return ( angle + m_angleAzCorrect );
}

}   // namespace geoGE
