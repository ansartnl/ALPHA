#ifndef COORDINATE_H
#define COORDINATE_H

#include <QtCore/QDataStream>

#include "TVAError.h"

//! Angle
class CAngle
{
  public:
    CAngle(int iDegrees, int iMinutes, double dSeconds);
    explicit CAngle(double dAngle);
    CAngle();

    void SetDegrees(int iDegrees);
    int GetDegrees() const;
    void SetMinutes(int iMinutes);
    int GetMinutes() const;
    void SetSeconds(double dSeconds);
    double GetSeconds() const;
    void SetAngle(double dAngle);
    double GetAngle() const
    {
      return m_dAngle;
    }
    bool IsNull() const;
    bool operator==(const CAngle& angle) const;
    bool operator<(const CAngle& angle) const;

  private:
    void _CalcDegrees();
    void _CalcAngle();
    int m_iDegrees;
    int m_iMinutes;
    double m_dSeconds;
    double m_dAngle;
};

QDataStream &operator<<(QDataStream &out, const CAngle &value);
QDataStream &operator>>(QDataStream &in, CAngle &value);

//! Coordinate
class CCoord
{
  public:
    CCoord(const CAngle& Longitude, const CAngle& Latitude);
    CCoord(double Longitude, double Latitude);
    CCoord();

    void SetLongitude(const CAngle& Longitude)
    {
      m_Longitude = Longitude;
    }

    const CAngle GetLongitude() const
    {
      return m_Longitude;
    }

    void SetLatitude(const CAngle& Latitude)
    {
      m_Latitude = Latitude;
    }

    CAngle GetLatitude() const
    {
      return m_Latitude;
    }

    void SetX(qint64 illX)
    {
      m_X = illX;
    }
    const qint64 GetX() const
    {
      return m_X;
    }
    void SetY(qint64 illY)
    {
      m_Y = illY;
    }
    const qint64 GetY() const
    {
      return m_Y;
    }

    void Center(const CCoord& Center);
    void DeCenter(const CCoord& Center);
    const bool IsCenter() const
    {
      return m_bCenter;
    }
    QString GetAsString(const QString sFormat = QString::null) const;
    bool IsNull() const;
    bool operator ==(const CCoord& coord) const;
    bool operator !=(const CCoord& coord) const;
    bool operator <(const CCoord& coord) const;

  private:
    CAngle m_Longitude;
    CAngle m_Latitude;
    qint64 m_X;
    qint64 m_Y;
    bool m_bCenter;
};

QDataStream &operator<<(QDataStream &out, const CCoord &value);
QDataStream &operator>>(QDataStream &in, CCoord &value);

//! Velocity
class CVelocity
{
  public:
    CVelocity() :
      m_Vx(0), m_Vy(0)
    {
    }
    CVelocity(double Vx, double Vy) :
      m_Vx(Vx), m_Vy(Vy)
    {
    }

    const double &GetVx() const
    {
      return m_Vx;
    }
    void SetVx(const double &V)
    {
      m_Vx = V;
    }

    const double &GetVy() const
    {
      return m_Vy;
    }
    void SetVy(const double &V)
    {
      m_Vy = V;
    }
    //! Get velocity
    double GetV() const;
  private:
    double m_Vx;
    double m_Vy;
};

double GetAzimuth(const CCoord& First, const CCoord& Second);
double Azimuth(double dAzim);
double GetDistance(const CCoord& First, const CCoord& Second);
double PreRound(double dVal);
double GetAzimuthDif(double dAzim1, double dAzim2);
double AzimuthToAngle(double dAzim);
double AngleToAzimuth(double dAngle);

/*!
 * \brief CbetweenAandB verify is a point C between A and B
 * \return
 *  0 - none
 *  1 - yes
 *  2 - yes, lie on one line
 */
int CbetweenAandB(const CCoord& C, const CCoord& A, const CCoord& B);

#endif // COORDINATE_H
