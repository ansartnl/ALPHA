#ifndef CMDGUIREFRESH_H
#define CMDGUIREFRESH_H

#include "proto/TCP/XMLCmd.h"

#include "GUIView.h"
#include "XMasterWindow.h"

class CMaster;

namespace obelix
{
    //! Notification to client to refresh SFPL data
    class CCmdGUIRefresh
        : public CXMLCmd
    {
    public:
        /*! \name String constants */
        //@{
        static const char XML_CMDNAME[];
        static const char XML_REFRESH[];
        static const char XML_URZ_ID[];
        static const char XML_FIR_NAME[];
        static const char XML_TYPE[];
        static const char XML_URZTYPE[];

        //! Constructor
        CCmdGUIRefresh();

        //! Set the value of an XML attribute
        /*!
            \param sAttrName attribute name
            \param sVal value
        */
        template <class T>
        void SetValue(const QString& sAttrName, const T& sVal)
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
        explicit CCmdGUIRefresh(const CXMLData::TXMLData& pData);

    private:
        QDomElement        m_DERefresh;
    };

    //! Notification to client to refresh SFPL data (execution)
    class CCmdGUIRefreshExe
        : public CCmdGUIRefresh
    {
    public:
        //! Constructor
        /*!
            \param pData smart pointer to CXMLData
            \param refMaster reference to CMaster
        */
        CCmdGUIRefreshExe(
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

#endif // CMDGUIREFRESH_H
