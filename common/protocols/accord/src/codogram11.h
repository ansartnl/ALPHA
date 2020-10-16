#ifndef ACCORD_CODOGRAM_11_H
#define ACCORD_CODOGRAM_11_H

#include "accordexport.h"
#include "accorderrors.h"
#include "accord.h"

#include <QtCore/QDataStream>


//! Generator and parser for Codogram 11 Аккорд-ССПД protocol.
/*! Сообщение  о  пеленге.

    Максимальный  темп  передачи   -  12  сообщений  за  2,5 с
*/
class ACCORD_EXPORT Codogram11 : public Accord
{
    void testAzimuth(qreal azimuth) const;
    void testChannelNumber(quint8 value) const;

public:
    Codogram11();

    static int classCategory();
    static QString classEdition();

    virtual int category() const;
    virtual QString edition() const;

    virtual AccordErrors::ErrorCode parse(QDataStream &stream);
    virtual AccordErrors::ErrorCode generate(QByteArray &result) const;

public:
    // Номер канала  пеленгатора
    quint8 mChannelNumber;      // 0 - 15

    // Азимут  пеленгационный
    qreal mAzimuth;             // 0 - 360

    // Время  локации
    quint16 mVRF;               // in ms % 100 (precision)
};

#endif // ACCORD_CODOGRAM_11_H
