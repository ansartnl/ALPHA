#ifndef PPNMESSAGE_H
#define PPNMESSAGE_H

#include "AtsMessage.h"

/*!
 * \struct PpnStruct
 * \brief Структура PPN
 *
 * Данная структура содержит все поля телеграммы LAM.
 */

struct PpnStruct
{
    QString ACID;
    QString VORL;

    QString other;

    QDate dof;
};

inline QDataStream & operator<< (QDataStream &stream, const PpnStruct &msg)
{
    stream << msg.ACID;
    stream << msg.VORL;
    stream << msg.other;
    stream << msg.dof;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, PpnStruct &msg)
{
    stream >> msg.ACID;
    stream >> msg.VORL;
    stream >> msg.other;
    stream >> msg.dof;
    return stream;
}

class PpnMessage : public AtsMessage
{
public:
    PpnMessage();
    PpnMessage(const PpnStruct &ppn);

    virtual QByteArray toAftn() const;
    virtual QByteArray toJson() const;

    virtual LamMessage * lam() const;

    PpnStruct mPpn;

    inline friend QDataStream & operator<< (QDataStream &stream, const PpnMessage &msg)
    {
        stream << (const AtsMessage &)msg;
        stream << msg.mPpn;
        return stream;
    }

    inline friend QDataStream & operator>> (QDataStream &stream, PpnMessage &msg)
    {
        stream >> (AtsMessage &)msg;
        stream >> msg.mPpn;
        return stream;
    }
};

#endif // PPNMESSAGE_H
