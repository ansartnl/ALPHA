#include "main.h"

#include <QtDebug>

#include <QtCore/QCoreApplication>
#include <QtCore/QTextCodec>

#include <QtCore/QDateTime>
#include <QtCore/QLibrary>

#include "aftnservice.h"

#include "qita2textcodec.h"

int main(int argc, char** argv)
{
    QThread::currentThread()->setObjectName("main");

// Uncomment next lines to temporaly install this service in /tmp folder under none-Windows OS
//#if !defined(Q_WS_WIN)
//    // QtService stores service settings in SystemScope, which normally require root privileges.
//    // To allow testing this example as non-root, we change the directory of the SystemScope settings file.
//    QSettings::setPath(QSettings::NativeFormat, QSettings::SystemScope, QDir::tempPath());
//    qWarning("(Example uses dummy settings file: %s/QtSoftware.conf)", QDir::tempPath().toLatin1().constData());
//#endif

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    // GOTOV K PRIEMU
    //qDebug() << (new QITA2TextCodec())->makeDecoder()->toUnicode(QByteArray::fromHex("00110e110e041610191b16181304170601170404040404000808020d0d0407101010150d151d0808021b160717060117000407101010110a1116080802051e0e041a18101813040f04160a06011c0700080802020202020202020c0c0c0c"));
    //qDebug() << (new QITA2TextCodec())->makeDecoder()->toUnicode(QByteArray::fromHex("00110e110e041610191b161517041706130704040404040808021f0d0d0407101010150d151d0808021b160717061307041f07101010110a111608080802051e0e041a18101813040f04160a06011c07080802020202020202020c0c0c0c"));
    //qDebug() << (new QITA2TextCodec())->makeDecoder()->toUnicode(QByteArray::fromHex("00110e110e041610191b161517041718011004040404040808021f0d0d0407101010150d151d0808021b160717180110041f07101010110a1116080802051e0e041a18101813040f04160a06011c07080802020202020202020c0c0c0c"));

    //if(driver == "QOCI")
    {
        // fix Oracle bug for heap error
        QLibrary lib("oci");
        //lib.load();
    }

    AFTNService service(argc, argv);
    service.setStartupType(QtServiceController::AutoStartup);

    return service.exec();
}

const QString xmlPath(const QString &init)
{
    static QString path(init);
    return path;
}
