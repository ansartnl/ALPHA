#ifndef CMDMETEOUPDATE_H
#define CMDMETEOUPDATE_H


#include "proto/TCP/XMLCmd.h"
#include "OLDIMessages.h"
#include "QMessageBox"
#include "qobject.h"

#include "obelix/ObRespond.h"

class CMaster;

namespace obelix
{
    //!
    class CCmdMeteoUpdate
        : public CXMLCmd
    {
    public:
        /*! \name String constants */
        //@{
        static const char XML_CMDNAME[];
        static const char XML_METEOUPDATE[];
        static const char XML_HEXDATA[];

        //@}
        //! Constructor
        /*!
        */
        explicit CCmdMeteoUpdate(const QString& sHexData);

        QString GetHexData() const;

    protected:
        //! Constructor
        /*!
            \param pData smart pointer to CXMLData
        */
        explicit CCmdMeteoUpdate(const CXMLData::TXMLData& pData);

    private:
        QDomElement     m_DEUserZone;
        QDomAttr        m_DAHexData;
    };
    //!
    class CCmdMeteoUpdateExe
        : public CCmdMeteoUpdate
    {
    public:
        //! Constructor
        /*!
        */
        CCmdMeteoUpdateExe(
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

#endif // CMDMETEOUPDATE_H
