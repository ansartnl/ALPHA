#ifndef CMDAIRPLANADEL_H
#define CMDAIRPLANADEL_H

#include "proto/TCP/XMLCmd.h"

class CMaster;
namespace obelix
{
    class CConnect;
    //! Notifacation about the end of airplane simulation(creation)
    class CCmdAirplaneDel
        : public CXMLCmd
    {
    public:
        /*! \name String constants */
        //@{
        static const char XML_CMDNAME[];
        static const char XML_AIR[];
        static const char XML_AIRID[];
        //@}
        //! Constructor
        /*!
            \param uiId airplane identifeir
        */
        CCmdAirplaneDel(quint32 uiId);
        //! Get airplane identifeir
        /*!
            \return airplane identifeir
        */
        quint32 GetAirID() const;

    protected:
        //! Constructor for CCmdAirplaneDelExe
        /*!
            \param pData smart pointer to CXMLData
        */
        explicit CCmdAirplaneDel(const CXMLData::TXMLData& pData);

    private:
        QDomElement        m_DNAirplane;
        QDomAttr        m_DAAirID;
    };

    //! Notifacation about the end of airplane simulation(execution)
    class CCmdAirplaneDelExe
        : public CCmdAirplaneDel
    {
    public:
        //! Constructor
        /*!
            \param pData smart pointer to CXMLData
            \param refMaster reference to CMaster
        */
        CCmdAirplaneDelExe(
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

#endif // CMDAIRPLANADEL_H
