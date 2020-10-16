#include "main.h"

#include "arincmergemainwindow.h"

#include "CmdLineArguments.h"
#include "InitMain.h"
#include "qtxmlsettings.h"
#include "qttranslator.h"
#include "version.h"

#include <QtCore/QDateTime>
#include <QtCore/QDir>
#include <QtCore/QFile>
#ifdef QT_DEBUG
#   include <QtCore/QLibrary>
#endif

#include <QApplication>
#include <QBitmap>

#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

QFile *logFile = 0;
static QtXmlSettings *mSettings = 0;

QSettings * settings()
{
    return mSettings;
}

static void closeLog()
{
    qDebug() << "=========> END_SESSION <=========";
    if (!logFile)
        return;
    logFile->flush();
    logFile->close();
    logFile = 0;
}

void logOutput(QtMsgType type, const char *msg)
{
    QString message = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss,zzz ");
    switch (type) {
    case QtDebugMsg:
        message.push_back("[INFO] ");
        break;
    case QtWarningMsg:
        message.push_back("[WARNING] ");
        break;
    case QtCriticalMsg:
        message.push_back("[CRITICAL] ");
        break;
    case QtFatalMsg:
        message.push_back("[FATAL] ");
        break;
    }
    message.push_back(msg);
    if (logFile)
    {
        if (logFile->size() > 1024 * 1024)
            logFile->resize(0);
        QTextStream(logFile) << message << endl;
    }
    QTextStream(stderr) << message << endl;
    if (QtFatalMsg == type)
    {
        closeLog();
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    InitMain::setupCodec();

    QCoreApplication::setApplicationName("environmenteditor");
    QCoreApplication::setApplicationVersion(VERSION_FULL);
    QCoreApplication::setOrganizationName("ITOS");

    QApplication app(argc, argv);
    app.setApplicationVersion(VERSION_FULL);

    InitMain::setupSearchPath();

    QPixmap pixMap("images:32x32/merge1.png");
    pixMap.setMask(pixMap.createHeuristicMask());
    QApplication::setWindowIcon(QIcon(pixMap));

    QtTranslator::instance()->loadTranslations(QDir("translations:"));
    QtTranslator::instance()->loadLanguageFromSettings();

    QString pathConfig = InitMain::findConfigFilePath();
    if (pathConfig.isEmpty())
        pathConfig = QDir("etc:").filePath(qApp->applicationName() + ".xml");
    mSettings = new QtXmlSettings(pathConfig);
    QString sLogFile = CmdLineArguments::instance(QCoreApplication::arguments()).log();
    if(sLogFile.isEmpty())
        sLogFile = QFileInfo(QCoreApplication::applicationFilePath()).completeBaseName() + ".log";
    qInstallMsgHandler(logOutput);
    qAddPostRoutine(closeLog);
    if (!settings()->contains("KeepLog"))
        settings()->setValue("KeepLog", true);
    if (settings()->value("KeepLog").toBool())
    {
        logFile = new QFile(sLogFile, &app);
        if (!logFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
            qWarning("Unable to create log file");
    } else
        QFile::remove(sLogFile);

    qDebug() << "=========> NEW_SESSION_STARTED <=========";
    qDebug() << QString(" %1, Version %2 ").arg(qApp->applicationName(), qApp->applicationVersion());
    qDebug() << QString(" Build date: %1 ").arg(__DATE__);
    qWarning() << QString(" Configuration file: '%1' ").arg(pathConfig) << "\n";

    settings()->beginGroup("Database");
    const QString driverName = settings()->value("Driver").toString();

#ifdef QT_DEBUG
    if(driverName == "QOCI") {
        // fix Oracle bug for heap error
        QLibrary lib("oci");
        //lib.load();
    }
#endif
    QSqlDatabase db = QSqlDatabase::addDatabase(driverName);
    db.setDatabaseName(settings()->value("DatabaseName").toString());
    db.setHostName(settings()->value("Host").toString());
    db.setPort(settings()->value("Port").toInt());
    db.setUserName(settings()->value("User").toString());
    db.setPassword(settings()->value("Password").toString());

    if (driverName == "QPSQL") {
        db.setConnectOptions("connect_timeout=5");
    }

    db.open();

    if(QSqlDatabase::database().driverName() == "QOCI")
    {
        QSqlQuery query(QSqlDatabase::database());
        query.prepare("BEGIN DBMS_SESSION.SET_CONTEXT ( 'CLIENTCONTEXT', 'ARINC_CHANGES_FLAG', 777 ); END;");
        if (!query.exec())
        {
            qWarning() << query.lastError().text();
            return 1;
        }
    }

    ArincMergeMainWindow w;
    w.show();
    return app.exec();
}
