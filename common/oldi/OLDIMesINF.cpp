#include "OLDIMesINF.h"

OLDIMesINF::OLDIMesINF(const OLDIMesDataINF& pData, QObject* parent)
    : OLDIMessage(OLDIMesData(), parent)
{
    OLDIMesData& mesData = GetMesSourceData();
    mesData.sType = oldi::MES_INF;
    mesData.iMesNum = pData.iMesNum;
    mesData.sFrom = pData.sFrom;
    mesData.sTo = pData.sTo;
    mesData.sDataPart = QString("-%1-%2-%3-%4-%5-%6").arg(pData.sTypeINF).arg(pData.sACID)
                        .arg(pData.sADEP).arg(pData.sADES).arg(pData.sCODE).arg(pData.sSector);

    // (INFE/L001-ASSUME-AMM253-LMML-EGBB-1700-S1)
    mesData.sOLDIMes = QString("(%1%2/%3%4%5)").arg(mesData.sType).arg(mesData.sFrom)
                    .arg(mesData.sTo).arg(mesData.iMesNum, 3, 10, QLatin1Char('0'))
                    .arg(mesData.sDataPart);
}

OLDIMesINF::OLDIMesINF(const QString& sData, QObject* parent)
    : OLDIMessage(sData, parent)
{

}

OLDIMesINF::~OLDIMesINF()
{
}

OLDIMesDataINF OLDIMesINF::GetDataINF() const
{
    OLDIMesData data = GetMesData();
    OLDIMesDataINF dataINF;
    dataINF.iMesNum = data.iMesNum;
    dataINF.sFrom = data.sFrom;
    dataINF.sTo = data.sTo;
    QStringList sData = data.sDataPart.split("-", QString::SkipEmptyParts);
    if(sData.size() >= 6)
    {
        dataINF.sTypeINF = sData[0];
        dataINF.sACID = sData[1];
        dataINF.sADEP = sData[2];
        dataINF.sADES = sData[3];
        dataINF.sCODE = sData[4];
        dataINF.sSector = sData[5];
        if(dataINF.sSector == " ")
            dataINF.sSector = "";
    }
    return dataINF;
}
