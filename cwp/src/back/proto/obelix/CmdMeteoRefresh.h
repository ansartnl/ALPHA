#ifndef CMDMETEOREFRESH_H
#define CMDMETEOREFRESH_H

#include "proto/TCP/XMLCmd.h"

class CMaster;
namespace obelix
{
    //! Command for meteo zone refresh
    class CCmdMeteoRefresh
        : public CXMLCmd
    {
    public:
        /*! \name String constants */
        //@{
        static const char XML_CMDNAME[];
        static const char XML_METEO[];
        static const char XML_METEO_FIR[];
        //@}

        //! Constructor
        /*!
            \param idFIR id FIR
        */
        CCmdMeteoRefresh(quint32 idFIR);
        //! Get FIR
        /*!
            \return id FIR
        */
        quint32 GetFIR() const;

    protected:
        //! Constructor for CCmdMeteoRefreshExe
        /*!
            \param pData smart pointer to CXMLData
        */
        CCmdMeteoRefresh(const CXMLData::TXMLData& pData);

    private:
        QDomElement     m_DNMeteo;
        QDomAttr        m_DAFirID;
    };

    //! Command for meteo zone refresh(execution)
    class CCmdMeteoRefreshExe
        : public CCmdMeteoRefresh
    {
    public:
        //! Constructor
        /*!
            \param pData smart pointer to CXMLData
            \param refMaster reference to CMaster
        */
        CCmdMeteoRefreshExe(
                const CXMLData::TXMLData& pData,
                CMaster& refMaster);
        /*! \name Implementation of CXMLCmd interface */
        //@{
        virtual CXMLResp::TXMLResp Execute();
        //@}

    private:
        CMaster& m_refMaster;
    };
}

#endif // CMDMETEOREFRESH_H
