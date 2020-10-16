#ifndef ZONE_H
#define ZONE_H

#include <QtCore/QString>
#include <QtCore/QSharedPointer>

class CBoundaryPoint;
class CCoord;

//! Class contains zone(FIR or sector) information
class Zone
{
  public:
    //! List of boundary points
    typedef QList<QSharedPointer<CBoundaryPoint> > TBoundary;
    //! List of coordinate
    typedef QList<CCoord> TCoordList;

    //! Zone types
    enum Type
    {
      enFIR = 1, //!< FIR
        enSector
    //!< sector
    };

    //! Constructor
    Zone();
    //! Destructor
    virtual ~Zone();

    //! Set zone name
    /*!
     \param qsName zone name
     */
    void SetName(const QString& qsName);
    //! Get zome name
    /*!
     \return  zone name
     */
    const QString& GetName() const;
    //! Set unique zone name
    /*!
     \param qsIDName unique zone name
     */
    void SetIDName(const QString& qsIDName);
    //! Get unique zone name
    /*!
     \return unique zone name
     */
    const QString& GetIDName() const;
    //! Set zone type
    /*!
     \param iType zone type
     */
    void SetType(int iType);
    //! Get zone type
    /*!
     \return zone type
     */
    int GetType() const;
    //! Set zone identifier
    /*!
     \param uiID zone identifier
     */
    void SetID(unsigned int uiID);
    //! Get zone identifier
    /*!
     \return zone identifier
     */
    unsigned int GetID() const;
    //! Set lower limit
    /*!
     \param iLowLimit lower limit
     */
    void SetLowLimit(int iLowLimit);
    //! Get lower limit
    /*!
     \return lower limit
     */
    int GetLowLimit() const;
    //! Set upper limit
    /*!
     \param iUpLimit lower limit
     */
    void SetUpLimit(int iUpLimit);
    //! Get upper limit
    /*!
     \return upper limit
     */
    int GetUpLimit() const;
    //! Set parent identifier
    /*!
     \param uiID parent identifier
     */
    void SetParentID(uint uiID);
    //! Get parent identifier
    /*!
     \return parent identifier
     */
    uint GetParentID() const;

    //! Add boundary point
    /*!
     \param pPoint pointer to boundary point
     */
    virtual void AddBoundaryPoint(const QSharedPointer<CBoundaryPoint>& pPoint);

    //! Get boundary points
    /*!
     \return boundary points
     */
    const TBoundary& GetBoundary() const
    {
      return m_Boundary;
    }
    //! Get boundary points
    /*!
     \return boundary points
     */
    TBoundary& GetBoundary()
    {
      return m_Boundary;
    }
    //! Get polygon coordinate
    /*!
     \return coordinate
     */
    TCoordList& GetPolygonCoord()
    {
      return m_PolyCoord;
    }
    //! Set short name
    /*!
     \param sName short name
     */
    void SetShortName(const QString &sName);
    //! Get short name
    /*!
     \return short name
     */
    QString GetShortName() const;
    //! Set label name
    /*!
     \param sName label name
     */
    void SetLabelName(QString sName);
    //! Get label name
    /*!
     \return label name
     */
    QString GetLabelName() const;
    //! Set frequency
    /*!
     \param dFrequency frequency
     */
    void SetFrequency(double dFrequency);
    //! Get frequency
    /*!
     \return Frequency
     */
    double GetFrequency() const;

  protected:
    /*! Boundary points*/
    TBoundary m_Boundary;
    //! Polygon Coordinate
    TCoordList m_PolyCoord;

    //! Zone name
    QString m_qsName;
    //! Unique zone name
    QString m_qsIDName;
    //! Zone type
    int m_iType;
    //! Zone identifier
    unsigned int m_uiId;
    //! Lower limit
    int m_iLowLimit;
    //! Upper limit
    int m_iUpLimit;
    //! Parent identifier
    int m_uiParentID;
    //! Short name
    QString m_qsShortName;
    //! Show name
    QString m_qsLabelName;
    //! Frequency
    double m_dFrequency;
};
#endif // ZONE_H
