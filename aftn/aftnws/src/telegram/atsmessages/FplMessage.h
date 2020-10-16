#ifndef FPLMESSAGE_H
#define FPLMESSAGE_H

#include "AtsMessage.h"

/*!
 * \struct FplStruct
 * \brief Структура FPL
 *
 * Данная структура содержит все поля телеграммы FPL.
 */

struct FplStruct : public FplChange
{
};

inline QDataStream & operator<< (QDataStream &stream, const FplStruct &msg)
{
    stream << (const FplChange &)msg;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, FplStruct &msg)
{
    stream >> (FplChange &)msg;
    return stream;
}

class ArrMessage;
class ChgMessage;
class CnlMessage;
class DepMessage;
class DlaMessage;

struct ArrStruct;
struct ChgStruct;
struct CnlStruct;
struct DepStruct;
struct DlaStruct;

class FplMessage : public AtsMessage
{
public:
    FplMessage();
    FplMessage(const FplStruct &fpl);

    virtual QByteArray toAftn() const;
    virtual QByteArray toJson() const;

    virtual LamMessage * lam() const;

    ArrMessage * arr() const;
    ChgMessage * chg() const;
    CnlMessage * cnl() const;
    DepMessage * dep() const;
    DlaMessage * dla() const;

    ArrStruct arrStruct() const;
    ChgStruct chgStruct() const;
    CnlStruct cnlStruct() const;
    DepStruct depStruct() const;
    DlaStruct dlaStruct() const;

    FplStruct mFpl;

    inline friend QDataStream & operator<< (QDataStream &stream, const FplMessage &msg)
    {
        stream << (const AtsMessage &)msg;
        stream << msg.mFpl;
        return stream;
    }

    inline friend QDataStream & operator>> (QDataStream &stream, FplMessage &msg)
    {
        stream >> (AtsMessage &)msg;
        stream >> msg.mFpl;
        return stream;
    }
};

#endif // FPLMESSAGE_H
