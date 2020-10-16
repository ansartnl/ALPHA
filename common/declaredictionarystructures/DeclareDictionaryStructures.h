#ifndef DECLARE_DICTIONARY_STRUCTURES_H
#define DECLARE_DICTIONARY_STRUCTURES_H

#include <QtCore/QList>
#include <QtCore/QVariant>
#include <QtCore/QUuid>

/*!
 * \macro QT_DECLARE_DICTIONARY_STRUCTURES(BaseStructure, ParamsStructure)
 * \brief Объявление структур редактирования словаря
 *
 * Данный макрос предназначен для создания 6-ти стандартных структур запроса, ответа и редактирования
 * данных в табличном представлении.
 * \param[in]   BaseStructure   структура, содержащая список редактируемых полей
 * \param[in]   ParamsStructure структура, содержащая список данных, используемых для запроса
 *
 * \example forwarding_rules.h
 */

#define QT_DECLARE_DICTIONARY_STRUCTURES(BaseStructure, ParamsStructure)                        \
    struct tagAdd##BaseStructure : public BaseStructure                                         \
    {                                                                                           \
        tagAdd##BaseStructure()                                                                 \
        {}                                                                                      \
        tagAdd##BaseStructure(const BaseStructure &b) :                                         \
            BaseStructure(b)                                                                    \
        {}                                                                                      \
    }; Q_DECLARE_METATYPE(tagAdd##BaseStructure)                                                \
    inline QDataStream & operator<< (QDataStream &stream, const tagAdd##BaseStructure &msg)     \
    {                                                                                           \
        stream << (const BaseStructure &)msg;                                                   \
        return stream;                                                                          \
    }                                                                                           \
    inline QDataStream & operator>> (QDataStream &stream, tagAdd##BaseStructure &msg)           \
    {                                                                                           \
        stream >> (BaseStructure &)msg;                                                         \
        return stream;                                                                          \
    }                                                                                           \
    struct tagRemove##BaseStructure                                                             \
    {                                                                                           \
        int id;                                                                                 \
        tagRemove##BaseStructure() :                                                            \
            id(-1)                                                                              \
        {}                                                                                      \
        tagRemove##BaseStructure(const int i) :                                                 \
            id(i)                                                                               \
        {}                                                                                      \
    }; Q_DECLARE_METATYPE(tagRemove##BaseStructure)                                             \
    inline QDataStream & operator<< (QDataStream &stream, const tagRemove##BaseStructure &msg)  \
    {                                                                                           \
        stream << msg.id;                                                                       \
        return stream;                                                                          \
    }                                                                                           \
    inline QDataStream & operator>> (QDataStream &stream, tagRemove##BaseStructure &msg)        \
    {                                                                                           \
        stream >> msg.id;                                                                       \
        return stream;                                                                          \
    }                                                                                           \
    struct tagChange##BaseStructure : public BaseStructure                                      \
    {                                                                                           \
        int id;                                                                                 \
        tagChange##BaseStructure() :                                                            \
            id(-1)                                                                              \
        {}                                                                                      \
        tagChange##BaseStructure(const BaseStructure &b) :                                      \
            BaseStructure(b), id(-1)                                                            \
        {}                                                                                      \
        tagChange##BaseStructure(const BaseStructure &b, const int i) :                         \
            BaseStructure(b), id(i)                                                             \
        {}                                                                                      \
    }; Q_DECLARE_METATYPE(tagChange##BaseStructure)                                             \
    inline QDataStream & operator<< (QDataStream &stream, const tagChange##BaseStructure &msg)  \
    {                                                                                           \
        stream << (const BaseStructure &)msg;                                                   \
        stream << msg.id;                                                                       \
        return stream;                                                                          \
    }                                                                                           \
    inline QDataStream & operator>> (QDataStream &stream, tagChange##BaseStructure &msg)        \
    {                                                                                           \
        stream >> (BaseStructure &)msg;                                                         \
        stream >> msg.id;                                                                       \
        return stream;                                                                          \
    }                                                                                           \
    struct tagRequest##BaseStructure : public ParamsStructure                                   \
    {                                                                                           \
        QUuid uid;                                                                              \
        tagRequest##BaseStructure()                                                             \
        {}                                                                                      \
        tagRequest##BaseStructure(const ParamsStructure &p) :                                   \
            ParamsStructure(p)                                                                  \
        {}                                                                                      \
        tagRequest##BaseStructure(const ParamsStructure &p, const QUuid &u) :                   \
            ParamsStructure(p), uid(u)                                                          \
        {}                                                                                      \
    }; Q_DECLARE_METATYPE(tagRequest##BaseStructure)                                            \
    inline QDataStream & operator<< (QDataStream &stream, const tagRequest##BaseStructure &msg) \
    {                                                                                           \
        stream << (const ParamsStructure &)msg;                                                 \
        stream << msg.uid;                                                                      \
        return stream;                                                                          \
    }                                                                                           \
    inline QDataStream & operator>> (QDataStream &stream, tagRequest##BaseStructure &msg)       \
    {                                                                                           \
        stream >> (ParamsStructure &)msg;                                                       \
        stream >> msg.uid;                                                                      \
        return stream;                                                                          \
    }                                                                                           \
    struct tagReply##BaseStructure : public BaseStructure                                       \
    {                                                                                           \
        int id;                                                                                 \
        tagReply##BaseStructure() :                                                             \
            id(-1)                                                                              \
        {}                                                                                      \
        tagReply##BaseStructure(const BaseStructure &b) :                                       \
            BaseStructure(b), id(-1)                                                            \
        {}                                                                                      \
        tagReply##BaseStructure(const BaseStructure &b, const int i) :                          \
            BaseStructure(b), id(i)                                                             \
        {}                                                                                      \
    };                                                                                          \
    inline QDataStream & operator<< (QDataStream &stream, const tagReply##BaseStructure &msg)   \
    {                                                                                           \
        stream << (const BaseStructure &)msg;                                                   \
        stream << msg.id;                                                                       \
        return stream;                                                                          \
    }                                                                                           \
    inline QDataStream & operator>> (QDataStream &stream, tagReply##BaseStructure &msg)         \
    {                                                                                           \
        stream >> (BaseStructure &)msg;                                                         \
        stream >> msg.id;                                                                       \
        return stream;                                                                          \
    }                                                                                           \
    struct tagReply##BaseStructure##s                                                           \
    {                                                                                           \
        QList < tagReply##BaseStructure > reply##BaseStructure##s;                              \
        QUuid uid;                                                                              \
        tagReply##BaseStructure##s()                                                            \
        {}                                                                                      \
    }; Q_DECLARE_METATYPE(tagReply##BaseStructure##s)                                           \
    inline QDataStream & operator<< (QDataStream &stream, const tagReply##BaseStructure##s &msg)\
    {                                                                                           \
        stream << msg.reply##BaseStructure##s;                                                  \
        stream << msg.uid;                                                                      \
        return stream;                                                                          \
    }                                                                                           \
    inline QDataStream & operator>> (QDataStream &stream, tagReply##BaseStructure##s &msg)      \
    {                                                                                           \
        stream >> msg.reply##BaseStructure##s;                                                  \
        stream >> msg.uid;                                                                      \
        return stream;                                                                          \
    }

/*!
 * \struct FooParamStructure
 * \brief Пустая структура параметров запроса
 */

struct FooParamStructure
{
    FooParamStructure()
    {}
};

inline QDataStream & operator<< (QDataStream &stream, const FooParamStructure &)
{return stream;}

inline QDataStream & operator>> (QDataStream &stream, FooParamStructure &)
{return stream;}

#endif // DECLARE_DICTIONARY_STRUCTURES_H
