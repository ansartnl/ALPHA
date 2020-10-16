#include "Coordinate.h"
#include "math.h"
#include <stdlib.h>
#include <QtGlobal>

CAngle::CAngle(
	int iDegrees,
	int iMinutes,
	double dSeconds)
		: m_iDegrees(iDegrees), m_iMinutes(iMinutes), m_dSeconds(dSeconds)
{
	if (m_dSeconds < 0)
		m_dSeconds *= -1;
	_CalcAngle();
}

CAngle::CAngle(double dAngle)
	: m_dAngle(dAngle)
{
	_CalcDegrees();
}

CAngle::CAngle()
	: m_iDegrees(0), m_iMinutes(0), m_dSeconds(0.0), m_dAngle(0.0)
{

}

void CAngle::SetDegrees(int iDegrees)
{
	m_iDegrees = iDegrees;
	_CalcAngle();
}

int CAngle::GetDegrees() const
{
	return m_iDegrees;
}

void CAngle::SetMinutes(int iMinutes)
{
	m_iMinutes = abs(iMinutes);
	_CalcAngle();
}

int CAngle::GetMinutes() const
{
	return m_iMinutes;
}

void CAngle::SetSeconds(double dSeconds)
{
	m_dSeconds = dSeconds;
	if (m_dSeconds < 0)
		m_dSeconds *= -1;
	_CalcAngle();
}

double CAngle::GetSeconds() const
{
	return m_dSeconds;
}

void CAngle::SetAngle(double dAngle)
{
	m_dAngle = dAngle;
	_CalcDegrees();
}
void CAngle::_CalcAngle()
{
	m_dAngle = abs(m_iDegrees);
	m_dAngle += ((double)m_iMinutes*60 + m_dSeconds)/3600;
	if (m_iDegrees < 0)
		m_dAngle *= -1;
}

void CAngle::_CalcDegrees()
{
	m_iDegrees = (int)m_dAngle;
	m_dSeconds = (m_dAngle - m_iDegrees)*60;
	m_iMinutes = (int)m_dSeconds;
	m_dSeconds = (m_dSeconds - m_iMinutes)*60;
	m_iMinutes = abs(m_iMinutes);
	if (m_dSeconds < 0)
		m_dSeconds *= -1;
}

bool CAngle::IsNull() const
{
	return ((m_iDegrees == 0) && (m_iMinutes == 0) && (m_dSeconds == 0));
}

bool CAngle::operator ==(const CAngle& angle) const
{
    return ( qFuzzyCompare(m_dAngle+1.0, angle.GetAngle()+1.0) );
}

CCoord::CCoord(const CAngle& Longitude, const CAngle& Latitude, double Height)
    : m_Longitude(Longitude), m_Latitude(Latitude), m_Height(Height),
    m_X(0), m_Y(0), m_Z(0)
{

}

CCoord::CCoord(double Longitude, double Latitude, double Height)
    : m_Longitude(Longitude), m_Latitude(Latitude), m_Height(Height),
    m_X(0), m_Y(0), m_Z(0)
{

}

CCoord::CCoord()
    : m_X(0), m_Y(0), m_Z(0)
{

}

QString CCoord::GetAsString(const QString sFormat/* = QString::null*/) const
{
    Q_UNUSED(sFormat)
	QString sStatus;
//	if (GetLatitude().GetAngle()>=0)
//		sStatus += QL1C('N');
//	else
//		sStatus += QL1C('S');
//	sStatus += QString::number(abs(GetLatitude().GetDegrees())).rightJustified(2, QL1C('0'));
//	sStatus += QL1C(' ');
//	sStatus += QString::number(GetLatitude().GetMinutes()).rightJustified(2, QL1C('0'));
//	sStatus += QL1C(' ');
//	sStatus += QString::number((int)GetLatitude().GetSeconds()).rightJustified(2, QL1C('0'));
//	sStatus += QL1C(' ');
//	if (GetLongitude().GetAngle()>=0)
//		sStatus += QL1C('E');
//	else
//		sStatus += QL1C('W');
//	sStatus += QString::number(abs(GetLongitude().GetDegrees())).rightJustified(3, QL1C('0'));
//	sStatus += QL1C(' ');
//	sStatus += QString::number(GetLongitude().GetMinutes()).rightJustified(2, QL1C('0'));
//	sStatus += QL1C(' ');
//	sStatus += QString::number((int)GetLongitude().GetSeconds()).rightJustified(2, QL1C('0'));

    sStatus = QString::fromUtf8("B = %1\302\260 %2\" %3'")
               .arg(GetLatitude().GetDegrees(), 2, 'f', 0, '0')
               .arg(GetLatitude().GetMinutes(), 2, 'f', 0, '0')
               .arg(GetLatitude().GetSeconds(), 2, 'f', 0, '0');
    sStatus += QString::fromUtf8(" L = %1\302\260 %2\" %3'")
               .arg(GetLongitude().GetDegrees(), 2, 'f', 0, '0')
               .arg(GetLongitude().GetMinutes(), 2, 'f', 0, '0')
               .arg(GetLongitude().GetSeconds(), 2, 'f', 0, '0');

	return sStatus;
}

bool CCoord::IsNull() const
{
	return (m_Longitude.IsNull() && m_Latitude.IsNull());
}

bool CCoord::operator ==(const CCoord& coord) const
{
	return ((m_Longitude == coord.GetLongitude()) && m_Latitude == coord.GetLatitude());
}

bool CCoord::operator !=(const CCoord& coord) const
{
	return !(*this == coord);
}

bool CVelocity::operator == (const CVelocity &vel) const
{
    return ( qFuzzyCompare(m_Vx+1.0, vel.m_Vx+1.0) && qFuzzyCompare(m_Vy+1.0, vel.m_Vy+1.0) );
}
