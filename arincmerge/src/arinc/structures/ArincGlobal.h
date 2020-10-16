#ifndef ARINCGLOBAL_H
#define ARINCGLOBAL_H

namespace ArincGlobal
{

    enum TurnDirection
    {
        Unknown = ' ',
        Left = 'L',
        Right = 'R'
    };

    enum UnitIndicator
    {
        MSL = 'M',
        AGL = 'A'
    };

    enum Level
    {
        AllAltitudes = 'B',
        HighLevelAirways = 'H',
        LowLevelAirways = 'L'
    };

}

#endif // ARINCGLOBAL_H
