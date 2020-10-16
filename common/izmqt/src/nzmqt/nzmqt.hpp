// Copyright 2011-2013 Johann Duscher (a.k.a. Jonny Dee). All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//
//    1. Redistributions of source code must retain the above copyright notice, this list of
//       conditions and the following disclaimer.
//
//    2. Redistributions in binary form must reproduce the above copyright notice, this list
//       of conditions and the following disclaimer in the documentation and/or other materials
//       provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY JOHANN DUSCHER ''AS IS'' AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
// FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// The views and conclusions contained in the software and documentation are those of the
// authors and should not be interpreted as representing official policies, either expressed
// or implied, of Johann Duscher.

#ifndef NZMQT_H
#define NZMQT_H

#include <zmq.hpp>

#include <QByteArray>
#include <QFlag>
#include <QList>
#include <QMetaType>
#include <QMutex>
#include <QObject>
#include <QRunnable>
#include <QVector>
#include <QDebug>
#include <QMutexLocker>
#include <QSocketNotifier>
#include <QTimer>


// Define default context implementation to be used.
#ifndef NZMQT_DEFAULT_ZMQCONTEXT_IMPLEMENTATION
    #define NZMQT_DEFAULT_ZMQCONTEXT_IMPLEMENTATION PollingZMQContext
    //#define NZMQT_DEFAULT_ZMQCONTEXT_IMPLEMENTATION SocketNotifierZMQContext
#endif

// Define default number of IO threads to be used by ZMQ.
#ifndef NZMQT_DEFAULT_IOTHREADS
    #define NZMQT_DEFAULT_IOTHREADS 4
#endif

// Define default poll interval for polling-based implementation.
#ifndef NZMQT_POLLINGZMQCONTEXT_DEFAULT_POLLINTERVAL
    #define NZMQT_POLLINGZMQCONTEXT_DEFAULT_POLLINTERVAL 10 /* msec */
#endif

// Declare metatypes for using them in Qt signals.
Q_DECLARE_METATYPE(QList< QList<QByteArray> >)
Q_DECLARE_METATYPE(QList<QByteArray>)

class QSocketNotifier;

namespace nzmqt
{
    typedef zmq::free_fn free_fn;
    typedef zmq::pollitem_t pollitem_t;

    typedef zmq::error_t ZMQException;

    using zmq::poll;
    using zmq::version;

    // This class wraps ZMQ's message structure.
    class ZMQMessage : private zmq::message_t
    {
        friend class ZMQSocket;

        typedef zmq::message_t super;

    public:
        ZMQMessage();

        ZMQMessage(size_t size_);

        ZMQMessage(void* data_, size_t size_, free_fn *ffn_, void* hint_ = 0);

        ZMQMessage(const QByteArray& b);

        using super::rebuild;

        void move(ZMQMessage* msg_);

        void copy(ZMQMessage* msg_);

        using super::more;

        void clone(ZMQMessage* msg_);

        using super::data;

        using super::size;

        QByteArray toByteArray();
    };

    class ZMQContext;

    // This class cannot be instantiated. Its purpose is to serve as an
    // intermediate base class that provides Qt-based convenience methods
    // to subclasses.
    class ZMQSocket : public QObject, private zmq::socket_t
    {
        Q_OBJECT
        Q_ENUMS(Type Event SendFlag ReceiveFlag Option)
        Q_FLAGS(Event Events)
        Q_FLAGS(SendFlag SendFlags)
        Q_FLAGS(ReceiveFlag ReceiveFlags)

        typedef QObject qsuper;
        typedef zmq::socket_t zmqsuper;

    public:
        enum Type
        {
            TYP_PUB = ZMQ_PUB,
            TYP_SUB = ZMQ_SUB,
            TYP_PUSH = ZMQ_PUSH,
            TYP_PULL = ZMQ_PULL,
            TYP_REQ = ZMQ_REQ,
            TYP_REP = ZMQ_REP,
            TYP_DEALER = ZMQ_DEALER,
            TYP_ROUTER = ZMQ_ROUTER,
            TYP_PAIR = ZMQ_PAIR,
            TYP_XPUB = ZMQ_XPUB,
            TYP_XSUB = ZMQ_XSUB
        };

        enum Event
        {
            EVT_POLLIN = ZMQ_POLLIN,
            EVT_POLLOUT = ZMQ_POLLOUT,
            EVT_POLLERR = ZMQ_POLLERR
        };
        Q_DECLARE_FLAGS(Events, Event)

        enum SendFlag
        {
            SND_MORE = ZMQ_SNDMORE,
            SND_NOBLOCK = ZMQ_DONTWAIT
        };
        Q_DECLARE_FLAGS(SendFlags, SendFlag)

        enum ReceiveFlag
        {
            RCV_NOBLOCK = ZMQ_DONTWAIT
        };
        Q_DECLARE_FLAGS(ReceiveFlags, ReceiveFlag)

        enum Option
        {
            // Get only.
            OPT_TYPE = ZMQ_TYPE,
            OPT_RCVMORE = ZMQ_RCVMORE,
            OPT_FD = ZMQ_FD,
            OPT_EVENTS = ZMQ_EVENTS,

            // Set only.
            OPT_SUBSCRIBE = ZMQ_SUBSCRIBE,
            OPT_UNSUBSCRIBE = ZMQ_UNSUBSCRIBE,

            // Get and set.
            OPT_AFFINITY = ZMQ_AFFINITY,
            OPT_IDENTITY = ZMQ_IDENTITY,
            OPT_RATE = ZMQ_RATE,
            OPT_RECOVERY_IVL = ZMQ_RECOVERY_IVL,
            OPT_SNDBUF = ZMQ_SNDBUF,
            OPT_RCVBUF = ZMQ_RCVBUF,
            OPT_LINGER = ZMQ_LINGER,
            OPT_RECONNECT_IVL = ZMQ_RECONNECT_IVL,
            OPT_RECONNECT_IVL_MAX = ZMQ_RECONNECT_IVL_MAX,
            OPT_BACKLOG = ZMQ_BACKLOG,
            OPT_RCVHWM = ZMQ_RCVHWM,
            OPT_SNDHWM = ZMQ_SNDHWM
        };

        ~ZMQSocket();

        using zmqsuper::operator void *;

        void disconnectFromHost(const QString& address);

        void unbind(const QString& address);

        void setOption(Option optName_, const void *optionVal_, size_t optionValLen_);

        void setOption(Option optName_, const char* str_);

        void setOption(Option optName_, const QByteArray& bytes_);

        void setOption(Option optName_, qint32 value_);

        void setOption(Option optName_, quint32 value_);

        void setOption(Option optName_, qint64 value_);

        void setOption(Option optName_, quint64 value_);

        void getOption(Option option_, void *optval_, size_t *optvallen_) const;

        void bindTo(const QString& addr_);

        void bindTo(const char *addr_);

        void connectTo(const QString& addr_);

        void connectTo(const char* addr_);

        bool sendMessage(ZMQMessage& msg_, SendFlags flags_ = SND_NOBLOCK);

        bool sendMessage(const QByteArray& bytes_, SendFlags flags_ = SND_NOBLOCK);

        // Interprets the provided list of byte arrays as a multi-part message
        // and sends them accordingly.
        // If an empty list is provided this method doesn't do anything and returns trua.
        bool sendMessage(const QList<QByteArray>& msg_, SendFlags flags_ = SND_NOBLOCK);

        // Receives a message or a message part.
        bool receiveMessage(ZMQMessage* msg_, ReceiveFlags flags_ = RCV_NOBLOCK);

        // Receives a message.
        // The message is represented as a list of byte arrays representing
        // a message's parts. If the message is not a multi-part message the
        // list will only contain one array.
        QList<QByteArray> receiveMessage();

        // Receives all messages currently available.
        // Each message is represented as a list of byte arrays representing the messages
        // and their parts in case of multi-part messages. If a message isn't a multi-part
        // message the corresponding byte array list will only contain one element.
        // Note that this method won't work with REQ-REP protocol.
        QList< QList<QByteArray> > receiveMessages();

        qint32 fileDescriptor() const;

        Events events() const;

        // Returns true if there are more parts of a multi-part message
        // to be received.
        bool hasMoreMessageParts() const;

        void setIdentity(const char* nameStr_);

        void setIdentity(const QString& name_);

        void setIdentity(const QByteArray& name_);

        QByteArray identity() const;

        void setLinger(int msec_);

        qint32 linger() const;

        void subscribeTo(const char* filterStr_);

        void subscribeTo(const QString& filter_);

        void subscribeTo(const QByteArray& filter_);

        void unsubscribeFrom(const char* filterStr_);

        void unsubscribeFrom(const QString& filter_);

        void unsubscribeFrom(const QByteArray& filter_);

    signals:
        void messageReceived(const QList<QByteArray>&);
        void sentSize(quint32);
        void receivedSize(quint32);

    public slots:
        void close();

    protected:
        ZMQSocket(ZMQContext* context_, Type type_);

    private:
        friend class ZMQContext;

        ZMQContext* m_context;
    };
    Q_DECLARE_OPERATORS_FOR_FLAGS(ZMQSocket::Events)
    Q_DECLARE_OPERATORS_FOR_FLAGS(ZMQSocket::SendFlags)
    Q_DECLARE_OPERATORS_FOR_FLAGS(ZMQSocket::ReceiveFlags)


    // This class is an abstract base class for concrete implementations.
    class ZMQContext : public QObject, private zmq::context_t
    {
        Q_OBJECT

        typedef QObject qsuper;
        typedef zmq::context_t zmqsuper;

        friend class ZMQSocket;

    public:
        ZMQContext(QObject* parent_ = 0, int io_threads_ = NZMQT_DEFAULT_IOTHREADS);

        // Deleting children is necessary, because otherwise the children are deleted after the context
        // which results in a blocking state. So we delete the children before the zmq::context_t
        // destructor implementation is called.
        ~ZMQContext();

        using zmqsuper::operator void*;

        // Creates a socket instance of the specified type and parent.
        // The created instance will have the specified parent
        // (as usual you can also call 'ZMQSocket::setParent()' method to change
        // ownership later on). Make sure, however, that the socket's parent
        // belongs to the same thread as the socket instance itself (as it is required
        // by Qt). Otherwise, you will encounter strange errors.
        ZMQSocket* createSocket(ZMQSocket::Type type_, QObject* parent_ = 0);

        // Start watching for incoming messages.
        virtual void start() = 0;

        // Stop watching for incoming messages.
        virtual void stop() = 0;

        // Indicates if watching for incoming messages is enabled.
        virtual bool isStopped() const = 0;

    protected:
        typedef QVector<ZMQSocket*> Sockets;

        // Creates a socket instance of the specified type.
        virtual ZMQSocket* createSocketInternal(ZMQSocket::Type type_) = 0;

        virtual void registerSocket(ZMQSocket* socket_);

        // Remove the given socket object from the list of registered sockets.
        virtual void unregisterSocket(ZMQSocket* socket_);

        virtual const Sockets& registeredSockets() const;

    private:
        Sockets m_sockets;
    };

/*
    class ZMQDevice : public QObject, public QRunnable
    {
        Q_OBJECT
        Q_ENUMS(Type)

    public:
        enum Type
        {
            TYP_QUEUE = ZMQ_QUEUE,
            TYP_FORWARDED = ZMQ_FORWARDER,
            TYP_STREAMER = ZMQ_STREAMER
        };

        ZMQDevice(Type type, ZMQSocket* frontend, ZMQSocket* backend);

        void run();

    private:
        Type type_;
        ZMQSocket* frontend_;
        ZMQSocket* backend_;
    };
*/

    class PollingZMQContext;

    // An instance of this class cannot directly be created. Use one
    // of the 'PollingZMQContext::createSocket()' factory methods instead.
    class PollingZMQSocket : public ZMQSocket
    {
        Q_OBJECT

        typedef ZMQSocket super;

        friend class PollingZMQContext;

    protected:
        PollingZMQSocket(PollingZMQContext* context_, Type type_);

        // This method is called by the socket's context object in order
        // to signal a new received message.
        void onMessageReceived(const QList<QByteArray>& message);
    };

    class PollingZMQContext : public ZMQContext, public QRunnable
    {
        Q_OBJECT

        typedef ZMQContext super;

    public:
        PollingZMQContext(QObject* parent_ = 0, int io_threads_ = NZMQT_DEFAULT_IOTHREADS);

        // Sets the polling interval.
        // Note that the interval does not denote the time the zmq::poll() function will
        // block in order to wait for incoming messages. Instead, it denotes the time in-between
        // consecutive zmq::poll() calls.
        void setInterval(int interval_);

        int getInterval() const;

        // Starts the polling process by scheduling a call to the 'run()' method into Qt's event loop.
        void start();

        // Stops the polling process in the sense that no further 'run()' calls will be scheduled into
        // Qt's event loop.
        void stop();

        bool isStopped() const;

    public slots:
        // If the polling process is not stopped (by a previous call to the 'stop()' method) this
        // method will call the 'poll()' method once and re-schedule a subsequent call to this method
        // using the current polling interval.
        void run();

        // This method will poll on all currently available poll-items (known ZMQ sockets)
        // using the given timeout to wait for incoming messages. Note that this timeout has
        // nothing to do with the polling interval. Instead, the poll method will block the current
        // thread by waiting at most the specified amount of time for incoming messages.
        // This method is public because it can be called directly if you need to.
        void poll(long timeout_ = 0);

    signals:
        // This signal will be emitted by run() method if a call to poll(...) method
        // results in an exception.
        void pollError(int errorNum, const QString& errorMsg);

    protected:
        PollingZMQSocket* createSocketInternal(ZMQSocket::Type type_);

        // Add the given socket to list list of poll-items.
        void registerSocket(ZMQSocket* socket_);

        // Remove the given socket object from the list of poll-items.
        void unregisterSocket(ZMQSocket* socket_);

    private:
        typedef QVector<pollitem_t> PollItems;

        PollItems m_pollItems;
        QMutex m_pollItemsMutex;
        int m_interval;
        volatile bool m_stopped;
    };


    // An instance of this class cannot directly be created. Use one
    // of the 'SocketNotifierZMQContext::createSocket()' factory methods instead.
    class SocketNotifierZMQSocket : public ZMQSocket
    {
        Q_OBJECT

        friend class SocketNotifierZMQContext;

        typedef ZMQSocket super;

//    public:
//        using super::sendMessage;

//        bool sendMessage(const QByteArray& bytes_, SendFlags flags_ = SND_NOBLOCK);

    protected:
        SocketNotifierZMQSocket(ZMQContext* context_, Type type_);

    protected slots:
        void socketReadActivity();

//        void socketWriteActivity();

    private:
        QSocketNotifier *socketNotifyRead_;
//        QSocketNotifier *socketNotifyWrite_;
    };

    class SocketNotifierZMQContext : public ZMQContext
    {
        Q_OBJECT

        typedef ZMQContext super;

    public:
        SocketNotifierZMQContext(QObject* parent_ = 0, int io_threads_ = NZMQT_DEFAULT_IOTHREADS);

        void start();

        void stop();

        bool isStopped() const;

    protected:
        SocketNotifierZMQSocket* createSocketInternal(ZMQSocket::Type type_);
    };

    inline ZMQContext* createDefaultContext(QObject* parent_ = 0, int io_threads_ = NZMQT_DEFAULT_IOTHREADS)
    {
        return new NZMQT_DEFAULT_ZMQCONTEXT_IMPLEMENTATION(parent_, io_threads_);
    }
}

namespace nzmqt
{

/*
 * ZMQMessage
 */

inline ZMQMessage::ZMQMessage()
    : super()
{
}

inline ZMQMessage::ZMQMessage(size_t size_)
    : super(size_)
{
}

inline ZMQMessage::ZMQMessage(void* data_, size_t size_, free_fn *ffn_, void* hint_)
    : super(data_, size_, ffn_, hint_)
{
}

inline ZMQMessage::ZMQMessage(const QByteArray& b)
    : super(b.size())
{
    memcpy(data(), b.constData(), b.size());
}

inline void ZMQMessage::move(ZMQMessage* msg_)
{
    super::move(static_cast<zmq::message_t*>(msg_));
}

inline void ZMQMessage::copy(ZMQMessage* msg_)
{
    super::copy(msg_);
}

inline void ZMQMessage::clone(ZMQMessage* msg_)
{
    rebuild(msg_->size());
    memcpy(data(), msg_->data(), size());
}

inline QByteArray ZMQMessage::toByteArray()
{
    return QByteArray((const char *)data(), size());
}



/*
 * ZMQSocket
 */

inline ZMQSocket::ZMQSocket(ZMQContext* context_, Type type_)
    : qsuper(0)
    , zmqsuper(*context_, type_)
    , m_context(context_)
{
}

inline ZMQSocket::~ZMQSocket()
{
//    qDebug() << Q_FUNC_INFO << "Context:" << m_context;
    close();
}

inline void ZMQSocket::disconnectFromHost(const QString &address)
{
    zmqsuper::disconnect(address.toStdString().c_str());
}

inline void ZMQSocket::unbind(const QString &address)
{
    zmqsuper::unbind(address.toStdString().c_str());
}

inline void ZMQSocket::close()
{
//    qDebug() << Q_FUNC_INFO << "Context:" << m_context;
    if (m_context)
    {
        m_context->unregisterSocket(this);
        m_context = 0;
    }
    zmqsuper::close();
}

inline void ZMQSocket::setOption(Option optName_, const void *optionVal_, size_t optionValLen_)
{
    setsockopt(optName_, optionVal_, optionValLen_);
}

inline void ZMQSocket::setOption(Option optName_, const char* str_)
{
    setOption(optName_, str_, strlen(str_));
}

inline void ZMQSocket::setOption(Option optName_, const QByteArray& bytes_)
{
    setOption(optName_, bytes_.constData(), bytes_.size());
}

inline void ZMQSocket::setOption(Option optName_, qint32 value_)
{
    setOption(optName_, &value_, sizeof(value_));
}

inline void ZMQSocket::setOption(Option optName_, quint32 value_)
{
    setOption(optName_, &value_, sizeof(value_));
}

inline void ZMQSocket::setOption(Option optName_, qint64 value_)
{
    setOption(optName_, &value_, sizeof(value_));
}

inline void ZMQSocket::setOption(Option optName_, quint64 value_)
{
    setOption(optName_, &value_, sizeof(value_));
}

inline void ZMQSocket::getOption(Option option_, void *optval_, size_t *optvallen_) const
{
    const_cast<ZMQSocket*>(this)->getsockopt(option_, optval_, optvallen_);
}

inline void ZMQSocket::bindTo(const QString& addr_)
{
    bind(addr_.toLocal8Bit());
}

inline void ZMQSocket::bindTo(const char *addr_)
{
    bind(addr_);
}

inline void ZMQSocket::connectTo(const QString& addr_)
{
    zmqsuper::connect(addr_.toLocal8Bit());
}

inline void ZMQSocket::connectTo(const char* addr_)
{
    zmqsuper::connect(addr_);
}

inline bool ZMQSocket::sendMessage(ZMQMessage& msg_, SendFlags flags_)
{
    size_t size = send(msg_.data(), msg_.size(), flags_);
    if (size > 0) {
        emit sentSize(size);
        return true;
    }
    return false;
}

inline bool ZMQSocket::sendMessage(const QByteArray& bytes_, SendFlags flags_)
{
    ZMQMessage msg(bytes_);
    return sendMessage(msg, flags_);
}

inline bool ZMQSocket::sendMessage(const QList<QByteArray>& msg_, SendFlags flags_)
{
    int i;
    for (i = 0; i < msg_.size() - 1; i++)
    {
        if (!sendMessage(msg_[i], flags_ | SND_MORE))
            return false;
    }
    if (i < msg_.size())
        return sendMessage(msg_[i], flags_);

    return true;
}

inline bool ZMQSocket::receiveMessage(ZMQMessage* msg_, ReceiveFlags flags_)
{
    size_t size = recv_ex(msg_, flags_);
    if (size > 0) {
        emit receivedSize(size);
        return true;
    }
    return false;
}

inline QList<QByteArray> ZMQSocket::receiveMessage()
{
    QList<QByteArray> parts;

    ZMQMessage msg;
    while (receiveMessage(&msg))
    {
        parts += msg.toByteArray();
        msg.rebuild();

        if (!hasMoreMessageParts())
            break;
    }

    return parts;
}

inline QList< QList<QByteArray> > ZMQSocket::receiveMessages()
{
    QList< QList<QByteArray> > ret;

    QList<QByteArray> parts = receiveMessage();
    while (!parts.isEmpty())
    {
        ret += parts;

        parts = receiveMessage();
    }

    return ret;
}

inline qint32 ZMQSocket::fileDescriptor() const
{
    qint32 value;
    size_t size = sizeof(value);
    getOption(OPT_FD, &value, &size);
    return value;
}

inline ZMQSocket::Events ZMQSocket::events() const
{
    qint32 value;
    size_t size = sizeof(value);
    getOption(OPT_EVENTS, &value, &size);
    return static_cast<Events>(value);
}

// Returns true if there are more parts of a multi-part message
// to be received.
inline bool ZMQSocket::hasMoreMessageParts() const
{
    qint32 value;
    size_t size = sizeof(value);
    getOption(OPT_RCVMORE, &value, &size);
    return value;
}

inline void ZMQSocket::setIdentity(const char* nameStr_)
{
    setOption(OPT_IDENTITY, nameStr_);
}

inline void ZMQSocket::setIdentity(const QString& name_)
{
    setOption(OPT_IDENTITY, name_.toLocal8Bit());
}

inline void ZMQSocket::setIdentity(const QByteArray& name_)
{
    setOption(OPT_IDENTITY, const_cast<char*>(name_.constData()), name_.size());
}

inline QByteArray ZMQSocket::identity() const
{
    char idbuf[256];
    size_t size = sizeof(idbuf);
    getOption(OPT_IDENTITY, idbuf, &size);
    return QByteArray(idbuf, size);
}

inline void ZMQSocket::setLinger(int msec_)
{
    setOption(OPT_LINGER, msec_);
}

inline qint32 ZMQSocket::linger() const
{
    qint32 msec=-1;
    size_t size = sizeof(msec);
    getOption(OPT_LINGER, &msec, &size);
    return msec;
}

inline void ZMQSocket::subscribeTo(const char* filterStr_)
{
    setOption(OPT_SUBSCRIBE, filterStr_);
}

inline void ZMQSocket::subscribeTo(const QString& filter_)
{
    setOption(OPT_SUBSCRIBE, filter_.toLocal8Bit());
}

inline void ZMQSocket::subscribeTo(const QByteArray& filter_)
{
    setOption(OPT_SUBSCRIBE, filter_);
}

inline void ZMQSocket::unsubscribeFrom(const char* filterStr_)
{
    setOption(OPT_UNSUBSCRIBE, filterStr_);
}

inline void ZMQSocket::unsubscribeFrom(const QString& filter_)
{
    setOption(OPT_UNSUBSCRIBE, filter_.toLocal8Bit());
}

inline void ZMQSocket::unsubscribeFrom(const QByteArray& filter_)
{
    setOption(OPT_UNSUBSCRIBE, filter_);
}



/*
 * ZMQContext
 */

inline ZMQContext::ZMQContext(QObject* parent_, int io_threads_)
    : qsuper(parent_)
    , zmqsuper(io_threads_)
{
}

inline ZMQContext::~ZMQContext()
{
//    qDebug() << Q_FUNC_INFO << "Sockets:" << m_sockets;
    foreach (ZMQSocket* socket, m_sockets)
    {
        socket->m_context = 0;
        // As stated by 0MQ, close() must ONLY be called from the thread
        // owning the socket. So we use 'invokeMethod' which (hopefully)
        // results in a 'close' call from within the socket's thread.
        QMetaObject::invokeMethod(socket, "close");
    }
}

inline ZMQSocket* ZMQContext::createSocket(ZMQSocket::Type type_, QObject* parent_)
{
    ZMQSocket* socket = createSocketInternal(type_);
    registerSocket(socket);
    socket->setParent(parent_);
    return socket;
}

inline void ZMQContext::registerSocket(ZMQSocket* socket_)
{
    m_sockets.push_back(socket_);
}

inline void ZMQContext::unregisterSocket(ZMQSocket* socket_)
{
    Sockets::iterator soIt = m_sockets.begin();
    while (soIt != m_sockets.end())
    {
        if (*soIt == socket_)
        {
            m_sockets.erase(soIt);
            break;
        }
        ++soIt;
    }
}

inline const ZMQContext::Sockets& ZMQContext::registeredSockets() const
{
    return m_sockets;
}



/*
 * ZMQDevice
 */
/*
ZMQDevice::ZMQDevice(Type type, ZMQSocket* frontend, ZMQSocket* backend)
    : type_(type)
    , frontend_(frontend)
    , backend_(backend)
{
}

void ZMQDevice::run()
{
    zmq::device(type_, *frontend_, *backend_);
}
*/


/*
 * PollingZMQSocket
 */

inline PollingZMQSocket::PollingZMQSocket(PollingZMQContext* context_, Type type_)
    : super(context_, type_)
{
}

inline void PollingZMQSocket::onMessageReceived(const QList<QByteArray>& message)
{
    emit messageReceived(message);
}



/*
 * PollingZMQContext
 */

inline PollingZMQContext::PollingZMQContext(QObject* parent_, int io_threads_)
    : super(parent_, io_threads_)
    , m_pollItemsMutex(QMutex::Recursive)
    , m_interval(NZMQT_POLLINGZMQCONTEXT_DEFAULT_POLLINTERVAL)
    , m_stopped(false)
{
    setAutoDelete(false);
}

inline void PollingZMQContext::setInterval(int interval_)
{
    m_interval = interval_;
}

inline int PollingZMQContext::getInterval() const
{
    return m_interval;
}

inline void PollingZMQContext::start()
{
    m_stopped = false;
    QTimer::singleShot(0, this, SLOT(run()));
}

inline void PollingZMQContext::stop()
{
    m_stopped = true;
}

inline bool PollingZMQContext::isStopped() const
{
    return m_stopped;
}

inline void PollingZMQContext::run()
{
    if (m_stopped)
        return;

    try
    {
        poll();
    }
    catch (const ZMQException& ex)
    {
        qWarning("Exception during poll: %s", ex.what());
        emit pollError(ex.num(), ex.what());
    }

    if (!m_stopped)
        QTimer::singleShot(m_interval, this, SLOT(run()));
}

inline void PollingZMQContext::poll(long timeout_)
{
    int cnt;
    do {
        QMutexLocker lock(&m_pollItemsMutex);

        if (m_pollItems.empty())
            return;

        cnt = zmq::poll(&m_pollItems[0], m_pollItems.size(), timeout_);
        Q_ASSERT_X(cnt >= 0, Q_FUNC_INFO, "A value < 0 should be reflected by an exception.");
        if (0 == cnt)
            return;

        PollItems::iterator poIt = m_pollItems.begin();
        ZMQContext::Sockets::const_iterator soIt = registeredSockets().begin();
        int i = 0;
        while (i < cnt && poIt != m_pollItems.end())
        {
            if (poIt->revents & ZMQSocket::EVT_POLLIN)
            {
                PollingZMQSocket* socket = static_cast<PollingZMQSocket*>(*soIt);
                QList<QByteArray> message = socket->receiveMessage();
                socket->onMessageReceived(message);
                i++;
            }
            ++soIt;
            ++poIt;
        }
    } while (cnt > 0);
}

inline PollingZMQSocket* PollingZMQContext::createSocketInternal(ZMQSocket::Type type_)
{
    return new PollingZMQSocket(this, type_);
}

inline void PollingZMQContext::registerSocket(ZMQSocket* socket_)
{
    pollitem_t pollItem = { *socket_, 0, ZMQSocket::EVT_POLLIN, 0 };

    QMutexLocker lock(&m_pollItemsMutex);

    m_pollItems.push_back(pollItem);

    super::registerSocket(socket_);
}

inline void PollingZMQContext::unregisterSocket(ZMQSocket* socket_)
{
    QMutexLocker lock(&m_pollItemsMutex);

    PollItems::iterator poIt = m_pollItems.begin();
    ZMQContext::Sockets::const_iterator soIt = registeredSockets().begin();
    while (soIt != registeredSockets().end())
    {
        if (*soIt == socket_)
        {
            m_pollItems.erase(poIt);
            break;
        }
        ++soIt;
        ++poIt;
    }

    super::unregisterSocket(socket_);
}



/*
 * SocketNotifierZMQSocket
 */

inline SocketNotifierZMQSocket::SocketNotifierZMQSocket(ZMQContext* context_, Type type_)
    : super(context_, type_)
    , socketNotifyRead_(0)
//    , socketNotifyWrite_(0)
{
    int fd = fileDescriptor();

    socketNotifyRead_ = new QSocketNotifier(fd, QSocketNotifier::Read, this);
    QObject::connect(socketNotifyRead_, SIGNAL(activated(int)), this, SLOT(socketReadActivity()));

//    socketNotifyWrite_ = new QSocketNotifier(fd, QSocketNotifier::Write, this);
//    socketNotifyWrite_->setEnabled(false);
//    QObject::connect(socketNotifyWrite_, SIGNAL(activated(int)), this, SLOT(socketWriteActivity()));
}

//bool SocketNotifierZMQSocket::sendMessage(const QByteArray& bytes_, SendFlags flags_)
//{
//    bool result = super::sendMessage(bytes_, flags_);
//
//    if (!result)
//        socketNotifyWrite_->setEnabled(true);
//
//    return result;
//}

inline void SocketNotifierZMQSocket::socketReadActivity()
{
    socketNotifyRead_->setEnabled(false);

    while(events() & EVT_POLLIN)
    {
        QList<QByteArray> message = receiveMessage();
        emit messageReceived(message);
    }

    socketNotifyRead_->setEnabled(true);
}

//void SocketNotifierZMQSocket::socketWriteActivity()
//{
//    if(events() == 0)
//    {
//        socketNotifyWrite_->setEnabled(false);
//    }
//}



/*
 * SocketNotifierZMQContext
 */

inline SocketNotifierZMQContext::SocketNotifierZMQContext(QObject* parent_, int io_threads_)
    : super(parent_, io_threads_)
{
}

inline void SocketNotifierZMQContext::start()
{
}

inline void SocketNotifierZMQContext::stop()
{
}

inline bool SocketNotifierZMQContext::isStopped() const
{
    return false;
}

inline SocketNotifierZMQSocket* SocketNotifierZMQContext::createSocketInternal(ZMQSocket::Type type_)
{
    return new SocketNotifierZMQSocket(this, type_);
}

}

#endif // NZMQT_H
