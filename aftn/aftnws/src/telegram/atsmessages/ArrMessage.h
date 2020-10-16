#ifndef ARRMESSAGE_H
#define ARRMESSAGE_H

#include "AtsMessage.h"

/*!
 * \struct ArrStruct
 * \brief Структура ARR
 *
 * Данная структура содержит все поля телеграммы ARR.
 */

struct ArrStruct : public BaseAtsStruct
{
    QString arrivalAirport;
    QString arrivalAirportName;
};

inline QDataStream & operator<< (QDataStream &stream, const ArrStruct &msg)
{
    stream << (const BaseAtsStruct &)msg;
    stream << msg.arrivalAirport;
    stream << msg.arrivalAirportName;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, ArrStruct &msg)
{
    stream >> (BaseAtsStruct &)msg;
    stream >> msg.arrivalAirport;
    stream >> msg.arrivalAirportName;
    return stream;
}

class ArrMessage : public AtsMessage
{
public:
    ArrMessage();
    ArrMessage(const ArrStruct &arr);

    virtual QByteArray toAftn() const;
    virtual QByteArray toJson() const;

    virtual LamMessage * lam() const;

    ArrStruct mArr;

    inline friend QDataStream & operator<< (QDataStream &stream, const ArrMessage &msg)
    {
        stream << (const AtsMessage &)msg;
        stream << msg.mArr;
        return stream;
    }

    inline friend QDataStream & operator>> (QDataStream &stream, ArrMessage &msg)
    {
        stream >> (AtsMessage &)msg;
        stream >> msg.mArr;
        return stream;
    }
};

#endif // ARRMESSAGE_H
