#ifndef IZMQT_MONITOR_H
#define IZMQT_MONITOR_H

//#include <QObject>
//#include <QRunnable>

//namespace nzmqt {
////    class ZMQContext;
////    class ZMQSocket;
//}

//namespace izmqt
//{

//class Monitor : public QObject, public QRunnable
//{
//    Q_OBJECT

//public:
//    //! c-tor
//    explicit Monitor(nzmqt::ZMQContext *context, nzmqt::ZMQSocket *socket, QObject* parent = 0);

//public slots:
//    //! Starts monitoring in the thread pool.
//    void start();

//    //! Quit from the runner.
//    void quit();

//    //! Monitoring process
//    virtual void run();

//private:
//    /** Monitor socket */
//    nzmqt::ZMQSocket    *mSocket;
//    /** State for stopping the process */
//    volatile bool       mIsStopped;
//};

//}   // namespace izmqt

#endif // IZMQT_MONITOR_H
