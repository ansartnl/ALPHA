#ifndef CFPL_H_INCLUDE
#define CFPL_H_INCLUDE

#include <QtCore/QDateTime>
#include <QtCore/QUuid>
#include <QtCore/QVariantMap>
#include "airplane/AirplaneType.h"
#include "Coordinate.h"

#include "TVAMacro.h"
#include "TVAError.h"

//! Class describes flight plan
class CFPL
{
  public:
    //! Constructor
    CFPL() :
        m_ID(0), m_AirplanePtr(new CAirplaneType()), m_Speed(0), m_Level(0), m_Num(1)
    {
    }
    virtual ~CFPL() {}

    //! FPL Identifier

  public:
    const quint32 &GetID() const
    {
      return m_ID;
    }
    void SetID(const quint32 &V)
    {
      m_ID = V;
    }
    const QUuid& GetUid() const
    {
        return m_Uid;
    }
    void SetUid(const QUuid& uid)
    {
        m_Uid = uid;
    }
    const QString &GetFirName() const
    {
      return m_FirName;
    }
    void SetFirName(const QString &V)
    {
      m_FirName = V;
    }
    const QString &GetACID() const
    {
      return m_ACID;
    }
    void SetACID(const QString &V)
    {
      m_ACID = V;
    }
    const QSharedPointer<CAirplaneType> &GetAirplanePtr() const
    {
      return m_AirplanePtr;
    }

    void SetAirplanePtr(const QSharedPointer<CAirplaneType> & V)
    {
      m_AirplanePtr = V;
    }

    const QString &GetDepAerodrome() const
    {
      return m_DepAerodrome;
    }
    void SetDepAerodrome(const QString &V)
    {
      m_DepAerodrome = V;
    }

    const double &GetSpeed() const
    {
      return m_Speed;
    }
    void SetSpeed(const double &V)
    {
      m_Speed = V;
    }
    const QString &GetSpeedS() const
    {
      return m_SpeedS;
    }
    void SetSpeedS(const QString &V)
    {
      m_SpeedS = V;
    }
    const quint32 &GetLevel() const
    {
      return m_Level;
    }
    void SetLevel(const quint32 &V)
    {
      m_Level = V;
    }
    //! Cruising flight level, string format

    const QString &GetLevelS() const
    {
      return m_LevelS;
    }
    void SetLevelS(const QString &V)
    {
      m_LevelS = V;
    }

    //! Route
    const QString &GetRoute() const
    {
      return m_Route;
    }
    void SetRoute(const QString &V)
    {
      m_Route = V;
    }
    const QString &GetDestAerodrome() const
    {
      return m_DestAerodrome;
    }
    void SetDestAerodrome(const QString &V)
    {
      m_DestAerodrome = V;
    }
    const QString &GetFRules() const
    {
      return m_FRules;
    }
    void SetFRules(const QString &V)
    {
      m_FRules = V;
    }
    const QString &GetTypeFlight() const
    {
      return m_TypeFlight;
    }
    void SetTypeFlight(const QString &V)
    {
      m_TypeFlight = V;
    }
    const quint32 &GetNum() const
    {
      return m_Num;
    }
    void SetNum(const quint32 &V)
    {
      m_Num = V;
    }
    //! EET
    const QString &GetEET() const
    {
      return m_EET;
    }
    void SetEET(const QString &V)
    {
      m_EET = V;
    }
    //! AddInfo
    const QString &GetAddInfo() const
    {
      return m_AddInfo;
    }
    void SetAddInfo(const QString &V)
    {
      m_AddInfo = V;
    }
    //! AircraftType
    const QString &GetFlightType() const
    {
      return m_FlightType;
    }
    void SetFlightType(const QString &V)
    {
      m_FlightType = V;
    }
    //! Turbulence
    const QString &GetTurbulence() const
    {
      return m_Turbulence;
    }
    void SetTurbulence(const QString &V)
    {
      m_Turbulence = V;
    }
    const QString &GetEquipment() const
    {
      return m_Equipment;
    }
    void SetEquipment(const QString &V)
    {
      m_Equipment = V;
    }
    bool isRVSM() const;
    //! Departure time

    const QDateTime &GetDepTime() const
    {
      return m_DepTime;
    }
    void SetDepTime(const QDateTime &V)
    {
      m_DepTime = V;
    }

    //! Destination time
    const QString &GetDestTime() const
    {
      return m_DestTime;
    }
    void SetDestTime(const QString &V)
    {
      m_DestTime = V;
    }
    //! Alternative aerodrom #1
    const QString &GetAlt1Aerodrome() const
    {
      return m_Alt1Aerodrome;
    }
    void SetAlt1Aerodrome(const QString &V)
    {
      m_Alt1Aerodrome = V;
    }
    //! Alternative aerodrom #2
    const QString &GetAlt2Aerodrome() const
    {
      return m_Alt2Aerodrome;
    }
    void SetAlt2Aerodrome(const QString &V)
    {
      m_Alt2Aerodrome = V;
    }
    //! Additional information
    const QString &GetOther() const
    {
      return m_Other;
    }
    void SetOther(const QString &V)
    {
      m_Other = V;
    }

    //! SID
    const QString &GetSID() const
    {
        return m_SID;
    }
    void SetSID(const QString &V)
    {
        m_SID = V;
    }

    //! STAR
    const QString &GetSTAR() const
    {
        return m_STAR;
    }
    void SetSTAR(const QString &V)
    {
        m_STAR = V;
    }

    //! Set cruising speed
    /*!
     \param sSpeed cruising speed
     */
    void SetSpeed(const QString& sSpeed);
    //! Set cruising level
    /*!
     \param sLevel cruising level
     */
    void SetLevel(const QString& sLevel);
    //! Parse coordinate from string into logical structure
    /*!
     \param sCoord coordinate in string format
     \return logical structure
     */
    static CCoord ParseCoordinate(const QString& sCoord);
    //! Parse flight level from string into number
    /*!
     \param sLevel flight level in string format
     \return flight level
     */
    static quint32 ParseLevel(const QString& sLevel);
    //! Parse speed from string into number
    /*!
     \param sSpeed speed in string format
     \return speed
     */
    static double ParseSpeed(const QString& sSpeed);

    virtual QVariantMap toVariantMap() const;
    virtual void fromVariantMap(const QVariantMap& map);

  private:
    quint32 m_ID;
    QUuid m_Uid;
    QString m_FirName;
    QSharedPointer<CAirplaneType> m_AirplanePtr;
    QString m_ACID;
    QString m_DepAerodrome;
    double m_Speed;
    QString m_SpeedS;
    quint32 m_Level;
    QString m_LevelS;
    QString m_Route;
    QString m_DestAerodrome;
    QString m_Turbulence;
    QString m_Equipment;
    QString m_FRules;
    QString m_TypeFlight;
    quint32 m_Num;
    QDateTime m_DepTime;
    QString m_DestTime;
    QString m_Alt1Aerodrome;
    QString m_Alt2Aerodrome;
    QString m_Other;
    QString m_EET;
    QString m_AddInfo;
    QString m_FlightType;
    QString m_SID;
    QString m_STAR;
};

#endif // CFPL_H_INCLUDE
