
#ifndef _QEXTSERIALBASE_H_
#define _QEXTSERIALBASE_H_

#include <QtCore/QIODevice>
#include <QtCore/QFile>
#include <QtCore/QMap>
#include <QtCore/QDebug>

/*macros for thread support*/
#ifdef QT_THREAD_SUPPORT
    #include <QThread>
    #include <QMutex>
    #define LOCK_MUTEX() mutex->lock()
    #define UNLOCK_MUTEX() mutex->unlock()
#else
    #define LOCK_MUTEX()
    #define UNLOCK_MUTEX()
#endif

/*if all warning messages are turned off, flag portability warnings to be turned off as well*/
#ifdef _TTY_NOWARN_
#define _TTY_NOWARN_PORT_
#endif

/*macros for warning messages*/
#ifdef _TTY_NOWARN_PORT_
#define TTY_PORTABILITY_WARNING(s)
#else
#define TTY_PORTABILITY_WARNING(s) qWarning(s)
#endif
#ifdef _TTY_NOWARN_
#define TTY_WARNING(s)
#else
#define TTY_WARNING(s) qWarning(s)
#endif

class QextSerialBase : public QIODevice
{
    Q_OBJECT
    Q_ENUMS(BaudRate)
public:

    enum Status {
        E_NO_ERROR = 0,
        E_INVALID_FD,
        E_NO_MEMORY,
        E_CAUGHT_NON_BLOCKED_SIGNAL,
        E_PORT_TIMEOUT,
        E_INVALID_DEVICE,
        E_BREAK_CONDITION,
        E_FRAMING_ERROR,
        E_IO_ERROR,
        E_BUFFER_OVERRUN,
        E_RECEIVE_OVERFLOW,
        E_RECEIVE_PARITY_ERROR,
        E_TRANSMIT_OVERFLOW,
        E_READ_FAILED,
        E_WRITE_FAILED
    };

    enum LineStatusFlag {
        LS_CTS = 0x01,
        LS_DSR = 0x02,
        LS_DCD = 0x04,
        LS_RI  = 0x08,
        LS_RTS = 0x10,
        LS_DTR = 0x20,
        LS_ST  = 0x40,
        LS_SR  = 0x80
    };

    /*enums for port settings*/
    enum NamingConvention {
        WIN_NAMES,
        IRIX_NAMES,
        HPUX_NAMES,
        SUN_NAMES,
        DIGITAL_NAMES,
        FREEBSD_NAMES,
        LINUX_NAMES
    };

    enum BaudRate {
        BaudRateUndefined = -1,
        BaudRate50 = 50,                //POSIX ONLY
        BaudRate75 = 75,                    //POSIX ONLY
        BaudRate110 = 110,
        BaudRate134 = 134,                  //POSIX ONLY
        BaudRate150 = 150,                  //POSIX ONLY
        BaudRate200 = 200,                  //POSIX ONLY
        BaudRate300 = 300,
        BaudRate600 = 600,
        BaudRate1200 = 1200,
        BaudRate1800 = 1800,                //POSIX ONLY
        BaudRate2400 = 2400,
        BaudRate4800 = 4800,
        BaudRate9600 = 9600,
        BaudRate14400 = 14400,              //WINDOWS ONLY
        BaudRate19200 = 19200,
        BaudRate38400 = 38400,
        BaudRate56000 = 56000,              //WINDOWS ONLY
        BaudRate57600 = 57600,
        BaudRate76800 = 76800,              //POSIX ONLY
        BaudRate115200 = 115200,
        BaudRate128000 = 128000,            //WINDOWS ONLY
        BaudRate256000 = 256000             //WINDOWS ONLY
    };

    enum DataBitsType {
        DATA_5,
        DATA_6,
        DATA_7,
        DATA_8
    };

    enum ParityType {
        PAR_NONE,
        PAR_ODD,
        PAR_EVEN,
        PAR_MARK,               //WINDOWS ONLY
        PAR_SPACE
    };

    enum StopBitsType {
        STOP_1,
        STOP_1_5,               //WINDOWS ONLY
        STOP_2
    };

    enum FlowType {
        FLOW_OFF,
        FLOW_HARDWARE,
        FLOW_XONXOFF
    };

    /*structure to contain port settings*/
    struct PortSettings {
        BaudRate baudRate;
        DataBitsType DataBits;
        ParityType Parity;
        StopBitsType StopBits;
        FlowType FlowControl;
        ulong Timeout_Sec;
        ulong Timeout_Millisec;
    };

    explicit QextSerialBase(QObject *parent = 0);
    explicit QextSerialBase(const QString &name, QObject *parent = 0);
    virtual ~QextSerialBase();
    virtual void construct();
    virtual void setPortName(const QString &name);
    virtual QString portName() const;

    virtual bool setBaudRate(BaudRate newBaudRate) = 0;
    virtual BaudRate baudRate() const;
    virtual void setDataBits(DataBitsType)=0;
    virtual DataBitsType dataBits() const;
    virtual void setParity(ParityType)=0;
    virtual ParityType parity() const;
    virtual void setStopBits(StopBitsType)=0;
    virtual StopBitsType stopBits() const;
    virtual void setFlowControl(FlowType)=0;
    virtual FlowType flowControl() const;
    virtual void setTimeout(ulong, ulong)=0;

    virtual bool open(OpenMode mode=0)=0;
    virtual bool isSequential() const;
    virtual void close()=0;
    virtual void flush()=0;

    virtual qint64 size() const=0;
    virtual qint64 bytesAvailable()=0;
    virtual bool atEnd() const;

    virtual void ungetChar(char c)=0;
    virtual qint64 readLine(char * data, qint64 maxSize);

    virtual ulong lastError() const;
    virtual void translateError(ulong error)=0;

    virtual void setDtr(bool set=true)=0;
    virtual void setRts(bool set=true)=0;
    virtual ulong lineStatus()=0;

protected:
    QString port;
    PortSettings Settings;
    ulong lastErr;

#ifdef QT_THREAD_SUPPORT
    static QMutex* mutex;
    static ulong refCount;
#endif

    virtual qint64 readData(char * data, qint64 maxSize)=0;
    virtual qint64 writeData(const char * data, qint64 maxSize)=0;

private:
    Q_DISABLE_COPY(QextSerialBase)
};

QDebug operator<<(QDebug debug, QextSerialBase::BaudRate value);

#endif
