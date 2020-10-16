#ifndef CMDPUT_H
#define CMDPUT_H

#include "proto/TCP/XMLCmd.h"
#include "OLDIMessages.h"
#include "QMessageBox"
#include "qobject.h"

#include "obelix/ObRespond.h"

class CMaster;

namespace obelix
{
    //! Notifiaction to server about wish to transmist airplane control to another operator(creation)
    class CCmdPut
        : public CXMLCmd
    {
    public:
        /*! \name String constants */
        //@{
        static const char XML_CMDNAME[];
        static const char XML_TRANSFER[];
        static const char XML_AIRID[];
        static const char XML_OPERNAME[];

        static const char XML_OLDITYPE[];
        static const char XML_FPLID[];        
        //@}
        //! Constructor
        /*!
            \param uiAirID airplane identifeir
            \param sOperName operator name
        */
        explicit CCmdPut(quint32 uiAirID, const QString& sOperName);
        //! Get operator name
        /*!
            \return operator name
        */
        QString GetOperator() const;
        //! Get airplane identifeir
        /*!
            \return airplane identifeir
        */
        quint32 GetAirID() const;
        //! Set the value of an XML attribute
        /*!
            \param sAttrName attribute name
            \param sVal value
        */
        template <class T>
        void SetValue(const QString& sAttrName, const T& sVal)
        {
            m_DNTransfer.setAttribute(sAttrName, sVal);
        }

        QString GetValue(const QString& sAttrName)
        {
            return m_DNTransfer.attribute(sAttrName);
        }

    protected:
        //! Constructor for CCmdPutExe
        /*!
            \param pData smart pointer to CXMLData
        */
        explicit CCmdPut(const CXMLData::TXMLData& pData);

    private:
        QDomElement        m_DNTransfer;
        QDomAttr        m_DAAirID;
        QDomAttr        m_DAOperName;        
    };
    //! Notifiaction to server about wish to transmist airplane control to another operator(execute)
    class CCmdPutExe
        : public CCmdPut
    {
    public:
        //! Constructor
        /*!
            \param pData smart pointer to CXMLData
            \param refMaster reference to CMaster
        */
//        CCmdPutExe(
//            const CXMLData::TXMLData& pData,
//            CMaster& refMaster);
        //! Constructor
        /*!
            \param pData smart pointer to CXMLData
            \param refMaster reference to CMaster
        */
        CCmdPutExe(
            const CXMLData::TXMLData& pData,
            CMaster& refMaster,
            CConnect &connect);
        /*! \name Implementation of CXMLCmd interface */
        //@{
        virtual CXMLResp::TXMLResp Execute();
        //@}

    private:
        CMaster&            m_refMaster;
        obelix::CConnect&   m_refConnect;
    };
}

#endif // CMDPUT_H
