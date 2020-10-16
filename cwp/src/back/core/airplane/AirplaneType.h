#ifndef AIRPLANETYPE_H
#define AIRPLANETYPE_H

#include <QtCore/qglobal.h>
#include <QtCore/QString>
#include <QtCore/QSharedPointer>

//! Class contains the airplane's speed
class CAirplaneSpeed
{
  public:
    CAirplaneSpeed() :
      m_ID(0), m_Level(0), m_VClimb(0), m_VClimbMax(0), m_VDescend(0),
          m_VDescendMax(0), m_IAS(0), m_IASMin(0), m_IASMax(0)
    {
    }
    //! Primary key in DB

  public:
    const quint32 &GetID() const
    {
      return m_ID;
    }
    void SetID(const quint32 &V)
    {
      m_ID = V;
    }
  private:
    quint32 m_ID;
    //! Flight level

  public:
    const float &GetLevel() const
    {
      return m_Level;
    }
    void SetLevel(const float &V)
    {
      m_Level = V;
    }
  private:
    float m_Level;
    //! Default climb

  public:
    const float &GetVClimb() const
    {
      return m_VClimb;
    }
    void SetVClimb(const float &V)
    {
      m_VClimb = V;
    }
  private:
    float m_VClimb;
    //! Max climb

  public:
    const float &GetVClimbMax() const
    {
      return m_VClimbMax;
    }
    void SetVClimbMax(const float &V)
    {
      m_VClimbMax = V;
    }
  private:
    float m_VClimbMax;
    //! Default descend

  public:
    const float &GetVDescend() const
    {
      return m_VDescend;
    }
    void SetVDescend(const float &V)
    {
      m_VDescend = V;
    }
  private:
    float m_VDescend;
    //! Max descend

  public:
    const float &GetVDescendMax() const
    {
      return m_VDescendMax;
    }
    void SetVDescendMax(const float &V)
    {
      m_VDescendMax = V;
    }
  private:
    float m_VDescendMax;
    //! Default IAS

  public:
    const float &GetIAS() const
    {
      return m_IAS;
    }
    void SetIAS(const float &V)
    {
      m_IAS = V;
    }
  private:
    float m_IAS;
    //! Min IAS

  public:
    const float &GetIASMin() const
    {
      return m_IASMin;
    }
    void SetIASMin(const float &V)
    {
      m_IASMin = V;
    }
  private:
    float m_IASMin;
    //! Max IAS

  public:
    const float &GetIASMax() const
    {
      return m_IASMax;
    }
    void SetIASMax(const float &V)
    {
      m_IASMax = V;
    }
  private:
    float m_IASMax;
    //! Airplane name ( e.g. A320 )

  public:
    const QString &GetAirTypeID() const
    {
      return m_AirTypeID;
    }
    void SetAirTypeID(const QString &V)
    {
      m_AirTypeID = V;
    }
  private:
    QString m_AirTypeID;
};

//! Class contains the airplane's characteristics
class CAirplaneType
{
  public:
    typedef QSharedPointer<CAirplaneSpeed> TSpeed;
    typedef QList<TSpeed> TSpeedList;
    CAirplaneType() :
      m_LevelMax(14000), m_BankMax(30), m_BankDef(20)
    {
    }
    //! Airplane name ( e.g. A320 )

  public:
    const QString &GetName() const
    {
      return m_Name;
    }
    void SetName(const QString &V)
    {
      m_Name = V;
    }
  private:
    QString m_Name;
    //! Max flight level

  public:
    const float &GetLevelMax() const
    {
      return m_LevelMax;
    }
    void SetLevelMax(const float &V)
    {
      m_LevelMax = V;
    }
  private:
    float m_LevelMax;
    //! Max bank

  public:
    const int &GetBankMax() const
    {
      return m_BankMax;
    }
    void SetBankMax(const int &V)
    {
      m_BankMax = V;
    }
  private:
    int m_BankMax;
    //! Default bank

  public:
    const int &GetBankDef() const
    {
      return m_BankDef;
    }
    void SetBankDef(const int &V)
    {
      m_BankDef = V;
    }
  private:
    int m_BankDef;
    //! List of CAirplaneSpeed

  public:
    const TSpeedList &GetSpeeds() const
    {
      return m_Speeds;
    }
    void SetSpeeds(const TSpeedList &V)
    {
      m_Speeds = V;
    }
  private:
    TSpeedList m_Speeds;

  public:
    TSpeedList& GetSpeeds()
    {
      return m_Speeds;
    }
    void AddSpeed(const TSpeed& pSpeed)
    {
      m_Speeds.append(pSpeed);
    }
    double GetMaxIAS(double dHigh) const;
    double GetMinIAS(double dHigh) const;
    double GetIAS(double dHigh) const;
    double GetMaxClimb(double dHigh) const;
    double GetClimb(double dHigh) const;
    double GetMaxDescend(double dHigh) const;
    double GetDescend(double dHigh) const;
    int GetLevelN(double dHigh) const;

};
#endif // AIRPLANETYPE_H
