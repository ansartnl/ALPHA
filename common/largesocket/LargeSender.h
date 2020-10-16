#ifndef LARGESENDER_H
#define LARGESENDER_H

#include <QtCore/QObject>

class QTcpSocket;

class LargeSender : public QObject
{
    Q_OBJECT
public:
    explicit LargeSender(QObject *parent = 0);

    QTcpSocket * setSocket(QTcpSocket *socket);
    inline QTcpSocket * socket() const { return mSocket; }

public slots:
    qint64 writeData(const QByteArray &data);

signals:
    void error(const QString &error);
    void started();
    void finished();

protected:
    qint64 sendNextPart();

protected slots:
    void onDisconnected();
    void onConnected();
    void onError();

protected:
    QTcpSocket *mSocket;
    QList< QByteArray > parts;
    quint32 mIdentifier;
    int num;
};

#endif // LARGESENDER_H
