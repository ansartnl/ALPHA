#ifndef DATABASETRANSLATIONS_H
#define DATABASETRANSLATIONS_H

#include "aftnmessageprocessor.h"

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QHash>

class QSqlTableModel;
#ifdef AFTNWS_PRJ_APP
class OciQueryModel;
#endif


/**
* @brief Using for translation DB fields after select request
*
* @class Translations translations.h
*/
class Translations : public QObject
{
  Q_OBJECT

  static QHash<QString, QString> names;
  static QHash<QString, QHash<QString,QString> > tableColumnNames;
public:


  static void initialize();
  static QString columnTr(const QString& column);
  static void translateSqlTableModel(QSqlTableModel *model);
#ifdef AFTNWS_PRJ_APP
  static void translateOciSqlModel(OciQueryModel *model);
#endif

  static QString tableNameTr(const QString& tableName);

};

#endif //DATABASETRANSLATIONS_H
