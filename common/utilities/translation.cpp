#include "translation.h"

#include <QtCore/QtGlobal>
#include <QString>
#include <QVariant>
#include <QCoreApplication>
#include <QDebug>
#include <QTranslator>
#include <QRegExp>


namespace utilities {


namespace Tr {


QString TranslationRule::GetText() const {
    return (expand_.isEmpty() ? Tr(translate_args_) : multiArg(Tr(translate_args_), expand_.size(), expand_.data()));
}


Translation translate(const char* context, const char* sourceText, const char* disambiguation /*= 0*/, int n /*= -1*/
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
                      , QCoreApplication::Encoding encoding /*= QCoreApplication::CodecForTr*/
#endif
                     ) {
    Translation tr = {context, sourceText, disambiguation, n
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
                      , encoding
#endif
                     };
    return std::move(tr);
}

QString Tr(const Translation& translateArgs) {
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    return QCoreApplication::translate(translateArgs.context, translateArgs.key, translateArgs.disambiguation,
                                       translateArgs.encoding, translateArgs.n);
#else
    return QCoreApplication::translate(translateArgs.context, translateArgs.key, translateArgs.disambiguation, translateArgs.n);
#endif
}

Translation Plural(const Translation& translateArgs, int n) {
    Translation plural = translateArgs;
    plural.n = n;
    return plural;
}

void Retranslate(QObject* obj) {
    if (!obj) return;

    QVariant variant = obj->property(kProperty);
    if (variant.canConvert<TranslationRules>()) {
        auto rules = variant.value<TranslationRules>();
        for (auto rule : rules)
            rule.first->execute(rule.second->GetText());
    }
}

void RetranslateAll(QObject* obj) {
    if (!obj) return;

    Retranslate(obj);

    auto objects = obj->findChildren<QObject*>();
    for (auto object : objects)
        RetranslateAll(object);
}


} // namespace Tr


QString languageString(const Tr::Translation& translation, const QString& locName, QTranslator& translator) {
    QLocale loc(locName);
    QString langStr = translator.translate(translation.context, translation.key, translation.disambiguation);
    if (langStr.isEmpty())
        langStr = QString("%1 (%2)").arg(
#if (QT_VERSION < QT_VERSION_CHECK(4,8,0))
                      QLocale::languageToString(loc.language())
#else
                      loc.nativeLanguageName()
#endif
                  ).arg(QLocale::languageToString(loc.language()));

    return std::move(langStr);
}

QString locationString(const QString& fileName) {
    QString locName;

    QRegExp rx("^.+_(\\S{2})\\.qm$", Qt::CaseInsensitive);
    if (rx.exactMatch(fileName))
        locName = rx.cap(1).toLower();

    return std::move(locName);
}


} // namespace utilities