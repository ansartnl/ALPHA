#ifndef TEXTEDITDELEGATE_H
#define TEXTEDITDELEGATE_H

#include <QStyledItemDelegate>

class QTextDocument;
/*!
	\ingroup delegates
	Класс аналогичен PlainTextEdit.
	\sa PlainTextEdit
* @class TextEditDelegate texteditdelegate.h "src/kernel/delegates/texteditdelegate.h"
*/
class TextEditDelegate: public QStyledItemDelegate
{
	Q_OBJECT
public:
	explicit TextEditDelegate(QObject* parent = 0);

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

	virtual QString displayText(const QVariant &value, const QLocale &locale) const;

	virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

protected:
	virtual bool eventFilter(QObject *object, QEvent *event);
private:
	QRectF layoutRect(const QTextDocument & td, QRect rect, Qt::Alignment align) const;
};

#endif // TEXTEDITDELEGATE_H

