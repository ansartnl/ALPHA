#include "SubscribeQueue.h"

#include "nzmqt/nzmqt.hpp"

#include <QDebug>

namespace izmqt
{

SubscribeQueue::SubscribeQueue(const QString& address, const QStringList& topicList, int mode, QObject *parent)
    : QObject(parent), ModeProperty(mode), mAddress(address), mTopicList(topicList)
{
    nzmqt::ZMQContext* context = nzmqt::createDefaultContext(this);
    context->start();

    mSocket = context->createSocket(nzmqt::ZMQSocket::TYP_SUB);
    connect(mSocket, SIGNAL(messageReceived(const QList<QByteArray>&)),
            SLOT(onMessageReceived(const QList<QByteArray>&)));
}

int SubscribeQueue::start()
{
    if ( isStarted() )
        return true;

    setStarted(false);

    try
    {
        if ( mode() == izmqt::CLIENT )
            mSocket->connectTo(mAddress);

        else if ( mode() == izmqt::SERVER )
            mSocket->bindTo(mAddress);

        else
            Q_ASSERT(false);

        foreach(QString topic, mTopicList)
            mSocket->subscribeTo(topic);
    }
    catch ( const zmq::error_t &e )
    {
        qDebug() << tr("Subscriber '%1'> Error: ").arg(mAddress) << e.what();
        return e.num();
    }
    catch ( ... )
    {
        qDebug() << tr("Subscriber '%1'> Unknown error (start)").arg(mAddress);
        return -1;
    }

    setStarted(true);

    return 0;
}

int SubscribeQueue::start(const QString& address, const QStringList &topicList)
{
    mAddress = address;
    mTopicList = topicList;

    return start();
}

int SubscribeQueue::subscribeTo(const QStringList &topicList)
{
    try
    {
        mTopicList.append(topicList);
        foreach(QString topic, topicList)
            mSocket->subscribeTo(topic);
    }
    catch ( const zmq::error_t &e )
    {
        qDebug() << tr("Subscriber '%1'> Error: ").arg(mAddress) << e.what();
        return e.num();
    }
    catch ( ... )
    {
        qDebug() << tr("Subscriber '%1'> Unknown error (subscribeTo)").arg(mAddress);
        return -1;
    }

    return 0;
}

int SubscribeQueue::unsubscribeFrom(const QStringList &topicList)
{
    try
    {
        foreach(QString topic, topicList)
        {
            if (mTopicList.removeOne(topic))
                mSocket->unsubscribeFrom(topic);
        }
    }
    catch ( const zmq::error_t &e )
    {
        qDebug() << tr("Subscriber '%1'> Error: ").arg(mAddress) << e.what();
        return e.num();
    }
    catch ( ... )
    {
        qDebug() << tr("Subscriber '%1'> Unknown error (unsubscribeFrom)").arg(mAddress);
        return -1;
    }

    return 0;
}

void SubscribeQueue::stop()
{
    if ( !isStarted() )
        return;

    try
    {
        if ( mode() == izmqt::CLIENT )
            mSocket->disconnectFromHost(mAddress);

        else if ( mode() == izmqt::SERVER )
            mSocket->unbind(mAddress);

        else
            Q_ASSERT(false);
    }
    catch ( const zmq::error_t &e )
    {
        qDebug() << tr("Subscriber '%1'> Error: ").arg(mAddress) << e.what();
    }
    catch ( ... )
    {
        qDebug() << tr("Subscriber '%1'> Unknown error (stop)").arg(mAddress);
    }

    setStarted(false);
}

void SubscribeQueue::onMessageReceived(const QList<QByteArray>& message)
{
    // We assume that our message is multi-part message with 1-st byte array - topic.
    // We wait only 2x byte array in the list.
    if ( message.size() < 1 )
        return;

    // FIXME: In most cases subscription works well but to be on the safe side
    // we do this comparison.
    /*if ( mTopic != QString(message[0]) )
        return;*/

    if ( message.size() > 1 )
    {
        QByteArray m = message[1];

        // Byte array -> variant
        QVariant v;
        QDataStream stream(&m, QIODevice::ReadOnly);
        stream >> v;

        if ( !v.isNull() && v.isValid() )
            //emit readyRead(v);
            emit readyRead(QString(message[0]), v);
    }
}

} // namespace izmqt
