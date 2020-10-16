#ifndef IZMQT_REPLY_QUEUE_H
#define IZMQT_REPLY_QUEUE_H

#include <QObject>
#include <QVariant>
#include <QList>
#include <QByteArray>
#include <QString>

#include "../properties/StartedProperty.h"
#include "../properties/ModeProperty.h"
#include "../izmq_global.h"

namespace nzmqt {
class ZMQSocket;
}

namespace izmqt {

class IZMQT_EXPORT ReplyQueue : public QObject, public StartedProperty, public ModeProperty {
    Q_OBJECT

public:
    //! c-tor
    explicit ReplyQueue(const QString& address, int mode = SERVER, QObject* parent = 0);

    //! Starts reply queue for specified address.
    /*!
     *  \return 0 - when succedded,
     *          -1 - unknown error,
     *          otherwise - error code (refer to zeromq documentation)
    */
    int start(const QString& address);

signals:
    void requestReceived(QVariant);

public slots:
    //! Starts reply queue for specified mode.
    /*!
     *  \return 0 - when succedded,
     *          -1 - unknown error,
     *          otherwise - error code (refer to zeromq documentation)
    */
    int start();

    //! Stops reply queue.
    void stop();

    //! Restarts reply queue.
    void restart();

    //! Sends reply.
    void reply(QVariant);

private slots:
    /** Request received from the request queue */
    void onRequestReceived(const QList<QByteArray>& request);

private:
    /** Binding address */
    QString             mAddress;
    /** Communication socket */
    nzmqt::ZMQSocket*    mSocket;
};

}   // namespace izmqt

#endif // IZMQT_REPLY_QUEUE_H
