#ifndef LINGVIST_H
#define LINGVIST_H

#include <QtCore/QObject>
#include <QtCore/QMap>

class QTranslator;

/// Provide global language managment
/*! Use this class for implementation language switch. There are all UI windows wich have reimplemented 'protected virtual void languageChange();' inhireted from QWidged will be redrowen with new language settings through QEvent::LanguageChange*/
/*! How to add new laguage (for example Japanese). Copy qt_ja_JP.qm to translation folder. Perform 'lupdate -no-obsolete -recursive ../src ../../aftnmessageprocessor -ts aftnws_jp.ts' to get translation file. Translate ts file. Tranlate "Lingvist"->"translate this row into current language name" as "Japanese". Compile ts into qm. Language will be added automaticly.*/
class Lingvist : public QObject
{
  Q_OBJECT

  Q_PROPERTY(QString currentLanguage READ currentLanguage WRITE setCurrentLanguage)
public:
  Lingvist(QObject* parent);
  virtual ~Lingvist();

  QStringList languages() const; /*!< Retrieve all availiable languages */
  inline const QString& currentLanguage() const {
    return mCurrentLanguage;  /*!< Return current language */
  }

public slots:
  void setCurrentLanguage(const QString& newLang); /*!< \param newLang switch to 'newLang' language. If newLang is invalid or doesn't exist, switch to default language (English)*/
private:
  QList<QTranslator*> installedTranslations;
  QString mCurrentLanguage;
  QMap<QString, QString> languageFiles; /*!< Language-extension */
  static const QString defaultLanguage;
};

///Global singelton pointer. Init it after creating QApplication.
Lingvist* lingvist();

#endif // LINGVIST_H
