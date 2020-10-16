#include "Database.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QLibrary>
#include "DBMySQL.h"
#include "DBOCI.h"
#include "DBPostgreSQL.h"

namespace db
{
	CDatabase::CDatabase()
	{

	}

    CDatabase::CDatabase(QSqlDatabase &dbSql)
    {
        m_DB = dbSql;

        if ( !m_DB.isValid() || !m_DB.isOpen() )
            throw QObject::tr("Database driver is not valid or database was not open yet");

        QString sDriver = m_DB.driverName();
        if ( sDriver == "QMYSQL" )
            m_Query = QSharedPointer<CDBQuery>(new CDBMYSQLQuery(m_DB));
        else if ( sDriver == "QOCI" )
            m_Query = QSharedPointer<CDBQuery>(new CDBOCIQuery(m_DB));
        else if ( sDriver == "QPSQL" )
            m_Query = QSharedPointer<CDBQuery>(new CDBPostgreSQLQuery(m_DB));
        else
            throw QString(QObject::tr("Unsupported database type '%1'")).arg( m_DB.driverName() );
    }

	void CDatabase::Init(
		const QString& sDB,
		const QString& sHost,
		const QString& sDBName,
		const QString& sUser,
		const QString& sPassword,
		const QString  sConnectionName)
	{
        QString sDBUp = sDB.toUpper();
		if (sDBUp == QLatin1String("ORACLE"))
		{
            // fix Oracle bug for heap error, obsolete
            //QLibrary lib("oci");
			//lib.load();
			
			m_DB = QSqlDatabase::addDatabase(QLatin1String("QOCI"), sConnectionName);
			m_Query = QSharedPointer<CDBQuery>(new CDBOCIQuery(m_DB));
        } else if (sDBUp == QLatin1String("MYSQL"))
        {
            m_DB = QSqlDatabase::addDatabase(QLatin1String("QMYSQL"), sConnectionName);
            m_Query = QSharedPointer<CDBQuery>(new CDBMYSQLQuery(m_DB));
        } else if (sDBUp == QLatin1String("POSTGRESQL"))
        {
            m_DB = QSqlDatabase::addDatabase(QLatin1String("QPSQL"), sConnectionName);
            m_Query = QSharedPointer<CDBQuery>(new CDBPostgreSQLQuery(m_DB));
        } else
            throw QString(QObject::tr("Unknown database '%1'")).arg(sDB);

		m_DB.setHostName(sHost);
		m_DB.setDatabaseName(sDBName);
		m_DB.setUserName(sUser);
		m_DB.setPassword(sPassword);

        if ( !m_DB.open() )
            throw QObject::tr("Unable to open database");
	}


	QSqlDatabase& CDatabase::GetDB()
	{
        // If connection is not open - try to open it first.
        // Do workaround for this - making transaction to check if there is a connection
        // to database.
        if ( !m_DB.transaction() )
        {
            m_DB.close();
            if ( !m_DB.open() )
                throw QObject::tr("Unable to open database");
        }
        m_DB.rollback();

        return m_DB;
	}

    CDBQuery* CDatabase::CreateQuery() const
	{
		return m_Query->Clone();
	}

    CDatabase* CDatabase::Clone(const QString &connectionName) const
    {
        CDatabase* database = new CDatabase();
        database->m_DB = QSqlDatabase::cloneDatabase(m_DB, connectionName);

        database->m_Query = QSharedPointer<CDBQuery>(m_Query->Clone());

        if ( !database->m_DB.open() )
            throw QObject::tr("Unable to open database");

        return database;
    }

    QSharedPointer<CDBQuery> CreateQuery(QSqlDatabase base/* = QSqlDatabase()*/)
    {
        QSharedPointer<CDBQuery> query;
        QString sDriver = base.driverName();

        if ( sDriver == "QMYSQL" )
            query = QSharedPointer<CDBQuery>(new CDBMYSQLQuery(base));
        else if ( sDriver == "QOCI" )
            query = QSharedPointer<CDBQuery>(new CDBOCIQuery(base));
        else if ( sDriver == "QPSQL" )
            query = QSharedPointer<CDBQuery>(new CDBPostgreSQLQuery(base));
        else
            query = QSharedPointer<CDBQuery>(new CDBQuery(base));

        return query;
    }
}
