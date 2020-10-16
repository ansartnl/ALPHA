#include <fcntl.h>
#include <unistd.h>
#include <linux/hdreg.h>
#include <sys/ioctl.h>

#include <QtCore/QFile>
#include <QtCore/QProcess>
#include <QtNetwork/QNetworkInterface>

#include "HardwareProvider.h"

HardwareId HardwareProvider::getHardwareId()
{
    HardwareId id;
    fillBoardId(id);
    fillDriveId(id);
    fillEthernetId(id);
    fillMemoryId(id);

    return id;
}

void HardwareProvider::fillBoardId(HardwareId &id)
{
    QString result = readLineFromFile("/sys/class/dmi/id/board_serial");
    if (result.contains(' '))
        return;

    id.setId(HardwareId::Board, result);
}

void HardwareProvider::fillDriveId(HardwareId &id)
{
    QList<QByteArray> drives;
    drives << "/dev/sda" << "/dev/hda";

    foreach (const QByteArray &drive, drives)
    {
        int device = open(drive.constData(), O_RDONLY | O_NONBLOCK);
        if (device < 0)
            continue;

        struct hd_driveid hd;
        int err = ioctl(device, HDIO_GET_IDENTITY, &hd);
        close(device);

        if (!err)
        {
            if (id.setId(HardwareId::Drive, QString((char*)hd.serial_no)))
            {
                break;
            }
        }
    }
}

void HardwareProvider::fillEthernetId(HardwareId &id)
{
    QStringList patterns;
    patterns << "en" << "eth";

    foreach (const QNetworkInterface &interface, QNetworkInterface::allInterfaces())
    {
        bool valid = false;
        foreach (const QString pattern, patterns)
        {
            if (interface.name().startsWith(pattern))
            {
                valid = true;
                break;
            }
        }

        if (valid)
        {
            id.setId(HardwareId::Ethernet, interface.hardwareAddress());
            return;
        }
    }
}

void HardwareProvider::fillMemoryId(HardwareId &id)
{
    QProcess process;
    process.start("dmidecode", QStringList() << "-q" << "-t" << "memory");
    if (!process.waitForStarted())
        return;

    if (!process.waitForFinished())
        return;

    id.setId(HardwareId::Memory, process.readAll());
}

QString HardwareProvider::readLineFromFile(const QString &path)
{
    QString result;

    QFile file(path);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        result = file.readLine();
    }

    return result;
}
