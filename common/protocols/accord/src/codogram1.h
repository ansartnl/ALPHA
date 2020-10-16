#ifndef ACCORD_CODOGRAM_1_H
#define ACCORD_CODOGRAM_1_H

#include "accordexport.h"
#include "accorderrors.h"
#include "accord.h"

#include <QtCore/QDataStream>


//! Generator and parser for Codogram 1 Аккорд-ССПД protocol.
/*! Сообщение о приоритетном СЕВЕРЕ, о техническом состоянии источников информации
    и их режимах работы.

    Темп  передачи  -  2  сообщения  за  обзор
*/
class ACCORD_EXPORT Codogram1 : public Accord
{
public:
    void testCB(quint8 value) const;
    void testCP(quint8 value) const;
    void testBI(quint8 value) const;
    void testRK(quint8 value) const;
    void testMI(quint8 value) const;
    void testDmax(quint8 value) const;
    void testVPK(quint16 value) const;
    void testZV(quint8 value) const;
    void testCKP(quint16 value) const;

public:
    Codogram1();

    static int classCategory();
    static QString classEdition();

    virtual int category() const;
    virtual QString edition() const;

    virtual AccordErrors::ErrorCode parse(QDataStream &stream);
    virtual AccordErrors::ErrorCode generate(QByteArray &result) const;

public:
    //! Состояние общей аппаратуры РЛК
    enum ECO
    {
        CO_Ready     = 1,   //!< Исправен
        CO_Reserve   = 0    //!< Переход на резерв
    };

    //! Состояние ВРЛ
    enum ECB
    {
        CB_Ready    = 0x03,     //!< Исправен
        CB_Reserve  = 0x01,     //!< Переход на резерв
        CB_Refuse   = 0x02      //!< Полный отказ
    };

    //! Состояние ПРЛ
    enum ECP
    {
        CP_Ready                = 0x07,   //!< Исправен
        CP_Reserve              = 0x03,   //!< Переход на резерв
        CP_FaultPartly          = 0x05,   //!< Частичная неисправность
        CP_FaultPartlyReserve   = 0x01,   //!< Част. неисправн. с переходом на резерв
        CP_Refuse               = 0x06    //!< Полный отказ
    };

    // Бланки информации
    enum EBI
    {
        BI_1    = 0x01,
        BI_2    = 0x02,
        BI_3    = 0x04,
        BI_4    = 0x08,
        BI_5    = 0x10
    };

    // Режим выдачи информации в каналы связи
    enum EModeRK
    {
        RK_Repeated     = 0x01,     //!< каждое сообщение передается по двум каналам
        RK_Distributed  = 0x02      //!< все сообщения распределяются по двум каналам
    };

    // Дальность РЛК максимальная
    enum EDistanceMax
    {
        Km250   = 0x06,
        Km300   = 0x04,
        Km350   = 0x02,
        Km400   = 0x00
    };

    // Метеоинформация
    enum EMeteoInfo
    {
        MI_On   = 0x03,     //!< выдается  (выдавать)
        MI_Off  = 0x00      //!< не выдается  (не  выдавать)
    };

    enum EModeRequestVRL
    {
        ZV_ModeA    = 0x08,   //!< режим А
        ZV_ModeB    = 0x04,   //!< режим B
        ZV_ModeC    = 0x02    //!< режим C
    };

    // Выдача пеленга по каналу
    enum EChannel
    {
        Channel_1    = 0x0001,
        Channel_2    = 0x0002,
        Channel_3    = 0x0004,
        Channel_4    = 0x0008,
        Channel_5    = 0x0010,
        Channel_6    = 0x0020,
        Channel_7    = 0x0040,
        Channel_8    = 0x0080,
        Channel_9    = 0x0100,
        Channel_10   = 0x0200,
        Channel_11   = 0x0400,
        Channel_12   = 0x0800,
        Channel_Refuse  = 0x0FFF
    };

    // Работа 1-го комплекта АПОИ
    bool mA1;
    // Работа 2-го комплекта АПОИ
    bool mA2;

    // Число выброшенных пакетов
    quint8 mPackageNumberDiscarded;

    // Состояние общей аппаратуры РЛК
    ECO mCO;

    // Состояние ВРЛ
    ECB mCB;

    // Состояние ПРЛ
    ECP mCP;

    // Бланки информации
    EBI mBI;

    // Передача в АПД-1
    bool mTransferAPD1;
    // Передача в АПД-2
    bool mTransferAPD2;
    // Прием из АПД-1
    bool mReceiveAPD1;
    // Прием из АПД-2
    bool mReceiveAPD2;
    // Работа БПРД-С 2/1
    bool mFunctionBPRD;

    // Режим выдачи информации в каналы связи
    EModeRK mRK;

    // Исправность 1-го комплекта АПОИ
    bool mI1;
    // Исправность 2-го комплекта АПОИ
    bool mI2;

    // Дальность РЛК максимальная
    EDistanceMax mDmax;

    // Метеоинформация
    EMeteoInfo mMI;

    // Режимы запроса ВРЛ
    EModeRequestVRL mZV;

    // Выдача пеленга по каналу
    EChannel mVPK;           // int - Channel

    // Время  локации
    quint16 mVRF;           // in ms % 100 (precision)

    // Включение ПРЛ
    bool mVP;
    // Включение ВРЛ
    bool mVV;
    // Включение общей аппаратуры
    bool mVO;

    // Состояние каналов АРП
    EChannel mCKP;
};

#endif // ACCORD_CODOGRAM_1_H
