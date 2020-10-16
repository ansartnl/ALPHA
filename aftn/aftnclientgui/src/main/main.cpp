
#include <QtCore/QTextCodec>
#include <QtCore/QDir>

#include <QtGui/QApplication>

#include "qita2textcodec.h"

#include "qtxmlsettings.h"

#include "mainwindow.h"

#include "qtlogevent.h"

#include "main.h"

#include <iostream>

static QMainWindow *debugMainWindow = NULL;
QSettings* networkSettings = 0;

QtMsgHandler qtMsgHandler;
void logOutput(QtMsgType type, const char *message)
{
  QtLogEvent::Level level;
  switch (type) {
  case QtDebugMsg:
    level = QtLogEvent::DEBUG_INT;
    break;
  case QtWarningMsg:
    level = QtLogEvent::WARN_INT;
    break;
  case QtCriticalMsg:
    level = QtLogEvent::ERROR_INT;
    break;
  case QtFatalMsg:
    level = QtLogEvent::FATAL_INT;
  }
  std::cerr << message << "\n";
  QtLogEvent event(level, message);
  QCoreApplication::sendEvent(debugMainWindow, &event);
}



/*!
  Default network connection paramaeters. All network parameters stored by networkSettings in "etc/config.xml"
  */
void initDefaultNetworkSettings()
{
  networkSettings()->beginGroup(SETTINGS_NETWORK_GROUP);
  networkSettings()->setValue(SETTINGS_AFTNSERVER1, "localhost");
  networkSettings()->setValue(SETTINGS_AFTNSERVER2, "localhost");
  networkSettings()->setValue(SETTINGS_PORT, "10200");
  networkSettings()->setValue(SETTINGS_AFTN_NAME, "SOMENAME");
  networkSettings()->setValue(SETTINGS_PING_TIMEOUT, "SOMENAME");
  networkSettings()->setValue(SETTINGS_LOG_FILE, QCoreApplication::applicationName() + ".log");
  networkSettings()->setValue(SETTINGS_RAWDATA_FILE, QCoreApplication::applicationName() + ".raw");
  networkSettings()->endGroup();
}


int main(int argc, char** argv)
{
  //! Qt customization
  //! UTF8 codecs for all possible strings in application
  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF8"));
  QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
  QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF8"));

  qtMsgHandler = qInstallMsgHandler(logOutput);

  QApplication application(argc, argv);

  QCoreApplication::setOrganizationDomain("itos.eu.org");
  QCoreApplication::setOrganizationName("Itos");
  QCoreApplication::setApplicationName("AFTNClient");

  //! Initialize some application directories
  QDir currentDir = QDir(QCoreApplication::applicationDirPath());
  QString binAppPath = currentDir.absolutePath();
  QDir::addSearchPath("bin", binAppPath);

  QString pluginsAppPath;
  QString translationsAppPath;
  QString shareAppPath;
  QString imagesAppPath;
  QString etcAppPath;

  currentDir.cdUp();
  QString rootAppPath = currentDir.absolutePath();
  QDir::addSearchPath("approot", rootAppPath);

  if (currentDir.cd("plugins")) {
      pluginsAppPath = currentDir.absolutePath();
      QDir::addSearchPath("plugins", pluginsAppPath);
      currentDir.cdUp();
  }
  if (currentDir.cd("translations")) {
      translationsAppPath = currentDir.absolutePath();
      QDir::addSearchPath("translations", translationsAppPath);
      currentDir.cdUp();
  }
  if (currentDir.cd("etc")) {
      etcAppPath = currentDir.absolutePath();
      QDir::addSearchPath("etc", etcAppPath);
      currentDir.cdUp();
  }

  if (currentDir.cd("share")) {
      shareAppPath = currentDir.absolutePath();
      QDir::addSearchPath("share", shareAppPath);
      if (currentDir.cd("images")) {
          imagesAppPath = currentDir.absolutePath();
          QDir::addSearchPath("images", imagesAppPath);
          currentDir.cdUp();
      }
      currentDir.cdUp();
  }

  QCoreApplication::addLibraryPath(pluginsAppPath);

  //! Load "etc/config.xml"
  networkSettings = new QtXmlSettings(QDir("etc:").filePath("config.xml"), &application);
  if (!networkSettings()->contains(SETTINGS_NETWORK_GROUP"/"SETTINGS_AFTNSERVER1))
      initDefaultNetworkSettings();

  MainWindow mainWindow;
  mainWindow.show();

  debugMainWindow = &mainWindow;

  return application.exec();
}
