#ifndef MOXASETTINGS_H
#define MOXASETTINGS_H

#include "MoxaSettingsCommon.h"

#include <QtCore/QQueue>

class MoxaSettingsPrivate;

class MoxaSettings : public QObject
{
    Q_OBJECT
public:
    explicit MoxaSettings(QObject *parent = 0);
    virtual ~MoxaSettings();

public slots:
    void setHost(const QString &h);
    void setPort(quint16 p);

    void getBaudRate();
    void setBaudRate(int rate);

signals:
    void error(const QString &text);
    void debug(const QString &text);

    void baudRate(int rate);
    void baudRateSet();

protected:
    struct Request
    {
        Field::Field field;
        bool isSetRequest;
        QChar index;

        Request() :
            field(Field::F_NONE), isSetRequest(false)
        {}
        Request(Field::Field f, bool s, const QChar &i = QChar()) :
            field(f), isSetRequest(s), index(i)
        {}

        inline static const Request get(Field::Field f)
        {
            return Request(f, false);
        }
        inline static const Request set(Field::Field f, const QChar &i)
        {
            return Request(f, true, i);
        }

        inline void clear()
        {
            field = Field::F_NONE;
            isSetRequest = false;
            index = QChar();
        }
    };

    void processNextRequest();

protected slots:
    void getFinished(const QString &v);
    void setFinished();

protected:
    MoxaSettingsPrivate *d;
    QThread *thread;

    QQueue < Request > mRequestList;
    Request mCurrentRequest;
};

#endif // MOXASETTINGS_H
