#ifndef IZMQT_DEALER_CLIENT_H
#define IZMQT_DEALER_CLIENT_H

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

class IZMQT_EXPORT DealerClient : public QObject, public StartedProperty {
    Q_OBJECT

public:
    explicit DealerClient(const QString& address, const QString& identity, QObject* parent = 0);

    //! Return address of the client.
    const QString& address() const { return mAddress; }

    //! Return unique client identifier.
    QString identity() const;

    //! Connects request client to specified address.
    /*!
     *  \return 0 - when succedded,
     *          -1 - unknown error,
     *          otherwise - error code (refer to zeromq documentation)
    */
    int start(const QString& address);

signals:
    void replyReceived(QVariant);

    void receivedSize(quint32);
    void sentSize(quint32);

public slots:
    //! Connects to reply server.
    /*!
     *  \return 0 - when succedded,
     *          -1 - unknown error,
     *          otherwise - error code (refer to zeromq documentation)
    */
    int start();

    //! Disconnects from reply server.
    void stop();

    //! Restart request client.
    void restart();

    //! Sends request to server.
    bool request(QVariant);

private slots:
    void onReplyReceived(const QList<QByteArray>& reply);

    void onSentSize(quint32 size);

private:
    /** Connecting address */
    QString             mAddress;
    /** Communication socket */
    nzmqt::ZMQSocket*    mSocket;
};

}   // namespace izmqt

#endif // IZMQT_DEALER_CLIENT_H
