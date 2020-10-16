#ifndef LARGERECEIVER_H
#define LARGERECEIVER_H

#include <QtCore/QObject>

class QTcpSocket;

namespace LargeData
{
struct DataPacket;
}

class LargeReceiver : public QObject
{
    Q_OBJECT
public:
    explicit LargeReceiver(QObject *parent = 0);

    QTcpSocket * setSocket(QTcpSocket *socket);
    inline QTcpSocket * socket() const { return mSocket; }

signals:
    void error(const QString &error);
    void dataCame(const QByteArray &data);
    void packageReceived();
    void packageReceived(int number, int count);

protected slots:
    void onDisconnected();
    void onConnected();
    void onError();
    void onReadyRead();

protected:
    void timerEvent(QTimerEvent *e);
    void proceed();
    void proceed(const LargeData::DataPacket &packet);
    void postData();
    void stopTimer();

protected:
    QTcpSocket *mSocket;
    QList< QByteArray > parts;
    quint32 mIdentifier;
    QByteArray buffer;
    int timerId;
};

#endif // LARGERECEIVER_H
