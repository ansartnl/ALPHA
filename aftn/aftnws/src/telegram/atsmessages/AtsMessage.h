#ifndef ATSMESSAGE_H
#define ATSMESSAGE_H

#include "ParserErrors.h"

#include <QtCore/QDateTime>
#include <QtCore/QStringList>

const QByteArray ADP = "ADP 1.0/n";

namespace Ats
{

const char Unknown[] = "UKN";

const char FPL[] = "FPL";

const char ARR[] = "ARR";
const char CHG[] = "CHG";
const char CNL[] = "CNL";
const char DEP[] = "DEP";
const char DLA[] = "DLA";

const char FPL1[] = "ФПЛ";

const char ARR1[] = "АРР";
const char CHG1[] = "ЦХГ";
const char CNL1[] = "ЦНЛ";
const char DEP1[] = "ДЕП";
const char DLA1[] = "ДЛА";

const char PLN[] = "PLN";
const char PLN1[] = "ПЛН";
const char PPN[] = "PPN";
const char PPN1[] = "ППН";

inline bool isFpl(const QString &type)
{
    return ((type == FPL) || (type == FPL1));
}
inline bool isArr(const QString &type)
{
    return ((type == ARR) || (type == ARR1));
}
inline bool isChg(const QString &type)
{
    return ((type == CHG) || (type == CHG1));
}
inline bool isCnl(const QString &type)
{
    return ((type == CNL) || (type == CNL1));
}
inline bool isDep(const QString &type)
{
    return ((type == DEP) || (type == DEP1));
}
inline bool isDla(const QString &type)
{
    return ((type == DLA) || (type == DLA1));
}
inline bool isPln(const QString &type)
{
    return ((type == PLN) || (type == PLN1));
}
inline bool isPpn(const QString &type)
{
    return ((type == PPN) || (type == PPN1));
}

inline bool isTC95(const QString &type)
{
    return (type == Ats::ARR1)
            || (type == Ats::CHG1)
            || (type == Ats::CNL1)
            || (type == Ats::DEP1)
            || (type == Ats::DLA1)
            || (type == Ats::FPL1)
            || (type == Ats::PLN1)
            || (type == Ats::PPN1);
}
inline bool is4444(const QString &type)
{
    return (type == Ats::ARR)
            || (type == Ats::CHG)
            || (type == Ats::CNL)
            || (type == Ats::DEP)
            || (type == Ats::DLA)
            || (type == Ats::FPL)
            || (type == Ats::PLN)
            || (type == Ats::PPN);
}

const char NOTAM[] = "NOTAM";

const char AFPL[] = "AFP";
const char CFPL[] = "CFP";
const char CROUT[] = "CRO";
const char DFPL[] = "DFP";
const char EFPL[] = "EFP";
const char FPLINF[] = "FIN";
const char GAFPL[] = "GAF";
const char GDFPL[] = "GDF";
const char LAM[] = "LAM";
const char MET[] = "MET";
const char NFPL[] = "NFP";
const char NTM[] = "NTM";
const char RMET[] = "RME";
const char RNTM[] = "RNT";
const char RROUT[] = "RRO";

} // namespace Ats

/*!
 * \struct AtsStruct
 * \brief Базавая структура телеграммы
 *
 * Данная структура заполняется без участия парсера.
 */

struct AtsStruct
{
    /*!
     * \brief Полный текст телеграммы
     */
    QString allMessage;
    /*!
     * \brief Данные телеграммы
     */
    QString data;
    /*!
     * \brief UTC время прибытия телеграммы
     */
    QDateTime arrivalTime;
    /*!
     * \brief Версия парсинга
     */
    int version;
};

inline QDataStream & operator<< (QDataStream &stream, const AtsStruct &msg)
{
    stream << msg.allMessage;
    stream << msg.data;
    stream << msg.arrivalTime;
    stream << msg.version;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, AtsStruct &msg)
{
    stream >> msg.allMessage;
    stream >> msg.data;
    stream >> msg.arrivalTime;
    stream >> msg.version;
    return stream;
}

/*!
 * \struct AtsInfoStruct
 * \brief Информационная структура телеграммы
 *
 * Данная структура заполняется парсером.
 */

struct AtsInfoStruct
{
    /*!
     * \brief Приоритет АФТН сообщения
     */
    QString aftnPriority;
    /*!
     * \brief Отправитель АФТН сообщения
     */
    QString aftnSender;
    /*!
     * \brief Тип телеграммы
     */
    QString type;
    /*!
     * \brief Отправитель телеграммы
     */
    QString sender;
    /*!
     * \brief Получатель телеграммы
     */
    QString receiver;
    /*!
     * \brief Номер телеграммы
     */
    int number;
    /*!
     * \brief Отправитель первой телеграммы
     *
     * Отправитель телеграммы, с которой началась цепь телеграмм
     */
    QString firstSender;
    /*!
     * \brief Получатель первой телеграммы
     *
     * Получатель телеграммы, с которой началась цепь телеграмм
     */
    QString firstReceiver;
    /*!
     * \brief Номер первой телеграммы
     *
     * Номер телеграммы, с которой началась цепь телеграмм
     */
    int firstNumber;
};

inline QDataStream & operator<< (QDataStream &stream, const AtsInfoStruct &msg)
{
    stream << msg.aftnPriority;
    stream << msg.aftnSender;
    stream << msg.type;
    stream << msg.sender;
    stream << msg.receiver;
    stream << msg.number;
    stream << msg.firstSender;
    stream << msg.firstReceiver;
    stream << msg.firstNumber;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, AtsInfoStruct &msg)
{
    stream >> msg.aftnPriority;
    stream >> msg.aftnSender;
    stream >> msg.type;
    stream >> msg.sender;
    stream >> msg.receiver;
    stream >> msg.number;
    stream >> msg.firstSender;
    stream >> msg.firstReceiver;
    stream >> msg.firstNumber;
    return stream;
}

/*!
 * \struct BaseAtsStruct
 * \brief Базавая структура стандартных телеграмм
 */

struct BaseAtsStruct
{
    /*!
     * \brief Версия парсинга
     */
    int version;

    QString ACID;
    QString VORL;

    QString ADEP;
    QString dtADEP;

    QString ADES;
    QString dtADES;
    QString alt1;
    QString alt2;

    QDateTime departureTime;
    QDateTime arrivalTime;

    BaseAtsStruct()
        : version(ParseVersion::n_4444) {}
};

inline QDataStream & operator<< (QDataStream &stream, const BaseAtsStruct &msg)
{
    stream << msg.version;
    stream << msg.ACID;
    stream << msg.VORL;
    stream << msg.ADEP;
    stream << msg.dtADEP;
    stream << msg.ADES;
    stream << msg.dtADES;
    stream << msg.alt1;
    stream << msg.alt2;
    stream << msg.departureTime;
    stream << msg.arrivalTime;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, BaseAtsStruct &msg)
{
    stream >> msg.version;
    stream >> msg.ACID;
    stream >> msg.VORL;
    stream >> msg.ADEP;
    stream >> msg.dtADEP;
    stream >> msg.ADES;
    stream >> msg.dtADES;
    stream >> msg.alt1;
    stream >> msg.alt2;
    stream >> msg.departureTime;
    stream >> msg.arrivalTime;
    return stream;
}

/*!
 * \struct FplChange
 * \brief Изменения FPL
 *
 * Данная структура содержит поля, которые могут измениться в телеграмме FPL.
 */

struct FplChange : public BaseAtsStruct
{
    QString changedACID;
    QString changedADEP;
    QString changedADES;

    QString flightRules;
    QString typeOfFlight;

    int numberOfAircrafts;
    QString aircraftType;
    QString turbulence;

    QString equipment;

    QString speed;
    QString flightLevel;
    QString route;

    QString other;

    enum ExFplType {
        DEFAULT = 0
        , DEPARTURED = 1
        , ARRIVED = 2
        , DELAYED = 3
        , CANCELLED = 4
    } exFpl;

    int enabled;        // 0 - disabled, 1 - enabled, 2 - on approval
};
QT_DECLARE_ENUM_STREAMING(FplChange::ExFplType)

inline QDataStream & operator<< (QDataStream &stream, const FplChange &msg)
{
    stream << (const BaseAtsStruct &)msg;
    stream << msg.changedACID;
    stream << msg.changedADEP;
    stream << msg.changedADES;
    stream << msg.aircraftType;
    stream << msg.flightRules;
    stream << msg.typeOfFlight;
    stream << msg.numberOfAircrafts;
    stream << msg.turbulence;
    stream << msg.equipment;
    stream << msg.speed;
    stream << msg.flightLevel;
    stream << msg.route;
    stream << msg.other;
    stream << msg.exFpl;
    stream << msg.enabled;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, FplChange &msg)
{
    stream >> (BaseAtsStruct &)msg;
    stream >> msg.changedACID;
    stream >> msg.changedADEP;
    stream >> msg.changedADES;
    stream >> msg.aircraftType;
    stream >> msg.flightRules;
    stream >> msg.typeOfFlight;
    stream >> msg.numberOfAircrafts;
    stream >> msg.turbulence;
    stream >> msg.equipment;
    stream >> msg.speed;
    stream >> msg.flightLevel;
    stream >> msg.route;
    stream >> msg.other;
    stream >> msg.exFpl;
    stream >> msg.enabled;
    return stream;
}



class LamMessage;

class AtsMessage
{
public:
    AtsMessage();

    virtual QByteArray toAftn() const = 0;
    virtual QByteArray toJson() const = 0;

    QString type() const;

    virtual LamMessage * lam() const = 0;

    AtsStruct mData;
    AtsInfoStruct mInfo;
    AtsErrorList errors;

    inline friend QDataStream & operator<< (QDataStream &stream, const AtsMessage &msg)
    {
        stream << msg.mData;
        stream << msg.mInfo;
        stream << msg.errors;
        return stream;
    }

    inline friend QDataStream & operator>> (QDataStream &stream, AtsMessage &msg)
    {
        stream >> msg.mData;
        stream >> msg.mInfo;
        stream >> msg.errors;
        return stream;
    }
};

QString removeColonFromTime(const QString &time);

#endif // ATSMESSAGE_H
