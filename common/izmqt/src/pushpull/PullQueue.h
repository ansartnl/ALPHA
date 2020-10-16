#ifndef IZMQT_PULL_QUEUE_H
#define IZMQT_PULL_QUEUE_H

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

class IZMQT_EXPORT PullQueue : public QObject, public StartedProperty, public ModeProperty {
    Q_OBJECT

public:
    //! c-tor
    explicit PullQueue(const QString& address, int mode = CLIENT, QObject* parent = 0);

    //! Starts queue for specified address.
    /*!
     *  \return 0 - when succedded,
     *          -1 - unknown error,
     *          otherwise - error code (refer to zeromq documentation)
    */
    int start(const QString& address);

    //! Set maximum size (high water mark) of the queue.
    void setQueueSize(qint32 hwm);

signals:
    void readyRead(QVariant);

public slots:
    //! Starts queue for specified mode.
    /*!
     *  \return 0 - when succedded,
     *          -1 - unknown error,
     *          otherwise - error code (refer to zeromq documentation)
    */
    int start();

    //! Stops queue.
    void stop();

private slots:
    /** New message received */
    void onMessageReceived(const QList<QByteArray>& message);

private:
    /** Connecting address */
    QString             mAddress;
    /** Communication socket */
    nzmqt::ZMQSocket*    mSocket;
};

}   // namespace izmqt

#endif // IZMQT_PULL_QUEUE_H
