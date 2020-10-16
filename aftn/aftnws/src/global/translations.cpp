#include "translations.h"
#ifdef AFTNWS_PRJ_APP
#include "ociquerymodel.h"
#endif

#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlRecord>

QHash<QString, QString> Translations::names;

QHash<QString, QHash<QString, QString> > Translations::tableColumnNames;

void Translations::initialize()
{
  names.clear();
  // FPL Table
  names.insert(TBL_FPL_FIR_NAME, tr("FIR"));
  names.insert(TBL_FPL_AIRCRAFT_ID, tr("Aircraft id"));
  names.insert(TBL_FPL_FRULES, tr("Flight rules"));
  names.insert(TBL_FPL_TFLIGHT, tr("Type of flight"));
  names.insert(TBL_FPL_NUM, tr("Aircraft amount"));
  names.insert(TBL_FPL_TAIRCRAFT, tr("Aircraft type"));
  names.insert(TBL_FPL_TURBULENCE_CATEGORY, tr("WTC"));
  names.insert(TBL_FPL_EQUIPMENT, tr("Equipment"));
  names.insert(TBL_FPL_ADEP, tr("Departure airdrome"));
  names.insert(TBL_FPL_TIME, tr("Departure time"));
  names.insert(TBL_FPL_SPEED, tr("Speed"));
  names.insert(TBL_FPL_FLEVEL, tr("Level"));
  names.insert(TBL_FPL_ROUTE, tr("Route"));
  names.insert(TBL_FPL_ADES, tr("Arrival airdrome"));
  names.insert(TBL_FPL_EET, tr("EET"));
  names.insert(TBL_FPL_ALT1, tr("ALTN aerodrome"));
  names.insert(TBL_FPL_ALT2, tr("2nd ALTN aerodrome"));
  names.insert(TBL_FPL_OTHER, tr("Other information"));
  names.insert(TBL_FPL_EXFPL, tr("Status"));

  names.insert(TBL_HIST_FPL_FIR_NAME, tr("FIR"));
  names.insert(TBL_HIST_FPL_AIRCRAFT_ID, tr("Aircraft id"));
  names.insert(TBL_HIST_FPL_FRULES, tr("Flight rules"));
  names.insert(TBL_HIST_FPL_TFLIGHT, tr("Type of flight"));
  names.insert(TBL_HIST_FPL_NUM, tr("Aircraft amount"));
  names.insert(TBL_HIST_FPL_TAIRCRAFT, tr("Aircraft type"));
  names.insert(TBL_HIST_FPL_WTC, tr("WTC"));
  names.insert(TBL_HIST_FPL_EQUIPMENT, tr("Equipment"));
  names.insert(TBL_HIST_FPL_ADEP, tr("Departure airdrome"));
  names.insert(TBL_HIST_FPL_TIME, tr("Departure time"));
  names.insert(TBL_HIST_FPL_SPEED, tr("Speed"));
  names.insert(TBL_HIST_FPL_FLEVEL, tr("Level"));
  names.insert(TBL_HIST_FPL_ROUTE, tr("Route"));
  names.insert(TBL_HIST_FPL_ADES, tr("Arrival airdrome"));
  names.insert(TBL_HIST_FPL_EET, tr("EET"));
  names.insert(TBL_HIST_FPL_ALT1, tr("ALTN aerodrome"));
  names.insert(TBL_HIST_FPL_ALT2, tr("2nd ALTN aerodrome"));
  names.insert(TBL_HIST_FPL_OTHER, tr("Other information"));

  names.insert(TBL_HIST_FPL_MODIFIED_BY, tr("Modified by"));
  names.insert(TBL_HIST_FPL_MODIFIED_AT, tr("Midified at"));
  names.insert(TBL_HIST_FPL_MODIFICATION_TYPE, tr("Modification type"));


  names.insert(TBL_NOTAM_SERIES, tr("Series"));
  names.insert(TBL_NOTAM_SNUMBER, tr("#"));
  names.insert(TBL_NOTAM_YEAR, tr("Year"));
  names.insert(TBL_NOTAM_NATURE, tr("Type"));
  names.insert(TBL_NOTAM_REPLACE_SERIES, tr("Series"));
  names.insert(TBL_NOTAM_REPLACE_SNUMBER,tr("#"));
  names.insert(TBL_NOTAM_REPLACE_YEAR, tr("Year"));
  names.insert(TBL_NOTAM_FIR, tr("FIR"));
  names.insert(TBL_NOTAM_SUBJECT, tr("Subject"));
  names.insert(TBL_NOTAM_CONDITION, tr("Condition"));
  names.insert(TBL_NOTAM_TRAFFIC, tr("Traffic"));
  names.insert(TBL_NOTAM_PURPOSE, tr("Purpose"));
  names.insert(TBL_NOTAM_SCOPE, tr("Scope"));
  names.insert(TBL_NOTAM_Q_LOWER_LIMIT, tr("Lower limit"));
  names.insert(TBL_NOTAM_Q_UPPER_LIMIT, tr("Upper limit"));
  names.insert(TBL_NOTAM_LOCATION, tr("Location"));
  names.insert(TBL_NOTAM_RADIUS, tr("Radius"));
  names.insert(TBL_NOTAM_AERODROME, tr("Aerodrome"));
  names.insert(TBL_NOTAM_VALID_FROM, tr("Valid from"));
  names.insert(TBL_NOTAM_VALID_UNTIL, tr("Valid until"));
  names.insert(TBL_NOTAM_SCHEDULE, tr("Schedule"));
  names.insert(TBL_NOTAM_DESCRIPTION, tr("Description"));
  names.insert(TBL_NOTAM_LOWER_LIMIT, tr("Lower limit"));
  names.insert(TBL_NOTAM_UPPER_LIMIT, tr("Upper limit"));

  names.insert(TBL_RPL_FIR_NAME, tr("FIR"));
  names.insert(TBL_RPL_AIRCRAFT_ID,tr("Aircraft id"));
  names.insert(TBL_RPL_FRULES, tr("Flight rules"));
  names.insert(TBL_RPL_TFLIGHT, tr("Type of flight"));
  names.insert(TBL_RPL_NUM, tr("Aircraft amount"));
  names.insert(TBL_RPL_TAIRCRAFT, tr("Aircraft type"));
  names.insert(TBL_RPL_TURBULENCE_CATEGORY, tr("Turbulence category"));
  names.insert(TBL_RPL_EQUIPMENT, tr("Equipment"));
  names.insert(TBL_RPL_ADEP, tr("Departure airdrome"));
  names.insert(TBL_RPL_TIME, tr("Departure time"));
  names.insert(TBL_RPL_SPEED, tr("Speed"));
  names.insert(TBL_RPL_FLEVEL, tr("Level"));
  names.insert(TBL_RPL_ROUTE, tr("Route"));
  names.insert(TBL_RPL_ADES, tr("Arrival airdrome"));
  names.insert(TBL_RPL_EET, tr("EET"));
  names.insert(TBL_RPL_ALT1, tr("ALTN aerodrome"));
  names.insert(TBL_RPL_ALT2, tr("2nd ALTN aerodrome"));
  names.insert(TBL_RPL_OTHER, tr("Other information"));
  names.insert(TBL_RPL_DAYS, tr("Days of week"));

  names.insert(TBL_ARC_AFTN_CREATED, tr("Created"));
  names.insert(TBL_ARC_AFTN_TEXT, tr("Source text"));

  names.insert(TBL_ARC_AFTN_ID, tr("Created"));
  names.insert(TBL_ARC_AFTN_TEXT, tr("Source text"));

  names.insert(TBL_ARC_AFTN_HEADER, tr("Header"));
  names.insert("message", tr("Message"));
  names.insert("addresses", tr("Addresses"));

  names.insert(TBL_RPL_VALID_FROM, tr("Valid from"));
  names.insert(TBL_RPL_VALID_TO, tr("Valid to"));

  names.insert(TBL_NOTAM_SEQUENCE_ID, tr("Sequence identifier"));
  names.insert(TBL_NOTAM_STATE, tr("State"));
  names.insert(TBL_NOTAM_VALID_UNTIL_T, tr("Valid until type"));

  names.insert("id", tr("Identifier"));
  names.insert("entry_fir_eet", tr("Entry in FIR EET"));

  names.insert("id_name", tr("ICAO String indentifier"));
  names.insert("parent_id", tr("Parent identifier"));

  names.insert("low_limit", tr("Parent identifier"));
  names.insert("up_limit", tr("Parent identifier"));

  names.insert("name", tr("String indentifier"));
  names.insert("level_max", tr("Maximum level"));

  names.insert("type", tr("Type"));

  names.insert("bank_max", tr("Maximum bank"));
  names.insert("bank_default", tr("Default bank"));

  names.insert("message_id", tr("Message identifier"));

  names.insert("hist_id", tr("History identifier"));
  names.insert("user_name", tr("User identifier"));
  names.insert("actual", tr("Actual"));
  names.insert("trtime", tr("Creation time"));


  names.insert("visibility", tr("Visibility"));


  names.insert("isactive", tr("Is active"));
  names.insert("fullname", tr("Full name"));

  // TABLES
  names.insert("airplane_type", tr("Airplane type"));
  names.insert("zone", tr("Zone"));
}

QString Translations::columnTr(const QString& column)
{
  return names.value(column.toLower(), column);
}

void Translations::translateSqlTableModel(QSqlTableModel* model)
{
    Translations::initialize();
  for (int i = 0, columnCount = model->record().count(); i < columnCount; ++i) {
    QString columnName = model->record().fieldName(i);
    int column = model->fieldIndex(columnName);
    model->setHeaderData(column, Qt::Horizontal, Translations::columnTr(columnName));
  }
}

#ifdef AFTNWS_PRJ_APP
void Translations::translateOciSqlModel(OciQueryModel* model)
{
  for (int i = 0, columnCount = model->record().count(); i < columnCount; ++i) {
    QString columnName = model->record().fieldName(i);
    int column = model->record().indexOf(columnName);
    model->setHeaderData(column, Qt::Horizontal, Translations::columnTr(columnName));
  }
}
#endif

QString Translations::tableNameTr(const QString &tableName)
{
  return names.value(tableName.toLower(), tableName);
}
