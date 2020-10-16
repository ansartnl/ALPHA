#ifndef PARSERERRORS_H
#define PARSERERRORS_H

#include "enumstreaming.h"

#include <QtCore/QString>

/*!
 * \struct AtsError
 * \brief Структура ошибки разбора
 */

struct AtsError
{
    /*!
     * \brief Тип ошибки
     *
     * Возможные типы:
     * - ошибка при разборе АФТН сообщения
     * - ошибка при разборе стандартной ОВД телеграммы
     * - ошибка при разборе нестрандартонй ОВД телеграммы
     */
    enum Type {AFTN = 0, STANDARD = 1, EXTENDED = 2} type;
    /*!
     * \brief Код ошибки
     */
    int code;
    AtsError()
    {
        type = AFTN;
        code = 0;
    }
    AtsError(Type t, int c)
    {
        type = t;
        code = c;
    }
};

QT_DECLARE_ENUM_STREAMING(AtsError::Type)

inline QDataStream & operator<< (QDataStream &stream, const AtsError &msg)
{
    stream << msg.type;
    stream << msg.code;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, AtsError &msg)
{
    stream >> msg.type;
    stream >> msg.code;
    return stream;
}

typedef QList < AtsError > AtsErrorList;

namespace ParserErrors
{

QString toString(AtsError e);

enum StandardError {
    UnknownError = -1,
    Success = 0,
    InvalidMessage = 1,
    InvalidMessageFormat = 2,

    InvalidField3Format,
    InvalidField5Format,
    InvalidField7Format,
    InvalidField8Format,
    InvalidField9Format,
    InvalidField10Format,
    InvalidField13Format,
    InvalidField14Format,
    InvalidField15Format,
    InvalidField16Format,
    InvalidField17Format,
    InvalidField18Format,
    InvalidField19Format,
    InvalidField20Format,
    InvalidField21Format,
    InvalidField22Format,

    TooManyFields,

    InvalidFlightRules,
    InvalidFlightType,
    InvalidTurbulenceCategory,
    InvalidRadioDevice,
    InvalidEquipment,
    InvalidBorderCrossTime,
    InvalidBorderCrossTerm,
    InvalidRoute,
    InvalidRouteItem,
    InvalidRouteItemSequence,
    NeededRouteNotExists,
    InvalidRoutePointPair,
    InvalidElapsedTime,
    InvalidArrivalTime,
    InvalidDateOfFLight,
    InvalidFirEet,
    InvalidDepartureTime,

    NotamTimeExpired,
    NotamFieldQRequired,
    NotamFieldARequired,
    NotamFieldBRequired,
    NotamFieldCRequired,
    NotamFieldDRequired,
    NotamFieldERequired,

    InvalidACID,
    InvalidAircraftType,
    InvalidADEP,
    InvalidADES,

    NoSuchFpl,
    NotOurAirport,
    StatusError,
    PlnNotFromZces,
    PpnNotFromZces
};

QString toString(ParserErrors::StandardError e);

enum AftnError {
    AftnNoError = 0,
    AftnNoZCZC,
    AftnNoNNNN,
    AftnErrorTransmissionIdentification,
    AftnErrorPriorityIndicator,
    AftnErrorReceiverAddress,
    AftnErrorFillingTime,
    AftnErrorSenderAddress
};

QString toString(ParserErrors::AftnError e);

enum ExtendedError {
    ExtendedNoError = 0
};

QString toString(ParserErrors::ExtendedError e);

} // namespace ParserErrors







namespace ParseVersion
{

const int n_4444 = 0;
const int n_TC95 = 1;

}

#endif // PARSERERRORS_H
