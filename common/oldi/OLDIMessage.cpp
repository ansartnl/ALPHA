#include "OLDIMessage.h"

#ifdef CWP_PRJ
#include "AClist/SFPL.h"
#include "back/common/FLVal.h"
#endif

OLDIMessage::OLDIMessage(const OLDIMesData& pData, QObject* parent)
    : QObject(parent), m_Data(pData), m_bExecute(false)
{
}

OLDIMessage::OLDIMessage(const QString& sData, QObject* parent)
    : QObject(parent), m_bExecute(false)
{
    m_Data = ParseHeader(sData);
}

OLDIMessage::~OLDIMessage()
{
}

bool OLDIMessage::isAnswer(const OLDIMessage* const pMesLAM)
{
    if(((oldi::MES_ERR == pMesLAM->GetMesData().sType) || (oldi::MES_LAM == pMesLAM->GetMesData().sType))
        && (pMesLAM->GetMesData().sFrom == m_Data.sTo) && (pMesLAM->GetMesData().sTo == m_Data.sFrom))
    {
        QString sTempMes = "(LAM" + pMesLAM->GetMesData().sDataPart + ")";
        OLDIMesData tempData = ParseHeader(sTempMes);
        if((tempData.iMesNum == m_Data.iMesNum)
            && (tempData.sFrom == m_Data.sFrom) && (tempData.sTo == m_Data.sTo))
        {
            QStringList sListErr = tempData.sDataPart.split("-", QString::SkipEmptyParts);
            emit messageAnswer(this, sListErr);
            return true;
        }
    }
    return false;
}

void OLDIMessage::Cancel()
{

}

bool OLDIMessage::isExecuted()
{
    return m_bExecute;
}

void OLDIMessage::TimeOut()
{
    emit messageTimeOut(this);
}

OLDIMesData OLDIMessage::ParseHeader(const QString& sMes) const
{
    OLDIMesData data;
    if(sMes.length() < 11) return data;
    data.sType = sMes.mid(1, 3);
    data.sOLDIMes = sMes;
    int indSl = sMes.indexOf("/");
    int indNum = sMes.indexOf(QRegExp("[0-9]"));
    if(indSl != -1 && indNum != -1)
    {
        data.sFrom = sMes.mid(4, indSl-4);
        data.sTo = sMes.mid(indSl+1, indNum-indSl-1);
        data.iMesNum = sMes.mid(indNum, 3).toUShort();
        data.sDataPart = sMes.mid(indNum+3);
        data.sDataPart.truncate(data.sDataPart.length()-1);
    }
    return data;
}

OLDIMesData OLDIMessage::GetLAM(quint16 iMesNumLAM) const
{
    OLDIMesData mesData;
    mesData.iMesNum = iMesNumLAM;
    mesData.bAnswer = false;
    mesData.sType = oldi::MES_LAM;
    mesData.sFrom = m_Data.sTo;
    mesData.sTo = m_Data.sFrom;
    mesData.sDataPart = m_Data.sFrom + "/" + m_Data.sTo + QString("%1").arg(m_Data.iMesNum, 3, 10, QLatin1Char('0'));

    // (LAMFDP/CWPI015CWPI/FDP014)
    mesData.sOLDIMes = QString("(%1%2/%3%4%5)").arg(mesData.sType).arg(mesData.sFrom)
                    .arg(mesData.sTo).arg(mesData.iMesNum, 3, 10, QLatin1Char('0'))
                    .arg(mesData.sDataPart);

    return mesData;
}

bool OLDIMessage::operator == (const OLDIMessage& ref2nd) const
{
    return m_Data == ref2nd.GetMesData();
}

#ifdef CWP_PRJ
OLDIMesDataABI OLDIMessage::GetABIfromSFPL(const QSharedPointer<CSFPL>& pSFPL)
{
    OLDIMesDataABI dataABI;
    if(pSFPL)
    {
        dataABI.sACID = pSFPL->GetACID();
        dataABI.sFRules = pSFPL->GetFRules();
        dataABI.sTFlight = pSFPL->GetTypeFlight();
        dataABI.iNum = pSFPL->GetNum();
        dataABI.sTPlane = pSFPL->GetAirplanePtr() ? pSFPL->GetAirplanePtr()->GetName() : "";
        dataABI.sTurbulence = pSFPL->GetTurbulence();
        dataABI.sEquipment = pSFPL->GetEquipment();
        dataABI.sADEP = pSFPL->GetDepAerodrome();
        dataABI.dtADEP = pSFPL->GetDepTime();
        dataABI.sSpeed = pSFPL->GetSpeedS();
        dataABI.sLevel = pSFPL->GetLevelS();
        dataABI.sRoute = pSFPL->GetRoute();
        dataABI.sADES = pSFPL->GetDestAerodrome();
        dataABI.str_dtADES = pSFPL->GetDestTime();
        dataABI.sAlt1 = pSFPL->GetAlt1Aerodrome();
        dataABI.sAlt2 = pSFPL->GetAlt2Aerodrome();
        dataABI.sOther = pSFPL->GetOther();
        dataABI.sFirName = pSFPL->GetFirName();

        dataABI.sCODE = QString("%1").arg(pSFPL->GetCODE(), 4, 8, QLatin1Char('0'));
        dataABI.sCOPin = pSFPL->GetCOPin();
        dataABI.dtETOin = pSFPL->GetETOin();
        if(pSFPL->GetCOPinFL())
            dataABI.sCOPinLevel = pSFPL->GetCOPinFL().toString();
    }

    return dataABI;
}

void OLDIMessage::SetABItoSFPL(const OLDIMesDataABI& dataABI, QSharedPointer<CSFPL>& pSFPL)
{
    if(pSFPL)
    {
        if(!dataABI.sACID.isEmpty()) pSFPL->SetACID(dataABI.sACID);
        if(!dataABI.sFRules.isEmpty()) pSFPL->SetFRules(dataABI.sFRules);
        if(!dataABI.sTFlight.isEmpty()) pSFPL->SetTypeFlight(dataABI.sTFlight);
        if(dataABI.iNum) pSFPL->SetNum(dataABI.iNum);
        if(!dataABI.sTPlane.isEmpty())
        {
            if(pSFPL->GetAirplanePtr())
            {
                pSFPL->GetAirplanePtr()->SetName(dataABI.sTPlane);
            }
            else
            {
                QSharedPointer<CAirplaneType> pPlaneType(new CAirplaneType);
                pPlaneType->SetName(dataABI.sTPlane);
                pSFPL->SetAirplanePtr(pPlaneType);
            }
        }
        if(!dataABI.sTurbulence.isEmpty()) pSFPL->SetTurbulence(dataABI.sTurbulence);
        if(!dataABI.sEquipment.isEmpty()) pSFPL->SetEquipment(dataABI.sEquipment);
        if(!dataABI.sADEP.isEmpty()) pSFPL->SetDepAerodrome(dataABI.sADEP);
        if(!dataABI.dtADEP.isNull()) pSFPL->SetDepTime(dataABI.dtADEP);
        if(!dataABI.sSpeed.isEmpty()) pSFPL->SetSpeed(dataABI.sSpeed);
        if(!dataABI.sLevel.isEmpty()) pSFPL->SetLevel(dataABI.sLevel);
        if(!dataABI.sRoute.isEmpty()) pSFPL->SetRoute(dataABI.sRoute);
        if(!dataABI.sADES.isEmpty()) pSFPL->SetDestAerodrome(dataABI.sADES);
        if(!dataABI.str_dtADES.isEmpty()) pSFPL->SetDestTime(dataABI.str_dtADES);
        if(!dataABI.sAlt1.isEmpty()) pSFPL->SetAlt1Aerodrome(dataABI.sAlt1);
        if(!dataABI.sAlt2.isEmpty()) pSFPL->SetAlt2Aerodrome(dataABI.sAlt2);
        if(!dataABI.sOther.isEmpty()) pSFPL->SetOther(dataABI.sOther);
        if(!dataABI.sFirName.isEmpty()) pSFPL->SetFirName(dataABI.sFirName);

        if(!dataABI.sCODE.isEmpty()) pSFPL->SetCODE(dataABI.sCODE.toUInt(0, 8));
        if(!dataABI.sCOPin.isEmpty()) pSFPL->SetCOPin(dataABI.sCOPin);
        if(!dataABI.dtETOin.isNull()) pSFPL->SetETOin(dataABI.dtETOin);
        if(!dataABI.sCOPinLevel.isEmpty())
        {
            pSFPL->SetCOPinFL(FLVal(dataABI.sCOPinLevel));
        }
    }
}
#endif
