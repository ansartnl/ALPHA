#ifndef IZMQT_ROUTER_SERVER_H
#define IZMQT_ROUTER_SERVER_H

#include <QObject>
#include <QVariant>
#include <QList>
#include <QByteArray>
#include <QString>

#include "../properties/StartedProperty.h"
#include "../izmq_global.h"

namespace nzmqt {
class ZMQSocket;
}

namespace izmqt {

class IZMQT_EXPORT RouterServer : public QObject, public StartedProperty {
    Q_OBJECT

public:
    //! c-tor
    explicit RouterServer(const QString& address, QObject* parent = 0);

    //! Binds reply server to specified address.
    /*!
     *  \return 0 - when succedded,
     *          -1 - unknown error,
     *          otherwise - error code (refer to zeromq documentation)
    */
    int start(const QString& address);

signals:
    //! Request received from client.
    /*!
     *  \param QVariant - message data
     *  \param QString - client's identity (unique client identifier)
    */
    void requestReceived(QVariant, QString);

    void receivedSize(quint32);

public slots:
    //! Binds reply server.
    /*!
     *  \return 0 - when succedded,
     *          -1 - unknown error,
     *          otherwise - error code (refer to zeromq documentation)
    */
    int start();

    //! Unbinds reply server.
    void stop();

    //! Restart reply server.
    void restart();

    //! Sends reply to client.
    /*!
     *  \param QVariant Data frame
     *  \param Identity of the client (receiver)
    */
    bool reply(QVariant, QString);

private slots:
    /** Request received from the client */
    void onRequestReceived(const QList<QByteArray>& request);

private:
    /** Binding address */
    QString             mAddress;
    /** Communication socket */
    nzmqt::ZMQSocket*    mSocket;
};

}   // namespace izmqt

#endif // IZMQT_ROUTER_SERVER_H
