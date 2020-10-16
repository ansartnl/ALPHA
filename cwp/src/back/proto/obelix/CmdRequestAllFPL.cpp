#include "CmdRequestAllFPL.h"

#include "Common.h"
#include "proto/Obelix.h"
#include "Master.h"
#include "XMasterWindow.h"

namespace obelix
{
    const char CCmdRequestAllFPL::XML_CMDNAME[] = "request_fpl";

    CCmdRequestAllFPL::CCmdRequestAllFPL()
        : CXMLCmd(QL1S(XML_PROTO))
    {
        SetCmdName(QL1S(XML_CMDNAME));
        MarkACK(false);
    }

    CCmdRequestAllFPL::CCmdRequestAllFPL(const CXMLData::TXMLData& pData)
        : CXMLCmd(pData)
    {

    }

    CCmdRequestAllFPLExe::CCmdRequestAllFPLExe(
        const CXMLData::TXMLData& pData,
        CMaster& refMaster)
        : CCmdRequestAllFPL(pData), m_refMaster(refMaster)
    {

    }

    CXMLResp::TXMLResp CCmdRequestAllFPLExe::Execute()
    {
        CXMLResp::TXMLResp pResp;
        TVA_CATCHALL_TRY
        {
            ((XMasterWindow*)getMainWindow())->RequestAllFPL();
        } TVA_CATCHALL(err)
        {
            LOG_TVAERROR(ERROR, err)
        }

        return pResp;
    }
}
