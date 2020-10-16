#ifndef AFTNCLIENT_CTA_H
#define AFTNCLIENT_CTA_H

#include "aftnclient.h"

class HttpPinger;

class QTimer;

class AFTNClientCTA: public AFTNClient
{
    Q_OBJECT
public:
    AFTNClientCTA(QObject* parent = 0);
    virtual ~AFTNClientCTA();

signals:
    void cksState(bool isConnected);

protected:
    virtual void sendAuthorizationRequest();
    virtual int processMessages();
    virtual void closeChannel();

protected slots:
    virtual void pingTimeoutTimer();
    virtual void ping(bool bSend = true);
    void onChTimer();

    void onConnectionLost();
    void onHttpError(const QString &errorString);
    void onStateChanged(QAbstractSocket::SocketState state);

private:
    QTimer *chTimer;
    HttpPinger *pinger;
};

#endif // AFTNCLIENT_CTA_H
