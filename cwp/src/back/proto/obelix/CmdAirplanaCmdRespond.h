#ifndef CMDAIRPLANACMDRESPOND_H
#define CMDAIRPLANACMDRESPOND_H

#include "proto/TCP/XMLCmd.h"
#include "core/airplane/Reminder.h"
#include "obelix/ObRespond.h"

class CMaster;
namespace obelix
{
    //! Airplane management(creation)
    class CCmdAirplaneCmdRespond
        : public CXMLCmd
    {
    public:
        /*! \name String constants */
        //@{
        static const char XML_CMDNAME[];
        static const char XML_AIR[];

        static const char XML_AIRID[];
        static const char XML_CMDTYPE[];
        static const char XML_VALUE[];
        static const char XML_ALTVALUE[];
        //@}

        enum CmdType {CODE, Vz, Roll, Cfl, Tfl, HDG, spd1, spd2, NextPoint};

        //! Constructor
        /*!
            \param uiId airplane identifeir
            \param sOperName operator name, which controls this airplane
        */
        CCmdAirplaneCmdRespond(
                quint32 uiId,
                CmdType type,
                QString value);

        quint32 GetAirID() const;
        CCmdAirplaneCmdRespond::CmdType GetType() const;

        QString GetValue()
        {
            return m_DAVal.value();
        }

        void SetAltValue(const QString& sVal)
        {
            m_DEAir.setAttribute(XML_ALTVALUE, sVal);
        }

        QString GetAltValue()
        {
            return m_DEAir.attributeNode(QL1S(XML_ALTVALUE)).value();
        }

    protected:
        //! Constructor for CCmdAirplaneCmdExe
        /*!
            \param pData smart pointer to CXMLData
        */
        CCmdAirplaneCmdRespond(const CXMLData::TXMLData& pData);

    private:
        QDomElement        m_DEAir;
        QDomAttr        m_DAAirID;
        QDomAttr        m_DACmdType;
        QDomAttr        m_DAVal;
    };

    //! Airplane management(execution)
    class CCmdAirplaneCmdRespondExe
        : public CCmdAirplaneCmdRespond
    {
    public:
        //! Constructor
        /*!
            \param pData smart pointer to CXMLData
            \param refMaster reference to CMaster
        */
        CCmdAirplaneCmdRespondExe(
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
#endif // CMDAIRPLANACMDRESPOND_H
