#ifndef EXTENDEDSTYLE_H
#define EXTENDEDSTYLE_H

#include <QtCore/QVariant>

#include <QtGui/QPalette>

struct ExtendedStyle
{
    QPalette palette;
    QString style;

    void saveToFile(const QString &fileName);
    void loadFromFile(const QString &fileName);
};

QDataStream & operator <<(QDataStream &stream, const ExtendedStyle &s);
QDataStream & operator >>(QDataStream &stream, ExtendedStyle &s);

#endif // EXTENDEDSTYLE_H
