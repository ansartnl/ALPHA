#ifndef IZMQT_REQUEST_QUEUE_H
#define IZMQT_REQUEST_QUEUE_H

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

class IZMQT_EXPORT RequestQueue : public QObject, public StartedProperty, public ModeProperty {
    Q_OBJECT

public:
    explicit RequestQueue(const QString& address, int mode = CLIENT, QObject* parent = 0);

    //! Starts request queue for specified address.
    /*!
     *  \return 0 - when succedded,
     *          -1 - unknown error,
     *          otherwise - error code (refer to zeromq documentation)
    */
    int start(const QString& address);

signals:
    void replyReceived(QVariant);

public slots:
    //! Starts request queue for specified mode.
    /*!
     *  \return 0 - when succedded,
     *          -1 - unknown error,
     *          otherwise - error code (refer to zeromq documentation)
    */
    int start();

    //! Stops request queue.
    void stop();

    //! Restarts request queue.
    void restart();

    //! Sends request to reply queue.
    void request(QVariant);

private slots:
    void onReplyReceived(const QList<QByteArray>& reply);

private:
    /** Connecting address */
    QString             mAddress;
    /** Communication socket */
    nzmqt::ZMQSocket*    mSocket;
};

}   // namespace izmqt

#endif // IZMQT_REQUEST_QUEUE_H
