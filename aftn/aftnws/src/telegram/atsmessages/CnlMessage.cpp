#include "CnlMessage.h"

#include "LamMessage.h"

CnlMessage::CnlMessage()
{
    mInfo.type = Ats::CNL;
}

CnlMessage::CnlMessage(const CnlStruct &cnl)
{
    mCnl = cnl;
    if (cnl.version == ParseVersion::n_4444)
        mInfo.type = Ats::CNL;
    else if (cnl.version == ParseVersion::n_TC95)
        mInfo.type = Ats::CNL1;
}

QByteArray CnlMessage::toAftn() const
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
    ret += mCnl.ACID;
    if (!mCnl.VORL.isEmpty())
        ret += "/" + mCnl.VORL;
    ret += "-";
    ret += mCnl.ADEP;
    if (!mCnl.ADES.isEmpty())
    {
        ret += "-";
        ret += mCnl.ADES;
    }
    ret += ")";
    return ret.toAscii();
}

QByteArray CnlMessage::toJson() const
{
    QByteArray ret;
    return ret;
}

LamMessage * CnlMessage::lam() const
{
    LamStruct lamStruct;
    return new LamMessage(lamStruct);
}
