#include "ChgMessage.h"

#include "LamMessage.h"

ChgMessage::ChgMessage()
{
    mInfo.type = Ats::CHG;
}

ChgMessage::ChgMessage(const ChgStruct &chg)
{
    mChg = chg;
    if (chg.version == ParseVersion::n_4444)
        mInfo.type = Ats::CHG;
    else if (chg.version == ParseVersion::n_TC95)
        mInfo.type = Ats::CHG1;
}

QByteArray ChgMessage::toAftn() const
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
    ret += mChg.ACID;
    if (!mChg.VORL.isEmpty())
        ret += "/" + mChg.VORL;

    ret += "-";
    ret += mChg.ADEP;

    ret += "-";
    ret += mChg.ADES;

    if (mChg.changesFlags.testFlag(ChgStruct::CHANGE_ACID))
    {
        ret += "-";
        ret += "7/" + mChg.changedACID;
    }

    if (mChg.changesFlags.testFlag(ChgStruct::FLIGHT_RULES)
            || mChg.changesFlags.testFlag(ChgStruct::TYPE_OF_FLIGHT))
    {
        ret += "-";
        ret += "8/" + mChg.flightRules + mChg.typeOfFlight;
    }

    if (mChg.changesFlags.testFlag(ChgStruct::NUMBER_OF_AIRCRAFTS)
            || mChg.changesFlags.testFlag(ChgStruct::AIRCRAFT_TYPE)
            || mChg.changesFlags.testFlag(ChgStruct::TURBULENCE))
    {
        ret += "-";
        QString sTemp;
        if(mChg.numberOfAircrafts > 1)
            sTemp = QString::number(mChg.numberOfAircrafts);
        ret += QString("9/%1%2/%3").arg(sTemp).arg(mChg.aircraftType)
                .arg(mChg.turbulence);
    }

    if (mChg.changesFlags.testFlag(ChgStruct::EQUIPMENT))
    {
        ret += "-";
        ret += "10/" + mChg.equipment;
    }

    if (mChg.changesFlags.testFlag(ChgStruct::CHANGE_ADEP)
            || mChg.changesFlags.testFlag(ChgStruct::DEPARTURE_TIME))
    {
        ret += "-";
        ret += "13/" + mChg.changedADEP + removeColonFromTime(mChg.dtADEP);
    }

    if (mChg.changesFlags.testFlag(ChgStruct::SPEED)
            || mChg.changesFlags.testFlag(ChgStruct::FLIGHT_LEVEL)
            || mChg.changesFlags.testFlag(ChgStruct::ROUTE))
    {
        ret += "-";
        ret += "15/" + mChg.speed + mChg.flightLevel + " " + mChg.route;
    }

    if (mChg.changesFlags.testFlag(ChgStruct::CHANGE_ADES)
            || mChg.changesFlags.testFlag(ChgStruct::EET)
            || mChg.changesFlags.testFlag(ChgStruct::ALT1)
            || mChg.changesFlags.testFlag(ChgStruct::ALT2))
    {
        ret += "-";
        ret += "16/" + mChg.changedADES + removeColonFromTime(mChg.dtADES);
        if (!mChg.alt1.isEmpty())
            ret += " " + mChg.alt1;
        if (!mChg.alt2.isEmpty())
            ret += " " + mChg.alt2;
    }

    if (mChg.changesFlags.testFlag(ChgStruct::OTHER))
    {
        ret += "-";
        ret += "18/" + mChg.other;
    }

    ret += ")";
    return ret.toAscii();
}

QByteArray ChgMessage::toJson() const
{
    QByteArray ret;
    return ret;
}

LamMessage * ChgMessage::lam() const
{
    LamStruct lamStruct;
    return new LamMessage(lamStruct);
}
