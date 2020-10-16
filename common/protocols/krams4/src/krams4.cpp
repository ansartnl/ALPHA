#include "krams4.h"


#include <QtCore/QStringList>
#include <QtCore/QDateTime>

/*!
    Вид информационного сообщения:

    <SOH>'ID#hh:mm:ss#DD-MM-YYYY'<CR><LF>
    <STX>DATA<ETX>CRC<CR><LF><EOT>

    где:
    ID – идентификатор сообщения в виде: KRAMS#V2.0
    hh – часы, mm  - минуты, ss – секунды,  DD – дата,  MM – месяц, YYYY – год.
    DATA – данные метеорологических наблюдений за погодой на аэродроме (формат описан далее).
    CRC – контрольная сумма, вычисляемая по следующему алгоритму:

    Установить CRC=0;
    Для каждого символа поля данных (с первого после <STX>, не включая <ETX>)
    CRC = CRC  XOR символ данных;

    Вычисленное значение CRC записывается в виде трехзначного десятичного числа (трех символов ASCII), выровненного по правой границе.
    Пример: 105
    Пример: #79

    Вся информация «информационного сообщения» записана в символьном виде (коды 0-127 ASCII).
*/

// Protocol name and version
static const QString KRAMS = "KRAMS";
static const QString VERSION = "V2.0";

// Parser function pointer declaration
typedef QVariant (*parameterParser)(const QString &, QList<TKramsError> &);
// Generator function pointer declaration
typedef QString (*parameterGenerator)(const QVariant &, QList<TKramsError> &);


// Data parameters
const QString Krams4::DateTime = "DateTime";
const QString Krams4::Data001 = "001";
const QString Krams4::Data002 = "002";
const QString Krams4::Data010 = "010";
const QString Krams4::Data011 = "011";
const QString Krams4::Data020 = "020";
const QString Krams4::Data021 = "021";
const QString Krams4::Data022 = "022";
const QString Krams4::Data023 = "023";
const QString Krams4::Data024 = "024";
const QString Krams4::Data025 = "025";
const QString Krams4::Data030 = "030";
const QString Krams4::Data031 = "031";
const QString Krams4::Data032 = "032";
const QString Krams4::Data033 = "033";
const QString Krams4::Data034 = "034";
const QString Krams4::Data040 = "040";
const QString Krams4::Data042 = "042";
const QString Krams4::Data043 = "043";
const QString Krams4::Data050 = "050";
const QString Krams4::Data051 = "051";
const QString Krams4::Data060 = "060";
const QString Krams4::Data061 = "061";
const QString Krams4::Data100 = "100";
const QString Krams4::Data101 = "101";
const QString Krams4::Data102 = "102";
const QString Krams4::Data103 = "103";
const QString Krams4::Data104 = "104";
const QString Krams4::Data111 = "111";
const QString Krams4::Data112 = "112";
const QString Krams4::Data113 = "113";
const QString Krams4::Data121 = "121";
const QString Krams4::Data122 = "122";
const QString Krams4::Data123 = "123";
const QString Krams4::Data140 = "140";
const QString Krams4::Data141 = "141";
const QString Krams4::Data150 = "150";
const QString Krams4::Data151 = "151";
const QString Krams4::Data152 = "152";
const QString Krams4::Data160 = "160";
const QString Krams4::Data161 = "161";
const QString Krams4::Data162 = "162";
const QString Krams4::Data170 = "170";
const QString Krams4::Data171 = "171";
const QString Krams4::Data172 = "172";
const QString Krams4::Data173 = "173";
const QString Krams4::Data180 = "180";
const QString Krams4::Data181 = "181";
const QString Krams4::Data182 = "182";
const QString Krams4::Data183 = "183";
const QString Krams4::Data184 = "184";
const QString Krams4::Data185 = "185";
const QString Krams4::Data186 = "186";
const QString Krams4::Data187 = "187";
const QString Krams4::Data188 = "188";
const QString Krams4::Data190 = "190";
const QString Krams4::Data200 = "200";
const QString Krams4::Data201 = "201";
const QString Krams4::Data210 = "210";
const QString Krams4::Data220 = "220";
const QString Krams4::Data230 = "230";


Krams4::Krams4()
{
    mIterator = mData.constBegin();
}

Krams::Type Krams4::classType()
{
    return Krams::InfoMessage;
}

QString Krams4::classVersion()
{
    return "2.0";
}

Krams::Type Krams4::type() const
{
    return classType();
}

QString Krams4::version() const
{
    return classVersion();
}

void Krams4::toFront()
{
    mIterator = mData.constBegin();
}

bool Krams4::next()
{
    if ( mIterator == mData.constEnd() )
        return false;
    ++mIterator;
    return mIterator != mData.constEnd();
}

bool Krams4::next(const QString &key)
{
    if ( mIterator == mData.constEnd() )
        return false;

    for ( ++mIterator; mIterator != mData.constEnd(); ++mIterator )
    {
        if ( mIterator->first == key )
            return true;
    }
    return false;
}

QString Krams4::key() const
{
    if ( mIterator != mData.constEnd() )
        return mIterator->first;
    return QString();
}

QVariant Krams4::value() const
{
    if ( mIterator != mData.constEnd() )
        return mIterator->second;
    return QString();
}

void Krams4::append(const QString& key, const QVariant &value)
{
    if (value.isValid())
    {
        mData.append( TDataPair(key, value) );
        mIterator = mData.constBegin();
    }
}

QList<TKramsError> Krams4::parse(const QByteArray &input)
{
    QList<TKramsError> errors;

    // <SOH>'ID#hh:mm:ss#DD-MM-YYYY'<CR><LF>
    // <STX>DATA<ETX>CRC<CR><LF><EOT>

    try
    {
        // Message does not look as "info message".
        if ( input.isNull() || input[0] != 1 )
            throw TKramsError(new KramsError(KramsError::Error_InvalidInput));

        QTextStream stream(input);

        QString headerStr = stream.readLine();
        QString dataStr = stream.readLine();

        int pos = stream.pos();

        const char *dataPtr = input.constData();
        // Check that there is something to parse.
        if ( headerStr.isEmpty() || dataStr.isEmpty() || dataPtr[pos] != 4 )
            throw TKramsError(new KramsError(KramsError::Error_InvalidInput));

        parseHeader(headerStr.right(headerStr.length()-1), errors);

        // Extract CRC and data message.
        char crc = (char)dataStr.right(3).toUInt();
        QString data = dataStr.mid(1, dataStr.length()-5);

        // Check CRC
        if ( crc != makeCRC(data) )
            throw TKramsError(new KramsError(KramsError::Error_CRC));

        parseData(data, errors);

        // Set length of parsed messaged.
        mLength = pos+1;

        mIterator = mData.constBegin();
    }
    catch ( const TKramsError &err )
    {
        errors.append(err);
    }

    return errors;
}

void Krams4::parseHeader(const QString &header, QList<TKramsError> &errors)
{
    // Check protocol version.
    QStringList headerList = header.split(QChar(' '));
    if ( headerList.size() < 4 || headerList[0] != KRAMS )
        throw TKramsError(new KramsErrorMessage(KramsError::Error_InvalidInput,
                                                tr("Header does not belong to KRAMS-4 message")));

    if ( headerList[1] != VERSION )
        throw TKramsError(new KramsErrorMessage(KramsError::Error_InvalidInput,
                                                tr("Unsupported KRAMS-4 message version")));

    append(DateTime, QDateTime::fromString(QString("%1 %2").arg(headerList[2]).arg(headerList[3]),
                                           "hh:mm:ss dd-MM-yyyy"));
}

static QVariant parse_to_int(const QString &value, QList<TKramsError> &errors)
{
    Q_UNUSED(errors);
    return value.toUInt();
}

static QVariant parse_to_string(const QString &value, QList<TKramsError> &errors)
{
    Q_UNUSED(errors);
    return value;
}

static QVariant parse_010(const QString &value, QList<TKramsError> &errors)
{
    Q_UNUSED(errors);
    return (value.toDouble() / 100.);
}

static QVariant parse_011(const QString &value, QList<TKramsError> &errors)
{
    if ( value.size() < 4 )
    {
        errors.append( TKramsError(new KramsErrorMessage(KramsError::Error_InputCorrupted,
                QObject::tr("Incorrect value '%1' for '&011' parameter").arg(value))) );
        return QVariant();
    }

    Krams4::SVPPState state;
    state.isCleanDry = value == "CLRD";
    if ( !state.isCleanDry )
    {
        state.a = value[0].toAscii();
        state.b = value[1].toAscii() * 10;
        state.mm = value.right(2).toUInt();
    }
    return (value.toDouble() / 100.);
}

void Krams4::parseData(const QString &data, QList<TKramsError> &errors)
{
    QStringList dataList = data.split(QChar('&'), QString::SkipEmptyParts);

    typedef QMap<QString, parameterParser>  TParserFuncsMap;
    static TParserFuncsMap parserFuncs;
    // Initialization of parser functions.
    if ( parserFuncs.empty() )
    {
        QStringList intKeys;
        intKeys << "001"
                << "020" << "021" << "022" << "023" << "024" << "025"
                << "030" << "031" << "032" << "033" << "034"
                << "040" << "042" << "043"
                << "050" << "051"
                << "060" << "061"
                << "100" << "101" << "102" << "103" << "104"
                << "111" << "112" << "113"
                << "121" << "122" << "123"
                << "140" << "141"
                << "150" << "151" << "152"
                << "160" << "161" << "162"
                << "170" << "171" << "172" << "173"
                << "180" << "181" << "182" << "183" << "184" << "185" << "186" << "187" << "188"
                << "190"
                << "210"
                << "220"
                << "230";

        QStringList stringKeys;
        stringKeys << "002" << "200" << "201";

        parserFuncs["010"] = parse_010;
        parserFuncs["011"] = parse_011;

        foreach ( const QString &key, intKeys )
            parserFuncs[key] = parse_to_int;

        foreach ( const QString &key, stringKeys )
            parserFuncs[key] = parse_to_string;
    }

    // Parameter parsing.
    foreach ( const QString &dataItem, dataList )
    {
        // Extract key and value from string
        QString key = dataItem.left(3);
        QString value = dataItem.right(dataItem.length()-3).trimmed();

        TParserFuncsMap::const_iterator itfunc = parserFuncs.constFind(key);
        if ( itfunc != parserFuncs.constEnd() )
        {
            QVariant v = (*itfunc)(value, errors);
            if ( !v.isNull() )
                append(key, v);
        }
        else
        {
            errors.append( TKramsError(new KramsErrorMessage(
                    tr("Parser function is not defined for '&%1' parameter").arg(key))) );
        }
    }
}

char Krams4::makeCRC(const QString &data) const
{
    char crc = 0;
    QByteArray ba = data.toAscii();
    for ( int i = 0; i < ba.size(); i++ )
        crc ^= ba[i];
    return crc;
}

QList<TKramsError> Krams4::generate(QByteArray &result) const
{
    QList<TKramsError> errors;

    // <SOH>'ID#hh:mm:ss#DD-MM-YYYY'<CR><LF>
    // <STX>DATA<ETX>CRC<CR><LF><EOT>

    try
    {
        if ( mData.empty() )
            throw TKramsError(new KramsErrorMessage(KramsError::Error_InvalidInput,
                    tr("Empty data container")));

        // Header writing ...

        result += char(1);  // <SOH>

        QString header = generateHeader(errors);
        result += header.toAscii();

        result += "\r\n";   // <CR><LF>

        // Data writing ...

        result += char(2);  // <STX>

        QString data = generateData(errors);
        if ( data.isEmpty() )
            errors.append( TKramsError(new KramsErrorMessage(KramsError::Error_InvalidInput,
                            tr("Data were not defined for the message. Message is empty"))));
        result += data.toAscii();

        result += char(3);  // <ETX>

        result += QString("%1").arg(makeCRC(data), 3, 10, QChar(' ')).toAscii();

        result += "\r\n";   // <CR><LF>
        result += char(4);  // <EOT>

        // Set length of generated messaged.
        mLength = result.size();
    }
    catch ( const TKramsError &err )
    {
        errors.append(err);
    }

    return errors;
}

QString Krams4::generateHeader(QList<TKramsError> &errors) const
{
    Q_UNUSED(errors);

    if ( mData.empty() || mData.at(0).first != DateTime )
        throw TKramsError(new KramsErrorMessage(KramsError::Error_InvalidInput,
                tr("DateTime parameter is not defined as first parameter in the container")));

    QDateTime dt = mData.at(0).second.toDateTime();

    // ID#hh:mm:ss#DD-MM-YYYY
    // # - is space (32 == 0x20)
    return QString("%1 %2 %3").arg(KRAMS).arg(VERSION).arg(dt.toString("hh:mm:ss dd-MM-yyyy"));
}

static QString generate_from_int(const QVariant &value, QList<TKramsError> &errors)
{
    Q_UNUSED(errors);
    return QString("%1").arg(value.toUInt(), 6, 10, QChar(' '));
}

static QString generate_from_string(const QVariant &value, QList<TKramsError> &errors)
{
    Q_UNUSED(errors);
    return QString(" %1").arg(value.toString().trimmed());
}

static QString generate_from_002(const QVariant &value, QList<TKramsError> &errors)
{
    Q_UNUSED(errors);
    return QString("%1").arg(value.toString().trimmed(), 6, QChar(' '));
}

static QString generate_from_010(const QVariant &value, QList<TKramsError> &errors)
{
    Q_UNUSED(errors);
    return QString("%1").arg(value.toUInt()*100, 6, 10, QChar(' '));
}

static QString generate_from_011(const QVariant &value, QList<TKramsError> &errors)
{
    Q_UNUSED(errors);

    Krams4::SVPPState state = value.value<Krams4::SVPPState>();
    if ( state.isCleanDry )
        return "  CLRD";

    return QString("  %1%2%3").arg(state.a, 1).arg(state.b/10, 1).arg(state.mm, 2, 10, QChar('0'));
}

QString Krams4::generateData(QList<TKramsError> &errors) const
{
    QString data;

    typedef QMap<QString, parameterGenerator>  TGeneratorFuncsMap;
    static TGeneratorFuncsMap generatorFuncs;
    // Initialization of generator functions.
    if ( generatorFuncs.empty() )
    {
        QStringList intKeys;
        intKeys << "001"
                << "020" << "021" << "022" << "023" << "024" << "025"
                << "030" << "031" << "032" << "033" << "034"
                << "040" << "042" << "043"
                << "050" << "051"
                << "060" << "061"
                << "100" << "101" << "102" << "103" << "104"
                << "111" << "112" << "113"
                << "121" << "122" << "123"
                << "140" << "141"
                << "150" << "151" << "152"
                << "160" << "161" << "162"
                << "170" << "171" << "172" << "173"
                << "180" << "181" << "182" << "183" << "184" << "185" << "186" << "187" << "188"
                << "190"
                << "210"
                << "220"
                << "230";

        QStringList stringKeys;
        stringKeys << "200" << "201";

        generatorFuncs["002"] = generate_from_002;
        generatorFuncs["010"] = generate_from_010;
        generatorFuncs["011"] = generate_from_011;

        foreach ( const QString &key, intKeys )
            generatorFuncs[key] = generate_from_int;

        foreach ( const QString &key, stringKeys )
            generatorFuncs[key] = generate_from_string;
    }

    // Generating output string.
    foreach ( const TDataPair &dataItem, mData )
    {
        const QString &key = dataItem.first;
        const QVariant &value = dataItem.second;

        TGeneratorFuncsMap::const_iterator itfunc = generatorFuncs.constFind(key);
        if ( itfunc != generatorFuncs.constEnd() )
        {
            QString v = (*itfunc)(value, errors);
            if ( !v.isEmpty() )
                data += QString("&%1%2").arg(key).arg(v);
        }
        else
        {
            if ( key != DateTime )
                errors.append( TKramsError(new KramsErrorMessage(
                        tr("Generator function is not defined for '&%1' parameter").arg(key))) );
        }
    }

    return data;
}
