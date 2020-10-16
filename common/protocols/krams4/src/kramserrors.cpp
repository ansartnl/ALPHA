#include "kramserrors.h"

KramsError::KramsError(KramsError::ErrorCode error) : mCode(error)
{}

KramsError::KramsError(const KramsError &other) : mCode(other.mCode)
{}

bool KramsError::operator== (const KramsError &other) const
{
    return mCode == other.mCode;
}

bool KramsError::operator== (const KramsError::ErrorCode &otherCode) const
{
    return mCode == otherCode;
}

bool KramsError::operator!= (const KramsError &other) const
{
    return !operator ==(other);
}

bool KramsError::operator!= (const KramsError::ErrorCode &other) const
{
    return !operator ==(other);
}

KramsError::operator QString() const
{
    return toString();
}

KramsError::operator bool() const
{
    return mCode == OK;
}

KramsError::operator ErrorCode() const
{
    return mCode;
}

KramsError::ErrorCode KramsError::code() const
{
    return mCode;
}

QString KramsError::toString() const
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
        return tr("CRC check summ does not match");
    case Error_NullMessage:
        return tr("Message is null");
    case Error_Message:
        return tr("Message error");
    }
    return tr("Unknown error");
}


KramsErrorMessage::KramsErrorMessage(QString text)
    : KramsError(Error_Message), mText(text)
{}

KramsErrorMessage::KramsErrorMessage(KramsError::ErrorCode error, QString text)
    : KramsError(error), mText(text)
{}

QString KramsErrorMessage::toString() const
{
    return mText;
}


QDebug operator<< (QDebug dbg, const KramsError &c)
{
    dbg << c.toString();
    return dbg;
}
