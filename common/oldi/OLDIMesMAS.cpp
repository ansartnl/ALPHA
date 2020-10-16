#include "OLDIMesMAS.h"

OLDIMesMAS::OLDIMesMAS(const OLDIMesDataMAS& pData, QObject* parent)
    : OLDIMessage(OLDIMesData(), parent)
{
    OLDIMesData& mesData = GetMesSourceData();
    mesData.sType = oldi::MES_MAS;
    mesData.iMesNum = pData.iMesNum;
    mesData.sFrom = pData.sFrom;
    mesData.sTo = pData.sTo;
    mesData.sDataPart = "-" + pData.sACID;

    // (MASE/L001-AMM253)
    mesData.sOLDIMes = QString("(%1%2/%3%4%5)").arg(mesData.sType).arg(mesData.sFrom)
                    .arg(mesData.sTo).arg(mesData.iMesNum, 3, 10, QLatin1Char('0'))
                    .arg(mesData.sDataPart);
}

OLDIMesMAS::OLDIMesMAS(const QString& sData, QObject* parent)
    : OLDIMessage(sData, parent)
{

}

OLDIMesMAS::~OLDIMesMAS()
{
}

OLDIMesDataMAS OLDIMesMAS::GetDataMAS() const
{
    OLDIMesData data = GetMesData();
    OLDIMesDataMAS dataMAS;
    dataMAS.iMesNum = data.iMesNum;
    dataMAS.sFrom = data.sFrom;
    dataMAS.sTo = data.sTo;
    QStringList sData = data.sDataPart.split("-", QString::SkipEmptyParts);
    if(sData.size() >= 1)
    {
        dataMAS.sACID = sData[0];
    }
    return dataMAS;
}
