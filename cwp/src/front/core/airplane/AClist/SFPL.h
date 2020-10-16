#ifndef SFPL_H
#define SFPL_H

#include "RouteFragmentation.h"
#include "back/common/FLVal.h"
#include "SimulatorTrack.h"

#include <QtCore/QTime>
#include <QtCore/QSharedPointer>
#include <QtCore/QMetaType>

//! Class describes a SFPL
class CSFPL : public CFPL
{
public:
	//! Constructor
	CSFPL()
        : m_CODE(0), m_isCOPin(false), m_isCOPout(false) {};

    CSFPL(const CFPL& fpl)
        : CFPL(fpl), m_isCOPin(false), m_isCOPout(false) {};

	//! CODE ( Squawk )
	
    const quint32 &GetCODE() const { return m_CODE; }
    void SetCODE(const quint32 &V) { m_CODE = V; }
    QString GetCODEtoString() const
    { return QString("%1").arg(GetCODE(), 4, 8, QLatin1Char('0')); }
	
    const QString &GetCOPin() const { return m_COPin; }
    void SetCOPin(const QString &V) { m_COPin = V; }

    const QSharedPointer<CPointFrag> &GetCOPinPoint() const { return m_pCOPin; }
    void SetCOPinPoint(const QSharedPointer<CPointFrag> &V) { m_pCOPin = V; }

    const QTime &GetETOin() const { return m_ETOin; }
    void SetETOin(const QTime &V) { m_ETOin = V; }

    const FLVal &GetCOPinFL() const { return m_COPinFL; }
    void SetCOPinFL(const FLVal &V) { m_COPinFL = V; }

    const bool &GetIsCOPin() const  { return m_isCOPin; }
    void SetIsCOPin(const bool V)   { m_isCOPin = V; }

    const QString &GetCOPout() const { return m_COPout; }
    void SetCOPout(const QString &V) { m_COPout = V; }

    const QSharedPointer<CPointFrag> &GetCOPoutPoint() const { return m_pCOPout; }
    void SetCOPoutPoint(const QSharedPointer<CPointFrag> &V) { m_pCOPout = V; }

    const QTime &GetETOout() const { return m_ETOout; }
    void SetETOout(const QTime &V) { m_ETOout = V; }

    const bool &GetIsCOPout() const  { return m_isCOPout; }
    void SetIsCOPout(const bool V)   { m_isCOPout = V; }

    const QString &GetAllSectors() const { return m_AllSectors; }
    void SetAllSectors(const QString &V) { m_AllSectors = V; }

    const QString &GetInSector() const { return m_InSector; }
    void SetInSector(const QString &V) { m_InSector = V; }

    const QString &GetCurSector() const { return m_CurSector; }
    void SetCurSector(const QString &V) { m_CurSector = V; }

    const QString &GetPassedSector() const { return m_PassedSector; }
    void SetPassedSector(const QString &V) { m_PassedSector = V; }

    const QString &GetNextSector() const { return m_NextSector; }
    void SetNextSector(const QString &V) { m_NextSector = V; }

    const QSharedPointer<CFPLFrag>& GetFPLFrag() const {return m_FPLFrag;}
    void SetFPLFrag(const QSharedPointer<CFPLFrag>& FPLFrag) {m_FPLFrag = FPLFrag;}

    const FLVal &GetCFL() const { return m_CFL; }
    void SetCFL(const FLVal &V) { m_CFL = V; }

    const FLVal &GetTFL() const { return m_TFL; }
    void SetTFL(const FLVal &V) { m_TFL = V; }

    const FLVal &GetRFL() const { return m_RFL; }
    void SetRFL(const FLVal &V) { m_RFL = V; }

    const QString &GetNextPoint() const { return m_NextPoint; }
    void SetNextPoint(const QString &V) { m_NextPoint = V; }

    const QString &GetHdg() const { return m_Hdg; }
    void SetHdg(const QString &V) { m_Hdg = V; }

    const QString &GetSpd() const { return m_Spd; }
    void SetSpd(const QString &V) { m_Spd = V; }

    const QString &GetVrc() const { return m_Vrc; }
    void SetVrc(const QString &V) { m_Vrc = V; }

    const QString &GetStatus() const { return m_Status; }
    void SetStatus(const QString &V) { m_Status = V; }

    const QString &GetLit() const { return m_Lit; }
    void SetLit(const QString &V) { m_Lit = V; }

    const QString &GetFPLEqf1() const { return m_FPLEqf1; }
    void SetFPLEqf1(const QString &V) { m_FPLEqf1 = V; }

    const QString &GetFPLEqf2() const { return m_FPLEqf2; }
    void SetFPLEqf2(const QString &V) { m_FPLEqf2 = V; }

    const QTime &GetETD() const { return m_ETD; }
    void SetETD(const QTime &V) { m_ETD = V; }

    const QTime &GetETA() const { return m_ETA; }
    void SetETA(const QTime &V) { m_ETA = V; }

    const QTime &GetATA() const { return m_ATA; }
    void SetATA(const QTime &V) { m_ATA = V; }

    const QString &GetFType() const { return m_FType; }
    void SetFType(const QString &V) { m_FType = V; }

    const QString &GetMesType() const { return m_MesType; }
    void SetMesType(const QString &V) { m_MesType = V; }

    const QVariantMap &GetFPLFragState() const { return m_FPLFragState; }

    const QList<QPair<QString, QString> > &GetConflicts() const { return m_Conflicts; }

    //! Track generated by Simulator
    TVA_PROPERTIE(QSharedPointer<SimulatorTrack>, SimTrack)

public:
    virtual QVariantMap toVariantMap() const;
    virtual void fromVariantMap(const QVariantMap& map);
    void CreateFPLFrag(const QSharedPointer<CFPL>& pFPL);

private:
    quint32 m_CODE;
    QString m_COPin;
    QTime   m_ETOin;
    bool    m_isCOPin;
    FLVal   m_COPinFL;
    QSharedPointer<CPointFrag>  m_pCOPin;
    QString m_COPout;
    QTime   m_ETOout;
    bool    m_isCOPout;
    QSharedPointer<CPointFrag>  m_pCOPout;
    QString m_AllSectors;
    QString m_InSector;
    QString m_CurSector;
    QString m_PassedSector;
    QString m_NextSector;
    QSharedPointer<CFPLFrag> m_FPLFrag;

    FLVal   m_CFL;
    FLVal   m_TFL;
    FLVal   m_RFL;
    QString m_NextPoint;
    QString m_Hdg;
    QString m_Spd;
    QString m_Vrc;
    QString m_Status;
    QString m_Lit;
    QString m_FPLEqf1;
    QString m_FPLEqf2;
    QTime m_ETD;
    QTime m_ETA;
    QTime m_ATA;
    QString m_FType;
    QString m_MesType;

    QVariantMap m_FPLFragState;
    QList<QPair<QString, QString> > m_Conflicts;
};

Q_DECLARE_METATYPE(QSharedPointer<CSFPL>)

#endif // SFPL_H
