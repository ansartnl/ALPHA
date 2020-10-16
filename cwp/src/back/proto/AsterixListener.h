#ifndef ASTERIX_LISTENER_H
#define ASTERIX_LISTENER_H

#include <QUdpSocket>
#include <QList>
#include <QTime>
#include "common/AirplaneState.h"
#include "asterixdata.h"

//! Namespace contains classes to work with asterix protocol cat.62
namespace asterix
{

//! Asterix listener
class CSocketListener : public QObject, public IAsterixDataSrc
{
    Q_OBJECT
public:
    //! Constructor
    /*!
    \param pParent pointer to data handler
    \param qsIP address for listening
    \param quiPort port for listening
    */
    CSocketListener(IParent* pParent, const QString& qsIP, quint16 quiPort, bool modeS, bool log);
    //! Destructor
    virtual ~CSocketListener();
    //! Start listening
    virtual void Start();
    //! Stop listening
    virtual void Stop();

    //! Block for incoming messages.
    virtual void BlockMessages(bool on)
    { mIsMessagesBlocked = on; }

protected slots:
    //! Slot to QUdpSocket::readyRead() signal
    virtual void OnUDPData();

private:
    void addToLog(const QString &key, const QVariant &value);
    void addToLog(const QString &key);
    void processCat004(const AsterixData &aData);
    void processCat062(const AsterixData &aData);
    void processCat253(const AsterixData &aData);

protected:
    //@}
    //! Pointer to data handler
    IParent* m_pParent;
    //! Host name for listen
    QHostAddress m_Host;
    //! Port for listening
    quint16 m_ushPort;
    //! Low-level UDP socket
    QUdpSocket m_Socket;
    /** State of blocking incoming messages */
    bool       mIsMessagesBlocked;

    bool m_ModeS;
private:
    bool m_Logging;
};

}

#endif // ASTERIX_LISTENER_H
