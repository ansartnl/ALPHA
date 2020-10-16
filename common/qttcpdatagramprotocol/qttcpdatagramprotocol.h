#ifndef QTTCPDATAGRAMPROTOCOL_H
#define QTTCPDATAGRAMPROTOCOL_H

#include <QtNetwork/QTcpSocket>
#include <QtCore/QByteArray>
#include <QtCore/QPointer>

/*!
  * \class QtTcpDatagramProtocol
  * \brief Отправка пакетов данных по TCP/IP
  *
  * Данный класс предоставляет интерфейс для отправки пакетов данных
  * через стек TCP/IP.
  */

// Provide datagram sending via TCP/IP protocol
class QtTcpDatagramProtocol : public QObject
{
    Q_OBJECT
public:
    explicit QtTcpDatagramProtocol(QTcpSocket *socket, QObject *parent = 0);
    explicit QtTcpDatagramProtocol(QObject *parent = 0);
    virtual ~QtTcpDatagramProtocol();

    QTcpSocket *setSocket(QTcpSocket *socket);
    inline QTcpSocket *socket() const { return mSocket; }

    //! Set new protocol version.
    void setVersion(quint32 v) { version = v; }

    bool hasPendingDatagrams() const;
    int datagramAmount() const;
    QByteArray nextDatagram();
    QList<QByteArray> readAllDatagrams();    

    inline quint64 bufferLimit() const { return limit; }
    inline void setBufferLimit(quint64 bufferLimit) { limit = bufferLimit; }

signals:
    void incomingDatagram();
    void disconnected();
    void connected();

public slots:
    qint64 writeDatagram(const QByteArray &datagram);
    void clear();

private slots:
    void onSocketStateChanged(QAbstractSocket::SocketState socketState);
    void onReadyRead();

private:
    void trunc();

    QPointer<QTcpSocket> mSocket;
    QList<QByteArray> datargams;
    quint32 blockSize;
    bool needReadBlockSize;
    bool needEmit;
    quint64 limit;
    quint32 version;
};

#endif // QTTCPDATAGRAMPROTOCOL_H
