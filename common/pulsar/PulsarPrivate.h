#ifndef PULSARPRIVATE_H
#define PULSARPRIVATE_H

#include <QtCore/QDateTime>
#include <QtCore/QElapsedTimer>
#include <QtCore/QObject>
#include <QtCore/QVariantHash>
#include <QStringList>
#include <QtNetwork/QHostAddress>

class QTimer;
class QUdpSocket;

/*!
  * \class PulsarPrivate
  * \brief Ядро пульсара
  *
  * Класс, реализующий основные функции пульсара.
  *
  * Объект данного класса имеет два основных состояния: активное и пассивное.
  *
  * В пассивном состоянии объект пытается стать активным, для чего запускает таймер,
  * по истечении времени которого меняет свой статус на активный. Кроме того в пассивном
  * состоянии объект принимает сообщения, посылаемые активным объектом и сбрасывающие таймер.
  * То есть, до тех пор пока пассивный объект получает сообщения от активного, он
  * всегда будет пассивным.
  *
  * В активном состоянии объект получает от пассивных объектов специальное сообщение, и посылает
  * по полученным адресам сигнал, сбрасывающий таймеры пассивных объектов.
  *
  * Таким образом, лишь один из объектов данного класса может находиться в активном состоянии. Если
  * в какой-то момент времени образовалось два активных объекта, один из них меняет свой статус на
  * пассивный.
  */

class PulsarPrivate : public QObject
{
    Q_OBJECT
public:
    explicit PulsarPrivate(bool passive, quint16 port, const QString& additionalStatus = QString(), const QVariantHash& additionalInfo = QVariantHash(), QObject* parent = 0, const QString& serverAddress = QString());

    void setMask(const QString &m);
    void setBroadcast(const QStringList &bl);

signals:
    void started();
    void stopped(const QHostAddress &address, const QString &info, const QVariantHash &addInfo);
    void masterChanged(const QHostAddress &newpulsar, const QString &status, const QHostAddress &oldpulsar, const QVariantHash &addInfo);
    void otherMasterInfoChanged(const QHostAddress &master, const QVariantHash &info);

protected:
    bool isOurMask(const QHostAddress &mask);
    bool isOurBroadcast(const QHostAddress &broadcast);

protected slots:
    void readyRead();
    void becomeMaster();
    void sendStatus();
    void sendActivate();
    void broadcast(const QByteArray &data, quint16 port);

    void setAdditionalStatus(const QString& status);

    void setAdditionalInfo(const QVariantHash &info);
    void addAdditionalInfo(const QString &key, const QVariant &value);
    void removeAdditionalInfo(const QString &key);

    void deactivate();

protected:
    QUdpSocket *server;
    QElapsedTimer timer;

    QTimer *waitTimeout;
    QTimer *sendStatusTimeout;

    QDateTime dateTime;
    bool iam;

    QString additionalStatus;

    quint16 mPort;

    bool passive;
    QHostAddress lastpeer;
    QDateTime masterIdentificator;

    QVariantHash myAddInfo;
    QVariantHash otherAddInfo;

    QString mMask;
    QStringList mBroadcastList;
};


#endif // PULSARPRIVATE_H
