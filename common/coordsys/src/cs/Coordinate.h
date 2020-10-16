#ifndef COORDINATE_H
#define COORDINATE_H

#include "coordsys_global.h"

#include <QtCore/QString>

//! Angle
class COORDSYS_EXPORT CAngle
{
public:
	CAngle(
		int iDegrees,
		int iMinutes,
		double dSeconds);
	explicit CAngle(
		double dAngle);
	CAngle();

	void SetDegrees(int iDegrees);
	int GetDegrees() const;
	void SetMinutes(int iMinutes);
	int GetMinutes() const;
	void SetSeconds(double dSeconds);
	double GetSeconds() const;
	void SetAngle(double dAngle);
	double GetAngle() const { return m_dAngle; }
	bool IsNull() const;
	bool operator == (const CAngle& angle) const;
    bool operator != (const CAngle& angle) const
    { return !operator == (angle); }

private:
	void _CalcDegrees();
	void _CalcAngle();
	int m_iDegrees;
	int m_iMinutes;
	double m_dSeconds;
	double m_dAngle;
};


//! Coordinate
class COORDSYS_EXPORT CCoord
{
public:
    CCoord(const CAngle& Longitude, const CAngle& Latitude, double Height = 0.0);
    CCoord(double Longitude, double Latitude, double Height = 0.0);
	CCoord();
	void SetLongitude(const CAngle& Longitude) { m_Longitude = Longitude; }
	const CAngle GetLongitude() const { return m_Longitude; }
	void SetLatitude(const CAngle& Latitude) { m_Latitude = Latitude; }
	const CAngle GetLatitude() const { return m_Latitude; }
    void SetHeight(double Height) { m_Height = Height; }
    double GetHeight() const { return m_Height; }
    void SetX(qint64 illX) { m_X = illX; }
    qint64 GetX() const { return m_X; }
	void SetY(qint64 illY) { m_Y = illY; }
    qint64 GetY() const { return m_Y; }
    void SetZ(qint64 illZ) { m_Z = illZ; }
    qint64 GetZ() const { return m_Z; }

	QString GetAsString(const QString sFormat = QString::null) const;

	bool IsNull() const;

	bool operator == (const CCoord& coord) const;
	bool operator != (const CCoord& coord) const;

private:
	CAngle m_Longitude;
	CAngle m_Latitude;
    double m_Height;
	qint64 m_X;
	qint64 m_Y;
    qint64 m_Z;
};


//! Velocity
class COORDSYS_EXPORT CVelocity
{
public:
	CVelocity()
		: m_Vx(0), m_Vy(0) {}
	CVelocity(double Vx, double Vy)
		: m_Vx(Vx), m_Vy(Vy) {}

    const double& GetVx() const { return m_Vx; }
    void SetVx(const double& Vx) { m_Vx = Vx; }

    const double& GetVy() const { return m_Vy; }
    void SetVy(const double& Vy) { m_Vy = Vy; }

    bool operator == (const CVelocity &vel) const;
    bool operator != (const CVelocity &vel) const
    { return !operator==(vel); }

private:
    double m_Vx, m_Vy;
};

#endif // COORDINATE_H
