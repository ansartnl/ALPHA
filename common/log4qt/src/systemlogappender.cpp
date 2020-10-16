#include "systemlogappender.h"

#include "layout.h"
#include "level.h"
#include "loggingevent.h"

#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>

#if defined(Q_OS_WIN)
#ifndef UNICODE
#define UNICODE
#endif
#include <qt_windows.h>
#include <QtCore/QLibrary>


typedef HANDLE(WINAPI*PDeregisterEventSource)(HANDLE);
static PDeregisterEventSource pDeregisterEventSource = 0;
typedef BOOL(WINAPI*PReportEvent)(HANDLE,WORD,WORD,DWORD,PSID,WORD,DWORD,LPCTSTR*,LPVOID);
static PReportEvent pReportEvent = 0;
typedef HANDLE(WINAPI*PRegisterEventSource)(LPCTSTR,LPCTSTR);
static PRegisterEventSource pRegisterEventSource = 0;

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)

template < class FunctionPtr >
FunctionPtr union_cast( void* objectPtr ) {
    union {
        void* obj;
        FunctionPtr func;
    } var;

    var.obj = objectPtr;
    return var.func;
}

#else

template < class FunctionPtr >
FunctionPtr union_cast( QFunctionPointer objectPtr ) {
    union {
        QFunctionPointer obj;
        FunctionPtr func;
    } var;

    var.obj = objectPtr;
    return var.func;
}

#endif

//#define RESOLVE(name) p##name = (P##name)lib.resolve(#name);
//#define RESOLVEA(name) p##name = (P##name)lib.resolve(#name"A");
//#define RESOLVEW(name) p##name = (P##name)lib.resolve(#name"W");

#define RESOLVE(name) p##name = union_cast<P##name>(lib.resolve(#name));
#define RESOLVEA(name) p##name = union_cast<P##name>(lib.resolve(#name"A"));
#define RESOLVEW(name) p##name = union_cast<P##name>(lib.resolve(#name"W"));

static bool winServiceInit()
{
    if (!pDeregisterEventSource) {
        QLibrary lib("advapi32");

        // only resolve unicode versions
        RESOLVE(DeregisterEventSource);
        RESOLVEW(ReportEvent);
        RESOLVEW(RegisterEventSource);
    }
    return pDeregisterEventSource != 0;
}
#else

#include <pwd.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <signal.h>
#include <sys/stat.h>

static QString encodeName(const QString &name, bool allowUpper = false)
{
    QString n = name.toLower();
    QString legal = QLatin1String("abcdefghijklmnopqrstuvwxyz1234567890");
    if (allowUpper)
        legal += QLatin1String("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    int pos = 0;
    while (pos < n.size()) {
        if (legal.indexOf(n[pos]) == -1)
            n.remove(pos, 1);
        else
            ++pos;
    }
    return n;
}
#endif //#if defined(Q_WS_WIN)
namespace Log4Qt {
    SystemLogAppender::SystemLogAppender(QObject *parent) :
            AppenderSkeleton(parent), ident(0)
    {
        setServiceName(QCoreApplication::applicationName());
    }

    SystemLogAppender::~SystemLogAppender()
    {
        if (ident)
            delete[] ident;
    }

    void SystemLogAppender::append(const LoggingEvent &rEvent)
    {
        QString message(layout()->format(rEvent));

#if defined(Q_OS_WIN)
        //  Q_D(QtServiceBase);
        if (!winServiceInit())
            return;
        WORD wType;
        switch (rEvent.level().toInt()) {
        case Level::WARN_INT:
            wType = EVENTLOG_WARNING_TYPE;
            break;
        case Level::ERROR_INT:
            wType = EVENTLOG_ERROR_TYPE;
            break;
        case Level::FATAL_INT:
            wType = EVENTLOG_ERROR_TYPE;
            break;
        case Level::OFF_INT:
            wType = EVENTLOG_SUCCESS;
            break;
        default:
            wType = EVENTLOG_INFORMATION_TYPE;
            break;
        }

        HANDLE h = pRegisterEventSource(0, (wchar_t *) serviceName().utf16());
        if (h) {
            int id = 0;
            uint category = 0;
            const wchar_t *msg = (wchar_t*) message.utf16();
            const char *bindata = 0;//data.size() ? data.constData() : 0;
            const int datasize = 0;
            pReportEvent(h, wType, category, id, 0, 1, datasize,
                         (const wchar_t **) &msg, const_cast<char *> (bindata));
            pDeregisterEventSource(h);
        }
#else

        int st;
        switch (rEvent.level().toInt()) {
        case Level::WARN_INT:
            st = LOG_WARNING;
            break;
        case Level::ERROR_INT:
            st = LOG_ERR;
            break;
        case Level::FATAL_INT:
            st = LOG_ERR;
            break;
        default:
            st = LOG_INFO;
        }

        openlog(ident, LOG_PID, LOG_DAEMON);
        foreach(const QString& line, message.split('\n', QString::SkipEmptyParts))
            syslog(st, "%s", line.toLocal8Bit().constData());
        closelog();

#endif //#if defined(Q_WS_WIN)
    }

    QString SystemLogAppender::serviceName() const
    {
        return mServiceName;
    }

    void SystemLogAppender::setServiceName(const QString& serviceName)
    {
        mServiceName = serviceName;

#if !defined(Q_OS_WIN)
        if (ident)
            delete[] ident;
        QString tmp = encodeName(mServiceName, true);
        int len = tmp.toLocal8Bit().size();
        ident = new char[len + 1];
        ident[len] = '\0';
        ::memcpy(ident, tmp.toLocal8Bit().constData(), len);
#endif
    }

#ifndef QT_NO_DEBUG_STREAM
    QDebug SystemLogAppender::debug(QDebug &rDebug) const
    {
        QString layout_name;
        if (layout())
            layout_name = layout()->name();

        rDebug.nospace() << "WriterAppender(" << "name:" << name() << " "
                << "filter:" << firstFilter() << "isactive:" << isActive() << "isclosed:"
                << isClosed() << "layout:" << layout_name << "referencecount:"
                << referenceCount() << " " << "threshold:" << threshold().toString()
                << "serviceName:" << serviceName() << ")";
        return rDebug.space();
    }
#endif // QT_NO_DEBUG_STREAM
}

