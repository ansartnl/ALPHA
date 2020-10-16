#include <QtDebug>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

#include "aftnservice.h"
#include "main.h"
#include "qtxmlsettings.h"
#include "aftnmessageprocessor.h"
#include "fileworker.h"
#include "ffplworker.h"
#include "rplworker.h"
#include "tablecleaner.h"
#include "networkworker.h"
#include "oldiworker.h"
#include "mysqlconverter.h"
#include "tanmysql.h"
#include "moxastatuschanger.h"

#include "restarter.h"

#include "InitMain.h"
#include "CmdLineArguments.h"

#include "Pulsar.h"
#include "PulsarShareFunctions.h"

#include "pulsar_fdp_settings.h"

#include "version.h"

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
#include "mainthreadappender.h"

Pulsar *pulsar;

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
    QString formattedMessage = DateTime::fromMilliSeconds(rEvent.timeStamp()).toString("yyyy.MM.dd hh:mm:ss.zzz");
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
    :QObject(parent), restarter(0)
{
}

void PulsarStatus::setRestarterEnabled(bool on)
{
    if ( restarter )
    {
        restarter->exit();
        if ( !restarter->wait(30000) )
            restarter->terminate();
        restarter->deleteLater();
        restarter = 0;
    }

    if ( on )
        restarter = new Restarter(this);
}

void PulsarStatus::setRestartScriptPath(const QString &path)
{
    if ( restarter )
    {
        restarter->setScriptPath(path);
        qloginfo(tr("Restarting script: '%1'").arg(path));
    }
}

void PulsarStatus::setRestartInterval(int interval)
{
    if ( restarter )
        restarter->setInterval(interval);
}

void PulsarStatus::started()
{
    AFTNWorkerBase *t = new FileWorker(0);
    workers.append(t);
    t = new FFPLWorker(0);
    workers.append(t);
    t = new RPLWorker(0);
    workers.append(t);
    t = new TableCleaner(0);
    workers.append(t);
    t = new NetworkWorker(0);
    workers.append(t);
    connect(t, SIGNAL(aftnStatusChanged(bool)), this, SLOT(setAftnStatusChanged(bool)));
    t = new MoxaStatusChanger(0);
    workers.append(t);

#if defined(DUSHANBE_BUILD) || defined(COMPLETE_BUILD)
    //t = new MysqlConverter(this);
    t = new TANMySql(0);
    workers.append(t);
#endif

#if defined(TASHKENT_BUILD) || defined(COMPLETE_BUILD)
    t = new OldiWorker(0);
    workers.append(t);
#endif

    foreach(AFTNWorkerBase * worker, workers)
    {
        qloginfo(tr("Starting %1...").arg(worker->objectName()));
        worker->start();
        qloginfo(tr("Started %1").arg(worker->objectName()));
    }

    if ( restarter )
    {
        restarter->setWorkers(workers);
        restarter->start();
    }
}

void PulsarStatus::stopped()
{
    foreach(AFTNWorkerBase * worker, workers)
    {
        qloginfo(tr("Stopping %1...").arg(worker->objectName()));
//        worker->exit();
//        worker->wait();
//        worker->terminate();
        worker->stop();
        qloginfo(tr("Stopped %1").arg(worker->objectName()));
    }

    qDeleteAll(workers);
    workers.clear();

    if ( restarter )
        // Workaround to stop restarter.
        setRestarterEnabled(true);
}

void PulsarStatus::setAftnStatusChanged(bool connected)
{
    QSettings *settings = new QtXmlSettings(xmlPath());
    QString additionalStatus = QString("Database host: %1").arg(settings->value("db/host").toString());
    additionalStatus.append("\n");
    if (connected)
        additionalStatus.append("AFTN channel: connected");
    else
        additionalStatus.append("AFTN channel: unconnected");
    emit aftnStatusChanged(additionalStatus);
}

void AFTNService::init()
{
    if (objectName().isEmpty())
        setObjectName("fdp_service");
    setServiceDescription("FDP Service");
    setServiceFlags(QtServiceBase::CanBeSuspended);
}

AFTNService::AFTNService(int argc, char** argv) :
    QtService<QCoreApplication> (argc, argv, "fdpservice")
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
    patternLayout = new PatternLayout("%d{yyyy.MM.dd hh:mm:ss.zzz} [%t] %p %c %x - %m%n", 0);
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

    MainThreadAppender *mainThreadAppender = new MainThreadAppender();
    mainThreadAppender->setName("mainThreadAppender");
    mainThreadAppender->addAppender(telnetAppender);

    telnetAppender->activateOptions();

    QString sLogFile = CmdLineArguments::instance(QCoreApplication::arguments()).log();
    if(sLogFile.isEmpty())
        sLogFile = settings->value("log/path").toString();
    if(sLogFile.isEmpty())
        sLogFile = QDir(QCoreApplication::applicationDirPath()).filePath(QCoreApplication::applicationName() + ".log");
    fileAppender = new RollingFileAppender(patternLayout, sLogFile, true);
    fileAppender->setName("fileAppender");
    fileAppender->setMaximumFileSize(settings->value("log/file_size", "1048576").toUInt());
    fileAppender->setMaxBackupIndex(settings->value("log/max_backup_index", "10").toInt());
    fileAppender->activateOptions();

    LogManager::rootLogger()->addAppender(fileAppender);
    LogManager::rootLogger()->addAppender(mainThreadAppender);
    LogManager::rootLogger()->addAppender(colorConsoleAppender);
    LogManager::rootLogger()->addAppender(syslogAppender);

    LogManager::rootLogger()->setLevel(Level::fromString(settings->value("log/level", "ALL").toString()));
}

void AFTNService::deinitLog4qt()
{
    using namespace Log4Qt;
    LogManager::rootLogger()->removeAllAppenders();
}

void AFTNService::start()
{
    qApp->setApplicationName("fdpservice");
    //qApp->setApplicationName("fdpservice_debug_BAN");
    qApp->setApplicationVersion(VERSION_FULL);
    qApp->setOrganizationName("Peleng Ltd.");
    qApp->setOrganizationDomain("peleng.ru");

    InitMain::setupSearchPath();

    qDebug() << tr("Starting...");

    loadDefaultSettings();
    settings = new QtXmlSettings(xmlPath());

    // SET DEFAULT LOCALE FOR LOG OUTPUT
    QLocale::setDefault(QLocale::c());

    initLog4qt();

    qDebug() << " =========> NEW_SESSION_STARTED <========= ";
    qDebug() << QString(" %1, Version %2 ").arg(qApp->applicationName(), qApp->applicationVersion());
    qDebug() << QString(" Build date: %1 ").arg(__DATE__);
    qWarning() << QString(" Configuration file: '%1' ").arg(xmlPath()) << "\n";

    // Execute pulsar
    // When plus started,
    QString additionalStatus = QString("Database host: %1").arg(settings->value("db/host").toString());
    additionalStatus.append("\n");
    additionalStatus.append("AFTN channel: unconnected");

    status = new PulsarStatus;
    pulsar = new Pulsar(false, AFTNPulsarDefaultPort, additionalStatus, 0);
    //pulsar = new Pulsar(false, 12007, additionalStatus, 0);
    QString netMask = settings->value("pulsar/netmask").toString();
    if(!netMask.isEmpty())
    {
        pulsar->setMask(netMask);
    }
    QString netBroadcast = settings->value("pulsar/broadcast").toString();
    if(!netBroadcast.isEmpty())
    {
        pulsar->setBroadcast(netBroadcast.split(","));
    }
    QString netServerAddress = settings->value("pulsar/server_address").toString();
    if(!netServerAddress.isEmpty())
    {
        pulsar->setServerAddress(netServerAddress);
    }

    status->setRestarterEnabled( !settings->value("restarter/disable", true).toBool() );
    status->setRestartScriptPath( settings->value("restarter/script_path").toString() );
    status->setRestartInterval( settings->value("restarter/interval", 60).toInt() );

    connect(status, SIGNAL(aftnStatusChanged(QString)), pulsar, SLOT(setAdditionalStatus(QString)));

    connect(pulsar, SIGNAL(broadcastStarted()), status, SLOT(started()));
    connect(pulsar, SIGNAL(broadcastStopped(QHostAddress,QString,QVariantHash)), status, SLOT(stopped()));

    pulsar->addAdditionalInfo(PulsarSettings::fdpName, settings->value("oldi/owner_name", "FDP"));
    pulsar->start();

    qloginfo(tr("Started"));
}

void AFTNService::stop()
{
    qloginfo(tr("Stopping service..."));

    status->stopped();
    pulsar->stop();
//    pulsar->exit();
//    pulsar->wait();

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
    QString pathConfig = InitMain::findConfigFilePath();
    if (pathConfig.isEmpty())
        pathConfig = QDir(qApp->applicationDirPath()).filePath(qApp->applicationName() + ".xml");

    if (!QFile::exists(pathConfig))
    {
        if (!QFile::copy(":/bin/config.xml", pathConfig))
            qlogwarn(tr("Could not create configuration xml file."));
        else
        {
            QFile::Permissions perm = QFile::permissions(pathConfig);
            QFile::setPermissions(pathConfig, perm | QFile::ReadOther | QFile::WriteOther | QFile::ReadOwner | QFile::WriteOwner);
        }
    }
    xmlPath(pathConfig);
}
