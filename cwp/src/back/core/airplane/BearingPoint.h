#ifndef BEARING_POINT_H
#define BEARING_POINT_H

#include "QtCore/QString"
#include "utils/Coordinate.h"

//! Class contains the bearing point
class CBearingPoint
{
public:
    CBearingPoint()
        : m_ID(0), m_SAC(0), m_SIC(0), m_Channel(0) {}
    //! Primary key in DB
    quint32 GetID() const
    { return m_ID; }
    void SetID(quint32 ID)
    { m_ID = ID; }
    //! Name
    const QString& GetName() const
    { return m_Name; }
    void SetName(const QString& sName)
    { m_Name = sName; }
    //! FIR id name
    const QString& GetFIR_ID_NAME() const
    { return m_FIR_ID_NAME; }
    void SetFIR_ID_NAME(const QString& FIR_ID_NAME)
    { m_FIR_ID_NAME = FIR_ID_NAME; }
    //! Radar position
    const CCoord& GetCoord() const
    { return m_Coord; }
    CCoord& GetCoord()
    { return m_Coord; }
    void SetCoord(const CCoord& Coord)
    { m_Coord = Coord; }
    //! SAC
    quint8 GetSAC() const
    { return m_SAC; }
    void SetSAC(quint8 SAC)
    { m_SAC = SAC; }
    //! SIC
    quint8 GetSIC() const
    { return m_SIC; }
    void SetSIC(quint8 SIC)
    { m_SIC = SIC; }
    //! Channel/Frequency
    quint32 GetChannel() const
    { return m_Channel; }
    void SetChannel(quint32 Channel)
    { m_Channel = Channel; }
    quint32 GetDeclination() const
    { return m_Declination; }
    void SetDeclination(quint32 Declination)
    { m_Declination = Declination; }
    quint32 GetAirportID() const
    { return m_AirportID; }
    void SetAirportID(quint32 airportID)
    { m_AirportID = airportID; }
private:
    quint32     m_ID;
    QString     m_Name;
    QString     m_FIR_ID_NAME;
    CCoord      m_Coord;
    quint8      m_SAC;
    quint8      m_SIC;
    quint32     m_Channel;
    quint32     m_Declination;
    quint32     m_AirportID;
};

typedef QSet<quint32> TBearingIdSet;

#endif // BEARING_POINT_H
