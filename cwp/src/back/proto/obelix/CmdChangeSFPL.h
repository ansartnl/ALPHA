#ifndef CMDCHANGESFPL_H
#define CMDCHANGESFPL_H

#include "proto/TCP/XMLCmd.h"
#include "ListAC.h"

class CMaster;

namespace obelix
{
    //! Notifiaction to client about wish to transmist airplane control from another operator(creation)
    class CCmdChangeSFPL
        : public CXMLCmd
    {
    public:
        /*! \name String constants */
        //@{
        static const char XML_CMDNAME[];
        static const char XML_CHANGE[];
        static const char XML_TYPE[];

        static const char XML_AIRID[];
        static const char XML_FPLID[];  //quint32
        static const char XML_CODE[];   //toInt(0, 8)
        static const char XML_COPIN[];  //QString
        static const char XML_ETOIN[];  //time
        static const char XML_COPINFL[];//double

        enum Type{Change,ACC,ARR,DEP, Cancel, Edit, SetData, STAR};

        //! Constructor
        /*!
            \param uiAirID airplane identifeir
            \param sOLDIMes OLDI message type
        */
        CCmdChangeSFPL(Type type);
        //! Get airplane identifeir
        /*!
            \return airplane identifeir
        */
        Type GetType();
        //! Set the value of an XML attribute
        /*!
            \param sAttrName attribute name
            \param sVal value
        */
        template <class T>
        void SetValue(const QString& sAttrName, const T& sVal)
        {
            m_DNChange.setAttribute(sAttrName, sVal);
        }
        //! Get the value of an XML attribute
        /*!
            \param sAttrName attribute name
            \return value
        */
        QString GetValue(const QString& sAttrName);

    protected:
        //! Constructor for CCmdChangeSFPL
        /*!
            \param pData smart pointer to CXMLData
        */
        explicit CCmdChangeSFPL(const CXMLData::TXMLData& pData);

    private:
        QDomElement        m_DNChange;
        QDomAttr        m_DAType;
    };

    //! Notifiaction to client about wish to transmist airplane control from another operator(execution)
    class CCmdChangeSFPLExe
        : public CCmdChangeSFPL
    {
    public:
        //! Constructor
        /*!
            \param pData smart pointer to CXMLData
            \param refMaster reference to CMaster
        */
        CCmdChangeSFPLExe(
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


#endif // CMDCHANGESFPL_H
