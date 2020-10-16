#ifndef VIEWDELEGATE_H
#define VIEWDELEGATE_H

#include <QItemDelegate>
#include <QtSql/QSqlRelationalDelegate>

//! Class extends QItemDelegate
class CModifyDelegate
	: public QItemDelegate
{
	Q_OBJECT
public:
	CModifyDelegate(QObject* pObject = 0);

	//! Inherited from QItemDelegate
	virtual QWidget* createEditor(
			QWidget* parent,
			const QStyleOptionViewItem & option,
			const QModelIndex & index ) const;

signals:
	void change() const;

};

//! Class extends QSqlRelationalDelegate
class CRelModifyDelegate
	: public QSqlRelationalDelegate
{
	Q_OBJECT
public:
	typedef QMap<int, QVariant::Type> TColMap;

	CRelModifyDelegate(QObject* pObject = 0);

	//! Set item's type for column
	/*!
		\param colMap a map QVariant::Type for columns
	*/
	void SetColsType(const TColMap& colMap) { m_mapColType = colMap; }

	//! Inherited from QSqlRelationalDelegate
	virtual QWidget* createEditor(
			QWidget* parent,
			const QStyleOptionViewItem & option,
			const QModelIndex & index ) const;
    virtual void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

private:
	TColMap m_mapColType;
};

//! Class extends CRelModifyDelegate
class CNullDelegate:
		public CRelModifyDelegate
{
public:
	CNullDelegate(QObject* pObject = 0);

	//! Inherited from CRelModifyDelegate
	virtual QWidget* createEditor(
			QWidget* parent,
			const QStyleOptionViewItem & option,
			const QModelIndex & index ) const;


};

#endif // VIEWDELEGATE_H
