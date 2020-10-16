#include "ArrMessage.h"

#include "LamMessage.h"

ArrMessage::ArrMessage()
{
    mInfo.type = Ats::ARR;
}

ArrMessage::ArrMessage(const ArrStruct &arr)
{
    mArr = arr;
    if (arr.version == ParseVersion::n_4444)
        mInfo.type = Ats::ARR;
    else if (arr.version == ParseVersion::n_TC95)
        mInfo.type = Ats::ARR1;
}

QByteArray ArrMessage::toAftn() const
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
    ret += mArr.ACID;
    if (!mArr.VORL.isEmpty())
        ret += "/" + mArr.VORL;
    ret += "-";
    ret += mArr.ADEP;
    if (!mArr.ADES.isEmpty())
    {
        ret += "-";
        ret += mArr.ADES;
    }
    ret += "\n";
    ret += "-";
    ret += mArr.arrivalAirport + mArr.arrivalTime.toString("hhmm");
    if ((mArr.arrivalAirport == "ZZZZ") || (mArr.arrivalAirport == "ЗЗЗЗ"))
        ret += " " + mArr.arrivalAirportName;
    ret += ")";
    return ret.toAscii();
}

QByteArray ArrMessage::toJson() const
{
    QByteArray ret;
    return ret;
}

LamMessage * ArrMessage::lam() const
{
    LamStruct lamStruct;
    return new LamMessage(lamStruct);
}
