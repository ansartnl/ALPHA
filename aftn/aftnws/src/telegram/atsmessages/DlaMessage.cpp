#include "DlaMessage.h"

#include "LamMessage.h"

DlaMessage::DlaMessage()
{
    mInfo.type = Ats::DLA;
}

DlaMessage::DlaMessage(const DlaStruct &dla)
{
    mDla = dla;
    if (dla.version == ParseVersion::n_4444)
        mInfo.type = Ats::DLA;
    else if (dla.version == ParseVersion::n_TC95)
        mInfo.type = Ats::DLA1;
}

QByteArray DlaMessage::toAftn() const
{
    QString ret;
    ret += "(";
    ret += mInfo.type;
    if (!mInfo.sender.isEmpty() && !mInfo.receiver.isEmpty())
        ret += QString("%1/%2%3").arg(mInfo.sender).arg(mInfo.receiver)
                .arg(mInfo.number, 3, 10, QChar('0'));
    if (!mInfo.firstSender.isEmpty() && !mInfo.firstReceiver.isEmpty())
        ret += QString("%1/%2%3").arg(mInfo.firstSender).arg(mInfo.firstReceiver)
                .arg(mInfo.firstNumber, 3, 10, QChar('0'));
    ret += "-";
    ret += mDla.ACID;
    if (!mDla.VORL.isEmpty())
        ret += "/" + mDla.VORL;
    ret += "-";
    ret += mDla.ADEP + removeColonFromTime(mDla.dtADEP);
    if (!mDla.ADES.isEmpty())
    {
        ret += "-";
        ret += mDla.ADES;
    }
    ret += ")";
    return ret.toAscii();
}

QByteArray DlaMessage::toJson() const
{
    QByteArray ret;
    return ret;
}

LamMessage * DlaMessage::lam() const
{
    LamStruct lamStruct;
    return new LamMessage(lamStruct);
}
