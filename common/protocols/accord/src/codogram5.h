#ifndef ACCORD_CODOGRAM_5_H
#define ACCORD_CODOGRAM_5_H

#include "accordexport.h"
#include "accorderrors.h"
#include "accord.h"

#include <QtCore/QDataStream>


//! Generator and parser for Codogram 5 Аккорд-ССПД protocol.
/*! Сообщение  о  юстировочных  координатах.

    Максимальный  темп  передачи  определяется  количеством  отметок  в  стробе
*/
class ACCORD_EXPORT Codogram5 : public Accord
{
    void testAzimuth(qreal azimuth) const;

public:
    Codogram5();

    static int classCategory();
    static QString classEdition();

    virtual int category() const;
    virtual QString edition() const;

    virtual AccordErrors::ErrorCode parse(QDataStream &stream);
    virtual AccordErrors::ErrorCode generate(QByteArray &result) const;

public:
    // Координата X
    int mX;                     // meters % 100 (precision), can be negative

    // Координата X
    int mY;                     // meters % 100 (precision), can be negative

    // Азимут  юстировочный
    qreal mAzimuth;             // 0 - 360

    // Время  локации
    quint16 mVRF;               // in ms % 100 (precision)
};

#endif // ACCORD_CODOGRAM_5_H
