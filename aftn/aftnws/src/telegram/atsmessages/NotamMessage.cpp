#include "NotamMessage.h"

#include "LamMessage.h"

NotamMessage::NotamMessage()
{
    mInfo.type = Ats::NOTAM;
}

NotamMessage::NotamMessage(const NotamStruct &notam)
{
    mNotam = notam;
    mInfo.type = Ats::NOTAM;
}

QByteArray NotamMessage::toAftn() const
{
    QByteArray ret;
    return ret;
}

QByteArray NotamMessage::toJson() const
{
    QByteArray ret;
    return ret;
}

LamMessage * NotamMessage::lam() const
{
    LamStruct lamStruct;
    return new LamMessage(lamStruct);
}
