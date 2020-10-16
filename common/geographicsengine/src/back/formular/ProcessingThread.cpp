#include "ProcessingThread.h"

#include <QtCore/QLineF>


namespace FormularSeparation
{

ProcessingThread::ProcessingThread(QObject *parent)
    : QThread(parent), mIsStopped(false)
{
}

void ProcessingThread::add(quint16 id, const FormularInfo &fi)
{
    QMutexLocker lock(&mLocker);

    FormularMap::iterator it = mFormulars.find(id);
    if ( it == mFormulars.end() )
        mFormulars[id] = fi;
    else
        *it = fi;

    mQueueId.enqueue(id);

    mWait.wakeAll();
}

void ProcessingThread::remove(quint16 id)
{
    QMutexLocker lock(&mLocker);

    FormularMap::iterator it = mFormulars.find(id);
    if ( it != mFormulars.end() )
        mFormulars.erase(it);

    {
        QMutexLocker lock(&mWaitMutex);
        if ( mStrategy )
            mStrategy->remove(id);
    }
}

void ProcessingThread::change(quint16 id, const FormularInfo &fi)
{
    QMutexLocker lock(&mLocker);

    FormularMap::iterator it = mFormulars.find(id);
    if ( it == mFormulars.end() )
        return;

    *it = fi;

    mQueueId.enqueue(id);

    mWait.wakeAll();
}

void ProcessingThread::setRepositionRect(const QRectF &rectWindow)
{
    mRepositionRect = rectWindow;

    // Enlarge window rect twice.
    qreal widthHalf = mRepositionRect.width() / 2.;
    qreal heightHalf = mRepositionRect.height() / 2.;

    mRepositionRect.adjust(-widthHalf, -heightHalf, widthHalf, heightHalf);
}

void ProcessingThread::stop()
{
    mIsStopped = true;
    mWait.wakeAll();
}

void ProcessingThread::run()
{
    if ( !mStrategy )
    {
        qDebug("ProcessingThread::run(). Separation strategy was not defined. Do it before starting the calculations.");
        return;
    }

    QPointF pos;

    qDebug("ProcessingThread::run(). Separation thread started.");

    forever
    {
        if ( mIsStopped )
            break;

        QMutexLocker lock(&mWaitMutex);

        if ( mQueueId.empty() )
            mWait.wait(&mWaitMutex);

        if ( mIsStopped )
            break;

        quint16 id;
        {
            QMutexLocker lock(&mLocker);
            id = mQueueId.dequeue();

            // Limit number of items in the queue. Do not leave too much.
            if ( mQueueId.size() > 10000 )
            {
                while ( mQueueId.size() > 1000 )
                    mQueueId.removeLast();
                qDebug("ProcessingThread::run(). Number of formulars to process is too many. Queue is limited.");
            }
        }

        if ( mStrategy->separate(id, pos) )
            emit reposition_required(id, pos);
    }

    mIsStopped = false;

    qDebug("ProcessingThread::run(). Separation thread finished.");
}

}   // namespace FormularSeparation
