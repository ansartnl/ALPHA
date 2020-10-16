#ifndef ACCORD_CODOGRAM_3_H
#define ACCORD_CODOGRAM_3_H

#include "accordexport.h"
#include "accorderrors.h"
#include "codogramplane.h"

#include <QtCore/QDataStream>


//! Generator and parser for Codogram 3 Аккорд-ССПД protocol.
/*! Сообщение  о  двух  воздушных  судах.

    Максимальный  темп  передачи  -  380  сообщений  за  обзор.
*/
class ACCORD_EXPORT Codogram3 : public CodogramPlane
{
    void testProhibition(quint8 value) const;
    void testInformationSource(quint8 value) const;
    void testInformationType(quint8 value) const;

public:
    Codogram3();

    static int classCategory();
    static QString classEdition();

    virtual int category() const;
    virtual QString edition() const;

    virtual AccordErrors::ErrorCode parse(QDataStream &stream);
    virtual AccordErrors::ErrorCode generate(QByteArray &result) const;

public:

    // Канал  ответа  ВРЛ (1-е  ВС)
    EChannel mChannel1;
    // Канал  ответа  ВРЛ (2-е  ВС)
    EChannel mChannel2;
    // Знак  (SPI  по  ИКАО) (1-е  ВС)
    quint8 mSPI1;                   // quint8 - bit: 0,1
    // Знак  (SPI  по  ИКАО) (2-е  ВС)
    quint8 mSPI2;                   // quint8 - bit: 0,1

    // Вид  информации. Сообщения  о  координатах  ВС.
    EInformationType mInformationType;

    // Бедствие
    EDisaster mDisaster1;              // int - EDisaster
    // Бедствие
    EDisaster mDisaster2;              // int - EDisaster

    // Источник информации о 1 ВС
    EInformationSource mInformationSource1;     // int - EInformationSource
    // Источник информации о 2 ВС
    EInformationSource mInformationSource2;     // int - EInformationSource

    // Неразрешение (по 1-му  ВС)
    EProhibition mProhibition1;           // int - EProhibition
    // Неразрешение (по 2-му  ВС)
    EProhibition mProhibition2;           // int - EProhibition

    // Координата X
    int mX1;                     // int - meters % 100 (precision), can be negative
    // Координата X
    int mX2;                     // int - meters % 100 (precision), can be negative

    // Координата X
    int mY1;                     // int - meters % 100 (precision), can be negative
    // Координата X
    int mY2;                     // int - meters % 100 (precision), can be negative

    // Время  локации
    quint16 mVRF1;                   // quint16 in ms
    // Время  локации
    quint16 mVRF2;                   // quint16 in ms
};

#endif // ACCORD_CODOGRAM_3_H
