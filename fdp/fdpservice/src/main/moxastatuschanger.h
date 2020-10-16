#ifndef MOXASTATUSCHANGER_H
#define MOXASTATUSCHANGER_H

#include "aftnworkerbase.h"
#include "dio.h"

class QTimer;

class QTcpSocket;

class MoxaStatusChanger : public AFTNWorkerBase
{
    Q_OBJECT
public:
    explicit MoxaStatusChanger(QObject *parent = 0);

    void run();
};

class MoxaStatusChangerBase : public WorkerBase
{
    Q_OBJECT
public:
    explicit MoxaStatusChangerBase(QObject *parent = 0);

    bool start();
    void stop();

    bool isDisabled() const { return disable; }

protected:
    void loadSettings();
    void initDefaultSettings();

    void process(const QByteArray &ba);

protected slots:
    void onTimer();

    void onConnected();
    void onDisconnected();
    void onError();
    void onReadyRead();

private:
    bool disable;
    QTcpSocket *socket;
    QTimer *timer;
    bool extendedLog;

    static const dio::Channel channel = dio::CH_1;
    static const dio::ModeInOut mio = dio::MIO_OUT;
    dio::ModeLowHigh mlh;
};

#endif // MOXASTATUSCHANGER_H
