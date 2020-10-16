#include "HttpPinger.h"

#include "HttpPing.h"

HttpPinger::HttpPinger(QObject *parent) :
    QObject(parent)
  , mPing(new HttpPing(this))
  , mMaximumErrorsCount(DEFAULT_MAXIMUM_ERRORS_COUNT)
  , mIntervalSeconds(DEFAULT_INTERVAL_SECONDS)
  , mCurrentErrorCount(0)
  , mStatus(HttpPingGlobal::S_UNKNOWN)
{
    connect(mPing, SIGNAL(replyCame()), SLOT(onReplyCame()));
    connect(mPing, SIGNAL(replyLost(QString)), SLOT(onReplyLost(QString)));
}

HttpPing * HttpPinger::ping()
{
    return mPing;
}

void HttpPinger::setMaximumErrorsCount(int errorCount)
{
    mMaximumErrorsCount = errorCount;
}

void HttpPinger::setInterval(int intervalSeconds)
{
    mIntervalSeconds = intervalSeconds;
}

void HttpPinger::setRequestType(HttpPingGlobal::RequestType requestType)
{
    mPing->setRequestType(requestType);
}

void HttpPinger::start(const QString &url)
{
    mPing->start(url, mIntervalSeconds);
}

void HttpPinger::stop()
{
    mPing->stop();
    mCurrentErrorCount = 0;
    mStatus = HttpPingGlobal::S_UNKNOWN;
}

void HttpPinger::addError()
{
    if (mStatus != HttpPingGlobal::S_DISCONNECTED)
    {
        ++mCurrentErrorCount;
        if (mCurrentErrorCount > mMaximumErrorsCount)
        {
            mStatus = HttpPingGlobal::S_DISCONNECTED;
            emit connectionLost();
        }
    }
}

void HttpPinger::clearErrors()
{
    mCurrentErrorCount = 0;
    if (mStatus != HttpPingGlobal::S_CONNECTED)
    {
        mStatus = HttpPingGlobal::S_CONNECTED;
        emit connectionEstablished();
    }
}

void HttpPinger::onReplyCame()
{
    clearErrors();
}

void HttpPinger::onReplyLost(const QString &errorString)
{
    if (!errorString.isEmpty())
        emit error(errorString);
    addError();
}
