#ifndef OLDI_PROC_H
#define OLDI_PROC_H

#include <QObject>
#include <QMap>
#include <QSharedPointer>
#include <QSqlDatabase>
#include "OLDIMessage.h"

class QTimer;

//! OLDI processor
class OLDIProc : public QObject
{
    Q_OBJECT
public:
    //! Constructor
    /*!
      \param base a database object
      \param sOwnerSenderName a sender's ID
      \param iSecTimeout time out in seconds
      \param parent a parent QObject
    */
    explicit OLDIProc(QSqlDatabase base, QString sOwnerSenderName = "Unknown", quint32 iSecTimeout = 10, QObject* parent = 0);
    ~OLDIProc();

    //! Send message
    void SendMessage(QSharedPointer<OLDIMessage> pMes, bool bWait = true);

    //! Message number
    quint16 GetNextMesNum();

    //!@{
    //! Working as FDP server
    void SetIsFDP(bool bIsFDP)
    { m_bIsFDP = bIsFDP; }
    bool IsFDP() const
    { return m_bIsFDP; }
    //!@}

signals:
    void recieveMessage(QSharedPointer<OLDIMessage>);

private slots:
    void OnAckTimer();

private:
    void RecieveSTSMes(QSharedPointer<OLDIMessage> pMes);
    QSharedPointer<OLDIMessage> CreateMessage(const QString& sType, const QString& sOLDIMes) const;

private:
    Q_DISABLE_COPY(OLDIProc)

    struct MesMapKey
    {
        MesMapKey(quint16 num, QString from, QString to)
        { qNum = num, sFrom = from, sTo = to; }

        quint16 qNum;
        QString sFrom;
        QString sTo;

        bool operator <(const MesMapKey& other) const
        {
            if(qNum != other.qNum) return qNum < other.qNum;
            if(sFrom != other.sFrom) return sFrom < other.sFrom;
            if(sTo != other.sTo) return sTo < other.sTo;
            return false;
        }
    };

    typedef QMap<MesMapKey, QSharedPointer<OLDIMessage> > TMesMap;
    TMesMap     m_mapSendMes;
    TMesMap     m_mapReceiveMes;
    QTimer*     m_pAckTimer;
    quint16     m_mesnum;

    QSqlDatabase    m_DB;
    QString         m_sOwnerSenderName;
    quint32         m_iTimeOut;

    bool        m_bIsFDP;

private:
    bool CheckSTSMes(QSharedPointer<OLDIMessage> pMes, TMesMap& mapMes, QList<QString>& listCWPforUpdate);
};

#endif // OLDI_PROC_H
