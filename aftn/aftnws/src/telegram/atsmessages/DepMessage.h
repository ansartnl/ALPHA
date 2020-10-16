#ifndef DEPMESSAGE_H
#define DEPMESSAGE_H

#include "AtsMessage.h"

/*!
 * \struct DepStruct
 * \brief Структура DEP
 *
 * Данная структура содержит все поля телеграммы DEP.
 */

struct DepStruct : public BaseAtsStruct
{
};

inline QDataStream & operator<< (QDataStream &stream, const DepStruct &msg)
{
    stream << (const BaseAtsStruct &)msg;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, DepStruct &msg)
{
    stream >> (BaseAtsStruct &)msg;
    return stream;
}

class DepMessage : public AtsMessage
{
public:
    DepMessage();
    DepMessage(const DepStruct &dep);

    virtual QByteArray toAftn() const;
    virtual QByteArray toJson() const;

    virtual LamMessage * lam() const;

    DepStruct mDep;

    inline friend QDataStream & operator<< (QDataStream &stream, const DepMessage &msg)
    {
        stream << (const AtsMessage &)msg;
        stream << msg.mDep;
        return stream;
    }

    inline friend QDataStream & operator>> (QDataStream &stream, DepMessage &msg)
    {
        stream >> (AtsMessage &)msg;
        stream >> msg.mDep;
        return stream;
    }
};

#endif // DEPMESSAGE_H
