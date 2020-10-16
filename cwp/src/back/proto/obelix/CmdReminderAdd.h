#ifndef CMDREMINDERADD_H
#define CMDREMINDERADD_H

#include "proto/TCP/XMLCmd.h"
#include "core/airplane/Reminder.h"

class CMaster;
namespace obelix
{
    //! Add reminder(creator)
    class CCmdReminderAdd
        : public CXMLCmd
    {
    public:
        /*! \name String constants */
        //@{
        static const char XML_CMDNAME[];
        static const char XML_REMINDER[];
        static const char XML_AIRID[];
        static const char XML_REMTYPE[];
        static const char XML_REMVALUE[];

        static const char XML_REMLEVEL[];
        static const char XML_REMCOMPLETE[];

        static const char XML_REMTIME[];
        static const char XML_REMCLOSABLE[];
        //@}
        //! Constructor
        /*!
            \param uiAirId airplane identifeir
            \param pReminder smart pointer to CReminder
        */
        CCmdReminderAdd(
                quint32 uiAirId,
                const QSharedPointer<CReminder>& pReminder);

        //! Get airplane identifeir
        /*!
            \return airplane identifeir
        */
        quint32 GetAirID() const;
        //! Get reminder type
        /*!
            \return reminder type
        */
        CReminder::ReminderType GetRemType() const;
        //! Get reminder value
        /*!
            \return reminder value
        */
        QString GetRemValue() const;

        qint32 GetLevel() const;
        void SetLevel(qint32 level);

        bool GetComplete() const;
        void SetComplete(bool isComplete);

        QDateTime GetRemTime() const;
        bool GetRemClosable() const;

    protected:
        //! Constructor for CCmdReminderAddExe
        /*!
            \param pData smart pointer to CXMLData
        */
        explicit CCmdReminderAdd(const CXMLData::TXMLData& pData);

    private:
        QDomElement        m_DEReminder;
        QDomAttr        m_DAAirID;
        QDomAttr        m_DARemType;
        QDomAttr        m_DARemValue;
        QDomAttr        m_DARemLevel;
        QDomAttr        m_DARemComplete;
        QDomAttr        m_DARemTime;
        QDomAttr        m_DARemClosable;
    };

    //! Add reminder(executor)
    class CCmdReminderAddExe
        : public CCmdReminderAdd
    {
    public:
        //! Constructor
        /*!
            \param pData smart pointer to CXMLData
            \param refMaster reference to CMaster
        */
        CCmdReminderAddExe(
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

#endif // CMDREMINDERADD_H
