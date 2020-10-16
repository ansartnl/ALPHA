#include "qttranslator.h"
#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <QtCore/QTranslator>
#include <QtCore/QCoreApplication>
#include <QtCore/QLocale>
#include <QAction>

QtTranslator::TranslatorObject::TranslatorObject() : locale(0)
{}

QtTranslator::TranslatorObject::~TranslatorObject()
{
    qDeleteAll(translators);
    if (locale)
        delete locale;
}

QtTranslator *QtTranslator::self = 0;

QtTranslator::QtTranslator(QObject *parent) : QObject(parent)
                                            , defaultLocale(0)
                                            , globalDefaultLocale(0)
                                            , languageMark(QT_TR_NOOP("CURRENT LANGUAGE"))
                                            , localeMark(QT_TR_NOOP("CURRENT LOCALE"))
{
    self = this;
    languageGroup = new QActionGroup(this);
    defaultLanguage = new QAction(this);
    defaultLanguage->setCheckable(true);
    defaultLanguage->setChecked(true);
    defaultLanguage->setText("English");
    languageGroup->addAction(defaultLanguage);
    connect(defaultLanguage, SIGNAL(toggled(bool)), this, SLOT(installLanguage(bool)));
    loadLanguageFromSettings();
}

QtTranslator::~QtTranslator()
{
    saveLanguageIntoSettings();
    if (defaultLocale)
        delete defaultLocale;
    if (globalDefaultLocale)
        delete globalDefaultLocale;
    qDeleteAll(translationMap);
    self = 0;
}

QtTranslator *QtTranslator::instance()
{
    if (!self)
    {
        if (!QCoreApplication::instance())
        {
            qWarning("Unable to create QtTranslator: initialize QCoreApplication at first");
            return 0;
        }
        self = new QtTranslator(QCoreApplication::instance());
    }
    return self;
}

void QtTranslator::loadTranslations(const QDir &translationDir)
{
    // at first remove old translations
    defaultLanguage->setChecked(true);
    for (QMap<QAction *, TranslatorObject *>::iterator it = translationMap.begin(); it != translationMap.end(); ++it)
    {
        languageGroup->removeAction(it.key());
        it.key()->deleteLater();
    }
    qDeleteAll(translationMap);
    translationMap.clear();

    // load the new translations
    QStringList translationFiles = translationDir.entryList(QStringList(QCoreApplication::applicationName() + "_*.qm"));
    foreach (const QString &mainTranslationFile, translationFiles)
    {
        QScopedPointer<QTranslator> spMainTranslator(new QTranslator());
        if (spMainTranslator->load(mainTranslationFile, translationDir.absolutePath())) // loading main translator with pattern: applicationName_*.qm
        {
            QTranslator *mainTranslator = spMainTranslator.take();
            QAction *action = new QAction(this);
            action->setCheckable(true);
            action->setText(mainTranslator->translate(staticMetaObject.className(), languageMark)); // set language name as name of action
            languageGroup->addAction(action);

            QScopedPointer<TranslatorObject> traslatorForOneLanguage(new TranslatorObject());
            traslatorForOneLanguage->translators.append(mainTranslator);

            QString localeName = mainTranslator->translate(staticMetaObject.className(), localeMark); // search for locale
            if (!localeName.isEmpty())
                traslatorForOneLanguage->locale = new QLocale(localeName);

            // trying to find auxiliary translators with *_(TranslationPostfixFROMMainTranslationFile).qm
            QString translationPostfix = mainTranslationFile;
            translationPostfix.remove(0, mainTranslationFile.indexOf('_') + 1).chop(3);
            QStringList additionalTranslationFiles = translationDir.entryList(QStringList("*_" + translationPostfix + ".qm")); // loading additional translations with pattern: *_translationPostfix.qm
            foreach(const QString &additionalTranslationFile, additionalTranslationFiles)
            {
                if (additionalTranslationFile == mainTranslationFile) // skip if it main
                    continue;

                QScopedPointer<QTranslator> translator(new QTranslator());
                if (translator->load(additionalTranslationFile, translationDir.absolutePath()))
                {
                    QTranslator *t = translator.take();
                    if (action->text().isEmpty()) // if previous (including main) translation files don't contain name of language try to find it in the following
                        action->setText(t->translate(staticMetaObject.className(), languageMark));

                    if (!traslatorForOneLanguage->locale) // and try to find locale if it not contained by previous translation filed
                    {
                        QString localeName = t->translate(staticMetaObject.className(), localeMark);
                        if (!localeName.isEmpty())
                            traslatorForOneLanguage->locale = new QLocale(localeName);
                    }

                    traslatorForOneLanguage->translators.append(t); // append auxiliary translator
                }
            }

            translationMap.insert(action, traslatorForOneLanguage.take());
            connect(action, SIGNAL(toggled(bool)), this, SLOT(installLanguage(bool)));
        }
    }

    emit actionListChanged();
}

void QtTranslator::setDefaultLanguageName(const QString &defaultLanguageName)
{
    defaultLanguage->setText(defaultLanguageName);
}

void QtTranslator::setDefaultLocale(QLocale *locale)
{
    if (defaultLocale)
        delete defaultLocale;
    defaultLocale = locale;
    if (defaultLanguage->isChecked())
        setLocale(defaultLocale);
}

void QtTranslator::setGlobalDefaultLocale(QLocale *locale)
{
    if (globalDefaultLocale)
        delete globalDefaultLocale;
    globalDefaultLocale = locale;
    QAction *cur = languageGroup->checkedAction();
    if (cur == defaultLanguage)
        setLocale(defaultLocale);
    else if (translationMap.contains(cur))
        setLocale(translationMap[cur]->locale);
    else
        setLocale(globalDefaultLocale);
}

void QtTranslator::changeSettingPath(const QString &newSavePath)
{
    saveLanguageIntoSettings();
    savePath = newSavePath;
    if (!loadLanguageFromSettings())
        defaultLanguage->setChecked(true);
}

QList<QAction *> QtTranslator::languages() const
{
    return languageGroup->actions();
}

bool QtTranslator::setCurrentLanguage(const QString &lang)
{
    foreach (QAction *languageAction, languageGroup->actions())
    {
        if (languageAction->text() == lang)
        {
            languageAction->setChecked(true);
            return true;
        }
    }
    return false;
}

QString QtTranslator::currentLanguage() const
{
    return languageGroup->checkedAction()->text();
}

void QtTranslator::saveLanguageIntoSettings() const
{
    QSettings settings;
    if (!savePath.isEmpty())
        settings.beginGroup(savePath);
    settings.setValue("language", currentLanguage());
    if (!savePath.isEmpty())
        settings.endGroup();
}

bool QtTranslator::loadLanguageFromSettings()
{
    QSettings settings;
    if (!savePath.isEmpty())
        settings.beginGroup(savePath);
    bool isLangExisted = settings.contains("language");
    if (!savePath.isEmpty())
        settings.endGroup();
    return isLangExisted ? setCurrentLanguage(settings.value("language").toString()) : false;
}

void QtTranslator::installLanguage(bool enable)
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action == defaultLanguage)
    {
        if (enable)
            setLocale(defaultLocale);
    }
    else if (translationMap.contains(action))
    {
        TranslatorObject *translator = translationMap[action];
        if (enable)
        {
            foreach(QTranslator *t, translator->translators)
                QCoreApplication::installTranslator(t);
            setLocale(translator->locale);
        }
        else
        {
            foreach(QTranslator *t, translator->translators)
                QCoreApplication::removeTranslator(t);
        }
    }
}

void QtTranslator::setLocale(QLocale *locale)
{
    if (locale)
        QLocale::setDefault(*locale);
    else if (globalDefaultLocale)
        QLocale::setDefault(*globalDefaultLocale);
    else
        QLocale::setDefault(QLocale::system());
}
