#include "StcaService.h"
#include "InitMain.h"

#include "asterix.h"

#include "version.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QTextCodec>
#include <QtCore/QThread>
#include <QtCore/QFileInfo>

int main(int argc, char *argv[])
{
    QThread::currentThread()->setObjectName("main");

    QByteArray codecName;
    if ( QTextCodec::codecForLocale() )
        codecName = QTextCodec::codecForLocale()->name();

    InitMain::setupCodec( codecName );

    QCoreApplication::setApplicationName("snet");
    QCoreApplication::setOrganizationName("ITOS");
    QCoreApplication::setApplicationVersion( VERSION_FULL );

    Asterix::registre();

    Q_ASSERT(argc > 0);

    StcaService service(argc, argv,
                        QFileInfo(argv[0]).baseName());
    return service.exec();
}
