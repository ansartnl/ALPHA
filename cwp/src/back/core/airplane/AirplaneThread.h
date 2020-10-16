#ifndef _AIRPLANE_THREAD_H__
#define _AIRPLANE_THREAD_H__

#include <QtCore/QThread>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QMutex>
#include <QtCore/QWaitCondition>


//! Airplane thread that delays update or set position functionality.
class AirplaneThread : public QThread
{
    Q_OBJECT

public:
    typedef QList<quint16>  TListId;

    enum SignalType
    {
        ForUpdate = 1,  // UpdatePosition required
        ForSet          // SetPosition required
    };

    explicit AirplaneThread(QObject *parent = 0);

    void addIdList(const TListId &idList, SignalType type = ForUpdate);
    void addId(quint16 id, SignalType type);

    void run();

    void stop();
    bool isStopped() { return mIsStopped; }

signals:
    void signalUpdate(quint16 id);
    void signalSet(quint16 id);

private:
    typedef QMap<quint16, SignalType>   TMapId;

    /*! Airplane set of identifiers */
    TMapId          mIds;
    /*! Cancel state */
    bool            mIsStopped;

    /*! Update data mutex */
    QMutex          mDataMutex;
    /*! Airplane wait condition */
    QWaitCondition  mWait;
};

#endif // _AIRPLANE_THREAD_H__
