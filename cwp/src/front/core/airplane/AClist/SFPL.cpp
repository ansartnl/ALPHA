#include "SFPL.h"
#include "Master.h"
#include <QMetaType>

int metatype_id_QSharedPointer_SFPL = qRegisterMetaType<QSharedPointer<CSFPL> >("QSharedPointer<CSFPL>");

QVariantMap CSFPL::toVariantMap() const
{
    QVariantMap map = CFPL::toVariantMap();

    map.insert("CODE", m_CODE);
    map.insert("COPin", m_COPin);
    map.insert("ETOin", m_ETOin);
    map.insert("isCOPin", m_isCOPin);
    map.insert("COPinFL", m_COPinFL.toString());
    //map.insert("COPin", /*m_pCOPin*/ 0);
    map.insert("COPout", m_COPout);
    map.insert("ETOout", m_ETOout);
    map.insert("isCOPout", m_isCOPout);
    //map.insert("COPout", /*m_pCOPout*/ 0);
    if(m_AllSectors.size() == 0)
        map.insert("AllSectors", m_NextSector);
    else
        map.insert("AllSectors", m_AllSectors);
    map.insert("InSector", m_InSector);
    map.insert("CurSector", m_CurSector);
    map.insert("PassedSector", m_PassedSector);
    map.insert("NextSector", m_NextSector);
    map.insert("FPLFrag", m_FPLFrag ? m_FPLFrag->saveState() : QVariantMap());
    map.insert("CFL", m_CFL.toString());
    map.insert("TFL", m_TFL.toString());
    map.insert("RFL", m_RFL.toString());
    map.insert("NextPoint", m_NextPoint);
    map.insert("Hdg", m_Hdg);
    map.insert("Spd", m_Spd);
    map.insert("Vrc", m_Vrc);
    map.insert("Status", m_Status);
    map.insert("Lit", m_Lit);
    map.insert("FPLEqf1", m_FPLEqf1);
    map.insert("FPLEqf2", m_FPLEqf2);
    //map.insert("FPLEqf3", m_FPLEqf3);
    map.insert("ETD", m_ETD);
    map.insert("ETA", m_ETA);
    map.insert("ATA", m_ATA);
    map.insert("FType", m_FType);
    return map;
}

void CSFPL::fromVariantMap(const QVariantMap& map)
{
    CFPL::fromVariantMap(map);

    m_CODE       = map.value("CODE", m_CODE).toUInt();
    m_COPin      = map.value("COPin", m_COPin).toString();
    m_ETOin      = map.value("ETOin", m_ETOin).toTime();
    m_isCOPin    = map.value("isCOPin", m_isCOPin).toBool();
    m_COPinFL    = FLVal(map.value("COPinFL", m_COPinFL.toString()).toString());
//    m_pCOPin;
    m_COPout     = map.value("COPout", m_COPout).toString();
    m_ETOout     = map.value("ETOout", m_ETOout).toTime();
    m_isCOPout   = map.value("isCOPout", m_isCOPout).toBool();
//    m_pCOPout;
    m_AllSectors = map.value("AllSectors", m_AllSectors).toString();
    m_InSector   = map.value("InSector", m_InSector).toString();
    m_CurSector  = map.value("CurSector", m_CurSector).toString();
    m_PassedSector  = map.value("PassedSector", m_PassedSector).toString();
    m_NextSector = map.value("NextSector", m_NextSector).toString();
//    m_FPLFrag;
    m_CFL        = FLVal(map.value("CFL", m_CFL.toString()).toString());
    m_TFL        = FLVal(map.value("TFL", m_TFL.toString()).toString());
    m_RFL        = FLVal(map.value("RFL", m_RFL.toString()).toString());
    m_NextPoint  = map.value("NextPoint", m_NextPoint).toString();
    m_Hdg        = map.value("Hdg", m_Hdg).toString();
    m_Spd        = map.value("Spd", m_Spd).toString();
    m_Vrc        = map.value("Vrc", m_Vrc).toString();
    m_Status     = map.value("Status", m_Status).toString();
    m_Lit        = map.value("Lit", m_Lit).toString();
    m_FPLEqf1    = map.value("FPLEqf1", m_FPLEqf1).toString();
    m_FPLEqf2    = map.value("FPLEqf2", m_FPLEqf2).toString();
    //m_FPLEqf3    = map.value("FPLEqf3", m_FPLEqf3).toString();
    m_ETD        = map.value("ETD", m_ETD).toTime();
    m_ETA        = map.value("ETA", m_ETA).toTime();
    m_ATA        = map.value("ATA", m_ATA).toTime();
    m_FType      = map.value("FType", m_FType).toString();

    m_FPLFragState = map.value("FPLFrag", m_FPLFragState).toMap();

    m_Conflicts.clear();
    QVariantList conflictsList = map.value("Asmgcs_conflicts").toList();
    for (int i = 0; i < conflictsList.size(); ++i)
    {
        QVariantMap item = conflictsList[i].toMap();

        m_Conflicts.append(qMakePair(item.value("type").toString(),
                                     item.value("message").toString()));
    }
}

void CSFPL::CreateFPLFrag(const QSharedPointer<CFPL> &pFPL)
{
    QSharedPointer<CView> pView = qSharedPointerDynamicCast<CView>(master()->GetView());
    if (pView)
    {
        m_FPLFrag = CFPLFrag::FragmentateFPL(pFPL, pView);
        m_FPLFrag->SetFPL(pFPL);
    }
}
