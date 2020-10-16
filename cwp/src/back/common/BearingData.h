#ifndef BEARINGDATA_H
#define BEARINGDATA_H

#include <QtCore/QtGlobal>

class CBearingData
{
public:
    CBearingData(): m_ID(0), m_SAC(0), m_SIC(0), m_Bearing(0), m_Declination(0)
    {
    }

    // Channel/Frequency
    quint32 GetID() const { return m_ID; }
    void SetID(quint32 V) { m_ID = V; }

    quint8 GetSAC() const { return m_SAC; }
    void SetSAC(quint8 V) { m_SAC = V; }

    quint8 GetSIC() const { return m_SIC; }
    void SetSIC(quint8 V) { m_SIC = V; }

    double GetBearing() const { return m_Bearing; }
    void SetBearing(double V) { m_Bearing = V; }

    double GetDeclination() const { return m_Declination; }
    void SetDeclination(double V) { m_Declination = V; }

private:
    quint32 m_ID;
    quint8 m_SAC;
    quint8 m_SIC;
    double m_Bearing;
    double m_Declination;
};

#endif // BEARINGDATA_H
