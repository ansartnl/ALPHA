
#include "plaintexteditdelegate.h"

#include <QtCore/QDebug>
#include <QtCore/QMimeData>

#include <QApplication>
#include <QPlainTextEdit>
#include <QScrollBar>
#include <QPainter>
#include <QAbstractTextDocumentLayout>
#include <QClipboard>

namespace{
QString compressHtml(const QString& html)
{
	QString result(html);

	QRegExp rexp("(<style.*>.*</style>"
								 "|<\\!DOC[^>]*>"
								 "|</?(html|a|address|big|blockquote|body|div|dl|dt|head|hr|html|img|kbd|meta|lo|ol|qt|"
											 "table|tr|td|th|title|ul)[^>]*>)");
	result.remove(rexp);
	result = result.trimmed();
	return result;
}
}

PlainTextEditDelegate::PlainTextEditDelegate(QObject* parent)
	:QStyledItemDelegate(parent)
{
	if (objectName().isEmpty())
		setObjectName("PlainTextEditDelegate");
}

// http://www.forum.crossplatform.ru/lofiversion/index.php? t2565.html
void PlainTextEditDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
														 const QModelIndex &index) const
{

	// Some part parent paint function
	QStyleOptionViewItemV4 v4 = option;
	initStyleOption(&v4, index);

	const QWidget *widget;
	widget = v4.widget;
	QStyle *style = widget ? widget->style() : QApplication::style();

	// Отключаем ChechStateRole и DisplayRole
	v4.features &= (~QStyleOptionViewItemV2::HasCheckIndicator);
	v4.features &= (~QStyleOptionViewItemV2::HasDisplay);
	//v4.features &= (~QStyleOptionViewItemV2::)
	//v4.checkState = Unchecked
	v4.text.clear();

	style->drawControl(QStyle::CE_ItemViewItem, &v4, painter, widget);

	// Paint html text
	QString str = index.data(Qt::DisplayRole).toString();
	QTextDocument td;
	td.setHtml(str);

	QStyleOptionViewItem opt = option;
	QVariant varAlign = index.data(Qt::TextAlignmentRole);
	Qt::Alignment align;
	if (varAlign.isValid())
		align = QFlag(varAlign.toInt());
	else
		align = Qt::AlignLeft;
	QRectF lr = layoutRect(td, opt.rect, align);

	painter->save();
	painter->translate(lr.topLeft());
	painter->setClipRect(lr.translated(-lr.x(), -lr.y()));
	td.drawContents(painter, QRectF());
	painter->restore();
}

QWidget *PlainTextEditDelegate::createEditor(QWidget *parent,
																				const QStyleOptionViewItem &/*option*/,
																				const QModelIndex &/*index*/) const
{
	QPlainTextEdit *textEdit = new QPlainTextEdit(parent);
	textEdit->setWordWrapMode(QTextOption::NoWrap);
	textEdit->verticalScrollBar()->hide();
	textEdit->horizontalScrollBar()->hide();
	textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//textEdit->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
	textEdit->installEventFilter(const_cast<PlainTextEditDelegate *>(this));
	//textEdit->setWordWrapMode();
	return textEdit;
}

void PlainTextEditDelegate::setEditorData(QWidget *editor,
																		 const QModelIndex &index) const
{
	QString str = index.data().toString();
	QPlainTextEdit *textEdit = qobject_cast<QPlainTextEdit*>(editor);
	textEdit->document()->setHtml(str);
	textEdit->selectAll();
}

void PlainTextEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
																		const QModelIndex &index) const
{
	QPlainTextEdit *textEdit = qobject_cast<QPlainTextEdit*>(editor);
	QString str = textEdit->document()->toHtml();
	str = compressHtml(str);
	//qDebug() << str;
	if (!model->setData(index, str))
		setEditorData(editor, index);
}

void PlainTextEditDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option
																						, const QModelIndex &/*index*/) const
{
	QPlainTextEdit *textEdit = qobject_cast<QPlainTextEdit*>(editor);
	textEdit->setGeometry(option.rect);
	//	textEdit->setGeometry(option.rect.topLeft().x(), option.rect.topLeft().y()
	//						  , textEdit->sizeHint().width(), textEdit->sizeHint().height());
}

QString PlainTextEditDelegate::displayText(const QVariant &/*value*/, const QLocale &/*locale*/) const
{
	return QString::null;
}

// prog.org.ru
bool PlainTextEditDelegate::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QEvent::KeyPress) {
		QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
		if (keyEvent){
			switch(keyEvent->key()){
			case Qt::Key_Enter:     // цифровая клавиатура
			case Qt::Key_Return:    // основная клавиатура
				{
					Qt::KeyboardModifiers kbrdMod = QApplication::keyboardModifiers();
					if (kbrdMod & Qt::ControlModifier) {
						emit commitData(static_cast<QWidget *>(object));
						emit closeEditor(static_cast<QWidget *>(object), QAbstractItemDelegate::EditPreviousItem);
						return true;
					}

					emit commitData(static_cast<QWidget *>(object));
					emit closeEditor(static_cast<QWidget *>(object), QAbstractItemDelegate::EditNextItem);
					return true;
				}
			default:
				break;
			}
			if (keyEvent->matches(QKeySequence::Paste)) {
				if (QApplication::clipboard()->mimeData()->hasText()) {
					QString data = QApplication::clipboard()->mimeData()->text();
					data.replace(QRegExp("(\n|\r\n)"), " ");
					QPlainTextEdit *editor = qobject_cast<QPlainTextEdit *>(object);
					if (editor) {
						editor->insertPlainText(data);
					}
				}
				return true;
			}
		}
	} else if (event->type() == QEvent::Drop) {
		QDropEvent *dropEvent = static_cast<QDropEvent*>(event);
		if (dropEvent) {
			QString data;
			if (dropEvent->mimeData()->hasText()) {
				data = dropEvent->mimeData()->text();
				data.replace(QRegExp("(\n|\r\n)"), " ");
				QPlainTextEdit *editor = qobject_cast<QPlainTextEdit *>(object);
				if (editor) {
					editor->insertPlainText(data);
				}
			}
			return true;
		}
	} else if (event->type() == QEvent::FocusOut) {
		//QFocusEvent *focusEvent = static_cast<QFocusEvent *> (event);
		emit commitData(qobject_cast<QWidget *>(object));
		return true;
	}
	return QStyledItemDelegate::eventFilter(object, event);
	//return false;
}

QRectF PlainTextEditDelegate::layoutRect(const QTextDocument & td, QRect rect, Qt::Alignment align) const
{
	//QRectF rectf;
	qreal xo = 0;
	qreal yo = 0;

	if(align & Qt::AlignLeft) {
		xo = 0;
	} else if(align & Qt::AlignRight) {
		qreal rw = td.documentLayout()->documentSize().width();
		xo = 0;
		xo = qMax((rect.width()-rw), qreal(0));
	} else if(align & Qt::AlignHCenter) {
		qreal rw = td.documentLayout()->documentSize().width();
		xo = 0;
		xo = qMax((rect.width()-rw)/2, qreal(0));
	}

	if(align & Qt::AlignTop) {
		yo = 0;
	} else if(align & Qt::AlignBottom) {
		qreal rh = td.documentLayout()->documentSize().height();
		yo = 0;
		yo = qMax((rect.height()-rh), qreal(0));
	} else if(align & Qt::AlignVCenter) {
		qreal rh = td.documentLayout()->documentSize().height();
		yo = 0;
		yo = qMax((rect.height()-rh)/2, qreal(0));
	}
	return QRectF(xo + rect.x(), yo + rect.y(), rect.width(), rect.height());
}
