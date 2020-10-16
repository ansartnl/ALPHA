#ifndef ACCORDERRORS_H
#define ACCORDERRORS_H

#include "accordexport.h"

#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QCoreApplication>

class ACCORD_EXPORT AccordErrors
{
    Q_DECLARE_TR_FUNCTIONS(AccordErrors)
public:
    enum ErrorCode
    {
        OK = 0,
        Error_MessageIsShort,
        Error_NullCodogram,
        Error_ProtocolNotSupported,
        Error_InvalidParser,
        Error_UnexpectedEndOfData,

        Error_1_CB_IncorrectValue,
        Error_1_CP_IncorrectValue,
        Error_1_BI_IncorrectValue,
        Error_1_RK_IncorrectValue,
        Error_1_MI_IncorrectValue,
        Error_1_Dmax_IncorrectValue,
        Error_1_VPK_IncorrectValue,
        Error_1_ZV_IncorrectValue,
        Error_1_CKP_IncorrectValue,
        Error_1_PackageNumberDiscarded,

        Error_2_InformationType_IncorrectValue,
        Error_2_Prohibition_IncorrectValue,
        Error_2_InformationSource_IncorrectValue,
        Error_2_BoardNumber_IncorrectValue,

        Error_3_InformationType_IncorrectValue,
        Error_3_Prohibition_IncorrectValue,
        Error_3_InformationSource_IncorrectValue,

        Error_5_Azimuth_IncorrectValue,

        Error_6to9_Type_IncorrectValue,

        Error_11_Azimuth_IncorrectValue,
        Error_11_ChannelNumber_IncorrectValue
    };

    AccordErrors(AccordErrors::ErrorCode error);
    AccordErrors(const AccordErrors &other);

    bool operator== (const AccordErrors &other) const;
    bool operator== (const AccordErrors::ErrorCode &other) const;
    bool operator!= (const AccordErrors &other) const;
    bool operator!= (const AccordErrors::ErrorCode &other) const;
    operator QString() const;
    operator bool() const;
    operator ErrorCode() const;

    inline QString toString() const { return toString(code); }
    static QString toString(AccordErrors::ErrorCode error);

private:
    ErrorCode code;
};

QDebug ACCORD_EXPORT operator<< (QDebug dbg, const AccordErrors &c);

#endif // ACCORDERRORS_H
