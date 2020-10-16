#ifndef CMDAIRPLANASTAT_H
#define CMDAIRPLANASTAT_H

#include "proto/TCP/XMLCmd.h"
#include "AirplaneData.h"

class CMaster;
class CPlane;
class CAirplane;

namespace obelix
{
    //! Status of an airplane(creation)
    class CCmdAirplaneStat
        : public CXMLCmd
    {
    public:
        /*! \name String constants */
        //@{
        static const char XML_CMDNAME[];
        static const char XML_AIR[];
        static const char XML_AIRID[];
        static const char XML_AIROPER[];
        static const char XML_AIREXEID[];
        static const char XML_AIREXEFPLID[];
        static const char XML_AIRSQUAWK[];
        static const char XML_AIRIAS[];
        static const char XML_AIRFL[];
        static const char XML_AIRVZ[];
        static const char XML_AIRAZIMUTH[];
        static const char XML_AIRBANK[];
        static const char XML_AIRPNTPLAN[];
        static const char XML_AIRPNTID[];
        static const char XML_AIRPNTLONG[];
        static const char XML_AIRPNTLAT[];
        static const char XML_AIRHLDID[];
        static const char XML_AIRHLDSN[];
        static const char XML_AIRSSAID[];
        static const char XML_AIRSSASN[];
        static const char XML_REM[];
        static const char XML_REMID[];
        static const char XML_REMTYPE[];
        static const char XML_REMVALUE[];
        static const char XML_REMCOMPLETE[];
        static const char XML_REMTIME[];
        static const char XML_AIRSPI[];
        static const char XML_AIREQUIPMENT[];
        static const char XML_CUSTOMSTUNT[];
        //@}

        static const char XML_TYPE[];
        static const char XML_DATAPART[];
        static const char XML_OLDIMES[];
        static const char XML_TO[];
        static const char XML_FROM[];
        static const char XML_ACID[];
        static const char XML_SFPLID[];
//        bool    bAnswer;
//            QString sType;
//            QString sDataPart;
//            QString sOLDIMes;
//            QDateTime dtMes;
//        /////////////////////
//        quint16 iMesNum;
//            QString sTo;
//            QString sFrom;
//            QString sACID;
        //! Constructor
        /*!
            \param uiId airplane identifeir
            \param sOperName operator name, whom belongs this airplane
            \param pPlane smart pointer to CPlane
        */
        CCmdAirplaneStat(
                quint32 uiId,
                const QString& sOperName,
                const QSharedPointer<CAirplane> &pPlane);

        //! Constructor
        /*!
            \param uiId airplane identifeir
            \param sOLDIMes OLDI message type
        */
        CCmdAirplaneStat(
                quint32 uiId,
                const QString& sOLDIMes);

        //! Set the value of an XML attribute
        /*!
            \param sAttrName attribute name
            \param sVal value
        */
        template <class T>
        void SetValue(const QString& sAttrName, const T& sVal)
        {
            m_DEAir.setAttribute(sAttrName, sVal);
        }
        //! Get the value of an XML attribute
        /*!
            \param sAttrName attribute name
            \return value
        */
        QString GetValue(const QString& sAttrName);
        //! Get airplane ID
        quint32 GetAirID();
        //! Check presence of an attribute
        /*!
            \param sAttrName attribute name
            \return true if presents
        */
        bool IsValue(const QString& sAttrName);

    protected:
        //! Constructor for CCmdAirplaneStatExe
        /*!
            \param pData smart pointer to CXMLData
        */
        CCmdAirplaneStat(const CXMLData::TXMLData& pData);

        QDomElement        m_DERem;

    private:
        QDomElement        m_DEAir;
    };

    //! Status of an airplane(execution)
    class CCmdAirplaneStatExe
        : public CCmdAirplaneStat
    {
    public:
        //! Constructor
        /*!
            \param pData smart pointer to CXMLData
            \param refMaster reference to CMaster
        */
        CCmdAirplaneStatExe(
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

#endif // CMDAIRPLANASTAT_H
