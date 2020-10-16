#include <QtDebug>

#include <QtCore/QThread>
#include <QtCore/QCryptographicHash>
#include <QtCore/QTimer>
#include <QtCore/QDir>

#include <QtGui/QApplication>
#include <QtGui/QMessageBox>

#include <QLibrary>

#include <QtNetwork/QHostAddress>
#include <QtNetwork/QTcpSocket>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlIndex>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>

#include "logindialog.h"
#include "mainwindow.h"
#include "qtxmlsettings.h"
#include "aftnmessageprocessor.h"
#include "translations.h"

#include "version.h"

// ---------------
// LOG4QT
#include "logmanager.h"
#include "patternlayout.h"
#include "rollingfileappender.h"
#include "consoleappender.h"
#include "telnetappender.h"
#include "colorhtmllayout.h"
// ---------------

#include "InitMain.h"

#include "qttranslator.h"

#include "Pulsar.h"
#include "PulsReceiver.h"

#include "main.h"

#include "objectsmanager.h"
#include "aftnservicelog.h"

#include "CmdLineArguments.h"

struct Sleep: public QThread
{
    using QThread::msleep;
    using QThread::sleep;
};

PulsarStatus::PulsarStatus(QObject* parent)
    :QObject(parent), telnet(new QTcpSocket(this)), alreadyLogged(false), mainWindow(0)
    , initTimer(new QTimer(this))
{
    connect(initTimer, SIGNAL(timeout()), this, SLOT(initConnection()));
    initTimer->setSingleShot(true);
    initTimer->start(10000);

    connect(telnet, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(telnetStateChanged(QAbstractSocket::SocketState)));
    connect(telnet, SIGNAL(readyRead()),
            this, SLOT(telnetReadyRead()));
}

void PulsarStatus::disconnectFromOld(const QHostAddress &pulsar)
{
    QSqlDatabase database = QSqlDatabase::database(QSqlDatabase::defaultConnection, false);
    lastDbHost = database.hostName();
    if (database.isOpen())
    {
        setLogin_out(currentUserId, currentUser, 0);
        database.close();
    }

    if (telnet->state() != QTcpSocket::UnconnectedState)
        telnet->disconnectFromHost();
}

void PulsarStatus::connectToNew(const QHostAddress &pulsar, const QString& status)
{
    qDebug() << "AFTN service changed";
    qDebug() << "New AFTN service" << pulsar.toString();
    initTimer->stop();

    QHostAddress dbaddr = pulsar;
    QRegExp rexp("Database host: (\\d{,3}.\\d{,3}.\\d{,3}.\\d{,3})");
    if (rexp.indexIn(status) != -1) {
        dbaddr = QHostAddress(rexp.cap(1));
    }
    telnet->connectToHost(pulsar, 10023);

    QSqlDatabase database = QSqlDatabase::database(QSqlDatabase::defaultConnection, false);
    if (dbaddr == QHostAddress::LocalHost)
        dbaddr = pulsar;
    database.setHostName(dbaddr.toString());

    if (database.open())
    {
        if (mainWindow) {
            mainWindow->saveSettings();
            login(currentUser, currentPassword);
            ObjectsManager::instance()->refresh();
            mainWindow->restoreSettings();
        } else {
            LoginDialog* loginDialog = getLoginDialog(0);
            if (loginDialog->allFilled)
            {
                if (loginDialog->tryToLogin())
                {
                    ObjectsManager::instance()->refresh();
                    mainWindow = new MainWindow;
                    mainWindow->show();
                    return;
                }
            }
            int iRes = loginDialog->exec();
            if (iRes != QDialog::Accepted)
            {
                if(iRes != 555)
                {
                    Log4Qt::LogManager::rootLogger()->removeAllAppenders();
                    qApp->exit();
                }
            }
            else
            {
                ObjectsManager::instance()->refresh();
                mainWindow = new MainWindow;
                mainWindow->show();
            }
        }
    } else {
        database.setHostName(lastDbHost);
        database.open();
        qDebug() << "Could not open database" << dbaddr.toString();
    }
}


void PulsarStatus::pulsarChanged(const QHostAddress &newpulsar, const QString &status, const QHostAddress &oldpulsar)
{
    if (!oldpulsar.isNull())
        disconnectFromOld(oldpulsar);

    if (!newpulsar.isNull())
        connectToNew(newpulsar, status);
    else
        qDebug() << "Connection with aftnservice losted";
}

void PulsarStatus::statusChanged(const QHostAddress &pulsar, const QString &status)
{
    if (status.contains("Status: active")) {
        QAction *action = ObjectsManager::instance()->action(AFTNServiceLog::staticMetaObject.className());
        if (status.contains("AFTN channel: unconnected")) {
            if (action)
            {
                action->setProperty("color", 0xFF0000);
                action->setText(tr("AFTN Service: unconnected"));
            }
            if(mainWindow)
            {
                mainWindow->statusAFTNChanged(false);
            }
        } else if (status.contains("AFTN channel: connected"))
        {
            if (action)
            {
                action->setProperty("color", 0x10AA10);
                action->setText(tr("AFTN Service: connected"));
            }
            if(mainWindow)
            {
                mainWindow->statusAFTNChanged(true);
            }
        }
    }
}

void PulsarStatus::initConnection()
{
    LoginDialog* loginDialog = getLoginDialog(0);
    if (loginDialog->allFilled)
    {
        if (loginDialog->tryToLogin())
        {
            mainWindow = new MainWindow;
            mainWindow->show();
            return;
        }
    }
    int iRes = loginDialog->exec();
    if ( iRes != QDialog::Accepted)
    {
        if(iRes != 555)
        {
            Log4Qt::LogManager::rootLogger()->removeAllAppenders();
            qApp->exit();
        }
    }
    else
    {
        mainWindow = new MainWindow;
        mainWindow->show();
    }
}


void PulsarStatus::telnetStateChanged(
        QAbstractSocket::SocketState socketState)
{
    switch (socketState)
    {
    case QAbstractSocket::UnconnectedState:
        Log4Qt::Logger::logger("aftnservice")->warn("Telnet logger unconnected");
        break;
    case QAbstractSocket::ConnectedState:
        Log4Qt::Logger::logger("aftnservice")->warn("Telnet logger connected");
        break;
    default:
        return;
    }
}

void PulsarStatus::telnetReadyRead()
{
    QByteArray msg;
    msg = telnet->readAll();

    Log4Qt::Logger::logger("aftnservice")->info(QString::fromUtf8(msg));
}

// #####################
// global variables
// #####################

QSettings* networkSettings(const QString &pathFile)
{
    static QSettings *mNetworkSettings = new QtXmlSettings(pathFile, qApp);
    return mNetworkSettings;
}

QSettings* globalSettings()
{
    //! Load settings for application
    static QSettings *mGlobalSettings = new QSettings(qApp);
    return mGlobalSettings;
}

static void initDefaultNetworkSettings()
{
    QString sFile = QDir("etc:").filePath(qApp->applicationName() + ".xml");
    if (!QFile::exists(sFile))
    {
        if (!QFile::copy(":/etc/config.xml", sFile))
            qWarning() << QObject::tr("Could not create default configuration xml file.");
        else
        {
            QFile::Permissions perm = QFile::permissions(sFile);
            QFile::setPermissions(sFile, perm | QFile::ReadOther | QFile::WriteOther | QFile::ReadOwner | QFile::WriteOwner);
        }
    }
}

QString currentUser;
QString currentUserFullName;
int currentUserId = -1;
Permissions currentPermissions;
QString currentPassword;

AFTNMessageProcessor* aftnMessageProcessor = 0;


//! Entry point
int main(int argc, char* argv[])
{
    QThread::currentThread()->setObjectName("main");
    //! initialization of qttoolbardialog resources (icons, pixmaps)
    Q_INIT_RESOURCE(qttoolbardialog);

    //! Qt customization
    //! UTF8 codecs for all possible strings in application
    InitMain::setupCodec("UTF-8");

    QApplication application(argc, argv);

    // DON'T CHANGE. USED BY QSetting
    QCoreApplication::setApplicationName("aftnws");
    QCoreApplication::setApplicationVersion(QString("%1").arg(VERSION_FULL));
    QCoreApplication::setOrganizationName("Peleng Ltd.");
    QCoreApplication::setOrganizationDomain("peleng.ru");

    InitMain::setupSearchPath();

    QString pathConfig = InitMain::findConfigFilePath();
    if ( pathConfig.isEmpty() )
    {
        // Create "etc/config.xml" for network settings
        initDefaultNetworkSettings();
        pathConfig = InitMain::findConfigFilePath();
        if ( pathConfig.isEmpty() )
        {
            QMessageBox::critical(0, QObject::tr("Error configuration"),
                                  QObject::tr("Configuration file is not found.\r\n\r\n") +
                                  InitMain::configUsageString());
            return 0;
        }
    }
    // Init QtXmlSettings static accessor.
    networkSettings(pathConfig);

    // -----------------------------------------------------
    // INITIALISATION OF LOG4QT
    using namespace Log4Qt;
    // DONT WORRY ABOUT DELETING THIS OBJECTS.
    // LOG4QT USES REFERENCE COUNTING
    // Create logger
    LogManager::rootLogger();
    // Hook qDebug(), etc.
    LogManager::setHandleQtMessages(true);

    // Format log message "date time [thread] logger level - message \newline"
    PatternLayout *patternLayout = new PatternLayout("%d{locale:short} [%t] %p %c %x - %m%n", 0);
    patternLayout->setName("patternLayout");
    patternLayout->activateOptions();

    ColorHtmlLayout *htmlLayout = new ColorHtmlLayout();
    htmlLayout->setName("htmlLayout");
    htmlLayout->activateOptions();

    ConsoleAppender *consoleAppender = new ConsoleAppender(patternLayout, 0);
    consoleAppender->activateOptions();

    QString sLogFile = CmdLineArguments::instance(QCoreApplication::arguments()).log();
    if(sLogFile.isEmpty())
        sLogFile = networkSettings()->value("log/path").toString();
    if(sLogFile.isEmpty())
        sLogFile = QDir(QCoreApplication::applicationDirPath()).filePath(QCoreApplication::applicationName() + ".log");
    RollingFileAppender *fileAppender = new RollingFileAppender(patternLayout, sLogFile, true);
    fileAppender->setName("fileAppender");
    fileAppender->setMaximumFileSize(networkSettings()->value("log/file_size", "1048576").toUInt());
    fileAppender->setMaxBackupIndex(networkSettings()->value("log/max_backup_index", "10").toInt());
    fileAppender->activateOptions();

    TelnetAppender *telnetAppender = new TelnetAppender(htmlLayout, 10024);
    telnetAppender->setObjectName("telnetAppender");
    telnetAppender->setImmediateFlush(true);
    telnetAppender->activateOptions();

    LogManager::rootLogger()->addAppender(fileAppender);
    LogManager::rootLogger()->addAppender(consoleAppender);
    LogManager::rootLogger()->addAppender(telnetAppender);

    Level logLevel = Level::fromString(networkSettings()->value("log/level", "ALL").toString());
    LogManager::rootLogger()->setLevel(logLevel);

    LogManager::logger("aftnservice");
    // -----------------------------------------------------

    qDebug() << " =========> NEW_SESSION_STARTED <========= ";
    qDebug() << QString(" %1, Version %2 ").arg(qApp->applicationName(), qApp->applicationVersion());
    qDebug() << QString(" Build date: %1 ").arg(__DATE__);
    qWarning() << QString(" Configuration file: '%1' ").arg(pathConfig) << "\n";

    QApplication::setWindowIcon(QIcon("images:32x32/application/airplane.png"));


    QtTranslator::instance()->loadTranslations(QDir("translations:"));
    QtTranslator::instance()->loadLanguageFromSettings();
    Translations::initialize();

    //! Create singleton qt sql connection
    networkSettings()->beginGroup("db");
    if(networkSettings()->value("driver").toString() == "QOCI")
    {
        // fix Oracle bug for heap error
        QLibrary lib("oci");
        //lib.load();
    }
    QSqlDatabase database = QSqlDatabase::addDatabase(networkSettings()->value("driver").toString());
    //database.setHostName(networkSettings()->value("host").toString());
    database.setPort(networkSettings()->value("port").toInt());
    database.setDatabaseName(networkSettings()->value("name").toString());
    database.setUserName(networkSettings()->value("user").toString());
    database.setPassword(networkSettings()->value("password").toString());
    networkSettings()->endGroup();

    PulsarStatus *pulsarStatus = new PulsarStatus();
    PulsReceiver *pulsReceiver = new PulsReceiver(AFTNPulsarDefaultPort, qApp);
    QObject::connect(pulsReceiver, SIGNAL(pulsarChanged(QHostAddress,QString,QHostAddress,QVariantHash)), pulsarStatus, SLOT(pulsarChanged(QHostAddress,QString,QHostAddress)));
    QObject::connect(pulsReceiver, SIGNAL(status(QHostAddress,QString,QVariantHash)), pulsarStatus, SLOT(statusChanged(QHostAddress,QString)));

    pulsReceiver->start();

    bool ok = application.exec();

    setLogin_out(currentUserId, currentUser, 0);

    LogManager::rootLogger()->removeAllAppenders();
    return ok;
}

QString passwordHash(QString password)
{
    password.append("SOMESALTFROMFILONENKO\xAB\xCD\xEF");
    return QCryptographicHash::hash(password.toUtf8().data(), QCryptographicHash::Md5).toHex();
}

QString login(const QString& username, const QString& password)
{
    static bool firstCall = true;
    static Pulsar *pulsar = new Pulsar(true, AFTNPulsarDefaultPort2, QString(), qApp);

    if (!firstCall) {
//        pulsar->exit();
//        pulsar->wait();
        pulsar->stop();
    } else {
        firstCall = false;
    }

    QString oldUserName = currentUser;
    int oldUserId = currentUserId;

    QString hash = passwordHash(password);
    QSqlQuery query;
    query.prepare("select id, user_name, fullname from users "
                  "  where user_name = ? "
                  "   and password = ? "
                  "   and isactive != 0 "
                  "   and id != 0");

    query.addBindValue(username);
    query.addBindValue(hash);

    if (!query.exec())
    {
        qWarning() << Q_FUNC_INFO;
        qWarning() << query.lastError().text();
        return QObject::tr("SQL error: %1").arg(query.lastError().text());
    }

    // user or password invalid
    if (!query.next())
    {
        return QObject::tr("User name or password is invalid.");
    }

    currentUser = username;
    currentUserId = query.record().value("id").toInt();
    currentUserFullName = query.record().value("fullname").toString();
    currentPassword = password;

    // Supperuser
    query.prepare("select * from user_rights where user_id = ?");
    query.addBindValue(currentUserId);

    if (!query.exec())
    {
        qWarning() << Q_FUNC_INFO;
        qWarning() << query.lastError().text();
        return QObject::tr("SQL error: %1").arg(query.lastError().text());
    }

    currentPermissions.clear();
    while (query.next())
    {
        currentPermissions.insert(query.record().value("object_id").toString(), query.record().value("rights").toInt());
    }

    query.prepare("delete from login");

    if (!query.exec())
    {
        qWarning() << Q_FUNC_INFO;
        qWarning() << query.lastError().text();
        return QObject::tr("SQL error: %1").arg(query.lastError().text());
    }

    query.prepare("insert into login(user_id, user_name) values (?, ?)");
    query.addBindValue(currentUserId);
    query.addBindValue(currentUser);

    if (!query.exec())
    {
        qWarning() << Q_FUNC_INFO;
        qWarning() << query.lastError().text();
        return QObject::tr("SQL error: %1").arg(query.lastError().text());
    }

    if (!oldUserName.isEmpty())
    {
        QString sErr = setLogin_out(oldUserId, oldUserName, 0);
        if (!sErr.isEmpty())
        {
            qWarning() << Q_FUNC_INFO;
            qWarning() << sErr;
            return QObject::tr("SQL error: %1").arg(sErr);
        }
    }

    {
        QString sErr = setLogin_out(currentUserId, currentUser, 1);
        if (!sErr.isEmpty())
        {
            qWarning() << Q_FUNC_INFO;
            qWarning() << sErr;
            return QObject::tr("SQL error: %1").arg(sErr);
        }
    }

    // -----------------------------
    //! Load AFTNMessageProcessor
    if (aftnMessageProcessor)
        delete aftnMessageProcessor;
    aftnMessageProcessor = new AFTNMessageProcessor(QSqlDatabase::database(), qApp);

    networkSettings()->beginGroup("aftn_message_processor");
    aftnMessageProcessor->setFIRName(networkSettings()->value("fir_name").toString());
    aftnMessageProcessor->setSaveOnlyFIR(networkSettings()->value("cut_fir").toBool());
    aftnMessageProcessor->setValidateRoute(networkSettings()->value("validate_route").toBool());
    aftnMessageProcessor->setValidateRouteLite(networkSettings()->value("validate_route_lite", false).toBool());
    aftnMessageProcessor->setStandard(networkSettings()->value("FPL2012", false).toBool() ? AFTNMessageProcessor::FPL_2012 : AFTNMessageProcessor::FPL_2007);
    networkSettings()->endGroup();

    aftnMessageProcessor->setDatabase(QSqlDatabase::database());

    if (!oldUserName.isEmpty())
        if (globalSettings()->group() == oldUserName)
            globalSettings()->endGroup();
    globalSettings()->beginGroup(username);


    pulsar->setAdditionalStatus("Logged user: " + currentUserFullName);

    pulsar->start();

    return QString::null;
}

LoginDialog* getLoginDialog(QWidget* parent)
{
    static LoginDialog* dlg = 0;
    if(dlg)
    {
        dlg->done(555);
    }
    dlg = new LoginDialog(parent);
    // ? dlg->setAttribute(Qt::WA_DeleteOnClose);
    return dlg;
}

QString setLogin_out(int userid, const QString &username, int logintype)
{
#if defined(BAN_BUILD)
    if(!username.isEmpty())
    {
        QSqlQuery query;
        query.prepare("insert into login_log(user_id, user_name, logged_type) values (?, ?, ?)");
        query.addBindValue(userid);
        query.addBindValue(username);
        query.addBindValue(logintype);

        if (!query.exec())
        {
            return " " + query.lastError().text();
        }
    }
#endif
    return QString::null;
}
