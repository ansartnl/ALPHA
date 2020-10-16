#include "LamMessage.h"

LamMessage::LamMessage(const LamStruct &lam)
{
    mLam = lam;
    mInfo.type = Ats::LAM;
}

QByteArray LamMessage::toAftn() const
{
    QString ret;
    return ret.toAscii();
}

QByteArray LamMessage::toJson() const
{
    QByteArray ret;
    return ret;
}

LamMessage * LamMessage::lam() const
{
    LamStruct lamStruct;
    return new LamMessage(lamStruct);
}
