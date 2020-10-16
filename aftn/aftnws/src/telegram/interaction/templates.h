#ifndef TEMPLATES_H
#define TEMPLATES_H

#include "defines.h"

#include "AtsMessages.h"

/*!
 * \struct AddFplTemplate
 * \brief Добавить шаблон FPL
 */

struct AddFplTemplate
{
    QString name;
    FplStruct fpl;
};
Q_DECLARE_METATYPE(AddFplTemplate)

inline QDataStream & operator<< (QDataStream &stream, const AddFplTemplate &msg)
{
    stream << msg.name;
    stream << msg.fpl;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, AddFplTemplate &msg)
{
    stream >> msg.name;
    stream >> msg.fpl;
    return stream;
}

/*!
 * \struct AddArrTemplate
 * \brief Добавить шаблон ARR
 */

struct AddArrTemplate
{
    QString name;
    ArrStruct arr;
};
Q_DECLARE_METATYPE(AddArrTemplate)

inline QDataStream & operator<< (QDataStream &stream, const AddArrTemplate &msg)
{
    stream << msg.name;
    stream << msg.arr;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, AddArrTemplate &msg)
{
    stream >> msg.name;
    stream >> msg.arr;
    return stream;
}

/*!
 * \struct AddChgTemplate
 * \brief Добавить шаблон CHG
 */

struct AddChgTemplate
{
    QString name;
    ChgStruct chg;
};
Q_DECLARE_METATYPE(AddChgTemplate)

inline QDataStream & operator<< (QDataStream &stream, const AddChgTemplate &msg)
{
    stream << msg.name;
    stream << msg.chg;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, AddChgTemplate &msg)
{
    stream >> msg.name;
    stream >> msg.chg;
    return stream;
}

/*!
 * \struct AddCnlTemplate
 * \brief Добавить шаблон CNL
 */

struct AddCnlTemplate
{
    QString name;
    CnlStruct cnl;
};
Q_DECLARE_METATYPE(AddCnlTemplate)

inline QDataStream & operator<< (QDataStream &stream, const AddCnlTemplate &msg)
{
    stream << msg.name;
    stream << msg.cnl;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, AddCnlTemplate &msg)
{
    stream >> msg.name;
    stream >> msg.cnl;
    return stream;
}

/*!
 * \struct AddDepTemplate
 * \brief Добавить шаблон DEP
 */

struct AddDepTemplate
{
    QString name;
    DepStruct dep;
};
Q_DECLARE_METATYPE(AddDepTemplate)

inline QDataStream & operator<< (QDataStream &stream, const AddDepTemplate &msg)
{
    stream << msg.name;
    stream << msg.dep;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, AddDepTemplate &msg)
{
    stream >> msg.name;
    stream >> msg.dep;
    return stream;
}

/*!
 * \struct AddDlaTemplate
 * \brief Добавить шаблон DLA
 */

struct AddDlaTemplate
{
    QString name;
    DlaStruct dla;
};
Q_DECLARE_METATYPE(AddDlaTemplate)

inline QDataStream & operator<< (QDataStream &stream, const AddDlaTemplate &msg)
{
    stream << msg.name;
    stream << msg.dla;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, AddDlaTemplate &msg)
{
    stream >> msg.name;
    stream >> msg.dla;
    return stream;
}

/*!
 * \struct AddFreeTextTemplate
 * \brief Добавить шаблон свободного текста
 */

struct AddFreeTextTemplate
{
    QString name;
    QString text;
};
Q_DECLARE_METATYPE(AddFreeTextTemplate)

inline QDataStream & operator<< (QDataStream &stream, const AddFreeTextTemplate &msg)
{
    stream << msg.name;
    stream << msg.text;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, AddFreeTextTemplate &msg)
{
    stream >> msg.name;
    stream >> msg.text;
    return stream;
}

/*!
 * \struct RemoveTemplate
 * \brief Удалить шаблон
 */

struct RemoveTemplate
{
    Defines::TelegramType type;
    int id;
};
Q_DECLARE_METATYPE(RemoveTemplate)

inline QDataStream & operator<< (QDataStream &stream, const RemoveTemplate &msg)
{
    stream << msg.type;
    stream << msg.id;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, RemoveTemplate &msg)
{
    stream >> msg.type;
    stream >> msg.id;
    return stream;
}

/*!
 * \struct RequestTemplate
 * \brief Запрос списка шаблонов
 */

struct RequestTemplate
{
    Defines::TelegramType type;
    RequestTemplate(Defines::TelegramType t = Defines::FREE_TEXT)
    {
        type = t;
    }
};
Q_DECLARE_METATYPE(RequestTemplate)

inline QDataStream & operator<< (QDataStream &stream, const RequestTemplate &msg)
{
    stream << msg.type;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, RequestTemplate &msg)
{
    stream >> msg.type;
    return stream;
}

/*!
 * \struct ReplyFplTemplate
 * \brief Шаблон FPL
 */

struct ReplyFplTemplate
{
    QString name;
    FplStruct fpl;
    int id;
};
Q_DECLARE_METATYPE(QList < ReplyFplTemplate >)

inline QDataStream & operator<< (QDataStream &stream, const ReplyFplTemplate &msg)
{
    stream << msg.name;
    stream << msg.fpl;
    stream << msg.id;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, ReplyFplTemplate &msg)
{
    stream >> msg.name;
    stream >> msg.fpl;
    stream >> msg.id;
    return stream;
}

/*!
 * \struct ReplyArrTemplate
 * \brief Шаблон ARR
 */

struct ReplyArrTemplate
{
    QString name;
    ArrStruct arr;
    int id;
};
Q_DECLARE_METATYPE(QList < ReplyArrTemplate >)

inline QDataStream & operator<< (QDataStream &stream, const ReplyArrTemplate &msg)
{
    stream << msg.name;
    stream << msg.arr;
    stream << msg.id;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, ReplyArrTemplate &msg)
{
    stream >> msg.name;
    stream >> msg.arr;
    stream >> msg.id;
    return stream;
}

/*!
 * \struct ReplyChgTemplate
 * \brief Шаблон CHG
 */

struct ReplyChgTemplate
{
    QString name;
    ChgStruct chg;
    int id;
};
Q_DECLARE_METATYPE(QList < ReplyChgTemplate >)

inline QDataStream & operator<< (QDataStream &stream, const ReplyChgTemplate &msg)
{
    stream << msg.name;
    stream << msg.chg;
    stream << msg.id;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, ReplyChgTemplate &msg)
{
    stream >> msg.name;
    stream >> msg.chg;
    stream >> msg.id;
    return stream;
}

/*!
 * \struct ReplyCnlTemplate
 * \brief Шаблон CNL
 */

struct ReplyCnlTemplate
{
    QString name;
    CnlStruct cnl;
    int id;
};
Q_DECLARE_METATYPE(QList < ReplyCnlTemplate >)

inline QDataStream & operator<< (QDataStream &stream, const ReplyCnlTemplate &msg)
{
    stream << msg.name;
    stream << msg.cnl;
    stream << msg.id;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, ReplyCnlTemplate &msg)
{
    stream >> msg.name;
    stream >> msg.cnl;
    stream >> msg.id;
    return stream;
}

/*!
 * \struct ReplyDepTemplate
 * \brief Шаблон DEP
 */

struct ReplyDepTemplate
{
    QString name;
    DepStruct dep;
    int id;
};
Q_DECLARE_METATYPE(QList < ReplyDepTemplate >)

inline QDataStream & operator<< (QDataStream &stream, const ReplyDepTemplate &msg)
{
    stream << msg.name;
    stream << msg.dep;
    stream << msg.id;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, ReplyDepTemplate &msg)
{
    stream >> msg.name;
    stream >> msg.dep;
    stream >> msg.id;
    return stream;
}

/*!
 * \struct ReplyDlaTemplate
 * \brief Шаблон DLA
 */

struct ReplyDlaTemplate
{
    QString name;
    DlaStruct dla;
    int id;
};
Q_DECLARE_METATYPE(QList < ReplyDlaTemplate >)

inline QDataStream & operator<< (QDataStream &stream, const ReplyDlaTemplate &msg)
{
    stream << msg.name;
    stream << msg.dla;
    stream << msg.id;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, ReplyDlaTemplate &msg)
{
    stream >> msg.name;
    stream >> msg.dla;
    stream >> msg.id;
    return stream;
}

/*!
 * \struct ReplyFreeTextTemplate
 * \brief Шаблон свободного текса
 */

struct ReplyFreeTextTemplate
{
    QString name;
    QString text;
    int id;
};
Q_DECLARE_METATYPE(QList < ReplyFreeTextTemplate >)

inline QDataStream & operator<< (QDataStream &stream, const ReplyFreeTextTemplate &msg)
{
    stream << msg.name;
    stream << msg.text;
    stream << msg.id;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, ReplyFreeTextTemplate &msg)
{
    stream >> msg.name;
    stream >> msg.text;
    stream >> msg.id;
    return stream;
}

#endif // TEMPLATES_H
