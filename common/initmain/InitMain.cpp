#include "InitMain.h"
#include "CmdLineArguments.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QSettings>
#include <QtCore/QDir>
#include <QtCore/QTextCodec>
#include <QtCore/QDebug>
#include <QtCore/QFile>

void InitMain::setupSearchPath(const QString& applicationDirPath) {
    QDir currentDir = QDir(applicationDirPath);
    QDir::addSearchPath("bin", currentDir.absolutePath());

    if (currentDir.cd("translations")) {
        QDir::addSearchPath("translations", currentDir.absolutePath());
        currentDir.cdUp();
    }

    currentDir.cdUp();

    QDir::addSearchPath("approot", currentDir.absolutePath());
    if (currentDir.cd("plugins")) {
        QDir::addSearchPath("plugins", currentDir.absolutePath());
        // Michael
        QCoreApplication::addLibraryPath(currentDir.absolutePath());
        currentDir.cdUp();
    }

    if (currentDir.cd("etc")) {
        QDir::addSearchPath("etc", currentDir.absolutePath());
        currentDir.cdUp();
    }

    if (currentDir.cd("log")) {
        QDir::addSearchPath("log", currentDir.absolutePath());
        currentDir.cdUp();
    }

    if (currentDir.cd("share")) {
        QDir::addSearchPath("share", currentDir.absolutePath());
        if (!QDir("translations:").exists()) {
            if (currentDir.cd("translations")) {
                QDir::addSearchPath("translations", currentDir.absolutePath());
                currentDir.cdUp();
            } else
                QDir::addSearchPath("translations", currentDir.absolutePath());
        }
        if (currentDir.cd("images")) {
            QDir::addSearchPath("images", currentDir.absolutePath());
            currentDir.cdUp();
        }
        currentDir.cdUp();
    }
}

void InitMain::setupSearchPath() {
    setupSearchPath(QCoreApplication::applicationDirPath());
}

QString InitMain::findConfigFilePath(const QString& applicationDirPath, const QString& applicationName) {
    QString configMask("%1/%2.xml");
    QString pathConfigFile = QString(configMask).arg(applicationDirPath).arg(applicationName);
    if (!QFile::exists(pathConfigFile)) {
        QDir etcDir("etc:");
        if (!etcDir.exists())
            return QString();

        pathConfigFile = QString(configMask).arg(etcDir.absolutePath()).arg(applicationName);
        if (!QFile::exists(pathConfigFile)) {
            pathConfigFile = QString(configMask).arg(etcDir.absolutePath()).arg("config");
            if (!QFile::exists(pathConfigFile))
                return QString();
        }
    }

    return pathConfigFile;
}

QString InitMain::findConfigFilePath() {
    // Initialize command line signleton.
    CmdLineArguments::instance(QCoreApplication::arguments());

    // Try to find configuration file in the command line arguments.
    QString pathConfigFile = CmdLineArguments::instance().config();
    if ( QFile::exists(pathConfigFile) )
        return pathConfigFile;

    // Search path to config by executable file name.
    pathConfigFile = findConfigFilePath(QCoreApplication::applicationDirPath(),
                                        QFileInfo(QCoreApplication::applicationFilePath()).baseName());

    // Search path to config by application name.
    if ( pathConfigFile.isEmpty() )
        pathConfigFile = findConfigFilePath(QCoreApplication::applicationDirPath(),
                                            QCoreApplication::applicationName());

    return pathConfigFile;
}

// for QtService
QString InitMain::findConfigFilePath(int argc, char** argv) {
#ifdef Q_OS_WIN
    QStringList arguments;
    for (int i = 0; i < argc; ++i)
        arguments.push_back(argv[i]);
#else
    Q_UNUSED(argc);
    Q_UNUSED(argv);
    QStringList arguments = QCoreApplication::arguments();
#endif

    // Initialize command line signleton.
    CmdLineArguments::instance(arguments);

    // Try to find configuration file in the command line arguments.
    QString pathConfigFile = CmdLineArguments::instance().config();
    if (QFile::exists(pathConfigFile))
        return pathConfigFile;

    // Search path to config by executable file name.
    pathConfigFile = findConfigFilePath(QCoreApplication::applicationDirPath(),
                                        QFileInfo(QCoreApplication::applicationFilePath()).baseName());

    // Search path to config by application name.
    if (pathConfigFile.isEmpty())
        pathConfigFile = findConfigFilePath(QCoreApplication::applicationDirPath(),
                                            QCoreApplication::applicationName());

    return pathConfigFile;
}

QString InitMain::findXmlFilePath(const QString& applicationDirPath, const QString& typeConfig) {
    // Initialize command line signleton.
    CmdLineArguments::instance(QCoreApplication::arguments());

    // find suffix.
    QString suffix = CmdLineArguments::instance().xml();

    // Search path to config by executable file name.
    QStringList configName;
    configName << QFileInfo(QCoreApplication::applicationFilePath()).baseName();
    if (!typeConfig.isEmpty())
        configName << typeConfig;
    configName << suffix;

    QString pathConfigFile = findConfigFilePath(applicationDirPath, configName.join("_"));

    if (pathConfigFile.isEmpty())
        pathConfigFile = findConfigFilePath(QCoreApplication::applicationDirPath(), configName.join("_"));

    // Search path to config by application name.
    configName.clear();
    configName << QCoreApplication::applicationName();
    if (!typeConfig.isEmpty())
        configName << typeConfig;
    configName << suffix;

    if (pathConfigFile.isEmpty())
        pathConfigFile = findConfigFilePath(QCoreApplication::applicationDirPath(), configName.join("_"));

    return pathConfigFile;
}


QString InitMain::configUsageString() {
    return QObject::tr("Usage: configuration file locations\r\n"
                       "-c, --config, --config= : command line argument\r\n"
                       "- current folder 'file-name.xml'\r\n"
                       "- ../etc folder 'file-name.xml'\r\n"
                       "- ../etc/config.xml\r\n"
                       "- current folder '%1.xml'\r\n"
                       "- ../etc folder '%1.xml'").arg(QCoreApplication::applicationName());
}

void InitMain::setupCodec(const QByteArray& codecName) {
    QTextCodec* codec = QTextCodec::codecForName(codecName);
    if (codec) {
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
        QTextCodec::setCodecForTr(codec);
        QTextCodec::setCodecForCStrings(codec);
#endif
        QTextCodec::setCodecForLocale(codec);
    } else
        qWarning() << "Unable to setup codec: invalid codec name: " << codecName;
}

QVariant InitMain::initValue(QSettings& settings, const QString& key, const QVariant& defaultValue, const QString& comment) {
    if (!settings.contains(key))
        settings.setValue(key, defaultValue);
    if (!comment.isNull()) {
        settings.beginGroup(key);
        if (!settings.contains("comment"))
            settings.setValue("comment", comment);
        settings.endGroup();
    }
    return settings.value(key);
}
