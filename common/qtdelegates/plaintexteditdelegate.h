#ifndef PLAINTEXTEDITDELEGATE_H
#define PLAINTEXTEDITDELEGATE_H

#include <QStyledItemDelegate>

class QTextDocument;
/*!
	\ingroup formatmodel
	Класс для отображения и редактирования форматируемого текста. В качестве форматирования используется часть html языка. В модели сохраняется html. Из модели может загружаться как plain text так и html text.

* @class PlainTextEditDelegate plaintexteditdelegate.h "src/kernel/delegates/plaintexteditdelegate.h"
*/
class PlainTextEditDelegate: public QStyledItemDelegate
{
	Q_OBJECT
public:
	explicit PlainTextEditDelegate(QObject* parent = 0);

	virtual void paint(QPainter *painter, const QStyleOptionViewItem &option,
				 const QModelIndex &index) const;

	virtual QWidget *createEditor(QWidget *parent
																		, const QStyleOptionViewItem &option
																		, const QModelIndex &index) const;

	virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;

	virtual void setModelData(QWidget *editor, QAbstractItemModel *model,
						const QModelIndex &index) const;

	virtual void updateEditorGeometry(QWidget *editor
																				, const QStyleOptionViewItem &option
																				, const QModelIndex &index) const;

	/*!
		Текст не отображаем, так он отрисовывается в методе paint
	* @param value
	* @param locale
	* @return QString always QString::null
	*/
	QString displayText(const QVariant &value, const QLocale &locale) const;

protected:
	virtual bool eventFilter(QObject *object, QEvent *event);
private:
	QRectF layoutRect(const QTextDocument & td, QRect rect, Qt::Alignment align) const;
};

#endif // PLAINTEXTEDITDELEGATE_H

