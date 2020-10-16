#ifndef ARINCBASESTRUCT_H
#define ARINCBASESTRUCT_H

#include <QtCore/QString>

struct ArincBaseStruct
{
    ArincBaseStruct() : line(0) {}
    virtual ~ArincBaseStruct() {}
    virtual char subsectionCode() const = 0;
    virtual char sectionCode() const = 0;
    QString file;
    int line;
    inline virtual QString toString() const { return QString("File: %1:%2").arg(file).arg(line); }
};

#endif // ARINCBASESTRUCT_H
