#include "FileCopy.h"
#include <QFile>

CFileCopy::CFileCopy(QObject *parent) :
    QThread(parent),
    isRunning(false)
{
}

void CFileCopy::run()
{
    QFile file_src(source);
    QFile file_dst(dest);
    file_src.open(QIODevice::ReadOnly);
    file_dst.open(QIODevice::WriteOnly);

    if(!file_src.isOpen() || !file_dst.isOpen())
        return;

    isRunning = true;
    QByteArray str;
    int delta = file_src.size() / 100.;
    int n = 0;

    while(!file_src.atEnd() && isRunning)
    {
        str = file_src.readLine();
        n += str.size();
        file_dst.write(str);
        if(n >= delta)
        {
            emit tick();
            n = 0;
        }
    }
    file_src.close();
    file_dst.close();

    emit done();
}

void CFileCopy::cancel()
{
    isRunning = false;
    msleep(50);
    QFile::remove(dest);
    exit();
}
