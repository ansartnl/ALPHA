#ifndef DB__MYSQL_H
#define DB__MYSQL_H

#include <QMap>
#include "DBQuery.h"

namespace db
{
	//! Query for MySQL database
    class CDBMYSQLQuery : public CDBQuery
	{
	public:
		//! Map for placeholders
		typedef QMap<QString, int> TPlaceMap;
		//! Constructor
		/*!
			\param DB reference low-level database structure
		*/
		CDBMYSQLQuery(const QSqlDatabase& DB);
		virtual CDBQuery* Clone() const;
		virtual void Prepare(const QString& sStatement, const QString& sReturn = QString());
        virtual void BindValue(const QString& placeholder, const QVariant& val, QSql::ParamType type = QSql::In);
        virtual QString GetSystimestampStr() const;

	protected:
		//! Map with placeholder from the query
		TPlaceMap	m_PlaceHolders;
	};
}
#endif // DB__MYSQL_H
