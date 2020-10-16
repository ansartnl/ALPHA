#include "PpnMessage.h"

#include "LamMessage.h"

PpnMessage::PpnMessage()
{
    mInfo.type = Ats::PPN1;
}

PpnMessage::PpnMessage(const PpnStruct &ppn)
{
    mPpn = ppn;
    mInfo.type = Ats::PPN1;
}

QByteArray PpnMessage::toAftn() const
{
    QByteArray ret;
    return ret;
}

QByteArray PpnMessage::toJson() const
{
    QByteArray ret;
    return ret;
}

LamMessage * PpnMessage::lam() const
{
    LamStruct lamStruct;
    return new LamMessage(lamStruct);
}
