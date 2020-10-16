#ifndef AIRPORT_H
#define AIRPORT_H

#include "Coordinate.h"
#include "Runway.h"
#include "ItemHandler.h"
#include "SvgItem.h"
//! Class describes the airport
class CAirport: public CRunway::IParent, public ItemHandler
{
  public:
    //! Pointer to runway
    typedef QSharedPointer<CRunway> TRunwayPtr;
    typedef QSharedPointer<SvgItem> TSvgItemPtr;
    //! Map of runways. Key - runway name
    typedef QMap<QString, QSharedPointer<CRunway> > TRunways;
    //! Map of svg_items. Key - svg_item name
    typedef QMap<QString, QSharedPointer<SvgItem> > TSvgItems;
    //! Constructor
    CAirport() :
      m_dAlt(0), m_uiID(0)
    {
    }
    //! Destructor
    virtual ~CAirport()
    {
    }
    //! Set airport identifier
    /*!
     \param uiID airport identifier
     */
    void SetID(unsigned int uiID)
    {
      m_uiID = uiID;
    }
    //! Get airport identifier
    /*!
     \return airport identifier
     */
    unsigned int GetID() const
    {
      return m_uiID;
    }
    //! Set airport name
    /*!
     \param qsName airport name
     */
    void SetName(const QString& qsName)
    {
      m_qsName = qsName;
    }
    //! Get airport name
    /*!
     \return airport name
     */
    const QString& GetName() const
    {
      return m_qsName;
    }
    //! Set ICAO code
    /*!
     \param qsICAO ICAO code
     */
    void SetICAO(const QString& qsICAO)
    {
      m_qsICAO = qsICAO;
    }
    //! Get ICAO code
    /*!
     \return ICAO code
     */
    const QString& GetICAO() const
    {
      return m_qsICAO;
    }
    //! Set airport coordinate
    /*!
     \param Coord airport coordinate
     */
    void SetCoord(const CCoord& Coord)
    {
      m_Coord = Coord;
    }
    //! Get airport coordinate
    /*!
     \return airport coordinate
     */
    const CCoord& GetCoord() const
    {
      return m_Coord;
    }
    //! Get airport coordinate
    /*!
     \return airport coordinate
     */
    CCoord& GetCoord()
    {
      return m_Coord;
    }
    //! Set airport altitude
    /*!
     \param dAlt airport altitude
     */
    void SetAlt(double dAlt)
    {
      m_dAlt = dAlt;
    }
    //! Get airport altitude
    /*!
     \return airport altitude
     */
    double GetAlt() const
    {
      return m_dAlt;
    }
    //! Set airport declination
    /*!
     \param sDecl airport declination as a string (eg. E0060)
     */
    void SetDeclination(QString sDecl)
    {
        if(sDecl[0] == 'W')
            m_declination = -sDecl.right(4).toDouble() / 10.0;
        else if(sDecl[0] == 'E')
            m_declination = sDecl.right(4).toDouble() / 10.0;
        else
            m_declination = 0;
    }
    //! Get airport declination
    /*!
     \return airport declination
     */
    double GetDeclination() const
    {
      return m_declination;
    }

    /*! \name Interface CRunway::IParent */
    //@{
    virtual const CCoord& GetPlace() const;
    virtual double GetAirportAlt() const;
    //@}

    virtual void MarkItemAllActive(bool bActive);

    //! Runway list
    const TRunways &GetRunways() const
    {
      return m_Runways;
    }
    void SetRunways(const TRunways &V)
    {
      m_Runways = V;
    }

    //! Add runway
    /*!
     \param pPtr Runway pointer
     */
    TRunways::iterator AddRunway(const TRunwayPtr& pPtr);
    //! Get runway list
    /*!
     \return runway list
     */
    TRunways& GetRunways()
    {
      return m_Runways;
    }
    bool IsSvgItemExist(const QString& key) const
    {
        return m_svg_items.contains(key);
    }
    void AppendSvgItem(const QString & name, const TSvgItemPtr& pPtr)
    {
        if(!m_svg_items.contains(name))
        {
            m_svg_items.insert(name, pPtr);
        }
    }
    const TSvgItems& GetSvgItems() const
    {
        return m_svg_items;
    }
    QSharedPointer<CSSA> Find(const QString& sSSA, CSSA::Type SSAType);

  private:
    QString m_qsName;
    QString m_qsICAO;
    CCoord m_Coord;
    QString m_qsData;
    double m_dAlt;
    double m_declination;
    unsigned int m_uiID;
    TRunways m_Runways;
    TSvgItems m_svg_items;
};

//! Class describes the airport's key
class CAirportKey
{
  public:
    //! Constructor
    /*!
     \param qsName airport name
     */
    CAirportKey(const QString& qsName)
    {
      Init();
      SetName(qsName);
    }
    //! Constructor
    /*!
     \param qsName airport name
     \param qsICAO ICAO code
     */
    CAirportKey(const QString& qsName, const QString& qsICAO)
    {
      Init();
      SetName(qsName);
      SetICAO(qsICAO);
    }
    //! Airport name

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

    //! ICAO code
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
    //! Comparison operator
    bool operator<(const CAirportKey& ref2nd) const
    {
      if (GetName() != ref2nd.GetName())
        return GetName() < ref2nd.GetName();

      if (IsICAO() && ref2nd.IsICAO() && GetICAO() != ref2nd.GetICAO())
        return GetICAO() < ref2nd.GetICAO();
      return false;
    }
  private:
    void Init()
    {
      m_bName = false;
      m_bICAO = false;
    }
    QString m_Name;
    bool m_bName;
    QString m_ICAO;
    bool m_bICAO;

};

#endif // AIRPORT_H
