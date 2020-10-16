#include "UnknownMessage.h"

UnknownMessage::UnknownMessage()
{
    mInfo.type = Ats::Unknown;
}

QByteArray UnknownMessage::toAftn() const
{
    return QByteArray();
}

QByteArray UnknownMessage::toJson() const
{
    return QByteArray();
}

LamMessage * UnknownMessage::lam() const
{
    return 0;
}
