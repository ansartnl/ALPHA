#ifndef XPUBSUBPROXY_H
#define XPUBSUBPROXY_H

#include <QObject>
#include <QString>

#include "../properties/StartedProperty.h"
#include "../izmq_global.h"

namespace nzmqt {
class ZMQSocket;
}

namespace izmqt {

class IZMQT_EXPORT XPubSubProxy : public QObject, public StartedProperty {
    Q_OBJECT
public:
    explicit XPubSubProxy(const QString& frontAddress, const QString& backAddress, QObject* parent = 0);

    //! Binds proxy to specified front and back addresses.
    /*!
     *  \return 0 - when succedded,
     *          -1 - unknown error,
     *          otherwise - error code (refer to zeromq documentation)
    */
    int start(const QString& frontAddress, const QString& backAddress);

public slots:
    //! Binds proxy.
    /*!
     *  \return 0 - when succedded,
     *          -1 - unknown error,
     *          otherwise - error code (refer to zeromq documentation)
    */
    int start();

    //! Unbinds proxy.
    void stop();

    //! Restart proxy.
    void restart();

private slots:
    /** Message received*/
    void onSubMessageReceived(const QList<QByteArray>& message);
    void onPubMessageReceived(const QList<QByteArray>& message);

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

#endif // XPUBSUBPROXY_H
