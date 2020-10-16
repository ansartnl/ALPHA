#ifndef PULSAR_H
#define PULSAR_H

#include <QtCore/QThread>

#include "PulsarShareFunctions.h"

class PulsarPrivate;

/*!
  * \class Pulsar
  * \brief Пульсар
  *
  * Класс, реализующий интерфейс доступа к функциям пульсара.
  * Для большей информации смотри PulsarPrivate.
  */

class Pulsar : public QThread
{
    Q_OBJECT

public:
    explicit Pulsar(QObject *parent = 0);
    explicit Pulsar(bool passive = false, quint16 port = PulsarDefaultPort
            , const QString &additionalStatus = QString(), QObject *parent = 0);
    explicit Pulsar(bool passive = false, quint16 port = PulsarDefaultPort, QObject *parent = 0);
    virtual ~Pulsar();

    void run();
    void stop();

    inline void setPassive(bool val) { passive = val; }

    inline void setPort(quint16 port) { mPort = port; }
    inline quint16 port() const { return mPort; }

    inline void setMask(const QString &m) { mMask = m; }
    const QString mask() const { return mMask; }

signals:
    void broadcastStarted();
    void broadcastStopped(const QHostAddress &newMaster, const QString &status, const QVariantHash &info);

    void masterChanged(const QHostAddress &newpulsar, const QString &additionalData
                       , const QHostAddress &prevPulsar, const QVariantHash &info);
    void otherMasterInfoChanged(const QHostAddress &master, const QVariantHash &info);

    void p_deactivate();

public slots:
    void setAdditionalInfo(const QVariantHash &info);
    void addAdditionalInfo(const QString &key, const QVariant &value);
    void removeAdditionalInfo(const QString &key);

    void activatePulsar();

    void deactivate();

private slots:
    void onStop();
    void onDelete();

protected:
    QVariantHash mAdditionalInfo;
    quint16 mPort;
    bool passive;

    PulsarPrivate *p;

    friend class PulsarPrivate;

    bool removeOnQuit;

    QString mMask;
};

bool operator <(const QHostAddress &left, const QHostAddress &right);

#endif // PULSAR_H
