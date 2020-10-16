#ifndef CMDSECTORCHANGED_H
#define CMDSECTORCHANGED_H

#include "proto/TCP/XMLCmd.h"

class CMaster;

namespace obelix
{
    //! Sector change operation(creation)
    class CCmdSectorChanged
        : public CXMLCmd
    {
    public:
        /*! \name String constants */
        //@{
        static const char XML_CMDNAME[];
        static const char XML_SECTOR[];
        static const char XML_INDEX[];
        //@}

        //! Constructor
        /*!
            \param iIndex index of sector selected
        */
        explicit CCmdSectorChanged(const QString& sIndex);

        //! Get index
        /*!
            \return index
        */
        QString GetIndex() const;

    protected:
        //! Constructor for CCmdHelloExe
        /*!
            \param pData smart pointer to CXMLData
        */
        explicit CCmdSectorChanged(const CXMLData::TXMLData& pData);

    private:
        QDomElement        m_DNSector;
        QDomAttr        m_DAIndex;
    };

    //! Sector change operation(creation)
    class CCmdSectorChangedExe
        : public CCmdSectorChanged
    {
    public:
        //! Constructor
        /*!
            \param pData smart pointer to CXMLData
            \param refMaster reference to CMaster
        */
        CCmdSectorChangedExe(
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

#endif // CMDSECTORCHANGED_H
