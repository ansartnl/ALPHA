#ifndef ACCORD_CODOGRAM_4_H
#define ACCORD_CODOGRAM_4_H

#include "accordexport.h"
#include "accorderrors.h"
#include "accord.h"

#include <QtCore/QDataStream>


//! Generator and parser for Codogram 4 Аккорд-ССПД protocol.
/*! Сообщение  о  метеоинформации.

    Максимальный  темп  передачи  -  360  сообщений  за  обзор
*/
class ACCORD_EXPORT Codogram4 : public Accord
{
public:
    Codogram4();

    static int classCategory();
    static QString classEdition();

    virtual int category() const;
    virtual QString edition() const;

    virtual AccordErrors::ErrorCode parse(QDataStream &stream);
    virtual AccordErrors::ErrorCode generate(QByteArray &result) const;

public:
    // Координата X
    int mX1;                     // int - meters % 100 (precision), can be negative
    // Координата X
    int mX2;                     // int - meters % 100 (precision), can be negative

    // Координата X
    int mY1;                     // int - meters % 100 (precision), can be negative
    // Координата X
    int mY2;                     // int - meters % 100 (precision), can be negative

    // Время  локации
    quint16 mVRF;                   // in ms % 100 (precision)
};

#endif // ACCORD_CODOGRAM_4_H
