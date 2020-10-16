#include "MoxaSettingsCommon.h"

#include <QtCore/QStringList>

const QStringList endLines()
{
    static QStringList m = QStringList()
            << "Key in your selection:"
               ;
    return m;
}

static SettingInfoMap mInfoMap;

const SettingInfo baudRateInfo(
    "312"
    , "qqsy"
    , "\033qqq"
    , "Baud rate \\((\\d+)\\)"
    , 1);

const SettingInfoMap infoMap(bool init)
{
    if (init)
    {
        mInfoMap.clear();
        mInfoMap.insert(Field::F_BAUD_RATE, baudRateInfo);
    }
    return mInfoMap;
}

const QChar baudRateIndex(int value)
{
    if (value <= 50)
        return '0';
    if (value <= 75)
        return '1';
    if (value <= 110)
        return '2';
    if (value <= 134)
        return '3';
    if (value <= 150)
        return '4';
    if (value <= 300)
        return '5';
    if (value <= 600)
        return '6';
    if (value <= 1200)
        return '7';
    if (value <= 1800)
        return '8';
    if (value <= 2400)
        return '9';
    if (value <= 4800)
        return 'a';
    if (value <= 7200)
        return 'b';
    if (value <= 9600)
        return 'c';
    if (value <= 19200)
        return 'd';
    if (value <= 38400)
        return 'e';
    if (value <= 57600)
        return 'f';
    if (value <= 115200)
        return 'g';
    return 'h'; // 230400
}
