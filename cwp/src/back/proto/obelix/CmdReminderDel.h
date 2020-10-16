#ifndef CMDREMINDERDEL_H
#define CMDREMINDERDEL_H

#include "proto/TCP/XMLCmd.h"
#include "core/airplane/Reminder.h"

class CMaster;
namespace obelix
{
    //! Delete reminder(creator)
    class CCmdReminderDel
        : public CXMLCmd
    {
    public:
        /*! \name String constants */
        //@{
        static const char XML_CMDNAME[];
        static const char XML_REMINDER[];
        static const char XML_AIRID[];
        static const char XML_REMID[];

        static const char XML_REMTYPE[];
        static const char XML_REMVALUE[];
        //@}
        //! Constructor
        /*!
            \param uiAirId airplane identifeir
            \param uiRemId reminder identifier
        */
        CCmdReminderDel(quint32 uiAirId, qint32 iRemId);
        //! Get airplane identifeir
        /*!
            \return airplane identifeir
        */
        quint32 GetAirID() const;
        //! Get reminder identifeir
        /*!
            \return reminder identifeir
        */
        qint32 GetRemID() const;

        //! Set reminder type
        /*!
            \return reminder type
        */
        CReminder::ReminderType GetRemType() const;

        void SetRemValue(QString value){m_DARemValue.setValue(value);}
        //! Get reminder value
        /*!
            \return reminder value
        */
        QString GetRemValue() const;
        //! Set reminder type
        /*!
            \param val reminder type
        */
        void SetRemType(qint32 val);

    protected:
        //! Constructor for CCmdReminderDelExe
        /*!
            \param pData smart pointer to CXMLData
        */
        explicit CCmdReminderDel(const CXMLData::TXMLData& pData);

    private:
        QDomElement        m_DEReminder;
        QDomAttr        m_DARemID;
        QDomAttr        m_DAAirID;
        QDomAttr        m_DARemValue;
    };

    //! Delete reminder(executor)
    class CCmdReminderDelExe
        : public CCmdReminderDel
    {
    public:
        //! Constructor
        /*!
            \param pData smart pointer to CXMLData
            \param refMaster reference to CMaster
        */
        CCmdReminderDelExe(
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

#endif // CMDREMINDERDEL_H
