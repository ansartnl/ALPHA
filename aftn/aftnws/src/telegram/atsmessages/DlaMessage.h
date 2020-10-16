#ifndef DLAMESSAGE_H
#define DLAMESSAGE_H

#include "AtsMessage.h"

/*!
 * \struct DlaStruct
 * \brief Структура DLA
 *
 * Данная структура содержит все поля телеграммы DLA.
 */

struct DlaStruct : public BaseAtsStruct
{
};

inline QDataStream & operator<< (QDataStream &stream, const DlaStruct &msg)
{
    stream << (const BaseAtsStruct &)msg;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, DlaStruct &msg)
{
    stream >> (BaseAtsStruct &)msg;
    return stream;
}

class DlaMessage : public AtsMessage
{
public:
    DlaMessage();
    DlaMessage(const DlaStruct &dla);

    virtual QByteArray toAftn() const;
    virtual QByteArray toJson() const;

    virtual LamMessage * lam() const;

    DlaStruct mDla;

    inline friend QDataStream & operator<< (QDataStream &stream, const DlaMessage &msg)
    {
        stream << (const AtsMessage &)msg;
        stream << msg.mDla;
        return stream;
    }

    inline friend QDataStream & operator>> (QDataStream &stream, DlaMessage &msg)
    {
        stream >> (AtsMessage &)msg;
        stream >> msg.mDla;
        return stream;
    }
};

#endif // DLAMESSAGE_H
