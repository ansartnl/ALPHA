#include "OLDIMesCOD.h"

OLDIMesCOD::OLDIMesCOD(const OLDIMesDataCOD& pData, QObject* parent)
    : OLDIMessage(OLDIMesData(), parent)
{
    OLDIMesData& mesData = GetMesSourceData();
    mesData.sType = oldi::MES_COD;
    mesData.iMesNum = pData.iMesNum;
    mesData.sFrom = pData.sFrom;
    mesData.sTo = pData.sTo;
    mesData.sDataPart = QString("-%1/%2-%3-%4").arg(pData.sACID).arg(pData.sCODE)
                        .arg(pData.sADEP).arg(pData.sADES);

    // (CODP/PO011-AAL905/A0767-LFPO-KEWR)
    mesData.sOLDIMes = QString("(%1%2/%3%4%5)").arg(mesData.sType).arg(mesData.sFrom)
                    .arg(mesData.sTo).arg(mesData.iMesNum, 3, 10, QLatin1Char('0'))
                    .arg(mesData.sDataPart);
}

OLDIMesCOD::OLDIMesCOD(const QString& sData, QObject* parent)
    : OLDIMessage(sData, parent)
{

}

OLDIMesCOD::~OLDIMesCOD()
{
}

OLDIMesDataCOD OLDIMesCOD::GetDataCOD() const
{
    OLDIMesData data = GetMesData();
    OLDIMesDataCOD dataCOD;
    dataCOD.iMesNum = data.iMesNum;
    dataCOD.sFrom = data.sFrom;
    dataCOD.sTo = data.sTo;
    QStringList sData = data.sDataPart.split("-", QString::SkipEmptyParts);
    if(sData.size() >= 3)
    {
        dataCOD.sADEP = sData[1];
        dataCOD.sADES = sData[2];
        QStringList sTempList = sData[0].split("/", QString::SkipEmptyParts);
        if(sTempList.size() >= 2)
        {
            dataCOD.sACID = sTempList[0];
            dataCOD.sCODE = sTempList[1];
        }
    }
    return dataCOD;
}
