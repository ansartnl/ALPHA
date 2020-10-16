#include "OLDIMesURZ.h"

OLDIMesURZ::OLDIMesURZ(const OLDIMesDataURZ& pData, QObject* parent)
    : OLDIMessage(OLDIMesData(), parent)
{
    OLDIMesData& mesData = GetMesSourceData();
    mesData.sType = oldi::MES_URZ;
    mesData.iMesNum = pData.iMesNum;
    mesData.sFrom = pData.sFrom;
    mesData.sTo = pData.sTo;
    mesData.sDataPart = QString("-%1-%2-%3").arg(pData.sDBType)
                        .arg(pData.sFirIdName).arg(pData.sHexData);

    // (URZE/L001-I-UMMS-HEX)
    mesData.sOLDIMes = QString("(%1%2/%3%4%5)").arg(mesData.sType).arg(mesData.sFrom)
                    .arg(mesData.sTo).arg(mesData.iMesNum, 3, 10, QLatin1Char('0'))
                    .arg(mesData.sDataPart);
}

OLDIMesURZ::OLDIMesURZ(const QString& sData, QObject* parent)
    : OLDIMessage(sData, parent)
{

}

OLDIMesURZ::~OLDIMesURZ()
{
}

OLDIMesDataURZ OLDIMesURZ::GetDataURZ() const
{
    OLDIMesData data = GetMesData();
    OLDIMesDataURZ dataURZ;
    dataURZ.iMesNum = data.iMesNum;
    dataURZ.sFrom = data.sFrom;
    dataURZ.sTo = data.sTo;
    QStringList sData = data.sDataPart.split("-", QString::KeepEmptyParts);
    if(sData.size() >= 4)
    {
        dataURZ.sDBType = sData[1];
        dataURZ.sFirIdName = sData[2];
        dataURZ.sHexData = sData[3];
    }
    return dataURZ;
}
