#ifndef CMDHELLO_H
#define CMDHELLO_H

#include "proto/TCP/XMLCmd.h"

class CMaster;

namespace obelix
{
    class CConnect;
    //! Greeting command(creation)
    class CCmdHello
        : public CXMLCmd
    {
    public:
        /*! \name String constants */
        //@{
        static const char XML_CMDNAME[];
        static const char XML_OPER[];
        static const char XML_OPERNAME[];
        //@}

        //! Constructor
        /*!
            \param sOperName operator name, who sent this command
        */
        explicit CCmdHello(const QString& sOperName);

        //! Get operator name
        /*!
            \return operator name
        */
        QString GetOperName() const;

        /*! \name Implementation of CXMLCmd interface */
        //@{
        virtual void Answer(CXMLResp::TXMLResp& pResp);
        //@}

    protected:
        //! Constructor for CCmdHelloExe
        /*!
            \param pData smart pointer to CXMLData
        */
        explicit CCmdHello(const CXMLData::TXMLData& pData);

    private:
        QDomElement        m_DNOper;
        QDomAttr        m_DAOperName;
    };

    //! Greeting command(execution)
    class CCmdHelloExe
        : public CCmdHello
    {
    public:
        //! Constructor
        /*!
            \param pData smart pointer to CXMLData
            \param refMaster reference to CMaster
            \param refConnect reference to obelix::CConnect from which came this command
        */
        CCmdHelloExe(
            const CXMLData::TXMLData& pData,
            CMaster& refMaster,
            obelix::CConnect& refConnect);
        //! Destructor
        virtual ~CCmdHelloExe();
        /*! \name Implementation of CXMLCmd interface */
        //@{
        virtual CXMLResp::TXMLResp Execute();
        //@}
    private:
        CMaster&            m_refMaster;
        obelix::CConnect&    m_refConnect;
        bool                m_bSendList;
    };

    //! Response for greeting command
    class CRespHello
        : public CXMLResp
    {
    public:
        //! Constructor to create response fro a command
        /*!
            \param refCmd reference to CCmdHello
        */
        CRespHello(CCmdHello& refCmd);
        //! Constructor to parse XML data
        /*!
            \param pData smart pointer to CXMLData
        */
        CRespHello(const CXMLData::TXMLData& pData);
    };
}

#endif // CMDHELLO_H
