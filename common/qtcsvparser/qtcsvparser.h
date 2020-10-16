/*dummy CSV reader for QT4*/
/*version 0.1*/
/*11.1.2009*/
#ifndef QTCSVPARSER_H
#define QTCSVPARSER_H

#if defined(QTCSVPARSER_EXPORTS)
#  define QTCSVPARSER_EXPORT Q_DECL_EXPORT
#elif defined(QTCSVPARSER_IMPORTS)
#  define QTCSVPARSER_EXPORT Q_DECL_IMPORT
#else
#  define QTCSVPARSER_EXPORT    /**/
#endif


#include <QtCore/QStringList>

/*!
	\ingroup parsers
	Парсер строки имеющей csv формат.
	\warning при включеенном trimmedSpace в данном случае:
	"   123   ", "adsf ", " asdfasd "
	вернет
	123,asdf,asdfasd
	вырезая все пробелы. Для данного приложения некритично, поэтому не исправляется.
*/
class QTCSVPARSER_EXPORT QtCsvParser
{
public:
	QtCsvParser();
	QtCsvParser(const QString& source);
	virtual ~QtCsvParser();

	void setSource(const QString& source);
	QString source() const;

	/*!
		Символ окончания строки. По умолчанию \\n
	* @param terminator
	*/
	void setTerminator(const QChar& terminator);
	QChar terminator() const;

	/*!
		Альтернативных два символа окончания строки. По умолчанию \\r\\n
  \param terminator
	*/
	void setAlternativeTerminator(const QString& terminator);
	QString alternativeTerminator() const;

	/*!
		Символ ограничивающий значение
  \param enclose
	*/
	void setEncloseChar(const QChar& enclose);
	QChar encloseChar()const;

	/*!
		Символ разделяющий значения
  \param delimiter
	*/
	void setDelimiter(const QChar& delimiter);
	QChar delimiter() const;

	/*!
		Начать с начала.
	*/
	void reset();

	/*!
		Прочесть одну строку
  \param trimSpace убирать пробелы в начале и в конце значения
  \return QStringList список полученных значений
	*/
	QStringList parseLine(bool trimSpace = true);

private:
	QString src;
	int pos;
	QChar delim;
	QChar encl;
	QChar term;
	QString altTerm;
};

QString toCsv(const QString& plainText, const QChar& delimiter);
QString toCsv(const QList<QStringList>& plainText, const QChar& delimiter);

#endif // QTCSVPARSER_H
