#ifndef REQUESTS_H
#define REQUESTS_H

#include "defines.h"

#include "AftnMessage.h"

#include <QtCore/QDateTime>
#include <QtCore/QFlags>

/*!
 * \struct RequestFPL
 * \brief Запрос на FPL
 *
 * Данная структура предназначена для отправки запроса на список телеграмм FPL по критериям
 */

struct RequestFPL
{
    enum Flag {
        NONE = 0x0000
        , TIME = 0x0001
        , AIRPORT = 0x0002
    };
    typedef QFlags < Flag > Flags;

    Flags flags;
    QDateTime startTime;
    QDateTime stopTime;

    enum Type { ARIVAL, DEPARTURE, RESERVE } type;

    Defines::SourceOfRequest sourceOfRequest;
};
QT_DECLARE_ENUM_STREAMING(RequestFPL::Flags)
QT_DECLARE_ENUM_STREAMING(RequestFPL::Type)
Q_DECLARE_METATYPE(RequestFPL)

inline QDataStream & operator<< (QDataStream &stream, const RequestFPL &msg)
{
    stream << msg.flags;
    stream << msg.startTime;
    stream << msg.stopTime;
    stream << msg.type;
    stream << msg.sourceOfRequest;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, RequestFPL &msg)
{
    stream >> msg.flags;
    stream >> msg.startTime;
    stream >> msg.stopTime;
    stream >> msg.type;
    stream >> msg.sourceOfRequest;
    return stream;
}

/*!
 * \struct RequestDictionary
 * \brief Запрос словарей
 */

struct RequestDictionary
{
};Q_DECLARE_METATYPE(RequestDictionary)
inline QDataStream & operator<< (QDataStream &stream, const RequestDictionary &)
{return stream;}
inline QDataStream & operator>> (QDataStream &stream, RequestDictionary &)
{return stream;}

/*!
 * \struct RequestTelegram
 * \brief Запрос Телеграмм
 */

struct RequestTelegram
{
    enum State {
        GOOD        = 0x0001
        , ERROR     = 0x0002
        , DELETE    = 0x0004
        , SAVE      = 0x0008
        , SENT      = 0x0010
        , PROCESSED = 0x0020
        , ATS_ERROR = 0x0040
        , WRONG_AIRPORT = 0x0080

        , BY_FPL    = 0x1000
        , RELATED   = 0x2000

        , MANUAL = ERROR | ATS_ERROR | WRONG_AIRPORT
        , RECEIVED = GOOD | SAVE
        , ALL = RECEIVED | MANUAL | DELETE | SENT | PROCESSED
    };
    typedef QFlags < State > Flags;

    Flags state;
    int id;
    Defines::SourceOfRequest sourceOfRequest;
    QDateTime startTime;
    QDateTime stopTime;

    RequestTelegram()
    {
        id = 0;
    }
};
QT_DECLARE_ENUM_STREAMING(RequestTelegram::Flags)
Q_DECLARE_METATYPE(RequestTelegram)

inline QDataStream & operator<< (QDataStream &stream, const RequestTelegram &msg)
{
    stream << msg.state;
    stream << msg.id;
    stream << msg.sourceOfRequest;
    stream << msg.startTime;
    stream << msg.stopTime;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, RequestTelegram &msg)
{
    stream >> msg.state;
    stream >> msg.id;
    stream >> msg.sourceOfRequest;
    stream >> msg.startTime;
    stream >> msg.stopTime;
    return stream;
}

/*!
 * \struct RequestStatus
 * \brief Запрос статуса сервера
 */

struct RequestStatus
{
};Q_DECLARE_METATYPE(RequestStatus)
inline QDataStream & operator<< (QDataStream &stream, const RequestStatus &)
{return stream;}
inline QDataStream & operator>> (QDataStream &stream, RequestStatus &)
{return stream;}

/*!
 * \struct ChangeTelegramStatus
 * \brief Запрос статуса сервера
 */

struct ChangeTelegramStatus
{
    /*!
     * \brief id телеграммы
     */
    int id;
    /*!
     * \brief Тип запроса
     */
    enum Action {DELETE, SAVE, PROCESS} action;
    /*!
     * \brief id FPL
     *
     * Используется, если тип зарпоса - SAVE, не обязателен.
     */
    int fpl_id;
    /*!
     * \brief полный текст телеграммы
     *
     * Используется, если тип зарпоса - PROCESS.
     */
    QString text;

    ChangeTelegramStatus()
    {
        id = 0;
        fpl_id = 0;
    }
    QString actionToString() const
    {
        switch (action)
        {
        case DELETE:
            return QObject::tr("Delete");
        case SAVE:
            return QObject::tr("Save");
        case PROCESS:
            return QObject::tr("Process");
        }
        return QString();
    }
};
QT_DECLARE_ENUM_STREAMING(ChangeTelegramStatus::Action)
Q_DECLARE_METATYPE(ChangeTelegramStatus)

inline QDataStream & operator<< (QDataStream &stream, const ChangeTelegramStatus &msg)
{
    stream << msg.id;
    stream << msg.action;
    stream << msg.fpl_id;
    stream << msg.text;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, ChangeTelegramStatus &msg)
{
    stream >> msg.id;
    stream >> msg.action;
    stream >> msg.fpl_id;
    stream >> msg.text;
    return stream;
}

/*!
 * \struct SendTelegram
 * \brief Запрос на отправку телеграммы
 */

struct SendTelegram : public AftnStruct
{
    SendTelegram()
    {
    }
    SendTelegram(const AftnStruct &aftn) :
        AftnStruct(aftn)
    {
    }
};
Q_DECLARE_METATYPE(SendTelegram)

inline QDataStream & operator<< (QDataStream &stream, const SendTelegram &msg)
{
    stream << (const AftnStruct &)msg;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, SendTelegram &msg)
{
    stream >> (AftnStruct &)msg;
    return stream;
}

/*!
 * \struct RequestMessage
 * \brief Запрос на сообщение
 *
 * Данная структура предназначена для запроса класса, отнаследованного от AtsMessage,
 * сформированного по тексту телеграммы
 */

struct RequestMessage
{
    QString text;
};
Q_DECLARE_METATYPE(RequestMessage)

inline QDataStream & operator<< (QDataStream &stream, const RequestMessage &msg)
{
    stream << msg.text;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, RequestMessage &msg)
{
    stream >> msg.text;
    return stream;
}

/*!
 * \struct RequestJournal
 * \brief Запрос журнала
 */

struct RequestJournal
{
    /*!
     * \brief Количество запрашиваемых записей
     */
    int numberOfRecords;
    RequestJournal()
    {
        numberOfRecords = 100;
    }
};
Q_DECLARE_METATYPE(RequestJournal)

inline QDataStream & operator<< (QDataStream &stream, const RequestJournal &msg)
{
    stream << msg.numberOfRecords;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, RequestJournal &msg)
{
    stream >> msg.numberOfRecords;
    return stream;
}

/*!
 * \struct RequestNotam
 * \brief Запрос на NOTAM
 *
 * Данная структура предназначена для отправки запроса на список NOTAM
 */

struct RequestNotam
{
    QDateTime startTime;
    QDateTime stopTime;
    enum Status { ACTIVE, INACTIVE } state;
};

Q_DECLARE_METATYPE(RequestNotam)
QT_DECLARE_ENUM_STREAMING(RequestNotam::Status)

inline QDataStream & operator<< (QDataStream &stream, const RequestNotam &msg)
{
    stream << msg.startTime;
    stream << msg.stopTime;
    stream << msg.state;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, RequestNotam &msg)
{
    stream >> msg.startTime;
    stream >> msg.stopTime;
    stream >> msg.state;
    return stream;
}

/*!
 * \struct RequestIAS
 * \brief Запрос на ATM
 *
 * Данная структура предназначена для отправки запроса на список рестрикшенов
 */

struct RequestIAS
{
    QDateTime startTime;
    QDateTime stopTime;
    enum Type { RECEIVED, SENT } type;
};

Q_DECLARE_METATYPE(RequestIAS)
QT_DECLARE_ENUM_STREAMING(RequestIAS::Type)

inline QDataStream & operator<< (QDataStream &stream, const RequestIAS &msg)
{
    stream << msg.startTime;
    stream << msg.stopTime;
    stream << msg.type;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, RequestIAS &msg)
{
    stream >> msg.startTime;
    stream >> msg.stopTime;
    stream >> msg.type;
    return stream;
}


/*!
 * \struct ProcessTelegram
 * \brief Пометить телеграмму как прочтенную
 */

struct ProcessTelegram
{
    int id;
};

Q_DECLARE_METATYPE(ProcessTelegram)

inline QDataStream & operator<< (QDataStream &stream, const ProcessTelegram &msg)
{
    stream << msg.id;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, ProcessTelegram &msg)
{
    stream >> msg.id;
    return stream;
}


#endif // REQUESTS_H
