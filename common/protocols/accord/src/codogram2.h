#ifndef ACCORD_CODOGRAM_2_H
#define ACCORD_CODOGRAM_2_H

#include "accordexport.h"
#include "accorderrors.h"
#include "codogramplane.h"

#include <QtCore/QDataStream>


//! Generator and parser for Codogram 2 Аккорд-ССПД protocol.
/*! Сообщение  об  одном  воздушном  судне.

    Максимальный  темп  передачи  -  380  сообщений  за  обзор.
*/
class ACCORD_EXPORT Codogram2 : public CodogramPlane
{
    void testInformationType(quint8 value) const;
    void testProhibition(quint8 value) const;
    void testInformationSource(quint8 value) const;
    void testBoardNumber(quint32 value) const;

public:
    Codogram2();

    static int classCategory();
    static QString classEdition();

    virtual int category() const;
    virtual QString edition() const;

    virtual AccordErrors::ErrorCode parse(QDataStream &stream);
    virtual AccordErrors::ErrorCode generate(QByteArray &result) const;

public:
    // Уровень  отсчета  высоты
    enum EAltitudeLevel
    {
        Relative,
        Absolute
    };

    // Остаток  топлива
    enum EMinimumFuel
    {
        MinFuel_Enough,
        MinFuel_5,       //!< 5 %
        MinFuel_10,      //!< 10 %
        MinFuel_15,      //!< 15 %
        MinFuel_20,      //!< 20 %
        MinFuel_25,      //!< 25 %
        MinFuel_30,      //!< 30 %
        MinFuel_35,      //!< 35 %
        MinFuel_40,      //!< 40 %
        MinFuel_45,      //!< 45 %
        MinFuel_50,      //!< 50 %
        MinFuel_60,      //!< 60 %
        MinFuel_70,      //!< 70 %
        MinFuel_80,      //!< 80 %
        MinFuel_90,      //!< 90 %
        MinFuel_100      //!< 100 %
    };

    // Канал  ответа  ВРЛ (1-е, 2-е  ВС)
    EChannel mChannel;

    // Вид информации
    EInformationType mInformationType;

    // Знак  (SPI  по  ИКАО) (1-е, 2-е  ВС)
    quint8 mSPI;                            // bit: 0,1

    // Бедствие
    EDisaster mDisaster;

    // Уровень  отсчета  высоты
    EAltitudeLevel mAltitudeLevel;

    // Бедствие  по отечественному  каналу
    bool mDisasterDomesticChannel;

    // Остаток  топлива
    EMinimumFuel mMinumumFuel;

    // Неразрешение (по 1-му, 2-му  ВС)
    EProhibition mProhibition;

    // Источник информации о (1, 2) ВС
    EInformationSource mInformationSource;

    // Координата X
    int mX;                     // meters % 100 (precision), can be negative

    // Координата X
    int mY;                     // meters % 100 (precision), can be negative

    // Высота полета барометрическая
    quint32 mBarometricAltitude;    // meters % 10 (precision)

    // Бортовой  номер
    quint32 mBoardNumber;

    // Время  локации
    quint16 mVRF;                   // in ms % 100 (precision)
};

#endif // ACCORD_CODOGRAM_2_H
