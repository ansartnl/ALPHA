#ifndef ACCORD_H
#define ACCORD_H

#include "accorderrors.h"
#include "accordexport.h"

#include <QtCore/QSharedPointer>

class ACCORD_EXPORT Accord
{
public:
    virtual ~Accord();

    virtual int category() const = 0;
    virtual QString edition() const = 0;

    virtual AccordErrors::ErrorCode parse(QDataStream &stream) = 0;
    virtual AccordErrors::ErrorCode generate(QByteArray &result) const = 0;

protected:
    Accord();

    int parseCoordinate(quint16 value) const;
    quint16 generateCoordinate(int coord) const;
};

typedef QSharedPointer<Accord>  TAccordCodogram;

#endif // ACCORD_H
