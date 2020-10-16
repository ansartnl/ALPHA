#include "Calculation.h"

#include "Coordinate.h"

#include "core/View.h"
#include "Airport.h"

#if defined(_MSC_VER) && QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#define _USE_MATH_DEFINES
#endif
#include <math.h>

#include <QtCore/QStringList>
#include <QtCore/QLineF>
#include <QtCore/QPointF>

namespace avia
{
	double GetIASFactor(int iH)
	{
		double dA = (double)288 - (double)0.006496 * iH;
		double dB = pow(dA, 0.5);
		double dC = pow(dA, 2.628);
		return (double)171233 * dB / dC;
	}

	double GetSonicSpeed(int iH)
	{
		double dA = (double)288 - (double)0.006496 * iH;
		double dB = pow(dA, 0.5);
		return 20.05 * dB;
	}

	double GetTurningRadius(double dSpeed, double dBank)
	{
		return (dSpeed * dSpeed)/(9.81 * tan(dBank));
	}

	double GetTurningDAngle(double dSpeed, double dR, double dS)
	{
		return dSpeed * dS / dR;
	}

	double GetTurningDX(double dR, double dAzimuth, double dDAngle)
	{
		return dR * (cos(dAzimuth) - cos(dAzimuth + dDAngle));
	}

	double GetTurningDY(double dR, double dAzimuth, double dDAngle)
	{
		return dR * (sin(dAzimuth + dDAngle) - sin(dAzimuth));
	}

	int GetTurningOrientation(double dFirst, double dNext)
	{
		double dDelta = dNext - dFirst;
		if ((dDelta >= 0 && dDelta <= M_PI) ||
			(dDelta >= (-2 * M_PI) && dDelta <= -M_PI))
			return 1;
		return -1;
	}


	bool IsRightTurn(const QLineF& Line, const QPointF& Point)
	{
		QLineF LineTemp(Line.p1(), Point);
		double dAngle = Line.angleTo(LineTemp);
		if (dAngle > 0 && dAngle < 180)
			return false;
		return true;
	}

	quint64 GetTimeRoute(const CCoord& coord1, const CCoord& coord2, double speed)
	{
		CCoord coord = coord1;
		coord.Center(coord2);
		quint64 len = sqrt((pow(coord.GetX(),2)+pow(coord.GetY(),2)));
		return len/speed; //seconds
	}

}
