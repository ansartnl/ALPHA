#ifndef HTTPPINGPRIVATE_H
#define HTTPPINGPRIVATE_H

#include "HttpPingGlobal.h"

#include <QtCore/QMutex>

class QTimer;

class QNetworkAccessManager;
class QNetworkReply;

class HttpPingPrivate : public QObject
{
    Q_OBJECT
public:
    explicit HttpPingPrivate(QObject *parent = 0);

    HttpPingGlobal::Status status() const;

    const QString url() const;
    int interval() const;
    int intervalMs() const;
    HttpPingGlobal::RequestType requestType() const;

public slots:
    void setUrl(const QString &url);
    void setInterval(int intervalSeconds);
    void setRequestType(HttpPingGlobal::RequestType requestType);

    void start();
    void stop();

signals:
    /*!
     * Вызывается каждый раз, когда статус меняется на S_CONNECTED
     */
    void connectionEstablished();
    /*!
     * Вызывается каждый раз, когда статус меняется на S_DISCONNECTED
     */
    void connectionLost();
    /*!
     * Вызывается каждый раз, когда отправляется запрос
     */
    void requestSent();
    /*!
     * Вызывается каждый раз, когда получен ответ
     */
    void replyCame();
    /*!
     * Вызывается каждый раз, когда ответ теряется по таймауту (\a errorString == \a QString())
     * или возвращается ошибка (\a errorString содержит текст ошибки)
     */
    void replyLost(const QString &errorString);

private:
    void pongCame();
    void timeout(const QString &errorString = QString());
    QNetworkReply * request() const;

private slots:
    void sendRequest();
    void onRequestFinished();
    void onTimeout();

private:
    mutable QMutex mutex;

    HttpPingGlobal::Status mStatus;
    QTimer *timer;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;

    QString mUrl;
    int mIntervalSeconds;
    HttpPingGlobal::RequestType mRequestType;
};

#endif // HTTPPINGPRIVATE_H
