#include "main.h"
#include "XMasterWindow.h"
#include "qttranslator.h"
#include "ProfileSettings.h"
#include "MouseLocker.h"

#include "Dao.h"

#include "qtxmlsettings.h"

#include "asterix.h"

#include "version.h"
#include "lic.h"

// ---------------
// LOG4QT
#include "logmanager.h"

#include "patternlayout.h"
#include "rollingfileappender.h"
#include "consoleappender.h"
// ---------------

#include "InitMain.h"

#include <QtDebug>

#include <QtCore/QDir>
#include <QtCore/QTextCodec>
#include <QLibrary>

#include <QApplication>
#include <QMessageBox>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>

#include "aftnmessageprocessor.h"
#include "CmdLineArguments.h"

#include "dbconnectionsmanager.h"
#include "databasesubscriber.h"

#include "HardwareProvider.h"
#include "License.h"

AFTNMessageProcessor* aftnMessageProcessor = 0;

ExtendedXmlSettings *networkSettings(const QString & pathFile)
{
    static ExtendedXmlSettings *gNetworkSettings = new ExtendedXmlSettings(pathFile, qApp);
    return gNetworkSettings;
}

QSettings* globalSettings(const QString & pathFile)
{
    static QSettings *gStg = new QSettings(pathFile, QSettings::IniFormat, qApp);
    return gStg;
}

Dao* dao()
{
    static Dao* dao = new Dao();
    return dao;
}

CMaster* master(CMaster* pMaster/* = 0*/)
{
    static CMaster* master = 0;
    if(pMaster)
        master = pMaster;
    return master;
}

QWidget* getMainWindow()
{
    QWidgetList	widgets = QApplication::topLevelWidgets();
    for ( QWidgetList::iterator it = widgets.begin(); it != widgets.end(); ++it )
    {
        if ( qobject_cast<QMainWindow *>( *it ) )
            return *it;
            //return qobject_cast<QMainWindow *>( *it );
    }
    return 0;
}

void initLog4Qt()
{
    // INITIALISATION OF LOG4QT
    using namespace Log4Qt;
    // DONT WORRY ABOUT DELETING THIS OBJECTS.
    // LOG4QT USES REFERENCE COUNTING
    // Create logger
    LogManager::rootLogger();
    // Hook qDebug(), etc.
    LogManager::setHandleQtMessages(true);

    // Format log message "date time [thread] logger level - message \newline"
    PatternLayout *patternLayout = new PatternLayout("%d [%t] %p %c %x - %m%n", 0);
    patternLayout->setName("patternLayout");
    patternLayout->activateOptions();

    QString sLogFile = CmdLineArguments::instance(QCoreApplication::arguments()).log();
    if(sLogFile.isEmpty())
        sLogFile = networkSettings()->value("log/path").toString();
    if(sLogFile.isEmpty())
        sLogFile = QDir(QCoreApplication::applicationDirPath()).filePath(QCoreApplication::applicationName() + ".log");
    RollingFileAppender *fileAppender = new RollingFileAppender(patternLayout, sLogFile, true);
    fileAppender->setName("fileAppender");
    fileAppender->setMaximumFileSize(
                networkSettings()->value("log/file_size").toUInt() );
    fileAppender->setMaxBackupIndex(
                networkSettings()->value("log/max_backup_index").toInt() ); // Maximum 'N' log files
    fileAppender->activateOptions();

    ConsoleAppender *consoleAppender = new ConsoleAppender(patternLayout, ConsoleAppender::STDERR_TARGET);
    consoleAppender->activateOptions();

    LogManager::rootLogger()->addAppender(fileAppender);
    LogManager::rootLogger()->addAppender(consoleAppender);

    Level logLevel = Level::fromString(networkSettings()->value("log/level", "ALL").toString());
    LogManager::rootLogger()->setLevel(logLevel);

    // init operations log
    {
        QString filePath = networkSettings()->value("log/operations_log_path", "operations.log").toString();
        if (!filePath.endsWith(".log"))
            filePath.append("operations.log");
        operationsLog(filePath
                      , networkSettings()->value("log/file_size").toUInt()
                      , networkSettings()->value("log/max_backup_index").toInt()
                      , networkSettings()->value("oldi/name").toString());
    }
}

int main(int argc, char* argv[])
{
    //! Qt customization
    //! UTF8 codecs for all possible strings in application
    InitMain::setupCodec();

    // DON'T WRITE ANY CODE BELOW THIS LINE UNTIL COMMENT
    // ------------------------------------------------------------
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("atm");
    QCoreApplication::setOrganizationDomain("itos.eu.org");
    QCoreApplication::setApplicationName("cwp");
    QCoreApplication::setApplicationVersion(QString("%1").arg(VERSION_FULL));
    InitMain::setupSearchPath();

    if (!CheckLicense(app))
    {
        QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("Invalid license file."));
        return 0;
    }

    // Init QSettings static accessor.
    globalSettings(QApplication::applicationDirPath() + "/" +
                   QApplication::applicationName() + ".settings");


    if (pathConfig.isEmpty())
    {
        QMessageBox::critical(0, QObject::tr("Error configuration"),
                              QObject::tr("Configuration file is not found.\r\n\r\n") +
                              InitMain::configUsageString());
        return 0;
    }
    // Init QtXmlSettings static accessor.
    networkSettings(pathConfig);

    QtXmlSettings *alternative = 0;
    QStringList args = QApplication::arguments();
    if(args.contains("-a -f") && args.indexOf("-a") + 1 < args.size())
    {
        QString additionalConfigPath = args.at(args.indexOf("-a") + 1);
        alternative = new QtXmlSettings(additionalConfigPath, qApp);
        networkSettings()->setAlternativeConfig(alternative);
    }

    QString userPath = pathConfig;
    if (userPath.endsWith(".xml")) {
        userPath.chop(4);
    }
    userPath.append("-user.xml");
    networkSettings()->setVolatileConfig(new QtXmlSettings(userPath, qApp));

    Asterix::registre();

    QtTranslator::instance()->loadTranslations(QDir("translations:"));
    QtTranslator::instance()->loadLanguageFromSettings();

    initLog4Qt();

    const int lockMouse = globalSettings()->value("XMasterWindow/Settings/LockMouse", 1).toBool();
    globalSettings()->setValue("XMasterWindow/Settings/LockMouse", lockMouse);
    mouseLocker(qApp);

    qDebug() << " =========> NEW_SESSION_STARTED <========= ";
    qDebug() << QString(" %1, Version %2 ").arg(qApp->applicationName(), qApp->applicationVersion());
    qDebug() << QString(" Build date: %1 %2").arg(__DATE__);
    qWarning() << QString(" Configuration file: '%1' ").arg(pathConfig) << "\n";

    bool registered = false;
    {
        HardwareId currentHardwareId = HardwareProvider().getHardwareId();
        currentHardwareId.print();

        QFile hardwareIdFile(app.applicationDirPath() + "/key.hwid");
        hardwareIdFile.open(QIODevice::ReadOnly);
        HardwareId hardwareId(hardwareIdFile.readAll());
        hardwareIdFile.close();

        QFile licKeyFile(app.applicationDirPath() + "/key.lic");
        licKeyFile.open(QIODevice::ReadOnly);

        bool ok = License::verifyLicense(hardwareId.toRegKey(),
                                         licKeyFile.readAll(),
                                         SECRET_KEY);

        if (ok && hardwareId.verify(currentHardwareId))
        {
            registered = true;
        }
        else
        {
            if (currentHardwareId.isValid())
            {
                hardwareIdFile.open(QIODevice::WriteOnly);
                
            }
        }
    }

    int result(0);
    try
    {
        try
        {
            // Create singleton class of language window
//#ifdef QT_DEBUG
            DBConnectionsManager &dbAutoconnector = DBConnectionsManager::instance();

            dbAutoconnector.setDbAlias("checkConnection");
            dbAutoconnector.setInterval(networkSettings()->value("databases/*timeout", 5).toInt() * 1000);

            networkSettings()->beginGroup("databases");
            foreach(const QString& group, networkSettings()->childGroups())
            {
                networkSettings()->beginGroup(group);

                DbParams params;
                params.driver = networkSettings()->value("driver").toString();

                if(params.driver == "QOCI")
                {
                    // fix Oracle bug for heap error
                    QLibrary lib("oci");
                }

                params.host = networkSettings()->value("host").toString();
                params.port = networkSettings()->value("port").toInt();
                params.databaseName = networkSettings()->value("name").toString();
                params.user = networkSettings()->value("user").toString();
                params.password = networkSettings()->value("password").toString();

                dbAutoconnector.addDbParams(params);

                networkSettings()->endGroup();
            }
            networkSettings()->endGroup();

            XMasterWindow* mainWindow = new XMasterWindow(registered);

            // Redis
            mainWindow->InitRedis();
            QObject::connect(mainWindow, SIGNAL(redisConnected()), mainWindow, SLOT(_OnSetConnectRedisTrue()));

            QMessageBox *mb = new QMessageBox(QMessageBox::Information, qApp->applicationName()
                                              , QObject::tr("Connecting to database..."), QMessageBox::Cancel);
            mb->setAttribute(Qt::WA_DeleteOnClose);
            if (lockMouse)
                mouseLocker()->lockInside(mb);

            QObject::connect(mainWindow, SIGNAL(closeDialog()), mb, SLOT(reject()));
            QObject::connect(&dbAutoconnector, SIGNAL(connected()), mainWindow, SLOT(_OnSetConnectPostgresTrue()));
            QObject::connect(&dbAutoconnector, SIGNAL(connected()), mainWindow, SLOT(InitAFTNMessageProcessor()));
            QObject::connect(&dbAutoconnector, SIGNAL(connected(DbParams)), mainWindow, SLOT(_OnDBConnected(DbParams)));
            QObject::connect(&dbAutoconnector, SIGNAL(connectFailed(DbParams)), mainWindow, SLOT(_OnDBConnectAttemptFailed(DbParams)));
            QObject::connect(&dbAutoconnector, SIGNAL(disconnected()), mainWindow, SLOT(_OnDBDisconnected()));
            QObject::connect(&dbAutoconnector, SIGNAL(connected()), &DatabaseSubscriber::instance(), SLOT(onDatabaseConnected()));

            dbAutoconnector.start();

            if (mb->exec() == QDialog::Rejected)
            {
                if (mainWindow->InitWin(lockMouse))
                {
                    if (lockMouse)
                        mouseLocker()->lockInside(mainWindow);
                    result = app.exec();
                    UserProfile::CSettings::Instance().Save();
                }
            }

        } catch (const Error&)
        {
            throw;
        } catch (...)
        {
            throw Error(__FILE__, __LINE__, "catch(...)", E_FAIL);
        }
    } catch (Error& err)
    {
        QMessageBox::critical(NULL, QObject::tr("Error"), err.reason());
    }

    // Write settings on disc.
    delete networkSettings();
    delete globalSettings();

    if(alternative)
        delete alternative;

    if (aftnMessageProcessor)
        delete aftnMessageProcessor;

    Log4Qt::LogManager::rootLogger()->removeAllAppenders();

    DBConnectionsManager::instance().quit();
    return result;
}

bool CheckLicense(const QApplication& app)
{
    return true;
    //return QDateTime::currentDateTime() < QDateTime(QDate(2015, 1, 1), QTime());
//    bool bRes = lic::check_license(app.applicationDirPath() + ("/key.lic"));
//    for(int i = 1; !bRes && i < 6; ++i)
//    {
//        QString sFile = app.applicationDirPath() + QString("/key%1.lic").arg(i);
//        bRes = lic::check_license(sFile);
//    }
//    return bRes;
}
