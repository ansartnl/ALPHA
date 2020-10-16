#ifndef _QTSERIALPORT_H_
#define _QTSERIALPORT_H_

#include "qplatformdefs.h"

#if defined(Q_OS_UNIX) /*POSIX CODE*/
    #include "posix_qextserialport.h"
    #define QextBaseType Posix_QextSerialPort
#elif defined(Q_OS_WIN) /*MS WINDOWS CODE*/
    #include "win_qextserialport.h"
    #define QextBaseType Win_QextSerialPort
#endif

class QtSerialPort: public QextBaseType {
public:
    QtSerialPort(QObject *parent = 0);
    QtSerialPort(const QString & name, QObject *parent = 0);
    QtSerialPort(PortSettings const& s, QObject *parent = 0);
    QtSerialPort(const QString & name, PortSettings const& s, QObject *parent = 0);
    QtSerialPort(const QtSerialPort& s, QObject *parent = 0);
    virtual ~QtSerialPort();

    virtual void setDataBits(const QString &DataBits);
    virtual void setParity(const QString &Parity);
    virtual void setStopBits(const QString &StopBits);

    static const QMap<QextSerialBase::DataBitsType, QString> m_dataBitsMap;
    static const QMap<QextSerialBase::ParityType, QString> m_parityMap;
    static const QMap<QextSerialBase::StopBitsType, QString> m_stopBitsMap;
    static const QMap<QextSerialBase::FlowType, QString> m_flowMap;

    virtual bool waitForReadyRead(int msecs);

    using QextSerialBase::setDataBits;
    using QextSerialBase::setParity;
    using QextSerialBase::setStopBits;

    virtual void setCharIntervalTimeout(long msecs = 50);
    virtual long charIntervalTimeout() const;

protected:
    virtual qint64 writeData(const char *data, qint64 maxSize);
    virtual qint64 readData(char *data, qint64 maxSize);
    long m_charIntervalTimeout;

private:
    Q_DISABLE_COPY(QtSerialPort);
};

#endif
