#include "OLDIMesFPL.h"

OLDIMesFPL::OLDIMesFPL(const OLDIMesDataABI& pData, QObject* parent)
    : OLDIMessage(OLDIMesData(), parent)
{
    OLDIMesData& mesData = GetMesSourceData();
    mesData.sType = oldi::MES_FPL;
    mesData.iMesNum = pData.iMesNum;
    mesData.sFrom = pData.sFrom;
    mesData.sTo = pData.sTo;
    mesData.sDataPart = QString("-%1/%2-%3%4-%5/%6-%7%8%9%10-9/%11%12/%13-80/%14-81/%15-15/%16-8/%17-18/%18")
                        .arg(pData.sACID).arg(pData.sFirName)
                        .arg(pData.sADEP).arg(pData.dtADEP.toString("yyyyMMddhhmm"))
                        .arg(pData.sSpeed).arg(pData.sLevel)
                        .arg(pData.sADES).arg(QTime::fromString(pData.str_dtADES, "hh:mm").toString("hhmm")).arg(pData.sAlt1.isEmpty() ? "" : " "+pData.sAlt1).arg(pData.sAlt2.isEmpty() ? "" : " "+pData.sAlt2)
                        .arg(pData.iNum, 2, 10, QLatin1Char('0')).arg(pData.sTPlane).arg(pData.sTurbulence)
                        .arg(pData.sTFlight)
                        .arg(pData.sEquipment)
                        .arg(pData.sRoute)
                        .arg(pData.sFRules)
                        .arg(pData.sOther);

    // (FPLE/L001-AMM253/UMMV-LMML201103251223-K300/F350-EGBB0755 UMMS-9/B757/M-15/N0480F390 UB4 BNE UB4 BPK UB3 HON-80/N-81/W/EQ Y/NO)
    mesData.sOLDIMes = QString("(%1%2/%3%4%5)").arg(mesData.sType).arg(mesData.sFrom)
                    .arg(mesData.sTo).arg(mesData.iMesNum, 3, 10, QLatin1Char('0'))
                    .arg(mesData.sDataPart);
}

OLDIMesFPL::OLDIMesFPL(const QString& sData, QObject* parent)
    : OLDIMessage(sData, parent)
{

}

OLDIMesFPL::~OLDIMesFPL()
{
}

OLDIMesDataABI OLDIMesFPL::GetDataFPL() const
{
    OLDIMesData data = GetMesData();
    OLDIMesDataABI dataFPL;
    dataFPL.iMesNum = data.iMesNum;
    dataFPL.sFrom = data.sFrom;
    dataFPL.sTo = data.sTo;
    QStringList sData = data.sDataPart.split("-", QString::KeepEmptyParts);
    if(sData.size() >= 11)
    {
        QStringList sTempList = sData[1].split("/", QString::SkipEmptyParts);
        if(sTempList.size() >= 2)
        {
            dataFPL.sACID = sTempList[0];
            dataFPL.sFirName = sTempList[1];
        }
        dataFPL.sADEP = sData[2].left(4);
        dataFPL.dtADEP = QDateTime::fromString(sData[2].mid(4), "yyyyMMddhhmm");
        sTempList = sData[3].split("/", QString::SkipEmptyParts);
        if(sTempList.size() >= 2)
        {
            dataFPL.sSpeed = sTempList[0];
            dataFPL.sLevel = sTempList[1];
        }
        sTempList = sData[4].split(" ", QString::SkipEmptyParts);
        if(sTempList.size() >= 1)
        {
            dataFPL.sADES = sTempList[0].left(4);
            dataFPL.str_dtADES = QTime::fromString(sTempList[0].mid(4), "hhmm").toString("hh:mm");
        }
        if(sTempList.size() >= 2)
            dataFPL.sAlt1 = sTempList[1];
        if(sTempList.size() >= 3)
            dataFPL.sAlt2 = sTempList[2];
        sTempList = sData[5].split("/", QString::SkipEmptyParts);
        if(sTempList.size() >= 3)
        {
            QString sNine = sTempList[0];
            if(sNine == "9")
            {
                dataFPL.iNum = sTempList[1].left(2).toInt();
                dataFPL.sTPlane = sTempList[1].mid(2);
                dataFPL.sTurbulence = sTempList[2];
            }
        }
        sTempList = sData[6].split("/", QString::SkipEmptyParts);
        if(sTempList.size() >= 2)
        {
            QString sEighty = sTempList[0];
            if(sEighty == "80")
                dataFPL.sTFlight = sTempList[1];
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
                        dataFPL.sEquipment += "/";
                    dataFPL.sEquipment += sTempList[i];
                }
            }
        }
        sTempList = sData[8].split("/", QString::SkipEmptyParts);
        if(sTempList.size() >= 2)
        {
            QString sFifteen = sTempList[0];
            if(sFifteen == "15")
                dataFPL.sRoute = sTempList[1];
        }
        sTempList = sData[9].split("/", QString::SkipEmptyParts);
        if(sTempList.size() >= 2)
        {
            QString sEight = sTempList[0];
            if(sEight == "8")
                dataFPL.sFRules = sTempList[1];
        }
        sTempList = sData[10].split("/", QString::SkipEmptyParts);
        if(sTempList.size() >= 2)
        {
            QString sEighteen = sTempList[0];
            if(sEighteen == "18")
                dataFPL.sOther = sData[10].mid(3);//sTempList[1];
        }
    }
    return dataFPL;
}
