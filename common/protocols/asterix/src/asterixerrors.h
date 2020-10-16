#ifndef ASTERIXERRORS_H
#define ASTERIXERRORS_H

#include "asterixexport.h"

#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QCoreApplication>

class ASTERIX_EXPORT AsterixErrors
{
    Q_DECLARE_TR_FUNCTIONS(AsterixErrors)
public:
    enum ErrorCode
    {
        OK = 0,
        Error_MessageIsShort,
        Error_UnexpectedEndOfData,
        Error_ProtocolNotSupported,
        Error_InvalidInput,
        Error_InputCorrupted,
        Error_CRC,
        Error_FX,
        Error_REP,
        Error_FSPEC,
        Error_SPFieldInvalidLength,
        Error_REFieldInvalidLength,
        Error_Mandatory
    };

    AsterixErrors(AsterixErrors::ErrorCode error);
    AsterixErrors(const AsterixErrors &other);
    virtual ~AsterixErrors() {}

    bool operator== (const AsterixErrors &other) const;
    bool operator== (const AsterixErrors::ErrorCode &other) const;
    bool operator!= (const AsterixErrors &other) const;
    bool operator!= (const AsterixErrors::ErrorCode &other) const;
    operator QString() const;
    operator bool() const;
    operator ErrorCode() const;

    virtual QString toString() const;
    AsterixErrors::ErrorCode code() const;

private:
    ErrorCode mCode;
};

class ASTERIX_EXPORT AsterixErrorMandatory : public AsterixErrors
{
    Q_DECLARE_TR_FUNCTIONS(AsterixErrorMandatory)
public:
    AsterixErrorMandatory(quint8 category, QString field);
    virtual QString toString() const;

private:
    quint8 mCategory;
    QString mField;
};

QDebug ASTERIX_EXPORT operator<< (QDebug dbg, const AsterixErrors &c);

#endif // ASTERIXERRORS_H
