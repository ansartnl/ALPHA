#ifndef CMDREMINDERLIST_H
#define CMDREMINDERLIST_H

#include "proto/TCP/XMLCmd.h"
#include "core/airplane/Reminder.h"

class CMaster;
namespace obelix
{
    //! Send reminder list to client(creator)
    class CCmdReminderList
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
        static const char XML_LIST[];

        static const char XML_REMTIME[];
        static const char XML_REMCLOSABLE[];
        //@}
        //! Constructor
        /*!
            \param uiAirId airplane identifeir
            \param pReminder smart pointer to CReminder
        */
        CCmdReminderList(
                quint32 uiAirId);

        //! Get airplane identifeir
        /*!
            \return airplane identifeir
        */
        quint32 GetAirID();
        //! Get reminder list
        /*!
          \return reminder list
          */
        QDomNodeList GetReminders();
        //! Constructs reminder from QDomNode
        /*!
          \param QDomNode which contains reminder data
          \return smart pointer to reminder
          */
        QSharedPointer<CReminder> FormReminder(QDomNode rem);
        //! Appends reminder to message
        /*!
          \param smart pointer to reminder
          */
        void InsertReminder(QSharedPointer<CReminder> pReminder);
        //! Get reminder type
        /*!
            \return reminder type
        */
        CReminder::ReminderType GetRemType(QDomNode node) const;
        //! Get reminder value
        /*!
            \return reminder value
        */
        QString GetRemValue(QDomNode node) const;

        qint32 GetLevel(QDomNode node) const;
        void SetLevel(qint32 level);

        bool GetComplete(QDomNode node) const;
        void SetComplete(bool isComplete);

        QDateTime GetRemTime(QDomNode node) const;
        bool GetRemClosable(QDomNode node) const;

    protected:
        //! Constructor for CCmdReminderAddExe
        /*!
            \param pData smart pointer to CXMLData
        */
        explicit CCmdReminderList(const CXMLData::TXMLData& pData);

    private:
        QDomElement        m_DEReminder;
        QDomElement        m_DEList;
        QDomAttr        m_DAAirID;
        QDomAttr        m_DARemType;
        QDomAttr        m_DARemValue;
        QDomAttr        m_DARemLevel;
        QDomAttr        m_DARemComplete;
        QDomAttr        m_DARemTime;
        QDomAttr        m_DARemClosable;
    };

    //! Send reminder list to client(executor)
    class CCmdReminderListExe
        : public CCmdReminderList
    {
    public:
        //! Constructor
        /*!
            \param pData smart pointer to CXMLData
            \param refMaster reference to CMaster
        */
        CCmdReminderListExe(
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

#endif // CMDREMINDERLIST_H
