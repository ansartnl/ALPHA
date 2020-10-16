#ifndef DB__DATABASE_H
#define DB__DATABASE_H

#include "DBQuery.h"
#include <QtCore/QSharedPointer>


//! Namespace contains classes to work with database
namespace db
{
	//! Main class to work with database
    class CDatabase
	{
	public:
		//! Constructor
		CDatabase();
        CDatabase(QSqlDatabase &dbSql);
        //! Initialization of database
		/*!
			\param sDB database type(ORACLE or MYSQL)
			\param sHost host name, where database is located
			\param sDBName database name
			\param sUser user name for database
			\param sPassword password for database
			\param sConnectionName a session name
		*/
		void Init(
			const QString& sDB,
			const QString& sHost,
			const QString& sDBName,
			const QString& sUser,
			const QString& sPassword,
            const QString  sConnectionName = QLatin1String(QSqlDatabase::defaultConnection));   // throw QString
		//! Get reference to low-level database class
		/*!
			\return low-level database class
		*/
        QSqlDatabase& GetDB();  // throw QString
		//! Query factory
		/*!
			\return pointer to query
		*/
		CDBQuery* CreateQuery() const;

        //! Create new database connection.
        CDatabase* Clone(const QString &connectionName) const;

	protected:
		QSqlDatabase				m_DB;
		QSharedPointer<CDBQuery>	m_Query;
	};

    //! public functions
    QSharedPointer<CDBQuery> CreateQuery(QSqlDatabase base = QSqlDatabase::database());

}

#endif // DB__DATABASE_H
