#ifndef LARGESOCKET_H
#define LARGESOCKET_H

#include <QtCore/QSharedPointer>

#include <QtNetwork/QTcpSocket>

class LargeReceiver;
class LargeSender;

/*!
 * \class LargeSocket
 *
 * \brief Сокет больших данных
 *
 * Данный класс - наследник \a QTcpSocket, способный передавать данные большого размера.
 * Не все функции \a QTcpSocket перегружены, некоторые - не виртуальные, что не позволяет
 * абсолютно корректно работать с ним, как с наследником \a QTcpSocket. Вместо этого
 * можно использовать \a LargeSocket::socket(). Например в \a QtSocketAutoConnector.
 */

class LargeSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit LargeSocket(QObject *parent = 0);
    explicit LargeSocket(QTcpSocket *s, QObject *parent = 0);
    virtual ~LargeSocket();

    bool atEnd() const;
    bool isSequential() const;

    qint64 bytesAvailable() const;
    qint64 bytesToWrite() const;
    bool canReadLine() const;
    qint64 pos() const;
    bool reset();
    bool seek(qint64 pos);
    qint64 size() const;
    bool waitForBytesWritten(int msecs);
    bool waitForReadyRead(int msecs);

    // not virtual socket functions, delegated to mSocket
    void abort();
    void connectToHost(const QHostAddress &address, quint16 port
                       , OpenMode openMode = ReadWrite);
    void connectToHost(const QString &hostName, quint16 port
                       , OpenMode openMode = ReadWrite);
    void disconnectFromHost();
    bool flush();
    QHostAddress localAddress() const;
    quint16 localPort() const;
    QHostAddress peerAddress() const;
    QString peerName() const;
    quint16 peerPort() const;
    QAbstractSocket::SocketState state() const;
    bool setSocketDescriptor(int socketDescriptor, SocketState state = ConnectedState,
                             OpenMode openMode = ReadWrite);
    //\/

    inline QTcpSocket * socket() const { return mSocket.data(); }

signals:
    void error(const QString &text, const QString &object);
    void packageReceived();
    void packageReceived(int number, int count);

protected:
    void create();
    virtual qint64 readData(char *data, qint64 maxlen);
    virtual qint64 writeData(const char *data, qint64 len);

protected slots:
    void onError(const QString &str);
    void onDataReceived(const QByteArray &data);
    void onConnected();
    void onDisconnected();

protected:
    QSharedPointer< QTcpSocket > mSocket;
    LargeReceiver *largeReceiver;
    LargeSender *largeSender;
    QByteArray buffer;
};

#endif // LARGESOCKET_H
