#include "krams4dump.h"


#include <QtCore/QStringList>
#include <QtCore/QDateTime>
#include <QtCore/QLineF>

/*!
    Вид информационного сообщения:

    <STX>METEODISPLAY<CR><LF>garbage+DATA<ETX>

    Душанбе dump:

    METEODISPLAY
    @MES_MD_RW1
    PART_CH 01 10:00:00 19-08-2011;02 086;03 266;04 09CLRD70;05 ДЕНЬ   ;06                         ;07  ;08 НЕТ;09 НЕТ;10 ВПП-1     ;11 UTDD;12 Душанбе     ;13 09R;14 27L;
    PART_RL 21     1.0;27     0.0;28     1.0;29     0.0;30     0.0;31     0.0;32     0.0;33     0.0;34   916.9;36  1006.5;37    36.8;38     7.0;39    -4.5;40     0.0;41  9999.0;42  9999.0;43  9999.0;44  9999.0;45 10000.0;46  2000.0;47 99999.0;48   270.0;49     2.0;50 99999.0;51    40.0;52 
     LAGD т    1.0;53 99999.0;54   687.8;55   917.1;56   688.9;57   918.5;58     0.2;59     1.4;60 10000.0;61 10000.0;62 10000.0;63 10000.0;64   200.0;65   330.0;66    20.0;67    60.0;68     3.0;69     1.4;70     4.0;71 99999.0;72     0.4;73     5.8;74     2.0;75 99999.0;76     2.8;77     2.7;78  6560.0;79 99999.0;80     0.0;81     0.0;82     0.0;83     0.0;84     1.0;85     1.0;86     1.0;87     1.0;88     1.0;89     1.0;90     1.0;91     1.0;92     0.0;93     0.0;94     0.0;95     0.0;96     0.0;97    f
     LAGS    READY     Ч
     LAGD 6  0.0;98     0.0;99 99999.0;
    PART_ST 88          ;
    =ю

*/

// Parser function pointer declaration
typedef QVariant (*parameterParser)(const QString &, QList<TKramsError> &);


// Data parameters
const int Krams4dump::DateTime = 1;
const int Krams4dump::Course = 2;
const int Krams4dump::VPPState = 4;
const int Krams4dump::TimeOfDay = 5;
const int Krams4dump::VPPName = 10;
const int Krams4dump::AirportId = 11;
const int Krams4dump::AirportName = 12;
const int Krams4dump::VPPidBegin = 13;
const int Krams4dump::VPPidEnd = 14;
const int Krams4dump::QNHhPa = 36;
const int Krams4dump::Temperature = 37;
const int Krams4dump::Humidity = 38;
const int Krams4dump::DewPoint = 39;
const int Krams4dump::VisBegin = 42;
const int Krams4dump::VisMiddle = 43;
const int Krams4dump::VisEnd = 44;
const int Krams4dump::CloudsLow = 46;
const int Krams4dump::WindDirectionBegin = 48;
const int Krams4dump::WindSpeedAverageBegin = 49;
const int Krams4dump::WindDirectionEnd = 51;
const int Krams4dump::WindSpeedAverageEnd = 52;
const int Krams4dump::QFEmmBegin = 54;
const int Krams4dump::QFEhPaBegin = 55;
const int Krams4dump::QFEmmEnd = 56;
const int Krams4dump::QFEhPaEnd = 57;
const int Krams4dump::VisOVIBegin = 61;
const int Krams4dump::VisOVIMiddle = 62;
const int Krams4dump::VisOVIEnd = 63;

const int Krams4dump::WindSpeedSideBegin = 201;
const int Krams4dump::WindSpeedFollowBegin = 202;
const int Krams4dump::WindSpeedOppositeBegin = 203;
const int Krams4dump::WindSpeedSideEnd = 211;
const int Krams4dump::WindSpeedFollowEnd = 212;
const int Krams4dump::WindSpeedOppositeEnd = 213;


Krams4dump::Krams4dump()
{
}

Krams::Type Krams4dump::classType()
{
    return Krams::RawMessage;
}

QString Krams4dump::classVersion()
{
    return "2.1";
}

Krams::Type Krams4dump::type() const
{
    return classType();
}

QString Krams4dump::version() const
{
    return classVersion();
}

bool Krams4dump::contains(int key) const
{
    return mValues.contains(key);
}

QVariant Krams4dump::value(int key) const
{
    return contains(key) ? mValues[key] : QVariant();
}

QList<TKramsError> Krams4dump::parse(const QByteArray &input)
{
    QList<TKramsError> errors;

    try
    {
        QTextStream stream(input);
        QString inputString = stream.readAll();

        int pos = inputString.indexOf( "METEODISPLAY" );

        // Message does not look as "raw message".
        if ( pos == -1 )
            throw TKramsError(new KramsError(KramsError::Error_InvalidInput));

        bool isETXfound = false;
        // Try to find end of message.
        for ( int i = pos+1; i < input.size(); i++ )
        {
            if ( input[i] == 0x03 )
            {
                isETXfound = true;
                break;
            }
        }

        if ( !isETXfound )
            throw TKramsError(new KramsError(KramsError::Error_MessageIsShort));

        QStringList listRx;
        // Extract key-value pairs of Krams text message according to regexp:
        // "02 xxxxx 086;"
        QRegExp rx("\\d{2,3}\\s+[^;]+;");
        int count = 0;
        while ( (pos = rx.indexIn(inputString, pos)) != -1 )
        {
            ++count;
            listRx << inputString.mid(pos, rx.matchedLength());
            pos += rx.matchedLength();
        }

        // Set length of parsed messaged.
        mLength = pos+1;

        // Remove middle garbage in from the pairs.
        for ( QStringList::iterator it = listRx.begin(); it != listRx.end(); ++it )
        {
            QString &str = *it;

            QRegExp rxStart("\\d{2,3}\\s+");
            pos = rxStart.indexIn(str);
            if ( pos >= 0 )
            {
                QString strNew = str.mid(pos, rxStart.matchedLength());

                // For message date and time info - do not apply this regexp as it will
                // remove time from the message. Parse it later.
                if ( strNew.trimmed().toInt() == 1 )
                {
                    str.remove(';');
                    continue;
                }

                QRegExp rxEnd("\\s+[^;]+");
                pos = rxEnd.lastIndexIn(str);
                if ( pos != -1 )
                    str = strNew + str.mid(pos, rxEnd.matchedLength());
            }
        }

        parseData(listRx, errors);
        // Do calculations to find side, follow and opposite wind speed.
        calculateData();
    }
    catch ( const TKramsError &err )
    {
        errors.append(err);
    }

    return errors;
}


static QVariant parse_001(const QString &value, QList<TKramsError> &errors)
{
    QDateTime dt;

    // Extract time.
    QRegExp rxs("\\d{2}:\\d{2}:\\d{2}");
    int pos = rxs.indexIn(value);
    if ( pos > -1 )
        dt.setTime( QTime::fromString(value.mid(pos, rxs.matchedLength()).trimmed(), "hh:mm:ss") );
    else
    {
        errors.append( TKramsError(new KramsErrorMessage(KramsError::Error_InputCorrupted,
                QObject::tr("Time was not specified '%1' for '01' parameter. Parsing template is 'hh:mm:ss'").arg(value))) );
        return QVariant();
    }

    // Extract date.
    QRegExp rxe("\\d{2}-\\d{2}-\\d{4}");
    pos = rxe.lastIndexIn(value);
    if ( pos > -1 )
    {
        dt.setDate( QDate::fromString(value.mid(pos, rxe.matchedLength()).trimmed(), "dd-MM-yyyy") );
        dt.setTimeSpec(Qt::UTC);
    }
    else
    {
        errors.append( TKramsError(new KramsErrorMessage(KramsError::Error_InputCorrupted,
                QObject::tr("Date was not specified '%1' for '01' parameter. Parsing template is 'dd-MM-yyyy'").arg(value))) );
        return QVariant();
    }

    return dt;
}

static QVariant parse_004(const QString &value, QList<TKramsError> &errors)
{
    if ( value.size() < 8 )
    {
        errors.append( TKramsError(new KramsErrorMessage(KramsError::Error_InputCorrupted,
                QObject::tr("State code of VPP is incorrect '%1' for '04' parameter. Parsing template is 'DDECEEBB'").arg(value))) );
        return QVariant();
    }

    QString numberStr = value.mid(0,2);

    Krams4dump::SVPPState state;
    state.isCleanDry = value.indexOf("CLRD") != -1;
    state.number = value.mid(0,2).toInt();
    state.cond = value[2] == QChar('/') ? 0xFF : value[2].toAscii();
    state.level = value[3] == QChar('/') ? 0xFF : value[3].toAscii();
    state.thickness = value.mid(4,2).toUInt();
    QString coefStr = value.right(2);
    state.coef = coefStr == QString("//") ? 0xFF : coefStr.toUInt();

    return QVariant::fromValue(state);
}

static QVariant parse_to_int(const QString &value, QList<TKramsError> &errors)
{
    Q_UNUSED(errors);
    return value.toUInt();
}

static QVariant parse_to_double(const QString &value, QList<TKramsError> &errors)
{
    Q_UNUSED(errors);
    return value.toDouble();
}

void Krams4dump::parseData(const QStringList &dataList, QList<TKramsError> &errors)
{
    typedef QMap<int, parameterParser>  TParserFuncsMap;
    static TParserFuncsMap parserFuncs;
    // Initialization of parser functions.
    if ( parserFuncs.empty() )
    {
        QList<int> intKeys;
        intKeys << 2 << 46 << 48 << 49 << 51 << 52;

        QList<double> doubleKeys;
        intKeys << 36 << 37 << 38 << 39 << 42 << 43 << 44 << 54 << 55 << 56 << 57 << 61 << 62 << 63;

        parserFuncs[1] = parse_001;
        parserFuncs[4] = parse_004;

        foreach ( int key, intKeys )
            parserFuncs[key] = parse_to_int;

        foreach ( int key, doubleKeys )
            parserFuncs[key] = parse_to_double;
    }

    // Parameter parsing.
    foreach ( const QString &dataItem, dataList )
    {
        // Extract key and value from string
        QStringList dataPair = dataItem.split(QChar(' '), QString::SkipEmptyParts);
        if ( dataPair.size() < 2 )
            continue;

        int key = dataPair[0].trimmed().toInt();
        QString value = dataPair[1].trimmed();

        // For dateTime parameter needs other processing.
        if ( key == 1 && dataPair.size() > 2 )
            value += " " + dataPair[2].trimmed();

        TParserFuncsMap::const_iterator itfunc = parserFuncs.constFind(key);
        if ( itfunc != parserFuncs.constEnd() )
        {
            QVariant v = (*itfunc)(value, errors);
            if ( !v.isNull() )
                mValues[key] = v;
        }
        else
        {
            if ( !value.isEmpty() )
                mValues[key] = value;
        }
    }
}

void Krams4dump::calculateData()
{
    // Начало ВПП
    {
        double courseCorrection = mValues[Course].toInt() - 90;

        double windSpeed = mValues[WindSpeedAverageBegin].toInt();
        double windDirectionQt = 90 + mValues[WindDirectionBegin].toInt() - 360;

        // Rotate wind vector to cartesian plane.
        QLineF line(QPointF(0., 0.), QPointF(1., 0.));
        line.setLength(windSpeed);
        line.setAngle(windDirectionQt-courseCorrection);

        double dx = line.dx(),
               dy = qAbs( line.dy() );

        if ( dx >= 0. )
            mValues[WindSpeedFollowBegin] = (int)dx;
        else
            mValues[WindSpeedOppositeBegin] = (int)-dx;

        if ( !qIsNull(dy) )
            mValues[WindSpeedSideBegin] = (int)dy;
    }

    // Конец ВПП
    {
        double courseCorrection = 180+mValues[Course].toInt() - 90;

        double windSpeed = mValues[WindSpeedAverageEnd].toInt();
        double windDirectionQt = 90 + mValues[WindDirectionEnd].toInt() - 360;

        // Rotate wind vector to cartesian plane.
        QLineF line(QPointF(0., 0.), QPointF(1., 0.));
        line.setLength(windSpeed);
        line.setAngle(windDirectionQt-courseCorrection);

        double dx = line.dx(),
               dy = qAbs( line.dy() );

        if ( dx >= 0. )
            mValues[WindSpeedFollowEnd] = (int)dx;
        else
            mValues[WindSpeedOppositeEnd] = (int)-dx;

        if ( !qIsNull(dy) )
            mValues[WindSpeedSideEnd] = (int)dy;
    }
}

QList<TKramsError> Krams4dump::generate(QByteArray &result) const
{
    QList<TKramsError> errors;

    qDebug() << tr("Not implemented yet");
    return errors;
}
