#ifndef CMDSTANDCHANGED_H
#define CMDSTANDCHANGED_H

#include "proto/TCP/XMLCmd.h"

class CMaster;

namespace obelix
{
    //! Notifiaction to server about changing stand state
    class CCmdStandChanged
        : public CXMLCmd
    {
    public:
        /*! \name String constants */
        //@{
        static const char XML_CMDNAME[];
        static const char XML_STAND[];
        static const char XML_ID[];
        static const char XML_AIRPORT[];
        static const char XML_NAME[];
        static const char XML_ACTION[];
        static const char XML_FPLNAME[];
        //@}
        //! Constructor
        /*!
            \param id - key
            \param airport - airport name with this stand
            \param stand - stand name
            \param state - specyfies if stand has become free or occupied
        */
        CCmdStandChanged(quint32 id, const QString& airport, const QString& stand, quint32 state, const QString& fplName);

        quint32 GetID() const;
        QString GetAirport() const;
        QString GetStand() const;
        quint32 GetState() const;
        QString GetFplName() const;

    protected:
        //! Constructor for CCmdStandChangedExe
        /*!
            \param pData smart pointer to CXMLData
        */
        explicit CCmdStandChanged(const CXMLData::TXMLData& pData);

    private:
        QDomElement     m_DNStand;
        QDomAttr        m_DAID;
        QDomAttr        m_DAAirport;
        QDomAttr        m_DAStand;
        QDomAttr        m_DAAction;
        QDomAttr        m_DAFplName;
    };

    //! Notifiaction to client about wish to transmist airplane control from another operator(execution)
    class CCmdStandChangedExe
        : public CCmdStandChanged
    {
    public:
        //! Constructor
        /*!
            \param pData smart pointer to CXMLData
            \param refMaster reference to CMaster
        */
        CCmdStandChangedExe(
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

#endif // CMDSTANDCHANGED_H
