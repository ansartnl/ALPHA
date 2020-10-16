#include "AftnMessage.h"

#include "ParserErrors.h"

AftnMessage::AftnMessage(const AftnStruct &aftn)
{
    mAftn = aftn;
}

QByteArray AftnMessage::toByteArray() const
{
    QByteArray ret;
    if (mAftn.version == ParseVersion::n_4444)
        ret += "ZCZC ";
    else if (mAftn.version == ParseVersion::n_TC95)
        ret += "ЗЦЗЦ ";
    else
        return "Unknown version";
    ret += mAftn.transmissionIdentificator.toAscii();
    if (!mAftn.additionServiceIndication.isEmpty())
        ret += " " + mAftn.additionServiceIndication.toAscii();
    ret += "     ";
    ret += "\r\n";
    ret += mAftn.priorityIndicator;
    foreach (const QString &receiver, mAftn.receiverAddresses)
    {
        ret += " ";
        ret += receiver.toAscii();
    }
    ret += "\r\n";
    ret += QString("%1%2%3 ").arg(mAftn.fillingDate, 2, 10, QChar('0'))
            .arg(mAftn.fillingHour, 2, 10, QChar('0'))
            .arg(mAftn.fillingMinute, 2, 10, QChar('0'))
            .toAscii();
    ret += mAftn.senderAddress.toAscii();
    ret += "\r\n";
    // TODO: beginning of the text
    ret += mAftn.text.toAscii();
//    if (!mAftn.confirmation.isEmpty())
//    {
//        ret += "\r\n";
//        ret += "CFM" + mAftn.confirmation.toAscii();
//    }
//    if (!mAftn.correction.isEmpty())
//    {
//        ret += "\r\n";
//        ret += "COR" + mAftn.correction.toAscii();
//    }
    ret += "\r\n";

    // seven line feeds
    ret += "\n\n\n\n\n\n\n";
    if (mAftn.version == ParseVersion::n_4444)
        ret += "NNNN";
    else if (mAftn.version == ParseVersion::n_TC95)
        ret += "НННН";

    return ret;
}
