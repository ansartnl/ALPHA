#include "OLDIMesERR.h"

OLDIMesERR::OLDIMesERR(const OLDIMesDataERR& pData, QObject* parent)
    : OLDIMessage(OLDIMesData(), parent)
{
    OLDIMesData& mesData = GetMesSourceData();
    mesData.bAnswer = false;
    mesData.sType = oldi::MES_ERR;
    mesData.iMesNum = pData.iMesNum;
    mesData.sFrom = pData.sFrom;
    mesData.sTo = pData.sTo;
    mesData.sDataPart = pData.sAnwFrom + "/" + pData.sAnwTo + QString("%1").arg(pData.iAnwMesNum, 3, 10, QLatin1Char('0'));
    for(int i = 0; i < pData.sListError.size(); ++i)
    {
        mesData.sDataPart += QString("-%1").arg(pData.sListError[i].toUInt(), 3, 10, QLatin1Char('0'));
    }

    // (ERRFDP/CWPI013CWPI/FDP012-001-031)
    mesData.sOLDIMes = QString("(%1%2/%3%4%5)").arg(mesData.sType).arg(mesData.sFrom)
                    .arg(mesData.sTo).arg(mesData.iMesNum, 3, 10, QLatin1Char('0'))
                    .arg(mesData.sDataPart);
}

OLDIMesERR::OLDIMesERR(const QString& sData, QObject* parent)
    : OLDIMessage(sData, parent)
{

}

OLDIMesERR::~OLDIMesERR()
{
}

bool OLDIMesERR::isAnswer(const OLDIMessage* const pMesLAM)
{
    return false;
}

bool OLDIMesERR::isExecute()
{
    return true;
}

OLDIMesDataERR OLDIMesERR::GetDataERR() const
{
    OLDIMesData data = GetMesData();
    OLDIMesDataERR dataERR;
    dataERR.iMesNum = data.iMesNum;
    dataERR.sFrom = data.sFrom;
    dataERR.sTo = data.sTo;
    QStringList sData = data.sDataPart.split("-", QString::SkipEmptyParts);
    if(sData.size() >= 1)
    {
        QString sTempMes = "(LAM" + sData.at(0) + ")";
        OLDIMesData tempData = ParseHeader(sTempMes);
        dataERR.iAnwMesNum = tempData.iMesNum;
        dataERR.sAnwFrom = tempData.sFrom;
        dataERR.sAnwTo = tempData.sTo;
    }
    for(int i = 1; i < sData.size(); ++i)
        dataERR.sListError.append(sData[i]);
    return dataERR;
}
