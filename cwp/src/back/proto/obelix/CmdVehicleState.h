#ifndef CMDVEHICLESTATE_H
#define CMDVEHICLESTATE_H

#include "proto/TCP/XMLCmd.h"
//#include "back/core/Vehicle.h"
#include "back/common/VehicleState.h"

class CMaster;
namespace obelix
{
    class CCmdVehicleState
        : public CXMLCmd
    {
    public:
        /*! \name String constants */
        //@{
        static const char XML_CMDNAME[];
        static const char XML_VEHICLE[];
        static const char XML_ID[];
        static const char XML_NAME[];
        static const char XML_IAS[];
        static const char XML_LONGITUDE[];
        static const char XML_LATITUDE[];
        static const char XML_DEL[];
        static const char XML_HIDDEN[];
        static const char XML_LIST[];
        //@}
        //! Constructor
        /*!
            \param uiAirId airplane identifeir
            \param pReminder smart pointer to CReminder
        */
        CCmdVehicleState();

        bool IsValue(const QString& sAttrName);

        QDomNodeList GetVehicles();
        void SetDel(bool bDelete);
        bool GetDel() const;

        VehicleState FormVehicle(QDomNode rem);

        void InsertVehicle(quint32 id, const QString& name, double ias, const CCoord &coord, bool isHidden);

    protected:
        //! Constructor for CCmdReminderAddExe
        /*!
            \param pData smart pointer to CXMLData
        */
        explicit CCmdVehicleState(const CXMLData::TXMLData& pData);

    private:
        QDomElement        m_DEList;
    };

    //! Send reminder list to client(executor)
    class CCmdVehicleStateExe
        : public CCmdVehicleState
    {
    public:
        //! Constructor
        /*!
            \param pData smart pointer to CXMLData
            \param refMaster reference to CMaster
        */
        CCmdVehicleStateExe(
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

#endif // CMDVEHICLESTATE_H
