#ifndef CMDREQUESTALLFPL_H
#define CMDREQUESTALLFPL_H

#include "proto/TCP/XMLCmd.h"

class CMaster;

namespace obelix
{
    //! Requests all FPL's(creation)
    class CCmdRequestAllFPL
        : public CXMLCmd
    {
    public:
        /*! \name String constants */
        //@{
        static const char XML_CMDNAME[];
        //@}

        //! Constructor
        explicit CCmdRequestAllFPL();

    protected:
        //! Constructor for CCmdRequestAllFPLExe
        /*!
            \param pData smart pointer to CXMLData
        */
        explicit CCmdRequestAllFPL(const CXMLData::TXMLData& pData);
    };

    //! Requests all FPL's(execution)
    class CCmdRequestAllFPLExe
        : public CCmdRequestAllFPL
    {
    public:
        //! Constructor
        /*!
            \param pData smart pointer to CXMLData
            \param refMaster reference to CMaster
        */
        CCmdRequestAllFPLExe(
            const CXMLData::TXMLData& pData,
            CMaster& refMaster);
        /*! \name Implementation of CXMLCmd interface */
        //@{
        virtual CXMLResp::TXMLResp Execute();
        //@}
    private:
        CMaster&            m_refMaster;
    };
}

#endif // CMDREQUESTALLFPL_H
