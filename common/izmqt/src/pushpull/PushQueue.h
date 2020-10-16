#ifndef IZMQT_PUSH_QUEUE_H
#define IZMQT_PUSH_QUEUE_H

#include <QObject>
#include <QVariant>
#include <QString>

#include "../properties/StartedProperty.h"
#include "../properties/ModeProperty.h"
#include "../izmq_global.h"

namespace nzmqt {
class ZMQSocket;
}

namespace izmqt {

class IZMQT_EXPORT PushQueue : public QObject, public StartedProperty, public ModeProperty {
    Q_OBJECT

public:
    //! c-tor
    explicit PushQueue(const QString& address, int mode = CLIENT, QObject* parent = 0);

    //! Starts queue for specified address.
    /*!
     *  \return 0 - when succedded,
     *          -1 - unknown error,
     *          otherwise - error code (refer to zeromq documentation)
    */
    int start(const QString& address);

    //! Set maximum size (high water mark) of the queue.
    void setQueueSize(qint32 hwm);

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

    /** Push message to queue */
    void push(QVariant);

private:
    /** Binding address */
    QString             mAddress;
    /** Communication socket */
    nzmqt::ZMQSocket*    mSocket;
};

}   // namespace izmqt

#endif // IZMQT_PUSH_QUEUE_H
