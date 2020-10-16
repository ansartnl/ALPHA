#ifndef MESSAGEQUEUEBROKER_H
#define MESSAGEQUEUEBROKER_H

#include <QObject>
#include <QString>

#include "../properties/StartedProperty.h"
#include "../izmq_global.h"

namespace nzmqt {
class ZMQSocket;
}

namespace izmqt {

class IZMQT_EXPORT MessageQueueBroker : public QObject, public StartedProperty {
    Q_OBJECT

public:
    //! c-tor
    explicit MessageQueueBroker(const QString& frontAddress, const QString& backAddress, QObject* parent = 0);

    //! Binds broker to specified front and back addresses.
    /*!
     *  \return 0 - when succedded,
     *          -1 - unknown error,
     *          otherwise - error code (refer to zeromq documentation)
    */
    int start(const QString& frontAddress, const QString& backAddress);

public slots:
    //! Binds broker.
    /*!
     *  \return 0 - when succedded,
     *          -1 - unknown error,
     *          otherwise - error code (refer to zeromq documentation)
    */
    int start();

    //! Unbinds broker.
    void stop();

    //! Restart broker.
    void restart();

private slots:
    /** Request received from the client */
    void onRequestReceived(const QList<QByteArray>& request);

    /** Reply received from the server */
    void onReplyReceived(const QList<QByteArray>& reply);

private:
    /** Binding address for frontend*/
    QString             mFrontAddress;

    /** Binding address for backend*/
    QString             mBackAddress;

    /** Communication frontend socket */
    nzmqt::ZMQSocket*    mFrontSocket;

    /** Communication backend socket */
    nzmqt::ZMQSocket*    mBackSocket;

};

}   // namespace izmqt

#endif // MESSAGEQUEUEBROKER_H
