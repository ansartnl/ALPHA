#include "HttpPingPrivate.h"

#include <QtCore/QTimer>
#include <QtCore/QUrl>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

HttpPingPrivate::HttpPingPrivate(QObject *parent) :
    QObject(0), mStatus(HttpPingGlobal::S_UNKNOWN), manager(0), mRequestType(HttpPingGlobal::RT_HEAD)
{
    if (parent)
        connect(parent, SIGNAL(destroyed()), SLOT(deleteLater()));

    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), SLOT(onTimeout()));

    reply = 0;

    mIntervalSeconds = DEFAULT_INTERVAL_SECONDS;
}

HttpPingGlobal::Status HttpPingPrivate::status() const
{
    QMutexLocker locker(&mutex);
    return mStatus;
}

const QString HttpPingPrivate::url() const
{
    QMutexLocker locker(&mutex);
    return mUrl;
}

int HttpPingPrivate::interval() const
{
    QMutexLocker locker(&mutex);
    return mIntervalSeconds;
}

int HttpPingPrivate::intervalMs() const
{
    QMutexLocker locker(&mutex);
    return interval() * 1000;
}

HttpPingGlobal::RequestType HttpPingPrivate::requestType() const
{
    QMutexLocker locker(&mutex);
    return mRequestType;
}

void HttpPingPrivate::setUrl(const QString &url)
{
    QMutexLocker locker(&mutex);
    mUrl = url;
    if (!mUrl.startsWith("http://"))
        mUrl.prepend("http://");
}

void HttpPingPrivate::setInterval(int intervalSeconds)
{
    QMutexLocker locker(&mutex);
    mIntervalSeconds = intervalSeconds;
}

void HttpPingPrivate::setRequestType(HttpPingGlobal::RequestType requestType)
{
    QMutexLocker locker(&mutex);
    mRequestType = requestType;
}

void HttpPingPrivate::start()
{
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), SLOT(onRequestFinished()));

    timer->setInterval(intervalMs());
    staticMetaObject.invokeMethod(this, "sendRequest", Qt::QueuedConnection);
}

void HttpPingPrivate::stop()
{
    timer->stop();
    mStatus = HttpPingGlobal::S_UNKNOWN;
    if (reply)
    {
        reply->deleteLater();
        reply = 0;
    }

    disconnect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onRequestFinished()));
    manager->deleteLater();
    manager = 0;
}

void HttpPingPrivate::pongCame()
{
    emit replyCame();
    if (mStatus != HttpPingGlobal::S_CONNECTED)
    {
        mStatus = HttpPingGlobal::S_CONNECTED;
        emit connectionEstablished();
    }
}

void HttpPingPrivate::timeout(const QString &errorString)
{
    emit replyLost(errorString);
    if (mStatus != HttpPingGlobal::S_DISCONNECTED)
    {
        mStatus = HttpPingGlobal::S_DISCONNECTED;
        emit connectionLost();
    }
}

QNetworkReply * HttpPingPrivate::request() const
{
    switch (requestType())
    {
    case HttpPingGlobal::RT_HEAD:
        return manager->head(QNetworkRequest(QUrl(url())));
    case HttpPingGlobal::RT_GET:
        return manager->get(QNetworkRequest(QUrl(url())));
    }
    return manager->get(QNetworkRequest(QUrl(url())));
}

void HttpPingPrivate::sendRequest()
{
    if (!reply)
    {
        emit requestSent();
        reply = request();
        timer->start();
    }
}

void HttpPingPrivate::onRequestFinished()
{
    timer->stop();
    if (reply->error() == QNetworkReply::NoError)
        pongCame();
    else
        timeout(reply->errorString());
    reply->deleteLater();
    reply = 0;
    QTimer::singleShot(intervalMs(), this, SLOT(sendRequest()));
}

void HttpPingPrivate::onTimeout()
{
    reply->manager()->blockSignals(true);
    reply->abort();
    reply->manager()->blockSignals(false);
    reply->deleteLater();
    reply = 0;
    timeout();
    staticMetaObject.invokeMethod(this, "sendRequest", Qt::QueuedConnection);
}
