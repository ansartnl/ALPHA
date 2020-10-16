#include "CmdTransfer.h"
#include "Common.h"
#include "Master.h"


namespace obelix
{
    const char CCmdTransfer::XML_CMDNAME[]    = "transfer";
    const char CCmdTransfer::XML_TRANSFER[]    = "transfer";
    const char CCmdTransfer::XML_AIRID[]    = "airplane_id";
    const char CCmdTransfer::XML_TYPE[]     = "transfer_type";
    const char CCmdTransfer::XML_OPERNAME[] = "operator_name";

    const char CCmdTransfer::XML_CANCELTRANSFER[] = "cancel_transfer";         //bool
    const char CCmdTransfer::XML_SETCURSECTOR[] = "set_cur_sector";           //QString
    const char CCmdTransfer::XML_SETPLANETYPE[] = "set_plane_type";           //PlaneType
    const char CCmdTransfer::XML_ASSUMEOPERATOR[] = "assume_operator";         //void
    const char CCmdTransfer::XML_SETTRANSFERTIME[] = "set_transfer_time";        //void
    const char CCmdTransfer::XML_SETTRANSITION[] = "set_transition";          //QString
    const char CCmdTransfer::XML_SETOUR[] = "set_our";                 //bool
    const char CCmdTransfer::XML_DIRECTASSUME[] = "direct_assume";           //void
    const char CCmdTransfer::XML_CHANGEOPERATOR[] = "change_operator";         //int
    const char CCmdTransfer::XML_CHANGEOPERATOR_OPERNAME[] = "chenge_operator_name";//QString

    CCmdTransfer::CCmdTransfer(quint32 uiAirID)//, TransferType type)
        : CXMLCmd(QL1S(XML_PROTO))
    {
        SetCmdName(QL1S(XML_CMDNAME));
        MarkACK(false);

        m_DNTransfer = GetDoc().createElement(QL1S(XML_TRANSFER));
        m_DAAirID = GetDoc().createAttribute(QL1S(XML_AIRID));
        //m_DAType = GetDoc().createAttribute(QL1S(XML_TYPE));
        //m_DAOperName = GetDoc().createAttribute(QL1S(XML_OPERNAME));
        //m_DAOperName.setValue("");
        m_DAAirID.setValue(QString::number(uiAirID));
        //m_DAType.setValue( QString("%1").arg(type));

        m_DNTransfer.setAttributeNode(m_DAAirID);
        //m_DNTransfer.setAttributeNode(m_DAType);
        //m_DNTransfer.setAttributeNode(m_DAOperName);
        GetMainNode().appendChild(m_DNTransfer);
    }

    CCmdTransfer::CCmdTransfer(const CXMLData::TXMLData &pData)
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

//        m_DAType = m_DNTransfer.attributeNode(QL1S(XML_TYPE));
//        TVA_CHECK(
//            !m_DAType.isNull(),
//            QString(QL1S(XML_TYPE) + QObject::tr(" attribute in XML command is absent or has wrong name")),
//            E_FAIL);

//        m_DAOperName = m_DNTransfer.attributeNode(QL1S(XML_OPERNAME));
//        TVA_CHECK(
//            !m_DAOperName.isNull(),
//            QString(QL1S(XML_OPERNAME) + QObject::tr(" attribute in XML command is absent or has wrong name")),
//            E_FAIL);
    }

    quint32 CCmdTransfer::GetAirID() const
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

    CCmdTransfer::TransferType CCmdTransfer::GetType() const
    {
        bool bOk(false);
        TransferType type = (TransferType)m_DAType.value().toUInt(&bOk);
        TVA_CHECK(
            bOk,
            QString(QL1S(XML_TYPE) + QObject::tr(" attribute in XML command has wrong structure")),
            E_FAIL);

        return type;
    }

    void CCmdTransfer::setOperator(QString sName)
    {
        m_DAOperName.setValue(sName);
    }

    CCmdTransferExe::CCmdTransferExe(
        const CXMLData::TXMLData& pData,
        CMaster& refMaster)
        : CCmdTransfer(pData), m_refMaster(refMaster)
    {

    }

    QString CCmdTransfer::GetOperator() const
    {
        return m_DAOperName.value();
    }

    //! Setters for action definitions
    void CCmdTransfer::SetCancelTransfer(bool value)
    {
        if(value)
            m_DNTransfer.setAttribute(QL1S(XML_CANCELTRANSFER), "true");
        else
            m_DNTransfer.setAttribute(QL1S(XML_CANCELTRANSFER), "false");
    }

    void CCmdTransfer::SetCurSector(QString value)
    {
        m_DNTransfer.setAttribute(QL1S(XML_SETCURSECTOR), value);
    }

    void CCmdTransfer::SetPlaneType(CAirplaneData::PlaneType value)
    {
        m_DNTransfer.setAttribute(QL1S(XML_SETPLANETYPE), QString::number((quint32)value));
    }

    void CCmdTransfer::SetAssumeOperator()
    {
        m_DNTransfer.setAttribute(QL1S(XML_ASSUMEOPERATOR), QL1S("1"));
    }

    void CCmdTransfer::SetTransferTime()
    {
        m_DNTransfer.setAttribute(QL1S(XML_SETTRANSFERTIME), QL1S("1"));
    }

    void CCmdTransfer::SetTransition(QString value)
    {
        m_DNTransfer.setAttribute(QL1S(XML_SETTRANSITION), value);
    }

    void CCmdTransfer::SetOur(bool value)
    {
        if(value)
            m_DNTransfer.setAttribute(QL1S(XML_SETOUR), "true");
        else
            m_DNTransfer.setAttribute(QL1S(XML_SETOUR), "false");
    }

    void CCmdTransfer::SetDirectAssume()
    {
        m_DNTransfer.setAttribute(QL1S(XML_DIRECTASSUME), QL1S("1"));
    }

    void CCmdTransfer::SetChangeOperator(int idSector, QString sOperName)
    {
        m_DNTransfer.setAttribute(QL1S(XML_CHANGEOPERATOR), QString::number(idSector));
        m_DNTransfer.setAttribute(QL1S(XML_CHANGEOPERATOR_OPERNAME), sOperName);
    }

    //! Action's implementation
    void CCmdTransfer::CheckCancelTransfer(TAirplanesMap::iterator &itr)
    {
        if(m_DNTransfer.hasAttribute(QL1S(XML_CANCELTRANSFER))){
            QString sVal = m_DNTransfer.attribute(QL1S(XML_CANCELTRANSFER));
            if(sVal == "true")
                (*itr)->CancelTransfer(true);
            else
                (*itr)->CancelTransfer(false);
        }
    }

    void CCmdTransfer::CheckCurSector(TAirplanesMap::iterator& itr)
    {
        if(m_DNTransfer.hasAttribute(QL1S(XML_SETCURSECTOR))){
            QString sVal = m_DNTransfer.attribute(QL1S(XML_SETCURSECTOR));
            (*itr)->GetSFPL()->SetCurSector(sVal);
        }
    }

    void CCmdTransfer::CheckPlaneType(TAirplanesMap::iterator& itr)
    {
        if(m_DNTransfer.hasAttribute(QL1S(XML_SETPLANETYPE))){
            QString sVal = m_DNTransfer.attribute(QL1S(XML_SETPLANETYPE));
            CAirplaneData::PlaneType type = (CAirplaneData::PlaneType)sVal.toUInt();
            (*itr)->SetPlaneType(type);
        }
    }

    void CCmdTransfer::CheckAssumeOperator(TAirplanesMap::iterator& itr)
    {
        if(m_DNTransfer.hasAttribute(QL1S(XML_ASSUMEOPERATOR))){
            (*itr)->AssumeOperator();
        }
    }

    void CCmdTransfer::CheckTransferTime(TAirplanesMap::iterator& itr)
    {
        if(m_DNTransfer.hasAttribute(QL1S(XML_SETTRANSFERTIME))){
            (*itr)->SetTransferTime(QDateTime::currentDateTime());
        }
    }

    void CCmdTransfer::CheckTransition(TAirplanesMap::iterator& itr)
    {
        if(m_DNTransfer.hasAttribute(QL1S(XML_SETTRANSITION))){
            QString sVal = m_DNTransfer.attribute(QL1S(XML_SETTRANSITION));
            (*itr)->SetTransition(sVal);
        }
    }

    void CCmdTransfer::CheckOur(TAirplanesMap::iterator& itr)
    {
        if(m_DNTransfer.hasAttribute(QL1S(XML_SETOUR))){
            QString sVal = m_DNTransfer.attribute(QL1S(XML_SETOUR));
            if(sVal == "true")
                (*itr)->SetOur(true);
            else
                (*itr)->SetOur(false);
        }
    }

    void CCmdTransfer::CheckDirectAssume(TAirplanesMap::iterator& itr)
    {
        if(m_DNTransfer.hasAttribute(QL1S(XML_DIRECTASSUME))){
            (*itr)->DirectAssume();
        }
    }

    void CCmdTransfer::CheckChangeOperator(TAirplanesMap::iterator& itr)
    {
        if(m_DNTransfer.hasAttribute(QL1S(XML_CHANGEOPERATOR))){
            quint32 nID = m_DNTransfer.attribute(QL1S(XML_CHANGEOPERATOR)).toUInt();
            QString sOperName = m_DNTransfer.attribute(QL1S(XML_CHANGEOPERATOR_OPERNAME));
            (*itr)->ChangeOperator(nID, sOperName);
        }
    }

    CXMLResp::TXMLResp CCmdTransferExe::Execute()
    {
        TVA_CATCHALL_TRY
        {
            //QString sOperName = GetOperator();
            CMaster::TAirplanesMap::iterator itAirFind =
                                m_refMaster.GetPlanes()->find(GetAirID());
            if (itAirFind != m_refMaster.GetPlanes()->end())
            {
                CheckCancelTransfer(itAirFind);
                CheckCurSector(itAirFind);
                CheckPlaneType(itAirFind);
                CheckAssumeOperator(itAirFind);
                CheckTransferTime(itAirFind);
                CheckTransition(itAirFind);
                CheckOur(itAirFind);
                CheckDirectAssume(itAirFind);
                CheckChangeOperator(itAirFind);
            }
        } TVA_CATCHALL(err)
        {
            LOG_TVAERROR(ERROR, err)
        }

        return CXMLResp::TXMLResp();
    }
}
