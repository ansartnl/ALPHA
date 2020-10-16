#ifndef OLDI_MESSAGE_H
#define OLDI_MESSAGE_H

#include <QObject>
#include <QDateTime>
#include <QStringList>

//! Namespace contains constants to work with OLDI
namespace oldi
{
    //@{
    //! OLDI message type
    const char MES_ABI[] = "ABI";
    const char MES_ACT[] = "ACT";
    const char MES_COD[] = "COD";
    const char MES_COF[] = "COF";
    const char MES_ERR[] = "ERR";
    const char MES_FPL[] = "FPL";
    const char MES_HOP[] = "HOP";
    const char MES_INF[] = "INF";
    const char MES_LAM[] = "LAM";
    const char MES_MAC[] = "MAC";
    const char MES_MAS[] = "MAS";
    const char MES_ROF[] = "ROF";
    const char MES_STS[] = "STS";
    const char MES_URZ[] = "URZ";
    //@}

    //@{
    //! OLDI message INF type
    const char INF_ACT[]        = "ACT";
    const char INF_ASSUME[]     = "ASSUME";
    const char INF_FPL[]        = "FPL";
    const char INF_DEL_SFPL[]   = "DELETE_SFPL";
    const char INF_UNDO_SFPL[]  = "UNDO_SFPL";
    //@}
}

struct OLDIMesData
{
    OLDIMesData()
    : iMesNum(0), bAnswer(true)
    , dtMes(QDateTime::currentDateTime()) {}

    quint16 iMesNum;
    bool    bAnswer;
    QString sType;
    QString sTo;
    QString sFrom;
    QString sDataPart;
    QString sOLDIMes;
    QDateTime dtMes;

    bool operator == (const OLDIMesData& ref2nd) const
    { return
      iMesNum == ref2nd.iMesNum
      && bAnswer == ref2nd.bAnswer
      && sType == ref2nd.sType
      && sTo == ref2nd.sTo
      && sFrom == ref2nd.sFrom
      && sDataPart == ref2nd.sDataPart
      && sOLDIMes == ref2nd.sOLDIMes
      && dtMes == ref2nd.dtMes;
    }
};

struct OLDIMesDataABI
{
    OLDIMesDataABI()
        : iMesNum(0), iNum(1) {}

    quint16 iMesNum;
    QString sTo;
    QString sFrom;

    QString sACID;
    QString sFRules;
    QString sTFlight;
    int     iNum;
    QString sTPlane;
    QString sTurbulence;
    QString sEquipment;
    QString sADEP;
    QDateTime dtADEP;
    QString sSpeed;
    QString sLevel;
    QString sRoute;
    QString sADES;
    QString str_dtADES;
    QString sAlt1;
    QString sAlt2;
    QString sOther;
    QString sFirName;

    QString sCODE;
    QString sCOPin;
    QString sCOPinLevel;
    QTime   dtETOin;
};

#ifdef CWP_PRJ
class CSFPL;
#endif

//! OLDI message
class OLDIMessage : public QObject
{
    Q_OBJECT
public:
    //! Constructor
    /*!
      \param pData a message data
      \param parent a parent QObject
    */
    explicit OLDIMessage(const OLDIMesData& pData, QObject* parent = 0);
    //! Constructor
    /*!
      \param sData a message data in OLDI format
      \param parent a parent QObject
    */
    explicit OLDIMessage(const QString& sData, QObject* parent = 0);
    virtual ~OLDIMessage();

    //@{
    //! Message procedure
    //! It is answer to this message?
    /*!
      \param pMesLAM a reply message
    */
    virtual bool isAnswer(const OLDIMessage* const pMesLAM);
    //! Cancel message
    virtual void Cancel();
    //! Is executed received message?
    virtual bool isExecuted();
    //! Message timeout
    virtual void TimeOut();
    //! Create LAM message
    virtual OLDIMesData GetLAM(quint16 iMesNumLAM) const;
    //@}

    OLDIMesData& GetMesSourceData()
    { return m_Data; }
    OLDIMesData GetMesData() const
    { return m_Data; }
    void SetMesData(const OLDIMesData& data)
    { m_Data = data; }

    void SetExecuted(bool bExecute = true)
    { m_bExecute = bExecute; }

    //! Parse message on header and data part
    OLDIMesData ParseHeader(const QString& sMes) const;

    bool operator == (const OLDIMessage& ref2nd) const;

#ifdef CWP_PRJ
    static OLDIMesDataABI GetABIfromSFPL(const QSharedPointer<CSFPL>& pSFPL);
    static void SetABItoSFPL(const OLDIMesDataABI& dataABI, QSharedPointer<CSFPL>& pSFPL);
#endif

signals:
    void messageAnswer(const OLDIMessage*, const QStringList&);
    void messageTimeOut(const OLDIMessage*);

private:
    Q_DISABLE_COPY(OLDIMessage)

    OLDIMesData     m_Data;
    bool            m_bExecute;
};

#endif // OLDI_MESSAGE_H
