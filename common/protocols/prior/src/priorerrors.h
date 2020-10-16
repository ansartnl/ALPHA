#ifndef PRIOR_ERRORS_H
#define PRIOR_ERRORS_H

#include "priorexport.h"

#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QCoreApplication>

class PRIOR_EXPORT PriorErrors
{
    Q_DECLARE_TR_FUNCTIONS(PriorErrors)
public:
    enum ErrorCode
    {
        OK = 0,
        Error_MessageIsShort,
        Error_UnexpectedEndOfData,
        Error_ProtocolNotSupported,
        Error_InvalidInput,
        Error_InputCorrupted,
        Error_FX,
        Error_REP,
        Error_FSPEC,
        Error_Mandatory
    };

    PriorErrors(PriorErrors::ErrorCode error);
    PriorErrors(const PriorErrors &other);

    bool operator== (const PriorErrors &other) const;
    bool operator== (const PriorErrors::ErrorCode &other) const;
    bool operator!= (const PriorErrors &other) const;
    bool operator!= (const PriorErrors::ErrorCode &other) const;
    operator QString() const;
    operator bool() const;
    operator ErrorCode() const;

    virtual QString toString() const;
    PriorErrors::ErrorCode code() const;

private:
    ErrorCode mCode;
};

class PRIOR_EXPORT PriorErrorMandatory : public PriorErrors
{
    Q_DECLARE_TR_FUNCTIONS(PriorErrorMandatory)
public:
    PriorErrorMandatory(quint8 category, QString field);
    virtual QString toString() const;

private:
    quint8 mCategory;
    QString mField;
};

QDebug PRIOR_EXPORT operator<< (QDebug dbg, const PriorErrors &c);

#endif // PRIOR_ERRORS_H
