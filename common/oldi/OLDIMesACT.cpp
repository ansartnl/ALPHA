#include "OLDIMesACT.h"

OLDIMesACT::OLDIMesACT(const OLDIMesDataABI& pData, QObject* parent)
    : OLDIMessage(OLDIMesData(), parent)
{
    OLDIMesData& mesData = GetMesSourceData();
    mesData.sType = oldi::MES_ACT;
    mesData.iMesNum = pData.iMesNum;
    mesData.sFrom = pData.sFrom;
    mesData.sTo = pData.sTo;
    mesData.sDataPart = QString("-%1/A%2-%3-%4/%5%6-%7-9/%8%9/%10-80/%11-81/%12-15/%13-8/%14-18/%15")
                        .arg(pData.sACID).arg(pData.sCODE)
                        .arg(pData.sADEP)
                        .arg(pData.sCOPin).arg(pData.dtETOin.toString("hhmm")).arg(pData.sCOPinLevel)
                        .arg(pData.sADES)
                        .arg(pData.iNum, 2, 10, QLatin1Char('0')).arg(pData.sTPlane).arg(pData.sTurbulence)
                        .arg(pData.sTFlight)
                        .arg(pData.sEquipment)
                        .arg(pData.sRoute)
                        .arg(pData.sFRules)
                        .arg(pData.sOther);

    // (ACTE/L005-AMM253/A7012-LMML-BNE/1226F350-EGBB-9/B757/M-15/N0480F390 UB4 BNE UB4 BPK UB3 HON-80/N-81/W/EQ Y/NO)
    mesData.sOLDIMes = QString("(%1%2/%3%4%5)").arg(mesData.sType).arg(mesData.sFrom)
                    .arg(mesData.sTo).arg(mesData.iMesNum, 3, 10, QLatin1Char('0'))
                    .arg(mesData.sDataPart);
}

OLDIMesACT::OLDIMesACT(const QString& sData, QObject* parent)
    : OLDIMessage(sData, parent)
{

}

OLDIMesACT::~OLDIMesACT()
{
}

OLDIMesDataABI OLDIMesACT::GetDataACT() const
{
    OLDIMesData data = GetMesData();
    OLDIMesDataABI dataACT;
    dataACT.iMesNum = data.iMesNum;
    dataACT.sFrom = data.sFrom;
    dataACT.sTo = data.sTo;
    QStringList sData = data.sDataPart.split("-", QString::KeepEmptyParts);
    if(sData.size() >= 11)
    {
        QStringList sTempList = sData[1].split("/", QString::SkipEmptyParts);
        if(sTempList.size() >= 2)
        {
            dataACT.sACID = sTempList[0];
            dataACT.sCODE = sTempList[1].mid(1);
        }
        dataACT.sADEP = sData[2];
        sTempList = sData[3].split("/", QString::SkipEmptyParts);
        if(sTempList.size() >= 2)
        {
            dataACT.sCOPin = sTempList[0];
            dataACT.dtETOin = QTime::fromString(sTempList[1].left(4), "hhmm");
            dataACT.sCOPinLevel = sTempList[1].mid(4);
        }
        dataACT.sADES = sData[4];
        sTempList = sData[5].split("/", QString::SkipEmptyParts);
        if(sTempList.size() >= 3)
        {
            QString sNine = sTempList[0];
            if(sNine == "9")
            {
                dataACT.iNum = sTempList[1].left(2).toInt();
                dataACT.sTPlane = sTempList[1].mid(2);
                dataACT.sTurbulence = sTempList[2];
            }
        }
        sTempList = sData[6].split("/", QString::SkipEmptyParts);
        if(sTempList.size() >= 2)
        {
            QString sEighty = sTempList[0];
            if(sEighty == "80")
                dataACT.sTFlight = sTempList[1];
        }
        sTempList = sData[7].split("/", QString::SkipEmptyParts);
        if(sTempList.size() >= 2)
        {
            QString sEightyOne = sTempList[0];
            if(sEightyOne == "81")
            {
                for(int i = 1; i < sTempList.size(); ++i)
                {
                    if(i > 1)
                        dataACT.sEquipment += "/";
                    dataACT.sEquipment += sTempList[i];
                }
            }
        }
        sTempList = sData[8].split("/", QString::SkipEmptyParts);
        if(sTempList.size() >= 2)
        {
            QString sFifteen = sTempList[0];
            if(sFifteen == "15")
                dataACT.sRoute = sTempList[1];
        }
        sTempList = sData[9].split("/", QString::SkipEmptyParts);
        if(sTempList.size() >= 2)
        {
            QString sEight = sTempList[0];
            if(sEight == "8")
                dataACT.sFRules = sTempList[1];
        }
        sTempList = sData[10].split("/", QString::SkipEmptyParts);
        if(sTempList.size() >= 2)
        {
            QString sEighteen = sTempList[0];
            if(sEighteen == "18")
                dataACT.sOther = sData[10].mid(3);//sTempList[1];
        }
    }
    return dataACT;
}
