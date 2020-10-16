#ifndef ACCORD_CODOGRAM_10_H
#define ACCORD_CODOGRAM_10_H

#include "accordexport.h"
#include "accorderrors.h"
#include "accord.h"

#include <QtCore/QDataStream>


//! Generator and parser for Codogram 10 Аккорд-ССПД protocol.
/*! Сообщение  о  30 градусной  метке.

    Темп  передачи  -  12  сообщений  за  обзор
*/
class ACCORD_EXPORT Codogram10 : public Accord
{
public:
    Codogram10();

    static int classCategory();
    static QString classEdition();

    virtual int category() const;
    virtual QString edition() const;

    virtual AccordErrors::ErrorCode parse(QDataStream &stream);
    virtual AccordErrors::ErrorCode generate(QByteArray &result) const;

public:
    // Число выброшенных пакетов
    quint8 mPackageNumberDiscarded;

    // Код  метки  азимута  (30 градусов)
    quint8 mAzimuthMark;

    // Перегрузка  выходного  буфера
    bool mBufferOverload;

    // Время  локации
    quint16 mVRF;               // in ms % 100 (precision)
};

#endif // ACCORD_CODOGRAM_10_H
