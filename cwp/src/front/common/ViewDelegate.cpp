#include "ViewDelegate.h"
#include <QItemEditorFactory>
#include <QDoubleSpinBox>
#include "QTime"

// class CModifyDelegate 
CModifyDelegate::CModifyDelegate(QObject* pObject)
	: QItemDelegate(pObject)
{

}

QWidget* CModifyDelegate::createEditor(
		QWidget* parent,
		const QStyleOptionViewItem & option,
		const QModelIndex & index ) const
{
	emit change();
	return QItemDelegate::createEditor(parent, option, index);
}


// class CRelModifyDelegate
CRelModifyDelegate::CRelModifyDelegate(QObject* pObject)
	: QSqlRelationalDelegate(pObject)
{

}

QWidget* CRelModifyDelegate::createEditor(
		QWidget* parent,
		const QStyleOptionViewItem & option,
		const QModelIndex & index ) const
{
	const QSqlRelationalTableModel *sqlModel = qobject_cast<const QSqlRelationalTableModel *>(index.model());
	QSqlTableModel *childModel = sqlModel ? sqlModel->relationModel(index.column()) : 0;
	if (!childModel)
	{
		if (!index.isValid())
			return 0;
		QVariant::Type t = static_cast<QVariant::Type>(index.data(Qt::EditRole).userType());
		TColMap::const_iterator itFind = m_mapColType.find(index.column());
		if(itFind != m_mapColType.end())
			t = itFind.value();
		const QItemEditorFactory *factory = QItemEditorFactory::defaultFactory();
		QWidget* pEditor = factory->createEditor(t, parent);
		if(dynamic_cast<QDoubleSpinBox*>(pEditor))
			dynamic_cast<QDoubleSpinBox*>(pEditor)->setDecimals(4);
		return pEditor;
	}

	QComboBox *combo = new QComboBox(parent);
	combo->setModel(childModel);
	combo->setModelColumn(childModel->fieldIndex(sqlModel->relation(index.column()).displayColumn()));
	combo->installEventFilter(const_cast<CRelModifyDelegate *>(this));

	return combo;
}

void CRelModifyDelegate::paint(QPainter *painter,
           const QStyleOptionViewItem &option,
           const QModelIndex &index) const
{
    /*
    TColMap::const_iterator itFind = m_mapColType.find(index.column());
    if(itFind != m_mapColType.end())
    {
        QVariant var_orig(index.data(Qt::DisplayRole));
        QVariant var_mod(var_orig);
        if((static_cast<QVariant::Type>(var_orig.userType()) != itFind.value()) && var_mod.canConvert(itFind.value()))
        {
            var_mod.convert(itFind.value());

//            switch(itFind.value())
//            {
//            case QVariant::Time:
//                {
//                    QTime dt = var_mod.toTime();
//                    if(!dt.isNull())
//                    {
//                        QString sText = dt.toString("hh:mm");
//                        var_mod.clear();
//                        var_mod = QVariant::fromValue(sText);
//                    }
//                }
//                break;
//            default:
//                break;
//            }

            const_cast<QAbstractItemModel*>(index.model())->setData(index, var_mod, Qt::EditRole);
        }
    }
    */
    QSqlRelationalDelegate::paint(painter, option, index);
}


// class CNullDelegate
CNullDelegate::CNullDelegate(QObject* pObject)
	: CRelModifyDelegate(pObject)
{

}

QWidget* CNullDelegate::createEditor(
		QWidget* parent,
		const QStyleOptionViewItem & option,
		const QModelIndex & index ) const
{
	QWidget* pWidget = CRelModifyDelegate::createEditor(parent, option, index);
	QComboBox* pCombo = dynamic_cast<QComboBox*>(pWidget);
	if (pCombo)
	{
		pCombo->addItem("_NULL_", QVariant());
	}
	return pWidget;

}

