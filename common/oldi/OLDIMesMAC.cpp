#include "OLDIMesMAC.h"

OLDIMesMAC::OLDIMesMAC(const OLDIMesDataMAC& pData, QObject* parent)
    : OLDIMessage(OLDIMesData(), parent)
{
    OLDIMesData& mesData = GetMesSourceData();
    mesData.sType = oldi::MES_MAC;
    mesData.iMesNum = pData.iMesNum;
    mesData.sFrom = pData.sFrom;
    mesData.sTo = pData.sTo;
    mesData.sDataPart = "-" + pData.sACID;

    // (MACE/L001-AMM253)
    mesData.sOLDIMes = QString("(%1%2/%3%4%5)").arg(mesData.sType).arg(mesData.sFrom)
                    .arg(mesData.sTo).arg(mesData.iMesNum, 3, 10, QLatin1Char('0'))
                    .arg(mesData.sDataPart);
}

OLDIMesMAC::OLDIMesMAC(const QString& sData, QObject* parent)
    : OLDIMessage(sData, parent)
{

}

OLDIMesMAC::~OLDIMesMAC()
{
}

OLDIMesDataMAC OLDIMesMAC::GetDataMAC() const
{
    OLDIMesData data = GetMesData();
    OLDIMesDataMAC dataMAC;
    dataMAC.iMesNum = data.iMesNum;
    dataMAC.sFrom = data.sFrom;
    dataMAC.sTo = data.sTo;
    QStringList sData = data.sDataPart.split("-", QString::SkipEmptyParts);
    if(sData.size() >= 1)
    {
        dataMAC.sACID = sData[0];
    }
    return dataMAC;
}
