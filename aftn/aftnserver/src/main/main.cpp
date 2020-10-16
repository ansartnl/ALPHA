
#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QNetworkInterface>

#include "qtxmlsettings.h"
#include "aftnserver.h"

#define SETTINGS_NETWORK_GROUP          "network"
#define SETTINGS_INTERFACE              "interface"
#define SETTINGS_PORT                   "port"
#define SETTINGS_AFTN_NAMES             "names"
#define SETTINGS_LOG_FILE               "log_file"


int main(int argc, char** argv)
{
  QCoreApplication::setOrganizationDomain("peleng.ru");
  QCoreApplication::setOrganizationName("Peleng");
  QCoreApplication::setApplicationName("aftnclient");

  QCoreApplication application(argc, argv);

  QSettings *settings = new QtXmlSettings("./config.xml", &application);

  if (!settings->contains(SETTINGS_NETWORK_GROUP"/"SETTINGS_INTERFACE)) {
    // loadDefaults();
    settings->setValue(SETTINGS_INTERFACE, "127.0.0.1");
    settings->setValue(SETTINGS_PORT, "10200");
    settings->setValue(SETTINGS_AFTN_NAMES, QStringList() << "SOMENAME");
    settings->setValue(SETTINGS_LOG_FILE, QCoreApplication::applicationName() + ".log");
  }


  QHostAddress address(settings->value(SETTINGS_INTERFACE).toString());
  quint16 port = settings->value(SETTINGS_PORT).toUInt();
  qDebug() << QObject::tr("The AFTN server is running on\n\nIP: %1\nport: %2\n\n")
                       .arg(address.toString()).arg(port);

  AftnServer server(settings->value(SETTINGS_AFTN_NAMES).toStringList());

  if (!server.listen(address, port )) {
    qDebug() << server.errorString();
    return -1;
  } else
    return application.exec();
}
