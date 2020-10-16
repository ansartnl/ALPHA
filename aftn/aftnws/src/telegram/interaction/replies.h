#ifndef REPLIES_H
#define REPLIES_H

#include "defines.h"

#include "AftnMessage.h"
#include "AtsMessages.h"

#include <QtCore/QStringList>

/*!
 * \struct ReplyFPL
 * \brief Структуры ответа FPL
 *
 * Данная структура предназначена для отправки ответа на запрос FPL.
 */

struct ReplyFPL : public FplStruct
{
    QDateTime trTime;      // Time of getting this record
    int id;
    FplChange fplChange;
    enum Change {
        FLIGHT_RULES = 0x00000001
        , TYPE_OF_FLIGHT = 0x00000002
        , NUMBER_OF_AIRCRAFTS = 0x00000004
        , AIRCRAFT_TYPE = 0x00000008
        , TURBULENCE = 0x00000010
        , EQUIPMENT = 0x00000020
        , SPEED = 0x00000040
        , FLIGHT_LEVEL = 0x00000080
        , ROUTE = 0x00000100
        , OTHER = 0x00000200
        , DEPARTURE_TIME = 0x00000400
        , ARRIVAL_TIME = 0x00000800
        , ALT1 = 0x00001000
        , ALT2 = 0x00002000
        , EET = 0x00004000
        , EX_FPL = 0x00008000
        , ENABLED = 0x00010000
        , CHANGE_ACID = 0x00020000
        , CHANGE_ADEP = 0x00040000
        , CHANGE_ADES = 0x00080000
    };
    typedef QFlags < Change > Changes;
    Changes changesFlags;
    Defines::SourceOfRequest sourceOfRequest;
};
QT_DECLARE_ENUM_STREAMING(ReplyFPL::Changes)
Q_DECLARE_METATYPE(QList< ReplyFPL >)

inline QDataStream & operator<< (QDataStream &stream, const ReplyFPL &msg)
{
    stream << (const FplStruct &)msg;
    stream << msg.trTime;
    stream << msg.id;
    stream << msg.sourceOfRequest;
    stream << msg.fplChange;
    stream << msg.changesFlags;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, ReplyFPL &msg)
{
    stream >> (FplStruct &)msg;
    stream >> msg.trTime;
    stream >> msg.id;
    stream >> msg.sourceOfRequest;
    stream >> msg.fplChange;
    stream >> msg.changesFlags;
    return stream;
}

/*!
 * \struct ReplyFPLList
 * \brief Ответ на запрос FPL
 */

struct ReplyFPLList
{
    QList < ReplyFPL > replyFplList;
    Defines::SourceOfRequest sourceOfRequest;
};
Q_DECLARE_METATYPE(ReplyFPLList)

inline QDataStream & operator<< (QDataStream &stream, const ReplyFPLList &msg)
{
    stream << msg.replyFplList;
    stream << msg.sourceOfRequest;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, ReplyFPLList &msg)
{
    stream >> msg.replyFplList;
    stream >> msg.sourceOfRequest;
    return stream;
}

// dictionaries

/*!
 * \struct StandardDictionaryRecord
 * \brief Стандартная запись словаря
 *
 * Стандартная запись содержит значение, описание на английском, описание на русском.
 */

struct StandardDictionaryRecord
{
    QString value;
    QString descriptionRus;
    QString descriptionEng;
};

inline QDataStream & operator<< (QDataStream &stream, const StandardDictionaryRecord &msg)
{
    stream << msg.value;
    stream << msg.descriptionRus;
    stream << msg.descriptionEng;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, StandardDictionaryRecord &msg)
{
    stream >> msg.value;
    stream >> msg.descriptionRus;
    stream >> msg.descriptionEng;
    return stream;
}

/*!
 * \struct RouteRecord
 * \brief Запись словаря маршрутов
 *
 * Помимо стандартных полей содержит поля "аэропорт отправления" и "аэропорт прибытия".
 */

struct RouteRecord : public StandardDictionaryRecord
{
    QString adep;
    QString ades;
};

inline QDataStream & operator<< (QDataStream &stream, const RouteRecord &msg)
{
    stream << (const StandardDictionaryRecord &)msg;
    stream << msg.adep;
    stream << msg.ades;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, RouteRecord &msg)
{
    stream >> (StandardDictionaryRecord &)msg;
    stream >> msg.adep;
    stream >> msg.ades;
    return stream;
}

/*!
 * \struct RouteRecords
 * \brief Словарь маршрутов
 *
 * Служит для выборки списка маршрутов по аэропортам вылета и прилета.
 */

struct RouteRecords : public QList < RouteRecord >
{
    inline QStringList routesByAirports(const QString &adep, const QString &ades)
    {
        QStringList ret;
        foreach (const RouteRecord &r, *this)
        {
            if ((r.adep == adep) && (r.ades == ades))
                ret.append(r.value);
        }
        return ret;
    }
};

/*!
 * \struct ReplyDictionary
 * \brief Ответ на запрос словарей
 */

struct ReplyDictionary
{
    QMap < QString, StandardDictionaryRecord > taircraft;
    QMap < QString, StandardDictionaryRecord > frules;
    QMap < QString, StandardDictionaryRecord > tflight;
    QMap < QString, StandardDictionaryRecord > wtc;
    QMap < QString, StandardDictionaryRecord > equipment;
    QMap < QString, StandardDictionaryRecord > airports;
    RouteRecords routes;
    int version;
};

Q_DECLARE_METATYPE(ReplyDictionary)

inline QDataStream & operator<< (QDataStream &stream, const ReplyDictionary &msg)
{
    stream << msg.taircraft;
    stream << msg.frules;
    stream << msg.tflight;
    stream << msg.wtc;
    stream << msg.equipment;
    stream << msg.airports;
    stream << msg.routes;
    stream << msg.version;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, ReplyDictionary &msg)
{
    stream >> msg.taircraft;
    stream >> msg.frules;
    stream >> msg.tflight;
    stream >> msg.wtc;
    stream >> msg.equipment;
    stream >> msg.airports;
    stream >> msg.routes;
    stream >> msg.version;
    return stream;
}

/*!
 * \struct ReplyTelegram
 * \brief Структура ответа телеграммы
 */

struct ReplyTelegram
{
    QDateTime time;
    QString type;
    QString message;
    int state;
    int fpl_id;
    QString urgency;
    QString sender;
    AtsErrorList error;
    int id;
    QString name;
    bool processed;
};
Q_DECLARE_METATYPE(QList < ReplyTelegram >)

inline QDataStream & operator<< (QDataStream &stream, const ReplyTelegram &msg)
{
    stream << msg.time;
    stream << msg.type;
    stream << msg.error;
    stream << msg.message;
    stream << msg.state;
    stream << msg.fpl_id;
    stream << msg.urgency;
    stream << msg.sender;
    stream << msg.id;
    stream << msg.name;
    stream << msg.processed;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, ReplyTelegram &msg)
{
    stream >> msg.time;
    stream >> msg.type;
    stream >> msg.error;
    stream >> msg.message;
    stream >> msg.state;
    stream >> msg.fpl_id;
    stream >> msg.urgency;
    stream >> msg.sender;
    stream >> msg.id;
    stream >> msg.name;
    stream >> msg.processed;
    return stream;
}

/*!
 * \struct ReplyTelegramList
 * \brief Ответ на запрос телеграмм
 */

struct ReplyTelegramList
{
    QList < ReplyTelegram > replyTelegramList;
    Defines::SourceOfRequest sourceOfRequest;
};
Q_DECLARE_METATYPE(ReplyTelegramList)

inline QDataStream & operator<< (QDataStream &stream, const ReplyTelegramList &msg)
{
    stream << msg.replyTelegramList;
    stream << msg.sourceOfRequest;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, ReplyTelegramList &msg)
{
    stream >> msg.replyTelegramList;
    stream >> msg.sourceOfRequest;
    return stream;
}

/*!
 * \struct ReplyStatus
 * \brief Ответ на запрос статуса
 *
 * Данная структура предназначена для отправки ответа на запрос Статуса сервера.
 * Передаваемые данные:
 * - количество телеграмм на ручной обработке;
 */

struct ReplyStatus
{
    /*!
     * \brief Количество телеграмм на ручной обработке
     */
    int manualNumber;
    /*!
     * \brief Количество полученных телеграмм
     */
    int receivedNumber;
    /*!
     * \brief Количество отправленных телеграмм
     */
    int sentNumber;
    /*!
     * \brief Есть ли необработанные телеграммы SS на ручной обработке
     */
    bool hasManualSS;
    /*!
     * \brief Есть ли необработанные телеграммы SS в полученных
     */
    bool hasReceivedSS;
    /*!
     * \brief Количество отправленных телеграмм
     */
    /*!
     * \brief Количество ВПП
     */
    int runwayCount;
    /*!
     * \brief Количество не отмененных FPL на сегодня
     */
    int fplCount;
    /*!
     * \brief Количество планов вылета на сегодня
     */
    int fplDepCount;
    /*!
     * \brief Количество планов прибытия на сегодня
     */
    int fplArrCount;
    /*!
     * \brief Количество планов запасного аэропорта
     */
    int fplAltCount;
    ReplyStatus()
    {
        manualNumber = 0;
        receivedNumber = 0;
        sentNumber = 0;
        hasManualSS = false;
        hasReceivedSS = false;
        runwayCount = 0;
        fplCount = 0;
        fplDepCount = 0;
        fplArrCount = 0;
        fplAltCount = 0;
    }
};
Q_DECLARE_METATYPE(ReplyStatus)

inline QDataStream & operator<< (QDataStream &stream, const ReplyStatus &msg)
{
    stream << msg.manualNumber;
    stream << msg.receivedNumber;
    stream << msg.sentNumber;
    stream << msg.hasManualSS;
    stream << msg.hasReceivedSS;
    stream << msg.runwayCount;
    stream << msg.fplCount;
    stream << msg.fplDepCount;
    stream << msg.fplArrCount;
    stream << msg.fplAltCount;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, ReplyStatus &msg)
{
    stream >> msg.manualNumber;
    stream >> msg.receivedNumber;
    stream >> msg.sentNumber;
    stream >> msg.hasManualSS;
    stream >> msg.hasReceivedSS;
    stream >> msg.runwayCount;
    stream >> msg.fplCount;
    stream >> msg.fplDepCount;
    stream >> msg.fplArrCount;
    stream >> msg.fplAltCount;
    return stream;
}

/*!
 * \struct ReplyMessage
 * \brief Ответ на запрос сообщения
 */

struct ReplyMessage
{
    QString type;
    AftnStruct aftn;
    ArrMessage arr;
    ChgMessage chg;
    CnlMessage cnl;
    DepMessage dep;
    DlaMessage dla;
    FplMessage fpl;
    UnknownMessage ukn;
};
Q_DECLARE_METATYPE(ReplyMessage)

inline QDataStream & operator<< (QDataStream &stream, const ReplyMessage &msg)
{
    stream << msg.type;
    stream << msg.aftn;
    if (Ats::isArr(msg.type))
        stream << msg.arr;
    else if (Ats::isChg(msg.type))
        stream << msg.chg;
    else if (Ats::isCnl(msg.type))
        stream << msg.cnl;
    else if (Ats::isDep(msg.type))
        stream << msg.dep;
    else if (Ats::isDla(msg.type))
        stream << msg.dla;
    else if (Ats::isFpl(msg.type) || msg.type == Ats::PLN1 || msg.type == Ats::PLN)
        stream << msg.fpl;
    else if (msg.type == Ats::Unknown)
        stream << msg.ukn;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, ReplyMessage &msg)
{
    stream >> msg.type;
    stream >> msg.aftn;
    if (Ats::isArr(msg.type))
        stream >> msg.arr;
    else if (Ats::isChg(msg.type))
        stream >> msg.chg;
    else if (Ats::isCnl(msg.type))
        stream >> msg.cnl;
    else if (Ats::isDep(msg.type))
        stream >> msg.dep;
    else if (Ats::isDla(msg.type))
        stream >> msg.dla;
    else if (Ats::isFpl(msg.type) || msg.type == Ats::PLN1 || msg.type == Ats::PLN)
        stream >> msg.fpl;
    else if (msg.type == Ats::Unknown)
        stream >> msg.ukn;
    return stream;
}

/*!
 * \struct FreeMessage
 * \brief Ответ на запрос о статусе выполнения запроса
 *
 * Данная структура предназначена для отправки ответа на запрос  о статусе выполнения запроса.
 * Передаваемые данные:
 * - текст;
 */

struct FreeMessage
{
    enum Message {
        SQL_ERROR
        , OTHER_ERROR
        , TELEGRAM_DELETED_SUCCESSFULLY
        , TELEGRAM_SAVED_SUCCESSFULLY
        , TELEGRAM_PROCESSED_SUCCESSFULLY
        , TELEGRAM_PROCESSED_WITH_ERRORS
    } message;

   inline QString text() const
   {
       switch (message)
       {
       case SQL_ERROR:
           return QObject::tr("SQL error occured");
       case OTHER_ERROR:
           return QObject::tr("Some error occured");
       case TELEGRAM_DELETED_SUCCESSFULLY:
           return QObject::tr("Telegram deleted successfully");
       case TELEGRAM_SAVED_SUCCESSFULLY:
           return QObject::tr("Telegram saved successfully");
       case TELEGRAM_PROCESSED_SUCCESSFULLY:
           return QObject::tr("Telegram processed successfully");
       case TELEGRAM_PROCESSED_WITH_ERRORS:
           return QObject::tr("Telegram processed with errors");
       }
       return QObject::tr("Unknown message");
   }
};
QT_DECLARE_ENUM_STREAMING(FreeMessage::Message)
Q_DECLARE_METATYPE(FreeMessage)

inline QDataStream & operator<< (QDataStream &stream, const FreeMessage &msg)
{
    stream << msg.message;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, FreeMessage &msg)
{
    stream >> msg.message;
    return stream;
}

/*!
 * \struct JournalRecord
 * \brief Запись журнала
 */

struct JournalRecord
{
    QDateTime time;
    QString host;
    QString login;
    QString message;
    enum Type {
        OTHER = 0
        , ADD = 1
        , REMOVE = 2
        , CHANGE = 3
        , SEND = 4
        , SERVICE = 5
    } type;
};
QT_DECLARE_ENUM_STREAMING(JournalRecord::Type)
Q_DECLARE_METATYPE(QList < JournalRecord >)

inline QDataStream & operator<< (QDataStream &stream, const JournalRecord &msg)
{
    stream << msg.time;
    stream << msg.host;
    stream << msg.login;
    stream << msg.message;
    stream << msg.type;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, JournalRecord &msg)
{
    stream >> msg.time;
    stream >> msg.host;
    stream >> msg.login;
    stream >> msg.message;
    stream >> msg.type;
    return stream;
}

/*!
 * \struct ReplyNotam
 * \brief Ответ на запрос NOTAM
 *
 * Данная структура предназначена для отправки NOTAM в ответа на запрос.
 */

struct ReplyNotam : NotamStruct
{
    int id;
    enum State { ACTIVE, INACTIVE } state;
};
Q_DECLARE_METATYPE(QList < ReplyNotam >)
QT_DECLARE_ENUM_STREAMING(ReplyNotam::State)

inline QDataStream & operator<< (QDataStream &stream, const ReplyNotam &msg)
{
    stream << (const NotamStruct &)msg;
    stream << msg.id;
    stream << msg.state;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, ReplyNotam &msg)
{
    stream >> (NotamStruct &)msg;
    stream >> msg.id;
    stream >> msg.state;
    return stream;
}

/*!
 * \struct ReplyIAS
 * \brief Ответ на запрос рестрекшинов
 *
 * Данная структура предназначена для отправки ATM в ответа на запрос
 */

struct ReplyIAS
{
    QString typeMessage;
    QString text;
    QDateTime date;
    enum Type { RECEIVED, SENT } type;
};

Q_DECLARE_METATYPE(QList < ReplyIAS >)
QT_DECLARE_ENUM_STREAMING(ReplyIAS::Type)

inline QDataStream & operator<< (QDataStream &stream, const ReplyIAS &msg)
{
    stream << msg.typeMessage;
    stream << msg.text;
    stream << msg.date;
    stream << msg.type;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, ReplyIAS &msg)
{
    stream >> msg.typeMessage;
    stream >> msg.text;
    stream >> msg.date;
    stream >> msg.type;
    return stream;
}
#endif // REPLIES_H
