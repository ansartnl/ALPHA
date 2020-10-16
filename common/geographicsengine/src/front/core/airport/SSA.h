#ifndef _SSA_H__
#define _SSA_H__

#include "orm/SSA.h"
#include <QPainterPath>
#include "cs/CoordSystem.h"


//! Geodesy graphics engine
namespace geoGE {

//! Class used to draw Sid, Star and Approach
class SSA
    : public arinc::SSA
{
public:
    typedef QList<QSharedPointer<arinc::RoutePoint> >   TPointRouteList;

    /** Calculates path to store */
    void CalculatePath(const TCoordSystem &cs, bool bRoute, qreal angleAzimuthCorrection = 0.0);
    /** Gets the current path */
	QPainterPath& GetPath() { return m_Path; }
    /** Gets the const current path */
	const QPainterPath& GetPath() const { return m_Path; }
    /** Gets the current route */
	TPointRouteList& GetRoute() { return m_Route; }

    /** Correct angle to conform with the squared projection north direction */
    qreal _CorrectAngle(qreal angle);

private:
    void _CalcIF(const TPointPtr& pPoint, const QPointF& pnt);
    void _CalcTF(const TPointPtr& pPoint, const QPointF& pnt);
    void _CalcCA(const TCoordSystem &cs, const TPointPtr& pPoint, const QPointF& pnt);
    void _CalcCF(const TCoordSystem &cs, const TPointPtr& pPoint, const QPointF& pnt);
    void _CalcDF(const TCoordSystem &cs, const TPointPtr& pPoint, const QPointF& pnt);
    void _CalcCI(const TCoordSystem &cs, const TPointPtr& pPoint, const TPointPtr& pPointNext, const QPointF& pnt);
    void _CalcAzimuth(const TCoordSystem &cs, const TPointPtr& pPoint);

    void _AddArc(
		const QRectF& rectangle,
		qreal startAngle,
		qreal sweepLength,
		const TPointPtr& pPoint);
	void _AddLine(
		const QPointF& pnt,
		const TPointPtr& pPoint,
		bool bInclude);
	void _AddPoint(
		const QPointF& pnt,
		const TPointPtr& pPoint,
		bool bInclude);

	QPainterPath	m_Path;
	TPointRouteList m_Route;
	double			m_dAlt;
	int				m_iSN;
	bool			m_bRoute;
    /** Azimuth correction angle */
    qreal           m_angleAzCorrect;
};

}   // namespace geoGE

#endif // _SSA_H__
