#ifndef XT_AIRPLANE_INFO_DATA_H
#define XT_AIRPLANE_INFO_DATA_H

#include "xtproto_global.h"
#include "XtDefines.h"
#include "XtObject.h"
#include <QtGlobal>

XT_BEGIN_NAMESPACE

//! Дополнительные данные для отображения в формуляре, для конкретного ВС
/*! Хотя стуктура содержит все поля данных, но будет упакована в буфер для передачи
    данных только теми полями, которые перечислены в флаге содержания
 */
struct XTPROTO_EXPORT AirplaneInfo : public CObject
{
    enum { InvalidNumber = 0xFFFF };
    enum { InvalidRadar = 0xFFFF };

    AirplaneInfo() : mNumber(InvalidNumber), mId(InvalidRadar), mFlags(0)
    {}

    virtual void Serialize(QDataStream& out) const;
    virtual void Deserialize(QDataStream& in);

    QByteArray toArray() const;

    XT_OBJECT_TYPE_DECL

public:
    //! Флаг присутствия полей в структуре
    enum ContentFlag
    {
        CallSign        = 0x01,
        StateName       = 0x02,
        RemainingFuel   = 0x04,
        ICAO            = 0x08,
        Alarm           = 0x10
    };
    typedef int     TContentFlags;

    //! Флаг аварийной ситуации
    enum AlarmFlag
    {
        AlarmNone,      //!< Стирает сигнал бедствия
        AlarmGE,        //!< БД – бедствие (GE на англ.)
        AlarmUI,        //!< НП – нападение на воздушное судно (UI на англ.)
        AlarmLC,        //!< РС – потеря радиосвязи (LC на англ.)
        AlarmSPI,       //!< SPI - special position ident
        AlarmAPW,       //!< ЗЗ – вход в запретную зону (APW на англ.)
        AlarmSTCA,      //!< КС – конфликтная ситуация (STCA на англ.)
        AlarmMSAW,      //!< МВ – минимально безопасные высоты (MSAW на англ.)
        AlarmWAPW       //!< ОЯ – вход в зону опасного метеоявления (WAPW на англ.)
    };


    /** Идентификатор (номер борта или код ответчика) */
    quint16         mNumber;
    /** Идентификатор локатора */
    quint16         mId;
    /** Содержание полей в буфере */
    TContentFlags   mFlags;
    /** Уникальный индивидуальный номер ВС по классификации ИКАО */
    quint32         mICAO;
    /** Радиотелефонный позывной */
    QString         mCallSign;
    /** Cокращенное наименование государства, в котором зарегистрировано данное ВС */
    QString         mStateName;
    /** Остаток топлива */
    qreal           mRemainingFuel;
    //! Флаг аварийной ситуации
    AlarmFlag       mAlarmFlag;
};

XT_END_NAMESPACE

#endif // XT_AIRPLANE_INFO_DATA_H
