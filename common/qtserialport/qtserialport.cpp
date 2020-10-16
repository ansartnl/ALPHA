/*!
\class QextSerialPort
\version 1.0.0
\author Stefan Sander

A cross-platform serial port class.
This class encapsulates a serial port on both POSIX and Windows systems.  The user will be
notified of errors and possible portability conflicts at run-time by default - this behavior can
be turned off by defining _TTY_NOWARN_ (to turn off all warnings) or _TTY_NOWARN_PORT_ (to turn
off portability warnings) in the project.

\note
On Windows NT/2000/XP this class uses Win32 serial port functions by default.  The user may
select POSIX behavior under NT, 2000, or XP ONLY by defining _TTY_POSIX_ in the project. I can
make no guarantees as to the quality of POSIX support under NT/2000 however.

*/

#include <stdio.h>
#include "qtserialport.h"
#include <QtCore/QTime>
#include <QtCore/QThread>

class qSleep : private QThread
{
public:
    static void usleep(long microseconds) { QThread::usleep(microseconds); }
    static void sleep(long seconds) { QThread::sleep(seconds); }
    static void msleep(long milliseconds) { QThread::msleep(milliseconds); }
};

/*!
\fn QextSerialPort::QextSerialPort()
Default constructor.  Note that the naming convention used by a QextSerialPort constructed with
this constructor will be determined by #defined constants, or lack thereof - the default behavior
is the same as _TTY_LINUX_.  Possible naming conventions and their associated constants are:

\verbatim

Constant         Used By         Naming Convention
----------       -------------   ------------------------
_TTY_WIN_        Windows         COM1, COM2
_TTY_IRIX_       SGI/IRIX        /dev/ttyf1, /dev/ttyf2
_TTY_HPUX_       HP-UX           /dev/tty1p0, /dev/tty2p0
_TTY_SUN_        SunOS/Solaris   /dev/ttya, /dev/ttyb
_TTY_DIGITAL_    Digital UNIX    /dev/tty01, /dev/tty02
_TTY_FREEBSD_    FreeBSD         /dev/ttyd0, /dev/ttyd1
_TTY_LINUX_      Linux           /dev/ttyS0, /dev/ttyS1
<none>           Linux           /dev/ttyS0, /dev/ttyS1
\endverbatim

The object will be associated with the first port in the system, e.g. COM1 on Windows systems.
See the other constructors if you need to use a port other than the first.
*/
QtSerialPort::QtSerialPort(QObject *parent)
    : QextBaseType(parent), m_charIntervalTimeout(100)
{}

/*!
\fn QextSerialPort::QextSerialPort(const QString & name)
Constructs a serial port attached to the port specified by name.
name is the name of the device, which is windowsystem-specific,
e.g."COM1" or "/dev/ttyS0".
*/
QtSerialPort::QtSerialPort(const QString & name, QObject *parent)
    : QextBaseType(name, parent), m_charIntervalTimeout(100)
{}

/*!
\fn QextSerialPort::QextSerialPort(PortSettings const& settings)
Constructs a port with default name and settings specified by the settings parameter.
*/
QtSerialPort::QtSerialPort(PortSettings const& settings, QObject *parent)
    : QextBaseType(settings, parent), m_charIntervalTimeout(100)
{}

/*!
\fn QextSerialPort::QextSerialPort(const QString & name, PortSettings const& settings)
Constructs a port with the name and settings specified.
*/
QtSerialPort::QtSerialPort(const QString & name, PortSettings const& settings, QObject *parent)
    : QextBaseType(name, settings, parent), m_charIntervalTimeout(100)
{}

/*!
\fn QextSerialPort::QextSerialPort(const QextSerialPort& s)
Copy constructor.
*/
QtSerialPort::QtSerialPort(const QtSerialPort& s, QObject *parent)
    : QextBaseType(s, parent), m_charIntervalTimeout(100)
{}

/*!
\fn QextSerialPort::~QextSerialPort()
Standard destructor.
*/
QtSerialPort::~QtSerialPort()
{}

bool QtSerialPort::waitForReadyRead(int msecs)
{
    QTime stopWatch;
    stopWatch.start();

    while (msecs > stopWatch.elapsed())
    {
        if (bytesAvailable() > 0)
            return true;
        qSleep::msleep(100);
    }
    return false;
}

//static QMap<QextBaseType::BaudRate, QString> initBaudRateMap()
//{
//    QMap<QextBaseType::BaudRate, QString> result;
//    result[QextBaseType::BaudRate50] = "50 baud";
//    result[QextBaseType::BaudRate75] = "75 baud";
//    result[QextBaseType::BaudRate110] = "110 baud";
//    result[QextBaseType::BaudRate134] = "134 baud";
//    result[QextBaseType::BaudRate150] = "150 baud";
//    result[QextBaseType::BaudRate200] = "200 baud";
//    result[QextBaseType::BaudRate300] = "300 baud";
//    result[QextBaseType::BaudRate600] = "600 baud";
//    result[QextBaseType::BaudRate1200] = "1200 baud";
//    result[QextBaseType::BaudRate1800] = "1800 baud";
//    result[QextBaseType::BaudRate2400] = "2400 baud";
//    result[QextBaseType::BaudRate4800] = "4800 baud";
//    result[QextBaseType::BaudRate9600] = "9600 baud";
//    result[QextBaseType::BaudRate14400] = "14400 baud";
//    result[QextBaseType::BaudRate19200] = "19200 baud";
//    result[QextBaseType::BaudRate38400] = "38400 baud";
//    result[QextBaseType::BaudRate56000] = "56000 baud";
//    result[QextBaseType::BaudRate57600] = "57600 baud";
//    result[QextBaseType::BaudRate76800] = "76800 baud";
//    result[QextBaseType::BaudRate115200] = "115200 baud";
//    result[QextBaseType::BaudRate128000] = "128000 baud";
//    result[QextBaseType::BaudRate256000] = "256000 baud";
//    return result;
//}

static QMap<QextBaseType::DataBitsType, QString> initDataBitsMap()
{
    QMap<QextBaseType::DataBitsType, QString> result;
    result[QextBaseType::DATA_5] = "5 bit";
    result[QextBaseType::DATA_6] = "6 bit";
    result[QextBaseType::DATA_7] = "7 bit";
    result[QextBaseType::DATA_8] = "8 bit";
    return result;
}

static QMap<QextBaseType::ParityType, QString> initParityMap()
{
    QMap<QextBaseType::ParityType, QString> result;
    result[QextBaseType::PAR_NONE] = "None";
    result[QextBaseType::PAR_ODD] = "Odd";
    result[QextBaseType::PAR_EVEN] = "Even";
    result[QextBaseType::PAR_MARK] = "Mark";
    result[QextBaseType::PAR_SPACE] = "Space";
    return result;
}

static QMap<QextBaseType::StopBitsType, QString> initStopBitsMap()
{
    QMap<QextBaseType::StopBitsType, QString> result;
    result[QextBaseType::STOP_1] = "1";
    result[QextBaseType::STOP_1_5] = "1.5";
    result[QextBaseType::STOP_2] = "2";
    return result;
}

static QMap<QextBaseType::FlowType, QString> initFlowMap()
{
    QMap<QextBaseType::FlowType, QString> result;
    result[QextBaseType::FLOW_OFF] = "Off";
    result[QextBaseType::FLOW_HARDWARE] = "Hardware";
    result[QextBaseType::FLOW_XONXOFF] = "Xon/Xoff";
    return result;
}

// const QMap<QextBaseType::BaudRate, QString> QtSerialPort::m_baudRateMap = initBaudRateMap();
const QMap<QextBaseType::DataBitsType, QString> QtSerialPort::m_dataBitsMap = initDataBitsMap();
const QMap<QextBaseType::ParityType, QString> QtSerialPort::m_parityMap = initParityMap();
const QMap<QextBaseType::StopBitsType, QString> QtSerialPort::m_stopBitsMap = initStopBitsMap();
const QMap<QextBaseType::FlowType, QString> QtSerialPort::m_flowMap = initFlowMap();

void QtSerialPort::setDataBits(const QString &DataBits)
{
    setDataBits(m_dataBitsMap.key(DataBits, dataBits()));
}

void QtSerialPort::setParity(const QString &Parity)
{
    setParity(m_parityMap.key(Parity, parity()));
}

void QtSerialPort::setStopBits(const QString &StopBits)
{
    setStopBits(m_stopBitsMap.key(StopBits, stopBits()));
}

void QtSerialPort::setCharIntervalTimeout(long msecs)
{
    m_charIntervalTimeout = msecs;
}

long QtSerialPort::charIntervalTimeout() const
{
    return m_charIntervalTimeout;
}

qint64 QtSerialPort::writeData(const char *data, qint64 maxSize)
{
    enum { WRITE_CHUNK_SIZE = 1024 };
    qint64 ret = 0;
    for(;;) {
        qint64 bytesToSend = qMin<qint64>(WRITE_CHUNK_SIZE, maxSize - ret);
        qint64 bytesWritten = QextBaseType::writeData((const char*)(data + ret), bytesToSend);

        if ( (bytesWritten < 0) || (bytesWritten != bytesToSend) )
            return qint64(-1);

        ret += bytesWritten;

        if (ret == maxSize)
            break;
    }
    return ret;
}

qint64 QtSerialPort::readData(char *data, qint64 maxSize)
{
    qint64 readBytes = 0;
    qint64 ret = size();
    while (ret > 0) {
        ret = qMin<qint64>(ret, maxSize - readBytes);
        if (ret > 0) {
            ret = QextBaseType::readData(&data[readBytes], ret);
            if (ret > 0) {
                readBytes += ret;
                if (readBytes == maxSize)
                    break;
            }
            else
                break;
        }
        ret = bytesAvailable();
    }
    return readBytes;
}

