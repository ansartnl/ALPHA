#include <QFile>
#include <stdio.h>
#include <sys/types.h>
#include <iostream>
#include <fcntl.h>
#ifdef Q_OS_LINUX
#include <linux/hdreg.h>
#include <sys/ioctl.h>
#endif
#include <stdlib.h>
#include <errno.h>
#include "lic.h"

lic::lic()
{
}

QString lic::chd()
{
#ifdef Q_OS_LINUX
    struct hd_driveid id;
    int fd = open("/dev/sda", O_RDONLY|O_NONBLOCK);

    if (fd < 0)    perror("/dev/sda");

    int iRes = ioctl(fd, HDIO_GET_IDENTITY, &id);
    if(-1 != iRes)
    {
        QString sl((char *)id.serial_no);
#ifdef _DEBUG
        printf("Serial Number=%s\n",id.serial_no);
        printf("Model Number=%s\n",id.model);
        printf("Firmware Revision=%.8s\n",id.fw_rev);
        printf("Cylinders=%d\n",id.cyls);
        printf("heads=%d\n",id.heads);
        printf("Sectors/Track=%d\n",id.sectors);
#endif
        return sl.simplified();
    }
    else
    {
        perror("hdparm");
    }
#endif
    return "";
}


bool lic::check_license(QString lic_file)
{
#ifdef Q_OS_LINUX
    QFile f(lic_file);
    if (f.open(QFile::ReadOnly))
    {
        int size = f.size();
        char data[size];
        f.read(data,size);
        QString serial_no(chd());
        printf("serial number:%s \n",serial_no.toLatin1().data());
        if (serial_no.size() > 0)
        {
            // Decode license
            QString out;
            int n = 0;
            for (int i =0; i < size; i++)
            {
                out += (data[i]^serial_no.at(n).toLatin1());
                if (++n == serial_no.size()) n = 0;
            }
            return out == test_str;
        }
    }
    return false;
#endif
    return true;
}

bool lic::generate_license(QString lic_file, QString serial_no)
{
    QFile f(lic_file);
    if (serial_no.size() == 0)
        serial_no = chd();
    printf("\n Serial:%s\n",serial_no.toLatin1().data());
    if (serial_no.size() > 0 && f.open(QFile::WriteOnly))
    {
        QString out;
        int n = 0;
        for (int i = 0; i < test_str.size();i++)
        {
            out += (test_str.at(i).toLatin1() ^ serial_no.at(n).toLatin1());
            if (++n == serial_no.size()) n = 0;
        }
        QByteArray a;
        f.write(out.toLatin1().data(),out.size());
        f.close();
        return true;
    }
    return false;
}
