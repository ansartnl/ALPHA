#ifndef CMDSETSECTOR_H
#define CMDSETSECTOR_H

#include "proto/TCP/XMLCmd.h"

class CMaster;

namespace obelix
{
    //! Sector change operation(creation)
    class CCmdSetSector
        : public CXMLCmd
    {
    public:
        /*! \name String constants */
        //@{
        static const char XML_CMDNAME[];
        static const char XML_SECTOR[];
        static const char XML_INDEX[];
        static const char XML_SECTORNAME[];
        //@}

        //! Constructor
        /*!
            \param iIndex index of sector selected
        */
        explicit CCmdSetSector(const QString &sIndex);

        //! Get index
        /*!
            \return index
        */
        QString GetIndex() const;

        void SetSectorName(const QString& sName);

        QString GetSectorName();

    protected:
        //! Constructor for CCmdHelloExe
        /*!
            \param pData smart pointer to CXMLData
        */
        explicit CCmdSetSector(const CXMLData::TXMLData& pData);

    private:
        QDomElement        m_DNSector;
        QDomAttr        m_DAIndex;
        QDomAttr        m_DASectorName;
    };

    //! Sector change operation(creation)
    class CCmdSetSectorExe
        : public CCmdSetSector
    {
    public:
        //! Constructor
        /*!
            \param pData smart pointer to CXMLData
            \param refMaster reference to CMaster
        */
        CCmdSetSectorExe(
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

#endif // CMDSETSECTOR_H
