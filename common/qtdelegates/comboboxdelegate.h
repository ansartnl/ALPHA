#ifndef COMBOBOXDELEGATE_H
#define COMBOBOXDELEGATE_H


#include <QtCore/QModelIndex>
#include <QtCore/QStringList>

#include <QStyledItemDelegate>


class ComboBoxDelegatePrivate;
/*!
	\ingroup delegates
	Класс делегата выпадающего списка.
* @class ComboBoxDelegate
*/
class ComboBoxDelegate : public QStyledItemDelegate
{
	Q_OBJECT
    Q_DECLARE_PRIVATE(ComboBoxDelegate)

	// ItemDelegate
	/*!
		Редактируемый ComboBox
	\property editable
	*/
	Q_PROPERTY(bool editable READ isEditable WRITE setEditable)
	Q_PROPERTY(bool autoPopup READ autoPopup WRITE setAutoPopup)
	/*!
	*/
	Q_PROPERTY(int endEditHint READ endEditHint WRITE setEndEditHint)

	// SqlQuery
	/*!
		Столбец вспомогательной модели, значение которого используется будет вставляться в главную модель
	\property keyColumn
	*/
	Q_PROPERTY(int keyColumn READ keyColumn WRITE setKeyColumn)
	/*!
		Столбец вспомогательной модели, значение которого отображается.
	\property displayColumn
	*/
	Q_PROPERTY(int valueColumn READ valueColumn WRITE setValueColumn)

	/*!
	Hide key column
	*/
	Q_PROPERTY(bool hideKeyColumn READ isHideKeyColumn WRITE setHideKeyColumn)

	// LineEdit
	Q_PROPERTY(QString regExpValidatorString READ regExpValidatorString WRITE setRegExpValidatorString)
	Q_PROPERTY(bool enableCompleter READ isCompleterEnabled WRITE enableCompleter)
	/*!
		Маска ввода
	\property inputMask
	*/
	Q_PROPERTY(QString inputMask READ inputMask WRITE setInputMask)

	/*!
	Hide horizontal header
	*/
	Q_PROPERTY(bool hideHorizontalHeader READ isHideHorizontalHeader WRITE setHideHorizontalHeader)


	// ComboBox
protected:
	ComboBoxDelegate(ComboBoxDelegatePrivate &dd, QObject* parent);
	ComboBoxDelegatePrivate * const d_ptr;
public:

	ComboBoxDelegate(QObject *parent = 0);
	virtual ~ComboBoxDelegate();

	/*!
		Создает выпадающий список, с заданными ранее параметрами.
	* @param parent
	* @param option
	* @param index
	* @return QWidget
	*/
	QWidget *createEditor(QWidget *parent
																		, const QStyleOptionViewItem &option
																		, const QModelIndex &index) const;
	void setEditorData(QWidget *editor, const QModelIndex &index)const;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index)const;

	/*!
		Установка модели для выпадающего списка.
	* @param model
	*/
	void setModel(QAbstractItemModel* model);
	QAbstractItemModel* model() const;

	/*!
		Установка корневого индекса для модели.
	* @param index
	*/
	void setRootModelIndex(const QModelIndex& index);
	QModelIndex rootModelIndex() const;


	int keyColumn() const;

	int valueColumn() const;

	void setEditable(bool editable = true);
	bool isEditable() const;

	/*!
		Автоматически показывать выпадающий список.
	* @param popup
	*/
	void setAutoPopup(bool popup = true);
	bool autoPopup() const;

	/*!
		Переходить к другой ячейки при завершении редактирования.
	* @param hint
	*/
	void setEndEditHint(int hint);
	int endEditHint() const;

	bool isHideKeyColumn() const;
	void setHideKeyColumn(bool hideKeyColumn);

	/*!
		Регулярное выражение для проверки введенных данных
	* @return QString
	*/
	QString regExpValidatorString() const;
	/*!
		Наличие автоматического завершения вводимых данных
	* @return bool
	*/
	bool isCompleterEnabled() const;
	void enableCompleter(bool enable = true);

	QString inputMask() const;

	bool isHideHorizontalHeader() const;

public slots:
	void setHideHorizontalHeader(bool hideHorizontalHeader = true);

	void setKeyColumn(int column);
	void setValueColumn(int column);

	void setRegExpValidatorString(const QString& regExpression);

	void setInputMask(const QString& mask);

protected slots:
	void comboBoxCurrentIndexChanged(int current);

protected:
	QRect popupGeometry(int screen) const;
};

#endif // DATEDELEGATE_H
