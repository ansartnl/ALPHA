#include "OLDIMesCOF.h"

OLDIMesCOF::OLDIMesCOF(const OLDIMesDataCOF& pData, QObject* parent)
    : OLDIMessage(OLDIMesData(), parent)
{
    OLDIMesData& mesData = GetMesSourceData();
    mesData.sType = oldi::MES_COF;
    mesData.iMesNum = pData.iMesNum;
    mesData.sFrom = pData.sFrom;
    mesData.sTo = pData.sTo;
    mesData.sDataPart = "-" + pData.sACID;

    // (COFE/L001-AMM253)
    mesData.sOLDIMes = QString("(%1%2/%3%4%5)").arg(mesData.sType).arg(mesData.sFrom)
                    .arg(mesData.sTo).arg(mesData.iMesNum, 3, 10, QLatin1Char('0'))
                    .arg(mesData.sDataPart);
}

OLDIMesCOF::OLDIMesCOF(const QString& sData, QObject* parent)
    : OLDIMessage(sData, parent)
{

}

OLDIMesCOF::~OLDIMesCOF()
{
}

OLDIMesDataCOF OLDIMesCOF::GetDataCOF() const
{
    OLDIMesData data = GetMesData();
    OLDIMesDataCOF dataCOF;
    dataCOF.iMesNum = data.iMesNum;
    dataCOF.sFrom = data.sFrom;
    dataCOF.sTo = data.sTo;
    QStringList sData = data.sDataPart.split("-", QString::SkipEmptyParts);
    if(sData.size() >= 1)
    {
        dataCOF.sACID = sData[0];
    }
    return dataCOF;
}
