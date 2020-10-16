#ifndef DEFINES_H
#define DEFINES_H

#include "enumstreaming.h"

/*!
 * \struct Defines
 * \brief Структура объявлений
 *
 * Фиктивная структура, содержащая общие типпы данных.
 */

struct Defines
{
    enum SourceOfRequest {
        MAIN_WINDOW
        , FPL_WINDOW
        , TELEGRAM_WINDOW
        , RELATED_TELEGRAMS
        , ADDRESSES
        , AFTN_WIDGET
        , CREATE_TELEGRAM_DEP
        , CREATE_TELEGRAM_DES
    };

    enum TelegramType {
        NO = 0x0000
        , FPL = 0x0001
        , ARR = 0x0002
        , CHG = 0x0004
        , CNL = 0x0008
        , DEP = 0x0010
        , DLA = 0x0020

        , FREE_TEXT = 0x0040

        , ATS = FPL | ARR | CHG | CNL | DEP | DLA
    };
    typedef QFlags < TelegramType > TelegramTypes;
};

QT_DECLARE_ENUM_STREAMING(Defines::SourceOfRequest)
QT_DECLARE_ENUM_STREAMING(Defines::TelegramType)
QT_DECLARE_ENUM_STREAMING(Defines::TelegramTypes)

#endif // DEFINES_H
