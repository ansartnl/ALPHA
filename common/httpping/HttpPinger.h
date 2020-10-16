#ifndef HTTPPINGER_H
#define HTTPPINGER_H

#include "HttpPingGlobal.h"

class HttpPing;

class HttpPinger : public QObject
{
    Q_OBJECT
public:
    explicit HttpPinger(QObject *parent = 0);

    HttpPing * ping();

    void setMaximumErrorsCount(int errorCount);
    void setInterval(int intervalSeconds);

public slots:
    /*!
     * Установка типа http-запроса. Тип запроса по умолчанию - HEAD. Некоторые http-сервера его не
     * поддерживают, тогда следует сменить тип запроса на GET.
     */
    void setRequestType(HttpPingGlobal::RequestType requestType);

    void start(const QString &url);
    void stop();

signals:
    void connectionEstablished();
    void connectionLost();

    void error(const QString &errorString);

protected:
    void addError();
    void clearErrors();

protected slots:
    void onReplyCame();
    void onReplyLost(const QString &errorString);

protected:
    HttpPing *mPing;

    int mMaximumErrorsCount;
    int mIntervalSeconds;

    int mCurrentErrorCount;

    HttpPingGlobal::Status mStatus;
};

#endif // HTTPPINGER_H
