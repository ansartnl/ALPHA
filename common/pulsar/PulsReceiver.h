#ifndef PULSRECEIVER_H
#define PULSRECEIVER_H

#include "PulsarShareFunctions.h"

#include <QtCore/QThread>

class PulsReceiverPrivate;

/*!
  * \class PulsReceiver
  * \brief Получатель пульсар
  *
  * Класс, реализующий интерфейс доступа к функциям получателя пульсара.
  * Для большей информации смотри PulsReceiverPrivate.
  */

class PulsReceiver : public QThread
{
    Q_OBJECT

public:
    explicit PulsReceiver(QObject *parent = 0);
    explicit PulsReceiver(quint16 port = PulsarDefaultPort, QObject *parent = 0);
    virtual ~PulsReceiver();

    void run();
    void stop();

    quint16 port() const {return mPort;}
    void setPort(quint16 port) {mPort = port;}

public slots:
    void activatePulsar(const QHostAddress &newpulsar);

signals:
    void pulsarChanged(const QHostAddress &newpulsar, const QString &status, const QHostAddress &oldpulsar, const QVariantHash &info);
    void status(const QHostAddress &pulsar, const QString &data, const QVariantHash &info);
    void masterInfoChanged(const QHostAddress &master, const QVariantHash &info);

private slots:
    void onStop();
    void onDelete();
    void onActivatePulsar(const QHostAddress &newpulsar);

protected:

    quint16 mPort;

    PulsReceiverPrivate *p;

    bool removeOnQuit;
};

#endif // PULSRECEIVER_H
