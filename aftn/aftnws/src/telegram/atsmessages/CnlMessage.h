#ifndef CNLMESSAGE_H
#define CNLMESSAGE_H

#include "AtsMessage.h"

/*!
 * \struct CnlStruct
 * \brief Структура CNL
 *
 * Данная структура содержит все поля телеграммы CNL.
 */

struct CnlStruct : public BaseAtsStruct
{
};

inline QDataStream & operator<< (QDataStream &stream, const CnlStruct &msg)
{
    stream << (const BaseAtsStruct &)msg;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, CnlStruct &msg)
{
    stream >> (BaseAtsStruct &)msg;
    return stream;
}

class CnlMessage : public AtsMessage
{
public:
    CnlMessage();
    CnlMessage(const CnlStruct &cnl);

    virtual QByteArray toAftn() const;
    virtual QByteArray toJson() const;

    virtual LamMessage * lam() const;

    CnlStruct mCnl;

    inline friend QDataStream & operator<< (QDataStream &stream, const CnlMessage &msg)
    {
        stream << (const AtsMessage &)msg;
        stream << msg.mCnl;
        return stream;
    }

    inline friend QDataStream & operator>> (QDataStream &stream, CnlMessage &msg)
    {
        stream >> (AtsMessage &)msg;
        stream >> msg.mCnl;
        return stream;
    }
};

#endif // CNLMESSAGE_H
