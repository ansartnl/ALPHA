#ifndef FORMULAR_PROCESSING_THREAD_H
#define FORMULAR_PROCESSING_THREAD_H

#include <QtCore/QThread>
#include <QtCore/QMap>
#include <QtCore/QRectF>
#include <QtCore/QMutex>
#include <QtCore/QWaitCondition>
#include <QtCore/QQueue>

#include "SeparateStrategy.h"


namespace FormularSeparation
{

    //! Automatic formular separation thread.
    class ProcessingThread : public QThread
    {
        Q_OBJECT

    public:
        //! Constructor
        ProcessingThread(QObject *parent = 0);

        void add(quint16 id, const FormularInfo &fi);
        void remove(quint16 id);
        void change(quint16 id, const FormularInfo &fi);

        //! Define algorithm for formular separation.
        void setStrategy(const TStrategy &strategy)
        { mStrategy = strategy; mStrategy->init(mFormulars, mLocker, mRepositionRect); }

        //! Reference to current separation algorithm.
        const TStrategy& strategy() const
        { return mStrategy; }

        //! Stop thread.
        void stop();

        //! Set sizes of window where reposition is necessary.
        void setRepositionRect(const QRectF &rectWindow);

    signals:
        void reposition_required(quint16 id, const QPointF &posRect);

    protected:
        virtual void run();

    private:
        /** Formular parameters container */
        FormularMap     mFormulars;

        QMutex          mLocker;

        QMutex          mWaitMutex;
        QWaitCondition  mWait;

        /** Window rectangle for repositioning */
        QRectF          mRepositionRect;

        /** List of formular identifiers to process */
        QQueue<quint16> mQueueId;

        /** Current separation algorithm */
        TStrategy       mStrategy;

        /** State of thread working */
        volatile bool   mIsStopped;
    };

}   // namespace FormularSeparation

#endif // FORMULAR_PROCESSING_THREAD_H
