#include "asterixerrors.h"

AsterixErrors::AsterixErrors(AsterixErrors::ErrorCode error) : mCode(error)
{}

AsterixErrors::AsterixErrors(const AsterixErrors &other) : mCode(other.mCode)
{}

bool AsterixErrors::operator== (const AsterixErrors &other) const
{
    return mCode == other.mCode;
}

bool AsterixErrors::operator== (const AsterixErrors::ErrorCode &otherCode) const
{
    return mCode == otherCode;
}

bool AsterixErrors::operator!= (const AsterixErrors &other) const
{
    return !operator ==(other);
}

bool AsterixErrors::operator!= (const AsterixErrors::ErrorCode &other) const
{
    return !operator ==(other);
}

AsterixErrors::operator QString() const
{
    return toString();
}

AsterixErrors::operator bool() const
{
    return mCode == OK;
}

AsterixErrors::operator ErrorCode() const
{
    return mCode;
}

AsterixErrors::ErrorCode AsterixErrors::code() const
{
    return mCode;
}

QString AsterixErrors::toString() const
{
    switch (mCode)
    {
    case OK:
        return tr("No errors");
    case Error_UnexpectedEndOfData:
        return tr("Unexpected end of data");
    case Error_MessageIsShort:
        return tr("Input array is too short");
    case Error_ProtocolNotSupported:
        return tr("This protocol is not suported");
    case Error_InvalidInput:
        return tr("Input parameter is invalid");
    case Error_InputCorrupted:
        return tr("Input array is corrupted");
    case Error_CRC:
        return tr("CRC check summ does not match the data buffer");
    case Error_FX:
        return tr("Extandable field is corrupted");
    case Error_REP:
        return tr("Repetition field has invalid size");
    case Error_FSPEC:
        return tr("Field Specification is corrupted");
    case Error_SPFieldInvalidLength:
        return tr("The special purpose field has invalid size");
    case Error_REFieldInvalidLength:
        return tr("The reserved expansion field has invalid size");
    case Error_Mandatory:
        return tr("A mandatory field is absent");
    }
    return tr("Unknown error");
}

AsterixErrorMandatory::AsterixErrorMandatory(quint8 category, QString field)
    : AsterixErrors(Error_Mandatory), mCategory(category), mField(field)
{}

QString AsterixErrorMandatory::toString() const
{
    return tr("%1: '%2' mandatory field is absent").arg(QString("Asterix%1").arg(mCategory, 3, 10, QChar('0'))).arg(mField);
}

QDebug operator<< (QDebug dbg, const AsterixErrors &c)
{
    dbg << c.toString();
    return dbg;
}
