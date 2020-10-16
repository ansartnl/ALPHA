#include "FplMessage.h"

#include "LamMessage.h"

#include "ArrMessage.h"
#include "ChgMessage.h"
#include "CnlMessage.h"
#include "DepMessage.h"
#include "DlaMessage.h"

FplMessage::FplMessage()
{
    mInfo.type = Ats::FPL;
}

FplMessage::FplMessage(const FplStruct &fpl)
{
    mFpl = fpl;
    if (fpl.version == ParseVersion::n_4444)
        mInfo.type = Ats::FPL;
    else if (fpl.version == ParseVersion::n_TC95)
        mInfo.type = Ats::FPL1;
}

QByteArray FplMessage::toAftn() const
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
    ret += mFpl.ACID;
    if (!mFpl.VORL.isEmpty())
        ret += "/" + mFpl.VORL;

    ret += "-";
    ret += mFpl.flightRules + mFpl.typeOfFlight;

    ret += "\n";
    ret += "-";
    QString sTemp;
    if(mFpl.numberOfAircrafts > 1)
        sTemp = QString::number(mFpl.numberOfAircrafts);
    ret += QString("%1%2/%3").arg(sTemp).arg(mFpl.aircraftType).arg(mFpl.turbulence);

    ret += "-";
    ret += mFpl.equipment;

    ret += "\n";
    ret += "-";
    ret += mFpl.ADEP + removeColonFromTime(mFpl.dtADEP);

    ret += "\n";
    ret += "-";
    ret += mFpl.speed + mFpl.flightLevel + " " + mFpl.route;

    ret += "\n";
    ret += "-";
    ret += mFpl.ADES + removeColonFromTime(mFpl.dtADES);
    if (!mFpl.alt1.isEmpty())
        ret += " " + mFpl.alt1;
    if (!mFpl.alt2.isEmpty())
        ret += " " + mFpl.alt2;

    ret += "\n";
    ret += "-";
    ret += mFpl.other;

    ret += ")";
    return ret.toAscii();
}

QByteArray FplMessage::toJson() const
{
    QByteArray ret;
    return ret;
}

LamMessage * FplMessage::lam() const
{
    LamStruct lamStruct;
    return new LamMessage(lamStruct);
}

ArrMessage * FplMessage::arr() const
{
    ArrMessage *ret = new ArrMessage(arrStruct());
    if (mFpl.version == ParseVersion::n_TC95)
    {
        ret->mInfo.aftnPriority = "ФФ";
        ret->mInfo.type = "АРР";
    } else
    {
        ret->mInfo.aftnPriority = "FF";
        ret->mInfo.type = "ARR";
    }
    return ret;
}

ChgMessage * FplMessage::chg() const
{
    ChgMessage *ret = new ChgMessage(chgStruct());
    if (mFpl.version == ParseVersion::n_TC95)
    {
        ret->mInfo.aftnPriority = "ФФ";
        ret->mInfo.type = "ЦХГ";
    } else
    {
        ret->mInfo.aftnPriority = "FF";
        ret->mInfo.type = "CHG";
    }
    return ret;
}

CnlMessage * FplMessage::cnl() const
{
    CnlMessage *ret = new CnlMessage(cnlStruct());
    if (mFpl.version == ParseVersion::n_TC95)
    {
        ret->mInfo.aftnPriority = "ФФ";
        ret->mInfo.type = "ЦНЛ";
    } else
    {
        ret->mInfo.aftnPriority = "FF";
        ret->mInfo.type = "CNL";
    }
    return ret;
}

DepMessage * FplMessage::dep() const
{
    DepMessage *ret = new DepMessage(depStruct());
    if (mFpl.version == ParseVersion::n_TC95)
    {
        ret->mInfo.aftnPriority = "ФФ";
        ret->mInfo.type = "ДЕП";
    } else
    {
        ret->mInfo.aftnPriority = "FF";
        ret->mInfo.type = "DEP";
    }
    return ret;
}

DlaMessage * FplMessage::dla() const
{
    DlaMessage *ret = new DlaMessage(dlaStruct());
    if (mFpl.version == ParseVersion::n_TC95)
    {
        ret->mInfo.aftnPriority = "ФФ";
        ret->mInfo.type = "ДЛА";
    } else
    {
        ret->mInfo.aftnPriority = "FF";
        ret->mInfo.type = "DLA";
    }
    return ret;
}

ArrStruct FplMessage::arrStruct() const
{
    ArrStruct s;
    s.version = mFpl.version;
    s.ACID = mFpl.ACID;
    s.VORL = mFpl.VORL;
    s.ADEP = mFpl.ADEP;
    s.dtADEP = mFpl.dtADEP;
    s.arrivalAirport = mFpl.ADES;
    s.arrivalTime = mFpl.arrivalTime;
    return s;
}

ChgStruct FplMessage::chgStruct() const
{
    ChgStruct s;
    s.version = mFpl.version;
    s.ACID = mFpl.ACID;
    s.VORL = mFpl.VORL;
    s.ADEP = mFpl.ADEP;
    s.dtADEP = mFpl.dtADEP;
    s.ADES = mFpl.ADES;
    s.dtADES = mFpl.dtADES;
    return s;
}

CnlStruct FplMessage::cnlStruct() const
{
    CnlStruct s;
    s.version = mFpl.version;
    s.ACID = mFpl.ACID;
    s.VORL = mFpl.VORL;
    s.ADEP = mFpl.ADEP;
    s.dtADEP = mFpl.dtADEP;
    s.ADES = mFpl.ADES;
    s.dtADES = mFpl.dtADES;
    return s;
}

DepStruct FplMessage::depStruct() const
{
    DepStruct s;
    s.version = mFpl.version;
    s.ACID = mFpl.ACID;
    s.VORL = mFpl.VORL;
    s.ADEP = mFpl.ADEP;
    s.dtADEP = mFpl.dtADEP;
    s.ADES = mFpl.ADES;
    s.dtADES = mFpl.dtADES;
    return s;
}

DlaStruct FplMessage::dlaStruct() const
{
    DlaStruct s;
    s.version = mFpl.version;
    s.ACID = mFpl.ACID;
    s.VORL = mFpl.VORL;
    s.ADEP = mFpl.ADEP;
    s.dtADEP = mFpl.dtADEP;
    s.ADES = mFpl.ADES;
    s.dtADES = mFpl.dtADES;
    s.departureTime = mFpl.departureTime;
    s.arrivalTime = mFpl.arrivalTime;
    return s;
}
