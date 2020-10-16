#include "priorerrors.h"

PriorErrors::PriorErrors(PriorErrors::ErrorCode error) : mCode(error)
{}

PriorErrors::PriorErrors(const PriorErrors &other) : mCode(other.mCode)
{}

bool PriorErrors::operator== (const PriorErrors &other) const
{
    return mCode == other.mCode;
}

bool PriorErrors::operator== (const PriorErrors::ErrorCode &otherCode) const
{
    return mCode == otherCode;
}

bool PriorErrors::operator!= (const PriorErrors &other) const
{
    return !operator ==(other);
}

bool PriorErrors::operator!= (const PriorErrors::ErrorCode &other) const
{
    return !operator ==(other);
}

PriorErrors::operator QString() const
{
    return toString();
}

PriorErrors::operator bool() const
{
    return mCode == OK;
}

PriorErrors::operator ErrorCode() const
{
    return mCode;
}

PriorErrors::ErrorCode PriorErrors::code() const
{
    return mCode;
}

QString PriorErrors::toString() const
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
    case Error_FX:
        return tr("Extandable field is corrupted");
    case Error_REP:
        return tr("Repetition field has invalid size");
    case Error_FSPEC:
        return tr("Field Specification is corrupted");
    case Error_Mandatory:
        return tr("A mandatory field is absent");
    }
    return tr("Unknown error");
}


PriorErrorMandatory::PriorErrorMandatory(quint8 category, QString field)
    : PriorErrors(Error_Mandatory), mCategory(category), mField(field)
{}

QString PriorErrorMandatory::toString() const
{
    return tr("%1: '%2' mandatory field is absent").arg(QString("Prior%1").arg(mCategory, 3, 10, QChar('0'))).arg(mField);
}


QDebug operator<< (QDebug dbg, const PriorErrors &c)
{
    dbg << c.toString();
    return dbg;
}
