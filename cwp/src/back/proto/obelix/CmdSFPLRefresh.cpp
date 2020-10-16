#include "CmdSFPLRefresh.h"
#include "Common.h"
#include "Master.h"
#include "GUIView.h"

namespace obelix
{
    const char CCmdSFPLRefresh::XML_CMDNAME[]    = "sfpl_refresh";
    const char CCmdSFPLRefresh::XML_REFRESH[] = "sfplrefresh";
    const char CCmdSFPLRefresh::XML_TO[]    = "sTo";
    const char CCmdSFPLRefresh::XML_FROM[] = "sFrom";
    const char CCmdSFPLRefresh::XML_TYPE[] = "sType";
    const char CCmdSFPLRefresh::XML_ACID[] = "sACID";
    const char CCmdSFPLRefresh::XML_ADEP[] = "sADEP";
    const char CCmdSFPLRefresh::XML_ADES[] = "sADES";
    const char CCmdSFPLRefresh::XML_CODE[] = "sCODE";
    const char CCmdSFPLRefresh::XML_SECTOR[] = "sSector";

    const char CCmdSFPLRefresh::XML_COPIN[] = "COPin";
    const char CCmdSFPLRefresh::XML_COPINFL[] = "COPinFL";
    const char CCmdSFPLRefresh::XML_ETOIN[] = "ETOin";
    const char CCmdSFPLRefresh::XML_FPLID[] = "FPLID";

    CCmdSFPLRefresh::CCmdSFPLRefresh()
    : CXMLCmd(QL1S(XML_PROTO))
    {
        SetCmdName(QL1S(XML_CMDNAME));
        MarkACK(false);

        m_DERefresh = GetDoc().createElement(QL1S(XML_REFRESH));
        GetMainNode().appendChild(m_DERefresh);
    }

    QString CCmdSFPLRefresh::GetValue(const QString &sAttrName)
    {
        return m_DERefresh.attribute(sAttrName);
    }

    bool CCmdSFPLRefresh::IsValue(const QString &sAttrName)
    {
        return m_DERefresh.hasAttribute(sAttrName);
    }

    CCmdSFPLRefresh::CCmdSFPLRefresh(const CXMLData::TXMLData &pData)
        : CXMLCmd(pData)
    {
        m_DERefresh = GetMainNode().firstChildElement();
        TVA_CHECK(
            !m_DERefresh.isNull() && m_DERefresh.nodeName() == QL1S(XML_REFRESH),
            QString(QL1S(XML_REFRESH) + QObject::tr(" node in XML command is absent or has wrong name")),
            E_FAIL);
    }

    CCmdSFPLRefreshExe::CCmdSFPLRefreshExe(
        const CXMLData::TXMLData& pData,
        CMaster& refMaster)
        : CCmdSFPLRefresh(pData), m_refMaster(refMaster)
    {

    }

    CXMLResp::TXMLResp CCmdSFPLRefreshExe::Execute()
    {
        QString sMessageType = GetValue(QL1S(XML_TYPE));

        TVA_CATCHALL_TRY
        {
            if(sMessageType == "COMMON")
            {
                //COMMON
                quint32 ID = GetValue(QL1S(XML_ACID)).toUInt();
                LOG_OPER_STR("ALL", "ACID", ID);
                CListAC::RefreshAndSelect(ID);
            }
            else if(sMessageType == oldi::INF_DEL_SFPL)
            {//INF_DEL_SFPL
                QString sADEP = GetValue(QL1S(XML_ADEP));
                QString sADES = GetValue(QL1S(XML_ADES));
                QString sACID = GetValue(QL1S(XML_ACID));
                CListACData& pItemData = CListAC::GetDataBySFPLkey(sACID, sADEP, sADES);
                if(pItemData.GetSFPL())
                {
                    if(pItemData.GetAirplane())
                    {
                        CGUIAirplane* pGUIPlane = dynamic_cast<CGUIAirplane*> (pItemData.GetAirplane().data());
                        if (pGUIPlane)
                            pGUIPlane->ShowPathPoints(false);
                        pItemData.GetAirplane()->ClearSFPL();
                    }
                    pItemData.SetAirplane(QSharedPointer<CAirplane>());
                }
                CListAC::RefreshAndSelect(0);
            }
            else if(sMessageType == oldi::INF_UNDO_SFPL)
            {//INF_UNDO_SFPL
                QString sADEP = GetValue(QL1S(XML_ADEP));
                QString sADES = GetValue(QL1S(XML_ADES));
                QString sACID = GetValue(QL1S(XML_ACID));
                CListACData& pItemData = CListAC::GetDataBySFPLkey(sACID, sADEP, sADES);
                if(pItemData.GetAirplane())
                {
                    CGUIAirplane* pGUIPlane = dynamic_cast<CGUIAirplane*> (pItemData.GetAirplane().data());
                    if (pGUIPlane)  //just client
                        pGUIPlane->ShowPathPoints(false);
                    pItemData.GetAirplane()->ClearSFPL();
                }

                CListAC::RefreshAndSelect(0);  //just client ! crash
            }
            else if(sMessageType == oldi::INF_ACT)
            {
                QString sADEP = GetValue(QL1S(XML_ADEP));
                QString sADES = GetValue(QL1S(XML_ADES));
                QString sACID = GetValue(QL1S(XML_ACID));
                CListACData& pItemData = CListAC::GetDataBySFPLkey(sACID, sADEP, sADES);
                if(pItemData.GetSFPL())
                {
                    pItemData.GetSFPL()->SetCODE(GetValue(XML_CODE).toInt(0, 8));
                    pItemData.GetSFPL()->SetInSector(GetValue(XML_SECTOR));
                }
            }
            else if(sMessageType == oldi::INF_ASSUME)
            {
                QString sADEP = GetValue(QL1S(XML_ADEP));
                QString sADES = GetValue(QL1S(XML_ADES));
                QString sACID = GetValue(QL1S(XML_ACID));
                CListACData& pItemData = CListAC::GetDataBySFPLkey(sACID, sADEP, sADES);
                if(pItemData.GetSFPL())
                {
                    pItemData.GetSFPL()->SetCODE(GetValue(XML_CODE).toInt(0, 8));
                    pItemData.GetSFPL()->SetCurSector(GetValue(XML_SECTOR));
                }
            }
            else if(sMessageType == oldi::MES_ABI)
            {
                QList<CFPL> list;
                TVA_CATCHALL_TRY
                {
                    dao()->LoadFPLs(list, "visibility = 1");
                } TVA_CATCHALL(err)
                {
                    Q_UNUSED(err)
                }

                QList<CFPL>::iterator itList = list.begin();
                for(; itList != list.end(); ++itList)
                {
                    if( (*itList).GetID() == GetValue(QL1S(XML_FPLID)).toUInt())
                        break;
                }

                QSharedPointer<CSFPL> pSFPL = QSharedPointer<CSFPL>(new CSFPL);
                *(static_cast<CFPL*>(pSFPL.data())) = *(itList);
                pSFPL->SetCODE(GetValue(QL1S(XML_CODE)).toUInt());
                pSFPL->SetCOPin(GetValue(QL1S(XML_COPIN)));
                pSFPL->SetETOin(QTime().fromString(GetValue(QL1S(XML_ETOIN))));
                pSFPL->SetCOPinFL(FLVal(GetValue(QL1S(XML_COPINFL))));

                CListACData& itemData = CListAC::GetDataBySFPLkey(pSFPL->GetACID(), pSFPL->GetDepAerodrome(), pSFPL->GetDestAerodrome());

                if(itemData.GetAirplane())
                {
                    CGUIAirplane* pGUIPlane = dynamic_cast<CGUIAirplane*>(itemData.GetAirplane().data());
                    if (pGUIPlane)
                        pGUIPlane->ShowPathPoints(false);
                }
                CGUIView* pView = dynamic_cast<CGUIView*>(m_refMaster.GetView().data());
                if(pView)
                    pView->OnShangedSFPL(pSFPL);
            }
        } TVA_CATCHALL(err)
        {
            LOG_TVAERROR(ERROR, err)
        }

        return CXMLResp::TXMLResp();
    }
}
