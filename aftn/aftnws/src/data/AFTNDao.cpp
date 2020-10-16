#include "AFTNDao.h"

#include <QtDebug>

#include <QtCore/QVariant>
#include <QtCore/QString>
#include <QtCore/QStringList>

#include <QtSql/QSqlQuery>

#include "fpl.h"
#include "rpl.h"
#include "notam.h"

QSqlError AFTNDao::loadFPL(FPL& Fpl)
{
  QSqlQuery query;
  QSqlError error;

  Fpl.prepareSelect(query);

  if (query.exec()) {
    if (!query.next()) {
      error = query.lastError();
    }
  } else {
    error = query.lastError();
  }

  Fpl.updateInfo(query);
  return error;
}


QSqlError AFTNDao::saveFPL(FPL& Fpl)
{
  QSqlQuery query;

  findFPL(Fpl);

  if (Fpl.isNew())
    Fpl.prepareInsert(query);
  else
    Fpl.prepareUpdate(query);

  if (!query.exec()) {
    QSqlError error = query.lastError();
    return error;
  }

  if (Fpl.isNew())
    Fpl.updateId(query);

  QSqlQuery query3;

  if (query3.exec("SELECT DISTINCT client FROM oldi_clients"))
  {
      while (query3.next())
      {
          QSqlQuery query2;

          query2.prepare("INSERT INTO oldi_fpl VALUES (?, ?, ?, ?)");
          query2.addBindValue(Fpl.aircarftId());
          query2.addBindValue(Fpl.departureAerodrome());
          query2.addBindValue(Fpl.destinationAerodrome());
          query2.addBindValue(query3.record().value("client"));

          if (!query2.exec())
          {
              qDebug() << query2.lastError().text();
          }
      }
  } else
      qDebug() << query3.lastError().text();


  return QSqlError();
}

QSqlError AFTNDao::findFPL(FPL& Fpl)
{
    if (Fpl.isNew())
    {
        QSqlQuery query;
        query.prepare("SELECT id FROM fpl WHERE aircraft_id = ? AND adep = ? AND ades = ? AND time = ?");
        query.addBindValue(Fpl.aircarftId());
        query.addBindValue(Fpl.departureAerodrome());
        query.addBindValue(Fpl.destinationAerodrome());
        query.addBindValue(Fpl.departureDateTime());
        if (!query.exec()) {
          QSqlError error = query.lastError();
          qDebug() << error.text();
          return error;
        }
        if (query.next())
        {
            Fpl.setId(query.value(0).toInt());
        }
    }
    return QSqlError();
}

QSqlError AFTNDao::loadRPL(RPL& Rpl)
{
  QSqlQuery query;

  Rpl.prepareSelect(query);

  if (!query.exec()) {
    QSqlError error = query.lastError();
    return error;
  }

  if (!query.next()) {
    QSqlError error = query.lastError();
    return error;
  }

  Rpl.updateInfo(query);

  return QSqlError();
}

QSqlError AFTNDao::saveRPL(RPL& Rpl)
{
  QSqlQuery query;

  if (Rpl.isNew())
    Rpl.prepareInsert(query);
  else
    Rpl.prepareUpdate(query);

  if (!query.exec()) {
    QSqlError error = query.lastError();
    return error;
  }

  if (Rpl.isNew())
    Rpl.updateId(query);

  return QSqlError();
}

QSqlError AFTNDao::loadNOTAM(Notam& Notam)
{
  QSqlQuery query;

  Notam.prepareSelect(query);

  if (!query.exec()) {
    QSqlError error = query.lastError();
    return error;
  }

  if (!query.next()) {
    QSqlError error = query.lastError();
    return error;
  }

  Notam.updateInfo(query);

  return QSqlError();
}
