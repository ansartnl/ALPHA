#ifndef IZMQT_ASYNC_CONNECTOR_H
#define IZMQT_ASYNC_CONNECTOR_H

#include <QObject>
#include <QVariant>
#include <QMap>
#include <QDateTime>
#include "../izmq_global.h"
class QTimer;


namespace izmqt {

class Pinger;
class Ponger;
class IProtoPingAccessor;


class RouterServer;

//! Concrete implementation of ponger for router server.
class IZMQT_EXPORT RouterPonger : public QObject {
    Q_OBJECT

public:
    RouterPonger(RouterServer* router, QObject* parent = 0);

    //! Set timeout interval for losing the connection.
    /*!
        \param i Interval (msecs). Default = 5 sec.
    */
    void setInterval(int i);

    //! Returns true if connection with 'identity' queue exists.
    bool isConnected(QString identity) const { return mConnectedQueues.contains(identity); }

    //! Set specific to protocol ping/pong parser/generator.
    /*! \ note Do not destroy accessor object. It will be destroyed inside.
    */
    void setPingAccessor(IProtoPingAccessor* acc);

signals:
    /*! Informs the connector about stable connection for specific queue.
     *  \param identity Identifier of the queue.
    */
    void connected(QString identity);
    /*! Connection lost with specific queue.
     *  \param identity Identifier of the queue.
    */
    void disconnected(QString identity);

private slots:
    void onRequestReceived(QVariant, QString);
    void onPongReceived(QVariant, QVariant);

    //! Handler for checking validness of connections.
    void onTimeout();

private:
    typedef QMap<QString, QDateTime>    TConnectedMap;

    /** Reply server queue */
    RouterServer*    mRouterServer;
    /** Pinger */
    Ponger*          mPonger;
    /** Lose connection check timer */
    QTimer*          mDisconnectTimer;
    /** Map with connected queues */
    TConnectedMap   mConnectedQueues;
};


class DealerClient;

//! Controls connection state (connected/disconnected).
class IZMQT_EXPORT DealerPinger : public QObject {
    Q_OBJECT

public:
    DealerPinger(DealerClient* client, QObject* parent = 0);

    //! Retrieve pointer to current dealer client.
    DealerClient* dealerClient() const { return mDealerClient; }

    //! Set specific to protocol ping/pong parser/generator.
    /*! \ note Do not destroy accessor object. It will be destroyed inside.
    */
    void setPingAccessor(IProtoPingAccessor* acc);

    //! Set timeout interval for ping.
    /*!
        \param i Interval for sending pings (msecs). Default = 3 sec.
    */
    void setInterval(int i);

    //! Start connection controller.
    void start();

    //! Stop watching and restoring the connection.
    void stop();

    //! Returns true if connection is valid.
    bool isConnected() const;

signals:
    //! Informs the connector about stable connection.
    void connected();
    /*! New dealer socket was created before calling disconnected() signal.
     *  \note Dealer socket must be updated on the 'connector' side.
    */
    void disconnected();

private slots:
    void onDisconnected();

private:
    /** Request client queue */
    DealerClient*    mDealerClient;
    /** Address of connection point */
    Pinger*          mPinger;
};

}   // namespace izmqt

#endif // IZMQT_ASYNC_CONNECTOR_H
