#ifndef NOTAMMESSAGE_H
#define NOTAMMESSAGE_H

#include "AtsMessage.h"

/*!
 * \struct NotamStruct
 * \brief Структура Notam
 *
 * Данная структура содержит все поля телеграммы Notam.
 */

struct NotamStruct
{
    QString series;
    int number;
    int year;
    QString type;
    QString subject;
    QString condition;
    QString traffic;
    QString scope;
    int lowerLimit;
    int upperLimit;
    QString location;
    int radius;
    QString aerodrome;
    QDateTime validFrom;
    QDateTime validTill;
    QString validType;
    QString schedule;
    QString description;
    QString purpose;
};

inline QDataStream & operator<< (QDataStream &stream, const NotamStruct &msg)
{
    stream << msg.series;
    stream << msg.number;
    stream << msg.year;
    stream << msg.type;
    stream << msg.subject;
    stream << msg.condition;
    stream << msg.traffic;
    stream << msg.scope;
    stream << msg.lowerLimit;
    stream << msg.upperLimit;
    stream << msg.location;
    stream << msg.radius;
    stream << msg.aerodrome;
    stream << msg.validFrom;
    stream << msg.validTill;
    stream << msg.validType;
    stream << msg.schedule;
    stream << msg.description;
    stream << msg.purpose;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, NotamStruct &msg)
{
    stream >> msg.series;
    stream >> msg.number;
    stream >> msg.year;
    stream >> msg.type;
    stream >> msg.subject;
    stream >> msg.condition;
    stream >> msg.traffic;
    stream >> msg.scope;
    stream >> msg.lowerLimit;
    stream >> msg.upperLimit;
    stream >> msg.location;
    stream >> msg.radius;
    stream >> msg.aerodrome;
    stream >> msg.validFrom;
    stream >> msg.validTill;
    stream >> msg.validType;
    stream >> msg.schedule;
    stream >> msg.description;
    stream >> msg.purpose;
    return stream;
}

class NotamMessage : public AtsMessage
{
public:
    NotamMessage();
    NotamMessage(const NotamStruct &notam);

    virtual QByteArray toAftn() const;
    virtual QByteArray toJson() const;

    virtual LamMessage * lam() const;

    NotamStruct mNotam;

    inline friend QDataStream & operator<< (QDataStream &stream, const NotamMessage &msg)
    {
        stream << (const AtsMessage &)msg;
        stream << msg.mNotam;
        return stream;
    }

    inline friend QDataStream & operator>> (QDataStream &stream, NotamMessage &msg)
    {
        stream >> (AtsMessage &)msg;
        stream >> msg.mNotam;
        return stream;
    }
};

#endif // NOTAMMESSAGE_H
