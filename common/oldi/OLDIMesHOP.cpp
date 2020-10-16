#include "OLDIMesHOP.h"

OLDIMesHOP::OLDIMesHOP(const OLDIMesDataHOP& pData, QObject* parent)
    : OLDIMessage(OLDIMesData(), parent)
{
    OLDIMesData& mesData = GetMesSourceData();
    mesData.sType = oldi::MES_HOP;
    mesData.iMesNum = pData.iMesNum;
    mesData.sFrom = pData.sFrom;
    mesData.sTo = pData.sTo;
    mesData.sDataPart = "-" + pData.sACID;

    // (HOPE/L001-AMM253)
    mesData.sOLDIMes = QString("(%1%2/%3%4%5)").arg(mesData.sType).arg(mesData.sFrom)
                    .arg(mesData.sTo).arg(mesData.iMesNum, 3, 10, QLatin1Char('0'))
                    .arg(mesData.sDataPart);
}

OLDIMesHOP::OLDIMesHOP(const QString& sData, QObject* parent)
    : OLDIMessage(sData, parent)
{

}

OLDIMesHOP::~OLDIMesHOP()
{
}

OLDIMesDataHOP OLDIMesHOP::GetDataHOP() const
{
    OLDIMesData data = GetMesData();
    OLDIMesDataHOP dataHOP;
    dataHOP.iMesNum = data.iMesNum;
    dataHOP.sFrom = data.sFrom;
    dataHOP.sTo = data.sTo;
    QStringList sData = data.sDataPart.split("-", QString::SkipEmptyParts);
    if(sData.size() >= 1)
    {
        dataHOP.sACID = sData[0];
    }
    return dataHOP;
}
