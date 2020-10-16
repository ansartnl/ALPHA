#ifndef _WIN_QEXTSERIALPORT_H_
#define _WIN_QEXTSERIALPORT_H_

#include "qextserialbase.h"

/*if all warning messages are turned off, flag portability warnings to be turned off as well*/
#ifdef _TTY_NOWARN_
#define _TTY_NOWARN_PORT_
#endif

#include <windows.h>

class Win_QextSerialPort:public QextSerialBase {
public:
    Win_QextSerialPort(QObject *parent = 0);
    Win_QextSerialPort(Win_QextSerialPort const& s, QObject *parent = 0);
    Win_QextSerialPort(const QString & name, QObject *parent = 0);
    Win_QextSerialPort(const PortSettings& settings, QObject *parent = 0);
    Win_QextSerialPort(const QString & name, const PortSettings& settings, QObject *parent = 0);
    virtual ~Win_QextSerialPort();
    virtual bool open(OpenMode mode=0);
    virtual void close();
    virtual void flush();
    virtual qint64 size() const;
    virtual void ungetChar(char c);
    virtual void setFlowControl(FlowType);
    virtual void setParity(ParityType);
    virtual void setDataBits(DataBitsType);
    virtual void setStopBits(StopBitsType);
    virtual bool setBaudRate(BaudRate);
    virtual void setDtr(bool set=true);
    virtual void setRts(bool set=true);
    virtual ulong lineStatus(void);
    virtual qint64 bytesAvailable();
    virtual void translateError(ulong);
    virtual void setTimeout(ulong, ulong);

protected:
    HANDLE Win_Handle;
    COMMCONFIG Win_CommConfig;
    COMMTIMEOUTS Win_CommTimeouts;

    virtual qint64 readData(char *data, qint64 maxSize);
    virtual qint64 writeData(const char *data, qint64 maxSize);

private:
    Q_DISABLE_COPY(Win_QextSerialPort)
};

#endif
