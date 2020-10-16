#ifndef CMDSENDUSERZONE_H
#define CMDSENDUSERZONE_H


#include "proto/TCP/XMLCmd.h"
#include "OLDIMessages.h"
#include "QMessageBox"
#include "qobject.h"

#include "obelix/ObRespond.h"

class CMaster;

namespace obelix
{
    //! Notifiaction to server about wish to transmist airplane control to another operator(creation)
    class CCmdSendUserZone
        : public CXMLCmd
    {
    public:
        /*! \name String constants */
        //@{
        static const char XML_CMDNAME[];
        static const char XML_USERZONE[];
        static const char XML_DBTYPE[];
        static const char XML_FIRIDNAME[];
        static const char XML_HEXDATA[];

        //@}
        //! Constructor
        /*!
            \param uiAirID airplane identifeir
            \param sOperName operator name
        */
        explicit CCmdSendUserZone(const QString& sDBType,const QString& sFIRIDName, const QString& sHexData);

        QString GetDBType() const;
        QString GetFIRIDName() const;
        QString GetHexData() const;

    protected:
        //! Constructor for CCmdPutExe
        /*!
            \param pData smart pointer to CXMLData
        */
        explicit CCmdSendUserZone(const CXMLData::TXMLData& pData);

    private:
        QDomElement     m_DEUserZone;
        QDomAttr        m_DADBType;
        QDomAttr        m_DAFIRIDName;
        QDomAttr        m_DAHexData;
    };
    //! Notifiaction to server about wish to transmist airplane control to another operator(execute)
    class CCmdSendUserZoneExe
        : public CCmdSendUserZone
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
        CCmdSendUserZoneExe(
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

#endif // CMDSENDUSERZONE_H
