#ifndef CMDTAKEN_H
#define CMDTAKEN_H

#include "proto/TCP/XMLCmd.h"
#include "OLDIMesINF.h"

class CMaster;

namespace obelix
{
    class CConnect;
    //! Acknowledgement to server about airplane control accepting(creation)
    class CCmdTaken
        : public CXMLCmd
    {
    public:
        /*! \name String constants */
        //@{
        static const char XML_CMDNAME[];
        static const char XML_TRANSFER[];
        static const char XML_AIRID[];

        static const char XML_FPLID[];
        static const char XML_ANSWR[];
        //@}
        //! Constructor
        /*!
            \param uiAirID airplane identifeir
        */
        explicit CCmdTaken(quint32 uiAirID);
        quint32 GetAirID() const;

        template <class T>
        void SetValue(const QString& sAttrName, const T& sVal)
        {
            m_DNTransfer.setAttribute(sAttrName, sVal);
        }
        //! Get the value of an XML attribute
        /*!
            \param sAttrName attribute name
            \return value
        */
        QString GetValue(const QString& sAttrName,const QString& defValue = QString());

    protected:
        //! Constructor for CCmdTakeExe
        /*!
            \param pData smart pointer to CXMLData
        */
        explicit CCmdTaken(const CXMLData::TXMLData& pData);

    private:
        QDomElement        m_DNTransfer;
        QDomAttr        m_DAAirID;
    };
//! Acknowledgement to server about airplane control accepting(execution)
    class CCmdTakenExe
        : public CCmdTaken
    {
    public:
        //! Constructor
        /*!
            \param pData smart pointer to CXMLData
            \param refMaster reference to CMaster
            \param refConnect reference to obelix::CConnect, from which came this packet
        */
        CCmdTakenExe(
            const CXMLData::TXMLData& pData,
            CMaster& refMaster,
            obelix::CConnect& refConnect);
        /*! \name Implementation of CXMLCmd interface */
        //@{
        virtual CXMLResp::TXMLResp Execute();
        //@}

    private:
        CMaster&            m_refMaster;
        obelix::CConnect&    m_refConnect;
    };
}

#endif // CMDTAKEN_H
