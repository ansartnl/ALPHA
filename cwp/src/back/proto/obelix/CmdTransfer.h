#ifndef CMDTRANSFER_H
#define CMDTRANSFER_H

#include "proto/TCP/XMLCmd.h"
#include <QMap>
#include "Airplane.h"

class CMaster;
class CAirplane;

namespace obelix
{
    //! Notification to client about server repond on transfer operation (creation)
    class CCmdTransfer
        : public CXMLCmd
    {
    public:
        typedef QHash<quint16, QSharedPointer<CAirplane> > TAirplanesMap;
        /*! \name String constants */
        //@{
        static const char XML_CMDNAME[];
        static const char XML_TRANSFER[];
        static const char XML_AIRID[];
        static const char XML_TYPE[];
        static const char XML_OPERNAME[];

        static const char XML_CANCELTRANSFER[];         //bool
        static const char XML_SETCURSECTOR[];           //QString
        static const char XML_SETPLANETYPE[];           //PlaneType
        static const char XML_ASSUMEOPERATOR[];         //void
        static const char XML_SETTRANSFERTIME[];        //void
        static const char XML_SETTRANSITION[];          //QString
        static const char XML_SETOUR[];                 //bool
        static const char XML_DIRECTASSUME[];           //void
        static const char XML_CHANGEOPERATOR[];         //int
        static const char XML_CHANGEOPERATOR_OPERNAME[];//QString

        enum TransferType {Cancel, Assume, Transition, Transfer, TransferTransition,
                           Transfer_out, Transfer_begin, DirectAssume};

        //@}
        //! Constructor
        /*!
            \param uiAirID airplane identifeir
            \param bReset reset flag, if true, then cancel previous TAKE packet
        */
        CCmdTransfer(quint32 uiAirID);//, TransferType type);
        //! Get airplane identifeir
        /*!
            \return airplane identifeir
        */
        quint32 GetAirID() const;
        //! Get transfer type
        /*!
            \return transfer type
        */
        TransferType GetType() const;
        //! Set operator name
        /*!
            \param operator name
          */
        void setOperator(QString sName);
        //! Get operator name
        /*!
            \return operator name
        */
        QString GetOperator() const;
        //! Set the value of an XML attribute
        /*!
            \param sAttrName attribute name
            \param sVal value
        */

        void SetCancelTransfer(bool value);
        void SetCurSector(QString value);
        void SetPlaneType(CAirplaneData::PlaneType value);
        void SetAssumeOperator();
        void SetTransferTime();
        void SetTransition(QString value);
        void SetOur(bool value);
        void SetDirectAssume();
        void SetChangeOperator(int idSector, QString sOperName);

        void CheckCancelTransfer(TAirplanesMap::iterator &itr);
        void CheckCurSector(TAirplanesMap::iterator &itr);
        void CheckPlaneType(TAirplanesMap::iterator &itr);
        void CheckAssumeOperator(TAirplanesMap::iterator &itr);
        void CheckTransferTime(TAirplanesMap::iterator &itr);
        void CheckTransition(TAirplanesMap::iterator &itr);
        void CheckOur(TAirplanesMap::iterator &itr);
        void CheckDirectAssume(TAirplanesMap::iterator &itr);
        void CheckChangeOperator(TAirplanesMap::iterator &itr);

    protected:
        //! Constructor for CCmdTakeExe
        /*!
            \param pData smart pointer to CXMLData
        */
        explicit CCmdTransfer(const CXMLData::TXMLData& pData);

    private:
        QDomElement        m_DNTransfer;
        QDomAttr        m_DAAirID;
        QDomAttr        m_DAType;
        QDomAttr        m_DAOperName;
    };

    //! Notification to client about server repond on transfer operation (execution)
    class CCmdTransferExe
        : public CCmdTransfer
    {
    public:
        //! Constructor
        /*!
            \param pData smart pointer to CXMLData
            \param refMaster reference to CMaster
        */
        CCmdTransferExe(
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
#endif // CMDTRANSFER_H
