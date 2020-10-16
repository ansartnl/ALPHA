#include "OLDIMesSTS.h"

OLDIMesSTS::OLDIMesSTS(const OLDIMesDataSTS& pData, QObject* parent)
    : OLDIMessage(OLDIMesData(), parent)
{
    OLDIMesData& mesData = GetMesSourceData();
    mesData.sType = oldi::MES_STS;
    mesData.iMesNum = pData.iMesNum;
    mesData.sFrom = pData.sFrom;
    mesData.sTo = pData.sTo;
    mesData.sDataPart = "-" + QString::number(pData.iPriority);
    mesData.sDataPart += pData.sText;

    // (STSCWPA/FDP012-0-CWPA-S1-S2)
    mesData.sOLDIMes = QString("(%1%2/%3%4%5)").arg(mesData.sType).arg(mesData.sFrom)
                    .arg(mesData.sTo).arg(mesData.iMesNum, 3, 10, QLatin1Char('0'))
                    .arg(mesData.sDataPart);
}

OLDIMesSTS::OLDIMesSTS(const QString& sData, QObject* parent)
    : OLDIMessage(sData, parent)
{

}

OLDIMesSTS::~OLDIMesSTS()
{
}

OLDIMesDataSTS OLDIMesSTS::GetDataSTS() const
{
    OLDIMesData data = GetMesData();
    OLDIMesDataSTS dataSTS;
    dataSTS.iMesNum = data.iMesNum;
    dataSTS.sFrom = data.sFrom;
    dataSTS.sTo = data.sTo;
    QStringList sData = data.sDataPart.split("-", QString::SkipEmptyParts);
    if(sData.size())
    {
        dataSTS.iPriority = sData[0].toUInt();
        for(int i = 1; i < sData.size(); ++i)
            dataSTS.sText += "-" + sData[i];
    }
    return dataSTS;
}
