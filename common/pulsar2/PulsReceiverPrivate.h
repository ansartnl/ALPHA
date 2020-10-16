#ifndef PULSRECEIVERPRIVATE_H
#define PULSRECEIVERPRIVATE_H

#include <QtCore/QObject>
#include <QtCore/QDateTime>
#include <QtCore/QVariantHash>

#include <QtNetwork/QHostAddress>

class QTimer;
class QUdpSocket;

/*!
  * \class PulsReceiverPrivate
  * \brief Ядро получателя пульсара
  *
  * Класс, реализующий основные функции получателя пульсара.
  *
  * Объект данного класса получает сообщение от активного пульсара. В случае, если за
  * определенное время сообщение не было получено, объект издает соответсвующий сигнал.
  */

class PulsReceiverPrivate : public QObject
{
    Q_OBJECT

public:
    PulsReceiverPrivate(bool repiter, quint16 port, QObject *parent = 0);

    void setMask(const QString &m) { mMask = m; }

public slots:
    void activatePulsar(const QHostAddress &newpulsar);

signals:
    void pulsarChanged(const QHostAddress &newpulsar, const QString &status
                       , const QHostAddress &oldpulsar, const QVariantHash &info);
    void status(const QHostAddress &pulsar, const QString &data, const QVariantHash &info);
    void masterInfoChanged(const QHostAddress &master, const QVariantHash &info);

protected:
    void broadcast(const QByteArray &data, quint16 port);

protected slots:
    void readyRead();
    void masterLosted();
    void onMasterPingTimer();

protected:
    QUdpSocket *client;
    QHostAddress lastpeer;
    QDateTime masterIdentificator;
    QTimer *masterLost;
    quint16 mPort;
    bool mRepiter;
    QString mMask;

    QVariantHash masterInfo;

    QTimer *masterPingTimer;
    int masterPing;
};

#endif // PULSRECEIVERPRIVATE_H
