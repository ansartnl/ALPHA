#ifndef HTTPPING_H
#define HTTPPING_H

#include "HttpPingGlobal.h"

class HttpPingPrivate;

class HttpPing : public QObject
{
    Q_OBJECT
public:
    explicit HttpPing(QObject *parent = 0);

    HttpPingGlobal::Status status() const;

    bool isRunning() const;

public slots:
    /*!
     * Установка типа http-запроса. Тип запроса по умолчанию - HEAD. Некоторые http-сервера его не
     * поддерживают, тогда следует сменить тип запроса на GET.
     */
    void setRequestType(HttpPingGlobal::RequestType requestType);

    void start(const QString &url, int intervalSeconds = DEFAULT_INTERVAL_SECONDS);
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
    HttpPingPrivate *d;
    QThread *thread;
};

#endif // HTTPPING_H
