#include "Monitor.h"

//#include "nzmqt/nzmqt.hpp"

//#include <QDebug>
//#include <QUuid>
//#include <QTimer>

//namespace izmqt
//{

//Monitor::Monitor(nzmqt::ZMQContext *context, nzmqt::ZMQSocket *socket, QObject* parent)
//    : QObject(parent), mIsStopped(false)
//{
//    QString address = QString("inproc://monitor.%1").arg( QUuid::createUuid().toString() );

//    try
//    {
//        socket->monitorTo(address, ZMQ_EVENT_ALL);

//        mSocket = context->createSocket(nzmqt::ZMQSocket::TYP_PAIR);
//        Q_ASSERT(mSocket);

//        mSocket->connectTo( address );
//    }
//    catch ( const zmq::error_t &e )
//    {
//        qDebug() << tr("Monitor> Error: ") << e.what();
//    }
//}

//void Monitor::start()
//{
//    mIsStopped = false;

//    QTimer::singleShot(0, this, SLOT(run()));
//}

//void Monitor::quit()
//{
//    mIsStopped = true;
//}

//void Monitor::run()
//{
//    zmq_event_t event;

//    try
//    {
//        nzmqt::ZMQMessage msg;
//        mSocket->receiveMessage(&msg);

//        memcpy( &event, msg.data(), sizeof(event) );

//        switch (event.event)
//        {
//        case ZMQ_EVENT_LISTENING:
//            qDebug() << "listening socket descriptor " << event.data.listening.fd;
//            qDebug() << "listening socket address " << event.data.listening.addr;
//            break;
//        case ZMQ_EVENT_ACCEPTED:
//            qDebug() << "accepted socket descriptor " << event.data.accepted.fd;
//            qDebug() << "accepted socket address " << event.data.accepted.addr;
//            break;
//        case ZMQ_EVENT_CLOSE_FAILED:
//            qDebug() << "socket close failure error code " << event.data.close_failed.err;
//            qDebug() << "socket address " << event.data.close_failed.addr;
//            break;
//        case ZMQ_EVENT_CLOSED:
//            qDebug() << "closed socket descriptor " << event.data.closed.fd;
//            qDebug() << "closed socket address " << event.data.closed.addr;
//            break;
//        case ZMQ_EVENT_CONNECTED:
//            qDebug() << "connected socket descriptor " << event.data.disconnected.fd;
//            qDebug() << "connected socket address " << event.data.disconnected.addr;
//            break;
//        case ZMQ_EVENT_DISCONNECTED:
//            qDebug() << "disconnected socket descriptor " << event.data.disconnected.fd;
//            qDebug() << "disconnected socket address " << event.data.disconnected.addr;
//            break;
//        }

//        if ( !mIsStopped )
//            QTimer::singleShot(5, this, SLOT(run()));
//    }
//    catch ( const zmq::error_t &e )
//    {
//        qDebug() << tr("Monitor> Error: ") << e.what();
//    }
//}

//} // namespace izmqt
