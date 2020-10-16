#include <stdlib.h>


#include "Coordinate.h"

#if defined(_MSC_VER) && QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#define _USE_MATH_DEFINES
#endif
#include <math.h>

CAngle::CAngle(int iDegrees, int iMinutes, double dSeconds)
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

bool CAngle::operator== (const CAngle& angle) const
{
    //return (m_dAngle == angle.GetAngle());
    return qFuzzyCompare(m_dAngle, angle.GetAngle());
}

bool CAngle::operator< (const CAngle& angle) const
{
    return (m_dAngle < angle.GetAngle());
}

CCoord::CCoord(const CAngle& Longitude, const CAngle& Latitude)
    : m_Longitude(Longitude), m_Latitude(Latitude),
    m_X(0), m_Y(0), m_bCenter(false)
{

}

CCoord::CCoord(double Longitude, double Latitude)
    : m_Longitude(Longitude), m_Latitude(Latitude),
    m_X(0), m_Y(0), m_bCenter(false)
{

}

CCoord::CCoord()
    : m_X(0), m_Y(0), m_bCenter(false)
{

}

void CCoord::Center(const CCoord& Center)
{
    // http://gis-lab.info/qa/great-circles.html
    qint64 EarthRadius = 6372795;

    // Inverse calculation
    // points in radians
    double dLat1 = Center.GetLatitude().GetAngle()*M_PI/180;
    double dLat2 = GetLatitude().GetAngle()*M_PI/180;
    double dLong1 = Center.GetLongitude().GetAngle()*M_PI/180;
    double dLong2 = GetLongitude().GetAngle()*M_PI/180;

    // cos & sin latitudes and delta longitude
    double dCl1 = cos(dLat1);
    double dCl2 = cos(dLat2);
    double dSl1 = sin(dLat1);
    double dSl2 = sin(dLat2);
    double dDelta = dLong2 - dLong1;
    double dCDelta = cos(fabs(dDelta));
    double dSDelta = sin(dDelta);

    // the distance of a big circle
    double dP1 = pow(dCl2*dSDelta, 2);
    double dP2 = pow((dCl1*dSl2) - (dSl1*dCl2*dCDelta), 2);
    double dP3 = pow(dP1 + dP2, 0.5);
    double dP4 = dSl1*dSl2;
    double dP5 = dCl1*dCl2*dCDelta;
    double dP6 = dP4 + dP5;
    //double dP7 = dP3/dP6;
    //double dAngleRad = atan(dP7);
    double dAngleRad = atan2(dP3, dP6);
    qint64 qllDist = (qint64)PreRound(dAngleRad * EarthRadius);

    // azimuth
    double dAngleDiff = atan2(dSDelta*dCl2, dCl1*dSl2 - dSl1*dCl2*dCDelta);
    double dRadians = fmod(dAngleDiff,2*M_PI);

    m_X = (qint64)PreRound(qllDist*sin(dRadians));
    m_Y = (qint64)PreRound(qllDist*cos(dRadians));
    // X & Y
    m_bCenter = true;
}

void CCoord::DeCenter(const CCoord& Center)
{
    // http://en.wikipedia.org/wiki/Vincenty%27s_formulae
    qint64 EarthRadius = 6372795;
    // distance and azimuth
    qint64 qllDist = (qint64)GetDistance(*this, Center);
    if (qllDist == 0)
    {
        SetLatitude(Center.GetLatitude());
        SetLongitude(Center.GetLongitude());
    }
    else
    {
        double dAzim = M_PI - GetAzimuth(*this, Center);

        // Direct calculation
        double dAngleRad = (double)qllDist / EarthRadius;
        double dLat1 = Center.GetLatitude().GetAngle()*M_PI/180;
        double dLong1 = Center.GetLongitude().GetAngle()*M_PI/180;

        // cos & sin
        double dSl1        = sin(dLat1);
        double dSAngle    = sin(dAngleRad);
        double dSAzim    = sin(dAzim);
        double dCl1        = cos(dLat1);
        double dCAngle    = cos(dAngleRad);
        double dCAzim    = cos(dAzim);
        // general parts

        double dP1 = dSAngle * dCAzim;
        double dP2 = dSl1 * dSAngle;
        double dP3 = dCl1 * dCAngle;
        // latitude
        double dLatU = dSl1 * dCAngle + dCl1 * dP1;
        double dLatD = pow(pow(dCl1 * dSAzim, 2) + pow(dSl1*dSAngle - dP3 * dCAzim, 2), 0.5);
        double dLat2 = atan2(dLatU, dLatD);
        // longitude
        double dLongU = dSAngle * dSAzim;
        double dLongD = dP3 - dP2*dCAzim;
        double dLongDelta = atan2(dLongU, dLongD);
        double dLong2 = dLong1 - dLongDelta;

        dLat2 *= 180; dLat2 /= M_PI;
        dLong2 *= 180; dLong2 /= M_PI;
        SetLatitude(CAngle(dLat2));
        SetLongitude(CAngle(dLong2));
    }
}

QString CCoord::GetAsString(const QString sFormat/* = QString::null*/) const
{
    QString sStatus;
    if (GetLatitude().GetAngle()>=0)
        sStatus += 'N';
    else
        sStatus += 'S';
    sStatus += QString::number(abs(GetLatitude().GetDegrees())).rightJustified(2, '0');
    sStatus += ' ';
    sStatus += QString::number(GetLatitude().GetMinutes()).rightJustified(2, '0');
    sStatus += ' ';
    sStatus += QString::number((int)GetLatitude().GetSeconds()).rightJustified(2, '0');
    sStatus += ' ';
    if (GetLongitude().GetAngle()>=0)
        sStatus += 'E';
    else
        sStatus += 'W';
    sStatus += QString::number(abs(GetLongitude().GetDegrees())).rightJustified(3, '0');
    sStatus += ' ';
    sStatus += QString::number(GetLongitude().GetMinutes()).rightJustified(2, '0');
    sStatus += ' ';
    sStatus += QString::number((int)GetLongitude().GetSeconds()).rightJustified(2, '0');
    return sStatus;
}

bool CCoord::IsNull() const
{
    return (m_Longitude.IsNull() && m_Latitude.IsNull());
}

bool CCoord::operator ==(const CCoord& coord) const
{
    return ((m_Longitude == coord.GetLongitude()) && (m_Latitude == coord.GetLatitude()));
}

bool CCoord::operator !=(const CCoord& coord) const
{
    return !(*this == coord);
}

bool CCoord::operator <(const CCoord& coord) const
{
    if(m_Latitude == coord.GetLatitude())
        return (m_Longitude < coord.GetLongitude());
    return (m_Latitude < coord.GetLatitude());
}

double GetAzimuth(const CCoord& First, const CCoord& Second)
{
    double dAzim = atan2(
            Second.GetX() - First.GetX(),
            Second.GetY() - First.GetY());

    return Azimuth(dAzim);
}

double Azimuth(double dAzim)
{
    dAzim = fmod(dAzim, 2*M_PI);
    if (dAzim < 0)
        dAzim = 2*M_PI + dAzim;

    return dAzim;
}

double GetDistance(const CCoord& First, const CCoord& Second)
{
    return pow(
            pow(First.GetX() - Second.GetX(), 2) +
            pow(First.GetY() - Second.GetY(), 2), 0.5);
}

double PreRound(double dVal)
{
    return (dVal >= 0)? dVal + 0.5: dVal - 0.5;
}

double GetAzimuthDif(double dAzim1, double dAzim2)
{
    double dDelta = dAzim2 - dAzim1;
    if (dDelta < 0)
        dDelta *= -1;
    if (dDelta > M_PI)
        dDelta = 2*M_PI - dDelta;
    return dDelta;
}

double AzimuthToAngle(double dAzim)
{
    dAzim = Azimuth(2 * M_PI - dAzim + M_PI_2);
    if (dAzim > M_PI)
        dAzim = dAzim - 2 * M_PI;
    return dAzim;
}

double AngleToAzimuth(double dAngle)
{
    if (dAngle < 0)
        dAngle = dAngle + 2 * M_PI;
    dAngle = Azimuth(2 * M_PI - dAngle + M_PI_2);
    return dAngle;
}

double CVelocity::GetV() const
{
  return pow(pow(m_Vx, 2) + pow(m_Vy, 2), 0.5);
}

QDataStream &operator<<(QDataStream &out, const CAngle &value)
{
    out << value.GetAngle();
    return out;
}

QDataStream &operator>>(QDataStream &in, CAngle &value)
{
    double dVal(0);
    in >> dVal;
    value.SetAngle(dVal);
    return in;
}

QDataStream &operator<<(QDataStream &out, const CCoord &value)
{
    out << value.GetLatitude();
    out << value.GetLongitude();
    return out;
}

QDataStream &operator>>(QDataStream &in, CCoord &value)
{
    CAngle angle;
    in >> angle;
    value.SetLatitude(angle);
    in >> angle;
    value.SetLongitude(angle);
    return in;
}

int CbetweenAandB(const CCoord& C, const CCoord& A, const CCoord& B)
{
    if(!A.IsNull() && A.IsCenter() && !B.IsNull() && B.IsCenter() && !C.IsNull() && C.IsCenter())
    {
        double a = GetDistance(B, C);
        double b = GetDistance(A, C);
        double c = GetDistance(A, B);

        if(a==0 && b==0 && c==0)
            return 2;
        if(a==0)
            return 2;
        if(b==0)
            return 2;
        if(c==0)
            return 0;

        if(a+b == c)
            return 2;

        double alpha = acos((b*b + c*c - a*a) / (2*b*c));
        double beta = acos((a*a + c*c - b*b) / (2*a*c));

        return (alpha < M_PI/2 && beta < M_PI/2) ? 1 : 0;
    }
    return 0;
}
