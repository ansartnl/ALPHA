#ifndef ACCORD_CODOGRAM_6TO9_H
#define ACCORD_CODOGRAM_6TO9_H

#include "accordexport.h"
#include "accorderrors.h"
#include "accord.h"

#include <QtCore/QDataStream>


//! Generator and parser for Codogram 6-9 Аккорд-ССПД protocol.
/*! Контрольное  сообщение (Вид 1 - 4).

    Максимальный  темп  передачи  -  1  сообщение  за  обзор
*/
class ACCORD_EXPORT Codogram6to9 : public Accord
{
    void testType(quint8 value) const;

public:
    Codogram6to9();

    static int classCategoryBegin();
    static int classCategoryEnd();
    static QString classEdition();

    virtual int category() const;
    virtual QString edition() const;

    virtual AccordErrors::ErrorCode parse(QDataStream &stream);
    virtual AccordErrors::ErrorCode generate(QByteArray &result) const;

public:
    // Тип контрольного сообщения
    enum EType
    {
        TypeUnknown,
        Type1,
        Type2,
        Type3,
        Type4
    };

    // Тип контрольного сообщения
    EType mType;
};

#endif // ACCORD_CODOGRAM_6TO9_H
