#ifndef CALCULATION_H
#define CALCULATION_H

#include <QtCore/qglobal.h>

class CFPL;
class Scene;
class CPlane;
class CCoord;
class QLineF;
class QPointF;

//! Namespace contains common calculation functions
namespace avia
{
	//! Function returns IAS factor ( in m/sec ) for the height
	/*!
		\param iH a height in meters
	*/
	double GetIASFactor(int iH);

	//! Function returns sonic speed ( in m/sec ) for the height
	/*!
		\param iH a height in meters
	*/
	double GetSonicSpeed(int iH);

	//! Function returns the turning radius ( in meters )
	/*!
		\param dSpeed a ground speed in m/sec
		\param dBank a bank in radians
	*/
	double GetTurningRadius(double dSpeed, double dBank);

	//! Function returns the turning angle ( in radians )
	/*!
		\param dSpeed a actual speed in m/sec
		\param dR a radius in meters
		\param dS a seconds
	*/
	double GetTurningDAngle(double dSpeed, double dR, double dS);

	//! The length of the turning by axis X
	/*!
		\param dR a radius in meters
		\param dAzimuth a azimuth in radians
		\param dDAngle a angle in radians
		\return length in meters
	*/
	double GetTurningDX(double dR, double dAzimuth, double dDAngle);

	//! The length of the turning by axis Y
	/*!
		\param dR a radius in meters
		\param dAzimuth a azimuth in radians
		\param dDAngle a angle in radians
		\return length in meters
	*/
	double GetTurningDY(double dR, double dAzimuth, double dDAngle);

	int GetTurningOrientation(double dFirst, double dNext);


	//! Function returns list of FIR's points from route part of FPL
	/*!
		\param FPL a pointer to CFPL
		\param Scene a pointer to Scene ( described a FIR )
		\param PointList a list of points
	*/
	//void GetPointsFromFPL(const CFPL& FPL, const Scene& Scene, QVector<QSharedPointer<CRoutePoint> >& PointList);

	//! Function calculate time ( in seconds ) from point coord1 to point coord2 with speed speed
	/*!
		\param coord1 a first CCoord
		\param coord2 a second CCoord
		\param speed a speed in m/sec
	*/
	quint64 GetTimeRoute(const CCoord& coord1, const CCoord& coord2, double speed);

	//! Side turn
	bool IsRightTurn(const QLineF& Line, const QPointF& Point);

}

#endif // CALCULATION_H
