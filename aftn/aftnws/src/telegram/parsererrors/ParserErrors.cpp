#include "ParserErrors.h"

#include <QtCore/QObject>

namespace ParserErrors
{

QString toString(AtsError e)
{
    switch (e.type)
    {
    case AtsError::AFTN:
        return toString(ParserErrors::AftnError(e.code));
    case AtsError::STANDARD:
        return toString(ParserErrors::StandardError(e.code));
    case AtsError::EXTENDED:
        return toString(ParserErrors::ExtendedError(e.code));
    }
    return QString();
}

QString toString(ParserErrors::StandardError e)
{
    switch (e)
    {
    case ParserErrors::Success:
        return QString();
    case ParserErrors::UnknownError:
        return QObject::tr("Unknown error");
    case ParserErrors::InvalidMessage:
        return QObject::tr("Invalid message");
    case ParserErrors::InvalidMessageFormat:
        return QObject::tr("Invalid message format");
    case ParserErrors::InvalidField3Format:
        return QObject::tr("Invalid field 3 format");
    case ParserErrors::InvalidField5Format:
        return QObject::tr("Invalid field 5 format");
    case ParserErrors::InvalidField7Format:
        return QObject::tr("Invalid field 7 format");
    case ParserErrors::InvalidField8Format:
        return QObject::tr("Invalid field 8 format");
    case ParserErrors::InvalidField9Format:
        return QObject::tr("Invalid field 9 format");
    case ParserErrors::InvalidField10Format:
        return QObject::tr("Invalid field 10 format");
    case ParserErrors::InvalidField13Format:
        return QObject::tr("Invalid field 13 format");
    case ParserErrors::InvalidField14Format:
        return QObject::tr("Invalid field 14 format");
    case ParserErrors::InvalidField15Format:
        return QObject::tr("Invalid field 15 format");
    case ParserErrors::InvalidField16Format:
        return QObject::tr("Invalid field 16 format");
    case ParserErrors::InvalidField17Format:
        return QObject::tr("Invalid field 17 format");
    case ParserErrors::InvalidField18Format:
        return QObject::tr("Invalid field 18 format");
    case ParserErrors::InvalidField19Format:
        return QObject::tr("Invalid field 19 format");
    case ParserErrors::InvalidField20Format:
        return QObject::tr("Invalid field 20 format");
    case ParserErrors::InvalidField21Format:
        return QObject::tr("Invalid field 21 format");
    case ParserErrors::InvalidField22Format:
        return QObject::tr("Invalid field 22 format");
    case ParserErrors::TooManyFields:
        return QObject::tr("Too many fields");
    case ParserErrors::InvalidFlightRules:
        return QObject::tr("Invalid flight rules");
    case ParserErrors::InvalidFlightType:
        return QObject::tr("Invalid flight type");
    case ParserErrors::InvalidTurbulenceCategory:
        return QObject::tr("Invalid turbulence category");
    case ParserErrors::InvalidRadioDevice:
        return QObject::tr("Invalid radio device");
    case ParserErrors::InvalidEquipment:
        return QObject::tr("Invalid equipment");
    case ParserErrors::InvalidBorderCrossTime:
        return QObject::tr("Invalid border cross time");
    case ParserErrors::InvalidBorderCrossTerm:
        return QObject::tr("Invalid border cross term");
    case ParserErrors::InvalidRoute:
        return QObject::tr("Invalid route");
    case ParserErrors::InvalidRouteItem:
        return QObject::tr("Invalid route item");
    case ParserErrors::InvalidRouteItemSequence:
        return QObject::tr("Invalid route item sequence");
    case ParserErrors::NeededRouteNotExists:
        return QObject::tr("Needed route does not exist");
    case ParserErrors::InvalidRoutePointPair:
        return QObject::tr("Invalid route point pair");
    case ParserErrors::InvalidElapsedTime:
        return QObject::tr("Invalid elapsed time");
    case ParserErrors::InvalidArrivalTime:
        return QObject::tr("Invalid arrival time");
    case ParserErrors::InvalidDateOfFLight:
        return QObject::tr("Invalid date of flight");
    case ParserErrors::InvalidFirEet:
        return QObject::tr("Invalid Fir Eet");
    case ParserErrors::InvalidDepartureTime:
        return QObject::tr("Invalid Departure Time");
    case ParserErrors::NotamTimeExpired:
        return QObject::tr("Notam time expired");
    case ParserErrors::NotamFieldQRequired:
        return QObject::tr("Notam field Q required");
    case ParserErrors::NotamFieldARequired:
        return QObject::tr("Notam field A required");
    case ParserErrors::NotamFieldBRequired:
        return QObject::tr("Notam field B required");
    case ParserErrors::NotamFieldCRequired:
        return QObject::tr("Notam field C required");
    case ParserErrors::NotamFieldDRequired:
        return QObject::tr("Notam field D required");
    case ParserErrors::NotamFieldERequired:
        return QObject::tr("Notam field E required");
    case InvalidACID:
        return QObject::tr("Invalid ACID");
    case InvalidAircraftType:
        return QObject::tr("Invalid aircraft type");
    case InvalidADEP:
        return QObject::tr("Invalid ADEP");
    case InvalidADES:
        return QObject::tr("Invalid ADES");
    case NoSuchFpl:
        return QObject::tr("No fpl, associated to this telegram");
    case NotOurAirport:
        return QObject::tr("Fpl with ADEP, ADES and alternatives - not our airport");
    case StatusError:
        return QObject::tr("Telegram breaks fpl status");
    case PlnNotFromZces:
        return QObject::tr("PLN not from ZCES");
    case PpnNotFromZces:
        return QObject::tr("PPN not from ZCES");
    }
    return QObject::tr("Unknown error");
}

QString toString(ParserErrors::AftnError e)
{
    switch (e)
    {
    case AftnNoError:
        return QObject::tr("No error");
    case AftnNoZCZC:
        return QObject::tr("No ZCZC");
    case AftnNoNNNN:
        return QObject::tr("No NNNN");
    case AftnErrorTransmissionIdentification:
        return QObject::tr("Error in transmission identification field");
    case AftnErrorPriorityIndicator:
        return QObject::tr("Error in priority indicator field");
    case AftnErrorReceiverAddress:
        return QObject::tr("Error in receiver address field");
    case AftnErrorFillingTime:
        return QObject::tr("Error in filling time field");
    case AftnErrorSenderAddress:
        return QObject::tr("Error in sender address field");
    }
    return QString();
}

QString toString(ParserErrors::ExtendedError e)
{
    switch (e)
    {
    case ExtendedNoError:
        return QObject::tr("No error");
    }
    return QString();
}

} // namespace ParserErrors
