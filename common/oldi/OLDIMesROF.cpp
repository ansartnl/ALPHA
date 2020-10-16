#include "OLDIMesROF.h"

OLDIMesROF::OLDIMesROF(const OLDIMesDataROF& pData, QObject* parent)
    : OLDIMessage(OLDIMesData(), parent)
{
    OLDIMesData& mesData = GetMesSourceData();
    mesData.sType = oldi::MES_ROF;
    mesData.iMesNum = pData.iMesNum;
    mesData.sFrom = pData.sFrom;
    mesData.sTo = pData.sTo;
    mesData.sDataPart = "-" + pData.sACID;

    // (ROFE/L001-AMM253)
    mesData.sOLDIMes = QString("(%1%2/%3%4%5)").arg(mesData.sType).arg(mesData.sFrom)
                    .arg(mesData.sTo).arg(mesData.iMesNum, 3, 10, QLatin1Char('0'))
                    .arg(mesData.sDataPart);
}

OLDIMesROF::OLDIMesROF(const QString& sData, QObject* parent)
    : OLDIMessage(sData, parent)
{

}

OLDIMesROF::~OLDIMesROF()
{
}

OLDIMesDataROF OLDIMesROF::GetDataROF() const
{
    OLDIMesData data = GetMesData();
    OLDIMesDataROF dataROF;
    dataROF.iMesNum = data.iMesNum;
    dataROF.sFrom = data.sFrom;
    dataROF.sTo = data.sTo;
    QStringList sData = data.sDataPart.split("-", QString::SkipEmptyParts);
    if(sData.size() >= 1)
    {
        dataROF.sACID = sData[0];
    }
    return dataROF;
}
