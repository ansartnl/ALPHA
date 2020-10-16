#include "asterixdata.h"
#include "asterix.h"

#include <QtCore/QDebug>
#include <QtCore/QDataStream>

const AsterixErrors AsterixData::okError = AsterixErrors::OK;

AsterixData::AsterixData(int cat, AsterixVariety var)
    : category(cat), variety(var), mIsCheckCRC(false)
{}

AsterixData::AsterixData(const AsterixData &other)
    : category(other.category), variety(other.variety), mData(other.mData)
    , mIsCheckCRC(other.mIsCheckCRC)
{}

AsterixData::~AsterixData()
{
    clearErrors();
}

int AsterixData::categoryFromArray(const QByteArray &source)
{
    if (source.isEmpty())
        return -1;

    QDataStream stream(source);
    stream.setByteOrder(QDataStream::BigEndian);

    quint8 category;
    stream >> category;

    return category;
}

bool AsterixData::contains(const QString& field) const
{
    return mData.contains(field);
}

bool AsterixData::contains(const QStringList &fields) const
{
    foreach (const QString &field, fields)
        if (!mData.contains(field))
            return false;
    return true;
}

QVariant AsterixData::value(const QString& field) const
{
    return mData.value(field);
}

void AsterixData::insert(const QString& field, const QVariant &value)
{
    if (value.isValid())
        mData.insert(field, value);
}

AsterixErrors::ErrorCode AsterixData::fromArray(const QByteArray& source, int *readed)
{
    clearErrors();
    mData.clear();
    category = 0;

    if (readed)
        *readed = 0;

    int dataSize = source.size();
    if (dataSize < 3)
        return appendError(AsterixErrors::Error_MessageIsShort);

    QDataStream stream(source);
    stream.setByteOrder(QDataStream::BigEndian);

    stream >> category;
    QScopedPointer<Asterix> parser(asterixFactory.create(AsterixKey(variety, category)));

    quint16 length;
    stream >> length;

    if (length < 4)
    {
        if (readed)
            *readed = 3;
        return appendError(AsterixErrors::Error_InputCorrupted);
    }

    if (readed)
        *readed = length;

    if (!parser)
        return appendError(AsterixErrors::Error_ProtocolNotSupported);

    // Check CRC check sum if mode is enabled.
    if (mIsCheckCRC)
    {
        if (source.size() < length+2)
            return appendError(AsterixErrors::Error_MessageIsShort);

        quint16 crc = static_cast<quint16>(*(source.constData()+length));

        if (!testCRC(crc, source, length))
            return appendError(AsterixErrors::Error_CRC);
    }

    parser->setLength(length);
    parser->parse(stream, *this);
    return firstError()->code();
}

AsterixErrors::ErrorCode AsterixData::toArray(QByteArray& output)
{
    clearErrors();

    QByteArray result;
    QScopedPointer<Asterix> generator(asterixFactory.create(AsterixKey(variety, category)));
    if (generator)
        result = generator->generate(*this);
    else
        appendError(AsterixErrors::Error_ProtocolNotSupported);

    output.clear();
    QDataStream stream(&output, QIODevice::WriteOnly);
    stream << quint8(category);
    stream << quint16(3 + result.size());
    stream.writeRawData(result.data(), result.size());

    return firstError()->code();
}

bool AsterixData::testCRC(quint16 crc, const QByteArray &data, quint16 length)
{
    if ( data.size() < length )
        return false;

    for ( int i = 0; i < data.size(); i++ )
        crc ^= data[0];

    return ( crc == 0 );
}

const QMap<QString, QVariant> &AsterixData::data() const
{
    return mData;
}

void AsterixData::setData(const QMap<QString, QVariant> &data)
{
    mData = data;
}

const AsterixErrors *AsterixData::firstError() const
{
    return mErrors.isEmpty() ? &okError : mErrors.first();
}

const AsterixErrors *AsterixData::lastError() const
{
    return mErrors.isEmpty() ? &okError : mErrors.last();
}

const QList<AsterixErrors *> &AsterixData::errors() const
{
    return mErrors;
}

void AsterixData::clearErrors()
{
    qDeleteAll(mErrors);
    mErrors.clear();
}

AsterixErrors::ErrorCode AsterixData::appendError(AsterixErrors::ErrorCode code)
{
    mErrors.append(new AsterixErrors(code));
    return code;
}

AsterixErrors::ErrorCode AsterixData::appendError(AsterixErrors *error)
{
    mErrors.append(error);
    return error->code();
}
