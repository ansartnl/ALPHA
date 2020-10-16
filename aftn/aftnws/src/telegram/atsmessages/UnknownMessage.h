#ifndef UNKNOWNMESSAGE_H
#define UNKNOWNMESSAGE_H

#include "AtsMessage.h"

class UnknownMessage : public AtsMessage
{
public:
    UnknownMessage();

    virtual QByteArray toAftn() const;
    virtual QByteArray toJson() const;

    virtual LamMessage * lam() const;

    inline friend QDataStream & operator<< (QDataStream &stream, const UnknownMessage &msg)
    {
        stream << (const AtsMessage &)msg;
        return stream;
    }

    inline friend QDataStream & operator>> (QDataStream &stream, UnknownMessage &msg)
    {
        stream >> (AtsMessage &)msg;
        return stream;
    }
};

#endif // UNKNOWNMESSAGE_H
