#include "mainwindow.h"

#include "main.h"

#include "InitMain.h"

#include <QtGui/QApplication>

#include <QtNetwork/QHostAddress>

int main(int argc, char *argv[])
{
    InitMain::setupCodec("UTF-8");

    QApplication a(argc, argv);

    InitMain::setupSearchPath();

    qApp->setApplicationName("aftnmanager");
    qApp->setApplicationVersion(VERSION_STR);
    qApp->setOrganizationDomain("Peleng Ltd.");

    MainWindow w;
    w.show();

    return a.exec();
}
