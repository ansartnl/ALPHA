#include "ExtendedStyle.h"

#include <QtCore/QFile>

void ExtendedStyle::saveToFile(const QString &fileName)
{
    QFile file(fileName);
    file.open(QFile::WriteOnly);
    QDataStream stream(&file);
    stream << (*this);
    file.close();
}

void ExtendedStyle::loadFromFile(const QString &fileName)
{
    QFile file(fileName);
    file.open(QFile::ReadOnly);
    QDataStream stream(&file);
    stream >> (*this);
    file.close();
}

QDataStream & operator <<(QDataStream &stream, const ExtendedStyle &s)
{
    stream << s.palette;
    stream << s.style;
    return stream;
}

QDataStream & operator >>(QDataStream &stream, ExtendedStyle &s)
{
    stream >> s.palette;
    stream >> s.style;
    return stream;
}
