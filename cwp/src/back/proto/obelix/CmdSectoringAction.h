#ifndef CMDSECTORINGACTION_H
#define CMDSECTORINGACTION_H

#include "proto/TCP/XMLCmd.h"
#include "core/airplane/Reminder.h"
#include "obelix/ObRespond.h"

class CMaster;
namespace obelix
{
    //! Sectoring change message(creator)
    class CCmdSectoringAction
        : public CXMLCmd
    {
    public:
        /*! \name String constants */
        //@{
        static const char XML_CMDNAME[];
        static const char XML_SECTORING[];
        static const char XML_ACTION[];

        enum SectoringAction{FDP_agreed, FDP_rejected, FDP_timeout, Force, Undo};
        //@}
        //! Constructor
        /*!
            \param action Sectoring action
        */
        CCmdSectoringAction(SectoringAction action);

        //! Get action type
        /*!
            \return action type
        */
        SectoringAction GetAction() const;

    protected:
        //! Constructor for CCmdReminderAddExe
        /*!
            \param pData smart pointer to CXMLData
        */
        explicit CCmdSectoringAction(const CXMLData::TXMLData& pData);

    private:
        QDomElement        m_DESectoring;
        QDomAttr        m_DAAction;
    };

    //! Sectoring change message(executor)
    class CCmdSectoringActionExe
        : public CCmdSectoringAction
    {
    public:
        //! Constructor
        /*!
            \param pData smart pointer to CXMLData
            \param refMaster reference to CMaster
        */
        CCmdSectoringActionExe(
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

#endif // CMDSECTORINGACTION_H
