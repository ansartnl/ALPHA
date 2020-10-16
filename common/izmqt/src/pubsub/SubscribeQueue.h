#ifndef IZMQT_SUBSCRIBE_QUEUE_H
#define IZMQT_SUBSCRIBE_QUEUE_H

#include <QObject>
#include <QVariant>
#include <QList>
#include <QStringList>
#include <QByteArray>
#include <QString>

#include "../properties/StartedProperty.h"
#include "../properties/ModeProperty.h"
#include "../izmq_global.h"

namespace nzmqt {
class ZMQSocket;
}

namespace izmqt {

class IZMQT_EXPORT SubscribeQueue : public QObject, public StartedProperty, public ModeProperty {
    Q_OBJECT

public:
    //! c-tor
    explicit SubscribeQueue(const QString& address, const QStringList& topicList = QStringList(QString("")), int mode = CLIENT, QObject* parent = 0);

    //! Subscribes to specified address and topicList.
    /*!
     *  \return 0 - when succedded,
     *          -1 - unknown error,
     *          otherwise - error code (refer to zeromq documentation)
    */
    int start(const QString& address, const QStringList& topicList);

    //! Subscribes to specified topic list.
    //! New subscription list is a result of concatenation previous list with sprecified
    /*!
     *  \return 0 - when succedded,
     *          -1 - unknown error,
     *          otherwise - error code (refer to zeromq documentation)
    */
    int subscribeTo(const QStringList& topicList);

    //! Unsubscribes from specified topics.
    /*!
     *  \return 0 - when succedded,
     *          -1 - unknown error,
     *          otherwise - error code (refer to zeromq documentation)
    */
    int unsubscribeFrom(const QStringList& topicList);

public slots:
    //! Subscribes to publisher.
    /*!
     *  \return 0 - when succedded,
     *          -1 - unknown error,
     *          otherwise - error code (refer to zeromq documentation)
    */
    int start();

    //! Unbinds subscriber.
    void stop();

signals:
    void readyRead(const QString&, QVariant);

private slots:
    /** New message received */
    void onMessageReceived(const QList<QByteArray>& message);

private:
    /** Connecting address */
    QString             mAddress;
    /** List of topics */
    QStringList         mTopicList;
    /** Communication socket */
    nzmqt::ZMQSocket*    mSocket;
};

}   // namespace izmqt

#endif // IZMQT_SUBSCRIBE_QUEUE_H
