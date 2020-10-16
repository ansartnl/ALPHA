#ifndef ACCORD_CODOGRAM_0_H
#define ACCORD_CODOGRAM_0_H

#include "accordexport.h"
#include "accorderrors.h"
#include "accord.h"

#include <QtCore/QDataStream>


//! Generator and parser for Codogram 0 Аккорд-ССПД protocol.
/*! Сообщение  технического  состояния  и  статистики.

    Необходимость  передачи  сообщения  уточняется  дополнительно  для  каждого конкретного  объекта.
    Максимальный  темп  передачи  -  1  сообщение  за  обзор
*/
class ACCORD_EXPORT Codogram0 : public Accord
{
public:
    Codogram0();

    static int classCategory();
    static QString classEdition();

    virtual int category() const;
    virtual QString edition() const;

    virtual AccordErrors::ErrorCode parse(QDataStream &stream);
    virtual AccordErrors::ErrorCode generate(QByteArray &result) const;

public:
    // Число целей ПРЛ
    quint16 mTargetNumberPRL;

    // Число выброшенных пакетов
    quint8 mPackageNumberDiscarded;

    // Число целей ВРЛ
    quint16 mTargetNumberVRL;

    // Число объединенных целей
    quint16 mTargetNumberUnited;

    // Число сбоев (внешней) синхронизации
    quint8 mExternalFailureSyncNumber;

    // Регламент 1-го комплекта аппаратуры обработки информации ВРЛ АПОИ
    bool mOrderProcessing1VRL;
    // Регламент 2-го комплекта аппаратуры обработки информации ВРЛ АПОИ
    bool mOrderProcessing2VRL;
    // Регламент 1-го комплекта обработки информации ПРЛ АПОИ
    bool mOrderProcessing1PRL;
    // Регламент 2-го комплекта обработки информации ПРЛ АПОИ
    bool mOrderProcessing2PRL;
    // Норма основного (рабочего) комплекта аппаратуры объединения информации АПОИ
    bool mBasicDeviceUnioinQuota;
    // Авария 1-го комплекта обработки информации ВРЛ АПОИ
    bool mDamageProcessing1VRL;
    // Авария 2-го комплекта обработки информации ВРЛ АПОИ
    bool mDamageProcessing2VRL;
    // Авария 1-го комплекта аппаратуры обработки информации ПРЛ АПОИ
    bool mDamageProcessing1PRL;
    // Авария 2-го комплекта аппаратуры обработки информации ПРЛ АПОИ
    bool mDamageProcessing2PRL;
};

#endif // ACCORD_CODOGRAM_0_H
