#include "accorderrors.h"

AccordErrors::AccordErrors(AccordErrors::ErrorCode error) : code(error)
{}

AccordErrors::AccordErrors(const AccordErrors &other) : code(other.code)
{}

bool AccordErrors::operator== (const AccordErrors &other) const
{
    return code == other.code;
}

bool AccordErrors::operator== (const AccordErrors::ErrorCode &otherCode) const
{
    return code == otherCode;
}

bool AccordErrors::operator!= (const AccordErrors &other) const
{
    return !operator ==(other);
}

bool AccordErrors::operator!= (const AccordErrors::ErrorCode &other) const
{
    return !operator ==(other);
}

AccordErrors::operator QString() const
{
    return toString(code);
}

AccordErrors::operator bool() const
{
    return code == OK;
}

AccordErrors::operator ErrorCode() const
{
    return code;
}

QString AccordErrors::toString(AccordErrors::ErrorCode error)
{
    switch (error)
    {
    case OK:
        return tr("No errors");
    case Error_UnexpectedEndOfData:
        return tr("Unexpected end of data");
    case Error_MessageIsShort:
        return tr("Input array is too short");
    case Error_ProtocolNotSupported:
        return tr("This protocol is not suported");
    case Error_InvalidParser:
        return tr("Specified parser is not able to parse codogram");
    case Error_NullCodogram:
        return tr("Codogram is null");

    case Error_1_CP_IncorrectValue:
        return tr("%1: Incorrect value for field '%2'").arg("Accord.Codogram1").arg("PRL State (CP)");
    case Error_1_CB_IncorrectValue:
        return tr("%1: Incorrect value for field '%2'").arg("Accord.Codogram1").arg("VRL State (CB)");
    case Error_1_BI_IncorrectValue:
        return tr("%1: Incorrect value for field '%2'").arg("Accord.Codogram1").arg("Information Blanks (BI)");
    case Error_1_RK_IncorrectValue:
        return tr("%1: Incorrect value for field '%2'").arg("Accord.Codogram1").arg("Output mode of information to communication channels (RK)");
    case Error_1_MI_IncorrectValue:
        return tr("%1: Incorrect value for field '%2'").arg("Accord.Codogram1").arg("Meteinformation (MI)");
    case Error_1_Dmax_IncorrectValue:
        return tr("%1: Incorrect value for field '%2'").arg("Accord.Codogram1").arg("Maximum distance of Radar (Dmax)");
    case Error_1_VPK_IncorrectValue:
        return tr("%1: Incorrect value for field '%2'").arg("Accord.Codogram1").arg("Transfer Peleng on Channel (VPK)");
    case Error_1_ZV_IncorrectValue:
        return tr("%1: Incorrect value for field '%2'").arg("Accord.Codogram1").arg("Request modes of VRL (ZV)");
    case Error_1_CKP_IncorrectValue:
        return tr("%1: Incorrect value for field '%2'").arg("Accord.Codogram1").arg("Channel states of ARP (CKP)");
    case Error_1_PackageNumberDiscarded:
        return tr("%1: '%2' field is absent").arg("Accord.Codogram1").arg("Package Number Discarded");

    case Error_2_InformationType_IncorrectValue:
        return tr("%1: Incorrect value for field '%2'").arg("Accord.Codogram2").arg("Information Type");
    case Error_2_Prohibition_IncorrectValue:
        return tr("%1: Incorrect value for field '%2'").arg("Accord.Codogram2").arg("Prohibition");
    case Error_2_InformationSource_IncorrectValue:
        return tr("%1: Incorrect value for field '%2'").arg("Accord.Codogram2").arg("Information Source");
    case Error_2_BoardNumber_IncorrectValue:
        return tr("%1: Incorrect value for field '%2'").arg("Accord.Codogram2").arg("Board Number");

    case Error_3_InformationType_IncorrectValue:
        return tr("%1: Incorrect value for field '%2'").arg("Accord.Codogram3").arg("Information Type");
    case Error_3_Prohibition_IncorrectValue:
        return tr("%1: Incorrect value for field '%2'").arg("Accord.Codogram3").arg("Prohibition");
    case Error_3_InformationSource_IncorrectValue:
        return tr("%1: Incorrect value for field '%2'").arg("Accord.Codogram3").arg("InformationSource");

    case Error_5_Azimuth_IncorrectValue:
        return tr("%1: Incorrect value for field '%2'").arg("Accord.Codogram5").arg("Azimuth");

    case Error_6to9_Type_IncorrectValue:
        return tr("%1: Incorrect value for field '%2'").arg("Accord.Codogram6to9").arg("Type");

    case Error_11_Azimuth_IncorrectValue:
        return tr("%1: Incorrect value for field '%2'").arg("Accord.Codogram11").arg("Azimuth");
    case Error_11_ChannelNumber_IncorrectValue:
        return tr("%1: Incorrect value for field '%2'").arg("Accord.Codogram11").arg("Channel Number");
    }
    return tr("Unknown error");
}

QDebug operator<< (QDebug dbg, const AccordErrors &c)
{
    dbg << QString(c);
    return dbg;
}
