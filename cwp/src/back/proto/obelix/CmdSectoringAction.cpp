#include "CmdSectoringAction.h"

#include "Common.h"
#include "Master.h"
#include "XMasterWindow.h"

#include <QMessageBox>

namespace obelix
{
    const char CCmdSectoringAction::XML_CMDNAME[]    = "sectoring_action";
    const char CCmdSectoringAction::XML_SECTORING[]    = "sectoring";
    const char CCmdSectoringAction::XML_ACTION[]        = "action";

    CCmdSectoringAction::CCmdSectoringAction(SectoringAction action)
    : CXMLCmd(QL1S(XML_PROTO))
    {
        SetCmdName(QL1S(XML_CMDNAME));
        MarkACK(false);

        m_DESectoring = GetDoc().createElement(QL1S(XML_SECTORING));
        m_DAAction = GetDoc().createAttribute(QL1S(XML_ACTION));
        m_DAAction.setValue(QString::number((quint32)action));

        m_DESectoring.setAttributeNode(m_DAAction);

        GetMainNode().appendChild(m_DESectoring);
    }

    CCmdSectoringAction::CCmdSectoringAction(const CXMLData::TXMLData& pData)
        : CXMLCmd(pData)
    {
        m_DESectoring = GetMainNode().firstChildElement();
        TVA_CHECK(
            !m_DESectoring.isNull() && m_DESectoring.nodeName() == QL1S(XML_SECTORING),
            QString(QL1S(XML_SECTORING) + QObject::tr(" node in XML command is absent or has wrong name")),
            E_FAIL);

        m_DAAction = m_DESectoring.attributeNode(QL1S(XML_ACTION));
        TVA_CHECK(
            !m_DAAction.isNull(),
            QString(QL1S(XML_ACTION) + QObject::tr(" attribute in XML command is absent or has wrong name")),
            E_FAIL);
    }

    CCmdSectoringActionExe::CCmdSectoringActionExe(
        const CXMLData::TXMLData& pData,
        CMaster& refMaster)
        : CCmdSectoringAction(pData), m_refMaster(refMaster)
    {

    }

    CXMLResp::TXMLResp CCmdSectoringActionExe::Execute()
    {
        TVA_CATCHALL_TRY
        {
            QString sText("");
            switch(GetAction()){
            case FDP_agreed:
                sText = QObject::tr("FDP agree with our sectoring.");
                break;
            case FDP_rejected:
            {
                sText = QObject::tr("FDP reject our sectoring.");
                if(((XMasterWindow*)getMainWindow())->IsSectoringInitiator())
                {
                    if(QMessageBox::warning((XMasterWindow*)getMainWindow(), ((XMasterWindow*)getMainWindow())->windowTitle(), QObject::tr("Force sectoring?"), QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
                    {
                        QSharedPointer<obelix::CCmdSectoringAction> pCmd(new obelix::CCmdSectoringAction(obelix::CCmdSectoringAction::Undo));
                        m_refMaster.GetObClient().XMLSend(pCmd.staticCast<CXMLCmd>());
                    }
                    else
                    {
                        QSharedPointer<obelix::CCmdSectoringAction> pCmd(new obelix::CCmdSectoringAction(obelix::CCmdSectoringAction::Force));
                        m_refMaster.GetObClient().XMLSend(pCmd.staticCast<CXMLCmd>());
                    }
                }
                break;
            }
            case FDP_timeout:
                sText = QObject::tr("No connection with FDP.");
                break;
            case Force:
            {
                ((XMasterWindow*)getMainWindow())->ForceSectoring();
                break;
            }
            case Undo:
            {
                ((XMasterWindow*)getMainWindow())->_UndoSectoring();
                break;
            }
            default:
                break;
            }
            ((XMasterWindow*)getMainWindow())->SetTempStatusText(sText);
            ((XMasterWindow*)getMainWindow())->SetSectoringInitiator(false);
        } TVA_CATCHALL(err)
        {
            LOG_TVAERROR(ERROR, err)
        }

        return CXMLResp::TXMLResp();
    }

    CCmdSectoringAction::SectoringAction CCmdSectoringAction::GetAction() const
    {
        bool bOk(false);
        CCmdSectoringAction::SectoringAction type = (CCmdSectoringAction::SectoringAction)m_DAAction.value().toUInt(&bOk);
        TVA_CHECK(
            bOk,
            QString(QL1S(XML_ACTION) + QObject::tr(" attribute in XML command has wrong structure")),
            E_FAIL);

        return type;
    }
}
