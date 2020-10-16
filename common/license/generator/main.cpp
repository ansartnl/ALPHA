#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QApplication>

#include "HardwareId.h"
#include "License.h"
#include "MainWindow.h"

inline QTextStream& qStdout()
{
    static QTextStream r(stdout);
    return r;
}

int main(int argc, char* argv[])
{   
    QStringList arguments;
    for (int i = 1; i < argc; ++i)
    {
        arguments.append(argv[i]);
    }

    switch (arguments.count()) {
    case 0:
    {
        QApplication app(argc, argv);
        MainWindow window;
        window.show();
        return app.exec();
    }
    case 2:
    case 3:
    {
        QFile hardwareIdFile(arguments[0]);
        QFile licKeyFile(arguments[1]);
        QByteArray key;
        if (arguments.count() == 3)
        {
            key = arguments[2].toLatin1();
        }

        if (!hardwareIdFile.open(QIODevice::ReadOnly))
        {
            qStdout() << QString("Cannot open %1 for reading").arg(hardwareIdFile.fileName()) << endl;
            return 1;
        }

        HardwareId hardwareId(hardwareIdFile.readAll());
        QByteArray licKey = License::generateLicKey(hardwareId.toRegKey(), key);

        if (!licKeyFile.open(QIODevice::WriteOnly))
        {
            qStdout() << QString("Cannot open %1 for writing").arg(licKeyFile.fileName()) << endl;
            return 1;
        }

        if (licKeyFile.write(licKey) != licKey.size())
        {
            qStdout() << QString("Cannot write license key") << endl;
            return 1;
        }

        return 0;
    }
    default:
    {
        qStdout() << QString("Usage: %1 path_to_key.hwid path_to_key.lic [secret_key]").arg(argv[0]) << endl;
        return 1;
    }
    }
}
