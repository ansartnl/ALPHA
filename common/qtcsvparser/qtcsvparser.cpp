#include "qtcsvparser.h"

QtCsvParser::QtCsvParser()
	:src(QString::null)
	, pos(0)
	, delim(QChar(','))
	, encl(QChar('"'))
	, term('\n')
	, altTerm("\r\n")
{

}

QtCsvParser::QtCsvParser(const QString& source)
	:src(source)
	, pos(0)
	, delim(QChar(','))
	, encl(QChar('"'))
	, term('\n')
	, altTerm("\r\n")
{
}

QtCsvParser::~QtCsvParser()
{
}

void QtCsvParser::setSource(const QString& source)
{
	pos = 0;
	src = source;
}

QString QtCsvParser::source() const
{
	return src;
}

void QtCsvParser::setTerminator(const QChar& terminator)
{
	term = terminator;
}

QChar QtCsvParser::terminator() const
{
	return term;
}

void QtCsvParser::setAlternativeTerminator(const QString& terminator)
{
	altTerm = terminator;
}

QString QtCsvParser::alternativeTerminator() const
{
	return altTerm;
}

void QtCsvParser::setEncloseChar(const QChar& enclose)
{
	encl = enclose;
}

QChar QtCsvParser::encloseChar() const
{
	return encl;
}

void QtCsvParser::setDelimiter(const QChar& delimiter)
{
	delim = delimiter;
}

QChar QtCsvParser::delimiter() const
{
	return delim;
}

void QtCsvParser::reset()
{
	pos = 0;
}

QStringList QtCsvParser::parseLine(bool trimSpace)
{
	QStringList resultRow;
	if ((pos >= src.size() - 1) || delim.isNull()
		|| ((term.isNull()) && (altTerm.isEmpty()))
		|| src.isEmpty())
		return resultRow;

	bool isEnclosed = false;

	QString value;

	QChar first;
	QChar second;
	QString firstSecond;

	int i = pos;
	// Цикл прохода по данным
	while (i < src.size()) {
		first = src.at(i);
		if (i < src.size() - 1)
			second = src.at(i + 1);
		else
			second = QChar();

		firstSecond.append(first);
		firstSecond.append(second);

		if (isEnclosed) {
			if (first == encl) {
				if (second == encl) {
					value.append(first);
					pos = ++i;
				} else {
					isEnclosed = false;
				}
			} else {
				value.append(first);
			}
		} else {
			if (first == encl) {
				if (second == encl) {
					value.append(first);
					pos = ++i;
				} else {
					isEnclosed = true;
				}
			} else if (first == delim) {
				if (trimSpace)
					value = value.trimmed();
				if (!value.compare(encl))
					value.clear();
				resultRow.append(value);
				value.clear();
			} else if (first == term) {
				if (trimSpace)
					value = value.trimmed();
				if (!value.compare(encl))
					value.clear();
				resultRow.append(value);
				value.clear();
				pos = (++i);
				break;
			} else if (!firstSecond.compare(altTerm)) {
				if (trimSpace)
					value = value.trimmed();
				if (!value.compare(encl))
					value.clear();
				resultRow.append(value);
				value.clear();
				pos = (i+=2);
				break;
			} else {
				value.append(first);
			}
		}

		pos = ++i;
		firstSecond.clear();
	}
	if (!value.isEmpty() || isEnclosed || first == delim) {
		if (trimSpace)
			value = value.trimmed();
		if (!value.compare(encl))
			value.clear();
		resultRow.append(value);
	}
	return resultRow;
}


QString toCsv(const QString &plainText, const QChar& delimiter)
{
	QString result = plainText;
	result.replace("\"", "\"\"");
	result.replace("\t", QString("\"%1 \"").arg(delimiter));
	result.replace("\n", "\"\n\"");
	result.prepend("\"");
	result.append("\"");
	return result;
}

QString toCsv(const QList<QStringList>& plainText, const QChar& delimiter)
{
	QString result;
	foreach(QStringList row, plainText) {
		foreach(QString value, row) {
			value.replace("\"", "\"\"");
			value.replace(QRegExp("(\r\n|\n)"), " ");
			result.append("\"");
			result.append(value);
			result.append("\"");
			result.append(delimiter);
		}
		result.chop(1);
		result.append("\n");
	}
	return result;
}
