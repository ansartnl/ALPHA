#ifndef CHGMESSAGE_H
#define CHGMESSAGE_H

#include "AtsMessage.h"

#include "enumstreaming.h"

/*!
 * \struct ChgStruct
 * \brief Структура CHG
 *
 * Данная структура содержит все поля телеграммы CHG.
 */

struct ChgStruct : public FplChange
{
    enum Change {
        FLIGHT_RULES = 0x00000001
        , TYPE_OF_FLIGHT = 0x00000002
        , NUMBER_OF_AIRCRAFTS = 0x00000004
        , AIRCRAFT_TYPE = 0x00000008
        , TURBULENCE = 0x00000010
        , EQUIPMENT = 0x00000020
        , SPEED = 0x00000040
        , FLIGHT_LEVEL = 0x00000080
        , ROUTE = 0x00000100
        , OTHER = 0x00000200
        , DEPARTURE_TIME = 0x00000400
        , ARRIVAL_TIME = 0x00000800
        , ALT1 = 0x00001000
        , ALT2 = 0x00002000
        , EET = 0x00004000
        , EX_FPL = 0x00008000
        , ENABLED = 0x00010000
        , CHANGE_ACID = 0x00020000
        , CHANGE_ADEP = 0x00040000
        , CHANGE_ADES = 0x00080000

        , ALL = FLIGHT_RULES | TYPE_OF_FLIGHT | NUMBER_OF_AIRCRAFTS | AIRCRAFT_TYPE | TURBULENCE
        | EQUIPMENT | SPEED | FLIGHT_LEVEL | ROUTE | OTHER | DEPARTURE_TIME | ARRIVAL_TIME
        | ALT1 | ALT2 | EET | EX_FPL | ENABLED | CHANGE_ACID | CHANGE_ADEP | CHANGE_ADES
    };
    typedef QFlags < Change > Changes;
    Changes changesFlags;
};
QT_DECLARE_ENUM_STREAMING(ChgStruct::Changes)

inline QDataStream & operator<< (QDataStream &stream, const ChgStruct &msg)
{
    stream << (const FplChange &)msg;
    stream << msg.changesFlags;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, ChgStruct &msg)
{
    stream >> (FplChange &)msg;
    stream >> msg.changesFlags;
    return stream;
}

class ChgMessage : public AtsMessage
{
public:
    ChgMessage();
    ChgMessage(const ChgStruct &chg);

    virtual QByteArray toAftn() const;
    virtual QByteArray toJson() const;

    virtual LamMessage * lam() const;

    ChgStruct mChg;

    inline friend QDataStream & operator<< (QDataStream &stream, const ChgMessage &msg)
    {
        stream << (const AtsMessage &)msg;
        stream << msg.mChg;
        return stream;
    }

    inline friend QDataStream & operator>> (QDataStream &stream, ChgMessage &msg)
    {
        stream >> (AtsMessage &)msg;
        stream >> msg.mChg;
        return stream;
    }
};

#endif // CHGMESSAGE_H
