#include "main.h"
#include <QtGui/QApplication>
#include <QMessageBox>
#include "mainwindow.h"
#include "InitMain.h"
#include "qttranslator.h"
#include "qtxmlsettings.h"

QSettings& xmlSettings(const QString &pathConfig)
{
    static QtXmlSettings xmlStg(pathConfig);
    return xmlStg;
}

int main(int argc, char *argv[])
{
    InitMain::setupCodec();

    QApplication a(argc, argv);
    a.setOrganizationName( "ITOS" );
    a.setApplicationName("RecoveryTool");
    InitMain::setupSearchPath();

    QtTranslator::instance()->loadTranslations(QDir("translations:"));
    QtTranslator::instance()->loadLanguageFromSettings();

    QString pathConfigFile = InitMain::findConfigFilePath();

    // Make sure configuration exists.
    if ( pathConfigFile.isEmpty() )
    {
        QMessageBox::warning(0, QObject::tr("Configuration"),
                             QObject::tr("Configuration file is not found.\n\n") +
                             InitMain::configUsageString());
        return 0;
    }

    // Initialize xml settings technique.
    xmlSettings(pathConfigFile);

    QString lang = xmlSettings().value("language", "english").toString().toLower() == "english"?
                QString("English"):
                QString("Русский");
    QtTranslator::instance()->setCurrentLanguage(lang);

    MainWindow w;
    w.Init();
    w.show();
    
    return a.exec();
}
