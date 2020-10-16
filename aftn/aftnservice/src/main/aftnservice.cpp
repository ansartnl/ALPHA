#include <QtDebug>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

#include "aftnservice.h"
#include "aftnmessageprocessor.h"
#include "fileworker.h"
#include "ffplworker.h"
#include "rplworker.h"
#include "tablecleaner.h"
#include "networkworker.h"

#include "qtxmlsettings.h"

#include "Pulsar.h"

// Log4Qt
#include "logger.h"
#include "logmanager.h"
#include "patternlayout.h"
#include "colorconsoleappender.h"
#include "rollingfileappender.h"
#include "colorhtmllayout.h"
#include "systemlogappender.h"
#include "telnetappender.h"
#include "helpers/datetime.h"
#include "loggingevent.h"

namespace Log4Qt {
class ColorConsoleLayout: public Layout
{
public:
    ColorConsoleLayout(QObject* parent = 0);
    virtual QString format(const LoggingEvent &rEvent);
    virtual QDebug debug(QDebug &rDebug) const;
};

ColorConsoleLayout::ColorConsoleLayout(QObject *parent) :
    Layout(parent)
{
}
QString ColorConsoleLayout::format(const LoggingEvent &rEvent)
{
    QString formattedMessage = DateTime::fromMilliSeconds(rEvent.timeStamp()).toString(QLocale().dateTimeFormat(
                                                                                           QLocale::ShortFormat));
    formattedMessage.append(" [");
    formattedMessage.append(rEvent.threadName());
    formattedMessage.append("] ");
    formattedMessage.append(rEvent.level().toString());
    formattedMessage.append(" ");
    formattedMessage.append(rEvent.loggerName());
    formattedMessage.append(" ");
    switch (rEvent.level().toInt())
    {
    case Level::DEBUG_INT:
        formattedMessage.append("\e[33m%1\e[0m");
        break;
    case Level::INFO_INT:
        formattedMessage.append("\e[1;34m%1\e[0m");
        break;
    case Level::WARN_INT:
        formattedMessage.append("\e[1;31m%1\e[0m");
        break;
    case Level::ERROR_INT:
        formattedMessage.append("\e[1;31m%1\e[0m");
        break;
    case Level::FATAL_INT:
        formattedMessage.append("\e[1;31m%1\e[0m");
        break;
    }
    formattedMessage.append(endOfLine());
    return formattedMessage.arg(rEvent.message());
}
QDebug ColorConsoleLayout::debug(QDebug &rDebug) const
{
    rDebug.nospace() << metaObject()->className() << "(" << "name:" << objectName() << ")";
    return rDebug.space();
}

class EventLogLayout: public Layout
{
public:
    EventLogLayout(QObject* parent = 0);
    virtual QString format(const LoggingEvent &rEvent);
    virtual QDebug debug(QDebug &rDebug) const;
};

EventLogLayout::EventLogLayout(QObject *parent) :
    Layout(parent)
{
}
QString EventLogLayout::format(const LoggingEvent &rEvent)
{
    QString formattedMessage;
    formattedMessage.append("[");
    formattedMessage.append(rEvent.threadName());
    formattedMessage.append("] ");
    formattedMessage.append(" ");
    formattedMessage.append(rEvent.loggerName());
    formattedMessage.append(" ");
    formattedMessage.append(rEvent.message());
    return formattedMessage;
}
QDebug EventLogLayout::debug(QDebug &rDebug) const
{
    rDebug.nospace() << metaObject()->className() << "(" << "name:" << objectName() << ")";
    return rDebug.space();
}
}


PulsarStatus::PulsarStatus(QObject *parent)
    :QObject(parent)
{
}

void PulsarStatus::started()
{
    AFTNWorkerBase *t = new FileWorker(this);
    workers.append(t);
    t = new FFPLWorker(this);
    workers.append(t);
    t = new RPLWorker(this);
    workers.append(t);
    t = new TableCleaner(this);
    workers.append(t);
    t = new NetworkWorker(this);
    workers.append(t);
    connect(t, SIGNAL(aftnStatusChanged(bool)), this, SLOT(setAftnStatusChanged(bool)));

    foreach(AFTNWorkerBase * worker, workers)
        worker->start();
}

void PulsarStatus::stopped()
{
    foreach(AFTNWorkerBase * worker, workers)
    {
        qloginfo(tr("Stopping %1...").arg(worker->objectName()));
        if (worker->isRunning())
        {
            worker->quit();
        }
        qloginfo(tr("Stopped %1").arg(worker->objectName()));
    }

    //qDeleteAll(workers);
    workers.clear();
}

void PulsarStatus::setAftnStatusChanged(bool connected)
{
    QSettings *settings = new QtXmlSettings(QDir(QCoreApplication::applicationDirPath()).filePath(qApp->applicationName() + ".xml"));
    QString additionalStatus = QString("Database host: %1").arg(settings->value("db/host").toString());
    additionalStatus.append("\n");
    if (connected)
    {
        additionalStatus.append("AFTN channel: connected");
    } else
    {
        additionalStatus.append("AFTN channel: unconnected");
    }
    emit aftnStatusChanged(additionalStatus);
}

void AFTNService::init()
{
    if (objectName().isEmpty())
        setObjectName("aftn_service");
    setServiceDescription("AFTN Service");
    setServiceFlags(QtServiceBase::CanBeSuspended);
}

AFTNService::AFTNService(int argc, char** argv) :
    QtService<QCoreApplication> (argc, argv, "aftnservice")
{
    init();
}

AFTNService::~AFTNService()
{

}

void AFTNService::initLog4qt()
{
    using namespace Log4Qt;

    // DONT WORRY ABOUT DELETING THIS OBJECTS.
    // LOG4QT USES REFERENCE COUNTING
    // Create logger
    LogManager::rootLogger();
    // Hook qDebug(), etc.
    LogManager::setHandleQtMessages(true);

    // Format log message "date time [thread] logger level - message \newline"
    patternLayout = new PatternLayout("%d{locale:short} [%t] %p %c %x - %m%n", 0);
    patternLayout->activateOptions();

    colorConsoleLayout = new ColorConsoleLayout();
    colorConsoleLayout->setName("colorConsoleLayout");
    colorConsoleLayout->activateOptions();

    htmlLayout = new ColorHtmlLayout();
    htmlLayout->setName("htmlLayout");
    htmlLayout->activateOptions();

    // Create an appender to console with possible color formatiing
    colorConsoleAppender = new ColorConsoleAppender(colorConsoleLayout, ColorConsoleAppender::STDERR_TARGET);
    colorConsoleAppender->setName("colorConsoleAppender");
    colorConsoleAppender->activateOptions();

    syslogAppender = new SystemLogAppender();
    syslogAppender->setName("syslogAppender");
    syslogAppender->setServiceName(serviceName());
#if defined (Q_OS_WIN32)
    syslogAppender->setLayout(new EventLogLayout());
#else
    syslogAppender->setLayout(patternLayout);
#endif
    syslogAppender->activateOptions();

    telnetAppender = new TelnetAppender(htmlLayout, 10023);
    telnetAppender->setObjectName("telnetAppender");
    telnetAppender->setImmediateFlush(true);
    telnetAppender->setWelcomeMessage("AFTN client: unconnected");
    telnetAppender->activateOptions();

    fileAppender = new RollingFileAppender(patternLayout, QDir(QCoreApplication::applicationDirPath()).filePath(
                                               serviceName() + ".log"), true);
    fileAppender->setName("fileAppender");
    fileAppender->activateOptions();

    LogManager::rootLogger()->addAppender(fileAppender);
    LogManager::rootLogger()->addAppender(telnetAppender);
    LogManager::rootLogger()->addAppender(colorConsoleAppender);
    LogManager::rootLogger()->addAppender(syslogAppender);
}

void AFTNService::deinitLog4qt()
{
    using namespace Log4Qt;
    LogManager::rootLogger()->removeAllAppenders();
}

void AFTNService::start()
{
    qApp->setApplicationName("aftnservice");

    qloginfo(tr("Starting..."));

    loadDefaultSettings();
    settings = new QtXmlSettings(QDir(QCoreApplication::applicationDirPath()).filePath(qApp->applicationName() + ".xml"));

    // SET DEFAULT LOCALE FOR LOG OUTPUT
    QLocale::setDefault(QLocale::c());

    initLog4qt();

    // Execute pulsar
    // When plus started,
    QString additionalStatus = QString("Database host: %1").arg(settings->value("db/host").toString());
    additionalStatus.append("\n");
    additionalStatus.append("AFTN channel: unconnected");
    status = new PulsarStatus;
    pulsar = new Pulsar(false, AFTNPulsarDefaultPort, additionalStatus, 0);

    connect(status, SIGNAL(aftnStatusChanged(QString)), pulsar, SLOT(setAdditionalStatus(QString)));
    connect(pulsar, SIGNAL(broadcastStarted()), status, SLOT(started()));
    connect(pulsar, SIGNAL(broadcastStopped()), status, SLOT(stopped()));

    pulsar->start();

    qloginfo(tr("Started"));
}

void AFTNService::stop()
{
    qloginfo(tr("Stopping service..."));

    pulsar->exit();
    pulsar->wait();

    delete pulsar;
    delete status;

    delete settings;

    qloginfo(tr("Service stopped"));

    deinitLog4qt();
}

void AFTNService::processCommand(int code)
{
    qloginfo(tr("Process command: %1").arg(code));
}

void AFTNService::loadDefaultSettings()
{
    qloginfo(tr("Load default database settings"));
    if (!QFile::exists(QDir(qApp->applicationDirPath()).filePath(qApp->applicationName()+".xml")))
    {
        if (!QFile::copy(":/bin/config.xml", QDir(qApp->applicationDirPath()).filePath(qApp->applicationName()+".xml")))
            qlogwarn(tr("Could not create configuration xml file."));
    }
}
