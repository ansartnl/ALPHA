#ifndef MASTER_H
#define MASTER_H

#include "Dao.h"
#include "proto/AsterixOld.h"
#include "proto/AsterixListener.h"
#include "proto/xt/XtParent.h"
#include "proto/Obelix.h"

// Workaround
#include "View.h"

#include "main.h"

#include <QStringList>
#include <QScopedPointer>
#include <QTimer>

#include "OLDIProc.h"

#include "proto/Obelix.h"
#include "src/back/core/airplane/AirplaneSTCA.h"

#include "TVATimer.h"

#include "back/ground/IScene.h"
#include "back/ground/GroundStand.h"

#include "back/data/redisdbhelper.h"

class CPlane;
class CRoute;
class Scene;
class GroundScene;
class CView;
class CAirplaneType;
class MasterHelper;
class ORCAMDlg;

namespace Xt
{
    struct Stand;
    struct Vehicle;
}

//! The main class of simulator
class CMaster: public asterix::IParent, public xtProto::IParent
{
public:
    //! Map of planes. Key - plane identifier
    typedef QMap<qint16, QSharedPointer<CPlane> > TPlaneList;
    //! Map of airplanes. Key - airplane identifier
    typedef QHash<quint16, QSharedPointer<CAirplane> > TAirplanesMap;
    //! Constructor
    CMaster();
    virtual ~CMaster();

    //! Initialization function
    /*!
         \param pConfig a CConfig pointer
         */
    virtual void Init();

    //! Function load FIR from database
    //! Function load FIR from database
    /*!
         \param qsIdName a FIR's ID
         \param qsName a FIR's name
         \param bFIR is a FIR or sector
         \param Scene a created object Scene ( returned )
         */
    virtual void loadFromDB(uint uiZoneID, Scene& Scene, IScene& View);
    virtual void saveToDB(Scene& scene);

    virtual void addFormular(quint16 id) = 0;
    virtual void removeFormular(quint16 id) = 0;
    virtual void setFixedFormular(quint16 id, bool fixed) = 0;
    virtual qreal formularLinkAngle() const = 0;
    virtual int formularLinkLength() const = 0;
    virtual int formularLinkMinLength() const = 0;
    virtual int formularLinkMaxLength() const = 0;
    virtual bool formularLinkLengthLimited() const = 0;

    //! Function load list of FIRs from database
    /*!
         \param Zones a list of objects CZone ( returned )
         */
    void GetZonesFromDB(QList<Zone>& Zones) const;

    void GetSectorAirports(QStringList* airports);
    void GetSectorAirportRunways(const QString& airportName, QStringList* runways);

    /*! \name Implementation of the interface asterix::IParent */
    //@{
    virtual void OnAsterixData(CAirplaneState& Data);
    virtual void OnBearingData(CBearingData& Data);
    virtual void OnSTCAData(const QVariant &v);
    //@}

    /*! Implementation of xtProto::IParent interface */
    virtual void OnObjectData(const Xt::TObject& obj);

    //! Stop simulator exercise
    virtual void StopExercise(bool bSameDB);

    //! Meteo from simulator
    //@{
    virtual void BeginSimMeteo(QString sIdFIR, bool bSameDB);
    virtual void EndSimMeteo(QString sIdFIR, bool bSameDB) {}
    //@}

    //! Get scene list
    /*!
         \return scene list
         */
    QSharedPointer<IScene>& GetView()
    {
        return m_ZonesView;
    }
    const QSharedPointer<IScene>& GetView() const
    {
        return m_ZonesView;
    }
//    QSharedPointer<GroundScene>& GetGroundView()
//    {
//        return m_GroundView;
//    }
    //! Get pointer to scene by name
    //QSharedPointer<Scene> GetGUIScene(const QString sFIR) const;
    //! Load scene from database by name
    /*!
         \param sFIR scene name
         \return pointer to scene
         */
    //QSharedPointer<Scene> LoadScene(const QString sFIR);

    QSharedPointer<OLDIProc> GetOLDIProc() const
    { return m_pOLDIProc; }

    //! Block/unblock receiving of messages from the network.
    void BlockNetworkMessages(bool on);

    //! Get obelix server
    /*!
        \return reference to obelix server
    */
    obelix::CServer& GetObServer() { return m_ObServer; }
    //! Get obelix client
    /*!
        \return reference to obelix client
    */
    obelix::CConnect& GetObClient() { return m_ObClient; }

    //! Get plane list
    /*!
        \return plane list
    */
    TAirplanesMap* GetPlanes() { return m_Planes; }

    //! Set plane list
    /*!
        \return plane list
    */
    void SetPlanes(TAirplanesMap* value) { m_Planes = value; }

    //! Attempt to reconnect obelix client
    void ObelixReconnect();
    //! Send information about lane to obelix clients
    /*!
        \param pPlane pointer to plane
    */
    void SendPlaneState(const QSharedPointer<CAirplane>& pPlane);

    //! Sets or resets that this client has initiated sectoring
    /*!
      \param bInitiator true if has inited sectoring
      */
    void SetSectoringInitiator(bool bInitiator);
    //! Gets if this client has initiated sectoring
    bool IsSectoringInitiator(){return bSectoringInitiator;}

    //! Load Ground airport data from XML file
    bool GetAirportDataFromXML(GroundScene& scene);

    void VehicleChanged(const Xt::Vehicle* xtVehicle);
    //! Stands
    void StandChanged(const Xt::Stand* xtstand, bool bSend = true);
    void StandEnd();
    virtual void StandChangedGUI(const GroundStand& stand) {}

    inline int currentSectorId() const
    {return mCurrentSectorId;}
    inline void setCurrentSectorId(int id)
    {mCurrentSectorId = id;}

    QString getSI_FL() const;
    void setSI_FL(QString val);

    RedisDBHelper* getRedisDB()
    { return mRedisDB; }

    void showORCAMDialog();

    QSharedPointer<CPoint> findPointByName(const QString& name) const;

protected:
    //! Send to obelix clients that plane was deleted
    /*!
         \param pPlane pointer to plane
         */
    void _SendDeletePlane(const QSharedPointer<CPlane>& pPlane);

    //! Update airplane information
    /*!
     \param AirplaneState airplane information
     */
    void UpdateAirplane(CAirplaneState& AirplaneState);

    //! Add pointer to airplane
    /*!
     \param pAirplane pointer to airplane
     */
    void AddAirplane(const QSharedPointer<CAirplane>& pAirplane);

//    QSharedPointer<CView> m_ZonesView;
    QSharedPointer<IScene> m_ZonesView;
//    QSharedPointer<GroundScene> m_GroundView;

    QSharedPointer<IAsterixDataSrc> m_pAsterParser;     // Old asterix parser
    QSharedPointer<IAsterixDataSrc> m_pAsterListener;   // New asterix parser based on "asterix" library
    QSharedPointer<IAsterixDataSrc> m_pBearReader;
    QSharedPointer<IAsterixDataSrc> m_pSTCAReader;
    QSharedPointer<xtProto::IListenerParent> m_pTcpListener;
    QSharedPointer<xtProto::IListenerParent> m_pUdpListener;
    asterix::CBearGenerator m_BearGenerator;
    QList<core::CWind> m_Winds;
    int m_iBeforehandFly;
    QSharedPointer<OLDIProc>        m_pOLDIProc;
    stca::CListSTCA m_listSTCA;
    QMap<quint32, GroundStand>      m_mapStand;
    bool                            m_bStandSend;

    obelix::CServer             m_ObServer;
    obelix::CConnect            m_ObClient;
    TAirplanesMap               *m_Planes;
    tva::CTimer                 m_CalcTimer;

    bool bSectoringInitiator;
    int mCurrentSectorId;

    bool mSIcomboEnable;

    RedisDBHelper* mRedisDB;
    //RedisDBHelper* mRedisDB2;

    ORCAMDlg* mORCAMDialog;

private:
    QScopedPointer<MasterHelper> helper;

    friend class MasterHelper;
};

class MasterHelper: public QObject
{
    Q_OBJECT
public:
    MasterHelper(CMaster *master) :
            mMaster(master)
    {
        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(onCheckAirplane()));
        timer->start(4000);
    }
    virtual ~MasterHelper()
    {
    }
private slots:
    void onCheckAirplane();

private:
    CMaster *mMaster;
};
#endif // MASTER_H
