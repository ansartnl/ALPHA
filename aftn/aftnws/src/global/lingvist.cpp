#include "lingvist.h"
#include "translations.h"

#include <QtCore/QDir>
#include <QtCore/QTranslator>
#include <QtCore/QSettings>
#include <QtCore/QCoreApplication>
#include <QtCore/QLocale>

Lingvist* lingvist()
{
  static Lingvist* mLingvist = new Lingvist(0);
  return mLingvist;
}

const QString Lingvist::defaultLanguage("English");

Lingvist::Lingvist(QObject* parent) : QObject(parent), mCurrentLanguage(defaultLanguage)
{
  QDir translationDir("translations:");
  QStringList translations = translationDir.entryList(QStringList(QCoreApplication::applicationName() + "_*.qm"));
  foreach(QString translateFileName, translations) {
    QTranslator translator;
    translator.load(translateFileName, translationDir.absolutePath());
    QString language = translator.translate("Lingvist", "translate this row into current language name");

    if (!language.isEmpty()) {
      translateFileName.remove(0, translateFileName.indexOf('_') + 1);
      translateFileName.chop(3);
      languageFiles[language] = translateFileName;
    }
  }

  QSettings settings;
  setCurrentLanguage(settings.value("language").toString());
}

Lingvist::~Lingvist()
{
  qDeleteAll(installedTranslations);
  installedTranslations.clear();
}

void Lingvist::setCurrentLanguage(const QString& language)
{
  if (language == mCurrentLanguage)
    return;

  foreach(QTranslator* translator, installedTranslations)
  QCoreApplication::removeTranslator(translator);

  qDeleteAll(installedTranslations);
  installedTranslations.clear();
  QString newLanguage(defaultLanguage);

  if (language != defaultLanguage) {
    QMap<QString, QString>::const_iterator it = languageFiles.find(language);

    if (it != languageFiles.end() && it.key() == language) {
      QDir translationDir("translations:");
      QStringList translationFiles = translationDir.entryList(QStringList("*_"+ it.value() +".qm"));
      foreach(const QString &translationFile, translationFiles) {
        QTranslator* translator = new QTranslator;
        translator->load(translationFile, translationDir.absolutePath());
        QCoreApplication::installTranslator(translator);
        installedTranslations.push_back(translator);

        QLocale locale(it.value());
        QLocale::setDefault(locale);
      }

      if (!installedTranslations.isEmpty())
        newLanguage = language;
    }
  } else {
    QLocale::setDefault(QLocale::c());
  }

  if (newLanguage != mCurrentLanguage) {
    mCurrentLanguage = newLanguage;
    QSettings settings;
    settings.setValue("language", mCurrentLanguage);
    // Reinitializtaion all coded strings
    Translations::initialize(); //!< crutch
  }
}

QStringList Lingvist::languages() const
{
  QStringList result(defaultLanguage);
  result << languageFiles.keys();
  return result;
}
