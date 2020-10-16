#pragma once

#include <functional>

#include <QtCore/QtGlobal>
#include <QCoreApplication>
#include <QMetaType>
#include <QVariant>
#include <QList>
#include <QString>
#include <QVector>
#include <QSharedPointer>

#include "utils.h"
#include "utilities_global.h"


class QTranslator;


namespace utilities {


namespace Tr {


const char kProperty[] = "TranslationProperty";


struct UTILITIES_EXPORT Translation {
    const char* context;
    const char* key;
    const char* disambiguation;
    int n;
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    QCoreApplication::Encoding encoding;
#endif
};


class UTILITIES_EXPORT QStringVector : public QVector<QString> {};


struct UTILITIES_EXPORT iTranslationRule {
    virtual ~iTranslationRule() {}
    virtual QString GetText() const = 0;
};


// regular translation rule
class UTILITIES_EXPORT TranslationRule : public iTranslationRule {
public:
    TranslationRule(const Translation& translateArgs, const QStringVector& expand)
        : translate_args_(translateArgs)
        , expand_(expand)
    {}

    QString GetText() const;

private:
    Translation translate_args_;
    QStringVector expand_;
};


class UTILITIES_EXPORT TextTranslationRule : public iTranslationRule {
public:
    explicit TextTranslationRule(const QString& text = QString())
        : text_(text)
    {}

    inline QString GetText() const { return text_; }

private:
    QString text_;
};


template <typename ResTy, typename SrcTy>
ResTy union_cast(SrcTy v) {
    static_assert(sizeof(ResTy) >= sizeof(SrcTy), "Size of a result type have to be greater than size of a source type");
    union {
        SrcTy v1;
        ResTy v2;
    } u = {v};
    return u.v2;
}


template <typename SrcTy>
struct auto_union_cast_helper {
    SrcTy v_;

    template <typename ResTy>
    inline operator ResTy() {return union_cast<ResTy>(v_);}
};


template <typename SrcTy>
auto_union_cast_helper<SrcTy> auto_union_cast(SrcTy v) {
    auto_union_cast_helper<SrcTy> h = {v};
    return h;
}


struct UTILITIES_EXPORT IQStringSetterDelegate {
    virtual ~IQStringSetterDelegate() {};
    virtual void execute(const QString&) = 0;
};


template<typename T>
class QStringSetterDelegateImpl : public IQStringSetterDelegate {
    typedef void (T::*Method)(const QString&);

public:
    QStringSetterDelegateImpl(T* obj, Method method)
        : obj_(obj), method_(method)
    {}
    virtual void execute(const QString& param) {(obj_->*method_)(param);}

private:
    T* obj_;
    Method method_;
};


class UTILITIES_EXPORT TranslationRules : public QMap<qlonglong, QPair<QSharedPointer<IQStringSetterDelegate>, QSharedPointer<iTranslationRule> > > {};


UTILITIES_EXPORT Translation translate(const char* context, const char* sourceText, const char* disambiguation = 0, int n = -1
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
                                       , QCoreApplication::Encoding encoding = QCoreApplication::CodecForTr
#endif
                                      );
UTILITIES_EXPORT QString Tr(const Translation& translateArgs);
UTILITIES_EXPORT Translation Plural(const Translation& translateArgs, int n);
UTILITIES_EXPORT void Retranslate(QObject* obj);
UTILITIES_EXPORT void RetranslateAll(QObject* obj);


template <typename Ty, typename MemberFunc, typename T>
QString DoSetTr(Ty* obj, MemberFunc memFunc, const T& trRule) {
    if (!obj) return QString();

    TranslationRules rules;
    QVariant variant = obj->property(kProperty);
    if (variant.canConvert<TranslationRules>())
        rules = variant.value<TranslationRules>();

    QSharedPointer<IQStringSetterDelegate> func(new QStringSetterDelegateImpl<Ty>(obj, memFunc));

    rules[union_cast<qlonglong>(memFunc)] = qMakePair(func, QSharedPointer<iTranslationRule>(new T(trRule)));
    obj->setProperty(kProperty, QVariant::fromValue(rules));

    // update translation for now as well
    QString text = trRule.GetText();
    func->execute(text);

    return text;
}


template <typename Ty, typename MemberFunc>
QString SetTr(Ty* obj, MemberFunc memFunc, const QString& text) {
    TextTranslationRule trRule(text);
    return DoSetTr(obj, memFunc, trRule);
}


// this one uses function object
template <typename T>
class TranslationRuleImpl : public iTranslationRule {
public:
    TranslationRuleImpl(const Translation& translateArgs, const T& func)
        : translate_args_(translateArgs), func_(func)
    {}
    inline QString GetText() const { return func_(Tr(translate_args_)); }

private:
    Translation translate_args_;
    T func_;
};


template <typename Ty, typename MemberFunc>
QString SetTr(Ty* obj, MemberFunc memFunc, const Translation& translateArgs, std::function<QString(const QString&)> expand) {
    TranslationRuleImpl<std::function<QString(const QString&)> > trRule(translateArgs, expand);
    return DoSetTr(obj, memFunc, trRule);
}


template <typename Ty, typename MemberFunc>
QString SetTr(Ty* obj, MemberFunc memFunc, const Translation& translateArgs) {
    TranslationRule trRule(translateArgs, QStringVector());
    return DoSetTr(obj, memFunc, trRule);
}


#define UTILS_ARGS_MACRO_TEMPL(v) typename ArgType##v
#define UTILS_ARGS_MACRO_DECL(v) const ArgType##v &arg##v
#define UTILS_ARGS_MACRO_IMPL(v) expand.append(asString( arg##v ))


#define UTILS_INVOKE_PARAMS_MACRO(macro) \
    template <typename Ty, typename MemberFunc, macro(UTILS_ARGS_MACRO_TEMPL)> \
    QString SetTr(Ty* obj, MemberFunc memFunc, const Translation& translateArgs, macro(UTILS_ARGS_MACRO_DECL)) { \
        QStringVector expand; \
        macro(UTILS_ARGS_MACRO_IMPL); \
        TranslationRule trRule(translateArgs, expand); \
        return DoSetTr(obj, memFunc, trRule); \
    }


#include "multiparametric.h"


#undef UTILS_ARGS_MACRO_TEMPL
#undef UTILS_ARGS_MACRO_DECL
#undef UTILS_ARGS_MACRO_IMPL


} // namespace Tr


UTILITIES_EXPORT QString locationString(const QString& fileName);
UTILITIES_EXPORT QString languageString(const Tr::Translation& translation, const QString& locName, QTranslator& translator);


} // namespace utilities


Q_DECLARE_METATYPE(utilities::Tr::TranslationRules)
