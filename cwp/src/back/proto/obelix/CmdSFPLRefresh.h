#ifndef CMDSFPLREFRESH_H
#define CMDSFPLREFRESH_H

#include "proto/TCP/XMLCmd.h"

#include "GUIAirplane.h"
#include "ListACData.h"
#include "ListAC.h"

class CMaster;

namespace obelix
{
    //! Notification to client to refresh SFPL data
    class CCmdSFPLRefresh
        : public CXMLCmd
    {
    public:
        /*! \name String constants */
        //@{
        static const char XML_CMDNAME[];
        static const char XML_REFRESH[];
        static const char XML_TO[];
        static const char XML_FROM[];
        static const char XML_TYPE[];
        static const char XML_ACID[];
        static const char XML_ADEP[];
        static const char XML_ADES[];
        static const char XML_CODE[];
        static const char XML_SECTOR[];

        static const char XML_COPIN[];
        static const char XML_COPINFL[];
        static const char XML_ETOIN[];
        static const char XML_FPLID[];

        //! Constructor
        CCmdSFPLRefresh();

        //! Set the value of an XML attribute
        /*!
            \param sAttrName attribute name
            \param sVal value
        */
        void SetValue(const QString& sAttrName, const QString& sVal)
        {
            m_DERefresh.setAttribute(sAttrName, sVal);
        }
        //! Get the value of an XML attribute
        /*!
            \param sAttrName attribute name
            \return value
        */
        QString GetValue(const QString& sAttrName);
        //! Check presence of an attribute
        /*!
            \param sAttrName attribute name
            \return true if presents
        */
        bool IsValue(const QString& sAttrName);

    protected:
        //! Constructor for CCmdSFPLRefreshExe
        /*!
            \param pData smart pointer to CXMLData
        */
        explicit CCmdSFPLRefresh(const CXMLData::TXMLData& pData);

    private:
        QDomElement        m_DERefresh;
    };

    //! Notification to client to refresh SFPL data (execution)
    class CCmdSFPLRefreshExe
        : public CCmdSFPLRefresh
    {
    public:
        //! Constructor
        /*!
            \param pData smart pointer to CXMLData
            \param refMaster reference to CMaster
        */
        CCmdSFPLRefreshExe(
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

#endif // CMDSFPLREFRESH_H
