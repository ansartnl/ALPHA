#ifndef POINT_H
#define POINT_H

#include "Coordinate.h"
#include "ItemHandler.h"
#include <QtCore/QSharedPointer>

//! Class describes the point of FIR or sector
class CPoint: public ItemHandler
{
  public:
    //! Point type
    enum Type
    {
      enUnknown = 0, //!< Unknown
        enVHFNavAid = 1, //!< VHF navigation aid
        enNDBNavAid, //!< NDB navigation aid
        enWaypoint, //!< Waypoint
       // enFirBorderPoint = 1,                //!< Boundary point
       // enSectorBorderPoint = 1,
        //enZonePoint,        // Not used :)
        enAirportPoint = 6, //!< Airport waypoint
        enAirportNDB = 7,
        enSSAWaypoint = 8
    //!< NDB navigation aid of an airport
    };
    //! Constructor
    CPoint() :
      m_AirportName(QLatin1String("")), m_Type(enUnknown), m_qsICAO(
        QLatin1String("")), m_uiID(0), m_bSignActiveSSAWaypoint(false)
    {
    }

    //! Destructor
    virtual ~CPoint()
    {
    }


    //! Set identifier of a point
    /*!
     \param uiID identifier of a point
     */
    void SetID(unsigned int uiID)
    {
      m_uiID = uiID;
    }
    //! Get identifier of a point
    /*!
     \return identifier of a point
     */
    unsigned int GetID() const
    {
      return m_uiID;
    }
    //! Set type of a point
    /*!
     \param Type type of a point
     */
    void SetType(const Type& Type)
    {
      m_Type = Type;
    }
    //! Get type of a point
    /*!
     \return type of a point
     */
    Type GetType() const
    {
      return m_Type;
    }
    //! Set point name
    /*!
     \param qsName point name
     */
    void SetName(const QString& qsName)
    {
      m_qsName = qsName;
    }
    //! Get point name
    /*!
     \return point name
     */
    const QString& GetName() const
    {
      return m_qsName;
    }
    //! Set ICAO
    /*!
     \param qsICAO ICAO
     */
    void SetICAO(const QString& qsICAO)
    {
      m_qsICAO = qsICAO;
    }
    //! Get ICAO
    /*!
     \return ICAO
     */
    const QString& GetICAO() const
    {
      return m_qsICAO;
    }
    //! Set point coordinate
    /*!
     \param Coord point coordinate
     */
    void SetCoord(const CCoord& Coord)
    {
      m_Coord = Coord;
    }
    //! Get point coordinate
    /*!
     \return point coordinate
     */
    const CCoord& GetCoord() const
    {
      return m_Coord;
    }
    //! Get point coordinate
    /*!
     \return point coordinate
     */
    CCoord& GetCoord()
    {
      return m_Coord;
    }
    //! Airport name

  public:
    const QString &GetAirportName() const
    {
      return m_AirportName;
    }
    void SetAirportName(const QString &V)
    {
      m_AirportName = V;
    }

    void SetSignActive(bool sign)
    {
        m_bSignActiveSSAWaypoint = sign;
    }
    bool GetSignActive()
    {
        return m_bSignActiveSSAWaypoint;
    }

  private:
    QString m_AirportName;

  public:

    //! Compare two points by identifier
    bool operator==(const CPoint& ref2nd) const
    {
      return GetID() == ref2nd.GetID();
    }
    //------------------
      public:
        void SetFirBorder(bool V)
        {
            m_FirBorder = V;
        }
        void SetSectorBorder(bool V)
        {
            m_SectorBorder = V;
        }
        bool GetFirBorder() const
        {
          return m_FirBorder;
        }
        bool GetSectorBorder() const
        {
          return m_SectorBorder;
        }
      private:
        bool m_FirBorder;
        bool m_SectorBorder;
      //bool m_bType;
    //-------------------------------
  private:
    Type m_Type;
    QString m_qsName;
    QString m_qsICAO;
    CCoord m_Coord;
    unsigned int m_uiID;
    bool m_bSignActiveSSAWaypoint;
};

//! Class describes the point key for map
class CPointKey
{
  public:
    //! Constructor
    explicit CPointKey(const QString& qsName);
    //! Constructor
    /*!
     \param qsName point name
     \param Type point type
     \param qsICAO code ICAO of the point
     */
    CPointKey(const QString& qsName, CPoint::Type Type, const QString& qsICAO);
    //! Constructor
    /*!
     \param qsName point name
     \param Type point type
     \param qsICAO code ICAO of the point
     \param AirportName airport name
     */
    CPointKey(const QString& qsName, CPoint::Type Type, const QString& qsICAO,
      const QString& AirportName);

    //! Point name

  public:
    const QString &GetName() const
    {
      return m_Name;
    }
    void SetName(const QString &V)
    {
      m_Name = V;
      MarkName(true);
    }
    bool IsName() const
    {
      return m_bName;
    }
    void MarkName(bool bName)
    {
      m_bName = bName;
    }
  private:
    QString m_Name;
    bool m_bName;
    //! Point type

  public:
    const CPoint::Type &GetType() const
    {
      return m_Type;
    }
    void SetType(const CPoint::Type &V)
    {
      m_Type = V;
      MarkType(true);
    }
    bool IsType() const
    {
      return m_bType;
    }
    void MarkType(bool bType)
    {
      m_bType = bType;
    }
  private:
    CPoint::Type m_Type;
    bool m_bType;
    //! Point code ICAO

  public:
    const QString &GetICAO() const
    {
      return m_ICAO;
    }
    void SetICAO(const QString &V)
    {
      m_ICAO = V;
      MarkICAO(true);
    }
    bool IsICAO() const
    {
      return m_bICAO;
    }
    void MarkICAO(bool bICAO)
    {
      m_bICAO = bICAO;
    }
  private:
    QString m_ICAO;
    bool m_bICAO;
    //! Point airport name

  public:
    const QString &GetAirportName() const
    {
      return m_AirportName;
    }
    void SetAirportName(const QString &V)
    {
      m_AirportName = V;
      MarkAirportName(true);
    }
    bool IsAirportName() const
    {
      return m_bAirportName;
    }
    void MarkAirportName(bool bAirportName)
    {
      m_bAirportName = bAirportName;
    }
  private:
    QString m_AirportName;
    bool m_bAirportName;

  public:
    //! Compare 2 CPointKey
    bool operator<(const CPointKey& ref2nd) const;

  private:
    void Init();
};

//! Class describes the route point
class CRoutePoint
{
  public:
    //! Constructor
    /*!
     \param iSN sequence number
     \param pPoint pointer to a point
     \param uiRoutID route identifier from database
     \param uiID route point identifier
     */
    CRoutePoint(unsigned int iSN, const QSharedPointer<CPoint>& pPoint,
      unsigned int uiRoutID = 0, unsigned int uiID = 0);
    //! Constructor
    CRoutePoint();
    //! Destructor
    virtual ~CRoutePoint()
    {
    }
    ;
    //! Set identifier of a point
    /*!
     \param uiID identifier of a point
     */
    void SetID(unsigned int uiID)
    {
      m_uiID = uiID;
    }
    //! Get identifier of a point
    /*!
     \return identifier of a point
     */
    unsigned int GetID() const
    {
      return m_uiID;
    }
    //! Set sequence number
    /*!
     \param iSN sequence number
     */
    void SetSN(unsigned int iSN)
    {
      m_iSN = iSN;
    }
    //! Get sequence number
    /*!
     \return sequence number
     */
    unsigned int GetSN() const
    {
      return m_iSN;
    }
    //! Set pointer to point
    /*!
     \param pPoint pointer to point
     */
    void SetPoint(const QSharedPointer<CPoint>& pPoint)
    {
      m_pPoint = pPoint;
    }
    //! Get pointer to point
    /*!
     \return pointer to point
     */
    const QSharedPointer<CPoint>& GetPoint() const
    {
      return m_pPoint;
    }
    //! Set route identifier from database
    /*!
     \param uiRoutID identifier from database
     */
    void SetRoutID(unsigned int uiRoutID)
    {
      m_uiRoutID = uiRoutID;
    }
    //! Get route identifier from database
    /*!
     \return identifier from database
     */
    unsigned int GetRoutID() const
    {
      return m_uiRoutID;
    }
    //! Necessary speed on the point

  public:
    const double &GetSpeed() const
    {
      return m_Speed;
    }
    void SetSpeed(const double &V)
    {
      m_Speed = V;
      MarkSpeed(true);
    }
    bool IsSpeed() const
    {
      return m_bSpeed;
    }
    void MarkSpeed(bool bSpeed)
    {
      m_bSpeed = bSpeed;
    }
  private:
    double m_Speed;
    bool m_bSpeed;
    //! Necessary flight level on the point

  public:
    const double &GetLevel() const
    {
      return m_Level;
    }
    void SetLevel(const double &V)
    {
      m_Level = V;
      MarkLevel(true);
    }
    bool IsLevel() const
    {
      return m_bLevel;
    }
    void MarkLevel(bool bLevel)
    {
      m_bLevel = bLevel;
    }
  private:
    double m_Level;
    bool m_bLevel;
    //! Is point hand-made or from database

  public:
    const bool &GetCustom() const
    {
      return m_Custom;
    }
    void SetCustom(const bool &V)
    {
      m_Custom = V;
    }
  private:
    bool m_Custom;
    //! Is point an airport procedure

  public:
    const bool &GetIsSTAR() const
    {
      return m_IsSTAR;
    }
    void SetIsSTAR(const bool &V)
    {
      m_IsSTAR = V;
    }
    const bool &GetIsSID() const
    {
      return m_IsSID;
    }
    void SetIsSID(const bool &V)
    {
      m_IsSID = V;
    }
  private:
    bool m_IsSTAR;
    bool m_IsSID;
    //! Procedure name

  public:
    const QString &GetProcedureName() const
    {
      return m_ProcedureName;
    }
    void SetProcedureName(const QString &V)
    {
      m_ProcedureName = V;
    }
  private:
    QString m_ProcedureName;

  public:
    //! \name Out-of-date
    //@{
    void SetEnd(bool bEnd)
    {
      m_bEnd = bEnd;
    }
    bool IsEnd() const
    {
      return m_bEnd;
    }
    //@}

  private:
    unsigned int m_iSN;
    QSharedPointer<CPoint> m_pPoint;
    unsigned int m_uiRoutID;
    unsigned int m_uiID;
    bool m_bEnd;
};
#endif // POINT_H
