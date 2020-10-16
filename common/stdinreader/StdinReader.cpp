#include "StdinReader.h"

#include "sleep.h"

#include <QtCore/QDebug>
#include <QtCore/QTextStream>

StdinReader::StdinReader(QObject *parent)
    : QThread (parent)
{
}

//-----------------------------------------------------
void StdinReader::run()
{
    sleep(1);
    QTextStream input(stdin, QIODevice::ReadOnly);

    while (true)
    {
        QString line = input.readLine();
        if (!line.isNull())
            emit received(line);
        else
            sleep(1);
    }

    return;
}
