#ifndef RUNWAY_H
#define RUNWAY_H

#include "SSA.h"
#include "ItemHandler.h"

//! Class describes the runway
class CRunway: public CSSA::IParent, public ItemHandler
{
public:
    //! Interface to CRunway
    class IParent
    {
    public:
        virtual ~IParent()
        {
        }
        virtual const CCoord& GetPlace() const = 0;
        virtual double GetAirportAlt() const = 0;
        virtual double GetDeclination() const = 0;
    };
    typedef QSharedPointer<CSSA> TSSAPtr;
    typedef QMap<CSSAKey, QSharedPointer<CSSA> > TSSA;
    CRunway() :
        m_ID(0), m_Length(0), m_Azimuth(0), m_AirportID(0), m_pParent(0)
    {
    }
    virtual ~CRunway() {}

public:
    double GetDeclination() const
    {
        return GetParent()->GetDeclination();
    }

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

public:
    const CCoord &GetCoord() const
    {
        return m_Coord;
    }
    void SetCoord(const CCoord &V)
    {
        m_Coord = V;
    }
private:
    CCoord m_Coord;

public:
    const int &GetLength() const
    {
        return m_Length;
    }
    void SetLength(const int &V)
    {
        m_Length = V;
    }
private:
    int m_Length;

public:
    const double &GetWidth() const
    {
        return m_Width;
    }
    void SetWidth(const int &V)
    {
        m_Width = V;
    }
private:
    double m_Width;

public:
    const double &GetAzimuth() const
    {
        return m_Azimuth;
    }
    void SetAzimuth(const double &V)
    {
        m_Azimuth = V;
    }
private:
    double m_Azimuth; /*!< degree */

public:
    const quint32 &GetAirportID() const
    {
        return m_AirportID;
    }
    void SetAirportID(const quint32 &V)
    {
        m_AirportID = V;
    }
private:
    quint32 m_AirportID;

public:
    const TSSA &GetSSA() const
    {
        return m_SSA;
    }
    void SetSSA(const TSSA &V)
    {
        m_SSA = V;
    }
private:
    TSSA m_SSA;

public:
    virtual void MarkItemAllActive(bool bActive);

    CCoord& GetCoord()
    {
        return m_Coord;
    }
    /*! \name Interface CSSA::IParent */
    //@{
    virtual const CCoord& GetPlace() const;
    virtual double GetRunwayAlt() const;
    virtual double GetDirection() const;
    virtual double GetRunwayLength() const;
    virtual const QString& GetRunwayName() const;
    //@}
    void SetParent(IParent* pParent)
    {
        m_pParent = pParent;
    }
    IParent* GetParent() const
    {
        return m_pParent;
    }

    TSSA::iterator AddSSA(const TSSAPtr& pSSA);
    TSSA& GetSSA()
    {
        return m_SSA;
    }

private:
    IParent* m_pParent;
};
#endif // RUNWAY_H
