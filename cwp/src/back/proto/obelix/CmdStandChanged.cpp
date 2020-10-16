#include "CmdStandChanged.h"
#include "Common.h"
#include "Master.h"

namespace obelix
{

    const char CCmdStandChanged::XML_CMDNAME[] = "stand_changed";
    const char CCmdStandChanged::XML_STAND[] = "stand";
    const char CCmdStandChanged::XML_ID[] = "stand_id";
    const char CCmdStandChanged::XML_AIRPORT[] = "airport";
    const char CCmdStandChanged::XML_NAME[] = "stand_name";
    const char CCmdStandChanged::XML_ACTION[] = "stand_state";
    const char CCmdStandChanged::XML_FPLNAME[] = "fpl_name";

    CCmdStandChanged::CCmdStandChanged(quint32 id, const QString &airport, const QString &stand, quint32 state, const QString &fplName)
        : CXMLCmd(QL1S(XML_PROTO))
    {
        SetCmdName(QL1S(XML_CMDNAME));
        MarkACK(false);

        m_DNStand = GetDoc().createElement(QL1S(XML_STAND));
        m_DAID = GetDoc().createAttribute(QL1S(XML_ID));
        m_DAAirport = GetDoc().createAttribute(QL1S(XML_AIRPORT));
        m_DAStand = GetDoc().createAttribute(QL1S(XML_NAME));
        m_DAAction = GetDoc().createAttribute(QL1S(XML_ACTION));
        m_DAFplName = GetDoc().createAttribute(QL1S(XML_FPLNAME));

        m_DAID.setValue(QString::number(id));
        m_DAAirport.setValue(airport);
        m_DAStand.setValue(stand);
        m_DAAction.setValue(QString::number(state));
        m_DAFplName.setValue(fplName);

        m_DNStand.setAttributeNode(m_DAID);
        m_DNStand.setAttributeNode(m_DAAirport);
        m_DNStand.setAttributeNode(m_DAStand);
        m_DNStand.setAttributeNode(m_DAAction);
        m_DNStand.setAttributeNode(m_DAFplName);
        GetMainNode().appendChild(m_DNStand);
    }

    quint32 CCmdStandChanged::GetID() const
    {
        bool bOk(false);
        quint32 id = m_DAID.value().toUInt(&bOk);
        TVA_CHECK(
            bOk,
            QString(QL1S(XML_ID) + QObject::tr(" attribute in XML command has wrong structure")),
            E_FAIL);

        return id;
    }

    QString CCmdStandChanged::GetAirport() const
    {
        return m_DAAirport.value();
    }

    QString CCmdStandChanged::GetStand() const
    {
        return m_DAStand.value();
    }

    quint32 CCmdStandChanged::GetState() const
    {
        bool bOk(false);
        quint32 state = m_DAAction.value().toUInt(&bOk);
        TVA_CHECK(
            bOk,
            QString(QL1S(XML_ACTION) + QObject::tr(" attribute in XML command has wrong structure")),
            E_FAIL);

        return state;
    }

    QString CCmdStandChanged::GetFplName() const
    {
        return m_DAFplName.value();
    }

    CCmdStandChanged::CCmdStandChanged(const CXMLData::TXMLData &pData)
        : CXMLCmd(pData)
    {
        m_DNStand = GetMainNode().firstChildElement();
        TVA_CHECK(
            !m_DNStand.isNull() && m_DNStand.nodeName() == QL1S(XML_STAND),
            QString(QL1S(XML_STAND) + QObject::tr(" node in XML command is absent or has wrong name")),
            E_FAIL);

        m_DAID = m_DNStand.attributeNode(QL1S(XML_ID));
        TVA_CHECK(
            !m_DAID.isNull(),
            QString(QL1S(XML_ID) + QObject::tr(" attribute in XML command is absent or has wrong name")),
            E_FAIL);

        m_DAAirport = m_DNStand.attributeNode(QL1S(XML_AIRPORT));
        TVA_CHECK(
            !m_DAAirport.isNull(),
            QString(QL1S(XML_AIRPORT) + QObject::tr(" attribute in XML command is absent or has wrong name")),
            E_FAIL);

        m_DAStand = m_DNStand.attributeNode(QL1S(XML_NAME));
        TVA_CHECK(
            !m_DAStand.isNull(),
            QString(QL1S(XML_NAME) + QObject::tr(" attribute in XML command is absent or has wrong name")),
            E_FAIL);

        m_DAAction = m_DNStand.attributeNode(QL1S(XML_ACTION));
        TVA_CHECK(
            !m_DAAction.isNull(),
            QString(QL1S(XML_ACTION) + QObject::tr(" attribute in XML command is absent or has wrong name")),
            E_FAIL);

        m_DAFplName = m_DNStand.attributeNode(QL1S(XML_FPLNAME));
        TVA_CHECK(
            !m_DAFplName.isNull(),
            QString(QL1S(XML_FPLNAME) + QObject::tr(" attribute in XML command is absent or has wrong name")),
            E_FAIL);
    }

    CCmdStandChangedExe::CCmdStandChangedExe(const CXMLData::TXMLData &pData, CMaster &refMaster)
        : CCmdStandChanged(pData), m_refMaster(refMaster)
    {
    }

    CXMLResp::TXMLResp CCmdStandChangedExe::Execute()
    {
        TVA_CATCHALL_TRY
        {
            GroundStand stand;
            stand.mId = GetID();
            stand.mAirportName = GetAirport();
            stand.mStandName = GetStand();
            stand.mStateId = GetState();
            stand.mFplName = GetFplName();

            m_refMaster.StandChangedGUI(stand);

        } TVA_CATCHALL(err)
        {
            LOG_TVAERROR(ERROR, err)
        }
        return CXMLResp::TXMLResp();
    }
}
