#include "DepMessage.h"

#include "LamMessage.h"

DepMessage::DepMessage()
{
    mInfo.type = Ats::DEP;
}

DepMessage::DepMessage(const DepStruct &dep)
{
    mDep = dep;
    if (dep.version == ParseVersion::n_4444)
        mInfo.type = Ats::DEP;
    else if (dep.version == ParseVersion::n_TC95)
        mInfo.type = Ats::DEP1;
}

QByteArray DepMessage::toAftn() const
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
    ret += mDep.ACID;
    if (!mDep.VORL.isEmpty())
        ret += "/" + mDep.VORL;
    ret += "-";
    ret += mDep.ADEP + removeColonFromTime(mDep.dtADEP);
    if (!mDep.ADES.isEmpty())
    {
        ret += "-";
        ret += mDep.ADES;
    }
    ret += ")";
    return ret.toAscii();
}

QByteArray DepMessage::toJson() const
{
    QByteArray ret;
    return ret;
}

LamMessage * DepMessage::lam() const
{
    LamStruct lamStruct;
    return new LamMessage(lamStruct);
}
