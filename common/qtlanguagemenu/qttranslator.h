#ifndef QTTRANSLATOR_H
#define QTTRANSLATOR_H

#include <QtCore/QMap>
#include <QtCore/QObject>

class QDir;
class QAction;
class QActionGroup;
class QTranslator;
class QLocale;

/*!
  * \class QtTranslator
  * \brief Класс-переводчик
  *
  * Данный класс реализует динамический перевод интерфейса программы.
  */

class QtTranslator : public QObject
{
    Q_OBJECT
    QtTranslator(QObject *parent);
public:
    virtual ~QtTranslator();

    static QtTranslator *instance(); // instance of the singelton.
    void loadTranslations(const QDir &translationDir); // load translation files from dir and drop curent language to default.
    void setDefaultLanguageName(const QString &defaultLanguageName); // change the default language of the program. By defauld Default language is "English".
    void setDefaultLocale(QLocale *locale); // set locale for default language
    void setGlobalDefaultLocale(QLocale *locale); // set locale for all translations without locale set
    void changeSettingPath(const QString &savePath); // switch to other settings by path. Save curent language to old save path and restore language from new path.
    QList<QAction *> languages() const; // list of language acions
    QString currentLanguage() const; // get name of curent language
    bool setCurrentLanguage(const QString &lang); // set language by name
    void saveLanguageIntoSettings() const; // save current language into savePath
    bool loadLanguageFromSettings(); // load language from curent savePath

signals:
    void actionListChanged(); // languages were reloaded. Be careful and reload actions.

private slots:
    void installLanguage(bool enable);

private:
    void setLocale(QLocale *locale);

    struct TranslatorObject
    {
        TranslatorObject();
        virtual ~TranslatorObject();
        QList<QTranslator *> translators;
        QLocale *locale;
    };

    QMap<QAction *, TranslatorObject *> translationMap;
    QString savePath;
    QAction *defaultLanguage;
    QLocale *defaultLocale;
    QLocale *globalDefaultLocale;
    QActionGroup *languageGroup;

    const char *languageMark;
    const char *localeMark;

    static QtTranslator *self;

};

// HOW TO USE:
//
// QtTranslator::instance()->loadTranslations(QDir("translations:"));
// QtTranslator::instance()->loadLanguageFromSettings();
// mainMenuBar->addMenu(new QtLanguageMenu(this));
//

#endif // QTTRANSLATOR_H
