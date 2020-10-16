#ifndef DB__POSTGRESQL_H
#define DB__POSTGRESQL_H

#include <QVariant>
#include "DBQuery.h"

namespace db
{
    //! Query for PostgreSQL database
    class CDBPostgreSQLQuery : public CDBQuery
	{
	public:
		//! Constructor
		/*!
			\param DB reference low-level database structure
		*/
        CDBPostgreSQLQuery(const QSqlDatabase& DB);
		virtual CDBQuery* Clone() const;
		virtual void Prepare(const QString& sStatement, const QString& sReturn = QString());
		virtual QVariant GetLastInsertID();
        virtual QString GetSystimestampStr() const;
        virtual QString GetFromDualStr() const;
        virtual QString GetSelectDbId() const;

	private:
		QVariant	m_LastID;
	};
}

#endif // DB__POSTGRESQL_H
