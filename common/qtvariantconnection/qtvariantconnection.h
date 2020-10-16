#ifndef QTVARIANTCONNECTION_H
#define QTVARIANTCONNECTION_H

#include <QtCore/QObject>

class QVariant;
class QTcpSocket;
class QtTcpDatagramProtocol;

class QtVariantConnection : public QObject
{
    Q_OBJECT
public:
    explicit QtVariantConnection(QTcpSocket *socket, QObject *parent = 0);
    virtual ~QtVariantConnection();

    const QTcpSocket *socket() const;

    void setProtoVersion(quint32 v);

signals:
    void gotVariant(const QVariant &v);
    void disconnected();

public slots:
    qint64 sendVariant(const QVariant &v);

private slots:
    void onIncomingDatagram();

private:
    QtTcpDatagramProtocol *protocol;
};

#endif // QTVARIANTCONNECTION_H
