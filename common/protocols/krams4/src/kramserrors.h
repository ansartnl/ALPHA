#ifndef KRAMS_ERRORS_H
#define KRAMS_ERRORS_H

#include "kramsexport.h"

#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QCoreApplication>
#include <QtCore/QSharedPointer>


class KRAMS_EXPORT KramsError
{
    Q_DECLARE_TR_FUNCTIONS(KramsError)
public:
    enum ErrorCode
    {
        OK = 0,
        Error_MessageIsShort,
        Error_UnexpectedEndOfData,
        Error_ProtocolNotSupported,
        Error_InvalidInput,
        Error_InputCorrupted,
        Error_NullMessage,
        Error_Message,
        Error_CRC
    };

    KramsError(KramsError::ErrorCode error);
    KramsError(const KramsError &other);

    bool operator== (const KramsError &other) const;
    bool operator== (const KramsError::ErrorCode &other) const;
    bool operator!= (const KramsError &other) const;
    bool operator!= (const KramsError::ErrorCode &other) const;
    operator QString() const;
    operator bool() const;
    operator ErrorCode() const;

    virtual QString toString() const;
    KramsError::ErrorCode code() const;

private:
    ErrorCode mCode;
};

typedef QSharedPointer<KramsError>      TKramsError;

class KRAMS_EXPORT KramsErrorMessage : public KramsError
{
public:
    KramsErrorMessage(QString text);
    KramsErrorMessage(KramsError::ErrorCode error, QString text);
    virtual QString toString() const;

private:
    QString     mText;
};

QDebug KRAMS_EXPORT operator<< (QDebug dbg, const KramsError &c);

#endif // KRAMS_ERRORS_H
