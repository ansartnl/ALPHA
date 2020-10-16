#ifndef IZMQT_PUBLISH_QUEUE_H
#define IZMQT_PUBLISH_QUEUE_H

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

class IZMQT_EXPORT PublishQueue : public QObject, public StartedProperty, public ModeProperty {
    Q_OBJECT

public:
    //! c-tor
    explicit PublishQueue(const QString& address, const QString& topic, int mode = SERVER, QObject* parent = 0);

    //! Binds publisher to specified address.
    /*!
     *  \return 0 - when succedded,
     *          -1 - unknown error,
     *          otherwise - error code (refer to zeromq documentation)
    */
    int start(const QString& address, const QString& topic);

public slots:
    //! Binds publisher.
    /*!
     *  \return 0 - when succedded,
     *          -1 - unknown error,
     *          otherwise - error code (refer to zeromq documentation)
    */
    int start();
    //! Unbinds publisher.
    void stop();

    /** Publish message for defined topic */
    void publish(QVariant);
    void publish(const QString& topic, QVariant);

private:
    /** Binding address */
    QString             mAddress;
    /** Queue name */
    QString             mTopic;
    /** Communication socket */
    nzmqt::ZMQSocket*    mSocket;
};

}   // namespace izmqt

#endif // IZMQT_PUBLISH_QUEUE_H
