#ifndef CMDTAKE_H
#define CMDTAKE_H

#include "proto/TCP/XMLCmd.h"

class CMaster;

namespace obelix
{
    //! Notifiaction to client about wish to transmist airplane control from another operator(creation)
    class CCmdTake
        : public CXMLCmd
    {
    public:
        /*! \name String constants */
        //@{
        static const char XML_CMDNAME[];
        static const char XML_TRANSFER[];
        static const char XML_AIRID[];
        static const char XML_RESET[];

        static const char XML_OLDIMES[];
        //@}
        //! Constructor
        /*!
            \param uiAirID airplane identifeir
            \param bReset reset flag, if true, than cancel of the previous TAKE packet
        */
        CCmdTake(quint32 uiAirID, bool bReset);
        //! Constructor
        /*!
            \param uiAirID airplane identifeir
            \param sOLDIMes OLDI message type
        */
        CCmdTake(quint32 uiAirID);
        //! Get airplane identifeir
        /*!
            \return airplane identifeir
        */
        quint32 GetAirID() const;
        //! Get reset flag
        /*!
            \return reset flag
        */
        bool IsReset() const;
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
        //! Get the value of an XML attribute
        /*!
            \param sAttrName attribute name
            \return value
        */
        QString GetValue(const QString& sAttrName);

    protected:
        //! Constructor for CCmdTakeExe
        /*!
            \param pData smart pointer to CXMLData
        */
        explicit CCmdTake(const CXMLData::TXMLData& pData);

    private:
        QDomElement        m_DNTransfer;
        QDomAttr        m_DAAirID;
        QDomAttr        m_DAReset;
    };

    //! Notifiaction to client about wish to transmist airplane control from another operator(execution)
    class CCmdTakeExe
        : public CCmdTake
    {
    public:
        //! Constructor
        /*!
            \param pData smart pointer to CXMLData
            \param refMaster reference to CMaster
        */
        CCmdTakeExe(
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

#endif // CMDTAKE_H
