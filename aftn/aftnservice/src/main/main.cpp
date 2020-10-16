
#include <QtDebug>

#include <QtCore/QCoreApplication>
#include <QtCore/QTextCodec>

#include "aftnservice.h"

int main(int argc, char** argv)
{
    QThread::currentThread()->setObjectName("main");
#if !defined(Q_WS_WIN)
  // QtService stores service settings in SystemScope, which normally require root privileges.
  // To allow testing this example as non-root, we change the directory of the SystemScope settings file.
  QSettings::setPath(QSettings::NativeFormat, QSettings::SystemScope, QDir::tempPath());
  qWarning("(Example uses dummy settings file: %s/QtSoftware.conf)", QDir::tempPath().toLatin1().constData());
#endif

  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
  QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
  QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

  QThread::currentThread()->setObjectName("main");
  AFTNService service(argc, argv);
  service.setStartupType(QtServiceController::AutoStartup);

  return service.exec();
}
