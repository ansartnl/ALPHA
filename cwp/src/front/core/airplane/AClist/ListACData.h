#ifndef LISTACDATA_H
#define LISTACDATA_H

#include "SFPL.h"
#include "back/core/Airplane.h"
#include "front/core/airplane/fpltracer/TracerFPL.h"

//! AC warning
class CListACWarning
{
public:
	//! Constructor
	CListACWarning() : m_Level(0) {}
	
public:
    const QString &GetText() const { return m_Text; }
    void SetText(const QString &V) { m_Text = V; }
private:
    QString m_Text;
	
public:
    const int &GetLevel() const { return m_Level; }
    void SetLevel(const int &V) { m_Level = V; }
private:
    int m_Level;
};

//! Item data for lists AC
class CListACData
{
public:
    static const int GetDeltaIPS_Small(int iDelta = 0);
    static const int GetDeltaIPS_Big(int iDelta = 0);

public:
	//! Constructor
    CListACData();
	
public:
    const QSharedPointer<CAirplane> &GetAirplane() const { return m_Airplane; }
    void SetAirplane(const QSharedPointer<CAirplane> &V) { m_Airplane = V; }
private:
    QSharedPointer<CAirplane> m_Airplane;
	
public:
    const QSharedPointer<CSFPL> &GetSFPL() const { return m_SFPL; }
    void SetSFPL(const QSharedPointer<CSFPL> &V) { m_SFPL = V; }
private:
    QSharedPointer<CSFPL> m_SFPL;

public:
    const QSharedPointer<CTracerFPL> &GetTracer() const { return m_Tracer; }
    void SetTracer(const QSharedPointer<CTracerFPL> &V) { m_Tracer = V; }
private:
    QSharedPointer<CTracerFPL> m_Tracer;

public:
    const int &GetIPS_Small() const { return m_IPS_Small; }
    void SetIPS_Small(const int &V) { m_IPS_Small = V; }
private:
    int m_IPS_Small;
	
public:
    const int &GetIPS_Big() const { return m_IPS_Big; }
    void SetIPS_Big(const int &V) { m_IPS_Big = V; }
private:
    int m_IPS_Big;
	
public:
    const CListACWarning &GetWRN() const { return m_WRN; }
    void SetWRN(const CListACWarning &V) { m_WRN = V; }
private:
    CListACWarning m_WRN;

public:
    const QString &GetRVSM() const { return m_RVSM; }
    void SetRVSM(const QString &V) { m_RVSM = V; }
private:
    QString m_RVSM;

public:
	int& GetIPS_Small() { return m_IPS_Small; }
	int& GetIPS_Big() { return m_IPS_Big; }
};

Q_DECLARE_METATYPE(CListACData)

#endif // LISTACDATA_H
