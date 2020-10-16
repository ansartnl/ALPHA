#include "HttpPing.h"

#include "HttpPingPrivate.h"

#include <QtCore/QThread>

HttpPing::HttpPing(QObject *parent) :
    QObject(parent)
{
    d = new HttpPingPrivate(this);
    thread = new QThread(this);

    d->moveToThread(thread);

    connect(thread, SIGNAL(started()), d, SLOT(start()));
    connect(thread, SIGNAL(finished()), d, SLOT(stop()));

    connect(d, SIGNAL(connectionEstablished()), SIGNAL(connectionEstablished()));
    connect(d, SIGNAL(connectionLost()), SIGNAL(connectionLost()));
    connect(d, SIGNAL(requestSent()), SIGNAL(requestSent()));
    connect(d, SIGNAL(replyCame()), SIGNAL(replyCame()));
    connect(d, SIGNAL(replyLost(QString)), SIGNAL(replyLost(QString)));
}

HttpPingGlobal::Status HttpPing::status() const
{
    return d->status();
}

bool HttpPing::isRunning() const
{
    return thread->isRunning();
}

void HttpPing::setRequestType(HttpPingGlobal::RequestType requestType)
{
    d->setRequestType(requestType);
}

void HttpPing::start(const QString &url, int intervalSeconds)
{
    if (isRunning())
        return;

    d->setUrl(url);
    d->setInterval(intervalSeconds);

    thread->start();
}

void HttpPing::stop()
{
    thread->quit();
}
