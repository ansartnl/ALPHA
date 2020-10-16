#include "OLDIMesLAM.h"

OLDIMesLAM::OLDIMesLAM(const OLDIMesDataLAM& pData, QObject* parent)
    : OLDIMessage(OLDIMesData(), parent)
{
    OLDIMesData& mesData = GetMesSourceData();
	mesData.bAnswer = false;
    mesData.sType = oldi::MES_LAM;
    mesData.iMesNum = pData.iMesNum;
    mesData.sFrom = pData.sFrom;
    mesData.sTo = pData.sTo;
    mesData.sDataPart = pData.sAnwFrom + "/" + pData.sAnwTo + QString("%1").arg(pData.iAnwMesNum, 3, 10, QLatin1Char('0'));

    // (LAMFDP/CWPI015CWPI/FDP014)
    mesData.sOLDIMes = QString("(%1%2/%3%4%5)").arg(mesData.sType).arg(mesData.sFrom)
                    .arg(mesData.sTo).arg(mesData.iMesNum, 3, 10, QLatin1Char('0'))
                    .arg(mesData.sDataPart);
}

OLDIMesLAM::OLDIMesLAM(const OLDIMesData& pData, QObject* parent)
    : OLDIMessage(pData, parent)
{

}

OLDIMesLAM::OLDIMesLAM(const QString& sData, QObject* parent)
    : OLDIMessage(sData, parent)
{

}

OLDIMesLAM::~OLDIMesLAM()
{
}

bool OLDIMesLAM::isAnswer(const OLDIMessage* const pMesLAM)
{
    return false;
}

bool OLDIMesLAM::isExecute()
{
    return true;
}

OLDIMesDataLAM OLDIMesLAM::GetDataLAM() const
{
    OLDIMesData data = GetMesData();
    OLDIMesDataLAM dataLAM;
    dataLAM.iMesNum = data.iMesNum;
    dataLAM.sFrom = data.sFrom;
    dataLAM.sTo = data.sTo;
    QString sTempMes = "(LAM" + data.sDataPart + ")";
    OLDIMesData tempData = ParseHeader(sTempMes);
    dataLAM.iAnwMesNum = tempData.iMesNum;
    dataLAM.sAnwFrom = tempData.sFrom;
    dataLAM.sAnwTo = tempData.sTo;
    return dataLAM;
}
