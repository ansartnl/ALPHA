#include "CmdTaken.h"
#include "Common.h"
#include "Master.h"

#include <QMessageBox>

namespace obelix
{
    const char CCmdTaken::XML_CMDNAME[]        = "taken";
    const char CCmdTaken::XML_TRANSFER[]    = "transfer";
    const char CCmdTaken::XML_AIRID[]        = "airplane_id";

    const char CCmdTaken::XML_FPLID[] = "fpl_id";
    const char CCmdTaken::XML_ANSWR[] = "need_answer";

    CCmdTaken::CCmdTaken(quint32 uiAirID)
        : CXMLCmd(QL1S(XML_PROTO))
    {
        SetCmdName(QL1S(XML_CMDNAME));
        MarkACK(false);

        m_DNTransfer = GetDoc().createElement(QL1S(XML_TRANSFER));
        m_DAAirID = GetDoc().createAttribute(QL1S(XML_AIRID));
        m_DAAirID.setValue(QString::number(uiAirID));
        m_DNTransfer.setAttributeNode(m_DAAirID);
        GetMainNode().appendChild(m_DNTransfer);
    }

    CCmdTaken::CCmdTaken(const CXMLData::TXMLData& pData)
        : CXMLCmd(pData)
    {
        m_DNTransfer = GetMainNode().firstChildElement();
        TVA_CHECK(
            !m_DNTransfer.isNull() && m_DNTransfer.nodeName() == QL1S(XML_TRANSFER),
            QString(QL1S(XML_TRANSFER) + QObject::tr(" node in XML command is absent or has wrong name")),
            E_FAIL);

        m_DAAirID = m_DNTransfer.attributeNode(QL1S(XML_AIRID));
        TVA_CHECK(
            !m_DAAirID.isNull(),
            QString(QL1S(XML_AIRID) + QObject::tr(" attribute in XML command is absent or has wrong name")),
            E_FAIL);
    }

    quint32 CCmdTaken::GetAirID() const
    {
        quint32 uiID(0);
        bool bOk(false);
        uiID = m_DAAirID.value().toUInt(&bOk);
        TVA_CHECK(
            bOk,
            QString(QL1S(XML_AIRID) + QObject::tr(" attribute in XML command has wrong structure")),
            E_FAIL);
        return uiID;
    }

    QString CCmdTaken::GetValue(const QString &sAttrName,const QString& defValue)
    {
        return m_DNTransfer.attribute(sAttrName, defValue);
    }

    CCmdTakenExe::CCmdTakenExe(
        const CXMLData::TXMLData& pData,
        CMaster& refMaster,
        obelix::CConnect& refConnect)
        : CCmdTaken(pData), m_refMaster(refMaster), m_refConnect(refConnect)
    {
    }

    CXMLResp::TXMLResp CCmdTakenExe::Execute()
    {
        TVA_CATCHALL_TRY
        {
            //LOG_STR(CLog::enDebug, (QObject::tr("Execution of command") + QL1S(": %1")).arg(QL1S(XML_CMDNAME)));

            // Find airplane
            CMaster::TAirplanesMap::iterator itFindPlane = m_refMaster.GetPlanes()->find(GetAirID());
            if (itFindPlane != m_refMaster.GetPlanes()->end())
            {
                bool bOk(false);
                quint32 id = GetValue(QL1S(XML_FPLID)).toUInt(&bOk);
                TVA_CHECK(
                    bOk,
                    QString(QL1S(XML_FPLID) + QObject::tr(" attribute in XML command has wrong structure")),
                    E_FAIL);


                QList<QSharedPointer<CSFPL> > SFPLList;
                TVA_CATCHALL_TRY
                {
                    dao()->LoadSFPLs(SFPLList);
                }TVA_CATCHALL(err){
                    QMessageBox::critical(NULL, QObject::tr("SFPL"), err.reason());
                }

                QList<QSharedPointer<CSFPL> >::iterator itList = SFPLList.begin();
                for(; itList != SFPLList.end(); ++itList)
                {
                    if( (*itList)->GetID() == id)
                        break;
                }

                TVA_CHECK(
                    itList != SFPLList.end(),
                    QString("SFPL with id %1 not found!").arg(id),
                    E_FAIL);

                // send INF
                OLDIMesDataINF dataINF;
                dataINF.iMesNum = m_refMaster.GetOLDIProc()->GetNextMesNum();
                dataINF.sFrom = networkSettings()->value("oldi/name", "Unknown").toString();
                dataINF.sTo = networkSettings()->value("users/current_fdp_name", "FDP").toString();
                dataINF.sTypeINF = oldi::INF_ASSUME;
                dataINF.sACID = (*itList)->GetACID();
                dataINF.sADEP = (*itList)->GetDepAerodrome();
                dataINF.sADES = (*itList)->GetDestAerodrome();
                dataINF.sCODE = (*itList)->GetCODEtoString();
                dataINF.sSector = (*itList)->GetCurSector();
                QSharedPointer<OLDIMessage> pMesINF(new OLDIMesINF(dataINF));
                if(GetValue(QL1S(XML_ANSWR), "true") == "false")
                    pMesINF->GetMesSourceData().bAnswer = false;
                //connect(pMes.data(), SIGNAL(messageAnswer(const OLDIMessage*,const QStringList&)), this, SLOT(OLDIMesAnswer(const OLDIMessage*,const QStringList&)));
                //connect(pMes.data(), SIGNAL(messageTimeOut(const OLDIMessage*)), this, SLOT(OLDIMesTimeout(const OLDIMessage*)));
                m_refMaster.GetOLDIProc()->SendMessage(pMesINF);

                QString sSectors = networkSettings()->value("oldi/cwp_name", "CWPA,CWPB,CWPC").toString();
                QStringList sSectorsList = sSectors.split(",", QString::SkipEmptyParts);
                for(int i = 0; i < sSectorsList.size(); ++i)
                {
                    dataINF.sTo = sSectorsList[i];
                    pMesINF = QSharedPointer<OLDIMessage>(new OLDIMesINF(dataINF));

                    if(GetValue(QL1S(XML_ANSWR), "true") == "false")
                        pMesINF->GetMesSourceData().bAnswer = false;
                    //connect(pMes.data(), SIGNAL(messageAnswer(const OLDIMessage*,const QStringList&)), this, SLOT(OLDIMesAnswer(const OLDIMessage*,const QStringList&)));
                    //connect(pMes.data(), SIGNAL(messageTimeOut(const OLDIMessage*)), this, SLOT(OLDIMesTimeout(const OLDIMessage*)));
                    m_refMaster.GetOLDIProc()->SendMessage(pMesINF);
                }
            }

        } TVA_CATCHALL(err)
        {
            LOG_TVAERROR(ERROR, err)
        }
        return CXMLResp::TXMLResp();
    }
}
